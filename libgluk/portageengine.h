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
#ifndef PORTAGEENGINE_H
#define PORTAGEENGINE_H

#include "gluk_macros.h"

#include <QObject>
#include <QStringList>
#include <QHash>
class QProcess;
class Package;
class QApplication;

/**
 * @class PortageEngine
 * @brief The engine that interfaces with the emerge program.
 * @author Alessandro Diaferia
 */
class GLUK_EXPORT PortageEngine : public QObject
{
    Q_OBJECT

public:
    /**
     * @enum Action defines the current action
     */
    enum Action {
                 NoAction, /**< the engine is performing no action. */
                 Pretend,  /**< the engine is asking emerge for pretending packages. */
                 Install   /**< the engine is using emerge to install packages. */
                };

    /**
     * The instance of the engine. Use this to retrieve a pointer to the engine.
     */
    static PortageEngine *instance();
    ~PortageEngine();

    /**
     * Performs a pretend action using emerge.
     * @param atoms are the package atoms to be pretended.
     */
    void pretend(const QStringList &atoms);

    /**
     * @returns the current packages resulting from the last
     * Portage interrogation or an empty list if no interrogation
     * occurred.
     */
    QList<Package*> packages();

    /**
     * @returns the action currently performing.
     */
    Action currentAction();

private:
    static QHash<QApplication*, PortageEngine*> m_instances;

    class Private;
    Private *d;

protected:
    PortageEngine(QObject *parent = 0);

protected slots:
    /**
     * This slot is called to update references count and eventually destroy
     * the engine when no app uses it anymore.
     */
    void deRef();

    void parseEmergeOutput();
    void slotFinished();

signals:
    /**
     * This signal is emitted whenever the last action
     * performing finishes.
     */
    void finished();

    /**
     * This signal is emitted whenever an error occurs.
     * @param details might be empty,
     * @param title always valid.
     * @param body always valid.
     */
    void error(const QString &title, const QString &body, const QString &details);
};

#endif
