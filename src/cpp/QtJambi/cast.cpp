/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>
#include <QtCore/QSettings>
QT_WARNING_DISABLE_DEPRECATED
#include "pch_p.h"
#include "qtjambi_cast_buffer.h"
#include "qtjambi_cast_array.h"
#include "qtjambi_cast_future.h"
#include "qtjambi_cast_model.h"
#include "qtjambi_cast_arithmetic.h"
#include "qtjambi_cast_container.h"
#include "qtjambi_cast_enum.h"
#include "qtjambi_cast_iterator.h"
#include "qtjambi_cast_smartpointer.h"
#include "qtjambi_cast_template1.h"
#include "qtjambi_cast_template2.h"
#include "qtjambi_cast_template3.h"
#include "qtjambi_cast_template4.h"
#include "qtjambi_cast_template5.h"
#include "qtjambi_cast_time.h"

#if defined(Q_CC_CLANG) && defined(Q_OS_WIN)
// this is nessesary due to a llvm clang bug with exported template classes
template QTJAMBI_EXPORT PointerArray<false,jbyteArray,false,jbyte,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jshortArray,false,jshort,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jintArray,false,jint,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jlongArray,false,jlong,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jcharArray,false,jchar,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jfloatArray,false,jfloat,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jdoubleArray,false,jdouble,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jbooleanArray,false,jboolean,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jbyteArray,true,jbyte,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jshortArray,true,jshort,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jintArray,true,jint,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jlongArray,true,jlong,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jcharArray,true,jchar,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jfloatArray,true,jfloat,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jdoubleArray,true,jdouble,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<false,jbooleanArray,true,jboolean,true>::~PointerArray();

template QTJAMBI_EXPORT PointerArray<true,jbyteArray,false,jbyte,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jshortArray,false,jshort,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jintArray,false,jint,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jlongArray,false,jlong,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jcharArray,false,jchar,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jfloatArray,false,jfloat,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jdoubleArray,false,jdouble,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jbooleanArray,false,jboolean,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jbyteArray,true,jbyte,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jshortArray,true,jshort,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jintArray,true,jint,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jlongArray,true,jlong,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jcharArray,true,jchar,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jfloatArray,true,jfloat,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jdoubleArray,true,jdouble,true>::~PointerArray();
template QTJAMBI_EXPORT PointerArray<true,jbooleanArray,true,jboolean,true>::~PointerArray();
#endif

template jobject qtjambi_cast<jobject,Qt::InputMethodQuery&>(JNIEnv *, Qt::InputMethodQuery&);
template jobject qtjambi_cast<jobject,Qt::ItemSelectionMode&>(JNIEnv *, Qt::ItemSelectionMode&);

template QStringList qtjambi_cast<QStringList,jobject&>(JNIEnv *, jobject&);
template QStringList qtjambi_cast<QStringList,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template const QStringList& qtjambi_cast<const QStringList&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template jobject qtjambi_cast<jobject,QStringList>(JNIEnv *, QStringList&&);
template jobject qtjambi_cast<jobject,const QStringList&>(JNIEnv *, const QStringList&);
template QList<QObject*> qtjambi_cast<QList<QObject*>,jobject&>(JNIEnv *, jobject&);
template QList<QObject*> qtjambi_cast<QList<QObject*>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template const QList<QObject*>& qtjambi_cast<const QList<QObject*>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template jobject qtjambi_cast<jobject,QList<QObject*>>(JNIEnv *, QList<QObject*>&&);
template jobject qtjambi_cast<jobject,const QList<QObject*>&>(JNIEnv *, const QList<QObject*>&);
template QList<QVariant> qtjambi_cast<QList<QVariant>,jobject&>(JNIEnv *, jobject&);
template QList<QVariant> qtjambi_cast<QList<QVariant>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template const QList<QVariant>& qtjambi_cast<const QList<QVariant>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template jobject qtjambi_cast<jobject,QList<QVariant>>(JNIEnv *, QList<QVariant>&&);
template jobject qtjambi_cast<jobject,const QList<QVariant>&>(JNIEnv *, const QList<QVariant>&);
template QList<JObjectWrapper> qtjambi_cast<QList<JObjectWrapper>,jobject&>(JNIEnv *, jobject&);
template QList<JObjectWrapper> qtjambi_cast<QList<JObjectWrapper>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template const QList<JObjectWrapper>& qtjambi_cast<const QList<JObjectWrapper>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template jobject qtjambi_cast<jobject,QList<JObjectWrapper>>(JNIEnv *, QList<JObjectWrapper>&&);
template jobject qtjambi_cast<jobject,const QList<JObjectWrapper>&>(JNIEnv *, const QList<JObjectWrapper>&);

template jobject qtjambi_cast<jobject,QPromise<QVariant>&>(JNIEnv *, QPromise<QVariant>&);
template jobject qtjambi_cast<jobject,QPromise<void>&>(JNIEnv *, QPromise<void>&);

template QFuture<QVariant> qtjambi_cast<QFuture<QVariant>,jobject&>(JNIEnv *, jobject&);
template const QFuture<QVariant>& qtjambi_cast<const QFuture<QVariant>&,jobject&>(JNIEnv *, jobject&);
template jobject qtjambi_cast<jobject,QFuture<QVariant>>(JNIEnv *, QFuture<QVariant>&&);
template jobject qtjambi_cast<jobject,const QFuture<QVariant>&>(JNIEnv *, const QFuture<QVariant>&);
template QFuture<void> qtjambi_cast<QFuture<void>,jobject&>(JNIEnv *, jobject&);
template const QFuture<void>& qtjambi_cast<const QFuture<void>&,jobject&>(JNIEnv *, jobject&);
template jobject qtjambi_cast<jobject,QFuture<void>>(JNIEnv *, QFuture<void>&&);
template jobject qtjambi_cast<jobject,const QFuture<void>&>(JNIEnv *, const QFuture<void>&);
template QFuture<JObjectWrapper> qtjambi_cast<QFuture<JObjectWrapper>,jobject&>(JNIEnv *, jobject&);
template const QFuture<JObjectWrapper>& qtjambi_cast<const QFuture<JObjectWrapper>&,jobject&>(JNIEnv *, jobject&);
template jobject qtjambi_cast<jobject,QFuture<JObjectWrapper>>(JNIEnv *, QFuture<JObjectWrapper>&&);
template jobject qtjambi_cast<jobject,const QFuture<JObjectWrapper>&>(JNIEnv *, const QFuture<JObjectWrapper>&);

template QVariant qtjambi_cast<QVariant,jobject&>(JNIEnv *, jobject&);
template jobject qtjambi_cast<jobject,QVariant>(JNIEnv *, QVariant&&);
template jobject qtjambi_cast<jobject,QVariant&>(JNIEnv *, QVariant&);
template jobject qtjambi_cast<jobject,const QVariant&>(JNIEnv *, const QVariant&);
template jobject qtjambi_cast<jobject,const QVariant*&>(JNIEnv *, const QVariant*&);
template jobject qtjambi_cast<jcoreobject,QVariant>(JNIEnv *, QVariant&&);
template jobject qtjambi_cast<jcoreobject,QVariant&>(JNIEnv *, QVariant&);
template jobject qtjambi_cast<jcoreobject,QVariant*&>(JNIEnv *, QVariant*&);
template jobject qtjambi_cast<jcoreobject,const QVariant&>(JNIEnv *, const QVariant&);

template QString qtjambi_cast<QString,jstring&>(JNIEnv *, jstring&);
template jstring qtjambi_cast<jstring,QString>(JNIEnv *, QString&&);
template jstring qtjambi_cast<jstring,QString&>(JNIEnv *, QString&);
template jstring qtjambi_cast<jstring,const QString&>(JNIEnv *, const QString&);
template jobject qtjambi_cast<jcoreobject,QString>(JNIEnv *, QString&&);
template jobject qtjambi_cast<jcoreobject,QString&>(JNIEnv *, QString&);
template jobject qtjambi_cast<jcoreobject,QString*&>(JNIEnv *, QString*&);
template jobject qtjambi_cast<jcoreobject,const QString&>(JNIEnv *, const QString&);

