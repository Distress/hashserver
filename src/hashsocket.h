#pragma once

#include <QTcpSocket>

#include <QCryptographicHash>
#include <QTimer>

class HashSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit HashSocket(int msec = 60000,
                        QObject *parent = nullptr);

private:
    QCryptographicHash m_hash;
    QTimer m_sessionTimer;
    int m_timeout;

    void writeLine(const QByteArray& line);

private slots:
    void onSocketReadyRead();
};
