#ifndef QTJAMBI_WEBSOCKETS_HASHES_H
#define QTJAMBI_WEBSOCKETS_HASHES_H

#include <qtjambi/qtjambi_global.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 4, 0)
#include <QtWebSockets/QWebSocketHandshakeOptions>

#if defined(QT_JAMBI_RUN)
hash_type qHash(const QWebSocketHandshakeOptions& value, hash_type seed = 0);
#else
class QWebSocketHandshakeOptionsPrivate{
public:
    static hash_type hash(const QWebSocketHandshakeOptions& value, hash_type seed){
        return qHash(quintptr(value.d.data()), seed);
    }
};

inline hash_type qHash(const QWebSocketHandshakeOptions& value, hash_type seed = 0)
{
    return QWebSocketHandshakeOptionsPrivate::hash(value, seed);
}
#endif
#endif

#endif // QTJAMBI_WEBSOCKETS_HASHES_H
