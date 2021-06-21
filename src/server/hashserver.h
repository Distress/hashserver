#pragma once

#include <QTcpServer>

#include "weightedthreadpool.h"

class HashServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit HashServer(QObject *parent = nullptr);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    WeightedThreadPool m_pool;
};
