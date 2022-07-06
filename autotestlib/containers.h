#ifndef CONTAINERS_H
#define CONTAINERS_H

#include <QtCore/QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
uint qHash(const std::pair<const int&,const int&>&);
uint qHash(const std::pair<const short&,const double&>&);
uint qHash(const std::pair<const float&,const double&>&);
#endif
#include <qtjambi_core/qtjambi_core_qhashes.h>

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QQueue>
#include <QtCore/QString>
#include <QtCore/QStack>
#include <QtCore/QStringList>
#include <QtCore/QSet>
#include <QtCore/QByteArray>

#include <qtjambi/qtjambi_global.h>
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
inline uint qHash(const std::pair<const float&,const double&>& pair){
    return qHash(pair, 0);
}
inline uint qHash(const std::pair<const short&,const double&>& pair){
    return qHash(pair, 0);
}
#endif

namespace ContainerTest{
    QSet<QString> valueAt(QList<QSet<QString>>& container, int position);
    void copyQListFDToJavaList(const QList<QPair<float,double>>& list, jobject results);
    size_t getQListFDHash(const QList<QPair<float,double>>& list);
    void readQListFD(QDataStream &s, QList<QPair<float,double>>& list);
    void writeQListFD(QDataStream &s, const QList<QPair<float,double>>& list);
    void copyQHashShortDoubleToJavaList(const QHash<short,double>& container, jobject results);
    size_t getQHashShortDoubleHash(const QHash<short,double>& container);
    void writeQHashShortDouble(QDataStream &s, const QHash<short,double>& container);
    void readQHashShortDouble(QDataStream &s, QHash<short,double>& container);
    void copyQSetQPointToJavaList(const QSet<QPoint>& container, jobject results);
    size_t getQSetQPointHash(const QSet<QPoint>& container);
    void writeQSetQPoint(QDataStream &s, const QSet<QPoint>& container);
    void readQSetQPoint(QDataStream &s, QSet<QPoint>& container);
    void copyQMapShortDoubleToJavaList(const QMap<short,double>& container, jobject results);
//    size_t getQMapShortDoubleHash(const QMap<short,double>& container);
    void writeQMapShortDouble(QDataStream &s, const QMap<short,double>& container);
    void readQMapShortDouble(QDataStream &s, QMap<short,double>& container);
    void copyQMultiHashShortDoubleToJavaList(const QMultiHash<short,double>& container, jobject results);
    size_t getQMultiHashShortDoubleHash(const QMultiHash<short,double>& container);
    void writeQMultiHashShortDouble(QDataStream &s, const QMultiHash<short,double>& container);
    void readQMultiHashShortDouble(QDataStream &s, QMultiHash<short,double>& container);
    void copyQMultiMapShortDoubleToJavaList(const QMultiMap<short,double>& container, jobject results);
//    size_t getQMultiMapShortDoubleHash(const QMultiMap<short,double>& container);
    void writeQMultiMapShortDouble(QDataStream &s, const QMultiMap<short,double>& container);
    void readQMultiMapShortDouble(QDataStream &s, QMultiMap<short,double>& container);
    void copyFromIterable(const QVariant& variant, jobject results);
    QList<QVariant> toQVariantList(const QVariant& variant);
    QHash<QString,QVariant> toQVariantHash(const QVariant& variant);
    QMap<QString,QVariant> toQVariantMap(const QVariant& variant);
    qsizetype containerSize(const QVariant& variant);
    QVariant associativeValue(const QVariant& variant, const QVariant& key);
    bool associativeFind(const QVariant& variant, const QVariant& key);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QVariant sequentialAt(const QVariant& variant, int index);
    QVariant sequentialRemoveFirst(QVariant variant);
    QVariant sequentialRemoveLast(QVariant variant);
    QVariant sequentialSetAt(QVariant variant, int index, const QVariant& value);
    QVariant sequentialAppend(QVariant variant, const QVariant& value);
    QVariant sequentialPrepend(QVariant variant, const QVariant& value);
    QVariant associativeRemoveKey(QVariant variant, const QVariant& key);
    QVariant associativeInsertKey(QVariant variant, const QVariant& key);
    QVariant associativeSetValue(QVariant variant, const QVariant& key, const QVariant& value);
    QPair<QVariant,QVariant> associativeFindAndReplace(QVariant variant, const QVariant& key, const QVariant& value);
#endif
}

#endif // CONTAINERS_H
