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
#include <gluktreemodel.h>
#include <gluksortfiltermodel.h>
#include <ebuild.h>
#include <portageengine.h>
#include <package.h>

#include <QProgressBar>
#include <QDockWidget>
#include <QFontMetrics>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTimer>

#include <KVBox>
#include <KStatusBar>
#include <KActionCollection>
#include <KAction>
#include <KMessageBox>
#include <KLocale>
#include <KDebug>

MainWindow::MainWindow(QWidget *parent) : KXmlGuiWindow(parent), m_model(0)
{
    m_progressBar = new QProgressBar(this);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(100);
    m_progressBar->setMaximumWidth(150);
    m_progressBar->setMaximumHeight(fontMetrics().height());
    statusBar()->addPermanentWidget(m_progressBar, 0);
    m_progressBar->hide();

    KVBox *mainWidget = new KVBox(this);

    QWidget *widget = new QWidget(mainWidget);
    ui.setupUi(widget);

    setCentralWidget(mainWidget);

    QDockWidget *packageDock = new QDockWidget(this);
    pDock.setupUi(packageDock);
    addDockWidget(Qt::RightDockWidgetArea, packageDock);
    packageDock->toggleViewAction()->setIcon(KIcon("dialog-information"));
    packageDock->toggleViewAction()->setText(i18n("Package information"));
    actionCollection()->addAction("pkgdock", packageDock->toggleViewAction());

    QDockWidget *installDock = new QDockWidget(this);
    iDock.setupUi(installDock);
    addDockWidget(Qt::BottomDockWidgetArea, installDock);
    installDock->toggleViewAction()->setIcon(KIcon("drive-harddisk"));
    installDock->toggleViewAction()->setText(i18n("Current installation"));
    actionCollection()->addAction("instdock", installDock->toggleViewAction());

    QDockWidget *installationResumeDock = new QDockWidget(this);
    irDock.setupUi(installationResumeDock);
    addDockWidget(Qt::BottomDockWidgetArea, installationResumeDock);
//     installDock->toggleViewAction()->setIcon(KIcon("drive-harddisk"));
    installDock->toggleViewAction()->setText(i18n("Current installation"));
    actionCollection()->addAction("setupresume", installDock->toggleViewAction());

    m_model = new GlukTreeModel(this);
    GlukSortFilterModel *sortModel = new GlukSortFilterModel(this);
    sortModel->setSourceModel(m_model);
    ui.treeView->setModel(sortModel);
    ui.treeView->setAlternatingRowColors(true);
    ui.treeView->setSortingEnabled(true);

    m_installModel = new QStandardItemModel(this);
    irDock.resumeView->setModel(m_installModel);
    m_installModel->setHorizontalHeaderLabels(QStringList() << i18n("Package Name") << i18n("Use Flags") << i18n("Size"));

    irDock.clearButton->setIcon(KIcon("edit-clear-list"));
    irDock.deleteButton->setIcon(KIcon("list-remove"));

    connect (m_model, SIGNAL(fetchProgress(qreal)), this, SLOT(notifyFetchProgress(qreal)));
    connect (m_model, SIGNAL(fetchCompleted()), this, SLOT(slotFetchCompleted()));

    connect (ui.treeView, SIGNAL(ebuildClicked(Ebuild*)), this, SLOT(slotEbuildInfo(Ebuild*)));
    connect (ui.treeView, SIGNAL(itemDoubleClicked(QStandardItem*)), this, SLOT(addInstallItem(QStandardItem *)));
    connect (irDock.clearButton, SIGNAL(clicked()), this, SLOT(clearInstallItems()));
    connect (irDock.deleteButton, SIGNAL(clicked()), this, SLOT(removeSelectedInstallItem()));

    connect (ui.searchbox, SIGNAL(textChanged(const QString &)), sortModel, SLOT(setFilterRegExp(const QString &)));

    setupActions();
    setupGUI(QSize(600, 400));

    QTimer::singleShot(0, m_model, SLOT(reloadTree()));
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

    m_config = new KAction(this);
    m_config->setText(i18n("Check installation"));
    m_config->setIcon(KIcon("configure"));
    actionCollection()->addAction("configure", m_config);
    connect(m_config, SIGNAL(triggered(bool)), this, SLOT(configureInstallation()));

    m_install = new KAction(this);
    m_install->setText(i18n("Install"));
    m_install->setIcon(KIcon("media-playback-start"));
    actionCollection()->addAction("install", m_install);
    connect(m_install, SIGNAL(triggered(bool)), this, SLOT(configureInstallation()));
    m_install->setEnabled(false);
}

