#include "workerengine.h"

#include <QDebug>
#include <QThread>

WorkerEngine::WorkerEngine(QObject *parent) : QObject(parent)
{
}

void WorkerEngine::handleSocket(qintptr socketDescriptor)
{
    auto handler = new ClientHandler(this);

    connect(handler, &ClientHandler::finished,
            this, &WorkerEngine::clientDisconnected);

    if (!handler->setSocketDescriptor(socketDescriptor))
        handler->deleteLater();

}
