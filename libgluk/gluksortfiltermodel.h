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
#ifndef GLUKSORTFILTERMODEL_H
#define GLUKSORTFILTERMODEL_H

#include <QSortFilterProxyModel>
#include "gluk_macros.h"

class GLUK_EXPORT GlukSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    GlukSortFilterModel(QObject *parent = 0);
    ~GlukSortFilterModel();

    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif
