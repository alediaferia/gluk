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

    Package* createPackageData(const QString &line);

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
    d->clearPackageList();
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
        Package *package = d->createPackageData(line);
        d->packages << package;
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

Package* PortageEngine::Private::createPackageData(const QString &line)
{
    if (!line.startsWith("[ebuild")) {
        return 0;
    }

    Package *package = new Package;
    // the parsing..
    QRegExp rexp("\\[ebuild.......\\]");

    // the name of the package
    QString name = line;
    name.remove(rexp);
    name = name.split(" ", QString::SkipEmptyParts)[0];
    package->m_name = name;

    // the use flags of the package
    rexp = QRegExp("USE=\".*\"");
    rexp.indexIn(line);
    QString useFlags = rexp.cap();
    useFlags.remove("USE=\"");
    useFlags.remove('\"');
    package->m_useFlags = useFlags.split(" ");

    // the size of the package
    rexp = QRegExp("\\ [0-9].*\\ .*B$");
    rexp.indexIn(line);
    QString size = rexp.cap();
    if (size.startsWith("\" ")) {
        size.remove("\" ");
    }
    package->m_size = size;

    // the attributes
    rexp = QRegExp("\\[ebuild.......\\]");
    rexp.indexIn(line);
    QString attrString = rexp.cap();
    attrString = attrString.mid(7);
    attrString.remove(']');
    attrString.remove(' '); // no need for spaces
    kDebug() << attrString;

    Package::Attributes attributes;
    if (attrString.contains('N')) {
        attributes |= Package::New;
    }
    if (attrString.contains('S')) {
        attributes |= Package::Slotted;
    }
    if (attrString.contains('U')) {
        attributes |= Package::Updating;
    }
    if (attrString.contains('D')) {
        attributes |= Package::Downgrading;
    }
    if (attrString.contains('R')) {
        attributes |= Package::Reinstalling;
    }
    if (attrString.contains('F')) {
        attributes |= Package::FetchRestrictionManualDownload;
    }
    if (attrString.contains('f')) {
        attributes |= Package::FetchRestrictionAlreadyDownloaded;
    }
    if (attrString.contains('I')) {
        attributes |= Package::Interactive;
    }
    if (attrString.contains('B')) {
        attributes |= Package::BlockedUnresolved;
    }
    if (attrString.contains('b')) {
        attributes |= Package::BlockedAutomaticallyResolved;
    }
    package->m_attributes = attributes;
    kDebug() << attributes;

   return package;
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
