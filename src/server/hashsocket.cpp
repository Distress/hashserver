#include "hashsocket.h"

HashSocket::HashSocket(int msec, QObject *parent) :
    QTcpSocket(parent),
    m_timeout(msec)
{
}

void HashSocket::setDescriptor(qintptr socketDescriptor)
{
    m_hash = std::make_unique<XxHashWrapper>();
    m_sessionTimer = std::make_unique<QTimer>();

    connect(m_sessionTimer.get(), &QTimer::timeout,
            [this]() {
        qInfo() << tr("Timeout signal received.");
        write("\nConnection closed due to timeout\n");
        close();
    });

    connect(this, &HashSocket::disconnected,
            [this]( ) {
        emit unregisterConnection(this->thread());
        deleteLater();
    });

    connect(this, &HashSocket::readyRead,
            this, &HashSocket::onSocketReadyRead);

    connect(this, &HashSocket::stateChanged,
            [this]( QAbstractSocket::SocketState socketState ) {
        if (socketState == QAbstractSocket::ConnectedState) {
            emit registerConnection(this->thread());
            m_sessionTimer->start(m_timeout);
        }
    });

    connect(this, static_cast<void ( HashSocket::* )(
                QAbstractSocket::SocketError )>( &QAbstractSocket::error ),
            [this]( QAbstractSocket::SocketError ) {
        qInfo() << tr("Socket error received: %1.").arg(errorString());
        m_hash.get()->reset();
    });

    if (!setSocketDescriptor(socketDescriptor)) {
        qCritical() << tr("Unable to set socket descriptor: %1.")
                       .arg(errorString());
        deleteLater();
    }
}

void HashSocket::onSocketReadyRead()
{
    m_sessionTimer->stop();

    char buf[4096];

    qint64 numRead;
    while ((numRead = readLine(buf, sizeof(buf))) > 0) {
        if (buf[numRead - 1] == '\n') {
            m_hash->addData(buf, numRead - 1);
            write(m_hash->result());
            m_hash.get()->reset();
        } else {
            m_hash->addData(buf, numRead);
        }
    }

    m_sessionTimer->start(m_timeout);
}
