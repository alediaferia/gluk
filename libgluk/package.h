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
#ifndef PACKAGE_H
#define PACKAGE_H

#include "gluk_macros.h"
#include <QString>
#include <QStringList>
#include <QFlags>

class GLUK_EXPORT Package
{
public:
    enum Attribute {
               Invalid = 0x0,
               New = 0x1,
               Slotted = 0x2,
               Updating = 0x4,
               Downgrading = 0x8,
               Reinstalling = 0x16,
               FetchRestrictionManualDownload = 0x32,
               FetchRestrictionAlreadyDownloaded = 0x64,
               Interactive = 0x128,
               BlockedUnresolved = 0x256,
               BlockedAutomaticallyResolved = 0x512
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    ~Package();

    /**
     * @Returns the package name.
     */
    QString packageName();

    /**
     * @Returns the use flags associated with the package.
     */
    QStringList useFlags();

    /**
     * @Returns the size of the package in the form of X,XXX kB
     */
    QString size();

    /**
     * @Returns the package attributes (New package, Reinstalling package..)
     */
    Attributes attributes();

private:
    friend class PortageEngine;
    Package() : m_attributes(Invalid) {}

    QString m_name;
    QStringList m_useFlags;
    /// in the form of "X,XXX kB"
    QString m_size;
    Attributes m_attributes;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Package::Attributes)

#endif
