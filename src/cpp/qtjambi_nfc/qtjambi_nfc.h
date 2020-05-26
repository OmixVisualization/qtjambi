#ifndef QTJAMBI_NFC_H
#define QTJAMBI_NFC_H

#include <QtNfc/QtNfc>

inline uint qHash(const QNearFieldTarget::RequestId &value)
{
    uint hashCode = qHash(value.isValid());
    hashCode = hashCode * 31 + qHash(value.refCount());
    return hashCode;
}

inline uint qHash(const QNdefMessage &value)
{
    uint hashCode = qHash(value.size());
    for(const QNdefRecord& record : value){
        hashCode = hashCode * 31 + qHash(record);
    }
    return hashCode;
}

#endif // QTJAMBI_NFC_H
