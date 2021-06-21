#pragma once

#include <QString>

#include <memory>
#include <xxhash/xxhash64.h>

class XxHashWrapper
{
public:
    explicit XxHashWrapper();

    void addData(const char *data, int length);
    void reset();
    QByteArray result() const;

private:
    std::unique_ptr<XXHash64> m_hash;
};
