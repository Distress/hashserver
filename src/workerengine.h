#ifndef WORKERENGINE_H
#define WORKERENGINE_H

#include <QObject>

class WorkerEngine : public QObject
{
    Q_OBJECT

public:
    explicit WorkerEngine(QObject *parent = nullptr);

public slots:
    void handleSocket(qintptr socketDescriptor);

signals:
    void clientDisconnected();
};

#endif // WORKER_H
