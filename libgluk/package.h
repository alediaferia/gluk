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

class GLUK_EXPORT Package
{
public:
    enum Type {
               Invalid,
               New,
               Slotted,
               Updating,
               Downgrading,
               Reinstalling,
               FetchRestrictionManualDownload,
               FetchRestrictionAlreadyDownloaded,
               Interactive,
               BlockedUnresolved,
               BlockedAutomaticallyResolved
    };

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
     * @Returns the package type (New package, Reinstalling package..)
     */
    Type type();

private:
    friend class PortageEngine;
    Package() : m_type(Invalid) {}


    QString m_name;
    QStringList m_useFlags;

    /// in the form of "X,XXX kB"
    QString m_size;

    Type m_type;
};

#endif
