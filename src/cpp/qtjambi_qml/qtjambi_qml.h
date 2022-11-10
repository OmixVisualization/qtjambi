#ifndef QTJAMBI_QML_H
#define QTJAMBI_QML_H

#include <qtjambi/qtjambi_global.h>
#include "qqmllistproperty.h"

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

#endif // QTJAMBI_QML_H
