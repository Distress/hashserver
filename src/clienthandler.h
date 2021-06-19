#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>

#include <QCryptographicHash>
#include <QTcpSocket>

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

    QTcpSocket m_socket;
    QCryptographicHash m_hash;

private slots:
    void onSocketError(QAbstractSocket::SocketError onSocketError);
    void onSocketReadyRead();
};

#endif // CLIENTHANDLER_H




