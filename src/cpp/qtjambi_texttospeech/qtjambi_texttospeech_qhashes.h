#ifndef QTJAMBI_TEXTTOSPEECH_QHASHES_H
#define QTJAMBI_TEXTTOSPEECH_QHASHES_H

#include <QtTextToSpeech/QtTextToSpeech>
#include <qtjambi/qtjambi_global.h>

#if !defined(QT_JAMBI_RUN) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline bool operator==(const QVoice& lhs, const QVoice& rhs)
{
    return const_cast<QVoice&>(lhs) == rhs;
}
#endif

inline hash_type qHash(const QVoice &value)
{
    hash_type hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.age());
    hashCode = hashCode * 31 + qHash(value.gender());
    return hashCode;
}

#endif // QTJAMBI_TEXTTOSPEECH_QHASHES_H
