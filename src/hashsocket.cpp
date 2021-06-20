#include "hashsocket.h"

HashSocket::HashSocket(int msec, QObject *parent) :
    QTcpSocket(parent),
    m_hash(QCryptographicHash::Md5),
    m_timeout(msec)
{
    connect(&m_sessionTimer, &QTimer::timeout,
            this, &HashSocket::onTimeout);

    connect(this, &QAbstractSocket::connected, this,
            &HashSocket::startTimeoutTimer);
    connect(this, &QAbstractSocket::disconnected,
            this, &HashSocket::deleteLater);

    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    connect(this, &QTcpSocket::readyRead,
            this, &HashSocket::onSocketReadyRead);
}

void HashSocket::startTimeoutTimer()
{
    m_sessionTimer.start(m_timeout);
}

void HashSocket::writeLine(const QByteArray& line)
{
    qInfo() << tr("Write line to socket:") << line;

    write(line);
    write("\n");
}

void HashSocket::onSocketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError)

    qInfo() << tr("Socket error received: %1.").arg(errorString());

    m_hash.reset();
}

void HashSocket::onSocketReadyRead()
{
    m_sessionTimer.stop();

    char buf[4096];

    qint64 numRead;
    while ((numRead = readLine(buf, sizeof(buf))) > 0) {
        if (buf[numRead - 1] == '\n') {
            if (numRead != 1) //if '\n' is not the only character
                m_hash.addData(buf, numRead - 1);

            writeLine(m_hash.result().toHex());
            m_hash.reset();
        } else {
            m_hash.addData(buf, numRead);
        }
    }

    startTimeoutTimer();
}

void HashSocket::onTimeout()
{
    qInfo() << tr("Timeout signal received.");

    writeLine("Connection closed due to timeout");
    close();
}
