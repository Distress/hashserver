#include <QString>
#include <QtTest>
#include <QCoreApplication>

class HashServerUT : public QObject
{
    Q_OBJECT

public:
    HashServerUT();

private Q_SLOTS:
    void testCase1();
};

HashServerUT::HashServerUT()
{
}

void HashServerUT::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(HashServerUT)

#include "hashserver_ut.moc"
