#pragma once

#include <QTcpSocket>
#include <QTimer>

#include "xxhashwrapper.h"

class HashSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit HashSocket(int msec = 60000,
                        QObject *parent = nullptr);

private:
    XxHashWrapper m_hash;
    QTimer m_sessionTimer;
    int m_timeout;

private slots:
    void onSocketReadyRead();
};
