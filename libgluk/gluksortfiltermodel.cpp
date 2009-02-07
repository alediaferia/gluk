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
#include "gluksortfiltermodel.h"
#include "treeitem.h"

GlukSortFilterModel::GlukSortFilterModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

GlukSortFilterModel::~GlukSortFilterModel()
{}

bool GlukSortFilterModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if (filterRegExp().pattern().length() < 2) {
        return true;
    }

    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (item->type() == TreeItem::Category) {
        if (index.data().toString().contains(filterRegExp())) {
            return true;
        }

        for (int i = 0; i < sourceModel()->rowCount(index); i++) {
            if (sourceModel()->index(i, 0, index).data().toString().contains(filterRegExp())) {
                return true; 
            }
        }
        return false;
    }

    if (index.data().toString().contains(filterRegExp())) {
        return true;
    }

    return false;
}
