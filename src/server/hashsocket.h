#pragma once

#include <QTcpSocket>
#include <QTimer>

#include <memory>

#include "xxhashwrapper.h"

class HashSocket : public QTcpSocket
{
    Q_OBJECT

public:
    explicit HashSocket(int msec = 60000,
                        QObject *parent = nullptr);

public slots:
    void setDescriptor(qintptr socketDescriptor);

signals:
    void registerConnection(QThread *thread);
    void unregisterConnection(QThread *thread);

private:
    std::unique_ptr<XxHashWrapper> m_hash;
    std::unique_ptr<QTimer> m_sessionTimer;
    int m_timeout;

private slots:
    void onSocketReadyRead();
};
