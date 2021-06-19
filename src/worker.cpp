#include "worker.h"

#include <QDebug>

#include "workerengine.h"

Worker::Worker(QObject *parent) : QObject(parent)
{
    auto engine = new WorkerEngine();
    engine->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, engine, &QObject::deleteLater);

    connect(this, &Worker::clientConnected, engine, &WorkerEngine::handleSocket);
    connect(engine, &WorkerEngine::clientDisconnected, this, &Worker::clientDisconnected);

    m_workerThread.start();

    qInfo() << tr("New thread started");
}

Worker::~Worker()
{
    m_workerThread.quit();
    m_workerThread.wait();
}

void Worker::handleSocket(qintptr socketDescriptor)
{
    emit clientConnected(socketDescriptor);
}
