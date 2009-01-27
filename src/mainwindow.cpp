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
#include "mainwindow.h"
#include "gluktreemodel.h"
#include "gluksortfiltermodel.h"

#include <KVBox>
#include <KActionCollection>
#include <KAction>
#include <KLocale>

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent), m_model(0)
{
    KVBox *mainWidget = new KVBox(this);

    QWidget *widget = new QWidget(mainWidget);
    ui.setupUi(widget);

    setCentralWidget(mainWidget);

    m_model = new GlukTreeModel(this);
    GlukSortFilterModel *sortModel = new GlukSortFilterModel(this);
    sortModel->setSourceModel(m_model);
    ui.treeView->setModel(sortModel);
    ui.treeView->setAlternatingRowColors(true);
    ui.treeView->setSortingEnabled(true);

    connect (ui.searchbox, SIGNAL(textChanged(const QString &)), sortModel, SLOT(setFilterRegExp(const QString &)));

    setupActions();
    setupGUI(QSize(600, 400));

    //m_model->reloadTree();
}

MainWindow::~MainWindow()
{}

void MainWindow::setupActions()
{
    KAction *refresh = new KAction(this); 
    refresh->setText(i18n("Refresh Tree"));
    refresh->setIcon(KIcon("view-refresh"));
    actionCollection()->addAction("refresh", refresh);

    connect(refresh, SIGNAL(triggered(bool)), m_model, SLOT(reloadTree()));
}

