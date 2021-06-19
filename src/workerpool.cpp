#include "workerpool.h"

#include <QDebug>

#include <QThread>

WorkerPool::WorkerPool(QObject *parent) : QObject(parent)
{
}

Worker* WorkerPool::createWorker()
{
    auto worker = new Worker(this);
    connect(worker, &Worker::clientDisconnected,
            this, &WorkerPool::onClientDisconnected);

    m_workers.insert(worker, 1);
    m_clientCounters.insert(1, worker);

    return worker;
}

Worker* WorkerPool::lowLoadWorker()
{
    if (m_clientCounters.empty()
            || (m_clientCounters.size() < QThread::idealThreadCount()
             && m_clientCounters.begin().key() > 0))
        return createWorker();

    auto i = m_clientCounters.begin();
    auto worker = i.value();
    m_clientCounters.erase(i);
    m_clientCounters.insert(++m_workers[worker], worker);

    return worker;
}

void WorkerPool::onClientDisconnected()
{
    auto worker = qobject_cast<Worker*>(sender());
    if (worker) {
        int newClientCounter = --m_workers[worker];
        m_clientCounters.remove((newClientCounter + 1), worker);
        m_clientCounters.insert(newClientCounter, worker);
    }
}
