/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>

#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#include <QtCore/qpropertyprivate.h>
#endif

struct BigIntegerValue{
    quint64 value;
    bool isNegative;
    bool outOfRange;
};

BigIntegerValue fromBigInteger(JNIEnv* env, jobject object);
jobject toBigInteger(JNIEnv* env, quint64 value, bool isNegative);

QObject* qtjambi_findChild(JNIEnv *env, const QObject *__qt_this, const QString& aName, Qt::FindChildOptions options);
QObject* qtjambi_findChild(JNIEnv *env, const QObject *__qt_this, jclass type, const QString& aName, Qt::FindChildOptions options);
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
QList<QObject*> qtjambi_findChildren(JNIEnv *env, const QObject *__qt_this, Qt::FindChildOptions options);
QList<QObject*> qtjambi_findChildren(JNIEnv *env, const QObject *__qt_this, jclass type, Qt::FindChildOptions options);
#endif
QList<QObject*> qtjambi_findChildren(JNIEnv *env, const QObject *__qt_this, const QString& aName, Qt::FindChildOptions options);
QList<QObject*> qtjambi_findChildren(JNIEnv *env, const QObject *__qt_this, jclass type, const QString& aName, Qt::FindChildOptions options);
QList<QObject*> qtjambi_findChildren(JNIEnv *env, const QObject *__qt_this, const QRegularExpression& regexp, Qt::FindChildOptions options);
QList<QObject*> qtjambi_findChildren(JNIEnv *env, const QObject *__qt_this, jclass type, const QRegularExpression& regexp, Qt::FindChildOptions options);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
QList<QObject*> qtjambi_findChildren(JNIEnv *env, const QObject *__qt_this, const QRegExp& regexp, Qt::FindChildOptions options);
QList<QObject*> qtjambi_findChildren(JNIEnv *env, const QObject *__qt_this, jclass type, const QRegExp& regexp, Qt::FindChildOptions options);
#endif

namespace Java{
namespace QtCore
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QUnhandledException,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDebug,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(__rcDevice)
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_FIELD(disabled))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCborStreamReader$StringResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCborValue$FromCborResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDate$Week,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QJsonDocument$FromJsonResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSharedMemory,
                                     QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(__qt_accessMode))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLockFile$LockInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QRunnable,)
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFile$TrashResult,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QByteProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoubleProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_DOUBLE_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatProperty,
                                     QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(value)
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setValue)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(binding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(setBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(makeBinding)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(bindingData)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(analyzeMetaType)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBytePropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoublePropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanPropertyBinding,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBytePropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoublePropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanPropertyAlias,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QByteBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoubleBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanBindable,
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(getValueBypassingBindings)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(valueMetaType)
                                     )
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QIntPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QShortPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBytePropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QLongPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QDoublePropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFloatPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QBooleanPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QCharPropertyData,
                                     QTJAMBI_REPOSITORY_DECLARE_BOOLEAN_METHOD(setValueBypassingBindings))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyChangeHandler,
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QPropertyNotifier,
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(invoke))
#endif
}

#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(M) QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(M)
#else
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(M)
#endif


namespace QtGui
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGuiApplication,
                                    QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(exec)
                                    QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(resolveInterface)
                                    )
}

namespace QtWidgets
{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QWidget,
                                 QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(setParent))
}

namespace QtJambi
{
QTJAMBI_REPOSITORY_DECLARE_CLASS(ReflectionUtility,
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(invocationInfoProvider))
QTJAMBI_REPOSITORY_DECLARE_CLASS(InternalAccess$CallerContext,
                                  QTJAMBI_REPOSITORY_DECLARE_CLASS_FIELD(declaringClass)
                                  QTJAMBI_REPOSITORY_DECLARE_STRING_FIELD(methodName)
                                  QTJAMBI_REPOSITORY_DECLARE_INT_FIELD(lineNumber))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities$ShortSupplier,
                                 QTJAMBI_REPOSITORY_DECLARE_SHORT_METHOD(getAsShort))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities$ByteSupplier,
                                 QTJAMBI_REPOSITORY_DECLARE_BYTE_METHOD(getAsByte))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities$FloatSupplier,
                                 QTJAMBI_REPOSITORY_DECLARE_FLOAT_METHOD(getAsFloat))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtUtilities$CharSupplier,
                                 QTJAMBI_REPOSITORY_DECLARE_CHAR_METHOD(getAsChar))
QTJAMBI_REPOSITORY_DECLARE_CLASS(QUnsuccessfulInvocationException,
              QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())
QTJAMBI_REPOSITORY_DECLARE_CLASS(QFlags,)
}

namespace Runtime{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(BigInteger,
                  QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(abs)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(signum)
                  QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(bitLength)
                  QTJAMBI_REPOSITORY_DECLARE_LONG_METHOD(longValue)
                  QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(toByteArray))
}
}

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
QtPrivate::QPropertyObserverCallback qtjambi_get_signal_callback(JNIEnv *, QUntypedPropertyData *);
#endif

#if defined(QT_NO_DEBUG)
#define QTJAMBI_CONTAINER_CAST(Type, target, source) Type* target = static_cast<Type*>(source)
#else
#define QTJAMBI_CONTAINER_CAST(Type, target, source) Type* target = dynamic_cast<Type*>(source);\
Q_ASSERT(target)
#endif

#endif // QTJAMBICORE_UTILS_H
