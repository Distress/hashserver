#pragma once

#include <QObject>

#include <QSocketNotifier>

#include "tcpserver.h"

class HashServer : public QObject
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

private:
    TcpServer m_server;
    static int m_sigtermFd[2];

    QSocketNotifier *m_snTerm;
};
