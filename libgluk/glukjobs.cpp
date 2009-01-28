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
#include "glukjobs.h"
#include "gluktreemodel.h"

#include <KDebug>

namespace GlukJobs
{

Thread::Thread(GlukJobs::Job *parent) : ThreadWeaver::Job(parent) , m_job(parent)
{
}

Thread::~Thread()
{
}

void Thread::run()
{
    m_job->doWork();
}

void Job::start()
{
    Thread *thread = new Thread(this);
    ThreadWeaver::Weaver::instance()->enqueue(thread);
}

TreeFetchJob::TreeFetchJob(GlukTreeModel *model, QObject *parent) : Job(parent), m_model(model)
{}

TreeFetchJob::~TreeFetchJob()
{}

void TreeFetchJob::doWork()
{
    kDebug() << "starting";
    m_model->loadEntries();

    emit completed();
}

};
