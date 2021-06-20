#include "tcpserver.h"

#include "hashsocket.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    qRegisterMetaType<qintptr>("qintptr");
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
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
