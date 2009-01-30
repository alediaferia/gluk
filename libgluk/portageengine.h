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
#ifndef PORTAGEENGINE_H
#define PORTAGEENGINE_H

#include "gluk_macros.h"

#include <QObject>
#include <QStringList>
class KProcess;

class GLUK_EXPORT PortageEngine : public QObject
{
    Q_OBJECT

public:
    static PortageEngine *instance();
    ~PortageEngine();

    void pretend(const QStringList &atoms);

private:
    static PortageEngine *m_instance;
    static int m_refCount;

    static void release();
    static void destroy();

protected:
    PortageEngine(QObject *parent = 0);

protected slots:
    void emitEmergeOutput();

signals:
    void emergeOutput(const QByteArray &);
};

#endif
