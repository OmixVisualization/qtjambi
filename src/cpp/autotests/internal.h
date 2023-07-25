#ifndef INTERNAL_H
#define INTERNAL_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QtCore>
#include <QtGui/QtGui>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif
#ifndef QTJAMBI_NO_SQL
#include <QtSql/QtSql>
#endif
#ifndef QTJAMBI_NO_XML
#include <QtXml/QtXml>
#endif
#ifndef QTJAMBI_NO_NETWORK
#include <QtNetwork/QtNetwork>
#endif
#include <QtJambi/QtJambiAPI>
#include <QtJambi/javaapi.h>
#include <QtJambiCore/hashes.h>

enum class UnknownEnum{
    E1 = 101,E2,E3,E4
};

inline hash_type qHash(const UnknownEnum & e, hash_type seed = 0)
{
    return qHash(*reinterpret_cast<const qint32*>(&e), seed);
}
Q_DECLARE_METATYPE(UnknownEnum);

struct UnknownValue{
    inline bool operator==(const UnknownValue&) const {return true;}
    inline bool operator<(const UnknownValue&) const {return false;}
};

inline hash_type qHash(const UnknownValue &, hash_type = 0)
{
    return 0;
}
Q_DECLARE_METATYPE(UnknownValue);

class InternalObject : public QObject{
    Q_OBJECT
public:
    InternalObject(QObject* parent) : QObject(parent){}
public slots:
    void _test1(const QVector<QPair<QString, QFileInfo>>& vec){
        emit test1(vec);
    }
    void _test2(const QPair<QString, QQueue<QFileInfo>>& p){
        emit test2(p);
    }
    void _test3(const QQueue<QVariant>& p){
        emit test3(p);
    }
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    void _test4(const QLinkedList<QColor>& p){
        emit test4(p);
    }
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
    void _test5(const QSet<QPoint>& p){
        emit test5(p);
    }
    void _test6(const QSet<QObject*>& p){
        emit test6(p);
    }
    void _test7(const QSet<QComboBox*>& p){
        emit test7(p);
    }
    void _test8(const QStack<int>& i){
        emit test8(i);
    }
    void _test9(const QList<QStack<int>>& i){
        emit test9(i);
    }
    void _test10(const QMap<QString,QFileInfo>& i){
        emit test10(i);
    }
    void _test11(const QVector<QStack<int>>& i){
        emit test11(i);
    }
    void _test12(const QMap<qint8,QStack<int>>& i){
        emit test12(i);
    }
    void _test13(const QHash<qint8,QStack<int>>& i){
        emit test13(i);
    }
    void _test14(const QMultiHash<QUrlTwoFlags<QUrl::UrlFormattingOption,QUrl::ComponentFormattingOption>,QStack<int>>& i){
        emit test14(i);
    }
    void _test15(const QStack<QQueue<int>>& i){
        emit test15(i);
    }
    void _test16(const QVector<QPair<QString, QObject*>>& vec){
        emit test16(vec);
    }
#ifndef QTJAMBI_NO_WIDGETS
    void _test17(const QStack<QGraphicsItem*>& p){
        emit test17(p);
    }
#endif
    void _test18(const QMultiMap<QUrlTwoFlags<QUrl::UrlFormattingOption,QUrl::ComponentFormattingOption>,QQueue<int>>& i){
        emit test18(i);
    }
signals:
    void test1(const QVector<QPair<QString, QFileInfo>>& vec);
    void test2(const QPair<QString, QQueue<QFileInfo>>& pair);
    void test3(const QQueue<QVariant>& l);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    void test4(const QLinkedList<QColor>& p);
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
    void test5(const QSet<QPoint>& l);
    void test6(const QSet<QObject*>& l);
    void test7(const QSet<QComboBox*>& l);
    void test8(const QStack<int>& i);
    void test9(const QList<QStack<int>>& i);
    void test10(const QMap<QString,QFileInfo>& i);
    void test11(const QVector<QStack<int>>& i);
    void test12(const QMap<qint8,QStack<int>>& i);
    void test13(const QHash<qint8,QStack<int>>& i);
    void test14(const QMultiHash<QUrlTwoFlags<QUrl::UrlFormattingOption,QUrl::ComponentFormattingOption>,QStack<int>>& i);
    void test15(const QStack<QQueue<int>>& i);
    void test16(const QVector<QPair<QString, QObject*>>& vec);
    void test17(const QStack<QGraphicsItem*>& p);
    void test18(const QMultiMap<QUrlTwoFlags<QUrl::UrlFormattingOption,QUrl::ComponentFormattingOption>,QQueue<int>>& i);
};

class VariantTestObject : public QObject{
    Q_OBJECT
public:
    VariantTestObject(int value) : QObject() { setObjectName(QString("VariantTestObject%1").arg(value)); }
signals:
    void testSignal();
};
Q_DECLARE_METATYPE(VariantTestObject*);

#endif // INTERNAL_H
