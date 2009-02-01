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

#include <KStandardDirs>
#include <KDebug>

#include <QProcess>
#include <QApplication>

PortageEngine::PortageEngine(QObject *parent) : QObject(parent), m_process(new QProcess(this)), m_currentAction(NoAction), m_done(false)
{
    connect (qApp, SIGNAL(aboutToQuit()), this, SLOT(destroy()));
    connect (m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(parseEmergeOutput()));
    connect (m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(slotFinished()));
}

PortageEngine::~PortageEngine()
{
    kDebug() << "dector";
    qDeleteAll(m_packages);
    m_packages.clear();
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

    m_process->waitForFinished();
    m_process->start(kdesu, QStringList() << "-td" << "emerge -pv --color n " + atoms.join(" "));
}

void PortageEngine::parseEmergeOutput()
{
    QString output(m_process->readAllStandardOutput());
    kDebug() << output;

    if (output.contains("done!")) {
        m_done = true;
    }
    if (!m_done) {
        return;
    }
    kDebug() << "DONE!" << output;
    emit emergeOutput(output);
}

void PortageEngine::slotFinished()
{
    m_done = false;
}
