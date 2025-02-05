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

#include "global.h"
#include "registryutil_p.h"
#include "typemanager_p.h"
#include "java_p.h"
#include "qtjambimetaobject_p.h"
#include "coreapi.h"
#include "qmlapi.h"
#include "qtjambi_cast.h"

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#define qtjambiMetaType QMetaType::fromType
//qMetaTypeId
void * pointerConstructHelper(void * where, const void *pointer);
void destructHelper(void *);
#else
#define qtjambiMetaType QMetaType::fromType
#endif

extern "C" JNIEXPORT jint JNICALL Java_io_qt_internal_MetaTypeUtility_registerRefMetaType(JNIEnv *env, jclass, jint id, jboolean isPointer, jboolean isReference){
    try{
        QMetaType metaType(id);
        if(metaType==qtjambiMetaType<JObjectWrapper>()
                || metaType==qtjambiMetaType<JMapWrapper>()
                || metaType==qtjambiMetaType<JCollectionWrapper>()
                || metaType==qtjambiMetaType<JObjectArrayWrapper>()
                || metaType==qtjambiMetaType<JIntArrayWrapper>()
                || metaType==qtjambiMetaType<JLongArrayWrapper>()
                || metaType==qtjambiMetaType<JByteArrayWrapper>()
                || metaType==qtjambiMetaType<JShortArrayWrapper>()
                || metaType==qtjambiMetaType<JDoubleArrayWrapper>()
                || metaType==qtjambiMetaType<JFloatArrayWrapper>()
                || metaType==qtjambiMetaType<JCharArrayWrapper>()
                || metaType==qtjambiMetaType<JBooleanArrayWrapper>()
                || metaType==qtjambiMetaType<JIteratorWrapper>()
                || metaType==qtjambiMetaType<JEnumWrapper>())
            return id;
        QByteArray typeName(metaType.name());
        if(isPointer){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if(!typeName.endsWith("*")
                    && !typeName.contains("(*)")
                    && !typeName.contains("(__cdecl*)")){
#else
            if(!(metaType.flags() & QMetaType::IsPointer)){
#endif
                if(!typeName.endsWith("*")){
                    typeName = QMetaObject::normalizedType(typeName + "*");
                }
                QMetaType::TypeFlags flags;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                flags |= QMetaType::IsPointer;
#endif
                if(metaType.metaObject()){
                    if(metaType.metaObject()->inherits(&QObject::staticMetaObject))
                        flags |= QMetaType::PointerToQObject;
                    else
                        flags |= QMetaType::PointerToGadget;
                }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                QMetaType::Destructor destructor = &destructHelper;
                QMetaType::Constructor constructor = &pointerConstructHelper;
                int typeId = QMetaType::registerNormalizedType(
                        typeName,
                        destructor,
                        constructor,
                        sizeof(void*),
                        flags,
                        metaType.metaObject()
                    );
                RegistryAPI::registerComparator(new QtPrivate::BuiltInComparatorFunction<void*>(), typeId);
                return typeId;
#else
                QMetaType _metaType = createMetaType(typeName,
                                                    true,
                                                    /*.defaultCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::defaultCtr,
                                                    /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::copyCtr,
                                                    /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::moveCtr,
                                                    /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::dtor,
                                                    /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                    /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                    /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                    /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                    /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                    /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::legacyRegisterOp,
                                                    /*.size=*/ sizeof(void*),
                                                    /*.alignment=*/ alignof(void*),
                                                    /*.typeId=*/ QMetaType::UnknownType,
                                                    /*.flags=*/ QMetaType::TypeFlags(flags),
                                                    nullptr,
                                                    QMetaType(metaType).iface()->metaObjectFn);
                _metaType.id();
                if(jclass clazz = CoreAPI::getClassForMetaType(env, metaType)){
                    registerConverterVariant(env, _metaType, typeName, QtJambiAPI::getClassName(env, clazz).replace('.', '/'), clazz);
                }
                return _metaType.id();
#endif
            }
        }else if(isReference){
            return 0;
            if(!typeName.endsWith("&")
                && !typeName.contains("(&)")
                && !typeName.contains("(__cdecl&)")){
                if(!typeName.endsWith("&")){
                    typeName = QMetaObject::normalizedType(typeName + "&");
                }
                QMetaType::TypeFlags flags;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                QMetaType::Destructor destructor = &destructHelper;
                QMetaType::Constructor constructor = &pointerConstructHelper;
                int typeId = QMetaType::registerNormalizedType(
                    typeName,
                    destructor,
                    constructor,
                    sizeof(void*),
                    flags,
                    metaType.metaObject()
                    );
                RegistryAPI::registerComparator(new QtPrivate::BuiltInComparatorFunction<void*>(), typeId);
                return typeId;
#else
                QMetaType _metaType = createMetaType(typeName,
                                                     true,
                                                     /*.defaultCtr=*/ nullptr,
                                                     /*.copyCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::copyCtr,
                                                     /*.moveCtr=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::moveCtr,
                                                     /*.dtor=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::dtor,
                                                     /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                     /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                     /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                     /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                     /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                     /*.legacyRegisterOp=*/ QtJambiPrivate::QMetaTypeInterfaceFunctions<void*>::legacyRegisterOp,
                                                     /*.size=*/ sizeof(void*),
                                                     /*.alignment=*/ alignof(void*),
                                                     /*.typeId=*/ QMetaType::UnknownType,
                                                     /*.flags=*/ QMetaType::TypeFlags(flags),
                                                     nullptr,
                                                     QMetaType(metaType).iface()->metaObjectFn);
                _metaType.id();
                if(jclass clazz = CoreAPI::getClassForMetaType(env, metaType)){
                    registerConverterVariant(env, _metaType, typeName, QtJambiAPI::getClassName(env, clazz).replace('.', '/'), clazz);
                }
                return _metaType.id();
#endif
            }
        }
        return id;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return 0;
    }
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_MetaTypeUtility_internalTypeName(JNIEnv *env, jclass, jstring s, jobject classLoader){
    try{
        QString signature = qtjambi_cast<QString>(env, s);

        auto prefix_end = signature.indexOf("(");
        QString prefix;
        if (prefix_end >= 0) {
            prefix = signature.mid(0, prefix_end+1);
            signature = signature.mid(prefix_end+1);
        }

        auto postfix_start = signature.lastIndexOf(")");
        QString postfix;
        if (postfix_start >= 0) {
            postfix = signature.mid(postfix_start);
            signature = signature.mid(0, postfix_start);
        }

        QStringList allArgs = signature.split(",");
        for (int i=0; i<allArgs.size(); ++i) {
            if (!allArgs.at(i).isEmpty()) {
                // may it allow enum and flags?
                allArgs[i] = QtJambiTypeManager::getInternalTypeName(env, QString(allArgs.at(i)).replace('.', '/'), classLoader);
                if (allArgs[i].isEmpty()){ // Can't convert type name, in which case we just return emptiness
                    return qtjambi_cast<jstring>(env, "");
                }
            }
        }

        return qtjambi_cast<jstring>(env, prefix + allArgs.join(",") + postfix);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_MetaTypeUtility_internalTypeNameByClass(JNIEnv *env, jclass, jclass cls){
    try{
        QString result = QtJambiTypeManager::getInternalTypeName(env, cls);
        return qtjambi_cast<jstring>(env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT int JNICALL Java_io_qt_internal_MetaTypeUtility_findMetaType(JNIEnv *env, jclass, jstring name){
    try{
        const char* _name = env->GetStringUTFChars(name, nullptr);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        int result = QMetaType::type(_name);
#else
        int result = QMetaType::fromName(_name).id();
#endif
        env->ReleaseStringUTFChars(name, _name);
        return result;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return 0;
    }
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_MetaTypeUtility_isObjectWrapperType(JNIEnv *, jclass, jint metaTypeId){
    return metaTypeId == registeredMetaTypeID(typeid(JObjectWrapper))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            || isJObjectWrappedMetaType(QMetaType(metaTypeId))
#endif
            || metaTypeId == registeredMetaTypeID(typeid(JCollectionWrapper))
            || metaTypeId == registeredMetaTypeID(typeid(JMapWrapper))
            || metaTypeId == registeredMetaTypeID(typeid(JIteratorWrapper));
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_MetaTypeUtility_isCustomValueType(JNIEnv *, jclass, jint metaTypeId){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return JObjectValueWrapper::isValueType(QMetaType(metaTypeId));
#else
    Q_UNUSED(metaTypeId)
    return false;
#endif
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_MetaTypeUtility_metaTypeHasDataStreamOperators(JNIEnv *, jclass, jint metaTypeId){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMetaType metaType(metaTypeId);
    return metaType.iface()->dataStreamIn || metaType.iface()->dataStreamOut;
#else
    Q_UNUSED(metaTypeId)
    return false;
#endif
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_MetaTypeUtility_metaTypeHasDebugStreamOperator(JNIEnv *, jclass, jint metaTypeId){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return JObjectValueWrapper::hasCustomDebugStreamOperator(QMetaType(metaTypeId));
#else
    Q_UNUSED(metaTypeId)
    return false;
#endif
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_MetaTypeUtility_registerCustomDataStreamOperators(JNIEnv *, jclass, jint metaTypeId){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMetaType metaType(metaTypeId);
    if(!metaType.iface()->dataStreamIn && !metaType.iface()->dataStreamOut){
        const_cast<QtPrivate::QMetaTypeInterface*>(metaType.iface())->dataStreamIn = QtPrivate::QDataStreamOperatorForType<JObjectWrapper>::dataStreamIn;
        const_cast<QtPrivate::QMetaTypeInterface*>(metaType.iface())->dataStreamOut = QtPrivate::QDataStreamOperatorForType<JObjectWrapper>::dataStreamOut;
    }
#else
    Q_UNUSED(metaTypeId)
#endif
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_MetaTypeUtility_registerCustomDebugStreamOperator(JNIEnv *, jclass, jint metaTypeId){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMetaType metaType(metaTypeId);
    if(!JObjectValueWrapper::hasCustomDebugStreamOperator(metaType))
        const_cast<QtPrivate::QMetaTypeInterface*>(metaType.iface())->debugStream = QtPrivate::QDebugStreamOperatorForType<JObjectWrapper>::debugStream;
#else
    Q_UNUSED(metaTypeId)
#endif
}

extern "C" JNIEXPORT bool JNICALL Java_io_qt_internal_MetaTypeUtility_registerConverter
(JNIEnv *env,
 jclass,
 jint metaTypeId1,
 jclass class1,
 jint metaTypeId2,
 jclass class2,
 jobject converterFn
 )
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QMetaType metaType1(metaTypeId1);
    QMetaType metaType2(metaTypeId2);
    if(!QMetaType::hasRegisteredConverterFunction(metaType1, metaType2)){
        class1 = getGlobalClassRef(env, class1);
        class2 = getGlobalClassRef(env, class2);
        QtJambiUtils::InternalToExternalConverter converter1 = QtJambiTypeManager::getInternalToExternalConverter(env, metaType1.name(), metaType1, class1);
        QtJambiUtils::ExternalToInternalConverter reconverter1 = QtJambiTypeManager::getExternalToInternalConverter(env, class1, metaType1.name(), metaType1);
        QtJambiUtils::InternalToExternalConverter converter2 = QtJambiTypeManager::getInternalToExternalConverter(env, metaType2.name(), metaType2, class2);
        QtJambiUtils::ExternalToInternalConverter reconverter2 = QtJambiTypeManager::getExternalToInternalConverter(env, class2, metaType2.name(), metaType2);
        if(converter1 && reconverter1 && converter2 && reconverter2){
            ParameterTypeInfo parameter1{metaTypeId1, class1, std::move(converter1), std::move(reconverter1)};
            ParameterTypeInfo parameter2{metaTypeId2, class2, std::move(converter2), std::move(reconverter2)};
            JObjectWrapper converter(env, converterFn);
            return QMetaType::registerConverterFunction([converter, parameter1, parameter2](const void *src, void *target)->bool{
                if(JniEnvironment env{500}){
                    QtJambiScope scope;
                    jvalue jv;
                    jv.l = nullptr;
                    if(parameter1.convertInternalToExternal(env, &scope, src, jv, true)){
                        jobject result{nullptr};
                        try{
                            result = Java::Runtime::Function::apply(env, converter.object(env), jv.l);
                        }catch(const JavaException&){
                            return false;
                        }
                        if(!result && !(QMetaType(parameter2.metaType()).flags() & QMetaType::IsPointer))
                            return false;
                        jv.l = result;
                        return parameter2.convertExternalToInternal(env, nullptr, jv, target, jValueType::l);
                    }
                }
                return false;
            }, metaType1, metaType2);
        }
    }
#else
    Q_UNUSED(env)
    Q_UNUSED(metaTypeId1)
    Q_UNUSED(class1)
    Q_UNUSED(metaTypeId2)
    Q_UNUSED(class2)
    Q_UNUSED(converterFn)
#endif
    return false;
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_internal_MetaTypeUtility_registerQmlListProperty(JNIEnv *env, jclass, jstring type){
    try{
        QByteArray _type = qtjambi_cast<QByteArray>(env, type);
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        int id = QMetaType::type("QQmlListProperty<QObject>");
        id = QMetaType::registerNormalizedTypedef(_type, id);
#else
        QMetaType t = QMetaType::fromName("QQmlListProperty<QObject>");
        int id = t.id();
        if(const QtPrivate::QMetaTypeInterface * copy = t.iface()){
            id = createMetaType(QMetaObject::normalizedType(_type),
                                true,
                                /*.defaultCtr=*/ copy->defaultCtr,
                                /*.copyCtr=*/ copy->copyCtr,
                                /*.moveCtr=*/ copy->moveCtr,
                                /*.dtor=*/ copy->dtor,
                                /*.equals=*/ copy->equals,
                                /*.lessThan=*/ copy->lessThan,
                                /*.debugStream=*/ copy->debugStream,
                                /*.dataStreamOut=*/ copy->dataStreamOut,
                                /*.dataStreamIn=*/ copy->dataStreamIn,
                                /*.legacyRegisterOp=*/ copy->legacyRegisterOp,
                                /*.size=*/ copy->size,
                                /*.alignment=*/ copy->alignment,
                                /*.typeId=*/ QMetaType::UnknownType,
                                /*.flags=*/ QMetaType::TypeFlags(int(copy->flags)),
                                nullptr, copy->metaObjectFn).id();
        }
#endif
        return id;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return QMetaType::UnknownType;
}
