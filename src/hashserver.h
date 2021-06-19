#ifndef HASHSERVER_H
#define HASHSERVER_H

#include <QTcpServer>

#include <QSocketNotifier>

#include "workerpool.h"

class HashServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit HashServer(QObject *parent = nullptr);

    // Unix signal handlers.
    static void termSignalHandler(int unused);

public slots:
    void start();

    // Qt signal handlers.
    void handleSigTerm();

signals:
    void stoped();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    WorkerPool m_pool;
    static int m_sigtermFd[2];

    QSocketNotifier *m_snTerm;
};

#endif // HASHSERVER_H
