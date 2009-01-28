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
#ifndef GLUKJOBS_H
#define GLUKJOBS_H

#include <KJob>

#include <ThreadWeaver/Job>
#include <ThreadWeaver/Weaver>

class GlukTreeModel;

namespace GlukJobs
{

class Job : public KJob
{
    Q_OBJECT
public:
    Job(QObject *parent = 0) : KJob(parent)
    {}
    virtual ~Job() {}

    void start();
    virtual void doWork() = 0;

signals:
    void completed();
};

class TreeFetchJob : public Job
{
    Q_OBJECT
public:
    TreeFetchJob(GlukTreeModel *model, QObject *parent = 0);
    ~TreeFetchJob();

    void doWork();

private:
    GlukTreeModel *m_model;
};

class Thread : public ThreadWeaver::Job
{
    Q_OBJECT
public:
    Thread(GlukJobs::Job *);
    virtual ~Thread();

private:
    void run();

    GlukJobs::Job *m_job;
};

};

#endif
