#pragma once

#include <QObject>

#include <QMap>

#include "worker.h"

class WorkerPool : public QObject
{
    Q_OBJECT

public:
    explicit WorkerPool(QObject *parent = nullptr);

    Worker* lowLoadWorker();

private:
    Worker* createWorker();

    QMap<Worker*, int> m_workers;
    QMultiMap<int, Worker*> m_clientCounters;

private slots:
    void onClientDisconnected();
};
