#include <cstring>
#include <signal.h>

#include <QCoreApplication>

#include <QDateTime>
#include <QObject>
#include <QTimer>

#include "hashserver.h"

static void myMessageOutput(QtMsgType type,
                            const QMessageLogContext &context,
                            const QString &msg)
{
    QString desc = QDateTime::currentDateTime().toString();

    desc += QString(" [%1]").arg(reinterpret_cast<quint64>(QThread::currentThreadId()));

    switch (type) {
    case QtDebugMsg:
        desc += " Debug   : ";
        break;
    case QtWarningMsg:
        desc += " Warning : ";
        break;
    case QtCriticalMsg:
        desc += " Critical: ";
        break;
    case QtFatalMsg:
        desc += " Fatal   : ";
        break;
    default:
        desc += " Info    : ";
    }

    desc += msg;
    if (context.file)
        desc += QString(" (%1:%2, %3)")
                .arg(context.file).arg(context.line).arg(context.function).toUtf8();

    QByteArray localMsg(desc.toUtf8());
    fprintf(stderr, "%s\n", localMsg.constData());
}

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

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    qRegisterMetaType<qintptr>("qintptr");

    QCoreApplication a(argc, argv);

    HashServer server;
    QObject::connect(&server, &HashServer::stoped, &a, &QCoreApplication::quit);

    QTimer::singleShot(0, &server, &HashServer::start);

    setup_unix_signal_handlers();

    return a.exec();
}
