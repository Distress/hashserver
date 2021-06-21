#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include <signal.h>

#include "hashdaemon.h"

class HashDaemonUT : public QObject
{
    Q_OBJECT

public:
    HashDaemonUT();

private Q_SLOTS:
    void handle_sigterm();
    void handle_sighup();
};

HashDaemonUT::HashDaemonUT()
{
}

void HashDaemonUT::handle_sigterm()
{
    HashDaemon server;
    QSignalSpy spy(&server, &HashDaemon::stoped);

    raise(SIGTERM);
    QTest::qWait(1000);

    QVERIFY(spy.count() == 1);
}

void HashDaemonUT::handle_sighup()
{
    HashDaemon server;
    QSignalSpy spy(&server, &HashDaemon::stoped);

    raise(SIGHUP);
    QTest::qWait(1000);

    QVERIFY(spy.count() == 1);
}

QTEST_GUILESS_MAIN(HashDaemonUT)

#include "hashdaemon_ut.moc"
