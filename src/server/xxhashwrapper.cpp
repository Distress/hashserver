#include "xxhashwrapper.h"

#include <QDebug>

#include <inttypes.h>

XxHashWrapper::XxHashWrapper()
{
    reset();
}

void XxHashWrapper::addData(const char *data, int length)
{
    m_hash->add(data, length);
}

void XxHashWrapper::reset()
{
    m_hash = std::make_unique<XXHash64>(69);
}

QByteArray XxHashWrapper::result() const
{
    QString hashsum = QString::asprintf("%" PRIx64 "\n", m_hash->hash());
    qInfo() << QObject::tr("Calculated hash: %1.").arg(hashsum);

    return hashsum.toUtf8();
}
