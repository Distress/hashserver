#pragma once

#include <QObject>

//#include <QHash>
//#include <QMap>
#include <unordered_map>
#include <map>

#include <QThread>

class WeightedThreadPool : public QObject
{
    Q_OBJECT

public:
    explicit WeightedThreadPool(QObject *parent = nullptr);
    ~WeightedThreadPool();
    WeightedThreadPool(const WeightedThreadPool &) = default;
    WeightedThreadPool &operator=(const WeightedThreadPool &) = default;

    QThread *lowLoadThread();

    int threadCount() const;

public slots: 
    void registerThreadJob(QThread *thread);
    void unregisterThreadJob(QThread *thread);

private:
    std::unordered_map<QThread*, int> m_threads;
    std::multimap<int, QThread*> m_jobCounters;

    QThread *createThread();
};
