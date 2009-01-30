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

#include <KStandardDirs>
#include <KDebug>

#include <QProcess>

PortageEngine::PortageEngine(QObject *parent) : QObject(parent)
{}

PortageEngine::~PortageEngine()
{}

PortageEngine* PortageEngine::m_instance = 0;
int PortageEngine::m_refCount = 0;

PortageEngine* PortageEngine::instance()
{
    if (!m_instance) {
        m_instance = new PortageEngine;
    }
    m_refCount++;
    return m_instance;
}

void PortageEngine::release()
{
    m_refCount--;
    if (!m_refCount) {
        destroy();
    }
}

void PortageEngine::destroy()
{
    m_instance->deleteLater();
//     delete m_instance;
//     m_instance = 0;
}

void PortageEngine::pretend(const QStringList &atoms)
{
    const QString kdesu = KStandardDirs::findExe("kdesu");

    QProcess *pretend = new QProcess(this);

    connect (pretend, SIGNAL(readyReadStandardOutput()), this, SLOT(emitEmergeOutput()));
    connect (pretend, SIGNAL(readyReadStandardOutput()), this, SLOT(emitEmergeOutput()));

    pretend->waitForFinished();
    pretend->start(kdesu, QStringList() << "-td" << "emerge -pv --color n " + atoms.join(" "));
}

void PortageEngine::emitEmergeOutput()
{
    kDebug() << "output ready";
    QProcess *process = qobject_cast<QProcess*>(sender());
    emit emergeOutput(process->readAllStandardOutput());
}
