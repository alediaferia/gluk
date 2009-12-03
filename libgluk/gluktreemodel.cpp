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
#include "ebuild.h"

#include <QDirIterator>
#include <QFileInfo>

#include <KDebug>
#include <KLocale>
#include <KIcon>

class GlukTreeModel::Private
{
public:
    Private(GlukTreeModel *q) : q(q), rootItem(0), fetchJob(0), helper(0)
    {}
    ~Private()
    {
        delete rootItem;
        delete helper;
    }

    GlukTreeModel *q;
    TreeItem *rootItem;
    GlukJobs::TreeFetchJob *fetchJob;
    SignalHelper *helper;
};

GlukTreeModel::GlukTreeModel(QObject *parent) : QAbstractItemModel(parent), d(new Private(this))
{
    d->helper = new SignalHelper(this);
    connect(d->helper, SIGNAL(fetchProgress(qreal, const QString &)), this,
            SIGNAL(fetchProgress(qreal, const QString &)));
    connect(d->helper, SIGNAL(fetchCompleted()), this, SIGNAL(fetchCompleted()));

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
        case GlukTreeModel::EbuildRole :
             return QVariant::fromValue(static_cast<TreeItem*>(index.internalPointer())->ebuild());
             break;
        case GlukTreeModel::TreeItemRole :
             return QVariant::fromValue(static_cast<TreeItem*>(index.internalPointer()));
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
    delete d->rootItem;

    d->fetchJob = new GlukJobs::TreeFetchJob(this, this);
//    connect (d->fetchJob, SIGNAL(completed()), this, SLOT(resetModel()));
    d->fetchJob->start();
//     reset();
}

void GlukTreeModel::resetModel()
{
    reset();
}

void GlukTreeModel::loadEntries()
{
    kDebug() << "loading entries";
    d->rootItem = new TreeItem("portage");

    // TODO: add code to check whether /usr/portage is the right location
    const QString portageDir = "/usr/portage";

    const int entriesCount = QDir(portageDir).entryList(QDir::AllDirs | QDir::NoDotAndDotDot).count();

    beginInsertRows(QModelIndex(), 0, entriesCount - 6);
    QDirIterator mainIt(portageDir, QDir::AllDirs | QDir::NoDotAndDotDot);

    int i = 0;
    while (mainIt.hasNext()) {
        const QString currentPath = mainIt.next();
        if (currentPath.contains("eclass") || currentPath.contains("distfiles") ||
            currentPath.contains("licenses") || currentPath.contains("scripts") || currentPath.contains("metadata")) {
            continue;
        }

        TreeItem *category = new TreeItem(mainIt.fileName(), TreeItem::Category, d->rootItem);
        d->rootItem->appendChild(category);

        QDirIterator subIt(mainIt.filePath(), QDir::AllDirs | QDir::NoDotAndDotDot);

//         int j = 0;
        while (subIt.hasNext()) {
            subIt.next();

            TreeItem *packageCat = new TreeItem(subIt.fileName(), TreeItem::Category, category);
            category->appendChild(packageCat);

            QDirIterator packageIt(subIt.filePath(), QStringList() << "*.ebuild", QDir::Files | QDir::NoDotAndDotDot);
            while (packageIt.hasNext()) {
                packageIt.next();
                TreeItem *package = new TreeItem(packageIt.fileInfo().completeBaseName(), TreeItem::Package, packageCat);
                packageCat->appendChild(package);

                Ebuild *ebuild = new Ebuild(packageIt.filePath());
                package->setEbuild(ebuild);
            }
//             j++;
        }
        d->helper->emitFetchProgress((qreal) i / entriesCount, currentPath);
        i++;
    }
    endInsertRows();
    d->helper->emitFetchCompleted();
}
