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
#include "treeitem.h"
#include "ebuild.h"

#include <QList>
#include <QStringList>

#include <KDebug>

class TreeItem::Private
{
public:
    Private() : parent(0), ebuild(0)
    {}
    ~Private() {}

    QStringList useFlags;
    TreeItem::Type type;
    TreeItem *parent;
    QList<TreeItem*> children;
    QString name;
    Ebuild* ebuild;
};

TreeItem::TreeItem() : d(new Private())
{}

TreeItem::TreeItem(const QString &name, TreeItem::Type type, TreeItem *parent) : d(new Private())
{
    d->type = type;
    setName(name);

//     if (type == TreeItem::Package) { // NOTE: not sure if this is needed
        d->parent = parent;
//     }
}

TreeItem::~TreeItem()
{
    qDeleteAll(d->children);
    d->children.clear();
    delete d->ebuild;
    delete d;
}

void TreeItem::appendChild(TreeItem *child)
{
    d->children << child;
}

void TreeItem::setName(const QString &name)
{
    d->name = name;
}

TreeItem* TreeItem::parent()
{
    return d->parent;
}

TreeItem::Type TreeItem::type() const
{
    return d->type;
}

int TreeItem::childCount() const
{
    return d->children.count();
}

TreeItem* TreeItem::packageAt(int i)
{
    if (d->type == TreeItem::Package) {
        return 0;
    }

    if (i >= d->children.count()) {
        return 0;
    }

    return d->children[i];
}

TreeItem *TreeItem::package(const QString &name)
{
    if (d->type == TreeItem::Package) {
        return 0;
    }

    foreach (TreeItem *item, d->children) {
        if (item->name() == name) {
            return item;
        }
    }

    return 0;
}

void TreeItem::setUseFlags(const QStringList &flags)
{
    d->useFlags = flags;
}

QStringList TreeItem::useFlags() const
{
    return d->useFlags;
}

int TreeItem::row() const
{
    if (!d->parent) {
        return 0;
    }

    return d->parent->d->children.indexOf(const_cast<TreeItem*>(this));
}

QString TreeItem::name() const
{
    return d->name;
}

void TreeItem::setEbuild( Ebuild *ebuild )
{
    d->ebuild = ebuild;
}

Ebuild* TreeItem::ebuild()
{
//     kDebug() << "item ebuild" << d->ebuild;
    return d->ebuild;
}
