/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "variants.h"
#include "internal.h"

Q_DECLARE_METATYPE_TEMPLATE_2ARG(QMultiHash);
Q_DECLARE_METATYPE_TEMPLATE_2ARG(QMultiMap);
Q_DECLARE_METATYPE(QDir);
Q_DECLARE_METATYPE(QRunnable*);
Q_DECLARE_METATYPE(QPaintDevice*);
Q_DECLARE_METATYPE(QEvent*);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
static const bool initialized = []()->bool{
        qRegisterMetaType<UnknownEnum>("UnknownEnum");
        qRegisterMetaType<UnknownValue>("UnknownValue");
        qRegisterMetaType<VariantTestObject*>("VariantTestObject*");
        return true;
}();
#endif

inline bool operator<(const QRectF& r1, const QRectF& r2) {return qHash(r1) < qHash(r2);}

QObject* Variants::createInternalObject(QObject* parent){
    return new InternalObject(parent);
}

QVariant Variants::pushThrough(const QVariant &variant) {
    current = variant;
    return current;
}

qint64 Variants::currentQInt64() const { return *reinterpret_cast<const qint64 *>(current.constData()); }
quint64 Variants::currentQUInt64() const { return *reinterpret_cast<const quint64 *>(current.constData()); }

qint32 Variants::currentQInt32() const { return *reinterpret_cast<const qint32 *>(current.constData()); }
quint32 Variants::currentQUInt32() const { return *reinterpret_cast<const quint32 *>(current.constData()); }

qint16 Variants::currentQInt16() const { return *reinterpret_cast<const qint16 *>(current.constData()); }
quint16 Variants::currentQUInt16() const { return *reinterpret_cast<const quint16 *>(current.constData()); }

qint8 Variants::currentQInt8() const { return *reinterpret_cast<const char *>(current.constData()); }
quint8 Variants::currentQUInt8() const { return *reinterpret_cast<const uchar *>(current.constData()); }

QChar Variants::currentQChar() const { return *reinterpret_cast<const QChar *>(current.constData()); }

float Variants::currentFloat() const { return *reinterpret_cast<const float *>(current.constData()); }
double Variants::currentDouble() const { return *reinterpret_cast<const double *>(current.constData()); }

bool Variants::currentBool() const { return *reinterpret_cast<const bool *>(current.constData()); }

QString Variants::currentString() const { return *reinterpret_cast<const QString *>(current.constData()); }

QVariantList Variants::currentQVariantList() const { return *reinterpret_cast<const QVariantList *>(current.constData()); }
QVariantMap Variants::currentQVariantMap() const { return *reinterpret_cast<const QVariantMap *>(current.constData()); }

QPixmap Variants::currentQPixmap() const { return *reinterpret_cast<const QPixmap *>(current.constData()); }
QImage Variants::currentQImage() const { return *reinterpret_cast<const QImage *>(current.constData()); }

QSize Variants::currentQSize() const { return current.toSize(); }
QSizeF Variants::currentQSizeF() const { return current.toSizeF(); }

QPoint Variants::currentQPoint() const { return current.toPoint(); }
QPointF Variants::currentQPointF() const { return current.toPointF(); }

QLine Variants::currentQLine() const { return current.toLine(); }
QLineF Variants::currentQLineF() const { return current.toLineF(); }

QRect Variants::currentQRect() const { return current.toRect(); }
QRectF Variants::currentQRectF() const { return current.toRectF(); }

int Variants::currentType() const { return current.userType(); }

bool Variants::isValid() const { return current.isValid(); }

QString Variants::currentToString() { return current.toString(); }

int Variants::currentToInt() { return current.toInt(); }

double Variants::currentToDouble() { return current.toDouble(); }

bool Variants::currentToBoolean() { return current.toBool(); }

QVariant Variants::getDockWidgetArea(){ return QVariant::fromValue<Qt::DockWidgetArea>(Qt::BottomDockWidgetArea); }

QVariant Variants::getDockWidgetAreas(){
    return QVariant::fromValue<QFlags<Qt::DockWidgetArea>>(Qt::DockWidgetAreas(Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea));
}

QVariant Variants::getPageSize(){
    return QVariant::fromValue<QPageSize>(QPageSize(QSize(20,20)));
}

