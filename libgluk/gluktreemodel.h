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

#ifndef GLUKTREEMODEL_H
#define GLUKTREEMODEL_H

#include <QAbstractItemModel>
#include "glukjobs.h"
#include "gluk_macros.h"

class GLUK_EXPORT GlukTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    enum EbuildData { EbuildRole = 33, TreeItemRole = 34 };
    GlukTreeModel(QObject *parent = 0);
    ~GlukTreeModel();

    int columnCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount (const QModelIndex &parent = QModelIndex()) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

public slots:
    void reloadTree();

protected slots:
    void resetModel();

signals:
    void fetchProgress(qreal, const QString &scanningDir);
    void fetchCompleted();

private:
    class Private;
    Private *d;

    friend class GlukJobs::TreeFetchJob;

    void loadEntries();
//     QStringList getUseFlags(const QString &file);
};

class SignalHelper : public QObject
{
    Q_OBJECT

private:
    friend class GlukTreeModel;

    SignalHelper(QObject *parent = 0) : QObject(parent)
    {}
    ~SignalHelper()
    {}

    void emitFetchProgress(qreal progress, const QString &scanningDir)
    { emit fetchProgress(progress, scanningDir); }

    void emitFetchCompleted()
    { emit fetchCompleted(); }

signals:
    void fetchProgress(qreal, const QString &);
    void fetchCompleted();
};

#endif
