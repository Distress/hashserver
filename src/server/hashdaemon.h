#pragma once

#include <QObject>

#include <QSocketNotifier>

#include "hashserver.h"

class HashDaemon : public QObject
{
    Q_OBJECT

public:
    explicit HashDaemon(QObject *parent = nullptr);

    // Unix signal handlers.
    static void termSignalHandler(int unused);

public slots:
    void start();

    // Qt signal handlers.
    void handleSigTerm();

signals:
    void stoped();

private:
    HashServer m_server;
    static int m_sigtermFd[2];

    QSocketNotifier *m_snTerm;
};
