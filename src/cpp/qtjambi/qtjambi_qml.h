#ifndef QTJAMBI_QML_H
#define QTJAMBI_QML_H

#include "qtjambi_global.h"

class QObject;
struct QMetaObject;

typedef QObject* (* CreateQmlErrorDummyObject) (const QMetaObject* metaObject, void* placement, int vsCast, int viCast);

QTJAMBI_EXPORT void registerCreateQmlErrorDummyObjectFunction(CreateQmlErrorDummyObject createQmlErrorDummyObject);

QTJAMBI_EXPORT QObject* createQmlErrorDummyObject(const QMetaObject* metaObject, void* placement, int vsCast, int viCast);

QTJAMBI_EXPORT jclass qtjambi_find_generated_superclass(JNIEnv * env, jclass clazz);

QTJAMBI_EXPORT jobject qtjambi_find_QmlAttachedProperties(JNIEnv * env, jclass clazz);

QTJAMBI_EXPORT size_t qtjambi_extended_size_for_class(JNIEnv *env, jclass object_class);

QTJAMBI_EXPORT jmethodID resolveMethod(JNIEnv *env, const char *methodName, const char *signature, jclass clazz, bool isStatic = false);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT void registerJavaClassForCustomMetaType(int metaType, const QByteArray& javaClass);
#else
QTJAMBI_EXPORT int qtjambi_register_metatype(JNIEnv *env, jclass clazz, const QString& javaClassName);
QTJAMBI_EXPORT int qtjambi_register_list_metatype(JNIEnv *env, const QMetaType& elementType);
QTJAMBI_EXPORT int registerQmlMetaType(const QByteArray& javaClass,
                                             const QByteArray& typeName,
                                             QtPrivate::QMetaTypeInterface::DefaultCtrFn defaultCtr,
                                             QtPrivate::QMetaTypeInterface::CopyCtrFn copyCtr,
                                             QtPrivate::QMetaTypeInterface::MoveCtrFn moveCtr,
                                             QtPrivate::QMetaTypeInterface::DtorFn dtor,
                                             QtPrivate::QMetaTypeInterface::EqualsFn equals,
                                             QtPrivate::QMetaTypeInterface::LessThanFn lessThan,
                                             QtPrivate::QMetaTypeInterface::DebugStreamFn debugStream,
                                             QtPrivate::QMetaTypeInterface::DataStreamOutFn dataStreamOutFn,
                                             QtPrivate::QMetaTypeInterface::DataStreamInFn dataStreamInFn,
                                             QtPrivate::QMetaTypeInterface::LegacyRegisterOp legacyRegisterOp,
                                             uint size,
                                             ushort align,
                                             int builtInTypeId,
                                             QMetaType::TypeFlags flags,
                                             const QMetaObject *metaObject,
                                             QtPrivate::QMetaTypeInterface::MetaObjectFn metaObjectFn);
#endif

#endif // QTJAMBI_QML_H