template QStringView qtjambi_cast<QStringView,jstring&>(JNIEnv *, QtJambiScope&, jstring&);
template jstring qtjambi_cast<jstring,QStringView>(JNIEnv *, QStringView&&);
template jstring qtjambi_cast<jstring,QStringView&>(JNIEnv *, QStringView&);
template jstring qtjambi_cast<jstring,const QStringView&>(JNIEnv *, const QStringView&);

template QAnyStringView qtjambi_cast<QAnyStringView,jstring&>(JNIEnv *, QtJambiScope&, jstring&);
template jstring qtjambi_cast<jstring,QAnyStringView>(JNIEnv *, QAnyStringView&&);
template jstring qtjambi_cast<jstring,QAnyStringView&>(JNIEnv *, QAnyStringView&);
template jstring qtjambi_cast<jstring,const QAnyStringView&>(JNIEnv *, const QAnyStringView&);

template QLatin1StringView qtjambi_cast<QLatin1StringView,jstring&>(JNIEnv *, QtJambiScope&, jstring&);
template jstring qtjambi_cast<jstring,QLatin1StringView>(JNIEnv *, QLatin1StringView&&);
template jstring qtjambi_cast<jstring,QLatin1StringView&>(JNIEnv *, QLatin1StringView&);
template jstring qtjambi_cast<jstring,const QLatin1StringView&>(JNIEnv *, const QLatin1StringView&);

template QObject* qtjambi_cast<QObject*,jobject&>(JNIEnv *, jobject&);
template jobject qtjambi_cast<jobject,QEvent*&>(JNIEnv *, QEvent*&);
template jobject qtjambi_cast<jobject,QTimerEvent*&>(JNIEnv *, QTimerEvent*&);
template jobject qtjambi_cast<jobject,QChildEvent*&>(JNIEnv *, QChildEvent*&);

template jobject qtjambi_cast<jobject,const QMetaMethod&>(JNIEnv *, const QMetaMethod&);
template jobject qtjambi_cast<jobject,const QMetaProperty&>(JNIEnv *, const QMetaProperty&);
template jobject qtjambi_cast<jobject,const QMetaObject*&>(JNIEnv *, const QMetaObject*&);

template jobject qtjambi_cast<jobject,QBindable<int>>(JNIEnv *, QBindable<int>&&);
template jobject qtjambi_cast<jobject,QBindable<bool>>(JNIEnv *, QBindable<bool>&&);
template jobject qtjambi_cast<jobject,QBindable<QString>>(JNIEnv *, QBindable<QString>&&);
template jobject qtjambi_cast<jobject,QBindable<QByteArray>>(JNIEnv *, QBindable<QByteArray>&&);
template jobject qtjambi_cast<jobject,QBindable<QObject*>>(JNIEnv *, QBindable<QObject*>&&);
template jobject qtjambi_cast<jobject,QBindable<const QObject*>>(JNIEnv *, QBindable<const QObject*>&&);
template jobject qtjambi_cast<jobject,QBindable<float>>(JNIEnv *, QBindable<float>&&);
template jobject qtjambi_cast<jobject,QBindable<double>>(JNIEnv *, QBindable<double>&&);

template QUrl qtjambi_cast<QUrl,jobject&>(JNIEnv *, jobject&);
template jobject qtjambi_cast<jobject,QUrl>(JNIEnv *, QUrl&&);
template jobject qtjambi_cast<jobject,QUrl&>(JNIEnv *, QUrl&);
template jobject qtjambi_cast<jobject,const QUrl&>(JNIEnv *, const QUrl&);

template QModelIndex qtjambi_cast<QModelIndex,jobject&>(JNIEnv *, jobject&);
template jobject qtjambi_cast<jobject,QModelIndex>(JNIEnv *, QModelIndex&&);
template jobject qtjambi_cast<jobject,QModelIndex&>(JNIEnv *, QModelIndex&);
template jobject qtjambi_cast<jobject,const QModelIndex&>(JNIEnv *, const QModelIndex&);

template jobject qtjambi_cast<jobject,QMargins>(JNIEnv *, QMargins&&);
template jobject qtjambi_cast<jobject,QMarginsF>(JNIEnv *, QMarginsF&&);

template jobject qtjambi_cast<jobject,QRect>(JNIEnv *, QRect&&);
template jobject qtjambi_cast<jobject,const QRect&>(JNIEnv *, const QRect&);
template jobject qtjambi_cast<jobject,QRectF>(JNIEnv *, QRectF&&);
template jobject qtjambi_cast<jobject,const QRectF&>(JNIEnv *, const QRectF&);
template jobject qtjambi_cast<jobject,QPoint>(JNIEnv *, QPoint&&);
template jobject qtjambi_cast<jobject,const QPoint&>(JNIEnv *, const QPoint&);
template jobject qtjambi_cast<jobject,QPointF>(JNIEnv *, QPointF&&);
template jobject qtjambi_cast<jobject,const QPointF&>(JNIEnv *, const QPointF&);
template jobject qtjambi_cast<jobject,QSize>(JNIEnv *, QSize&&);
template jobject qtjambi_cast<jobject,const QSize&>(JNIEnv *, const QSize&);
template jobject qtjambi_cast<jobject,QSizeF>(JNIEnv *, QSizeF&&);
template jobject qtjambi_cast<jobject,const QSizeF&>(JNIEnv *, const QSizeF&);

template QList<QPointF> qtjambi_cast<QList<QPointF>,jobject&>(JNIEnv *, jobject&);
template QList<QPointF> qtjambi_cast<QList<QPointF>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template const QList<QPointF>& qtjambi_cast<const QList<QPointF>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template jobject qtjambi_cast<jobject,QList<QPointF>>(JNIEnv *, QList<QPointF>&&);
template jobject qtjambi_cast<jobject,const QList<QPointF>&>(JNIEnv *, const QList<QPointF>&);
template QList<QPoint> qtjambi_cast<QList<QPoint>,jobject&>(JNIEnv *, jobject&);
template QList<QPoint> qtjambi_cast<QList<QPoint>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template const QList<QPoint>& qtjambi_cast<const QList<QPoint>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template jobject qtjambi_cast<jobject,QList<QPoint>>(JNIEnv *, QList<QPoint>&&);
template jobject qtjambi_cast<jobject,const QList<QPoint>&>(JNIEnv *, const QList<QPoint>&);
template QList<QSize> qtjambi_cast<QList<QSize>,jobject&>(JNIEnv *, jobject&);
template QList<QSize> qtjambi_cast<QList<QSize>,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template const QList<QSize>& qtjambi_cast<const QList<QSize>&,jobject&>(JNIEnv *, QtJambiScope&, jobject&);
template jobject qtjambi_cast<jobject,QList<QSize>>(JNIEnv *, QList<QSize>&&);
template jobject qtjambi_cast<jobject,const QList<QSize>&>(JNIEnv *, const QList<QSize>&);

#ifndef QT_NO_DEBUG
QT_WARNING_DISABLE_GCC("-Wstringop-overflow")

class UnknownKey{
public:
    UnknownKey(int){
//        QTJAMBI_IN_CONSTRUCTOR_CALL("UnknownKey(int)", this)
    }
    void test(){
//        QTJAMBI_JAVA_METHOD_CALL("UnknownKey::test()")
    }
};

//bool operator <(const UnknownKey&,const UnknownKey&){return false;}
//bool operator ==(const UnknownKey&,const UnknownKey&){return false;}
uint qHash(const UnknownKey&){return 0;}

class UnknownClass{
public:
    UnknownClass(int){
        QTJAMBI_IN_CONSTRUCTOR_CALL("UnknownClass(int)")
    }
};
bool operator ==(const UnknownClass&,const UnknownClass&){return false;}
//bool operator <(const UnknownClass&,const UnknownClass&){return false;}
uint qHash(const UnknownClass&){return 0;}
uint qHash(const QMap<QString,int>&){return 0;}
//uint qHash(const QLinkedList<QString,int>&){return 0;}

enum class EnumClass{
    None
};

