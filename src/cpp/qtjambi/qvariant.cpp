/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#include "qtjambilink.h"
#include "qtjambi_core.h"
#include "qtjambitypemanager_p.h"

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
#include <QtCore/QRegExp>
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QStringList>
#include <QtCore/QLocale>
#include <QtCore/QChar>
#include <QtCore/QBitArray>
#include <QtCore/QByteArray>

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QVariant__1_1qt_1convert)
    (JNIEnv *env, jclass, jint type, jobject object)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    if (v.convert(QVariant::Type(type)))
        return qtjambi_from_qvariant(env, v);
    else
        return 0;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QVariant__1_1qt_1canConvert)
    (JNIEnv *env, jclass, jobject obj, jint type)
{
    QVariant v = qtjambi_to_qvariant(env, obj);
    return v.canConvert(QVariant::Type(type));
}

static inline void setOk(JNIEnv *env, jobjectArray ok, bool isOk)
{
    if (!env->IsSameObject(ok, 0) && env->GetArrayLength(ok)) {
        StaticCache *sc = StaticCache::instance();
        sc->resolveBoolean();
        jfieldID fieldId = isOk ? sc->Boolean.field_TRUE : sc->Boolean.field_FALSE;
        jobject boolObject = env->GetStaticObjectField(sc->Boolean.class_ref, fieldId);
        env->SetObjectArrayElement(ok, 0, boolObject);
    }
}

extern "C" Q_DECL_EXPORT jdouble JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QVariant__1_1qt_1toDouble)
    (JNIEnv *env, jclass, jobject object, jobjectArray ok)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    bool isOk = false;
    double returned = v.toDouble(&isOk);
    setOk(env, ok, isOk);
    return returned;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QVariant__1_1qt_1toInt)
    (JNIEnv *env, jclass, jobject object, jobjectArray ok)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    bool isOk = false;
    int returned = v.toInt(&isOk);
    setOk(env, ok, isOk);
    return returned;
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QVariant__1_1qt_1toLong)
    (JNIEnv *env, jclass, jobject object, jobjectArray ok)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    bool isOk = false;
    qlonglong returned = v.toLongLong(&isOk);
    setOk(env, ok, isOk);
    return returned;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QVariant__1_1qt_1toString)
    (JNIEnv *env, jclass, jobject object)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    return qtjambi_from_qstring(env, v.toString());
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QVariant__1_1qt_1toBoolean)
    (JNIEnv *env, jclass, jobject object)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    return v.toBool();
}

extern "C" Q_DECL_EXPORT jchar JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_QVariant__1_1qt_1toChar)
    (JNIEnv *env, jclass, jobject object)
{
    QVariant v = qtjambi_to_qvariant(env, object);
    QChar c = v.toChar();
    return c.unicode();
}
