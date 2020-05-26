/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef VARIANTS_H
#define VARIANTS_H

#include <QtCore/QtCore>
#include <QtGui/QPixmap>
#include <QtGui/QImage>
#include <QtWidgets>
#include <qtjambi_core/qtjambi_core_qhashes.h>
#include <qtjambi_gui/qtjambi_gui_qhashes.h>
#include "namespace.h"

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
    void _test4(const QLinkedList<QColor>& p){
        emit test4(p);
    }
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
signals:
    void test1(const QVector<QPair<QString, QFileInfo>>& vec);
    void test2(const QPair<QString, QQueue<QFileInfo>>& pair);
    void test3(const QQueue<QVariant>& l);
    void test4(const QLinkedList<QColor>& p);
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
};

class VariantTestObject : public QObject{
    Q_OBJECT
public:
    VariantTestObject(int value) : QObject() { setObjectName(QString("VariantTestObject%1").arg(value)); }
signals:
    void testSignal();
};

inline bool operator<(const QRectF& r1, const QRectF& r2) {return qHash(r1) < qHash(r2);}
inline bool operator<(const QColor& r1, const QColor& r2) {return r1.rgba() < r2.rgba();}

struct UnknownValue{
    inline bool operator==(const UnknownValue&) const {return true;}
    inline bool operator<(const UnknownValue&) const {return false;}
};

inline uint qHash(const UnknownValue &)
{
    return 0;
}

class Variants
{
public:
    static QObject* createInternalObject(QObject* parent = nullptr){
        return new InternalObject(parent);
    }

    QVariant pushThrough(const QVariant &variant) {
        current = variant;
        return current;
    }

    qint64 currentQInt64() const { return *(qint64 *) current.constData(); }
    quint64 currentQUInt64() const { return *(quint64 *) current.constData(); }

    qint32 currentQInt32() const { return *(qint32 *) current.constData(); }
    quint32 currentQUInt32() const { return *(quint32 *) current.constData(); }

    qint16 currentQInt16() const { return *(qint16 *) current.constData(); }
    quint16 currentQUInt16() const { return *(quint16 *) current.constData(); }

    qint8 currentQInt8() const { return *(char *) current.constData(); }
    quint8 currentQUInt8() const { return *(char *) current.constData(); }

    QChar currentQChar() const { return *(QChar *) current.constData(); }

    float currentFloat() const { return *(float *) current.constData(); }
    double currentDouble() const { return *(double *) current.constData(); }

    bool currentBool() const { return *(bool *) current.constData(); }

    QString currentString() const { return *(QString *) current.constData(); }

    QVariantList currentQVariantList() const { return *(QVariantList *) current.constData(); }
    QVariantMap currentQVariantMap() const { return *(QVariantMap *) current.constData(); }

    QPixmap currentQPixmap() const { return *(QPixmap *) current.constData(); }
    QImage currentQImage() const { return *(QImage *) current.constData(); }

    QSize currentQSize() const { return current.toSize(); }
    QSizeF currentQSizeF() const { return current.toSizeF(); }

    QPoint currentQPoint() const { return current.toPoint(); }
    QPointF currentQPointF() const { return current.toPointF(); }

    QLine currentQLine() const { return current.toLine(); }
    QLineF currentQLineF() const { return current.toLineF(); }

    QRect currentQRect() const { return current.toRect(); }
    QRectF currentQRectF() const { return current.toRectF(); }

    int currentType() const { return current.userType(); }

    bool isValid() const { return current.isValid(); }

    QString currentToString() { return current.toString(); }

    int currentToInt() { return current.toInt(); }
	
	double currentToDouble() { return current.toDouble(); }
	
	bool currentToBoolean() { return current.toBool(); }

	static QVariant getDockWidgetArea(){ return QVariant::fromValue<Qt::DockWidgetArea>(Qt::BottomDockWidgetArea); }
	