void MainWindow::notifyFetchProgress(qreal progress)
{
//     kDebug() << progress;
    m_progressBar->show();
    m_progressBar->setValue(progress*100);
}

void MainWindow::slotFetchCompleted()
{
    m_progressBar->hide();
}

void MainWindow::slotEbuildInfo(Ebuild *ebuild)
{
    pDock.nameLabel->setText(ebuild->packageName());
    pDock.descriptionLabel->setText(ebuild->description());
    pDock.useLabel->setText(ebuild->useFlags().join(" "));
    pDock.keywordsLabel->setText(ebuild->keywords().join(" "));
    pDock.licenseLabel->setText(ebuild->license());
    pDock.homepageLabel->setText("<a href=\""+ebuild->homePage().toString()+"\">"+ebuild->homePage().toString()+"</a>");
}


void MainWindow::addInstallItem(QStandardItem *item)
{
    m_installModel->appendRow(item);
}

void MainWindow::clearInstallItems()
{
    m_installModel->clear();
    m_installModel->setHorizontalHeaderLabels(QStringList() << i18n("Package Name") << i18n("Use Flags") << i18n("Size"));
}

void MainWindow::removeSelectedInstallItem()
{
    QList<QStandardItem*> rows = m_installModel->takeRow(irDock.resumeView->currentIndex().row());
    qDeleteAll(rows);
    rows.clear();
}

void MainWindow::configureInstallation()
{
    QStringList atoms;
    for (int i = 0; i < m_installModel->rowCount(); i++) {
        atoms << m_installModel->item(i)->data(GlukTreeModel::EbuildRole).value<Ebuild*>()->atomName();
    }

    connect (PortageEngine::instance(), SIGNAL(finished()), this, SLOT(showOutput()));

    connect (PortageEngine::instance(), 
             SIGNAL(error(const QString &, const QString &,
                                                    const QString &)), this,
             SLOT(handleError(const QString &, const QString &, const QString &)));

    PortageEngine::instance()->pretend(atoms);
}

void MainWindow::doInstallation()
{}

void MainWindow::showOutput()
{
    clearInstallItems();

    foreach (Package *package, PortageEngine::instance()->packages()) {
        QList<QStandardItem*> row;
        QStandardItem *packageItem = new QStandardItem(package->packageName());
        row << packageItem
            << new QStandardItem(package->size());

        int i = 0;
        foreach (const QString &use, package->useFlags()) {
            kDebug() << "FLAG:" << use;
            QStandardItem *useFlag = new QStandardItem(use);
            packageItem->setChild(i, useFlag);

            useFlag->setCheckable(true);
            useFlag->setCheckState(Qt::Checked);
            if (useFlag->text().startsWith('-')) {
                QString text = useFlag->text();
                text = text.mid(1);
                useFlag->setText(text);
                useFlag->setCheckState(Qt::Unchecked);
            }
            // TODO: set proper Qt::CheckState
            i++;
        }

        m_installModel->appendRow(row);
    }
}

void MainWindow::handleError(const QString &title, const QString &body, const QString &details)
{
    KMessageBox::detailedError(this, "<p>"+title+"</p>"+"<p>"+body+"</p>", details, i18n("Errors occurred"));
}
