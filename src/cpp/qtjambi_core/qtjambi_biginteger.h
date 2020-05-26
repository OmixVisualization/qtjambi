#ifndef QTJAMBI_BIGINTEGER_H
#define QTJAMBI_BIGINTEGER_H

#include <QtCore/QtCore>
#include <qtjambi/qtjambi_core.h>

struct BigIntegerValue{
    quint64 value;
    bool isNegative;
    bool outOfRange;
};

BigIntegerValue fromBigInteger(JNIEnv* env, jobject object);
jobject toBigInteger(JNIEnv* env, quint64 value, bool isNegative);

#endif // QTJAMBI_BIGINTEGER_H