QVariant Variants::getSharedPointerOfWidget(){
#ifndef QTJAMBI_NO_WIDGETS
    QLabel* l = new QLabel();
    l->setText("TEST_TEXT");
    return QVariant::fromValue<QSharedPointer<QWidget> >(QSharedPointer<QWidget>(static_cast<QWidget*>(l)));
#else
    return QVariant();
#endif
}

QVariant Variants::getIntMap(){
    QMap<int, QString> mapping;
    mapping.insert(7, "Seven");
    mapping.insert(11, "Eleven");
    mapping.insert(42, "Forty-two");
    return QVariant::fromValue(mapping);
}

QVariant Variants::getColorHash(){
    QHash<QColor, QString> mapping;
    mapping.insert(QColor(Qt::red), "red");
    mapping.insert(QColor(Qt::green), "green");
    mapping.insert(QColor(Qt::blue), "blue");
    return QVariant::fromValue(mapping);
}

QVariant Variants::getIntMultiMap(){
    QMultiMap<int, QString> mapping;
    mapping.insert(7, "Seven");
    mapping.insert(7, "7");
    mapping.insert(11, "Eleven");
    mapping.insert(42, "Forty-two");
    return QVariant();
}

QVariant Variants::getIntVector(){
    QVector<int> vec;
    vec << 1 << 2 << 3 << 4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getIntLinkedList(){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QLinkedList<int> vec;
    vec << 1 << 2 << 3 << 4;
    return QVariant::fromValue(vec);
#else
    return QVariant();
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

QVariant Variants::getIntList(){
    QList<int> vec;
    vec << 1 << 2 << 3 << 4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getIntSet(){
    QSet<int> vec;
    vec << 1 << 2 << 3 << 4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getQObjectSet(){
    QSet<QObject*> vec;
    vec << QCoreApplication::instance() << QThread::currentThread() << QAbstractEventDispatcher::instance();
    return QVariant::fromValue(vec);
}

QVariant Variants::getIntQueue(){
    QQueue<int> vec;
    vec << 1 << 2 << 3 << 4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getIntStack(){
    QStack<int> vec;
    vec << 1 << 2 << 3 << 4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getCharMultiMap(){
    QMultiMap<char,qint16> vec;
    vec.insert('a', 1);
    vec.insert('a', 2);
    vec.insert('b', 501);
    vec.insert('b', 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getCharMultiHash(){
    QMultiHash<char,qint16> vec;
    vec.insert('a', 1);
    vec.insert('a', 2);
    vec.insert('b', 501);
    vec.insert('b', 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getInt16MultiMap(){
    QMultiMap<qint16,qint64> vec;
    vec.insert(3, 1);
    vec.insert(3, 2);
    vec.insert(7, 501);
    vec.insert(7, 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getInt16MultiHash(){
    QMultiHash<qint16,qint64> vec;
    vec.insert(3, 1);
    vec.insert(3, 2);
    vec.insert(7, 501);
    vec.insert(7, 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getInt64MultiMap(){
    QMultiMap<qint64,qint64> vec;
    vec.insert(3, 1);
    vec.insert(3, 2);
    vec.insert(7, 501);
    vec.insert(7, 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getInt64MultiHash(){
    QMultiHash<qint64,qint64> vec;
    vec.insert(3, 1);
    vec.insert(3, 2);
    vec.insert(7, 501);
    vec.insert(7, 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getStringVector(){
    QVector<QString> vec;
    vec << "S1" << "S2" << "S3" << "S4";
    return QVariant::fromValue(vec);
}

QVariant Variants::getStringLinkedList(){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QLinkedList<QString> vec;
    vec << "S1" << "S2" << "S3" << "S4";
    return QVariant::fromValue(vec);
#else
    return QVariant();
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

QVariant Variants::getStringList(){
    QList<QString> vec;
    vec << "S1" << "S2" << "S3" << "S4";
    return QVariant::fromValue(vec);
}

QVariant Variants::getStringSet(){
    QSet<QString> vec;
    vec << "S1" << "S2" << "S3" << "S4";
    return QVariant::fromValue(vec);
}

QVariant Variants::getStringQueue(){
    QQueue<QString> vec;
    vec << "S1" << "S2" << "S3" << "S4";
    return QVariant::fromValue(vec);
}

QVariant Variants::getStringStack(){
    QStack<QString> vec;
    vec << "S1" << "S2" << "S3" << "S4";
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumVector(){
    QVector<UnknownEnum> vec;
    vec << UnknownEnum::E1 << UnknownEnum::E2 << UnknownEnum::E3 << UnknownEnum::E4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumLinkedList(){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QLinkedList<UnknownEnum> vec;
    vec << UnknownEnum::E1 << UnknownEnum::E2 << UnknownEnum::E3 << UnknownEnum::E4;
    return QVariant::fromValue(vec);
#else
    return QVariant();
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

QVariant Variants::getUnknownEnumList(){
    QList<UnknownEnum> vec;
    vec << UnknownEnum::E1 << UnknownEnum::E2 << UnknownEnum::E3 << UnknownEnum::E4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumSet(){
    QSet<UnknownEnum> vec;
    vec << UnknownEnum::E1 << UnknownEnum::E2 << UnknownEnum::E3 << UnknownEnum::E4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumQueue(){
    QQueue<UnknownEnum> vec;
    vec << UnknownEnum::E1 << UnknownEnum::E2 << UnknownEnum::E3 << UnknownEnum::E4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumStack(){
    QStack<UnknownEnum> vec;
    vec << UnknownEnum::E1 << UnknownEnum::E2 << UnknownEnum::E3 << UnknownEnum::E4;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownValueVector(){
    QVector<UnknownValue> vec;
    vec << UnknownValue{};
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownValueLinkedList(){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QLinkedList<UnknownValue> vec;
    vec << UnknownValue{};
    return QVariant::fromValue(vec);
#else
    return QVariant();
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

QVariant Variants::getUnknownValueList(){
    QList<UnknownValue> vec;
    vec << UnknownValue{};
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownValueSet(){
    QSet<UnknownValue> vec;
    vec << UnknownValue{};
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownValueQueue(){
    QQueue<UnknownValue> vec;
    vec << UnknownValue{};
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownValueStack(){
    QStack<UnknownValue> vec;
    vec << UnknownValue{};
    return QVariant::fromValue(vec);
}

QVariant Variants::getDirVector(){
    QVector<QDir> vec;
    vec << QDir("/") << QDir("/home");
    return QVariant::fromValue(vec);
}

QVariant Variants::getDirLinkedList(){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QLinkedList<QDir> vec;
    vec << QDir("/") << QDir("/home");
    return QVariant::fromValue(vec);
#else
    return QVariant();
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

QVariant Variants::getDirList(){
    QList<QDir> vec;
    vec << QDir("/") << QDir("/home");
    return QVariant::fromValue(vec);
}

QVariant Variants::getDirSet(){
    QSet<QDir> vec;
    vec << QDir("/") << QDir("/home");
    return QVariant::fromValue(vec);
}

QVariant Variants::getDirQueue(){
    QQueue<QDir> vec;
    vec << QDir("/") << QDir("/home");
    return QVariant::fromValue(vec);
}

QVariant Variants::getDirStack(){
    QStack<QDir> vec;
    vec << QDir("/") << QDir("/home");
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectVector(){
    QVector<VariantTestObject*> vec;
    vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectLinkedList(){
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QLinkedList<VariantTestObject*> vec;
    vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
    return QVariant::fromValue(vec);
#else
    return QVariant();
#endif //QT_VERSION < QT_VERSION_CHECK(6,0,0)
}

QVariant Variants::getTestObjectList(){
    QList<VariantTestObject*> vec;
    vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectSet(){
    QSet<VariantTestObject*> vec;
    vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectQueue(){
    QQueue<VariantTestObject*> vec;
    vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectStack(){
    QStack<VariantTestObject*> vec;
    vec << new VariantTestObject(1) << new VariantTestObject(2) << new VariantTestObject(3) << new VariantTestObject(4);
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectMap(){
    QMap<VariantTestObject*,int> vec;
    for(int i=1; i<5; ++i){
        vec[new VariantTestObject(i)] = i;
    }
    vec[vec.keys().first()] = 100;
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectHash(){
    QHash<VariantTestObject*,int> vec;
    for(int i=1; i<5; ++i){
        vec[new VariantTestObject(i)] = i;
    }
    return QVariant::fromValue(vec);
}

QVariant Variants::getStringMultiMap(){
    QMultiMap<QString,int> vec;
    for(int i=1; i<5; ++i){
        vec.insert(QString("TEST%1").arg(i), i);
        vec.insert(QString("TEST%1").arg(i), i+200);
    }
    return QVariant::fromValue(vec);
}

QVariant Variants::getStringMultiHash(){
    QMultiHash<QString,int> vec;
    for(int i=1; i<5; ++i){
        vec.insert(QString("TEST%1").arg(i), i);
        vec.insert(QString("TEST%1").arg(i), i+200);
    }
    return QVariant::fromValue(vec);
}

QVariant Variants::getString64MultiHash(){
    QMultiHash<QString,qint64> vec;
    for(int i=1; i<5; ++i){
        vec.insert(QString("TEST%1").arg(i), i);
        vec.insert(QString("TEST%1").arg(i), i+200);
    }
    return QVariant::fromValue(vec);
}

QVariant Variants::getString16MultiHash(){
    QMultiHash<QString,qint16> vec;
    for(int i=1; i<5; ++i){
        vec.insert(QString("TEST%1").arg(i), qint16(i));
        vec.insert(QString("TEST%1").arg(i), qint16(i+200));
    }
    return QVariant::fromValue(vec);
}

QVariant Variants::getString8MultiHash(){
    QMultiHash<QString,qint8> vec;
    for(int i=1; i<5; ++i){
        vec.insert(QString("TEST%1").arg(i), qint8(i));
        vec.insert(QString("TEST%1").arg(i), qint8(i+20));
    }
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectMultiMap(){
    QMultiMap<VariantTestObject*,int> vec;
    for(int i=1; i<5; ++i){
        VariantTestObject* v = new VariantTestObject(i);
        vec.insert(v, i);
        vec.insert(v, i+200);
    }
    return QVariant::fromValue(vec);
}

QVariant Variants::getTestObjectMultiHash(){
    QMultiHash<VariantTestObject*,int> vec;
    for(int i=1; i<5; ++i){
        VariantTestObject* v = new VariantTestObject(i);
        vec.insert(v, i);
        vec.insert(v, i+200);
    }
    return QVariant::fromValue(vec);
}

QVariant Variants::getDirHash(){
    QHash<QDir,int> vec;
    vec[QDir("/")] = 1;
    vec[QDir("/home")] = 2;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumMap(){
    QMap<UnknownEnum,int> vec;
    vec[UnknownEnum::E1] = 1;
    vec[UnknownEnum::E2] = 2;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumHash(){
    QHash<UnknownEnum,int> vec;
    vec[UnknownEnum::E1] = 1;
    vec[UnknownEnum::E2] = 2;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumMultiMap(){
    QMultiMap<UnknownEnum,int> vec;
    vec.insert(UnknownEnum::E1, 1);
    vec.insert(UnknownEnum::E2, 2);
    vec.insert(UnknownEnum::E1, 501);
    vec.insert(UnknownEnum::E2, 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownEnumMultiHash(){
    QMultiHash<UnknownEnum,int> vec;
    vec.insert(UnknownEnum::E1, 1);
    vec.insert(UnknownEnum::E2, 2);
    vec.insert(UnknownEnum::E1, 501);
    vec.insert(UnknownEnum::E2, 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getColorMultiHash(){
    QMultiHash<QColor,QString> vec;
    vec.insert(QColor(Qt::red), "A");
    vec.insert(QColor(Qt::red), "B");
    vec.insert(QColor(Qt::green), "C");
    vec.insert(QColor(Qt::green), "D");
    return QVariant::fromValue(vec);
}

QVariant Variants::getColorMultiMap(){
    QMultiMap<QColor,qint16> vec;
    vec.insert(QColor(Qt::red), 1);
    vec.insert(QColor(Qt::red), 2);
    vec.insert(QColor(Qt::green), 501);
    vec.insert(QColor(Qt::green), 502);
    return QVariant::fromValue(vec);
}

QVariant Variants::getRectFMultiHash(){
    QMultiHash<QRectF,QString> vec;
    vec.insert(QRectF(1,2,3,4), "A");
    vec.insert(QRectF(2,3,4,5), "B");
    vec.insert(QRectF(1,2,3,4), "C");
    vec.insert(QRectF(2,3,4,5), "D");
    return QVariant::fromValue(vec);
}

QVariant Variants::getRectFMultiMap(){
    QMultiMap<QRectF,QString> vec;
    vec.insert(QRectF(1,2,3,4), "A");
    vec.insert(QRectF(2,3,4,5), "B");
    vec.insert(QRectF(1,2,3,4), "C");
    vec.insert(QRectF(2,3,4,5), "D");
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownValueMap(){
    QMap<UnknownValue,int> vec;
    vec[UnknownValue{}] = 1;
    return QVariant::fromValue(vec);
}

QVariant Variants::getUnknownValueHash(){
    QHash<UnknownValue,int> vec;
    vec[UnknownValue{}] = 1;
    return QVariant::fromValue(vec);
}

QVariant Variants::getDirPair(){
    QPair<QDir,QString> vec(QDir("/"), "ROOT");
    return QVariant::fromValue(vec);
}

QVariant Variants::getSpinBox(){
#ifndef QTJAMBI_NO_WIDGETS
    return QVariant::fromValue<QSpinBox* >(new QSpinBox());
#else
    return QVariant();
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QMetaType Variants::propertyType(const QObject* object, const QString& propertyName){
    QVariant variant = object->property(qPrintable(propertyName));
    return variant.metaType();
    return QMetaType(variant.userType());
}
#else
int Variants::propertyType(const QObject* object, const QString& propertyName){
    QVariant variant = object->property(qPrintable(propertyName));
    return variant.userType();
}
#endif

QColor Variants::fetchColorProperty(const QObject* object, const QString& propertyName){
    QVariant variant = object->property(qPrintable(propertyName));
    return variant.value<QColor>();
}

QObject* Variants::fetchObjectProperty(const QObject* object, const QString& propertyName){
    QVariant variant = object->property(qPrintable(propertyName));
    return variant.value<QObject*>();
}

QEvent* Variants::fetchEventProperty(const QObject* object, const QString& propertyName){
    QVariant variant = object->property(qPrintable(propertyName));
    return variant.value<QEvent*>();
}

QPaintDevice* Variants::fetchPaintDeviceProperty(const QObject* object, const QString& propertyName){
    QVariant variant = object->property(qPrintable(propertyName));
    return variant.value<QPaintDevice*>();
}

QRunnable* Variants::fetchRunnableProperty(const QObject* object, const QString& propertyName){
    QVariant variant = object->property(qPrintable(propertyName));
    return variant.value<QRunnable*>();
}

QStringList Variants::fetchStringListProperty(const QObject* object, const QString& propertyName){
    QVariant variant = object->property(qPrintable(propertyName));
    return variant.value<QStringList>();
}

void Variants::analyze(const JObjectWrapper& object){
    if(JniEnvironment env{16}){
        void* ptr = QtJambiAPI::convertJavaObjectToNative(env, object);
        Q_UNUSED(ptr)
    }
}

bool Variants::compare(QObject* object, const QString& property, const QList<QObject*>& list2){
    QVariant variant = object->property(qPrintable(property));
    QList<QObject*> list1 = variant.value<QList<QObject*>>();
    return list1==list2;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
bool Variants::compare(QObject* object, const QString& property, const QVector<QObject*>& list2){
    QVariant variant = object->property(qPrintable(property));
    QVector<QObject*> list1 = variant.value<QVector<QObject*>>();
    return list1==list2;
}
bool Variants::compare(QObject* object, const QString& property, const QLinkedList<QObject*>& list2){
    QVariant variant = object->property(qPrintable(property));
    QLinkedList<QObject*> list1 = variant.value<QLinkedList<QObject*>>();
    return list1==list2;
}
#endif


bool Variants::compare(QObject* object, const QString& property, const QPair<QObject*,QObject*>& pair2){
    QVariant variant = object->property(qPrintable(property));
    QPair<QObject*,QObject*> pair1 = variant.value<QPair<QObject*,QObject*>>();
    return pair1==pair2;
}

#ifndef QTJAMBI_NO_WIDGETS
QGraphicsItem* Variants::convertInterface(QGraphicsObject* object) {
    QVariant variant = QVariant::fromValue(object);
    return variant.value<QGraphicsItem*>();
}
#endif

QObject* Variants::convertJavaObject(jobject obj){
    QVariant variant = QVariant::fromValue<jobject>(obj);
    if(variant.convert(QMetaType::fromName("JObjectWrapper"))){
        return variant.value<QObject*>();
    }
    return nullptr;
}

