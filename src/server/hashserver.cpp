#include "hashserver.h"

#include "hashsocket.h"

HashServer::HashServer(QObject *parent) : QTcpServer(parent)
{
    qRegisterMetaType<qintptr>("qintptr");
}

void HashServer::incomingConnection(qintptr socketDescriptor)
{
    auto socket = new HashSocket(60000);
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