void test(JNIEnv *env){
    using namespace RegistryAPI;

    QtJambiScope scope;
    enum E{};

    QTJAMBI_TRY{
    }QTJAMBI_CATCH(const JavaException& exn){
        Q_UNUSED(exn)
    }QTJAMBI_TRY_END

    registerEnumTypeInfoNoMetaObject<E>("qt_name", "java_name");
    registerEnumTypeInfoNoMetaObject<E>("qt_name", "java_name", "flags_qt_name", "flags_qt_name_alias", "flags_java_name");

    jstring js = nullptr;
    jobject jo = nullptr;
    {
        int length{0};
        const char* arrayY = qtjambi_cast<const char*>(env, scope, js, length);
        const char* array1 = qtjambi_cast<const char*>(env, scope, js, 5);
        const char* arrayX = qtjambi_cast<const char*>(env, scope, js);
        const char* array2 = qtjambi_cast<const char[5]>(env, scope, js);
        const char(&array3)[5] = qtjambi_cast<const char(&)[5]>(env, scope, js);
        (void)qtjambi_cast<jstring>(env, array1);
        Q_UNUSED(array2)
        Q_UNUSED(arrayX)
        Q_UNUSED(arrayY)
        (void)qtjambi_cast<jstring>(env, array3);
    }
    {
        std::string_view v = qtjambi_cast<std::string_view>(env, js);
        (void)qtjambi_cast<jstring>(env, std::move(v));
        (void)qtjambi_cast<jobject>(env, std::move(v));
        (void)qtjambi_cast<jbyteArray>(env, std::move(v));
        std::string s = qtjambi_cast<std::string>(env, js);
        (void)qtjambi_cast<jstring>(env, std::move(s));
        (void)qtjambi_cast<jobject>(env, std::move(s));
        (void)qtjambi_cast<jbyteArray>(env, std::move(s));
    }
    {
        std::u16string_view v = qtjambi_cast<std::u16string_view>(env, js);
        (void)qtjambi_cast<jstring>(env, std::move(v));
        (void)qtjambi_cast<jobject>(env, std::move(v));
        (void)qtjambi_cast<jcharArray>(env, std::move(v));
        std::u16string s = qtjambi_cast<std::u16string>(env, js);
        (void)qtjambi_cast<jstring>(env, std::move(s));
        (void)qtjambi_cast<jobject>(env, std::move(s));
        (void)qtjambi_cast<jcharArray>(env, std::move(s));
    }
    {
        jbyteArray ba = nullptr;
        char& ref = qtjambi_cast<char&>(env, scope, ba);
        (void)qtjambi_cast<jbyteArray>(env, scope, ref);
        const char& cref = qtjambi_cast<const char&>(env, scope, ba);
        (void)qtjambi_cast<jbyteArray>(env, scope, cref);
    }
    {
        const char16_t* array1 = qtjambi_cast<const char16_t*>(env, scope, js, 5);
        const char16_t* arrayX = qtjambi_cast<const char16_t*>(env, scope, js);
        const char16_t* array2 = qtjambi_cast<const char16_t[5]>(env, scope, js);
        const char16_t(&array3)[5] = qtjambi_cast<const char16_t(&)[5]>(env, scope, js);
        (void)qtjambi_cast<jstring>(env, array1);
        Q_UNUSED(array2)
        Q_UNUSED(arrayX)
        (void)qtjambi_cast<jstring>(env, array3);
    }
    {
        jintArray ja{nullptr};
        const int* array1 = qtjambi_cast<const int*>(env, scope, ja, 5);
        const int* arrayX = qtjambi_cast<const int*>(env, scope, ja);
        const int* array2 = qtjambi_cast<const int[5]>(env, scope, ja);
        const int(&array3)[5] = qtjambi_cast<const int(&)[5]>(env, scope, ja);
        int(&array4)[5] = qtjambi_cast<int(&)[5]>(env, scope, ja);
        (void)qtjambi_cast<jintArray>(env, array1, 5);
        Q_UNUSED(array2)
        Q_UNUSED(arrayX)
        (void)qtjambi_cast<jintArray>(env, array3);
        (void)qtjambi_cast<jintArray>(env, array4);
        (void)qtjambi_cast<const qsizetype[5]>(env, scope, ja);
        try{(void)qtjambi_cast<const qsizetype(&)[5]>(env, scope, ja);}catch(const JavaException&){}
        (void)qtjambi_cast<const qsizetype*>(env, scope, ja, 5);
        (void)qtjambi_cast<qsizetype[5]>(env, scope, ja);
        try{(void)qtjambi_cast<qsizetype(&)[5]>(env, scope, ja);}catch(const JavaException&){}
        (void)qtjambi_cast<qsizetype*>(env, scope, ja, 5);
    }
    {
        const QEasingCurve::EasingFunction funPointer = [](qreal a) -> qreal{ return a*a; };
        (void)qtjambi_cast<jobject>(env, funPointer, "QEasingCurve::EasingFunction");
        auto funPointer1 = qtjambi_cast<jobject>(env, funPointer, "QEasingCurve::EasingFunction");
        (void)qtjambi_cast<QEasingCurve::EasingFunction>(env, funPointer1, "QEasingCurve::EasingFunction");
        (void)qtjambi_cast<QEasingCurve::EasingFunction>(env, funPointer1);
    }
    if(JniEnvironment env{256}){
        {
            jintArray ja{nullptr};
            const int* array1 = qtjambi_cast<const int*>(env, scope, ja, 5);
            const int* arrayX = qtjambi_cast<const int*>(env, scope, ja);
            const int* array2 = qtjambi_cast<const int[5]>(env, scope, ja);
            const int(&array3)[5] = qtjambi_cast<const int(&)[5]>(env, scope, ja);
            int(&array4)[5] = qtjambi_cast<int(&)[5]>(env, scope, ja);
            (void)qtjambi_cast<jintArray>(env, array1, 5);
            Q_UNUSED(array2)
            Q_UNUSED(arrayX)
            (void)qtjambi_cast<jintArray>(env, array3);
            (void)qtjambi_cast<jintArray>(env, array4);
            (void)qtjambi_cast<const qsizetype[5]>(env, scope, ja);
            try{(void)qtjambi_cast<const qsizetype(&)[5]>(env, scope, ja);}catch(const JavaException&){}
            (void)qtjambi_cast<const qsizetype*>(env, scope, ja, 5);
            (void)qtjambi_cast<qsizetype[5]>(env, scope, ja);
            try{(void)qtjambi_cast<qsizetype(&)[5]>(env, scope, ja);}catch(const JavaException&){}
            (void)qtjambi_cast<qsizetype*>(env, scope, ja, 5);
        }
        {
            const QEasingCurve::EasingFunction funPointer = [](qreal a) -> qreal{ return a*a; };
            (void)qtjambi_cast<jobject>(env, funPointer, "QEasingCurve::EasingFunction");
            auto funPointer1 = qtjambi_cast<jobject>(env, funPointer, "QEasingCurve::EasingFunction");
            (void)qtjambi_cast<QEasingCurve::EasingFunction>(env, funPointer1, "QEasingCurve::EasingFunction");
            (void)qtjambi_cast<QEasingCurve::EasingFunction>(env, funPointer1);
        }
    }
    {
        QString s;
        (void)qtjambi_cast<jobject>(env, s);
        (void)qtjambi_cast<jcoreobject>(env, s);
        (void)qtjambi_cast<jstring>(env, s);
        (void)qtjambi_cast<jstring>(env, &s);

        (void)qtjambi_cast<QString>(env, js);
        (void)qtjambi_cast<const QString>(env, js);
        (void)qtjambi_cast<const QString*>(env, scope, js);
        (void)qtjambi_cast<const QString&>(env, scope, js);

        (void)qtjambi_cast<QString>(env, jo);
        (void)qtjambi_cast<const QString>(env, jo);
        (void)qtjambi_cast<QString*>(env, jo);
        (void)qtjambi_cast<QString&>(env, jo);
        (void)qtjambi_cast<const QString*>(env, jo);
        (void)qtjambi_cast<const QString&>(env, jo);
    }

    {
        QStringView s;
        (void)qtjambi_cast<jobject>(env, s);
        (void)qtjambi_cast<jstring>(env, s);
        (void)qtjambi_cast<jstring>(env, &s);

        (void)qtjambi_cast<QStringView>(env, scope, js);
        (void)qtjambi_cast<const QStringView>(env, scope, js);
        (void)qtjambi_cast<QStringView*>(env, scope, js);
        (void)qtjambi_cast<QStringView&>(env, scope, js);
        (void)qtjambi_cast<const QStringView*>(env, scope, js);
        (void)qtjambi_cast<const QStringView&>(env, scope, js);

        (void)qtjambi_cast<QStringView>(env, jo);
        (void)qtjambi_cast<const QStringView>(env, jo);
        (void)qtjambi_cast<QStringView*>(env, scope, jo);
        (void)qtjambi_cast<QStringView&>(env, scope, jo);
        (void)qtjambi_cast<const QStringView*>(env, scope, jo);
        (void)qtjambi_cast<const QStringView&>(env, scope, jo);
    }

    {
        QLatin1String s;
        (void)qtjambi_cast<jobject>(env, s);
        (void)qtjambi_cast<jstring>(env, s);
        (void)qtjambi_cast<jstring>(env, &s);

        (void)qtjambi_cast<QLatin1String>(env, scope, js);
        (void)qtjambi_cast<const QLatin1String>(env, scope, js);
        (void)qtjambi_cast<QLatin1String*>(env, scope, js);
        (void)qtjambi_cast<QLatin1String&>(env, scope, js);
        (void)qtjambi_cast<const QLatin1String*>(env, scope, js);
        (void)qtjambi_cast<const QLatin1String&>(env, scope, js);

        (void)qtjambi_cast<QLatin1String>(env, jo);
        (void)qtjambi_cast<const QLatin1String>(env, jo);
        (void)qtjambi_cast<QLatin1String&>(env, scope, jo);
        (void)qtjambi_cast<const QLatin1String&>(env, scope, jo);
    }

    {
        QByteArray s;
        (void)qtjambi_cast<jobject>(env, s);
        (void)qtjambi_cast<jobject>(env, QtJambiNativeID::Invalid, s.constBegin());
        (void)qtjambi_cast<jstring>(env, s);
        (void)qtjambi_cast<jstring>(env, &s);

        (void)qtjambi_cast<QByteArray>(env, js);
        (void)qtjambi_cast<const QByteArray>(env, js);
        (void)qtjambi_cast<const QByteArray*>(env, scope, js);
        (void)qtjambi_cast<const QByteArray&>(env, scope, js);
    }

    {
        const char* s{nullptr};
        (void)qtjambi_cast<jobject>(env, s);
        (void)qtjambi_cast<jstring>(env, s);
        (void)qtjambi_cast<jstring>(env, &s);
    }
    {
        QByteArrayView s;
        (void)qtjambi_cast<jobject>(env, s);
        (void)qtjambi_cast<jstring>(env, s);
        (void)qtjambi_cast<jstring>(env, &s);

        (void)qtjambi_cast<QByteArrayView>(env, scope, js);
        (void)qtjambi_cast<const QByteArrayView>(env, scope, js);
        (void)qtjambi_cast<QByteArrayView*>(env, scope, js);
        (void)qtjambi_cast<QByteArrayView&>(env, scope, js);
        (void)qtjambi_cast<const QByteArrayView*>(env, scope, js);
        (void)qtjambi_cast<const QByteArrayView&>(env, scope, js);
    }

    {
        QAnyStringView s;
        (void)qtjambi_cast<jobject>(env, s);
        (void)qtjambi_cast<jstring>(env, s);
        (void)qtjambi_cast<jstring>(env, &s);

        (void)qtjambi_cast<QAnyStringView>(env, scope, js);
        (void)qtjambi_cast<const QAnyStringView>(env, scope, js);
        (void)qtjambi_cast<QAnyStringView*>(env, scope, js);
        (void)qtjambi_cast<QAnyStringView&>(env, scope, js);
        (void)qtjambi_cast<const QAnyStringView*>(env, scope, js);
        (void)qtjambi_cast<const QAnyStringView&>(env, scope, js);

        (void)qtjambi_cast<QAnyStringView>(env, jo);
        (void)qtjambi_cast<const QAnyStringView>(env, jo);
        (void)qtjambi_cast<QAnyStringView*>(env, scope, jo);
        (void)qtjambi_cast<QAnyStringView&>(env, scope, jo);
        (void)qtjambi_cast<const QAnyStringView*>(env, scope, jo);
        (void)qtjambi_cast<const QAnyStringView&>(env, scope, jo);
    }

    {
        QUtf8StringView s;
        (void)qtjambi_cast<jobject>(env, s);
        (void)qtjambi_cast<jstring>(env, s);
        (void)qtjambi_cast<jstring>(env, &s);

        (void)qtjambi_cast<QUtf8StringView>(env, scope, js);
        (void)qtjambi_cast<const QUtf8StringView>(env, scope, js);
        (void)qtjambi_cast<QUtf8StringView*>(env, scope, js);
        (void)qtjambi_cast<QUtf8StringView&>(env, scope, js);
        (void)qtjambi_cast<const QUtf8StringView*>(env, scope, js);
        (void)qtjambi_cast<const QUtf8StringView&>(env, scope, js);

        (void)qtjambi_cast<QUtf8StringView>(env, jo);
        (void)qtjambi_cast<const QUtf8StringView>(env, jo);
        (void)qtjambi_cast<QUtf8StringView*>(env, scope, jo);
        (void)qtjambi_cast<QUtf8StringView&>(env, scope, jo);
        (void)qtjambi_cast<const QUtf8StringView*>(env, scope, jo);
        (void)qtjambi_cast<const QUtf8StringView&>(env, scope, jo);
    }
    {
        (void)qtjambi_cast<jobject>(env, QFutureInterface<void>());
        (void)qtjambi_cast<jobject>(env, QFutureInterface<int>());
        (void)qtjambi_cast<jobject>(env, QFutureInterface<QVariant>());
        (void)qtjambi_cast<QFutureInterface<void>>(env, jobject(nullptr));
        (void)qtjambi_cast<QFutureInterface<int>>(env, jobject(nullptr));
        (void)qtjambi_cast<QFutureInterface<QVariant>>(env, jobject(nullptr));
        (void)qtjambi_cast<const QFutureInterface<void>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<const QFutureInterface<int>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<const QFutureInterface<QVariant>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<QFutureInterface<void>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<QFutureInterface<int>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<QFutureInterface<QVariant>&>(env, scope, jobject(nullptr));
    }
    {
        (void)qtjambi_cast<jobject>(env, QFuture<void>());
        (void)qtjambi_cast<jobject>(env, QFuture<int>());
        (void)qtjambi_cast<jobject>(env, QFuture<QVariant>());
        (void)qtjambi_cast<QFuture<void>>(env, jobject(nullptr));
        (void)qtjambi_cast<QFuture<int>>(env, jobject(nullptr));
        (void)qtjambi_cast<QFuture<QVariant>>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<const QFuture<void>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<const QFuture<int>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<const QFuture<QVariant>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<QFuture<void>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<QFuture<int>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<QFuture<QVariant>&>(env, scope, jobject(nullptr));
    }
    {
        void* ptr = nullptr;
        (void)qtjambi_cast<jobject>(env, *reinterpret_cast<QPromise<void>*>(ptr));
        (void)qtjambi_cast<jobject>(env, scope, *reinterpret_cast<QPromise<int>*>(ptr));
        (void)qtjambi_cast<jobject>(env, *reinterpret_cast<QPromise<QVariant>*>(ptr));
        (void)qtjambi_cast<QPromise<void>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<QPromise<int>&>(env, scope, jobject(nullptr));
        (void)qtjambi_cast<QPromise<QVariant>&>(env, scope, jobject(nullptr));
    }
    {
        (void)qtjambi_cast<jobject>(env, std::vector<int>());
        (void)qtjambi_cast<std::vector<int>>(env, jobject(nullptr));
        (void)qtjambi_cast<jobject>(env, QWeakPointer<QPoint>());
        (void)qtjambi_cast<QWeakPointer<QPoint>>(env, jobject(nullptr));
        (void)qtjambi_cast<jobject>(env, std::vector<QWeakPointer<QPoint>>());
        (void)qtjambi_cast<std::vector<QWeakPointer<QPoint>>>(env, jobject(nullptr));
    }
    {
        QList<int> list;
        QDataStream stream;
        stream << list;
        stream >> list;
        Q_STATIC_ASSERT(QtJambiPrivate::supports_stream_operators<QList<int>>::value);
        Q_STATIC_ASSERT(!QtJambiPrivate::supports_stream_operators<QList<UnknownClass>>::value);
    }
    {
        QtJambiUtils::QHashFunction hashFunction1;
        QtJambiUtils::QHashFunction hashFunction2 = [](const void*, size_t)->size_t{ return 0; };
        QtJambiUtils::QHashFunction hashFunction3 = hashFunction2;
        hashFunction3 = [](const void*, size_t)->size_t{ return 0; };
        QtJambiUtils::QHashFunction hashFunction4(QtJambiUtils::QHashFunction([](const void*, size_t)->size_t{ return 0; }));
        hashFunction3(nullptr,0);
        QtJambiUtils::QHashFunction hashFunction5 = nullptr;
    }
    {
        QtJambiUtils::InternalToExternalConverter c1;
        QtJambiUtils::InternalToExternalConverter c2 = c1;
        QtJambiUtils::InternalToExternalConverter c3 = [](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{return false;};
        QtJambiUtils::InternalToExternalConverter c4([](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{return false;});
        QtJambiUtils::InternalToExternalConverter c5(QtJambiUtils::InternalToExternalConverter([](JNIEnv*, QtJambiScope*, const void*, jvalue&, bool)->bool{return false;}));
        QtJambiUtils::InternalToExternalConverter c6 = nullptr;
    }
    {
        QtJambiUtils::ExternalToInternalConverter c1;
        QtJambiUtils::ExternalToInternalConverter c2 = c1;
        QtJambiUtils::ExternalToInternalConverter c3 = [](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType)->bool{return false;};
        QtJambiUtils::ExternalToInternalConverter c4([](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType)->bool{return false;});
        QtJambiUtils::ExternalToInternalConverter c5(QtJambiUtils::ExternalToInternalConverter([](JNIEnv*, QtJambiScope*, jvalue, void* &, jValueType)->bool{return false;}));
        QtJambiUtils::ExternalToInternalConverter c6 = nullptr;
    }
    {
        QtJambiUtils::Runnable c1;
        QtJambiUtils::Runnable c2 = c1;
        QtJambiUtils::Runnable c3 = [](){};
        QtJambiUtils::Runnable c4([](){});
        QtJambiUtils::Runnable c5(QtJambiUtils::Runnable([](){}));
        QtJambiUtils::Runnable c6 = nullptr;
    }
    {
        QStringList l;
        const QStringList& lr = l;
        (void)qtjambi_cast<jobject>(env, l);
        QtJambiScope __qtjambi_scope;
        (void)qtjambi_cast<jobject>(env, __qtjambi_scope, lr);
    }
    {
        jobject list(nullptr);
        QtJambiScope __qtjambi_scope;
        (void)qtjambi_cast<const QList<QPair<QString, QString>>&>(env, __qtjambi_scope, list);
        (void)qtjambi_cast<QList<QPair<QString, QString>>>(env, __qtjambi_scope, list);
        (void)qtjambi_cast<QList<QPair<QString, QString>>&>(env, __qtjambi_scope, list);
        (void)qtjambi_cast<QList<QPair<QString, QString>>*>(env, __qtjambi_scope, list);
        (void)qtjambi_cast<const QList<QPair<QString, QString>*>&>(env, __qtjambi_scope, list);
        (void)qtjambi_cast<QList<QPair<QString, QString>*>>(env, __qtjambi_scope, list);
        (void)qtjambi_cast<QList<QPair<QString, QString>*>&>(env, __qtjambi_scope, list);
        (void)qtjambi_cast<QList<QPair<QString, QString>*>*>(env, __qtjambi_scope, list);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        (void)qtjambi_cast<const std::map<QString,QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::map<QString,QString>>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::map<QString,QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::map<QString,QString>*>(env, __qtjambi_scope, obj);
        std::map<QString,QString> map;
        (void)qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        (void)qtjambi_cast<const std::multimap<QString,QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::multimap<QString,QString>>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::multimap<QString,QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::multimap<QString,QString>*>(env, __qtjambi_scope, obj);
        std::multimap<QString,QString> map;
        (void)qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        (void)qtjambi_cast<const std::unordered_map<QString,QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::unordered_map<QString,QString>>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::unordered_map<QString,QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::unordered_map<QString,QString>*>(env, __qtjambi_scope, obj);
        std::unordered_map<QString,QString> map;
        (void)qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        (void)qtjambi_cast<const std::unordered_set<QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::unordered_set<QString>>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::unordered_set<QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::unordered_set<QString>*>(env, __qtjambi_scope, obj);
        std::unordered_set<QString> map;
        (void)qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        (void)qtjambi_cast<const std::set<QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::set<QString>>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::set<QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::set<QString>*>(env, __qtjambi_scope, obj);
        std::set<QString> map;
        (void)qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
        jobject obj(nullptr);
        QtJambiScope __qtjambi_scope;
        (void)qtjambi_cast<const std::multiset<QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::multiset<QString>>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::multiset<QString>&>(env, __qtjambi_scope, obj);
        (void)qtjambi_cast<std::multiset<QString>*>(env, __qtjambi_scope, obj);
        std::multiset<QString> map;
        (void)qtjambi_cast<jobject>(env, __qtjambi_scope, map);
    }
    {
    QString string;
    (void)qtjambi_cast<uchar>(env, string);
    (void)qtjambi_cast<char>(env, string);
    (void)qtjambi_cast<ushort>(env, string);
    (void)qtjambi_cast<short>(env, string);
    (void)qtjambi_cast<uint>(env, string);
    (void)qtjambi_cast<int>(env, string);
    (void)qtjambi_cast<ulong>(env, string);
    (void)qtjambi_cast<long>(env, string);
    (void)qtjambi_cast<qulonglong>(env, string);
    (void)qtjambi_cast<qlonglong>(env, string);
    (void)qtjambi_cast<bool>(env, string);
    (void)qtjambi_cast<float>(env, string);
    (void)qtjambi_cast<double>(env, string);
    (void)qtjambi_cast<jstring>(env, string);
    (void)qtjambi_cast<jobject>(env, string);
    (void)qtjambi_cast<Qt::Orientation>(env, string);
    }
    {
    QString _string;
    QString& string = _string;
    (void)qtjambi_cast<uchar>(env, string);
    (void)qtjambi_cast<char>(env, string);
    (void)qtjambi_cast<ushort>(env, string);
    (void)qtjambi_cast<short>(env, string);
    (void)qtjambi_cast<uint>(env, string);
    (void)qtjambi_cast<int>(env, string);
    (void)qtjambi_cast<ulong>(env, string);
    (void)qtjambi_cast<long>(env, string);
    (void)qtjambi_cast<qulonglong>(env, string);
    (void)qtjambi_cast<qlonglong>(env, string);
    (void)qtjambi_cast<bool>(env, string);
    (void)qtjambi_cast<float>(env, string);
    (void)qtjambi_cast<double>(env, string);
    (void)qtjambi_cast<jstring>(env, string);
    (void)qtjambi_cast<jobject>(env, string);
    }
    {
    QChar c('a');
    (void)qtjambi_cast<uchar>(env, c);
    (void)qtjambi_cast<char>(env, c);
    (void)qtjambi_cast<ushort>(env, c);
    (void)qtjambi_cast<short>(env, c);
    (void)qtjambi_cast<uint>(env, c);
    (void)qtjambi_cast<int>(env, c);
    (void)qtjambi_cast<ulong>(env, c);
    (void)qtjambi_cast<long>(env, c);
    (void)qtjambi_cast<qulonglong>(env, c);
    (void)qtjambi_cast<qlonglong>(env, c);
    (void)qtjambi_cast<bool>(env, c);
    (void)qtjambi_cast<float>(env, c);
    (void)qtjambi_cast<double>(env, c);
    (void)qtjambi_cast<jobject>(env, c);
    }
    {
    QLatin1Char c('a');
    (void)qtjambi_cast<uchar>(env, c);
    (void)qtjambi_cast<char>(env, c);
    (void)qtjambi_cast<ushort>(env, c);
    (void)qtjambi_cast<short>(env, c);
    (void)qtjambi_cast<uint>(env, c);
    (void)qtjambi_cast<int>(env, c);
    (void)qtjambi_cast<ulong>(env, c);
    (void)qtjambi_cast<long>(env, c);
    (void)qtjambi_cast<qulonglong>(env, c);
    (void)qtjambi_cast<qlonglong>(env, c);
    (void)qtjambi_cast<bool>(env, c);
    (void)qtjambi_cast<float>(env, c);
    (void)qtjambi_cast<double>(env, c);
    (void)qtjambi_cast<jobject>(env, c);
    }
    {
    wchar_t c('a');
    (void)qtjambi_cast<uchar>(env, c);
    (void)qtjambi_cast<char>(env, c);
    (void)qtjambi_cast<ushort>(env, c);
    (void)qtjambi_cast<short>(env, c);
    (void)qtjambi_cast<uint>(env, c);
    (void)qtjambi_cast<int>(env, c);
    (void)qtjambi_cast<ulong>(env, c);
    (void)qtjambi_cast<long>(env, c);
    (void)qtjambi_cast<qulonglong>(env, c);
    (void)qtjambi_cast<qlonglong>(env, c);
    (void)qtjambi_cast<bool>(env, c);
    (void)qtjambi_cast<float>(env, c);
    (void)qtjambi_cast<double>(env, c);
    (void)qtjambi_cast<jobject>(env, c);
    }
    jbyte b = 0;
    (void)qtjambi_cast<jobject>(env, b);
    QObject* o(nullptr);
    (void)qtjambi_cast<jobject>(env, o);
    {
    jobject jo(nullptr);
    (void)qtjambi_cast<QObject*>(env, jo);
    (void)qtjambi_cast<QSize>(env, jo);
    (void)qtjambi_cast<QString>(env, jo);
    (void)qtjambi_cast<QList<QSize>>(env, jo);
    (void)qtjambi_cast<QStringList>(env, jo);
    }
    {
        QSize s;
        (void)qtjambi_cast<jobject>(env, s);
    }
    {
        QList<QSize> s;
        (void)qtjambi_cast<jobject>(env, s);
    }
    {
        Qt::Orientation o = Qt::Orientation(0);
        (void)qtjambi_cast<jobject>(env, o);
        (void)qtjambi_cast<int>(env, o);
        (void)qtjambi_cast<QString>(env, o);
    }
    {
        Qt::Orientations o;
        (void)qtjambi_cast<jobject>(env, o);
        (void)qtjambi_cast<int>(env, o);
    }
    {
        int i = 0;
        (void)qtjambi_cast<Qt::Orientation>(env, i);
        (void)qtjambi_cast<Qt::Orientations>(env, i);
    }

    QList<bool> results;
    {
        {
            QList<int> qlist;
            qlist << 4 << 6 << 12;
            jobject o = qtjambi_cast<jobject>(env, qlist);
            results << (qtjambi_cast<QList<int>>(env, o)==qlist);
            results << (qtjambi_cast<const QList<int>>(env, o)==qlist);
            //results << (qtjambi_cast<const QList<int>&>(env, o)==qlist); disallowed
        }
        {
            QStringList qlist;
            qlist << "4" << "6" << "12";
            jobject o = qtjambi_cast<jobject>(env, qlist);

            results << (qtjambi_cast<QStringList>(env, o)==qlist);
            results << (qtjambi_cast<const QStringList>(env, o)==qlist);
            //results << (qtjambi_cast<const QStringList&>(env, o)==qlist); disallowed
        }
        {
            QByteArrayList qlist;
            qlist << "A" << "B" << "C";
            jobject o = qtjambi_cast<jobject>(env, qlist);

            results << (qtjambi_cast<QByteArrayList>(env, o)==qlist);
            results << (qtjambi_cast<const QByteArrayList>(env, o)==qlist);
            //results << (qtjambi_cast<const QByteArrayList&>(env, o)==qlist); // this does not work because of returning a reference to local variable.
        }
        {
            QHash<QString,QFileInfo> qmap;
            qmap["test"] = QFileInfo("test");
            qmap["path"] = QFileInfo("path");
            qmap["hash"] = QFileInfo("hash");
            jobject o = qtjambi_cast<jobject>(env, qmap);

            results << (qtjambi_cast<QHash<QString,QFileInfo>>(env, o)==qmap);
            results << (qtjambi_cast<const QHash<QString,QFileInfo>>(env, o)==qmap);
            //results << (qtjambi_cast<const QHash<QString,QFileInfo>&>(env, o)==qmap);// this does not work because of returning a reference to local variable.
        }
        {
            double d = 2.3;
            jobject o = qtjambi_cast<jobject>(env, d);

            results << qFuzzyCompare(qtjambi_cast<double>(env, o), d);
            results << qFuzzyCompare(qtjambi_cast<const double>(env, o), d);
            //results << qFuzzyCompare(qtjambi_cast<const double&>(env, o), d); disallowed
        }
        {
            int i = 41;
            jobject o = qtjambi_cast<jobject>(env, i);

            results << (qtjambi_cast<int>(env, o) == i);
        }
        {
            jint i = Qt::ExtraButton6;
            Qt::MouseButton o = qtjambi_cast<Qt::MouseButton>(env, i);
            results << (qtjambi_cast<int>(env, o) == i);
        }
        {
            QMap<QString,QFileInfo> qmap;
            qmap["test"] = QFileInfo("test");
            qmap["path"] = QFileInfo("path");
            qmap["map"] = QFileInfo("map");
            jobject o = qtjambi_cast<jobject>(env, qmap);

            results << (qtjambi_cast<QMap<QString,QFileInfo>>(env, o)==qmap);
            results << (qtjambi_cast<const QMap<QString,QFileInfo>>(env, o)==qmap);
            //results << (qtjambi_cast<const QMap<QString,QFileInfo>&>(env, o)==qmap); // this does not work because of returning a reference to local variable.
        }
        {
            jobject o = qtjambi_cast<jobject>(env, &QObject::staticMetaObject);

            results << (qtjambi_cast<const QMetaObject*>(env, o) == &QObject::staticMetaObject);
        }

        {
            jobject o = qtjambi_cast<jobject>(env, QObject::staticMetaObject.property(0));

            results << (qtjambi_cast<QMetaProperty>(env, o).name() == QObject::staticMetaObject.property(0).name());
        }

        {
            QScopedPointer<double> widget(new double(9.876));
            jobject o = qtjambi_cast<jobject>(env, widget);

            results << qFuzzyCompare(qtjambi_cast<double>(env, o), 9.876);
        }

        {
            QBuffer buffer;
            QIODevice& device = buffer;
            jobject o = qtjambi_cast<jobject>(env, device);

            results << (&qtjambi_cast<QIODevice&>(env, o)==&device);
        }

        {
            QList<QObject const*> container;
            container << QCoreApplication::instance();
            container << QAbstractEventDispatcher::instance();
            container << QThread::currentThread();
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            QList<QObject const*>& recast = qtjambi_cast<QList<QObject const*>&>(env, scope, o);
            results << (&recast==&container);
        }

        {
            const QList<QVariant> container{8,7,6,5,4.3f};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QList<QVariant>&>(env, scope, o)==&container);
        }

        {
            QSet<QString> container;
            container << "set";
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<QSet<QString>&>(env, scope, o)==&container);
        }

        {
            const QSet<QObject const*> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QSet<QObject const*>&>(env, scope, o)==&container);
        }

        {
            const QQueue<float> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QQueue<float>&>(env, scope, o)==&container);
        }

        {
            const QVector<float> container{1, 2, 3, 4, 6};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QVector<float>&>(env, scope, o)==&container);
        }

        {
            const QStack<float> container;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QStack<float>&>(env, scope, o)==&container);
        }

        {
            const QMap<float,QVariant> container{{3.987f, "6"}, {8.f, "3"}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QMap<float,QVariant>&>(env, scope, o)==&container);
        }

        {
            QMap<QString,QVariant> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<QMap<QString,QVariant>&>(env, scope, o)==&container);
        }

        {
            const QHash<QString,int> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QHash<QString,int>&>(env, scope, o)==&container);
        }
        {
            const QMultiHash<QString,int> container{{"6", 6}, {"3", 9}};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (&qtjambi_cast<const QMultiHash<QString,int>&>(env, scope, o)==&container);
        }

