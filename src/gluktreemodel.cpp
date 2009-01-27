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
#include "gluktreemodel.h"
#include "treeitem.h"

#include <QDirIterator>
#include <KDebug>
#include <KLocale>
#include <KIcon>

class GlukTreeModel::Private
{
public:
    Private(GlukTreeModel *q) : q(q), rootItem(0)
    {}
    ~Private()
    { delete rootItem; }

    GlukTreeModel *q;
    TreeItem *rootItem;
    void loadEntries();
};

GlukTreeModel::GlukTreeModel(QObject *parent) : QAbstractItemModel(parent), d(new Private(this))
{
}

GlukTreeModel::~GlukTreeModel()
{
    delete d;
}

int GlukTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return 1;
}

int GlukTreeModel::rowCount(const QModelIndex &parent) const
{
    if (!d->rootItem) {
        return 0;
    }

    if (!parent.isValid()) {
        return d->rootItem->childCount();
    }

    return static_cast<TreeItem*>(parent.internalPointer())->childCount();
}

QVariant GlukTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return i18n("Portage Item");
    }

    return QVariant();
}

QVariant GlukTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    switch (role) {
        case Qt::DisplayRole :
            return static_cast<TreeItem*>(index.internalPointer())->name();
            break;
        case Qt::DecorationRole :
             if (item->type() == TreeItem::Category) {
                 return KIcon("folder-tar");
             } else {
                 return KIcon("package-x-generic");
             }
             break;
        case GlukTreeModel::Versions :
             return static_cast<TreeItem*>(index.internalPointer())->availableVersions();
             break;
         default : ;
    }

    return QVariant();
}

QModelIndex GlukTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row > rowCount(parent) || column > columnCount(parent)) {
        return QModelIndex();
    }

    TreeItem *parentItem = d->rootItem;
    if (parent.isValid()) {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->packageAt(row);
    if (!childItem) {
        return QModelIndex();
    }

    return createIndex(row, column, childItem);
}

QModelIndex GlukTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (item->parent() != d->rootItem) {
        return createIndex(item->row(), 0, item->parent());
    }

    return QModelIndex();
}

void GlukTreeModel::reloadTree()
{
    d->loadEntries();
    reset();
}

void GlukTreeModel::Private::loadEntries()
{
    rootItem = new TreeItem("portage");

    // TODO: add code to check whether /usr/portage is the right location
    const QString portageDir = "/usr/portage";

    QDirIterator mainIt(portageDir, QDir::AllDirs | QDir::NoDotAndDotDot);

    int i = 0;
    while (mainIt.hasNext()) {
        const QString currentPath = mainIt.next();
        if (currentPath.contains("eclass") || currentPath.contains("distfiles") ||
            currentPath.contains("licenses") || currentPath.contains("scripts")) {
            continue;
        }

//         q->beginInsertRows(QModelIndex(), i, i);
        TreeItem *category = new TreeItem(mainIt.fileName(), TreeItem::Category, rootItem);
        rootItem->appendChild(category);
//         q->endInsertRows();

        QDirIterator subIt(mainIt.filePath(), QDir::AllDirs | QDir::NoDotAndDotDot);

        int j = 0;
        while (subIt.hasNext()) {
            subIt.next();

//             q->beginInsertRows(q->index(i, 0, QModelIndex()), j, j);
            TreeItem *package = new TreeItem(subIt.fileName(), TreeItem::Package, category);
            category->appendChild(package);
//             q->endInsertRows();
            // TODO: set versions...
            j++;
        }
        
        i++;
    }
}