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
#include "gluktreeview.h"

#include "ebuild.h"
#include "treeitem.h"
#include "gluksortfiltermodel.h"
#include "gluktreemodel.h"

#include <KDebug>

GlukTreeView::GlukTreeView(QWidget *parent) : QTreeView(parent)
{
    connect (this, SIGNAL(activated(const QModelIndex &)), this, SLOT(emitEbuildClicked(const QModelIndex &)));
}

GlukTreeView::~GlukTreeView()
{}

void GlukTreeView::setModel(GlukSortFilterModel *model)
{
    QTreeView::setModel(model);
}

void GlukTreeView::emitEbuildClicked(const QModelIndex &index)
{
//     TreeItem *item = static_cast<//static_cast<TreeItem*>(index.internalPointer());
// 
//     kDebug() << item;
//     if (item->type() == TreeItem::Category) {
//         return;
//     }

    kDebug() << index.data(GlukTreeModel::EbuildRole).value<Ebuild*>();
//     GlukSortFilterModel *sortFilterModel = static_cast<GlukSortFilterModel*>(model());
//     GlukTreeModel *model = static_cast<GlukTreeModel*>(sortFilterModel->sourceModel());
// 
//     Ebuild *ebuild = model->data(index, GlukTreeModel::EbuildRole).value<Ebuild*>();
// 
//     kDebug() << ebuild;
//     if (!ebuild) {
//         return;
//     }
// 
//     emit ebuildClicked(ebuild);
}
