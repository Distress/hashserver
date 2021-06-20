#include "xxhashwrapper.h"

#include <QDebug>

#include <inttypes.h>

XxHashWrapper::XxHashWrapper()
{
    reset();
}

void XxHashWrapper::addData(const char *data, int length)
{
    hash->add(data, length);
}

void XxHashWrapper::reset()
{
    hash = std::make_unique<XXHash64>(69);
}

QByteArray XxHashWrapper::result() const
{
    QString hashsum = QString::asprintf("%" PRIx64, hash->hash());
    qInfo() << QObject::tr("Calculated hash: %1.").arg(hashsum);

    return hashsum.toUtf8();
}
