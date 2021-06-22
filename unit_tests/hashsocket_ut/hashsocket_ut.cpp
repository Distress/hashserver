#include <QString>
#include <QtTest>
#include <QTcpServer>

#include "hashsocket.h"

class TestTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit TestTcpServer(HashSocket *socket) {
       m_socket = socket;
    }

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        m_socket->setDescriptor(socketDescriptor);
    }

private:
    HashSocket *m_socket;
};

class HashSocketUT : public QObject
{
    Q_OBJECT

public:
    HashSocketUT();

private Q_SLOTS:
    void socket_register_unregister_connection();
    void socket_timeout();
};

HashSocketUT::HashSocketUT()
{
    qRegisterMetaType<QThread*>("QThread*");
}

void HashSocketUT::socket_register_unregister_connection()
{
    QTcpSocket testClient;

    HashSocket sut;
    QSignalSpy spyConnected(&sut, &HashSocket::registerConnection);
    QSignalSpy spyDisconnected(&sut, &HashSocket::unregisterConnection);
    TestTcpServer server(&sut);

    if (!server.listen())
        QFAIL("Unable to start testing server.");
    testClient.connectToHost(QHostAddress::LocalHost, server.serverPort());

    QVERIFY(spyConnected.wait(1000));
    sut.close();

    QVERIFY(spyDisconnected.count() == 1);
}

void HashSocketUT::socket_timeout()
{
    QTcpSocket testClient;

    HashSocket sut(2000);
    QSignalSpy spyConnected(&sut, &HashSocket::registerConnection);
    QSignalSpy spyDisconnected(&sut, &HashSocket::unregisterConnection);
    TestTcpServer server(&sut);

    if (!server.listen())
        QFAIL("Unable to start testing server.");
    testClient.connectToHost(QHostAddress::LocalHost, server.serverPort());

    QVERIFY(spyConnected.wait(1000));
    QVERIFY(spyDisconnected.wait(3000));
}

QTEST_MAIN(HashSocketUT)

#include "hashsocket_ut.moc"
