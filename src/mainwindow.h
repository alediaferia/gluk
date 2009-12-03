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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include "ui_main.h"
#include "ui_packagedock.h"
#include "ui_installdock.h"
#include "ui_instresumedock.h"

class GlukTreeModel;
class KProgressDialog;
class Ebuild;
class QStandardItemModel;
class QStandardItem;
class KAction;
class QByteArray;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupActions();

protected slots:
    void notifyFetchProgress(qreal, const QString &);
    void slotFetchCompleted();
    void slotEbuildInfo(Ebuild *);
    void addInstallItem(TreeItem *);
    void clearInstallItems();
    void removeSelectedInstallItem();
    void configureInstallation();
    void doInstallation();
    void showOutput();
    void handleError(const QString&, const QString&, const QString&);

private:
    Ui::MainWidget ui;
    Ui::DockWidget pDock;
    Ui::InstallResumeDock irDock;

    GlukTreeModel *m_model;
    KProgressDialog *m_progressDialog;
    QStandardItemModel *m_installModel;

    KAction *m_config;
    KAction *m_install;
};

#endif
