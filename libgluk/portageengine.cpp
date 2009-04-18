/***************************************************************************
 *   Copyright (C) 2009 Alessandro Diaferia <alediaferia@gmail.com>        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/
#include "portageengine.h"
#include "package.h"

#include <QProcess>
#include <QApplication>

#include <KStandardDirs>
#include <KDebug>

class PortageEngine::Private
{
public:
    Private(PortageEngine *q) : q(q),
                                process(new QProcess(q)),
                                currentAction(PortageEngine::NoAction),
                                done(false),
                                refs(0)
    {}
    ~Private() {}

    PortageEngine *q;

    QProcess *process;
    Action currentAction;
    bool done;
    QString output;
    QList<Package*> packages;

    QList<QApplication*> connectedApps;
    int refs;

    QString errorTitle;
    QString errorBody;
    QString errorDetails;

    QString getPackageName(const QString &line);
    QStringList getUseFlags(const QString &line);
    QString getSize(const QString &line);

    void clearPackageList();

    /// looks for errors
    bool checkError();
};

PortageEngine::PortageEngine(QObject *parent) : QObject(parent), d(new Private(this))
{
    connect (d->process, SIGNAL(readyReadStandardOutput()), this, SLOT(parseEmergeOutput()));
    connect (d->process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished()));
}

PortageEngine::~PortageEngine()
{
    kDebug() << "engine dying";
    delete d;
}

PortageEngine* PortageEngine::m_instance = 0;

PortageEngine* PortageEngine::instance()
{
    if (!m_instance) {
        m_instance = new PortageEngine;
    }

    if (!m_instance->d->connectedApps.contains(qApp)) {
        m_instance->d->connectedApps << qApp;
        m_instance->d->refs++;
        connect (qApp, SIGNAL(aboutToQuit()), m_instance, SLOT(deRef()));
    }

    return m_instance;
}

void PortageEngine::deRef()
{
    d->refs--;
    kDebug() << d->refs;
    if (!d->refs) {
        m_instance->deleteLater();
    }
}

void PortageEngine::pretend(const QStringList &atoms)
{
    d->clearPackageList();
    d->output.clear();
    d->currentAction = Pretend;

    const QString kdesu = KStandardDirs::findExe("kdesu");

    d->process->waitForFinished();
    d->process->start(kdesu, QStringList() << "-td" << "emerge -pv --color n " + atoms.join(" "));
}

void PortageEngine::parseEmergeOutput()
{
    QString output(d->process->readAllStandardOutput());

    if (output.contains("done!")) {
        d->done = true;
    }

    if (!d->done) {
        return;
    }

    d->output.append(output);
}

void PortageEngine::slotFinished()
{
    d->currentAction = NoAction;
    d->done = false;

    // error check
    if (d->checkError()) {
        emit error(d->errorTitle, d->errorBody, d->errorDetails);
        d->errorBody.clear();
        d->errorTitle.clear();
        d->errorDetails.clear();

        emit finished();
        return;
    }
    

    const QStringList lines = d->output.split('\n');

    foreach (const QString &line, lines) {
        if (!line.startsWith("[ebuild")) {
            continue;
        }
        kDebug() << line;
        Package *package = new Package;
        d->packages << package;

        package->m_name = d->getPackageName(line);
        package->m_useFlags = d->getUseFlags(line);
        package->m_size = d->getSize(line);
    }

    d->errorBody.clear();
    d->errorTitle.clear();
    d->errorDetails.clear();

    emit finished();
}

QList<Package*> PortageEngine::packages()
{
    return d->packages;
}

PortageEngine::Action PortageEngine::currentAction()
{
    return d->currentAction;
}

QString PortageEngine::Private::getPackageName(const QString &line)
{
    if (!line.startsWith("[ebuild")) {
        return QString();
    }

    QString name = line;
    name.remove(QRegExp("\\[ebuild.......\\]"));

    QStringList pieces = name.split(" ", QString::SkipEmptyParts);

    return pieces[0];
}

QStringList PortageEngine::Private::getUseFlags(const QString &line)
{
    if (!line.startsWith("[ebuild")) {
        return QStringList();
    }

    QString flags = line;
    QRegExp rexp("USE=\".*\"");

    int index = rexp.indexIn(flags);
    if (index == -1) { // no match found
        return QStringList();
    }

    QString match = rexp.cap();
    kDebug() << match;

    match.remove("USE=\"");
    match.remove("\"");

    return match.split(" ");
}

QString PortageEngine::Private::getSize(const QString &line)
{
    if (!line.startsWith("[ebuild")) {
        return QString();
    }

    QString size = line;
    QRegExp rexp("\\ [0-9].*\\ .*B$");

    rexp.indexIn(size);

    QString match = rexp.cap();
    kDebug() << match;

    if (match.startsWith("\" ")) {
        match.remove("\" ");
    }

    return match;
}

void PortageEngine::Private::clearPackageList()
{
    qDeleteAll(packages);
    packages.clear();
}

bool PortageEngine::Private::checkError()
{
    if (!output.contains("!!!")) {
        return false;
    }

    const QStringList lines = output.split('\n');
    int i = 0;

    foreach (const QString &line, lines) {
        if (line.contains("done!")) {
            continue;
        }
        if (i == 0) {
            errorTitle = line;
            errorTitle.remove("!!!");
            i++; 
            continue;
        }
        if (i == 1 || i == 2) {
            errorBody.append(line);
            i++;
            continue;
        }
        errorDetails.append(line);
        i++;
    }

    return true;
}
