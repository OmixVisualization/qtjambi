/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtJambi/QtJambiAPI>
#include <QtJambiQml/hashes.h>
#include <QtJambi/JavaAPI>

namespace Java{
namespace QtJambi {
QTJAMBI_REPOSITORY_DECLARE_CLASS(QtMetaType,
                                 QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(type)
                                 QTJAMBI_REPOSITORY_DECLARE_STRING_METHOD(name)
                                 QTJAMBI_REPOSITORY_DECLARE_INT_METHOD(id))
QTJAMBI_REPOSITORY_DECLARE_CLASS(ClassAnalyzerUtility,
                                 QTJAMBI_REPOSITORY_DECLARE_STATIC_CLASS_METHOD(findGeneratedSuperclass))
}

namespace QtQml {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlListProperty,)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlExpression$ValueIsUndefined,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlIncubationController$WhileFlag,
                                     QTJAMBI_REPOSITORY_DECLARE_LONG_FIELD(flag))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlListProperty$ReplaceFunction,
                                     QTJAMBI_REPOSITORY_DECLARE_VOID_METHOD(accept))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQmlListProperty$AtFunction,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(apply))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QJSValue,)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlTypeRegistrationException,
                                     QTJAMBI_REPOSITORY_DECLARE_THROWABLE_CONSTRUCTOR())
    namespace Util{
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlSingleton,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlElement,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlNamedElement,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlValueType,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlUncreatable,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlUnavailable,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlExtended,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(value))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlAnonymous,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlInterface,)
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlForeign,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(metaType))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlAttached,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(value))
        QTJAMBI_REPOSITORY_DECLARE_CLASS(QmlSequencialContainer,
                                         QTJAMBI_REPOSITORY_DECLARE_CLASS_METHOD(value)
                                         QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(valueType))
    }
}
}

QObject* qtjambi_qmlAttachedPropertiesObject(JNIEnv *env, jclass clazz, const QObject* obj, bool create);

int qtjambi_qmlRegisterAnonymousSequentialContainer(JNIEnv *env, jobject containerType, const char* uri, int versionMajor);

int qtjambi_qmlRegisterAnonymousType(JNIEnv *env, jclass clazz, const char* uri, int versionMajor);

int qtjambi_qmlRegisterAnonymousType(JNIEnv *env, jclass clazz, int metaObjectRevisionMinor, const char* uri, int versionMajor);

void qtjambi_qmlRegisterAnonymousTypesAndRevisions(JNIEnv *env, jclass clazz, const char* uri, int versionMajor);

int qtjambi_qmlRegisterExtendedType(JNIEnv *env, jclass clazz, jclass extendedClazz);

int qtjambi_qmlRegisterExtendedType(JNIEnv *env, jclass clazz, jclass extendedClazz, const char* uri, int versionMajor);

int qtjambi_qmlRegisterExtendedType(JNIEnv *env, jclass clazz, jclass extendedClazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName);

int qtjambi_qmlRegisterExtendedUncreatableType(JNIEnv *env, jclass clazz, jclass extendedClazz, const char* uri, int versionMajor, int versionMinor, const char* qmlName, QString reason);

int qtjambi_qmlRegisterExtendedUncreatableType(JNIEnv *env, jclass clazz, jclass extendedClazz, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName, QString reason);

int qtjambi_qmlRegisterInterface(JNIEnv *env, jclass clazz, const char* uri);

int qtjambi_qmlRegisterInterface(JNIEnv *env, jclass clazz, const char* uri, int versionMajor);

int qtjambi_qmlRegisterSingletonType(JNIEnv *env, const char* uri, int versionMajor, int versionMinor, const char* typeName, const QtQml::ValueCallback& callback);

int qtjambi_qmlRegisterSingletonType(JNIEnv *env, jclass type, const char* uri, int versionMajor, int versionMinor, const char* typeName, const QtQml::ObjectCallback& callback);

int qtjambi_qmlRegisterRevision(JNIEnv *env, jclass type, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor);

int qtjambi_qmlRegisterType(JNIEnv *env, jclass type);

int qtjambi_qmlRegisterType(JNIEnv *env, jclass type, const char* uri, int versionMajor, int versionMinor, const char* qmlName);

int qtjambi_qmlRegisterType(JNIEnv *env, jclass type, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName);

int qtjambi_qmlRegisterUncreatableType(JNIEnv *env, jclass type, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QString& reason);

int qtjambi_qmlRegisterUncreatableType(JNIEnv *env, jclass type, int metaObjectRevision, const char* uri, int versionMajor, int versionMinor, const char* qmlName, const QString& reason);

void qtjambi_qmlRegisterTypesAndRevisions(JNIEnv *env, jobjectArray types, const char* uri, int versionMajor, QList<int>* qmlTypeIds = nullptr);

jobject qtjambi_fromScriptValue(JNIEnv *__jni_env, QJSEngine *__qt_this, jobject type, const QJSValue& __qt_value0);

QJSValue qtjambi_toScriptValue(JNIEnv *__jni_env, QJSEngine *__qt_this, const QVariant& value);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
jobject qtjambi_fromManagedValue(JNIEnv *env, QJSEngine *, jobject type, const QJSManagedValue& value);
QJSManagedValue qtjambi_toManagedValue(JNIEnv *, QJSEngine *__qt_this, const QVariant& variant);
#if QT_VERSION >= QT_VERSION_CHECK(6, 3, 0)
jobject qtjambi_fromVariant(JNIEnv *env, QJSEngine *, jobject type, const QVariant& value);
#endif
#endif
#endif

#endif // UTILS_P_H
