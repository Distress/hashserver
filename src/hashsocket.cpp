#include "hashsocket.h"

HashSocket::HashSocket(int msec, QObject *parent) :
    QTcpSocket(parent),
    m_hash(QCryptographicHash::Md5),
    m_timeout(msec)
{
    connect(&m_sessionTimer, &QTimer::timeout,
            [this]() {
        qInfo() << tr("Timeout signal received.");
        write("Connection closed due to timeout\n");
        close();
    });

    connect(this, &HashSocket::connected,
            [this]() { m_sessionTimer.start(m_timeout); });

    connect(this, &HashSocket::disconnected,
            this, &HashSocket::deleteLater);


    connect(this, &HashSocket::readyRead,
            this, &HashSocket::onSocketReadyRead);

    connect(this, static_cast<void ( HashSocket::* )(
                QAbstractSocket::SocketError )>( &QAbstractSocket::error ),
            [this]( QAbstractSocket::SocketError ) {
        qInfo() << tr("Socket error received: %1.").arg(errorString());
        m_hash.reset();
    });
}

void HashSocket::writeLine(const QByteArray& line)
{
    qInfo() << tr("Write line to socket:") << line;

    write(line);
    write("\n");
}

void HashSocket::onSocketReadyRead()
{
    m_sessionTimer.stop();

    char buf[4096];

    qint64 numRead;
    while ((numRead = readLine(buf, sizeof(buf))) > 0) {
        if (buf[numRead - 1] == '\n') {
            m_hash.addData(buf, numRead - 1);
            writeLine(m_hash.result().toHex());
            m_hash.reset();
        } else {
            m_hash.addData(buf, numRead);
        }
    }

    m_sessionTimer.start(m_timeout);
}
