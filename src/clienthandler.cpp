#include "clienthandler.h"

#include <QDebug>

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

QString ClientHandler::lastErrorString()
{
    return m_socket.errorString();
}

void ClientHandler::onSocketError(QAbstractSocket::SocketError socketError)
{
    qInfo() << tr("Socket error received: %1.").arg(socketError);

    m_hash.reset();
}

void ClientHandler::onSocketReadyRead()
{
    char buf[4096];

    qint64 numRead;
    while ((numRead = m_socket.readLine(buf, sizeof(buf))) > 0) {
        if (buf[numRead - 1] == '\n') {
            if (numRead != 1) //if '\n' is not the only character
                m_hash.addData(buf, numRead - 1);

            writeLine(m_hash.result().toHex());
            m_hash.reset();
        } else {
            m_hash.addData(buf, numRead);
        }
    }
}

void ClientHandler::writeLine(const QByteArray& line)
{
    qInfo() << tr("Write line to socket:") << line;

    m_socket.write(line);
    m_socket.write("\n");
}
