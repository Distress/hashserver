#include "hashserver.h"

#include <QDebug>

#include <QThread>

#include <sys/types.h>
#include <sys/socket.h>

#include "unistd.h"

int HashServer::m_sigtermFd[2];

HashServer::HashServer(QObject *parent) : QTcpServer(parent)
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, m_sigtermFd))
          qFatal("Couldn't create HUP and TERM socketpair");

    m_snTerm = new QSocketNotifier(m_sigtermFd[1], QSocketNotifier::Read, this);
    connect(m_snTerm, &QSocketNotifier::activated, this, &HashServer::handleSigTerm);

}

void HashServer::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << QThread::currentThreadId() << Q_FUNC_INFO;
    m_pool.lowLoadWorker()->handleSocket(socketDescriptor);
}

void HashServer::start()
{
    if (!listen(QHostAddress::Any, 50000)) {
        qDebug() << QObject::tr("Unable to start the server: %1.")
                    .arg(errorString());
        //           close();
        //           return;
    }
}

void HashServer::termSignalHandler(int)
{
    char a = 1;
    ::write(m_sigtermFd[0], &a, sizeof(a));
}

void HashServer::handleSigTerm()
{
    m_snTerm->setEnabled(false);
    char tmp;
    ::read(m_sigtermFd[1], &tmp, sizeof(tmp));

    // do Qt stuff
    emit stoped();

    m_snTerm->setEnabled(true);
}
