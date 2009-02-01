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
                                done(false)
    {}
    ~Private() {}

    PortageEngine *q;

    QProcess *process;
    Action currentAction;
    bool done;
    QString output;
    QList<Package*> packages;

    QString getPackageName(const QString &line);
    QStringList getUseFlags(const QString &line);
    QString getSize(const QString &line);

    void clearPackageList();
};

PortageEngine::PortageEngine(QObject *parent) : QObject(parent), d(new Private(this))
{
    connect (qApp, SIGNAL(aboutToQuit()), this, SLOT(destroy()));
    connect (d->process, SIGNAL(readyReadStandardOutput()), this, SLOT(parseEmergeOutput()));
    connect (d->process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished()));
}

PortageEngine::~PortageEngine()
{
    delete d;
}

PortageEngine* PortageEngine::m_instance = 0;

PortageEngine* PortageEngine::instance()
{
    if (!m_instance) {
        m_instance = new PortageEngine;
    }

    return m_instance;
}

void PortageEngine::destroy()
{
    m_instance->deleteLater();
}

void PortageEngine::pretend(const QStringList &atoms)
{
    d->clearPackageList();
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
    emit emergeOutput(output);
}

void PortageEngine::slotFinished()
{
    d->currentAction = NoAction;
    d->done = false;

    const QStringList lines = d->output.split("\n");

    foreach (const QString &line, lines) {
        if (!line.startsWith("[ebuild")) {
            continue;
        }
        Package *package = new Package;
        d->packages << package;

        package->setPackageName(d->getPackageName(line));
        package->setUseFlags(d->getUseFlags(line));
        package->setSize(d->getSize(line));
    }

    emit finished();
}

QList<Package*> PortageEngine::packages()
{
    return d->packages;
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

    rexp.indexIn(flags);

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
