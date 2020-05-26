#ifndef QTJAMBI_QML_H
#define QTJAMBI_QML_H

#include <QtCore/QtCore>

typedef QObject* (* CreateQmlErrorDummyObject) (const QMetaObject* metaObject, void* placement, int vsCast, int viCast);

QTJAMBI_EXPORT void registerCreateQmlErrorDummyObjectFunction(CreateQmlErrorDummyObject createQmlErrorDummyObject);

QTJAMBI_EXPORT QObject* createQmlErrorDummyObject(const QMetaObject* metaObject, void* placement, int vsCast, int viCast);

QTJAMBI_EXPORT jclass qtjambi_find_generated_superclass(JNIEnv * env, jclass clazz);

QTJAMBI_EXPORT jobject qtjambi_find_QmlAttachedProperties(JNIEnv * env, jclass clazz);

#endif // QTJAMBI_QML_H
