#include "workerengine.h"

#include <QDebug>

#include "hashsocket.h"

WorkerEngine::WorkerEngine(QObject *parent) : QObject(parent)
{
}

void WorkerEngine::handleSocket(qintptr socketDescriptor)
{
    auto socket = new HashSocket(60000, this);

    connect(socket, &QAbstractSocket::disconnected,
            this, &WorkerEngine::clientDisconnected);

    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qCritical() << tr("Unable to set socket descriptor: %1.")
                       .arg(socket->errorString());
        socket->deleteLater();
    }
}
