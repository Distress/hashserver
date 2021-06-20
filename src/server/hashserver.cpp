#include "hashserver.h"

#include <QDebug>

#include <sys/types.h>
#include <sys/socket.h>

#include "unistd.h"
#include <signal.h>

int HashServer::m_sigtermFd[2];

static void setup_unix_signal_handlers()
{
    struct sigaction hup, term;

    hup.sa_handler = HashServer::termSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &hup, 0))
        qFatal("Couldn't set SIGHUP handler: %s.", strerror(errno));

    term.sa_handler = HashServer::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags = 0;
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0))
        qFatal("Couldn't set SIGTERM handler: %s.", strerror(errno));
}

HashServer::HashServer(QObject *parent) : QObject(parent)
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, m_sigtermFd))
        qFatal("Couldn't create HUP and TERM socketpair.");

    m_snTerm = new QSocketNotifier(m_sigtermFd[1], QSocketNotifier::Read, this);
    connect(m_snTerm, &QSocketNotifier::activated, this, &HashServer::handleSigTerm);

    setup_unix_signal_handlers();
}

void HashServer::start()
{
    if (!m_server.listen(QHostAddress::Any, 50000)) {
        QByteArray ba = tr("Unable to start the server: %1.")
                .arg(m_server.errorString()).toUtf8();
        qFatal("%s", ba.constData());
    }

    qInfo() << tr("Server started at port %1.").arg(50000);
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

    qInfo() << tr("Server stoped.");
    emit stoped();

    m_snTerm->setEnabled(true);
}
