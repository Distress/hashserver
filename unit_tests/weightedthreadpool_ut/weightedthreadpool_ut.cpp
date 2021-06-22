#include <QString>
#include <QtTest>

#include "weightedthreadpool.h"

class WeightedThreadPoolUT : public QObject
{
    Q_OBJECT

public:
    WeightedThreadPoolUT();

private Q_SLOTS:
    void default_value();
    void max_created_threads();
    void using_idle_thread();
    void selecting_lowest_loaded_thread();
};

WeightedThreadPoolUT::WeightedThreadPoolUT()
{
}

void WeightedThreadPoolUT::default_value()
{
    WeightedThreadPool wtp;

    QVERIFY(wtp.threadCount() == 0);
}

void WeightedThreadPoolUT::max_created_threads()
{
    WeightedThreadPool wtp;
    for (int i = 0; i < QThread::idealThreadCount() * 2; ++i) {
        wtp.registerThreadJob(wtp.lowLoadThread());
    }

    QVERIFY(wtp.threadCount() <= QThread::idealThreadCount());
}

void WeightedThreadPoolUT::using_idle_thread()
{
    if (QThread::idealThreadCount() < 3)
        QSKIP("This test requires more than 2 cores");

    WeightedThreadPool wtp;
    wtp.lowLoadThread();
    wtp.lowLoadThread();

    QVERIFY(wtp.threadCount() == 1);
}

void WeightedThreadPoolUT::selecting_lowest_loaded_thread()
{
    if (QThread::idealThreadCount() < 3)
        QSKIP("This test requires more than 2 cores");

    WeightedThreadPool wtp;
    auto oneJobThread = wtp.lowLoadThread();
    wtp.registerThreadJob(oneJobThread);

    for (int i = 1; i < QThread::idealThreadCount() - 1; ++i) {
        auto twoJobsThread = wtp.lowLoadThread();
        wtp.registerThreadJob(twoJobsThread);
        wtp.registerThreadJob(twoJobsThread);
    }

    QVERIFY(wtp.lowLoadThread() == oneJobThread);
}

QTEST_APPLESS_MAIN(WeightedThreadPoolUT)

#include "weightedthreadpool_ut.moc"
