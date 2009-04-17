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
class QProcess;
class Package;

class GLUK_EXPORT PortageEngine : public QObject
{
    Q_OBJECT

public:
    static PortageEngine *instance();
    ~PortageEngine();

    void pretend(const QStringList &atoms);

    /**
     * @Returns the current packages resulting from the last
     * Portage interrogation or an empty list if no interrogation
     * occurred.
     */
    QList<Package*> packages();

private:
    enum Action { NoAction, Pretend, Install };

    static PortageEngine *m_instance;

    class Private;
    Private *d;

protected:
    PortageEngine(QObject *parent = 0);

protected slots:
    /**
     * updates references count and eventually destroys
     * the engine when no app uses it anymore.
     */
    void deRef();

    void parseEmergeOutput();
    void slotFinished();

signals:
    void emergeOutput(const QString &);
    void finished();

    /**
     * This signal is emitted whenever an error occurs.
     * While @param details might be empty (eg. QString()),
     * @param title and @param body are always valid.
     */
    void error(const QString &title, const QString &body, const QString &details);
};

#endif
