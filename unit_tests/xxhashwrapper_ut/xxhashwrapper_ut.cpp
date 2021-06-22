#include <QString>
#include <QtTest>

#include "xxhashwrapper.h"

class XxHashWrapperUT : public QObject
{
    Q_OBJECT

public:
    XxHashWrapperUT();

private Q_SLOTS:
    void empty_string();
    void reset();
    void two_identical_strings();
    void two_different_strings();
    void byte_blocks();
};

XxHashWrapperUT::XxHashWrapperUT()
{
}

void XxHashWrapperUT::empty_string()
{
    XxHashWrapper hash;

    QCOMPARE(hash.result(), QByteArray("8ee9935d088b3fd9"));
}

void XxHashWrapperUT::reset()
{
    XxHashWrapper hash;
    hash.addData("test string", 11);
    QByteArray result = hash.result();

    hash.reset();

    QVERIFY(hash.result() != result);
}

void XxHashWrapperUT::two_identical_strings()
{
    XxHashWrapper hash1;
    hash1.addData("test string", 11);
    XxHashWrapper hash2;
    hash2.addData("test string", 11);

    QVERIFY(hash1.result() == hash2.result());
}

void XxHashWrapperUT::two_different_strings()
{
    XxHashWrapper hash1;
    hash1.addData("test string 1", 13);
    XxHashWrapper hash2;
    hash2.addData("test string 2", 13);

    QVERIFY(hash1.result() != hash2.result());
}

void XxHashWrapperUT::byte_blocks()
{
    XxHashWrapper hash1;
    hash1.addData("test string", 11);
    XxHashWrapper hash2;
    hash2.addData("test ", 5);
    hash2.addData("string", 6);

    QVERIFY(hash1.result() == hash2.result());
}

QTEST_APPLESS_MAIN(XxHashWrapperUT)

#include "xxhashwrapper_ut.moc"
