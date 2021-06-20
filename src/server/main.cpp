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

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);

    QCoreApplication a(argc, argv);

    HashServer server;
    QObject::connect(&server, &HashServer::stoped, &a, &QCoreApplication::quit);

    QTimer::singleShot(0, &server, &HashServer::start);

    return a.exec();
}
