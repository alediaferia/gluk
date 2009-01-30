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

#include <QStandardItem>

GlukTreeView::GlukTreeView(QWidget *parent) : QTreeView(parent)
{
    connect (this, SIGNAL(activated(const QModelIndex &)), this, SLOT(emitEbuildClicked(const QModelIndex &)));
    connect (this, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(emitItemDoubleClicked(const QModelIndex &)));
}

GlukTreeView::~GlukTreeView()
{}

void GlukTreeView::setModel(GlukSortFilterModel *model)
{
    QTreeView::setModel(model);
}

void GlukTreeView::emitEbuildClicked(const QModelIndex &index)
{
    Ebuild *ebuild = index.data(GlukTreeModel::EbuildRole).value<Ebuild*>();

    if (!ebuild) {
        return;
    }

    emit ebuildClicked(ebuild);
}

void GlukTreeView::emitItemDoubleClicked(const QModelIndex &index)
{
    if (!index.data(GlukTreeModel::EbuildRole).value<Ebuild*>()) {
        return; 
    }

    QStandardItem *item = new QStandardItem;
    item->setText(index.data().toString());
    item->setIcon(index.data(Qt::DecorationRole).value<QIcon>());
    item->setData(QVariant::fromValue(index.data(GlukTreeModel::EbuildRole).value<Ebuild*>()));

    emit itemDoubleClicked(item);
}
