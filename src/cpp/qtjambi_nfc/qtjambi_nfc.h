#ifndef QTJAMBI_NFC_H
#define QTJAMBI_NFC_H

#include <QtNfc/QtNfc>
#include <qtjambi/qtjambi_global.h>

inline hash_type qHash(const QNearFieldTarget::RequestId &value)
{
    hash_type hashCode = qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.refCount());
    return hashCode;
}

inline hash_type qHash(const QNdefMessage &value)
{
    hash_type hashCode = qHash(value.size());
    for(const QNdefRecord& record : value){
        hashCode = hashCode * 31 + qHash(record);
    }
    return hashCode;
}

#endif // QTJAMBI_NFC_H
