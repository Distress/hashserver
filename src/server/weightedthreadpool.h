#pragma once

#include <QObject>

#include <QMap>
#include <QThread>

class WeightedThreadPool : public QObject
{
    Q_OBJECT

public:
    explicit WeightedThreadPool(QObject *parent = nullptr);
    ~WeightedThreadPool();

    QThread *lowLoadThread();

    int threadCount() const;

public slots: 
    void registerThreadJob(QThread *thread);
    void unregisterThreadJob(QThread *thread);

private:
    QMap<QThread*, int> m_threads;
    QMultiMap<int, QThread*> m_jobCounters;

    QThread *createThread();
};
