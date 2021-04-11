/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

#include <QtCore/QVariant>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QSize>
#include <QtCore/QSizeF>
#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QLine>
#include <QtCore/QLineF>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QRegExp>
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QStringList>
#include <QtCore/QLocale>
#include <QtCore/QChar>
#include <QtCore/QBitArray>
#include <QtCore/QByteArray>
#include <QtCore/QDataStream>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define META_TYPE(t) t
#else
#define META_TYPE(t) QMetaType(t)
#endif

static inline void setOk(JNIEnv *env, jbooleanArray ok, bool isOk)
{
    if (!env->IsSameObject(ok, nullptr) && env->GetArrayLength(ok)) {
        jboolean* array = env->GetBooleanArrayElements(ok, nullptr);
        array[0] = isOk;
        env->ReleaseBooleanArrayElements(ok, array, 0);
        qtjambi_throw_java_exception(env)
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant_convert)
    (JNIEnv *env, jclass, jobject object, jint type, jbooleanArray ok)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    if(v.userType()==int(type)){
        setOk(env, ok, true);
        return object;
    }else if (v.convert(META_TYPE(int(type)))){
        setOk(env, ok, true);
        return qtjambi_from_qvariant(env, v);
    }else{
        setOk(env, ok, false);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant_canConvertByType)
    (JNIEnv *, jclass, jint type1, jint type2)
{
    return int(type1)==int(type2) || QVariant(META_TYPE(int(type1)), nullptr).canConvert(META_TYPE(int(type2)));
}

extern "C" Q_DECL_EXPORT jdouble JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1toDouble)
    (JNIEnv *env, jclass, jobject object, jbooleanArray ok)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    bool isOk = false;
    double returned = v.toDouble(&isOk);
    setOk(env, ok, isOk);
    return returned;
}

extern "C" Q_DECL_EXPORT jfloat JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1toFloat)
    (JNIEnv *env, jclass, jobject object, jbooleanArray ok)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    bool isOk = false;
    float returned = v.toFloat(&isOk);
    setOk(env, ok, isOk);
    return returned;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1toInt)
    (JNIEnv *env, jclass, jobject object, jbooleanArray ok)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    bool isOk = false;
    int returned = v.toInt(&isOk);
    setOk(env, ok, isOk);
    return returned;
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1toLong)
    (JNIEnv *env, jclass, jobject object, jbooleanArray ok)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    bool isOk = false;
    qlonglong returned = v.toLongLong(&isOk);
    setOk(env, ok, isOk);
    return returned;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1toString)
    (JNIEnv *env, jclass, jobject object)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    return qtjambi_from_qstring(env, v.toString());
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1toBoolean)
    (JNIEnv *env, jclass, jobject object)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    return v.toBool();
}

extern "C" Q_DECL_EXPORT jchar JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1toChar)
    (JNIEnv *env, jclass, jobject object)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    QChar c = v.toChar();
    return c.unicode();
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1saveObject)
    (JNIEnv *env, jclass, QtJambiNativeID stream_nativeId, jobject variant, jbooleanArray ok)
{
    QDataStream *__qt_stream = qtjambi_object_from_nativeId<QDataStream>(stream_nativeId);
    QVariant v = qtjambi_to_qvariant(env, variant);

    quint32 typeId = quint32(v.type());
    bool fakeUserType = false;
    if (__qt_stream->version() < QDataStream::Qt_5_0) {
        if (typeId == QMetaType::User) {
            typeId = 127; // QVariant::UserType had this value in Qt4
        } else if (typeId >= 128 - 97 && typeId <= QVariant::LastCoreType) {
            // In Qt4 id == 128 was FirstExtCoreType. In Qt5 ExtCoreTypes set was merged to CoreTypes
            // by moving all ids down by 97.
            typeId += 97;
        } else if (typeId == QMetaType::QSizePolicy) {
            typeId = 75;
        } else if (typeId >= QMetaType::QKeySequence && typeId <= QMetaType::QQuaternion) {
            // and as a result these types received lower ids too
            typeId +=1;
        } else if (typeId == QMetaType::QPolygonF) {
            // This existed in Qt 4 only as a custom type
            typeId = 127;
            fakeUserType = true;
        }
    }
    *__qt_stream << typeId;
    if (__qt_stream->version() >= QDataStream::Qt_4_2)
        *__qt_stream << qint8(v.data_ptr().is_null);
    if (v.type() >= QVariant::UserType || fakeUserType) {
        *__qt_stream << QMetaType::typeName(v.userType());
    }

    bool isOk = false;
    if(!v.isValid()){
        if (__qt_stream->version() < QDataStream::Qt_5_0)
            *__qt_stream << QString();
        return;
    }else if(v.type()==QVariant::UserType){
        isOk = QMetaType::save(*__qt_stream, v.userType(), v.data());
    }else{
        isOk = QMetaType::save(*__qt_stream, v.type(), v.data());
    }
    setOk(env, ok, isOk);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QVariant__1_1qt_1loadObject)
    (JNIEnv *env, jclass, QtJambiNativeID stream_nativeId, jbooleanArray ok)
{
    QDataStream *__qt_stream = qtjambi_object_from_nativeId<QDataStream>(stream_nativeId);
    bool isOk = false;
    quint32 typeId;
    *__qt_stream >> typeId;
    if (__qt_stream->version() < QDataStream::Qt_5_0) {
        if (typeId == 127 /* QVariant::UserType */) {
            typeId = QMetaType::User;
        } else if (typeId >= 128 && typeId != QVariant::UserType) {
            // In Qt4 id == 128 was FirstExtCoreType. In Qt5 ExtCoreTypes set was merged to CoreTypes
            // by moving all ids down by 97.
            typeId -= 97;
        } else if (typeId == 75 /* QSizePolicy */) {
            typeId = QMetaType::QSizePolicy;
        } else if (typeId > 75 && typeId <= 86) {
            // and as a result these types received lower ids too
            // QKeySequence QPen QTextLength QTextFormat QMatrix QTransform QMatrix4x4 QVector2D QVector3D QVector4D QQuaternion
            typeId -=1;
        }
    }

    qint8 is_null = false;
    if (__qt_stream->version() >= QDataStream::Qt_4_2)
        *__qt_stream >> is_null;
    if (typeId == QVariant::UserType) {
        QByteArray name;
        *__qt_stream >> name;
        typeId = quint32(QMetaType::type(name.constData()));
        if (typeId == QMetaType::UnknownType) {
            __qt_stream->setStatus(QDataStream::ReadCorruptData);
            setOk(env, ok, false);
            return nullptr;
        }
    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QVariant v(int(typeId), nullptr);
#else
    QVariant v(QMetaType(int(typeId)), nullptr);
#endif
    v.data_ptr().is_null = uint(is_null);

    if (!v.isValid()) {
        if (__qt_stream->version() < QDataStream::Qt_5_0) {
        // Since we wrote something, we should read something
            QString x;
            *__qt_stream >> x;
        }
        v.data_ptr().is_null = true;
        setOk(env, ok, true);
        return nullptr;
    }

    // const cast is safe since we operate on a newly constructed variant
    isOk = QMetaType::load(*__qt_stream, int(typeId), const_cast<void *>(v.constData()));
    if (!isOk) {
        __qt_stream->setStatus(QDataStream::ReadCorruptData);
    }
    setOk(env, ok, isOk);
    v.data_ptr().is_null = !isOk;
    return qtjambi_from_qvariant(env, v);
}
