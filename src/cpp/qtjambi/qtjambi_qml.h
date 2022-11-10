#ifndef QTJAMBI_QML_H
#define QTJAMBI_QML_H

#include "qtjambi_global.h"

class QObject;
struct QMetaObject;

typedef QObject* (* CreateQmlErrorDummyObject) (const QMetaObject* metaObject, void* placement, int vsCast, int viCast);

QTJAMBI_EXPORT void registerCreateQmlErrorDummyObjectFunction(CreateQmlErrorDummyObject createQmlErrorDummyObject);

QTJAMBI_EXPORT QObject* createQmlErrorDummyObject(const QMetaObject* metaObject, void* placement, int vsCast, int viCast);

QTJAMBI_EXPORT jclass qtjambi_find_generated_superclass(JNIEnv * env, jclass clazz);

QTJAMBI_EXPORT size_t qtjambi_extended_size_for_class(JNIEnv *env, jclass object_class);

QTJAMBI_EXPORT jmethodID resolveMethod(JNIEnv *env, const char *methodName, const char *signature, jclass clazz, bool isStatic = false, jthrowable* exceptionOccurred = nullptr);

QTJAMBI_EXPORT void qtjambi_register_qml_list_element_type(JNIEnv *env, jobject qmlList, const QMetaType& elementType);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QTJAMBI_EXPORT void registerJavaClassForCustomMetaType(JNIEnv *env, int metaType, jclass javaClass);
#else
QTJAMBI_EXPORT int qtjambi_register_metatype(JNIEnv *env, jclass clazz, const QString& javaClassName);

enum class ContainerType;
QTJAMBI_EXPORT int qtjambi_register_container_metatype(JNIEnv *env, ContainerType containerType, const QMetaType& elementType);
QTJAMBI_EXPORT int registerQmlMetaType(JNIEnv *env, jclass clazz,
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
QTJAMBI_EXPORT bool qtjambi_register_metatype_converter(JNIEnv *env, const QMetaType& metaType1, jclass jsvalueClass, const QMetaType& metaType2, jclass targetClass, jmethodID constructor);
QTJAMBI_EXPORT bool qtjambi_register_qml_extension(JNIEnv *env, const QMetaObject *extended_meta_object, jclass javaClass);
#endif

#endif // QTJAMBI_QML_H
