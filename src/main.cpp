#include <signal.h>

#include <QCoreApplication>
#include <QObject>
#include <QTimer>

#include "hashserver.h"

static int setup_unix_signal_handlers()
{
    struct sigaction hup, term;

    hup.sa_handler = HashServer::termSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &hup, 0))
        return 1;

    term.sa_handler = HashServer::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags = 0;
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0))
        return 2;

    return 0;
}

int main(int argc, char *argv[])
{
    qRegisterMetaType<qintptr>("qintptr");

    QCoreApplication a(argc, argv);

    HashServer server;
    QObject::connect(&server, &HashServer::stoped, &a, &QCoreApplication::quit);

    QTimer::singleShot(0, &server, &HashServer::start);

    if (setup_unix_signal_handlers() == 0)
        return a.exec();
}
