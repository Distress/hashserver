#pragma once

#include <QObject>

#include <QThread>

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = nullptr);
    virtual ~Worker();

public slots:
    void handleSocket(qintptr socketDescriptor);

signals:
    void clientDisconnected();
    void clientConnected(qintptr socketDescriptor);

private:
    QThread m_workerThread;
};
