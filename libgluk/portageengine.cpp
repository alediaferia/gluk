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
};

PortageEngine::PortageEngine(QObject *parent) : QObject(parent), d(new Private(this))
{
    connect (qApp, SIGNAL(aboutToQuit()), this, SLOT(destroy()));
    connect (d->process, SIGNAL(readyReadStandardOutput()), this, SLOT(parseEmergeOutput()));
    connect (d->process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished()));
}

PortageEngine::~PortageEngine()
{
    qDeleteAll(d->packages);
    d->packages.clear();

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
    d->done = false;
    const QStringList lines = d->output.split("\n");

    foreach (const QString &line, lines) {
        if (!line.startsWith("[ebuild")) {
            continue;
        }
        Package *package = new Package;
        d->packages << package;
    }
}
