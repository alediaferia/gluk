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
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QString>
#include <QVariant>
#include "gluk_macros.h"

class Ebuild;

class GLUK_EXPORT TreeItem
{
public:
    enum Type { Category, Package };

    TreeItem();
    /**
     * Defines an item of the portage tree. An item can be
     * a Category, e.g. media-gfx, or a Package, e.g. amarok.
     * Package items can have a parent (Category), Category items cannot.
     */
    TreeItem(const QString &name, Type = Category, TreeItem *parent = 0);
    ~TreeItem();

    int childCount() const;
    Type type() const;

    /**
     * @Returns the name either of the Category or of the Package.
     */
    QString name() const;

    void setName(const QString &name);
    TreeItem* parent();

    /**
     * @Returns the Package at the specified order.
     * @Note that Packages don't have a predefined order.
     * @Note that this method will always return 0 when Type is set to Package.
     */
    TreeItem *packageAt(int count);

    /**
     * @Returns the TreeItem of type Package with the given name.
     * If no packages are found under this category 0 is returned.
     * @Note that this method will return always 0 when Type is set to Package.
     */
    TreeItem *package(const QString &name);

    /**
     * Sets the use flags of the Package.
     * This method has no effect when the item is of type Category.
     */
    void setUseFlags(const QStringList &);

    /**
     * @Returns the use flags of the Package or QStringList if
     * Category is set as type.
     */
    QStringList useFlags() const;

    void setEbuild( Ebuild* );
    Ebuild* ebuild();

    /// internal use.
    int row() const;
    void appendChild(TreeItem *child);

private:
    class Private;
    Private *d;
};

Q_DECLARE_METATYPE(TreeItem*);

#endif