#if QT_VERSION >= QT_VERSION_CHECK(6,7,0)
        {
            jintArray array{nullptr};
            (void)qtjambi_cast<QSpan<int>>(env, scope, array);

            (void)qtjambi_cast<QSpan<const int>>(env, scope, jobject{nullptr});
            (void)qtjambi_cast<QSpan<QStringView>>(env, scope, jobject{nullptr});
            (void)qtjambi_cast<QSpan<char>>(env, scope, jobject{nullptr});
            (void)qtjambi_cast<QSpan<const char>>(env, scope, jobject{nullptr});

            QSpan<int> span;
            (void)qtjambi_cast<jintArray>(env, scope, span);
        }
        {
            jintArray array{nullptr};
            (void)qtjambi_cast<QSpan<const int>>(env, scope, array);

            QSpan<const int> span;
            (void)qtjambi_cast<jintArray>(env, span);
        }
        {
            jintArray array{nullptr};
            (void)qtjambi_cast<QSpan<const int>>(env, scope, array);

            QSpan<char> span;
            (void)qtjambi_cast<jobject>(env, span);
        }
#endif
        {
            std::optional<int> opt1;
            (void)qtjambi_cast<jobject>(env, opt1);
            std::optional<QString> opt2;
            (void)qtjambi_cast<jobject>(env, opt2);
            std::optional<char> opt3;
            (void)qtjambi_cast<jobject>(env, opt3);
            jobject o{nullptr};
            (void)qtjambi_cast<std::optional<int>>(env, o);
            (void)qtjambi_cast<std::optional<QString>>(env, o);
            (void)qtjambi_cast<std::optional<char>>(env, o);
        }

        {
            std::initializer_list<int> container{1,2,3,4,5,6,7,8,9};
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            (void)qtjambi_cast<std::initializer_list<int>>(env, scope, jobject{nullptr});

            std::initializer_list<int> container2 = qtjambi_cast<std::initializer_list<int>>(env, scope, o);
            results << (QList<int>(container2)==QList<int>(container));
        }
        {
            std::initializer_list<QList<QMap<QString,int>>> container{
                /*QList*/{
                    /*QMap*/{
                        {"A", 5}, {"B", 123}, {"C", 9}
                    },
                    /*QMap*/{
                        {"Y", 6}, {"Z", 39}
                    }
                },
                /*QList*/{}
            };
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            (void)qtjambi_cast<std::initializer_list<QList<QMap<QString,int>>>>(env, scope, o);
        }

        {
            QStringList stringList{"A", "B", "C", "D"};
            QStringList* container = &stringList;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            results << (qtjambi_cast<QStringList*>(env, scope, o)==container);
        }

        {
            QSharedPointer<QHash<QString,QString>> ptr(new QHash<QString,QString>{ {"A", "B"} });
            QSharedPointer<QHash<QString,QString>>* container = &ptr;
            jobject o = qtjambi_cast<jobject>(env, scope, container);

            QSharedPointer<QHash<QString,QString>>* ptr2 = qtjambi_cast<QSharedPointer<QHash<QString,QString>>*>(env, scope, o);
            results << (ptr2!=nullptr);
            if(ptr2){
                results << (ptr2->data()==ptr.data());
            }
        }

        {
            Qt::Orientation enm = Qt::Horizontal;
            (void)qtjambi_cast<int>(enm);
            (void)qtjambi_cast<int>(Qt::Horizontal);
            (void)qtjambi_cast<QString>(enm);
            (void)qtjambi_cast<QEvent::Type>(enm);
            int i = 5;
            (void)qtjambi_cast<QString>(i);
            (void)qtjambi_cast<double>(i);
            (void)qtjambi_cast<Qt::Orientation>(i);
            (void)qtjambi_cast<Qt::Orientation>(0);
        }

        if(QString("The following lines are for compilation only.").isEmpty()){
            /*{
                QList<const int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QList<const int>>(env, scope, o);
            }*/
            {
                QList<int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QList<int>>(env, scope, o);
            }
            {
                QList<QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QList<QObject*>>(env, scope, o);
            }
            {
                QList<QList<int>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QList<QList<int>>>(env, scope, o);
            }
            {
                QList<QList<QObject*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QList<QList<QObject*>>>(env, scope, o);
            }
            {
                QMap<int, QRect>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMap<int, QRect>>(env, scope, o);
            }
            {
                QMap<int, QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMap<int, QObject*>>(env, scope, o);
            }
            {
                QMap<QObject*,int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMap<QObject*,int>>(env, scope, o);
            }
            {
                QMap<QObject*,QRunnable*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMap<QObject*,QRunnable*>>(env, scope, o);
            }
            {
                QMap<QObject*,QList<QRunnable*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMap<QObject*,QList<QRunnable*>>>(env, scope, o);
            }
            {
                QMultiMap<int, QRect>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiMap<int, QRect>>(env, scope, o);
            }
            {
                QMultiMap<int, QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiMap<int, QObject*>>(env, scope, o);
            }
            {
                QMultiMap<QObject*,int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiMap<QObject*,int>>(env, scope, o);
            }
            {
                QMultiMap<QObject*,QRunnable*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiMap<QObject*,QRunnable*>>(env, scope, o);
            }
            {
                QMultiMap<QObject*,QList<QRunnable*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiMap<QObject*,QList<QRunnable*>>>(env, scope, o);
            }
            {
                QHash<int, QRect>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QHash<int, QRect>>(env, scope, o);
            }
            {
                QHash<int, QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QHash<int, QObject*>>(env, scope, o);
            }
            {
                QHash<QObject*,int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QHash<QObject*,int>>(env, scope, o);
            }
            {
                QHash<QObject*,QRunnable*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QHash<QObject*,QRunnable*>>(env, scope, o);
            }
            {
                QHash<QObject*,QList<QRunnable*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QHash<QObject*,QList<QRunnable*>>>(env, scope, o);
            }
            {
                QMultiHash<int, QRect>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiHash<int, QRect>>(env, scope, o);
            }
            {
                QMultiHash<int, QObject*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiHash<int, QObject*>>(env, scope, o);
            }
            {
                QMultiHash<QObject*,int>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiHash<QObject*,int>>(env, scope, o);
            }
            {
                QMultiHash<QObject*,QRunnable*>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiHash<QObject*,QRunnable*>>(env, scope, o);
            }
            {
                QMultiHash<QObject*,QList<QRunnable*>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiHash<QObject*,QList<QRunnable*>>>(env, scope, o);
            }
            /*
            {
                QMap<UnknownKey, UnknownClass>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QMap<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                QHash<UnknownKey,UnknownClass>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QHash<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                const QMap<UnknownKey, UnknownClass>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<const QMap<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                const QHash<UnknownKey,UnknownClass>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<const QHash<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                QMultiMap<UnknownKey, UnknownClass>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QMultiMap<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                QMultiHash<UnknownKey,UnknownClass>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QMultiHash<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                const QMultiMap<UnknownKey, UnknownClass>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<const QMultiMap<UnknownKey, UnknownClass>>(env, scope, o);
            }

            {
                const QMultiHash<UnknownKey,UnknownClass>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QMultiHash<UnknownKey, UnknownClass>>(env, scope, o);
            }
            {
                QSharedPointer<QMap<UnknownKey, UnknownClass>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QSharedPointer<QMap<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QHash<UnknownKey,UnknownClass>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QSharedPointer<QHash<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMap<UnknownKey, UnknownClass>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QSharedPointer<const QMap<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QHash<UnknownKey,UnknownClass>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QSharedPointer<const QHash<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QMultiMap<UnknownKey, UnknownClass>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QSharedPointer<QMultiMap<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QMultiHash<UnknownKey,UnknownClass>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QSharedPointer<QMultiHash<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiMap<UnknownKey, UnknownClass>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QSharedPointer<const QMultiMap<UnknownKey,UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiHash<UnknownKey,UnknownClass>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
//                (void)qtjambi_cast<QSharedPointer<const QMultiHash<UnknownKey,UnknownClass>>>(env, scope, o);
            }
*/
            {
                QList<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QList<UnknownClass>>(env, scope, o);
            }

            {
                QQueue<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QQueue<UnknownClass>>(env, scope, o);
            }

            {
                QStack<UnknownKey>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QStack<UnknownClass>&>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QVector<UnknownKey>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QVector<UnknownClass>&>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSet<UnknownClass>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSet<UnknownClass>>(env, scope, o);
            }

            {
                const QList<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QList<UnknownClass>>(env, scope, o);
            }

            {
                const QQueue<UnknownKey>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QQueue<UnknownClass>>(env, scope, o);
            }

            {
                const QStack<UnknownKey>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<const QStack<UnknownClass>&>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                const QVector<UnknownKey>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<const QVector<UnknownClass>&>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                const QSet<UnknownClass>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QSet<UnknownClass>>(env, scope, o);
            }

            {
                QSharedPointer<QList<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QList<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QQueue<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QQueue<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<QStack<UnknownKey>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QSharedPointer<QStack<UnknownClass>>>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSharedPointer<QVector<UnknownKey>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QSharedPointer<QVector<UnknownClass>>>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSharedPointer<QSet<UnknownClass>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QSet<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QList<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QList<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QQueue<UnknownKey>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QQueue<UnknownClass>>>(env, scope, o);
            }

            {
                QSharedPointer<const QStack<UnknownKey>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QSharedPointer<const QStack<UnknownClass>>>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSharedPointer<const QVector<UnknownKey>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, *container);
                //qtjambi_cast<QSharedPointer<const QVector<UnknownClass>>>(env, scope, o); // Cannot cast to QVector<T> because T does not have a standard constructor.
            }

            {
                QSharedPointer<const QSet<UnknownClass>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QSet<UnknownClass>>>(env, scope, o);
            }

            {
                QMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMap<QString, QString>>(env, scope, o);
            }

            {
                QHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QHash<QString, QString>>(env, scope, o);
            }

            {
                const QMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QMap<QString, QString>>(env, scope, o);
            }

            {
                const QHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QHash<QString, QString>>(env, scope, o);
            }

            {
                QMultiMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiMap<QString, QString>>(env, scope, o);
            }

            {
                QMultiHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiHash<QString, QString>>(env, scope, o);
            }

            {
                const QMultiMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QMultiMap<QString, QString>>(env, scope, o);
            }

            {
                const QMultiHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QMultiHash<QString, QString>>(env, scope, o);
            }
            {
                QSharedPointer<QMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QMap<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QHash<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QMap<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QHash<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QMultiMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QMultiMap<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QMultiHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QMultiHash<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QMultiMap<QString,QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QMultiHash<QString,QString>>>(env, scope, o);
            }

            {
                QList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QList<QString>>(env, scope, o);
            }

            {
                QQueue<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QQueue<QString>>(env, scope, o);
            }

            {
                QStack<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QStack<QString>>(env, scope, o);
            }

            {
                QVector<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QVector<QString>>(env, scope, o);
            }

            {
                QSet<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSet<QString>>(env, scope, o);
            }

            {
                const QList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QList<QString>>(env, scope, o);
            }

            {
                const QQueue<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QQueue<QString>>(env, scope, o);
            }

            {
                const QStack<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QStack<QString>>(env, scope, o);
            }

            {
                const QVector<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QVector<QString>>(env, scope, o);
            }

            {
                const QSet<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<const QSet<QString>>(env, scope, o);
            }

            {
                QSharedPointer<QList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QList<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QQueue<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QQueue<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QStack<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QStack<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QVector<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QVector<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<QSet<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QList<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QQueue<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QQueue<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QStack<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QStack<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QVector<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QVector<QString>>>(env, scope, o);
            }

            {
                QSharedPointer<const QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QSharedPointer<const QSet<QString>>>(env, scope, o);
            }

            {
                QWeakPointer<const QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, *container);
                (void)qtjambi_cast<QWeakPointer<const QSet<QString>>>(env, scope, o);
            }

            {
                QMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QMap<QString, QString>*>(env, scope, o);
            }

            {
                QHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QHash<QString, QString>*>(env, scope, o);
            }

            {
                const QMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<const QMap<QString, QString>*>(env, scope, o);
            }

            {
                const QHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<const QHash<QString, QString>*>(env, scope, o);
            }

            {
                QMultiMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QMultiMap<QString, QString>*>(env, scope, o);
            }

            {
                QMultiHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QMultiHash<QString, QString>*>(env, scope, o);
            }

            {
                const QMultiMap<QString, QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<const QMultiMap<QString, QString>*>(env, scope, o);
            }

            {
                const QMultiHash<QString,QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QMultiHash<QString, QString>*>(env, scope, o);
            }
            {
                QSharedPointer<QMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QMap<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QHash<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QMap<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QHash<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QMultiMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QMultiMap<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QMultiHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QMultiHash<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiMap<QString, QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QMultiMap<QString,QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QMultiHash<QString,QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QMultiHash<QString,QString>>*>(env, scope, o);
            }

            {
                QList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QList<QString>*>(env, scope, o);
            }

            {
                QQueue<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QQueue<QString>*>(env, scope, o);
            }

            {
                QStack<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QStack<QString>*>(env, scope, o);
            }

            {
                QVector<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QVector<QString>*>(env, scope, o);
            }

            {
                QSet<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSet<QString>*>(env, scope, o);
            }

            {
                const QList<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<const QList<QString>*>(env, scope, o);
            }

            {
                const QQueue<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<const QQueue<QString>*>(env, scope, o);
            }

            {
                const QStack<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<const QStack<QString>*>(env, scope, o);
            }

            {
                const QVector<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<const QVector<QString>*>(env, scope, o);
            }

            {
                const QSet<QString>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<const QSet<QString>*>(env, scope, o);
            }

            {
                QSharedPointer<QList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QList<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QQueue<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QQueue<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QStack<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QStack<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QVector<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QVector<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<QSet<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QList<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QList<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QQueue<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QQueue<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QStack<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QStack<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QVector<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QVector<QString>>*>(env, scope, o);
            }

            {
                QSharedPointer<const QSet<QString>>* container = nullptr;
                jobject o = qtjambi_cast<jobject>(env, scope, container);
                (void)qtjambi_cast<QSharedPointer<const QSet<QString>>*>(env, scope, o);
            }

            {
                QWeakPointer<const QSet<QString>>* container = nullptr;
                (void)qtjambi_cast<jobject>(env, scope, container);
                //qtjambi_cast<QWeakPointer<const QSet<QString>>*>(env, scope, o); //  Cannot cast to QWeakPointer<T> *
            }
            {
                QSettings::ReadFunc fn = [](QIODevice &, QSettings::SettingsMap &)->bool{return false;};
                jobject o = qtjambi_cast<jobject>(env, fn, "QSettings::ReadFunc");
                (void)qtjambi_cast<QSettings::ReadFunc>(env, o);
            }
            {
                std::function<bool(QIODevice &, QSettings::SettingsMap &)> fn = [](QIODevice &, QSettings::SettingsMap &)->bool{return false;};
                jobject o = qtjambi_cast<jobject>(env, fn);
                (void)qtjambi_cast<std::function<bool(QIODevice &, QSettings::SettingsMap &)>>(env, o);
            }
            {
                EnumClass ec = EnumClass::None;
                jobject o = qtjambi_cast<jobject>(env, ec);
                (void)qtjambi_cast<EnumClass>(env, o);
            }
        }
    }
}
#endif