    static QVariant getDockWidgetAreas(){
        return QVariant::fromValue<QFlags<Qt::DockWidgetArea>>(Qt::DockWidgetAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea));
    }
	
	static QVariant getPageSize(){ 
		return QVariant::fromValue<QPageSize>(QPageSize(QSize(20,20))); 
	}
	
	static QVariant getSharedPointerOfWidget(){ 
		QLabel* l = new QLabel();
		l->setText("TEST_TEXT");
		return QVariant::fromValue<QSharedPointer<QWidget> >(QSharedPointer<QWidget>((QWidget*)l)); 
	}

    static QVariant getIntMap(){
        QMap<int, QString> mapping;
        mapping.insert(7, "Seven");
        mapping.insert(11, "Eleven");
        mapping.insert(42, "Forty-two");
        return QVariant::fromValue(mapping);
    }

    static QVariant getColorHash(){
        QHash<QColor, QString> mapping;
        mapping.insert(QColor(Qt::red), "red");
        mapping.insert(QColor(Qt::green), "green");
        mapping.insert(QColor(Qt::blue), "blue");
        return QVariant::fromValue(mapping);
    }

    static QVariant getIntMultiMap(){
        QMultiMap<int, QString> mapping;
        mapping.insert(7, "Seven");
        mapping.insert(7, "7");
        mapping.insert(11, "Eleven");
        mapping.insert(42, "Forty-two");
        return QVariant();
    }

    static QVariant getIntVector(){
        QVector<int> vec;
        vec << 1 << 2 << 3 << 4;
        return QVariant::fromValue(vec);
    }

    static QVariant getIntLinkedList(){
        QLinkedList<int> vec;
        vec << 1 << 2 << 3 << 4;
        return QVariant::fromValue(vec);
    }

    static QVariant getIntList(){
        QList<int> vec;
        vec << 1 << 2 << 3 << 4;
        return QVariant::fromValue(vec);
    }

    static QVariant getIntSet(){
        QSet<int> vec;
        vec << 1 << 2 << 3 << 4;
        return QVariant::fromValue(vec);
    }

    static QVariant getQObjectSet(){
        QSet<QObject*> vec;
        vec << QCoreApplication::instance() << QThread::currentThread() << QAbstractEventDispatcher::instance();
        return QVariant::fromValue(vec);
    }

    static QVariant getIntQueue(){
        QQueue<int> vec;
        vec << 1 << 2 << 3 << 4;
        return QVariant::fromValue(vec);
    }

    static QVariant getIntStack(){
        QStack<int> vec;
        vec << 1 << 2 << 3 << 4;
        return QVariant::fromValue(vec);
    }

    static QVariant getCharMultiMap(){
        QMultiMap<char,qint16> vec;
        vec.insert('a', 1);
        vec.insert('a', 2);
        vec.insert('b', 501);
        vec.insert('b', 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getCharMultiHash(){
        QMultiHash<char,qint16> vec;
        vec.insert('a', 1);
        vec.insert('a', 2);
        vec.insert('b', 501);
        vec.insert('b', 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getInt16MultiMap(){
        QMultiMap<qint16,qint64> vec;
        vec.insert(3, 1);
        vec.insert(3, 2);
        vec.insert(7, 501);
        vec.insert(7, 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getInt16MultiHash(){
        QMultiHash<qint16,qint64> vec;
        vec.insert(3, 1);
        vec.insert(3, 2);
        vec.insert(7, 501);
        vec.insert(7, 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getInt64MultiMap(){
        QMultiMap<qint64,qint64> vec;
        vec.insert(3, 1);
        vec.insert(3, 2);
        vec.insert(7, 501);
        vec.insert(7, 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getInt64MultiHash(){
        QMultiHash<qint64,qint64> vec;
        vec.insert(3, 1);
        vec.insert(3, 2);
        vec.insert(7, 501);
        vec.insert(7, 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getStringVector(){
        QVector<QString> vec;
        vec << "S1" << "S2" << "S3" << "S4";
        return QVariant::fromValue(vec);
    }

    static QVariant getStringLinkedList(){
        QLinkedList<QString> vec;
        vec << "S1" << "S2" << "S3" << "S4";
        return QVariant::fromValue(vec);
    }

    static QVariant getStringList(){
        QList<QString> vec;
        vec << "S1" << "S2" << "S3" << "S4";
        return QVariant::fromValue(vec);
    }

    static QVariant getStringSet(){
        QSet<QString> vec;
        vec << "S1" << "S2" << "S3" << "S4";
        return QVariant::fromValue(vec);
    }

    static QVariant getStringQueue(){
        QQueue<QString> vec;
        vec << "S1" << "S2" << "S3" << "S4";
        return QVariant::fromValue(vec);
    }

    static QVariant getStringStack(){
        QStack<QString> vec;
        vec << "S1" << "S2" << "S3" << "S4";
        return QVariant::fromValue(vec);
    }

    enum UnknownEnum{
        E1 = 101,E2,E3,E4
    };

    static QVariant getUnknownEnumVector(){
        QVector<UnknownEnum> vec;
        vec << E1 << E2 << E3 << E4;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumLinkedList(){
        QLinkedList<UnknownEnum> vec;
        vec << E1 << E2 << E3 << E4;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumList(){
        QList<UnknownEnum> vec;
        vec << E1 << E2 << E3 << E4;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumSet(){
        QSet<UnknownEnum> vec;
        vec << E1 << E2 << E3 << E4;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumQueue(){
        QQueue<UnknownEnum> vec;
        vec << E1 << E2 << E3 << E4;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumStack(){
        QStack<UnknownEnum> vec;
        vec << E1 << E2 << E3 << E4;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownValueVector(){
        QVector<UnknownValue> vec;
        vec << UnknownValue{};
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownValueLinkedList(){
        QLinkedList<UnknownValue> vec;
        vec << UnknownValue{};
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownValueList(){
        QList<UnknownValue> vec;
        vec << UnknownValue{};
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownValueSet(){
        QSet<UnknownValue> vec;
        vec << UnknownValue{};
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownValueQueue(){
        QQueue<UnknownValue> vec;
        vec << UnknownValue{};
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownValueStack(){
        QStack<UnknownValue> vec;
        vec << UnknownValue{};
        return QVariant::fromValue(vec);
    }

    static QVariant getDirVector(){
        QVector<QDir> vec;
        vec << QDir("/") << QDir("/home");
        return QVariant::fromValue(vec);
    }

    static QVariant getDirLinkedList(){
        QLinkedList<QDir> vec;
        vec << QDir("/") << QDir("/home");
        return QVariant::fromValue(vec);
    }

    static QVariant getDirList(){
        QList<QDir> vec;
        vec << QDir("/") << QDir("/home");
        return QVariant::fromValue(vec);
    }

    static QVariant getDirSet(){
        QSet<QDir> vec;
        vec << QDir("/") << QDir("/home");
        return QVariant::fromValue(vec);
    }

    static QVariant getDirQueue(){
        QQueue<QDir> vec;
        vec << QDir("/") << QDir("/home");
        return QVariant::fromValue(vec);
    }

    static QVariant getDirStack(){
        QStack<QDir> vec;
        vec << QDir("/") << QDir("/home");
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectVector(){
        QVector<VariantTestObject*> vec;
        vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectLinkedList(){
        QLinkedList<VariantTestObject*> vec;
        vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectList(){
        QList<VariantTestObject*> vec;
        vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectSet(){
        QSet<VariantTestObject*> vec;
        vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectQueue(){
        QQueue<VariantTestObject*> vec;
        vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectStack(){
        QStack<VariantTestObject*> vec;
        vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectMap(){
        QMap<VariantTestObject*,int> vec;
        for(int i=1; i<5; ++i){
            vec[new VariantTestObject(i)] = i;
        }
        vec[vec.keys().first()] = 100;
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectHash(){
        QHash<VariantTestObject*,int> vec;
        for(int i=1; i<5; ++i){
            vec[new VariantTestObject(i)] = i;
        }
        return QVariant::fromValue(vec);
    }

    static QVariant getStringMultiMap(){
        QMultiMap<QString,int> vec;
        for(int i=1; i<5; ++i){
            vec.insert(QString("TEST%1").arg(i), i);
            vec.insert(QString("TEST%1").arg(i), i+200);
        }
        return QVariant::fromValue(vec);
    }

    static QVariant getStringMultiHash(){
        QMultiHash<QString,int> vec;
        for(int i=1; i<5; ++i){
            vec.insert(QString("TEST%1").arg(i), i);
            vec.insert(QString("TEST%1").arg(i), i+200);
        }
        return QVariant::fromValue(vec);
    }

    static QVariant getString64MultiHash(){
        QMultiHash<QString,qint64> vec;
        for(int i=1; i<5; ++i){
            vec.insert(QString("TEST%1").arg(i), i);
            vec.insert(QString("TEST%1").arg(i), i+200);
        }
        return QVariant::fromValue(vec);
    }

    static QVariant getString16MultiHash(){
        QMultiHash<QString,qint16> vec;
        for(int i=1; i<5; ++i){
            vec.insert(QString("TEST%1").arg(i), i);
            vec.insert(QString("TEST%1").arg(i), i+200);
        }
        return QVariant::fromValue(vec);
    }

    static QVariant getString8MultiHash(){
        QMultiHash<QString,qint8> vec;
        for(int i=1; i<5; ++i){
            vec.insert(QString("TEST%1").arg(i), i);
            vec.insert(QString("TEST%1").arg(i), i+20);
        }
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectMultiMap(){
        QMultiMap<VariantTestObject*,int> vec;
        for(int i=1; i<5; ++i){
            VariantTestObject* v = new VariantTestObject(i);
            vec.insert(v, i);
            vec.insert(v, i+200);
        }
        return QVariant::fromValue(vec);
    }

    static QVariant getTestObjectMultiHash(){
        QMultiHash<VariantTestObject*,int> vec;
        for(int i=1; i<5; ++i){
            VariantTestObject* v = new VariantTestObject(i);
            vec.insert(v, i);
            vec.insert(v, i+200);
        }
        return QVariant::fromValue(vec);
    }

    static QVariant getDirHash(){
        QHash<QDir,int> vec;
        vec[QDir("/")] = 1;
        vec[QDir("/home")] = 2;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumMap(){
        QMap<UnknownEnum,int> vec;
        vec[E1] = 1;
        vec[E2] = 2;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumHash(){
        QHash<UnknownEnum,int> vec;
        vec[E1] = 1;
        vec[E2] = 2;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumMultiMap(){
        QMultiMap<UnknownEnum,int> vec;
        vec.insert(E1, 1);
        vec.insert(E2, 2);
        vec.insert(E1, 501);
        vec.insert(E2, 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownEnumMultiHash(){
        QMultiHash<UnknownEnum,int> vec;
        vec.insert(E1, 1);
        vec.insert(E2, 2);
        vec.insert(E1, 501);
        vec.insert(E2, 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getColorMultiHash(){
        QMultiHash<QColor,QString> vec;
        vec.insert(QColor(Qt::red), "A");
        vec.insert(QColor(Qt::red), "B");
        vec.insert(QColor(Qt::green), "C");
        vec.insert(QColor(Qt::green), "D");
        return QVariant::fromValue(vec);
    }

    static QVariant getColorMultiMap(){
        QMultiMap<QColor,qint16> vec;
        vec.insert(QColor(Qt::red), 1);
        vec.insert(QColor(Qt::red), 2);
        vec.insert(QColor(Qt::green), 501);
        vec.insert(QColor(Qt::green), 502);
        return QVariant::fromValue(vec);
    }

    static QVariant getRectFMultiHash(){
        QMultiHash<QRectF,QString> vec;
        vec.insert(QRectF(1,2,3,4), "A");
        vec.insert(QRectF(2,3,4,5), "B");
        vec.insert(QRectF(1,2,3,4), "C");
        vec.insert(QRectF(2,3,4,5), "D");
        return QVariant::fromValue(vec);
    }

    static QVariant getRectFMultiMap(){
        QMultiMap<QRectF,QString> vec;
        vec.insert(QRectF(1,2,3,4), "A");
        vec.insert(QRectF(2,3,4,5), "B");
        vec.insert(QRectF(1,2,3,4), "C");
        vec.insert(QRectF(2,3,4,5), "D");
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownValueMap(){
        QMap<UnknownValue,int> vec;
        vec[UnknownValue{}] = 1;
        return QVariant::fromValue(vec);
    }

    static QVariant getUnknownValueHash(){
        QHash<UnknownValue,int> vec;
        vec[UnknownValue{}] = 1;
        return QVariant::fromValue(vec);
    }

    static QVariant getDirPair(){
        QPair<QDir,QString> vec(QDir("/"), "ROOT");
        return QVariant::fromValue(vec);
    }
	
	static QVariant getSpinBox(){ return QVariant::fromValue<QSpinBox* >(new QSpinBox()); }
private:
    QVariant current;
};

Q_DECLARE_METATYPE_TEMPLATE_2ARG(QMultiHash);
Q_DECLARE_METATYPE_TEMPLATE_2ARG(QMultiMap);
Q_DECLARE_METATYPE(Variants::UnknownEnum);
Q_DECLARE_METATYPE(UnknownValue);
Q_DECLARE_METATYPE(QDir);

#endif // VARIANTS_H
