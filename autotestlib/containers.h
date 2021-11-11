#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
uint qHash(const std::pair<const int&,const int&>&);
#endif

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QStack>
#include <QtCore/QStringList>
#include <QtCore/QSet>
#include <QtCore/QByteArray>

#include <qtjambi/qtjambi_typetests.h>

class QHash_int : public QHash<int, int> { };
class QList_int : public QList<int> { };
class QList_String : public QList<QString> { };
class QMap_int : public QMap<int, int> { };
class QMap_int_String : public QMap<int, QString> { };
class QMap_String_int : public QMap<QString, int> { };
class QMap_String : public QMap<QString, QString> { };
class QQueue_int : public QQueue<int> { };
class QQueue_String : public QQueue<QString> { };
class QSet_int : public QSet<int> { };
class QSet_String : public QSet<QString> { };
class QStack_int : public QStack<int> { };
class QStack_String : public QStack<QString> { };
class StringList : public QStringList { };
class ByteArrayList : public QByteArrayList { };
class QMultiMap_int : public QMultiMap<int, int> { };
class QMultiMap_String_int : public QMultiMap<QString, int> { };
class QMultiMap_int_String : public QMultiMap<QString, int> { };
class QMultiMap_String : public QMultiMap<QString, QString> { };
class QMultiHash_int : public QMultiHash<int, int> { };
class QMultiHash_String_int : public QMultiHash<QString, int> { };
class QMultiHash_int_String : public QMultiHash<QString, int> { };
class QMultiHash_String : public QMultiHash<QString, QString> { };

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
inline uint qHash(const std::pair<const int&,const int&>& pair){
    return qHash(pair, 0);
}
#endif

#endif // CONTAINERS_H
