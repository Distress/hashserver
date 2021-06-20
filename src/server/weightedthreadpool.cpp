#include "weightedthreadpool.h"

#include <QDebug>

WeightedThreadPool::WeightedThreadPool(QObject *parent) : QObject(parent)
{
    if (QThread::idealThreadCount() == 1) {
        m_threads.insert(QThread::currentThread(), 0);
        m_jobCounters.insert(0, QThread::currentThread());
    }
}

WeightedThreadPool::~WeightedThreadPool()
{
    foreach (auto thread, m_threads.keys()) {
        if (thread != QThread::currentThread()) {
            thread->quit();
            thread->wait();
            thread->deleteLater();
        }
    }
}

QThread* WeightedThreadPool::lowLoadThread()
{
    if (m_jobCounters.empty()
            || (m_jobCounters.size() < (QThread::idealThreadCount() - 1)
                && m_jobCounters.begin().key() > 0))
        return createThread();

    return m_jobCounters.begin().value();
}

int WeightedThreadPool::threadCount() const
{
    return m_threads.size();
}

void WeightedThreadPool::registerThreadJob(QThread *thread)
{
    int newJobCounter = ++m_threads[thread];
    m_jobCounters.remove((newJobCounter - 1), thread);
    m_jobCounters.insert(newJobCounter, thread);
}

void WeightedThreadPool::unregisterThreadJob(QThread *thread)
{
    int newJobCounter = --m_threads[thread];
    m_jobCounters.remove((newJobCounter + 1), thread);
    m_jobCounters.insert(newJobCounter, thread);
}

QThread *WeightedThreadPool::createThread()
{
    auto thread = new QThread();
    m_threads.insert(thread, 0);
    m_jobCounters.insert(0, thread);
    thread->start();
    return thread;
}
