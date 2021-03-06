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
#ifndef GLUKTREEVIEW_H
#define GLUKTREEVIEW_H
#include "gluk_macros.h"

#include <QTreeView>
class GlukSortFilterModel;
class Ebuild;
class TreeItem;

class GLUK_EXPORT GlukTreeView : public QTreeView
{
    Q_OBJECT
public:
    GlukTreeView(QWidget *parent = 0);
    ~GlukTreeView();

    void setModel(GlukSortFilterModel *model);

signals:
    void ebuildClicked(Ebuild*);
    void itemDoubleClicked(TreeItem *);

protected slots:
    void emitEbuildClicked(const QModelIndex &index);
    void emitItemDoubleClicked(const QModelIndex &index);
};

#endif
