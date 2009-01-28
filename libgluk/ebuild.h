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
#ifndef EBUILD_H
#define EBUILD_H
#include "gluk_macros.h"

#include <QMetaType>

#include <QFile>
class QUrl;

class GLUK_EXPORT Ebuild : public QFile
{
    Q_OBJECT
public:
    Ebuild(const QString &name, QObject *parent = 0);
    ~Ebuild();

    QStringList useFlags();
    QString description();
    QString license();
    bool isMasked();
    QUrl sourceUrl();
    QUrl homePage();
    QStringList keywords();
    bool isValid();

    /**
     * Searches for key @param key in the ebuild and returns
     * the value as QString or an empty QString if none is found.
     * @param key must be the exact ebuild key.
     * e.g. IUSE.
     */
    QString value(const QString &key);
};

Q_DECLARE_METATYPE(Ebuild*)

#endif
