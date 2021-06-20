#pragma once

#include <QTcpServer>

#include "weightedthreadpool.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit TcpServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    WeightedThreadPool m_pool;
};
