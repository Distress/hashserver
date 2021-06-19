#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>

#include <QCryptographicHash>
#include <QTcpSocket>
#include <QTimer>

class ClientHandler : public QObject
{
    Q_OBJECT

public:
    explicit ClientHandler(QObject *parent = nullptr);

    bool setSocketDescriptor(qintptr socketDescriptor);
    QString lastErrorString();

signals:
    void finished();

private:
    void writeLine(const QByteArray& line);

    QCryptographicHash m_hash;
    QTcpSocket m_socket;
    QTimer m_sessionTimer;

private slots:
    void onSocketError(QAbstractSocket::SocketError onSocketError);
    void onSocketReadyRead();

    void onTimeout();
};

#endif // CLIENTHANDLER_H




