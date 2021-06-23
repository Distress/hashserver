#include "weightedthreadpool.h"

#include <QDebug>

WeightedThreadPool::WeightedThreadPool(QObject *parent) : QObject(parent)
{
}

WeightedThreadPool::~WeightedThreadPool()
{
    for (auto thread : m_threads) {
        if (thread.first != QThread::currentThread()) {
            thread.first->quit();
            thread.first->wait();
            thread.first->deleteLater();
        }
    }
}

QThread* WeightedThreadPool::lowLoadThread()
{
    if (m_jobCounters.empty()
            || (m_jobCounters.size() < (QThread::idealThreadCount() - 1)
                && m_jobCounters.begin()->first > 0))
        return createThread();

    return m_jobCounters.begin()->second;
}

int WeightedThreadPool::threadCount() const
{
    return m_threads.size();
}

void WeightedThreadPool::registerThreadJob(QThread *thread)
{
    int newJobCounter = ++m_threads[thread];

    auto iterPair = m_jobCounters.equal_range(newJobCounter - 1);

    for (auto it = iterPair.first; it != iterPair.second; ++it) {
        if (it->second == thread) {
            m_jobCounters.erase(it);
            break;
        }
    }

    m_jobCounters.insert({newJobCounter, thread});
}

void WeightedThreadPool::unregisterThreadJob(QThread *thread)
{
    int newJobCounter = --m_threads[thread];

    auto iterPair = m_jobCounters.equal_range(newJobCounter + 1);

    for (auto it = iterPair.first; it != iterPair.second; ++it) {
        if (it->second == thread) {
            m_jobCounters.erase(it);
            break;
        }
    }

    m_jobCounters.insert({newJobCounter, thread});
}

QThread *WeightedThreadPool::createThread()
{
    auto thread = QThread::currentThread();

    if (QThread::idealThreadCount() > 1) {
        thread = new QThread();
        thread->start();
    }

    m_threads.insert({thread, 0});
    m_jobCounters.insert({0, thread});

    return thread;
}
