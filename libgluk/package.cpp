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
#include "package.h"

Package::Package()
{}

Package::~Package()
{}

void Package::setPackageName(const QString &name)
{
    m_name = name;
}

void Package::setUseFlags(const QStringList &flags)
{
    m_useFlags = flags;
}

void Package::setSize(const QString &size)
{
    m_size = size;
}

QString Package::packageName()
{
    return m_name;
}

QString Package::size()
{
    return m_size;
}

QStringList Package::useFlags()
{
    return m_useFlags;
}