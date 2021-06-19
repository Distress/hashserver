#include "clienthandler.h"

#include <QDebug>
#include <QThread>

ClientHandler::ClientHandler(QObject *parent) :
    QObject(parent),
    m_hash(QCryptographicHash::Md5)
{
    QObject::connect(&m_socket, &QTcpSocket::disconnected,
                     this, &ClientHandler::finished);
    QObject::connect(&m_socket, &QTcpSocket::disconnected,
                     this, &ClientHandler::deleteLater);
    QObject::connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    QObject::connect(&m_socket, &QTcpSocket::readyRead,
                     this, &ClientHandler::onSocketReadyRead);
}

bool ClientHandler::setSocketDescriptor(qintptr socketDescriptor)
{
    return m_socket.setSocketDescriptor(socketDescriptor);
}

void ClientHandler::onSocketError(QAbstractSocket::SocketError socketError)
{
    qInfo() << QThread::currentThreadId() << Q_FUNC_INFO << socketError;

    m_hash.reset();
}

void ClientHandler::onSocketReadyRead()
{
    char buf[1024];
    qint64 lineLength ;
//    while ((lineLength = socket.readLine(buf, 3)) > 0) {
    while ((lineLength = m_socket.readLine(buf, sizeof(buf))) > 0) {
        if (buf[lineLength - 1] == '\n') { //особый случай если 0, обработка ошибок
            m_hash.addData(buf, lineLength - 1);
            qDebug() << QThread::currentThreadId() << Q_FUNC_INFO << m_hash.result().toHex();
            m_socket.write(m_hash.result().toHex());
            m_socket.write("\n");
            m_hash.reset();
        } else {
            m_hash.addData(buf, lineLength);
        }
    }
}
