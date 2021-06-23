#include "hashserver.h"

#include <QSettings>

#include "hashsocket.h"

HashServer::HashServer(QObject *parent) : QTcpServer(parent)
{
    qRegisterMetaType<qintptr>("qintptr");
}

void HashServer::incomingConnection(qintptr socketDescriptor)
{
    QSettings settings;

    auto socket = new HashSocket(settings.value("timeout").toInt());
    auto thread = m_pool.lowLoadThread();

    socket->moveToThread(thread);

    connect(thread, &QThread::finished,
            socket, &HashSocket::deleteLater);

    connect(socket, &HashSocket::registerConnection,
            &m_pool, &WeightedThreadPool::registerThreadJob);

    connect(socket, &HashSocket::unregisterConnection,
            &m_pool, &WeightedThreadPool::unregisterThreadJob);

    QMetaObject::invokeMethod(socket, "setDescriptor",
                                Q_ARG(qintptr, socketDescriptor));
}
