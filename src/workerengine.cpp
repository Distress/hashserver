#include "workerengine.h"

#include <QDebug>

WorkerEngine::WorkerEngine(QObject *parent) : QObject(parent)
{
}

void WorkerEngine::handleSocket(qintptr socketDescriptor)
{
    auto handler = new ClientHandler(this);

    connect(handler, &ClientHandler::finished,
            this, &WorkerEngine::clientDisconnected);

    if (!handler->setSocketDescriptor(socketDescriptor)) {
        qCritical() << tr("Unable to set socket descriptor: %1.")
                       .arg(handler->lastErrorString());
        handler->deleteLater();
    }
}
