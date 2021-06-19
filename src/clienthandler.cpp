#include "clienthandler.h"

#include <QDebug>

ClientHandler::ClientHandler(QObject *parent) :
    QObject(parent),
    m_hash(QCryptographicHash::Md5)
{
    connect(&m_sessionTimer, &QTimer::timeout, this, &ClientHandler::onTimeout);

    connect(&m_socket, &QTcpSocket::disconnected,
            this, &ClientHandler::finished);
    connect(&m_socket, &QTcpSocket::disconnected,
            this, &ClientHandler::deleteLater);
    connect(&m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(&m_socket, &QTcpSocket::readyRead,
            this, &ClientHandler::onSocketReadyRead);
}

bool ClientHandler::setSocketDescriptor(qintptr socketDescriptor)
{
    if (!m_socket.setSocketDescriptor(socketDescriptor))
        return false;

    m_sessionTimer.start(600000);
    return true;
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
    m_sessionTimer.stop();

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

    m_sessionTimer.start();
}

void ClientHandler::writeLine(const QByteArray& line)
{
    qInfo() << tr("Write line to socket:") << line;

    m_socket.write(line);
    m_socket.write("\n");
}

void ClientHandler::onTimeout()
{
    qInfo() << tr("Timeout signal received.");

    writeLine("Connection closed due to timeout");
    m_socket.close();
}
