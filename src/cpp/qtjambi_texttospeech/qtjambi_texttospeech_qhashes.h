#ifndef QTJAMBI_TEXTTOSPEECH_QHASHES_H
#define QTJAMBI_TEXTTOSPEECH_QHASHES_H

#include <QtTextToSpeech/QtTextToSpeech>

inline uint qHash(const QVoice &value)
{
    uint hashCode = qHash(value.name());
    hashCode = hashCode * 31 + qHash(value.age());
    hashCode = hashCode * 31 + qHash(value.gender());
    return hashCode;
}

#endif // QTJAMBI_TEXTTOSPEECH_QHASHES_H
