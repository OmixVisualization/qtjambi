/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "pch_p.h"

#define EXCLUDE_GT_END(strg) strg //.endsWith(">") ? strg+" " : strg

void CoreAPI::registerMetaTypeOfPointer(const void* pointer, QMetaType&& metaType){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->lock());
        storage->metaTypesByPointer().insert(pointer, std::move(metaType));
    }
}

QMetaType CoreAPI::unregisterMetaTypeOfPointer(const void* pointer){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->lock());
        return storage->metaTypesByPointer().take(pointer);
    }
}

const QtPrivate::QBindableInterface* CoreAPI::registeredBindableInterface(size_t hash){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QReadLocker locker(storage->lock());
        return storage->bindableInterfacesHash().value(hash);
    }
}

const QtPrivate::QBindableInterface* CoreAPI::registerBindableInterface(size_t hash, const QtPrivate::QBindableInterface* iface){
    QtJambiStorage* storage = getQtJambiStorage();
    {
        QWriteLocker locker(storage->lock());
        if(const QtPrivate::QBindableInterface *_iface = storage->bindableInterfacesHash().value(hash)){
            delete iface;
            return _iface;
        }else{
            storage->bindableInterfacesHash().insert(hash, iface);
            return iface;
        }
    }
}

// enum helpers...

template<typename INT>
jobject convertEnumToJavaObject(JNIEnv *env, INT qt_enum, jclass cl, const QString& className, jobject (*resolveEnum)(JNIEnv *, jint, jclass, INT, jstring))
{
    Q_ASSERT(cl);

    jobject obj;
    QString sig;
    switch ( sizeof(INT) ) {
    case 1:  sig = QLatin1String("(B)L%1;"); break;
    case 2:  sig = QLatin1String("(S)L%1;"); break;
    case 8:  sig = QLatin1String("(J)L%1;"); break;
    default: sig = QLatin1String("(I)L%1;"); break;
    }
    jmethodID method = JavaAPI::resolveMethod(env, "resolve", qPrintable(sig.arg(className)), cl, true);
    if(method){
        if(env->ExceptionCheck()){
            env->ExceptionDescribe();
            env->ExceptionClear();
        }
        obj = env->CallStaticObjectMethod(cl, method, qt_enum);
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }else{
        jint hashCode = Java::Runtime::Object::hashCode(env,cl);
        obj = resolveEnum(env, hashCode, cl, qt_enum, nullptr);
    }
    return obj;
}

jobject CoreAPI::convertEnumToJavaObject(JNIEnv *env, qint32 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jint>(env, jint(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveIntEnum);
}

jobject CoreAPI::convertEnumToJavaObject(JNIEnv *env, qint16 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jshort>(env, jshort(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveShortEnum);
}

jobject CoreAPI::convertEnumToJavaObject(JNIEnv *env, qint8 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jbyte>(env, jbyte(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveByteEnum);
}

jobject CoreAPI::convertEnumToJavaObject(JNIEnv *env, qint64 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jlong>(env, jlong(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveLongEnum);
}

jobject CoreAPI::convertEnumToJavaObject(JNIEnv *env, quint32 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jint>(env, jint(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveIntEnum);
}

jobject CoreAPI::convertEnumToJavaObject(JNIEnv *env, quint16 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jshort>(env, jshort(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveShortEnum);
}

jobject CoreAPI::convertEnumToJavaObject(JNIEnv *env, quint8 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jbyte>(env, jbyte(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveByteEnum);
}

jobject CoreAPI::convertEnumToJavaObject(JNIEnv *env, quint64 qt_enum, jclass cls)
{
    return convertEnumToJavaObject<jlong>(env, jlong(qt_enum), cls, QtJambiAPI::getClassName(env, cls).replace(QLatin1Char('.'), QLatin1Char('/')), &resolveLongEnum);
}

QString CoreAPI::externalTypeTame(JNIEnv *env, const QString& internalName){
    return QtJambiTypeManager::getExternalTypeName(env, internalName);
}

jobject CoreAPI::javaObject(QtJambiNativeID nativeId, JNIEnv* env)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->getJavaObjectLocalRef(env) : nullptr;
}

jobject CoreAPI::fromDestroyedQObject(JNIEnv *env, QObject* object)
{
    jobject result{nullptr};
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(object)){
        result = link->getJavaObjectLocalRef(env);
    }
    if(!result)
        result = QtJambiAPI::convertQObjectToJavaObject(env, object);
    return result;
}

jobject CoreAPI::convertReflectedMethodToMeta(JNIEnv * env, jlong metaObjectPointer, jobjectArray ok)
{
    if(metaObjectPointer){
        env->EnsureLocalCapacity(100);
        jobject reflect_method = env->GetObjectArrayElement(ok, 0);
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        int index = QtJambiMetaObject::methodFromJMethod(metaObject, env->FromReflectedMethod(reflect_method));
        QMetaMethod method = metaObject->method(index);
        if(method.isValid()){
            return qtjambi_cast<jobject>(env, std::move(method));
        }else{
            env->SetObjectArrayElement(ok, 0, nullptr);
            return nullptr;
        }
    }
    return nullptr;
}

#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
struct MethodArgs{
    QVector<void*> parameters;
    QVector<const char*> typeNames;
    QVector<const QtPrivate::QMetaTypeInterface *> metaTypes;
    MethodArgs(int _paramCount)
        : parameters(_paramCount+1),
          typeNames(_paramCount+1),
          metaTypes(_paramCount+1){}
    inline qsizetype parameterCount() const {return parameters.size();}
};

namespace QtPrivate{
template <>
inline auto invokeMethodHelper(QMetaMethodReturnArgument, const MethodArgs& arguments)
{
    return arguments;
}
}
#endif

jobject invokeMetaMethodImpl(JNIEnv * env, const QMetaMethod& method,
                         QtJambiNativeID _qobject, QObject* object,
                         jint connection, jobjectArray args)
{
    jvalue result;
    result.l = nullptr;
    if(method.isValid()){
        env->EnsureLocalCapacity(500);
        if(connection==Qt::BlockingQueuedConnection
            && object->thread()
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
            && object->thread()->isCurrentThread()
#else
            && object->thread()==QThread::currentThread()
#endif
        ) {
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Blocking-queued invocation on object whose thread is the current thread is not allowed.") QTJAMBI_STACKTRACEINFO );
        }
        if(method.returnType()!=QMetaType::UnknownType && method.returnType()!=QMetaType::Void) {
            if(connection==Qt::QueuedConnection) {
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Unable to invoke method with return value in queued connections.") QTJAMBI_STACKTRACEINFO );
            }else if(connection==Qt::AutoConnection) {
                if(object->thread()
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
                    && !object->thread()->isCurrentThread()
#else
                    && object->thread()!=QThread::currentThread()
#endif
                ) {
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Unable to invoke method with return value in queued connections (auto connection with different threads).") QTJAMBI_STACKTRACEINFO );
                }
            }
        }
        QtJambiScope scope(_qobject);
        bool ok = false;
        const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, method);
        int argsCount = env->GetArrayLength(args);
        const int parameterCount = method.parameterCount();
#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
        if(parameterCount==argsCount && argsCount<11){
            QGenericArgument val[10];
#else
        if(parameterCount==argsCount){
            MethodArgs methodArgs(parameterCount);
#endif
            QList<QByteArray> parameterTypes = method.parameterTypes();
            ok = true;
            for(int i=0; i<argsCount; ++i){
                jvalue jval;
                jval.l = env->GetObjectArrayElement(args, i);
                void* qtPtr = nullptr;
                const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                ok = parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l);
                if(ok){
#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
                    val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
#else
                    methodArgs.typeNames[i+1] = parameterTypes[i];
                    methodArgs.parameters[i+1] = qtPtr;
                    methodArgs.metaTypes[i+1] = method.parameterMetaType(i).iface();
#endif
                }else
                    break;
            }
            if(ok){
                void* resultPtr = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
                if(method.returnMetaType().id()!=QMetaType::Void){
                    resultPtr = method.returnMetaType().create();
                    scope.addDeletion(method.returnMetaType(), resultPtr);
                }
                ok = method.invoke(object,
                                   Qt::ConnectionType(connection),
                                   QGenericReturnArgument(method.typeName(), resultPtr),
                                   val[0],
                                   val[1],
                                   val[2],
                                   val[3],
                                   val[4],
                                   val[5],
                                   val[6],
                                   val[7],
                                   val[8],
                                   val[9]);
#else
                if(method.returnMetaType().id()!=QMetaType::Void){
                    resultPtr = method.returnMetaType().create();
                    scope.addDeletion(method.returnMetaType(), resultPtr);
                    methodArgs.parameters[0] = resultPtr;
                }
                methodArgs.typeNames[0] = method.typeName();
                methodArgs.metaTypes[0] = method.returnMetaType().iface();
                ok = method.invoke(object,
                                   Qt::ConnectionType(connection),
                                   methodArgs);
#endif
                if(ok && resultPtr){
                    ok = parameterTypeInfos[0].convertInternalToExternal(env, nullptr, resultPtr, result, true);
                    if(parameterTypeInfos[0].metaType().flags() & QMetaType::IsPointer){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, result.l)){
                            if(!link->isQObject() || !link->qobject()->parent())
                                link->setDefaultOwnership(env);
                        }
                    }
                }
            }
        }else if(parameterCount!=argsCount){
            Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong number of arguments. expected: %1, given: %2").arg(parameterCount).arg(argsCount) QTJAMBI_STACKTRACEINFO );
        }
        if(!ok){
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String(method.methodSignature()) QTJAMBI_STACKTRACEINFO );
        }
    }else{
        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Cannot invoke an invalid QMetaMethod.") QTJAMBI_STACKTRACEINFO );
    }
    return result.l;
}

jobject CoreAPI::invokeMetaMethod(JNIEnv * env, QtJambiNativeID _metaMethod,
                                   QtJambiNativeID _qobject,
                                   jint connection, jobjectArray args)
{
    QObject* object = QtJambiAPI::objectFromNativeId<QObject>(_qobject);
    QtJambiAPI::checkNullPointer(env, object);
    QMetaMethod method = QtJambiAPI::valueFromNativeId<QMetaMethod>(_metaMethod);
    const QMetaObject* enclosingMetaObject = method.enclosingMetaObject();
    const QMetaObject* objectMO = object->metaObject();
    if(objectMO!=enclosingMetaObject && method.isValid() && !objectMO->inherits(enclosingMetaObject)) {
        Java::Runtime::IllegalArgumentException::throwNew(env, QString("Given object is not an instance of %1").arg(enclosingMetaObject->className()) QTJAMBI_STACKTRACEINFO );
    }
    return invokeMetaMethodImpl(env, method, _qobject, object, connection, args);
}

jobject CoreAPI::invokeMetaMethod(JNIEnv * env,
                                  jlong metaObjectPointer, jint index, QtJambiNativeID qobject_id,
                                  jint connection, jobjectArray args)
{
    const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
    QObject* object = QtJambiAPI::objectFromNativeId<QObject>(qobject_id);
    QtJambiAPI::checkNullPointer(env, object);
    if(!metaObject)
        metaObject = object->metaObject();
    QtJambiAPI::checkNullPointer(env, metaObject);
    return invokeMetaMethodImpl(env, metaObject->method(index), qobject_id, object, connection, args);
}

jobject CoreAPI::getMetaMethodParameterClassTypes(JNIEnv * env, jobject _this){
    const QMetaMethod *__qt_this = QtJambiAPI::convertJavaObjectToNative<QMetaMethod>(env, _this);
    jobject result = QtJambiAPI::newJavaArrayList(env, __qt_this->parameterCount());
    if(__qt_this && __qt_this->isValid()){
        const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *__qt_this);
        for(int i=1; i<parameterTypeInfos.size(); ++i){
            QtJambiAPI::addToJavaCollection(env, result, parameterTypeInfos[i].javaClass());
        }
    }
    return result;
}

jobject CoreAPI::getMetaMethodReturnClassType(JNIEnv * env, jobject _this){
    const QMetaMethod *__qt_this = QtJambiAPI::convertJavaObjectToNative<QMetaMethod>(env, _this);
    if(__qt_this && __qt_this->isValid()){
        if(__qt_this->methodType()==QMetaMethod::Constructor)
            return nullptr;
        const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *__qt_this);
        if(parameterTypeInfos.size()>0){
            return parameterTypeInfos[0].javaClass();
        }
    }
    return nullptr;
}

jobject CoreAPI::convertMetaMethodToReflected(JNIEnv * env, jobject _this){
    const QMetaMethod *__qt_this = QtJambiAPI::convertJavaObjectToNative<QMetaMethod>(env, _this);
    if(__qt_this && __qt_this->isValid()){
        return QtJambiMetaObject::toReflected(env, *__qt_this);
    }
    return nullptr;
}


jobject CoreAPI::invokeMetaMethodOnGadget(JNIEnv * env, QtJambiNativeID _metaMethod,
                                           jobject object,
                                           jobjectArray args)
{
    env->EnsureLocalCapacity(500);
    jvalue result;
    result.l = nullptr;
    JObjectWrapper wrapper;
    QMetaMethod method = QtJambiAPI::valueFromNativeId<QMetaMethod>(_metaMethod);
    if(method.isValid()){
        QtJambiScope scope;
        void* ptr = nullptr;
        if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object)){
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
                const std::type_info* typeId = nullptr;
                const QMetaObject *metaObject = method.enclosingMetaObject();
                do{
                    typeId = getTypeByQtName(metaObject->className());
                    if(typeId)
                        break;
                    metaObject = metaObject->superClass();
                }while(metaObject);
                if(typeId){
                    ptr = link->typedPointer(*typeId);
                }else{
                    typeId = &typeid(JObjectWrapper);
                    ptr = link->pointer();
                }
                QtJambiAPI::checkNullPointer(env, ptr, *typeId);
            }else{
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            }
        }else{
            wrapper = JObjectWrapper(env, object);
            ptr = &wrapper;
        }
        if(ptr){
            bool ok = false;
            const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, method);
            int argsCount = env->GetArrayLength(args);
            const int parameterCount = method.parameterCount();
#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
            if(parameterCount==argsCount && argsCount<11){
                QGenericArgument val[10];
#else
            if(parameterCount==argsCount){
                MethodArgs methodArgs(parameterCount);
#endif
                QList<QByteArray> parameterTypes = method.parameterTypes();
                ok = true;
                for(int i=0; i<argsCount; ++i){
                    jvalue jval;
                    jval.l = env->GetObjectArrayElement(args, i);
                    void* qtPtr = nullptr;
                    const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                    ok = parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l);
                    if(ok){
#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
                        val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
#else
                        methodArgs.typeNames[i+1] = parameterTypes[i];
                        methodArgs.parameters[i+1] = qtPtr;
                        methodArgs.metaTypes[i+1] = method.parameterMetaType(i).iface();
#endif
                    }else
                        break;
                }
                if(ok){
                    void* resultPtr = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
                    if(method.returnMetaType().id()!=QMetaType::Void){
                        resultPtr = method.returnMetaType().create();
                        scope.addDeletion(method.returnMetaType(), resultPtr);
                    }
                    ok = method.invokeOnGadget(ptr,
                                               QGenericReturnArgument(method.typeName(), resultPtr),
                                               val[0],
                                               val[1],
                                               val[2],
                                               val[3],
                                               val[4],
                                               val[5],
                                               val[6],
                                               val[7],
                                               val[8],
                                               val[9]);
#else
                    if(method.returnMetaType().id()!=QMetaType::Void){
                        resultPtr = method.returnMetaType().create();
                        scope.addDeletion(method.returnMetaType(), resultPtr);
                        methodArgs.parameters[0] = resultPtr;
                    }
                    methodArgs.typeNames[0] = method.typeName();
                    methodArgs.metaTypes[0] = method.returnMetaType().iface();
                    ok = method.invokeOnGadget(ptr, methodArgs);
#endif
                    if(ok && resultPtr){
                        ok = parameterTypeInfos[0].convertInternalToExternal(env, nullptr, resultPtr, result, true);
                        if(parameterTypeInfos[0].metaType().flags() & QMetaType::IsPointer){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, result.l)){
                                if(!link->isQObject() || !link->qobject()->parent())
                                    link->setDefaultOwnership(env);
                            }
                        }
                    }
                }
            }else if(parameterCount!=argsCount){
                Java::Runtime::IllegalArgumentException::throwNew(env, QString("Wrong number of arguments. expected: %1, given: %2").arg(parameterCount).arg(argsCount) QTJAMBI_STACKTRACEINFO );
            }
            if(!ok){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String(method.methodSignature()) QTJAMBI_STACKTRACEINFO );
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Cannot invoke an invalid QMetaMethod.") QTJAMBI_STACKTRACEINFO );
        }
    }
    return result.l;
}

jclass CoreAPI::getMetaPropertyClassType(JNIEnv *env, jobject _this){
    try{
        const QMetaProperty *__qt_this = QtJambiAPI::convertJavaObjectToNative<QMetaProperty>(env, _this);
        if(__qt_this && __qt_this->isValid()){
            if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(__qt_this->enclosingMetaObject())){
                return mo->typeOfProperty(__qt_this->relativePropertyIndex());
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

jboolean CoreAPI::resetMetaPropertyOnGadget(JNIEnv *env, jobject _this, jobject gadget){
    try{
        const QMetaProperty *__qt_this = QtJambiAPI::convertJavaObjectToNative<QMetaProperty>(env, _this);
        if(__qt_this && __qt_this->isValid()){
            void* ptr = nullptr;
            JObjectWrapper wrapper;
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, gadget)){
                QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, gadget);
                const std::type_info* typeId = nullptr;
                const QMetaObject* metaObject = __qt_this->enclosingMetaObject();
                do{
                    typeId = getTypeByQtName(metaObject->className());
                    if(typeId)
                        break;
                    metaObject = metaObject->superClass();
                }while(metaObject);
                if(!typeId)
                    typeId = &typeid(JObjectWrapper);
                if(link){
                    ptr = link->typedPointer(*typeId);
                }
                QtJambiAPI::checkNullPointer(env, ptr, *typeId);
            }else{
                wrapper = JObjectWrapper(env, gadget);
                ptr = &wrapper;
            }
            return __qt_this->resetOnGadget(ptr);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

jobject CoreAPI::readMetaPropertyOnGadget(JNIEnv *env, jobject _this, jobject gadget){
    try{
        QVariant variant;
        const QMetaProperty *__qt_this = QtJambiAPI::convertJavaObjectToNative<QMetaProperty>(env, _this);
        if(__qt_this && __qt_this->isValid()){
            void* ptr = nullptr;
            JObjectWrapper wrapper;
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, gadget)){
                QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, gadget);
                const std::type_info* typeId = nullptr;
                const QMetaObject* metaObject = __qt_this->enclosingMetaObject();
                do{
                    typeId = getTypeByQtName(metaObject->className());
                    if(typeId)
                        break;
                    metaObject = metaObject->superClass();
                }while(metaObject);
                if(!typeId)
                    typeId = &typeid(JObjectWrapper);
                if(link){
                    ptr = link->typedPointer(*typeId);
                }
                QtJambiAPI::checkNullPointer(env, ptr, *typeId);
            }else{
                wrapper = JObjectWrapper(env, gadget);
                ptr = &wrapper;
            }
            variant = __qt_this->readOnGadget(ptr);
            if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(__qt_this->enclosingMetaObject())){
                if(QtJambiUtils::InternalToExternalConverter converter = mo->internalToExternalConverterOfProperty(__qt_this->relativePropertyIndex())){
                    jvalue val;
                    val.l = nullptr;
                    QtJambiScope scope(nullptr);
                    if(converter(env, &scope, variant.data(), val, true))
                        return val.l;
                }
            }

            int type;
            QT_WARNING_DISABLE_DEPRECATED
            if (__qt_this->type()==QVariant::Invalid) {
                return nullptr;
            }else if(__qt_this->type()==QVariant::UserType){
                type = __qt_this->userType();
            }else{
                type = __qt_this->type();
            }
            QLatin1String qtName(__qt_this->typeName());
            if(const std::type_info* typeId = getTypeByQtName(qtName)){
                const char* fullJavaName = getJavaName(*typeId);
                jclass object_class = JavaAPI::resolveClass(env, fullJavaName);
                Q_ASSERT(object_class);
                jvalue val;
                val.l = nullptr;
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                QMetaType _type(type);
                QtJambiUtils::InternalToExternalConverter converter = QtJambiTypeManager::getInternalToExternalConverter(env, qtName, _type, object_class);
                QtJambiScope scope(nullptr);
                bool result = converter && converter(env, &scope, variant.data(), val, true);
                if(result)
                    return val.l;
            }else{
                return qtjambi_cast<jobject>(env, std::move(variant));
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

jboolean CoreAPI::writeMetaPropertyOnGadget(JNIEnv *env, jobject _this, jobject gadget, jobject value){
    try{
        const QMetaProperty *__qt_this = QtJambiAPI::convertJavaObjectToNative<QMetaProperty>(env, _this);
        if(__qt_this && __qt_this->isValid()){
            void* ptr = nullptr;
            JObjectWrapper wrapper;
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, gadget)){
                QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, gadget);
                const std::type_info* typeId = nullptr;
                const QMetaObject* metaObject = __qt_this->enclosingMetaObject();
                do{
                    typeId = getTypeByQtName(metaObject->className());
                    if(typeId)
                        break;
                    metaObject = metaObject->superClass();
                }while(metaObject);
                if(!typeId)
                    typeId = &typeid(JObjectWrapper);
                if(link){
                    ptr = link->typedPointer(*typeId);
                }
                QtJambiAPI::checkNullPointer(env, ptr, *typeId);
            }else{
                wrapper = JObjectWrapper(env, gadget);
                ptr = &wrapper;
            }
            QMetaType type;
            QT_WARNING_DISABLE_DEPRECATED
            if (__qt_this->type()==QVariant::Invalid) {
                return false;
            }else{
                type = __qt_this->metaType();
            }
            QLatin1String qtName(__qt_this->typeName());
            QVariant valueVariant;
            bool success = false;
            if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(__qt_this->enclosingMetaObject())){
                if(QtJambiUtils::ExternalToInternalConverter converter = mo->externalToInternalConverterOfProperty(__qt_this->relativePropertyIndex())){
                    jvalue val;
                    val.l = value;
                    void *copy = nullptr;
                    QtJambiScope scope(nullptr);
                    if(!(success = converter(env, &scope, val, copy, jValueType::l))){
                        qCWarning(DebugAPI::internalCategory, "Cannot convert external type '%s' to '%s'",
                                 qPrintable(QString(QtJambiAPI::getObjectClassName(env, value)).replace("$", ".")),
                                 qPrintable(qtName));
                        return false;
                    }
                    valueVariant = QVariant(type, copy);
                }
            }
            if(!success){
                if(const std::type_info* typeId = getTypeByQtName(qtName)){
                    const char* fullJavaName = getJavaName(*typeId);
                    jclass object_class = JavaAPI::resolveClass(env, fullJavaName);
                    Q_ASSERT(object_class);
                    if(value && !env->IsInstanceOf(value, object_class)){
                        return false;
                    }
                    jvalue val;
                    val.l = value;
                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    QtJambiUtils::ExternalToInternalConverter converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, qtName, type);
                    void *copy = nullptr;
                    QtJambiScope scope(nullptr);
                    if(!converter(env, &scope, val, copy, jValueType::l)){
                        qCWarning(DebugAPI::internalCategory, "Cannot convert external type '%s' to '%s'",
                                 qPrintable(QString(QLatin1String(fullJavaName)).replace("/", ".").replace("$", ".")),
                                 qPrintable(qtName));
                        return false;
                    }
                    valueVariant = QVariant(type, copy);
                }else{
                    valueVariant = qtjambi_cast<QVariant>(env, value);
                }
            }
            return __qt_this->writeOnGadget(ptr, valueVariant);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

void pre_exit_function(){
    QtJambiExceptionInhibitor __exnHandler;
    if(JniEnvironment env{500}){
        try{
            Java::QtJambi::LibraryUtility::clear(env);
        }catch(const JavaException& exn){
            __exnHandler.handle(env, exn, "postRoutine");
        }
    }
}

void CoreAPI::preExit(){
    qAddPostRoutine(&pre_exit_function);
}

void CoreAPI::unexit(){
    qRemovePostRoutine(&pre_exit_function);
}

void CoreAPI::registerQProperty(JNIEnv *env, QtJambiNativeID __object_nativeId, QtJambiNativeID property_nativeId){
    QSharedPointer<QtJambiLink> objectLink = QtJambiLink::fromNativeId(__object_nativeId);
    QSharedPointer<QtJambiLink> propertyLink = QtJambiLink::fromNativeId(property_nativeId);
    QObject *object = QtJambiAPI::objectFromNativeId<QObject>(__object_nativeId);
    QtJambiAPI::checkNullPointer(env, object);
    QUntypedPropertyData *property = QtJambiAPI::objectFromNativeId<QUntypedPropertyData>(property_nativeId);
    QtJambiAPI::checkNullPointer(env, property);
    QtJambiLink::registerConDestroyedObject(env, object, propertyLink);
}

void CoreAPI::registerDependentInterface(JNIEnv *env, jobject dependentObject, jobject owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaInterface(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::findLinkForJavaInterface(env, owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

void CoreAPI::registerDependentObject(JNIEnv *env, jobject dependentObject, jobject owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaObject(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::findLinkForJavaInterface(env, owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

void CoreAPI::registerDependentObject(QtJambiNativeID dependent, QtJambiNativeID owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependent);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

void CoreAPI::unregisterDependentInterface(JNIEnv *env, jobject dependentObject, jobject owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaInterface(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::findLinkForJavaInterface(env, owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterDependentObject(_dependentLink);
    }
}

void CoreAPI::unregisterDependentObject(JNIEnv *env, jobject dependentObject, jobject owner){
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::findLinkForJavaObject(env, dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::findLinkForJavaInterface(env, owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterDependentObject(_dependentLink);
    }
}

void CoreAPI::ckeckLinkExtension(JNIEnv *env, QtJambiNativeID nativeId){
    QtJambiLink * link = reinterpret_cast<QtJambiLink *>(nativeId);
    if(ExtendedLinkInterface* elink = dynamic_cast<ExtendedLinkInterface*>(link)){
        if(!elink->hasExtension())
            Java::QtJambi::QNoNativeResourcesException::throwNew(env, "Dependent object has been deleted." QTJAMBI_STACKTRACEINFO );
    }
}

jclass CoreAPI::getMetaObjectJavaType(JNIEnv *env, const QMetaObject *metaObject, bool exactOrNull)
{
    return QtJambiMetaObject::javaClass(env, metaObject, exactOrNull);
}

QMetaMethod CoreAPI::findMetaMethod(const QMetaObject *metaObject, const QString& sig){
    QMetaMethod method;
    if(metaObject){
        method = metaObject->method(metaObject->indexOfMethod(qPrintable(sig)));
        if(!method.isValid()){
            QString methodName;
            QStringList arguments = QtJambiTypeManager::parseSignature(sig, &methodName);
            const int methodCount = metaObject->methodCount();
            for(int i=0; i<methodCount; ++i){
                QMetaMethod _method = metaObject->method(i);
                QString qmethodName = _method.name();
                if(const QHash<int,const char*>* _renamedMethods = CoreAPI::renamedMethods(_method.enclosingMetaObject())){
                    if(const char* newName = _renamedMethods->value(_method.methodIndex(), nullptr)){
                        qmethodName = QLatin1String(newName);
                    }
                }
                const int parameterCount = _method.parameterCount();
                if(qmethodName==methodName && arguments.size()==parameterCount+1){
                    QList<QByteArray> parameterTypes = _method.parameterTypes();
                    bool found = true;
                    for(int p=0; p<parameterCount; ++p){
                        if(_method.parameterType(p)==QMetaType::UnknownType){
                            if(QMetaObject::normalizedType(arguments.at(p+1).toLatin1())!=parameterTypes.at(p)){
                                QString arg = arguments.at(p+1);
                                if(arg.startsWith("const ") && arg.endsWith("&")){
                                    arg = arg.mid(6).chopped(1).trimmed();
                                    if(arg.toLatin1()==parameterTypes.at(p)){
                                        continue;
                                    }
                                }
                                found = false;
                                break;
                            }
                        }else{
                            if(_method.parameterType(p) != QMetaType::type(qPrintable(arguments.at(p+1)))){
                                found = false;
                                break;
                            }
                        }
                    }
                    if(found){
                        method = _method;
                        break;
                    }
                }
            }
        }
    }
    return method;
}

jobject CoreAPI::newInstanceForMetaObject(JNIEnv *env, QtJambiNativeID constructorId, jobjectArray args){
    env->EnsureLocalCapacity(64);
    const QMetaMethod& constructor = QtJambiAPI::objectReferenceFromNativeId<QMetaMethod>(env, constructorId);
    QtJambiScope scope(nullptr);
    if(constructor.isValid()){
        const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, constructor);
        int argsCount = env->GetArrayLength(args);
        const int parameterCount = constructor.parameterCount();
        if(parameterCount==argsCount && argsCount<10){
            QObject* obj = nullptr;
            void** val = new void*[argsCount+1];
            val[0] = &obj;
            bool failed = false;
            for(int i=0; i<argsCount; ++i){
                jvalue jval;
                jval.l = env->GetObjectArrayElement(args, i);
                const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                val[i+1] = nullptr;
                if(!parameterTypeInfo.convertExternalToInternal(env, &scope, jval, val[i+1], jValueType::l)){
                    failed = true;
                    break;
                }
            }
            if(!failed){
                if(constructor.enclosingMetaObject()->static_metacall(QMetaObject::CreateInstance, constructor.enclosingMetaObject()->indexOfConstructor(constructor.methodSignature()), val) >= 0){
                    return nullptr;
                }
            }
            if(obj){
                if(constructor.enclosingMetaObject()->inherits(&QObject::staticMetaObject)){
                    jobject jobj = QtJambiAPI::convertQObjectToJavaObject(env, obj);
                    if(!obj->parent() && !isQmlJavaScriptOwnership(obj)){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(obj)){
                            link->setJavaOwnership(env);
                        }
                    }
                    return jobj;
                }else{
                    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(constructor.enclosingMetaObject())){
                        jclass cls = dynamicMetaObject->javaClass();
                        if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, cls)){
                            return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, obj, cls);
                        }else{
                            return reinterpret_cast<jobject>(obj);
                        }
                    }else if(const std::type_info* typeId = getTypeByQtName(constructor.enclosingMetaObject()->className())){
                        return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, obj, *typeId);
                    }
                }
            }
        }
    }
    return nullptr;
}

jobject CoreAPI::metaObjectCast(JNIEnv *env, jobject object, jclass targetType){
    jobject result = nullptr;
    if(QSharedPointer<QtJambiLink> objectLink = QtJambiLink::findLinkForJavaInterface(env, object)){
        QString targetTypeJavaName = QtJambiAPI::getClassName(env, targetType).replace(QLatin1Char('.'), QLatin1Char('/'));
        if(const std::type_info* targetTypeId = getTypeByJavaName(targetTypeJavaName)){
            void* sourceTypePointer = objectLink->pointer();
            void* targetTypePointer = nullptr;
            if(objectLink->isQObject()){
                if(isInterface(*targetTypeId)){
                    if(const char* iid = registeredInterfaceIDForClassName(targetTypeJavaName)){
                        targetTypePointer = objectLink->qobject()->qt_metacast(iid);
                    }
                    if(!targetTypePointer){
                        for(const QExplicitlySharedDataPointer<const PolymorphicIdHandler>& handler : getPolymorphicIdHandlers(typeid(QObject))){
                            if(typeid_equals(handler->m_targetTypeId, *targetTypeId)){
                                qintptr offset(0);
                                if(handler->m_polymorphyHandler(sourceTypePointer, offset) && offset!=qintptr(sourceTypePointer)){
                                    targetTypePointer = reinterpret_cast<void*>(qintptr(sourceTypePointer) - offset);
                                    break;
                                }
                            }
                        }
                    }
                }
                if(!targetTypePointer)
                    targetTypePointer = objectLink->qobject()->qt_metacast(getQtName(*targetTypeId));
                if(!targetTypePointer){
                    targetTypePointer = objectLink->typedPointer(*targetTypeId);
                    if(targetTypePointer==sourceTypePointer)
                        targetTypePointer = nullptr;
                }
            }else{
                targetTypePointer = objectLink->typedPointer(*targetTypeId);
                if(targetTypePointer==sourceTypePointer){
                    targetTypePointer = nullptr;
                    if(Java::QtCore::QObject::isAssignableFrom(env, targetType)){
                        if(jclass sourceType = JavaAPI::resolveClosestQtSuperclass(env, env->GetObjectClass(object), nullptr)){
                            QString sourceTypeJavaName = QtJambiAPI::getClassName(env, sourceType).replace(QLatin1Char('.'), QLatin1Char('/'));
                            if(const std::type_info* sourceTypeId = getTypeByJavaName(sourceTypeJavaName)){
                                if(isInterface(*sourceTypeId)){
                                    const std::type_info& qobjectId = typeid(QObject);
                                    for(const QExplicitlySharedDataPointer<const PolymorphicIdHandler>& handler : getPolymorphicIdHandlers(*sourceTypeId)){
                                        if(typeid_equals(handler->m_targetTypeId, qobjectId)){
                                            qintptr offset(0);
                                            if(handler->m_polymorphyHandler(sourceTypePointer, offset) && offset!=qintptr(sourceTypePointer)){
                                                targetTypePointer = reinterpret_cast<void*>(qintptr(sourceTypePointer) - offset);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(targetTypePointer){
                if(targetTypePointer!=sourceTypePointer){
                    if(isQObject(*targetTypeId)){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(reinterpret_cast<QObject*>(targetTypePointer))){
                            if(link!=objectLink){
                                jobject obj = link->getJavaObjectLocalRef(env);
                                if(!obj){
                                    link->invalidate(env);
                                }else if(env->IsInstanceOf(obj, targetType)){
                                    return obj;
                                }
                            }
                        }
                    }else{
                        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(targetTypePointer)){
                            if(link && link!=objectLink){
                                jobject obj = link->getJavaObjectLocalRef(env);
                                if(!obj){
                                    link->invalidate(env);
                                }else if(env->IsInstanceOf(obj, targetType)){
                                    return obj;
                                }
                            }
                        }
                    }
                }else if(!objectLink->createdByJava() && objectLink->isQObject() && Java::QtCore::QObject::isAssignableFrom(env, targetType)){
                    if(jobject obj = objectLink->getJavaObjectLocalRef(env)){
                        if(!env->IsInstanceOf(obj, targetType)){
                            objectLink->invalidate(env);
                        }else return obj;
                    }
                }
                result = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, targetTypePointer, *targetTypeId);
                if(QSharedPointer<QtJambiLink> newlink = QtJambiLink::findLinkForJavaObject(env, result)){
                    objectLink->registerDependentObject(newlink);
                    if(!newlink->isQObject()){
                        newlink->registerPointer(newlink->pointer());
                    }
                }
            }
        }else{
            void* basicPointer = objectLink->pointer();
            void* interfacePointer = nullptr;
            if(objectLink->isQObject()){
                interfacePointer = objectLink->qobject()->qt_metacast(qPrintable(targetTypeJavaName.replace("/", "::").replace("$", "::")));
                if(interfacePointer){
                    if(interfacePointer!=basicPointer){
                        for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(interfacePointer)){
                            if(link && link!=objectLink){
                                jobject obj = link->getJavaObjectLocalRef(env);
                                if(!obj){
                                    link->invalidate(env);
                                }else if(env->IsInstanceOf(obj, targetType)){
                                    return obj;
                                }
                            }
                        }
                    }else if(!objectLink->createdByJava() && objectLink->isQObject() && Java::QtCore::QObject::isAssignableFrom(env, targetType)){
                        jobject obj = objectLink->getJavaObjectLocalRef(env);
                        if(!obj){
                            objectLink->invalidate(env);
                        }else if(env->IsInstanceOf(obj, targetType)){
                            return obj;
                        }
                    }
                    result = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, interfacePointer, targetType);
                    if(QSharedPointer<QtJambiLink> newlink = QtJambiLink::findLinkForJavaObject(env, result)){
                        objectLink->registerDependentObject(newlink);
                        if(!newlink->isQObject()){
                            newlink->registerPointer(newlink->pointer());
                        }
                    }
                }
            }
        }
    }
    return result;
}

QMetaType qtjambiMetaType(JNIEnv *env, jclass clazz, jobjectArray instantiations, QString* templateName);

struct ResolvingInternalToExternalConverter{
    ResolvingInternalToExternalConverter(jclass elementClass, const QMetaType& metaType)
        : m_elementClass(elementClass),
          m_metaType(metaType),
          m_converter() {}
    ResolvingInternalToExternalConverter(ResolvingInternalToExternalConverter&& other)
        : m_elementClass(other.m_elementClass),
          m_metaType(other.m_metaType),
          m_converter(other.m_converter) {
        other.m_converter = nullptr;
    }
    ResolvingInternalToExternalConverter& operator=(ResolvingInternalToExternalConverter&& other) {
        m_elementClass = other.m_elementClass;
        m_metaType = other.m_metaType;
        m_converter = other.m_converter;
        other.m_converter = nullptr;
        return *this;
    }

    bool operator()(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue& out, bool forceBoxedType)const{
        if(m_elementClass){
            m_converter = QtJambiTypeManager::getInternalToExternalConverter(
                                                              env,
                                                              m_metaType.name(),
                                                              m_metaType,
                                                              m_elementClass);
            m_elementClass = nullptr;
        }
        return m_converter(env, scope, in, out, forceBoxedType);
    }
    mutable jclass m_elementClass;
    QMetaType m_metaType;
    mutable QtJambiUtils::InternalToExternalConverter m_converter;
};

struct ResolvingExternalToInternalConverter{
    ResolvingExternalToInternalConverter(jclass elementClass, const QMetaType& metaType)
        : m_elementClass(elementClass),
          m_metaType(metaType),
          m_converter() {}
    ResolvingExternalToInternalConverter(ResolvingExternalToInternalConverter&& other)
        : m_elementClass(other.m_elementClass),
          m_metaType(other.m_metaType),
          m_converter(other.m_converter) {
        other.m_converter = nullptr;
    }
    ResolvingExternalToInternalConverter& operator=(ResolvingExternalToInternalConverter&& other) {
        m_elementClass = other.m_elementClass;
        m_metaType = other.m_metaType;
        m_converter = other.m_converter;
        other.m_converter = nullptr;
        return *this;
    }

    bool operator()(JNIEnv* env, QtJambiScope* scope, jvalue val, void* &out, jValueType valueType)const{
        if(m_elementClass){
            m_converter = QtJambiTypeManager::getExternalToInternalConverter(
                                                              env,
                                                              m_elementClass,
                                                              m_metaType.name(),
                                                              m_metaType);
            m_elementClass = nullptr;
        }
        return m_converter(env, scope, val, out, valueType);
    }
    mutable jclass m_elementClass;
    QMetaType m_metaType;
    mutable QtJambiUtils::ExternalToInternalConverter m_converter;
};

enum class SmartPointerType{
    None,
    QPointer = 0x1,
    QSharedPointer = 0x10,
    QWeakPointer,
    QScopedPointer,
    QScopedArrayPointer,
    QtPointersMask = 0xff,
    shared_ptr = 0x100,
    weak_ptr,
    unique_ptr,
    StdPointersMask = 0xf00
};

QMetaType registerSmartPointerMetaType(const QByteArray& typeName,
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
                                 QMetaType::TypeFlags flags);

QMetaType registerMetaType(JNIEnv* env,
                     const QMetaType& metaType,
                     PtrDeleterFunction deleter_function,
                     const std::type_info* typeId,
                     bool isPointer,
                     const QtJambiUtils::InternalToExternalConverter& memberConverter,
                     const QtJambiUtils::ExternalToInternalConverter& memberReConverter,
                     SmartPointerType smartPointerType,
                     const QString& typeName){
    Q_UNUSED(env)
    if(!metaType.isValid() && !deleter_function)
        return QMetaType(QMetaType::UnknownType);
    struct DeleterDeleter{
        PtrDeleterFunction deleter_function;
        void operator()(char* ptr){
            deleter_function(ptr, false);
        }
    };
    struct MetaTypeDeleter{
        QMetaType elementMetaType;
        void operator()(char* ptr){
            elementMetaType.destroy(ptr);
        }
    };

    switch(smartPointerType){
    //Scoped pointer is not possible to implement as meta typea as it is not cloneable.
#if defined(ALLOW_SCOPED_POINTER_METATYPE)
    case SmartPointerType::QScopedPointer:
    {
        QtJambiStorage* storage = getQtJambiStorage();
        {
            if(metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)){
                QMetaType newMetaType = registerSmartPointerMetaType(typeName.toLocal8Bit(),
                                                               [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                                   new(ptr)QScopedPointer<QObject>();
                                                               },
                                                               nullptr,
                                                               [](const QtPrivate::QMetaTypeInterface *, void *ptr, void *other){
                                                                   new(ptr)QScopedPointer<QObject>(reinterpret_cast<QScopedPointer<QObject>*>(other)->take());
                                                               },
                                                               [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                                   reinterpret_cast<QScopedPointer<QObject>*>(ptr)->~QScopedPointer();
                                                               },
                                                               [](const QtPrivate::QMetaTypeInterface *, const void *ptr1, const void *ptr2)->bool{
                                                                   return *reinterpret_cast<const QScopedPointer<QObject>*>(ptr1)==*reinterpret_cast<const QScopedPointer<QObject>*>(ptr2);
                                                               },
                                                               nullptr,
                                                               nullptr,//[](const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){},
                                                               nullptr,
                                                               nullptr,
                                                               nullptr,
                                                               uint(sizeof(QScopedPointer<QObject>)),
                                                               ushort(alignof(QScopedPointer<QObject>)),
                                                               QMetaType::UnknownType,
                                                               QMetaType::NeedsConstruction
                                                                   | QMetaType::NeedsDestruction
                                                                   | QMetaType::RelocatableType
                                                                   | QMetaType::NeedsCopyConstruction
                                                                   | QMetaType::NeedsMoveConstruction
                                                               );
                if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                    QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                        QScopedPointer<QObject>* p = reinterpret_cast<QScopedPointer<QObject>*>(target);
                        p->reset(*reinterpret_cast<QObject*const*>(src));
                        return true;
                    }, metaType, newMetaType);
                }
                QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                        QScopedPointer<QObject>* p = reinterpret_cast<QScopedPointer<QObject>*>(target);
                        const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                        if(JniEnvironment env{200}){
                            p->reset(QtJambiAPI::convertJavaObjectToQObject(env, jow.object(env)));
                        }
                        return true;
                    }, jobjectWrapperType, newMetaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jobjectWrapperType)){
                    QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper *>(target);
                        const QScopedPointer<QObject>* p = reinterpret_cast<const QScopedPointer<QObject>*>(src);
                        if(JniEnvironment env{200}){
                            jow = JObjectWrapper(env, QtJambiAPI::convertQObjectToJavaObject(env, p->get()));
                        }
                        return true;
                    }, newMetaType, jobjectWrapperType);
                }
                QMetaType jqobjectWrapperType = QMetaType::fromType<JQObjectWrapper>();
                if(!QMetaType::hasRegisteredConverterFunction(jqobjectWrapperType, newMetaType)){
                    QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                        QScopedPointer<QObject>* p = reinterpret_cast<QScopedPointer<QObject>*>(target);
                        const JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper const*>(src);
                        if(JniEnvironment env{200}){
                            p->reset(jow.qobject());
                        }
                        return true;
                    }, jqobjectWrapperType, newMetaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jqobjectWrapperType)){
                    QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                        JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper *>(target);
                        const QScopedPointer<QObject>* p = reinterpret_cast<const QScopedPointer<QObject>*>(src);
                        if(JniEnvironment env{200}){
                            QtJambiAPI::convertQObjectToJavaObject(env, p->get());
                            jow = JQObjectWrapper(env, QtJambiLink::findLinkForQObject(p->get()));
                        }
                        return true;
                    }, newMetaType, jqobjectWrapperType);
                }
                return newMetaType;
            }else if(!isPointer){
                QMetaType newMetaType{registerSmartPointerMetaType(typeName.toLocal8Bit(),
                                                               [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                                   new(ptr)QScopedPointer<char>();
                                                               },
                                                               nullptr,
                                                               [](const QtPrivate::QMetaTypeInterface *, void *ptr, void *other){
                                                                   new(ptr)QScopedPointer<char>(reinterpret_cast<QScopedPointer<char>*>(other)->take());
                                                               },
                                                               [](const QtPrivate::QMetaTypeInterface *iface, void *ptr){
                                                                   QScopedPointer<char>* p = reinterpret_cast<QScopedPointer<char>*>(ptr);
                                                                   void* data = p->take();
                                                                   p->~QScopedPointer();
                                                                   if(data){
                                                                       const QtPrivate::QMetaTypeInterface * elMt;
                                                                       QtJambiStorage* storage = getQtJambiStorage();
                                                                       {
                                                                           QReadLocker l(storage->lock());
                                                                           elMt = storage->elementMetaTypesOfSmartPointers()[iface];
                                                                       }
                                                                        QMetaType(elMt).destroy(data);
                                                                   }
                                                               },
                                                               [](const QtPrivate::QMetaTypeInterface *, const void *ptr1, const void *ptr2)->bool{
                                                                   return *reinterpret_cast<const QScopedPointer<char>*>(ptr1)==*reinterpret_cast<const QScopedPointer<char>*>(ptr2);
                                                               },
                                                               nullptr,
                                                               nullptr,//[](const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){},
                                                               nullptr,
                                                               nullptr,
                                                               nullptr,
                                                               uint(sizeof(QScopedPointer<char>)),
                                                               ushort(alignof(QScopedPointer<char>)),
                                                               QMetaType::UnknownType,
                                                               QMetaType::NeedsConstruction
                                                                   | QMetaType::NeedsDestruction
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
                                                                   | QMetaType::MovableType
#else
                                                                   | QMetaType::RelocatableType
                                                                   | QMetaType::NeedsCopyConstruction
                                                                   | QMetaType::NeedsMoveConstruction
#endif
                                                               )};
                {
                    QWriteLocker l(storage->lock());
                    storage->elementMetaTypesOfSmartPointers()[META_TYPE_ACCESS(newMetaType).iface()] = META_TYPE_ACCESS(metaType).iface();
                }
                if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                    QMetaType::registerConverterFunction([metaType](const void *src, void *target)->bool{
                        QScopedPointer<char>* p = reinterpret_cast<QScopedPointer<char>*>(target);
                        void* data = p->take();
                        p->reset(reinterpret_cast<char*>(metaType.create(src)));
                        metaType.destroy(data);
                        return true;
                    }, metaType, newMetaType);
                }
                QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    QMetaType::registerConverterFunction([metaType, memberReConverter](const void *src, void *target)->bool{
                        QScopedPointer<char>* p = reinterpret_cast<QScopedPointer<char>*>(target);
                        const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                        if(JniEnvironment env{200}){
                            jvalue jv;
                            jv.l = jow.object(env);
                            void* out{metaType.create()};
                            if(memberReConverter(env, nullptr, jv, out, jValueType::l)){
                                void* data = p->take();
                                p->reset(reinterpret_cast<char*>(out));
                                metaType.destroy(data);
                            }else{
                                metaType.destroy(out);
                            }
                        }
                        return true;
                    }, jobjectWrapperType, newMetaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    QMetaType::registerConverterFunction([memberConverter](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        const QScopedPointer<char>* p = reinterpret_cast<QScopedPointer<char> const*>(src);
                        if(JniEnvironment env{200}){
                            jvalue jv;
                            jv.l = nullptr;
                            if(memberConverter(env, nullptr, p->data(), jv, true)){
                                jow = JObjectWrapper(env, jv.l);
                            }
                        }
                        return true;
                    }, newMetaType, jobjectWrapperType);
                }
                return newMetaType;
            }else if(deleter_function){
                QMetaType newMetaType = registerSmartPointerMetaType(typeName.toLocal8Bit(),
                                                               [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                                   new(ptr)QScopedPointer<char>();
                                                               },
                                                               nullptr,
                                                               [](const QtPrivate::QMetaTypeInterface *, void *ptr, void *other){
                                                                   new(ptr)QScopedPointer<char>(reinterpret_cast<QScopedPointer<char>*>(other)->take());
                                                               },
                                                               [](const QtPrivate::QMetaTypeInterface *iface, void *ptr){
                                                                   QScopedPointer<char>* p = reinterpret_cast<QScopedPointer<char>*>(ptr);
                                                                   void* data = p->take();
                                                                   p->~QScopedPointer();
                                                                   if(data){
                                                                       PtrDeleterFunction deleterFn;
                                                                       QtJambiStorage* storage = getQtJambiStorage();
                                                                       {
                                                                           QReadLocker l(storage->lock());
                                                                           deleterFn = storage->elementDeletersOfSmartPointers()[iface];
                                                                       }
                                                                       deleterFn(data,false);
                                                                   }
                                                               },
                                                               [](const QtPrivate::QMetaTypeInterface *, const void *ptr1, const void *ptr2)->bool{
                                                                   return *reinterpret_cast<const QScopedPointer<char>*>(ptr1)==*reinterpret_cast<const QScopedPointer<char>*>(ptr2);
                                                               },
                                                               nullptr,
                                                               nullptr,//[](const QtPrivate::QMetaTypeInterface *iface, QDebug &s, const void *ptr){},
                                                               nullptr,
                                                               nullptr,
                                                               nullptr,
                                                               uint(sizeof(QScopedPointer<char>)),
                                                               ushort(alignof(QScopedPointer<char>)),
                                                               QMetaType::UnknownType,
                                                               QMetaType::NeedsConstruction
                                                                   | QMetaType::NeedsDestruction
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
                                                                   | QMetaType::MovableType
#else
                                                                   | QMetaType::RelocatableType
                                                                   | QMetaType::NeedsCopyConstruction
                                                                   | QMetaType::NeedsMoveConstruction
#endif
                                                               );
                {
                    QWriteLocker l(storage->lock());
                    storage->elementDeletersOfSmartPointers()[newMetaType.iface()] = deleter_function;
                }
                if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                    QMetaType::registerConverterFunction([metaType](const void *src, void *target)->bool{
                        QScopedPointer<char>* p = reinterpret_cast<QScopedPointer<char>*>(target);
                        void* data = p->take();
                        p->reset(reinterpret_cast<char*>(metaType.create(src)));
                        metaType.destroy(data);
                        return true;
                    }, metaType, newMetaType);
                }
                QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    QMetaType::registerConverterFunction([metaType, memberReConverter](const void *src, void *target)->bool{
                        QScopedPointer<char>* p = reinterpret_cast<QScopedPointer<char>*>(target);
                        const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                        if(JniEnvironment env{200}){
                            jvalue jv;
                            jv.l = jow.object(env);
                            void* out{metaType.create()};
                            if(memberReConverter(env, nullptr, jv, out, jValueType::l)){
                                void* data = p->take();
                                p->reset(reinterpret_cast<char*>(out));
                                metaType.destroy(data);
                            }else{
                                metaType.destroy(out);
                            }
                        }
                        return true;
                    }, jobjectWrapperType, newMetaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    QMetaType::registerConverterFunction([memberConverter](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        const QScopedPointer<char>* p = reinterpret_cast<QScopedPointer<char> const*>(src);
                        if(JniEnvironment env{200}){
                            jvalue jv;
                            jv.l = nullptr;
                            if(memberConverter(env, nullptr, p->data(), jv, true)){
                                jow = JObjectWrapper(env, jv.l);
                            }
                        }
                        return true;
                    }, newMetaType, jobjectWrapperType);
                }
                return newMetaType;
            }
        }
    }
#endif
    case SmartPointerType::QPointer:{
        if(metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)){
            QMetaType newMetaType;
            QMetaType basicType = QMetaType::fromType<QPointer<QObject>>();
            if(metaType.id()==QMetaType::QObjectStar){
                newMetaType = basicType;
            }else{
                newMetaType = registerSmartPointerMetaType(typeName.toLocal8Bit(),
                                                           basicType.iface()->defaultCtr,
                                                           basicType.iface()->copyCtr,
                                                           basicType.iface()->moveCtr,
                                                           basicType.iface()->dtor,
                                                           basicType.iface()->equals,
                                                           basicType.iface()->lessThan,
                                                           basicType.iface()->debugStream,
                                                           basicType.iface()->dataStreamOut,
                                                           basicType.iface()->dataStreamIn,
                                                           basicType.iface()->legacyRegisterOp,
                                                           basicType.iface()->size,
                                                           basicType.iface()->alignment,
                                                           QMetaType::UnknownType,
                                                           QMetaType::TypeFlags(basicType.iface()->flags)
                                                           );
            }
            if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QPointer<QObject>* p = reinterpret_cast<QPointer<QObject>*>(target);
                    *p = QPointer<QObject>(*reinterpret_cast<QObject*const*>(src));
                    return true;
                }, metaType, newMetaType);
            }
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QPointer<QObject>* p = reinterpret_cast<QPointer<QObject>*>(target);
                    const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        *p = QPointer<QObject>(QtJambiAPI::convertJavaObjectToQObject(env, jow.object(env)));
                    }
                    return true;
                }, jobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper *>(target);
                    const QPointer<QObject>* p = reinterpret_cast<const QPointer<QObject>*>(src);
                    if(JniEnvironment env{200}){
                        jow = JObjectWrapper(env, QtJambiAPI::convertQObjectToJavaObject(env, p->get()));
                    }
                    return true;
                }, newMetaType, jobjectWrapperType);
            }
            QMetaType jqobjectWrapperType = QMetaType::fromType<JQObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jqobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QPointer<QObject>* p = reinterpret_cast<QPointer<QObject>*>(target);
                    const JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        *p = QPointer<QObject>(jow.qobject());
                    }
                    return true;
                }, jqobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jqobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper *>(target);
                    const QPointer<QObject>* p = reinterpret_cast<const QPointer<QObject>*>(src);
                    if(JniEnvironment env{200}){
                        QtJambiAPI::convertQObjectToJavaObject(env, p->get());
                        jow = JQObjectWrapper(env, QtJambiLink::findLinkForQObject(p->get()));
                    }
                    return true;
                }, newMetaType, jqobjectWrapperType);
            }
            return newMetaType;
        }
        return QMetaType();
    }
    case SmartPointerType::QWeakPointer:{
        QMetaType newMetaType = registerSmartPointerMetaType(typeName.toLocal8Bit(),
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                           new(ptr)QWeakPointer<char>();
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr, const void *other){
                                                           new(ptr)QWeakPointer<char>(*reinterpret_cast<const QWeakPointer<char>*>(other));
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr, void *other){
                                                           new(ptr)QWeakPointer<char>(std::move(*reinterpret_cast<QWeakPointer<char>*>(other)));
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                           reinterpret_cast<QWeakPointer<char>*>(ptr)->~QWeakPointer();
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, const void *ptr1, const void *ptr2)->bool{
                                                           return *reinterpret_cast<const QWeakPointer<char>*>(ptr1)==*reinterpret_cast<const QWeakPointer<char>*>(ptr2);
                                                       },
                                                       nullptr,
                                                       nullptr,
                                                       nullptr,
                                                       nullptr,
                                                       nullptr,
                                                       uint(sizeof(QWeakPointer<char>)),
                                                       ushort(alignof(QWeakPointer<char>)),
                                                       QMetaType::UnknownType,
                                                       QMetaType::NeedsConstruction
                                                           | QMetaType::NeedsDestruction
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
                                                           | QMetaType::MovableType
#else
                                                           | QMetaType::RelocatableType
                                                           | QMetaType::NeedsCopyConstruction
                                                           | QMetaType::NeedsMoveConstruction
                                                           | ((metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)) ? QMetaType::WeakPointerToQObject : QMetaType::TypeFlag(0))
#endif
                                                       );
        if(metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)){
            if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(*reinterpret_cast<QObject*const*>(src))){
                        QWeakPointer<QObject>* p = reinterpret_cast<QWeakPointer<QObject>*>(target);
                        if(JniEnvironment env{200}){
                            *p = QtJambiAPI::convertJavaObjectToSmartPointer<QSharedPointer,QObject>(env, link->getJavaObjectLocalRef(env));
                            return true;
                        }
                    }
                    return false;
                }, metaType, newMetaType);
            }
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    if(JniEnvironment env{200}){
                        QWeakPointer<QObject>* p = reinterpret_cast<QWeakPointer<QObject>*>(target);
                        const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                        *p = QtJambiAPI::convertJavaObjectToSmartPointer<QSharedPointer,QObject>(env, jow.object(env));
                        return true;
                    }
                    return false;
                }, jobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    if(JniEnvironment env{200}){
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper *>(target);
                        QSharedPointer<QObject> p = *reinterpret_cast<const QWeakPointer<QObject>*>(src);
                        jow = JObjectWrapper(env, QtJambiAPI::convertSmartPointerToJavaObject(env, p));
                        return true;
                    }
                    return false;
                }, newMetaType, jobjectWrapperType);
            }
            QMetaType jqobjectWrapperType = QMetaType::fromType<JQObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jqobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QWeakPointer<QObject>* p = reinterpret_cast<QWeakPointer<QObject>*>(target);
                    const JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        *p = QtJambiAPI::convertJavaObjectToSmartPointer<QSharedPointer,QObject>(env, jow.javaObject(env));
                        return true;
                    }
                    return false;
                }, jqobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jqobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper *>(target);
                    QSharedPointer<QObject> p = *reinterpret_cast<const QWeakPointer<QObject>*>(src);
                    if(JniEnvironment env{200}){
                        QtJambiAPI::convertSmartPointerToJavaObject(env, p);
                        jow = JQObjectWrapper(env, QtJambiLink::findLinkForQObject(p.get()));
                    }
                    return true;
                }, newMetaType, jqobjectWrapperType);
            }
        }else{
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                if(typeId){
                    QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        QSharedPointer<char> p = *reinterpret_cast<QWeakPointer<char> const*>(src);
                        if(JniEnvironment env{200}){
                            jow = JObjectWrapper(env, QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, p));
                            return true;
                        }
                        return false;
                    }, newMetaType, jobjectWrapperType);
                }else{
                    QMetaType::registerConverterFunction([memberConverter](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        QSharedPointer<char> p = *reinterpret_cast<QWeakPointer<char> const*>(src);
                        if(JniEnvironment env{200}){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(p.get()).value(0)){
                                jow = JObjectWrapper(env, link->getJavaObjectLocalRef(env));
                                return true;
                            }
                            jvalue jv;
                            jv.l = nullptr;
                            if(memberConverter(env, nullptr, p.data(), jv, true)){
                                jow = JObjectWrapper(env, jv.l);
                            }
                            return true;
                        }
                        return false;
                    }, newMetaType, jobjectWrapperType);
                }
            }
            if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                    QWeakPointer<char>* p = reinterpret_cast<QWeakPointer<char>*>(target);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(*reinterpret_cast<void*const*>(src)).value(0)){
                        if(JniEnvironment env{200}){
                            if(link->isSmartPointer()){
                                *p = QtJambiAPI::convertJavaObjectToQWeakPointer(env, typeId, link->getJavaObjectLocalRef(env));
                                return true;
                            }
                        }
                    }
                    return false;
                }, metaType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                    QWeakPointer<char>* p = reinterpret_cast<QWeakPointer<char>*>(target);
                    const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, jow.object(env))){
                            *p = QtJambiAPI::convertJavaObjectToQWeakPointer(env, typeId, link->getJavaObjectLocalRef(env));
                            return true;
                        }
                    }
                    return false;
                }, jobjectWrapperType, newMetaType);
            }
        }
        return newMetaType;
    }
    case SmartPointerType::QSharedPointer:{
        QMetaType newMetaType = registerSmartPointerMetaType(typeName.toLocal8Bit(),
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                           new(ptr)QSharedPointer<char>();
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr, const void *other){
                                                           new(ptr)QSharedPointer<char>(*reinterpret_cast<const QSharedPointer<char>*>(other));
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr, void *other){
                                                           new(ptr)QSharedPointer<char>(std::move(*reinterpret_cast<QSharedPointer<char>*>(other)));
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                           QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char>*>(ptr);
                                                           p->~QSharedPointer();
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, const void *ptr1, const void *ptr2)->bool{
                                                           return *reinterpret_cast<const QSharedPointer<char>*>(ptr1)==*reinterpret_cast<const QSharedPointer<char>*>(ptr2);
                                                       },
                                                       nullptr,
                                                       [](const QtPrivate::QMetaTypeInterface *iface, QDebug &debug, const void *ptr){
                                                           void* data = reinterpret_cast<const QSharedPointer<char>*>(ptr)->data();
                                                           QDebugStateSaver saver(debug);
                                                           if(iface->flags & QMetaType::SharedPointerToQObject){
                                                               debug.nospace() << "QSharedPointer(" << reinterpret_cast<QObject*>(data) << ")";
                                                           }else{
                                                               debug.nospace() << "QSharedPointer(" << data << ")";
                                                           }
                                                       },
                                                       nullptr,
                                                       nullptr,
                                                       nullptr,
                                                       uint(sizeof(QSharedPointer<char>)),
                                                       ushort(alignof(QSharedPointer<char>)),
                                                       QMetaType::UnknownType,
                                                       QMetaType::NeedsConstruction
                                                           | QMetaType::NeedsDestruction
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
                                                           | QMetaType::MovableType
#else
                                                           | QMetaType::RelocatableType
                                                           | QMetaType::NeedsCopyConstruction
                                                           | QMetaType::NeedsMoveConstruction
                                                           | ((metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)) ? QMetaType::SharedPointerToQObject : QMetaType::TypeFlag(0))
#endif
                                                       );
        if(metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)){
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, metaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QObject*& p = *reinterpret_cast<QObject**>(target);
                    p = reinterpret_cast<const QSharedPointer<QObject>*>(src)->data();
                    return true;
                }, newMetaType, metaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QSharedPointer<QObject>* p = reinterpret_cast<QSharedPointer<QObject>*>(target);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(*reinterpret_cast<QObject*const*>(src))){
                        if(JniEnvironment env{200}){
                            *p = QtJambiAPI::convertJavaObjectToSmartPointer<QSharedPointer,QObject>(env, link->getJavaObjectLocalRef(env));
                        }else return false;
                    }else{
                        p->reset(*reinterpret_cast<QObject*const*>(src));
                    }
                    return true;
                }, metaType, newMetaType);
            }
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QSharedPointer<QObject>* p = reinterpret_cast<QSharedPointer<QObject>*>(target);
                    const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        *p = QtJambiAPI::convertJavaObjectToSmartPointer<QSharedPointer,QObject>(env, jow.object(env));
                    }
                    return true;
                }, jobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper *>(target);
                    const QSharedPointer<QObject>* p = reinterpret_cast<const QSharedPointer<QObject>*>(src);
                    if(JniEnvironment env{200}){
                        jow = JObjectWrapper(env, QtJambiAPI::convertSmartPointerToJavaObject(env, *p));
                    }
                    return true;
                }, newMetaType, jobjectWrapperType);
            }
            QMetaType jqobjectWrapperType = QMetaType::fromType<JQObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jqobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QSharedPointer<QObject>* p = reinterpret_cast<QSharedPointer<QObject>*>(target);
                    const JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        *p = QtJambiAPI::convertJavaObjectToSmartPointer<QSharedPointer,QObject>(env, jow.javaObject(env));
                    }
                    return true;
                }, jqobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jqobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper *>(target);
                    const QSharedPointer<QObject>* p = reinterpret_cast<const QSharedPointer<QObject>*>(src);
                    if(JniEnvironment env{200}){
                        QtJambiAPI::convertSmartPointerToJavaObject(env, *p);
                        jow = JQObjectWrapper(env, QtJambiLink::findLinkForQObject(p->get()));
                    }
                    return true;
                }, newMetaType, jqobjectWrapperType);
            }
        }else{
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jobjectWrapperType)){
                if(typeId){
                    QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        const QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char> const*>(src);
                        if(JniEnvironment env{200}){
                            jow = JObjectWrapper(env, QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *p));
                            return true;
                        }
                        return false;
                    }, newMetaType, jobjectWrapperType);
                }else{
                    QMetaType::registerConverterFunction([memberConverter](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        const QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char> const*>(src);
                        if(JniEnvironment env{200}){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(p->get()).value(0)){
                                jow = JObjectWrapper(env, link->getJavaObjectLocalRef(env));
                                return true;
                            }
                            jvalue jv;
                            jv.l = nullptr;
                            if(memberConverter(env, nullptr, p->data(), jv, true)){
                                jow = JObjectWrapper(env, jv.l);
                            }
                            return true;
                        }
                        return false;
                    }, newMetaType, jobjectWrapperType);
                }
            }
            if(!isPointer && metaType.isValid()){
                if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                    QMetaType::registerConverterFunction([metaType, typeId](const void *src, void *target)->bool{
                        QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char>*>(target);
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(*reinterpret_cast<void*const*>(src)).value(0)){
                            if(JniEnvironment env{200}){
                                jobject jobj = link->getJavaObjectLocalRef(env);
                                link.clear();
                                *p = QtJambiAPI::convertJavaObjectToQSharedPointer(env, typeId, jobj);
                                return true;
                            }
                        }
                        p->reset(reinterpret_cast<char*>(metaType.create(src)), MetaTypeDeleter{metaType});
                        return true;
                    }, metaType, newMetaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(newMetaType, metaType)){
                    QMetaType::registerConverterFunction([metaType](const void *src, void *target)->bool{
                        const QSharedPointer<char>* p = reinterpret_cast<const QSharedPointer<char>*>(src);
                        metaType.construct(target, p->get());
                        return true;
                    }, newMetaType, metaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    if(typeId){
                        QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                            QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char>*>(target);
                            const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                            if(JniEnvironment env{200}){
                                *p = QtJambiAPI::convertJavaObjectToQSharedPointer(env, typeId, jow.object(env));
                                return true;
                            }
                            return false;
                        }, jobjectWrapperType, newMetaType);
                    }else{
                        QMetaType::registerConverterFunction([metaType, memberReConverter](const void *src, void *target)->bool{
                            QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char>*>(target);
                            const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                            if(JniEnvironment env{200}){
                                jvalue jv;
                                jv.l = jow.object(env);
                                void* out{metaType.create()};
                                if(memberReConverter(env, nullptr, jv, out, jValueType::l)){
                                    p->reset(reinterpret_cast<char*>(out), MetaTypeDeleter{metaType});
                                }else{
                                    metaType.destroy(out);
                                }
                                return true;
                            }
                            return false;
                        }, jobjectWrapperType, newMetaType);
                    }
                }
            }else if(deleter_function){
                if(metaType.isValid()){
                    if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                        QMetaType::registerConverterFunction([deleter_function, typeId](const void *src, void *target)->bool{
                            QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char>*>(target);
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(*reinterpret_cast<void*const*>(src)).value(0)){
                                if(JniEnvironment env{200}){
                                    jobject jobj = link->getJavaObjectLocalRef(env);
                                    link.clear();
                                    *p = QtJambiAPI::convertJavaObjectToQSharedPointer(env, typeId, jobj);
                                    return true;
                                }
                            }
                            p->reset(*reinterpret_cast<char*const*>(src), DeleterDeleter{deleter_function});
                            return true;
                        }, metaType, newMetaType);
                    }
                    if(!QMetaType::hasRegisteredConverterFunction(newMetaType, metaType)){
                        QMetaType::registerConverterFunction([metaType](const void *src, void *target)->bool{
                            const QSharedPointer<char>* p = reinterpret_cast<const QSharedPointer<char>*>(src);
                            void* ptr = p->get();
                            metaType.construct(target, &ptr);
                            return true;
                        }, newMetaType, metaType);
                    }
                }
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    if(typeId){
                        QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                            QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char>*>(target);
                            const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                            if(JniEnvironment env{200}){
                                *p = QtJambiAPI::convertJavaObjectToQSharedPointer(env, typeId, jow.object(env));
                                return true;
                            }
                            return false;
                        }, jobjectWrapperType, newMetaType);
                    }else{
                        QMetaType::registerConverterFunction([deleter_function, memberReConverter](const void *src, void *target)->bool{
                            QSharedPointer<char>* p = reinterpret_cast<QSharedPointer<char>*>(target);
                            const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                            if(JniEnvironment env{200}){
                                jvalue jv;
                                jv.l = jow.object(env);
                                char* ptr{nullptr};
                                void* out{&ptr};
                                if(memberReConverter(env, nullptr, jv, out, jValueType::l)){
                                    p->reset(ptr, DeleterDeleter{deleter_function});
                                }
                                return true;
                            }
                            return false;
                        }, jobjectWrapperType, newMetaType);
                    }
                }
            }
        }
        return newMetaType;
    }
    case SmartPointerType::weak_ptr:{
        QMetaType newMetaType = registerSmartPointerMetaType(typeName.toLocal8Bit(),
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                           new(ptr)std::weak_ptr<char>();
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr, const void *other){
                                                           new(ptr)std::weak_ptr<char>(*reinterpret_cast<const std::weak_ptr<char>*>(other));
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr, void *other){
                                                           new(ptr)std::weak_ptr<char>(std::move(*reinterpret_cast<std::weak_ptr<char>*>(other)));
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                           reinterpret_cast<std::weak_ptr<char>*>(ptr)->~weak_ptr();
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, const void *ptr1, const void *ptr2)->bool{
                                                           return std::shared_ptr<char>(*reinterpret_cast<const std::weak_ptr<char>*>(ptr1))==std::shared_ptr<char>(*reinterpret_cast<const std::weak_ptr<char>*>(ptr2));
                                                       },
                                                       nullptr,
                                                       nullptr,
                                                       nullptr,
                                                       nullptr,
                                                       nullptr,
                                                       uint(sizeof(std::weak_ptr<char>)),
                                                       ushort(alignof(std::weak_ptr<char>)),
                                                       QMetaType::UnknownType,
                                                       QMetaType::NeedsConstruction
                                                           | QMetaType::NeedsDestruction
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
                                                           | QMetaType::MovableType
#else
                                                           | QMetaType::RelocatableType
                                                           | QMetaType::NeedsCopyConstruction
                                                           | QMetaType::NeedsMoveConstruction
                                                           | ((metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)) ? QMetaType::WeakPointerToQObject : QMetaType::TypeFlag(0))
#endif
                                                       );
        if(metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)){
            if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(*reinterpret_cast<QObject*const*>(src))){
                        std::weak_ptr<QObject>* p = reinterpret_cast<std::weak_ptr<QObject>*>(target);
                        if(JniEnvironment env{200}){
                            *p = QtJambiAPI::convertJavaObjectToSmartPointer<std::shared_ptr,QObject>(env, link->getJavaObjectLocalRef(env));
                            return true;
                        }
                    }
                    return false;
                }, metaType, newMetaType);
            }
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    if(JniEnvironment env{200}){
                        std::weak_ptr<QObject>* p = reinterpret_cast<std::weak_ptr<QObject>*>(target);
                        const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                        *p = QtJambiAPI::convertJavaObjectToSmartPointer<std::shared_ptr,QObject>(env, jow.object(env));
                        return true;
                    }
                    return false;
                }, jobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    if(JniEnvironment env{200}){
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper *>(target);
                        std::shared_ptr<QObject> p(*reinterpret_cast<const std::weak_ptr<QObject>*>(src));
                        jow = JObjectWrapper(env, QtJambiAPI::convertSmartPointerToJavaObject(env, p));
                        return true;
                    }
                    return false;
                }, newMetaType, jobjectWrapperType);
            }
            QMetaType jqobjectWrapperType = QMetaType::fromType<JQObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jqobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    std::weak_ptr<QObject>* p = reinterpret_cast<std::weak_ptr<QObject>*>(target);
                    const JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        *p = QtJambiAPI::convertJavaObjectToSmartPointer<std::shared_ptr,QObject>(env, jow.javaObject(env));
                        return true;
                    }
                    return false;
                }, jqobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jqobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper *>(target);
                    std::shared_ptr<QObject> p(*reinterpret_cast<const std::weak_ptr<QObject>*>(src));
                    if(JniEnvironment env{200}){
                        QtJambiAPI::convertSmartPointerToJavaObject(env, p);
                        jow = JQObjectWrapper(env, QtJambiLink::findLinkForQObject(p.get()));
                    }
                    return true;
                }, newMetaType, jqobjectWrapperType);
            }
        }else{
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                if(typeId){
                    QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        std::shared_ptr<char> p(*reinterpret_cast<std::weak_ptr<char> const*>(src));
                        if(JniEnvironment env{200}){
                            jow = JObjectWrapper(env, QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, p));
                            return true;
                        }
                        return false;
                    }, newMetaType, jobjectWrapperType);
                }else{
                    QMetaType::registerConverterFunction([memberConverter](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        std::shared_ptr<char> p(*reinterpret_cast<std::weak_ptr<char> const*>(src));
                        if(JniEnvironment env{200}){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(p.get()).value(0)){
                                jow = JObjectWrapper(env, link->getJavaObjectLocalRef(env));
                                return true;
                            }
                            jvalue jv;
                            jv.l = nullptr;
                            if(memberConverter(env, nullptr, p.get(), jv, true)){
                                jow = JObjectWrapper(env, jv.l);
                            }
                            return true;
                        }
                        return false;
                    }, newMetaType, jobjectWrapperType);
                }
            }
            if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                    std::weak_ptr<char>* p = reinterpret_cast<std::weak_ptr<char>*>(target);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(*reinterpret_cast<void*const*>(src)).value(0)){
                        if(JniEnvironment env{200}){
                            if(link->isSmartPointer()){
                                *p = QtJambiAPI::convertJavaObjectToWeakPtr(env, typeId, link->getJavaObjectLocalRef(env));
                                return true;
                            }
                        }
                    }
                    return false;
                }, metaType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                    std::weak_ptr<char>* p = reinterpret_cast<std::weak_ptr<char>*>(target);
                    const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, jow.object(env))){
                            *p = QtJambiAPI::convertJavaObjectToWeakPtr(env, typeId, link->getJavaObjectLocalRef(env));
                            return true;
                        }
                    }
                    return false;
                }, jobjectWrapperType, newMetaType);
            }
        }
        return newMetaType;
    }
    case SmartPointerType::shared_ptr:{
        QMetaType newMetaType = registerSmartPointerMetaType(typeName.toLocal8Bit(),
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                           new(ptr)std::shared_ptr<char>();
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr, const void *other){
                                                           new(ptr)std::shared_ptr<char>(*reinterpret_cast<const std::shared_ptr<char>*>(other));
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr, void *other){
                                                           new(ptr)std::shared_ptr<char>(std::move(*reinterpret_cast<std::shared_ptr<char>*>(other)));
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, void *ptr){
                                                           std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char>*>(ptr);
                                                           p->~shared_ptr();
                                                       },
                                                       [](const QtPrivate::QMetaTypeInterface *, const void *ptr1, const void *ptr2)->bool{
                                                           return *reinterpret_cast<const std::shared_ptr<char>*>(ptr1)==*reinterpret_cast<const std::shared_ptr<char>*>(ptr2);
                                                       },
                                                       nullptr,
                                                       [](const QtPrivate::QMetaTypeInterface *iface, QDebug &debug, const void *ptr){
                                                           void* data = reinterpret_cast<const std::shared_ptr<char>*>(ptr)->get();
                                                           QDebugStateSaver saver(debug);
                                                           if(iface->flags & QMetaType::SharedPointerToQObject){
                                                               debug.nospace() << "std::shared_ptr(" << reinterpret_cast<QObject*>(data) << ")";
                                                           }else{
                                                               debug.nospace() << "std::shared_ptr(" << data << ")";
                                                           }
                                                       },
                                                       nullptr,
                                                       nullptr,
                                                       nullptr,
                                                       uint(sizeof(std::shared_ptr<char>)),
                                                       ushort(alignof(std::shared_ptr<char>)),
                                                       QMetaType::UnknownType,
                                                       QMetaType::NeedsConstruction
                                                           | QMetaType::NeedsDestruction
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
                                                           | QMetaType::MovableType
#else
                                                           | QMetaType::RelocatableType
                                                           | QMetaType::NeedsCopyConstruction
                                                           | QMetaType::NeedsMoveConstruction
                                                           | ((metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)) ? QMetaType::SharedPointerToQObject : QMetaType::TypeFlag(0))
#endif
                                                       );
        if(metaType.metaObject() && metaType.metaObject()->inherits(&QObject::staticMetaObject)){
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, metaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    QObject*& p = *reinterpret_cast<QObject**>(target);
                    p = reinterpret_cast<const std::shared_ptr<QObject>*>(src)->get();
                    return true;
                }, newMetaType, metaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    std::shared_ptr<QObject>* p = reinterpret_cast<std::shared_ptr<QObject>*>(target);
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(*reinterpret_cast<QObject*const*>(src))){
                        if(JniEnvironment env{200}){
                            *p = QtJambiAPI::convertJavaObjectToSmartPointer<std::shared_ptr,QObject>(env, link->getJavaObjectLocalRef(env));
                        }else return false;
                    }else{
                        p->reset(*reinterpret_cast<QObject*const*>(src));
                    }
                    return true;
                }, metaType, newMetaType);
            }
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    std::shared_ptr<QObject>* p = reinterpret_cast<std::shared_ptr<QObject>*>(target);
                    const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        *p = QtJambiAPI::convertJavaObjectToSmartPointer<std::shared_ptr,QObject>(env, jow.object(env));
                    }
                    return true;
                }, jobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper *>(target);
                    const std::shared_ptr<QObject>* p = reinterpret_cast<const std::shared_ptr<QObject>*>(src);
                    if(JniEnvironment env{200}){
                        jow = JObjectWrapper(env, QtJambiAPI::convertSmartPointerToJavaObject(env, *p));
                    }
                    return true;
                }, newMetaType, jobjectWrapperType);
            }
            QMetaType jqobjectWrapperType = QMetaType::fromType<JQObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jqobjectWrapperType, newMetaType)){
                QMetaType::registerConverterFunction([](const void *src, void *target)->bool{
                    std::shared_ptr<QObject>* p = reinterpret_cast<std::shared_ptr<QObject>*>(target);
                    const JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper const*>(src);
                    if(JniEnvironment env{200}){
                        *p = QtJambiAPI::convertJavaObjectToSmartPointer<std::shared_ptr,QObject>(env, jow.javaObject(env));
                    }
                    return true;
                }, jqobjectWrapperType, newMetaType);
            }
            if(!QMetaType::hasRegisteredConverterFunction(newMetaType, jqobjectWrapperType)){
                QMetaType::registerConverterFunction([memberReConverter](const void *src, void *target)->bool{
                    JQObjectWrapper& jow = *reinterpret_cast<JQObjectWrapper *>(target);
                    const std::shared_ptr<QObject>* p = reinterpret_cast<const std::shared_ptr<QObject>*>(src);
                    if(JniEnvironment env{200}){
                        QtJambiAPI::convertSmartPointerToJavaObject(env, *p);
                        jow = JQObjectWrapper(env, QtJambiLink::findLinkForQObject(p->get()));
                    }
                    return true;
                }, newMetaType, jqobjectWrapperType);
            }
        }else{
            QMetaType jobjectWrapperType = QMetaType::fromType<JObjectWrapper>();
            if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                if(typeId){
                    QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        const std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char> const*>(src);
                        if(JniEnvironment env{200}){
                            jow = JObjectWrapper(env, QtJambiAPI::convertSmartPointerToJavaObject(env, *typeId, *p));
                            return true;
                        }
                        return false;
                    }, newMetaType, jobjectWrapperType);
                }else{
                    QMetaType::registerConverterFunction([memberConverter](const void *src, void *target)->bool{
                        JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper*>(target);
                        const std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char> const*>(src);
                        if(JniEnvironment env{200}){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(p->get()).value(0)){
                                jow = JObjectWrapper(env, link->getJavaObjectLocalRef(env));
                                return true;
                            }
                            jvalue jv;
                            jv.l = nullptr;
                            if(memberConverter(env, nullptr, p->get(), jv, true)){
                                jow = JObjectWrapper(env, jv.l);
                            }
                            return true;
                        }
                        return false;
                    }, newMetaType, jobjectWrapperType);
                }
            }
            if(!isPointer && metaType.isValid()){
                if(!QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                    QMetaType::registerConverterFunction([metaType, typeId](const void *src, void *target)->bool{
                        std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char>*>(target);
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(*reinterpret_cast<void*const*>(src)).value(0)){
                            if(JniEnvironment env{200}){
                                jobject jobj = link->getJavaObjectLocalRef(env);
                                link.clear();
                                *p = QtJambiAPI::convertJavaObjectToSharedPtr(env, typeId, jobj);
                                return true;
                            }
                        }
                        p->reset(reinterpret_cast<char*>(metaType.create(src)), MetaTypeDeleter{metaType});
                        return true;
                    }, metaType, newMetaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    if(typeId){
                        QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                            std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char>*>(target);
                            const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                            if(JniEnvironment env{200}){
                                *p = QtJambiAPI::convertJavaObjectToSharedPtr(env, typeId, jow.object(env));
                                return true;
                            }
                            return false;
                        }, jobjectWrapperType, newMetaType);
                    }else{
                        QMetaType::registerConverterFunction([metaType, memberReConverter](const void *src, void *target)->bool{
                            std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char>*>(target);
                            const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                            if(JniEnvironment env{200}){
                                jvalue jv;
                                jv.l = jow.object(env);
                                void* out{metaType.create()};
                                if(memberReConverter(env, nullptr, jv, out, jValueType::l)){
                                    p->reset(reinterpret_cast<char*>(out), MetaTypeDeleter{metaType});
                                }else{
                                    metaType.destroy(out);
                                }
                                return true;
                            }
                            return false;
                        }, jobjectWrapperType, newMetaType);
                    }
                }
            }else if(deleter_function){
                if(metaType.isValid() && !QMetaType::hasRegisteredConverterFunction(metaType, newMetaType)){
                    QMetaType::registerConverterFunction([deleter_function, typeId](const void *src, void *target)->bool{
                        std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char>*>(target);
                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinksForPointer(*reinterpret_cast<void*const*>(src)).value(0)){
                            if(JniEnvironment env{200}){
                                jobject jobj = link->getJavaObjectLocalRef(env);
                                link.clear();
                                *p = QtJambiAPI::convertJavaObjectToSharedPtr(env, typeId, jobj);
                                return true;
                            }
                        }
                        p->reset(*reinterpret_cast<char*const*>(src), DeleterDeleter{deleter_function});
                        return true;
                    }, metaType, newMetaType);
                }
                if(!QMetaType::hasRegisteredConverterFunction(jobjectWrapperType, newMetaType)){
                    if(typeId){
                        QMetaType::registerConverterFunction([typeId](const void *src, void *target)->bool{
                            std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char>*>(target);
                            const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                            if(JniEnvironment env{200}){
                                *p = QtJambiAPI::convertJavaObjectToSharedPtr(env, typeId, jow.object(env));
                                return true;
                            }
                            return false;
                        }, jobjectWrapperType, newMetaType);
                    }else{
                        QMetaType::registerConverterFunction([deleter_function, memberReConverter](const void *src, void *target)->bool{
                            std::shared_ptr<char>* p = reinterpret_cast<std::shared_ptr<char>*>(target);
                            const JObjectWrapper& jow = *reinterpret_cast<JObjectWrapper const*>(src);
                            if(JniEnvironment env{200}){
                                jvalue jv;
                                jv.l = jow.object(env);
                                char* ptr{nullptr};
                                void* out{&ptr};
                                if(memberReConverter(env, nullptr, jv, out, jValueType::l)){
                                    p->reset(ptr, DeleterDeleter{deleter_function});
                                }
                                return true;
                            }
                            return false;
                        }, jobjectWrapperType, newMetaType);
                    }
                }
            }
        }
        return newMetaType;
    }
    default:
        break;
    }
    return QMetaType();
}

QMetaType CoreAPI::registerMetaType(JNIEnv *env, jclass containerType, jobjectArray instantiations)
{
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    using namespace RegistryAPI;
    QString templateName;
    QMetaType registeredMetaType = qtjambiMetaType(env, containerType, instantiations, &templateName);
    if(registeredMetaType==RegistryAPI::registerMetaType<JObjectWrapper>("JObjectWrapper")
            || registeredMetaType==RegistryAPI::registerMetaType<JCollectionWrapper>("JCollectionWrapper")
            || registeredMetaType==RegistryAPI::registerMetaType<JMapWrapper>("JMapWrapper")){
        registeredMetaType = QMetaType(QMetaType::UnknownType);
    }
    if(registeredMetaType.isValid())
        return registeredMetaType;
    try{
        JConstObjectArrayPointer<jobject> _instantiations(env, instantiations);
        QStringList names;
        bool registerDerivedClass = false;
        AbstractContainerAccess* containerAccess = nullptr;
        switch(_instantiations.length()){
        case 1: {
            SequentialContainerType type;
            if(templateName.startsWith(QStringLiteral("QQueue<"))){
                type = SequentialContainerType::QQueue;
                names << templateName;
            }else if(templateName.startsWith(QStringLiteral("QStack<"))){
                type = SequentialContainerType::QStack;
                names << templateName;
            }else if(templateName.startsWith(QStringLiteral("QList<"))){
                type = SequentialContainerType::QList;
                names << templateName;
            }else if(templateName.startsWith(QStringLiteral("QSet<"))){
                type = SequentialContainerType::QSet;
                names << templateName;
            }else if(Java::QtCore::QQueue::isSameClass(env, containerType)){
                type = SequentialContainerType::QQueue;
                names << QStringLiteral("QQueue<%1>");
            }else if(Java::QtCore::QQueue::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QQueue;
                names << QStringLiteral("QQueue<%1>");
                registerDerivedClass = true;
            }else if(Java::QtCore::QStack::isSameClass(env, containerType)){
                type = SequentialContainerType::QStack;
                names << QStringLiteral("QStack<%1>");
            }else if(Java::Runtime::Deque::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QStack;
                names << QStringLiteral("QStack<%1>");
            }else if(Java::QtCore::QStack::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QStack;
                names << QStringLiteral("QStack<%1>");
                registerDerivedClass = true;
            }else if(Java::QtCore::QList::isSameClass(env, containerType)){
                type = SequentialContainerType::QList;
                names << QStringLiteral("QList<%1>");
            }else if(Java::QtCore::QList::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QList;
                names << QStringLiteral("QList<%1>");
                registerDerivedClass = true;
            }else if(Java::Runtime::List::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QList;
                names << QStringLiteral("QList<%1>");
            }else if(Java::QtCore::QSet::isSameClass(env, containerType)){
                type = SequentialContainerType::QSet;
                names << QStringLiteral("QSet<%1>");
            }else if(Java::QtCore::QSet::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QSet;
                names << QStringLiteral("QSet<%1>");
                registerDerivedClass = true;
            }else if(Java::Runtime::Set::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QSet;
                names << QStringLiteral("QSet<%1>");
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
            }else if(Java::QtCore::QSpan::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QSpan;
                names << QStringLiteral("QSpan<%1>");
            }else if(Java::QtCore::QConstSpan::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QConstSpan;
                names << QStringLiteral("QSpan<const %1>");
#endif
            }else{
                SmartPointerType smartPointerType = SmartPointerType::None;
                if(templateName.startsWith(QStringLiteral("QScopedPointer<"))){
                    smartPointerType = SmartPointerType::QScopedPointer;
                    names << templateName;
                }else if(templateName.startsWith(QStringLiteral("QScopedArrayPointer<"))){
                    smartPointerType = SmartPointerType::QScopedArrayPointer;
                    names << templateName;
                }else if(templateName.startsWith(QStringLiteral("QPointer<"))){
                    smartPointerType = SmartPointerType::QPointer;
                    names << templateName;
                }else if(templateName.startsWith(QStringLiteral("QWeakPointer<"))){
                    smartPointerType = SmartPointerType::QWeakPointer;
                    names << templateName;
                }else if(templateName.startsWith(QStringLiteral("QSharedPointer<"))){
                    smartPointerType = SmartPointerType::QSharedPointer;
                    names << templateName;
                }else if(templateName.startsWith(QStringLiteral("std::shared_ptr<"))){
                    smartPointerType = SmartPointerType::shared_ptr;
                    names << templateName;
                }else if(templateName.startsWith(QStringLiteral("std::weak_ptr<"))){
                    smartPointerType = SmartPointerType::weak_ptr;
                    names << templateName;
                }else if(templateName.startsWith(QStringLiteral("std::unique_ptr<"))){
                    smartPointerType = SmartPointerType::unique_ptr;
                    names << templateName;
                }
                if(smartPointerType==SmartPointerType::None)
                {
                    return registerMetaType(env, containerType, false, false);
                }else{
                    const QMetaType& metaType1 = qtjambi_cast<const QMetaType&>(env, _instantiations[0]);
                    PtrDeleterFunction deleter_function = nullptr;
                    const std::type_info* typeId = nullptr;
                    bool isPointer = AbstractContainerAccess::isPointerType(metaType1);
                    if(isPointer){
                        typeId = getTypeByQtName(QByteArray(metaType1.name()).chopped(1));
                    }else{
                        typeId = getTypeByQtName(metaType1.name());
                    }
                    if(typeId){
                        deleter_function = deleter(*typeId);
                    }
                    QString elementType = QLatin1String(metaType1.name());
                    if(elementType.endsWith("*"))
                        elementType = elementType.chopped(1).trimmed();
                    for(QString& name : names){
                        if(name.contains(QLatin1String("%1")))
                            name = name.arg(EXCLUDE_GT_END(elementType));
                        registeredMetaType = QMetaType::fromName(name.toLocal8Bit());
                        if(registeredMetaType.isValid())
                            return registeredMetaType;
                    }
                    jclass elementClass = CoreAPI::getClassForMetaType(env, metaType1);
                    QtJambiUtils::InternalToExternalConverter memberConverter = QtJambiTypeManager::tryGetInternalToExternalConverter(
                        env,
                        metaType1.name(),
                        metaType1,
                        elementClass);
                    if(!memberConverter)
                        memberConverter = ResolvingInternalToExternalConverter(elementClass, metaType1);

                    QtJambiUtils::ExternalToInternalConverter memberReConverter = QtJambiTypeManager::tryGetExternalToInternalConverter(
                        env,
                        elementClass,
                        metaType1.name(),
                        metaType1
                        );
                    if(!memberReConverter)
                        memberReConverter = ResolvingExternalToInternalConverter(elementClass, metaType1);
                    return registerMetaType(env,
                                            metaType1,
                                            deleter_function,
                                            typeId,
                                            isPointer,
                                            memberConverter,
                                            memberReConverter,
                                            smartPointerType,
                                            names.first());
                }
            }
            QMetaType metaType1 = qtjambi_cast<QMetaType>(env, _instantiations[0]);
            QString elementType = QLatin1String(metaType1.name());
            for(QString& name : names){
                if(name.contains(QLatin1String("%1")))
                    name = name.arg(EXCLUDE_GT_END(elementType));
            }
            containerAccess = ContainerAccessAPI::createContainerAccess(type, metaType1);
            if(!containerAccess){
                jclass elementClass = CoreAPI::getClassForMetaType(env, metaType1);
                bool isPointer = AbstractContainerAccess::isPointerType(metaType1);
                QPair<size_t,size_t> sizeAlign{0,0};
                if(!isPointer){
                    if(const std::type_info* t = getTypeByQtName(elementType)){
                        sizeAlign = getValueSizeAndAlignment(*t);
                    }else{
                        if(metaType1.isValid()){
                            int sz = metaType1.sizeOf();
                            sizeAlign.first = sz<0 ? 0 : size_t(sz);
                            sizeAlign.second = metaType1.alignOf();
                        }
                    }
                    if(sizeAlign.first==0)
                        sizeAlign.first = QtJambiTypeManager::getInternalSize(elementType);
                    if(sizeAlign.second==0)
                        sizeAlign.second = QtJambiTypeManager::getInternalAlignment(elementType);
                }

                QtJambiUtils::InternalToExternalConverter memberConverter = QtJambiTypeManager::tryGetInternalToExternalConverter(
                            env,
                            metaType1.name(),
                            metaType1,
                            elementClass);
                if(!memberConverter)
                    memberConverter = ResolvingInternalToExternalConverter(elementClass, metaType1);

                QtJambiUtils::ExternalToInternalConverter memberReConverter = QtJambiTypeManager::tryGetExternalToInternalConverter(
                            env,
                            elementClass,
                            metaType1.name(),
                            metaType1
                        );
                if(!memberReConverter)
                    memberReConverter = ResolvingExternalToInternalConverter(elementClass, metaType1);

                QtJambiUtils::QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, metaType1);
                QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess = findContainerAccess(metaType1);
                const std::type_info* typeId = getTypeByQtName(metaType1.name());
                if(!typeId){
                    typeId = getTypeByMetaType(metaType1);
                }
                PtrOwnerFunction memberOwnerFunction = nullptr;
                if(typeId)
                    memberOwnerFunction = ContainerAPI::registeredOwnerFunction(*typeId);
                containerAccess = ContainerAccessAPI::createContainerAccess(env, type,
                                                                  metaType1,
                                                                  sizeAlign.second, sizeAlign.first,
                                                                  isPointer,
                                                                  hashFunction,
                                                                  memberConverter,
                                                                  memberReConverter,
                                                                  memberNestedContainerAccess,
                                                                  memberOwnerFunction);
            }
#if defined(QTJAMBI_GENERIC_ACCESS)
            switch(type){
            case SequentialContainerType::QSet:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractSetAccess*>(containerAccess));
                break;
            case SequentialContainerType::QStack:
            case SequentialContainerType::QQueue:
            case SequentialContainerType::QList:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractListAccess*>(containerAccess));
                break;
            default:
                break;
            }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
            QMetaType metaType;
            for(QString& name : names){
                metaType = containerAccess->registerContainer(name.toLatin1());
            }
            if(registerDerivedClass){
                metaType = registerMetaType(env, containerType, false, false, metaType);
                registerContainerAccess(metaType, containerAccess);
                containerAccess->dispose();
                return metaType;
            }else{
                containerAccess->dispose();
                return metaType;
            }
        } break;
        case 2: {
            QTJAMBI_JNI_LOCAL_FRAME(env, 64);
            QMetaType metaType1 = qtjambi_cast<QMetaType>(env, _instantiations[0]);
            QMetaType metaType2 = qtjambi_cast<QMetaType>(env, _instantiations[1]);
            AssociativeContainerType type;
            if(templateName.startsWith(QStringLiteral("QMultiMap<"))){
                type = AssociativeContainerType::QMultiMap;
                names << templateName;
            }else if(templateName.startsWith(QStringLiteral("QMap<"))){
                type = AssociativeContainerType::QMap;
                names << templateName;
            }else if(templateName.startsWith(QStringLiteral("QMultiHash<"))){
                type = AssociativeContainerType::QMultiHash;
                names << templateName;
            }else if(templateName.startsWith(QStringLiteral("QHash<"))){
                type = AssociativeContainerType::QHash;
                names << templateName;
            }else if(Java::QtCore::QPair::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QPair;
                names << QStringLiteral("std::pair<%1,%2>");
            }else if(Java::QtCore::QMultiMap::isSameClass(env, containerType)){
                type = AssociativeContainerType::QMultiMap;
                names << QStringLiteral("QMultiMap<%1,%2>");
            }else if(Java::QtCore::QMultiMap::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QMultiMap;
                names << QStringLiteral("QMultiMap<%1,%2>");
                registerDerivedClass = true;
            }else if(Java::QtCore::QMap::isSameClass(env, containerType)){
                type = AssociativeContainerType::QMap;
                names << QStringLiteral("QMap<%1,%2>");
            }else if(Java::QtCore::QMap::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QMap;
                names << QStringLiteral("QMap<%1,%2>");
                registerDerivedClass = true;
            }else if(Java::Runtime::NavigableMap::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QMap;
                names << QStringLiteral("QMap<%1,%2>");
            }else if(Java::QtCore::QMultiHash::isSameClass(env, containerType)){
                type = AssociativeContainerType::QMultiHash;
                names << QStringLiteral("QMultiHash<%1,%2>");
            }else if(Java::QtCore::QMultiHash::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QMultiHash;
                names << QStringLiteral("QMultiHash<%1,%2>");
                registerDerivedClass = true;
            }else if(Java::QtCore::QHash::isSameClass(env, containerType)){
                type = AssociativeContainerType::QHash;
                names << QStringLiteral("QHash<%1,%2>");
            }else if(Java::QtCore::QHash::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QHash;
                names << QStringLiteral("QHash<%1,%2>");
                registerDerivedClass = true;
            }else if(Java::Runtime::Map::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QHash;
                names << QStringLiteral("QHash<%1,%2>");
            }else{
                return registerMetaType(env, containerType, false, false);
            }
            QString keyType = QLatin1String(metaType1.name());
            QString valueType = QLatin1String(metaType2.name());
            for(QString& name : names){
                if(name.contains(QLatin1String("%1")) && name.contains(QLatin1String("%2")))
                    name = name.arg(keyType, EXCLUDE_GT_END(valueType));
            }

            jclass keyClass = CoreAPI::getClassForMetaType(env, metaType1);
            jclass valueClass = CoreAPI::getClassForMetaType(env, metaType2);
            bool isPointer1 = AbstractContainerAccess::isPointerType(metaType1);
            QPair<size_t,size_t> sizeAlign1{0,0};
            bool isPointer2 = AbstractContainerAccess::isPointerType(metaType2);
            QPair<size_t,size_t> sizeAlign2{0,0};
            if(!isPointer1){
                if(const std::type_info* t = getTypeByQtName(keyType)){
                    sizeAlign1 = getValueSizeAndAlignment(*t);
                }else{
                    if(metaType1.isValid()){
                        int sz = metaType1.sizeOf();
                        sizeAlign1.first = sz<0 ? 0 : size_t(sz);
                        sizeAlign1.second = metaType1.alignOf();
                    }
                }
                if(sizeAlign1.first==0)
                    sizeAlign1.first = QtJambiTypeManager::getInternalSize(keyType);
                if(sizeAlign1.second==0)
                    sizeAlign1.second = QtJambiTypeManager::getInternalAlignment(keyType);
            }
            if(!isPointer2){
                if(const std::type_info* t = getTypeByQtName(valueType)){
                    sizeAlign2 = getValueSizeAndAlignment(*t);
                }else{
                    if(metaType2.isValid()){
                        int sz = metaType2.sizeOf();
                        sizeAlign2.first = sz<0 ? 0 : size_t(sz);
                        sizeAlign2.second = metaType2.alignOf();
                    }
                }
                if(sizeAlign2.first==0)
                    sizeAlign2.first = QtJambiTypeManager::getInternalSize(valueType);
                if(sizeAlign2.second==0)
                    sizeAlign2.second = QtJambiTypeManager::getInternalAlignment(valueType);
            }

            containerAccess = ContainerAccessAPI::createContainerAccess(type, metaType1, metaType2);
            if(!containerAccess){
                QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, metaType1);
                QtJambiUtils::QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, metaType2);
                QtJambiUtils::InternalToExternalConverter memberConverter1 = QtJambiTypeManager::tryGetInternalToExternalConverter(
                            env,
                            metaType1.name(),
                            metaType1,
                            keyClass);
                if(!memberConverter1)
                    memberConverter1 = ResolvingInternalToExternalConverter(keyClass, metaType1);

                QtJambiUtils::ExternalToInternalConverter memberReConverter1 = QtJambiTypeManager::tryGetExternalToInternalConverter(
                            env,
                            keyClass,
                            metaType1.name(),
                            metaType1
                        );
                if(!memberReConverter1)
                    memberReConverter1 = ResolvingExternalToInternalConverter(keyClass, metaType1);

                QtJambiUtils::InternalToExternalConverter memberConverter2 = QtJambiTypeManager::tryGetInternalToExternalConverter(
                            env,
                            metaType2.name(),
                            metaType2,
                            valueClass);
                if(!memberConverter2)
                    memberConverter2 = ResolvingInternalToExternalConverter(valueClass, metaType2);

                QtJambiUtils::ExternalToInternalConverter memberReConverter2 = QtJambiTypeManager::tryGetExternalToInternalConverter(
                            env,
                            valueClass,
                            metaType2.name(),
                            metaType2
                        );
                if(!memberReConverter2)
                    memberReConverter2 = ResolvingExternalToInternalConverter(valueClass, metaType2);
                QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess1 = findContainerAccess(metaType1);
                QSharedPointer<AbstractContainerAccess> memberNestedContainerAccess2 = findContainerAccess(metaType2);
                const std::type_info* typeId = getTypeByQtName(metaType1.name());
                if(!typeId){
                    typeId = getTypeByMetaType(metaType1);
                }
                PtrOwnerFunction memberOwnerFunction1 = nullptr;
                if(typeId)
                    memberOwnerFunction1 = ContainerAPI::registeredOwnerFunction(*typeId);
                typeId = getTypeByQtName(metaType2.name());
                if(!typeId){
                    typeId = getTypeByMetaType(metaType2);
                }
                PtrOwnerFunction memberOwnerFunction2 = nullptr;
                if(typeId)
                    memberOwnerFunction2 = ContainerAPI::registeredOwnerFunction(*typeId);

                containerAccess = ContainerAccessAPI::createContainerAccess(
                                                env, type,
                                                metaType1,
                                                sizeAlign1.second, sizeAlign1.first,
                                                isPointer1,
                                                hashFunction1,
                                                memberConverter1,
                                                memberReConverter1,
                                                memberNestedContainerAccess1,
                                                memberOwnerFunction1,
                                                metaType2,
                                                sizeAlign2.second, sizeAlign2.first,
                                                isPointer2,
                                                hashFunction2,
                                                memberConverter2,
                                                memberReConverter2,
                                                memberNestedContainerAccess2,
                                                memberOwnerFunction2);
            }
#if defined(QTJAMBI_GENERIC_ACCESS)
            switch(type){
            case AssociativeContainerType::QMap:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractMapAccess*>(containerAccess));
                break;
            case AssociativeContainerType::QMultiMap:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractMultiMapAccess*>(containerAccess));
                break;
            case AssociativeContainerType::QHash:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractHashAccess*>(containerAccess));
                break;
            case AssociativeContainerType::QMultiHash:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractMultiHashAccess*>(containerAccess));
                break;
            default:
                containerAccess = dynamic_cast<AbstractPairAccess*>(containerAccess);
                QtJambiAPI::checkNullPointer(env, containerAccess);
                break;
            }
#endif //defined(QTJAMBI_GENERIC_ACCESS)
            QMetaType id;
            for(QString& name : names){
                id = containerAccess->registerContainer(name.toLatin1());
            }
            if(registerDerivedClass){
                id = registerMetaType(env, containerType, false, false, id);
                registerContainerAccess(id, containerAccess);
                containerAccess->dispose();
                return id;
            }else{
                containerAccess->dispose();
                return id;
            }
        } break;
        }

        return registerMetaType(env, containerType, false, false);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return QMetaType();
    }
}

QMetaType CoreAPI::registeredMetaType(JNIEnv *env, jclass clazz, jobjectArray instantiations){
    return qtjambiMetaType(env, clazz, instantiations, nullptr);
}

QMetaType qtjambiMetaType(JNIEnv *env, jclass clazz, jobjectArray instantiations, QString* templateName)
{
    using namespace RegistryAPI;
    QMetaType metaType;
    if(clazz){
        if(jobject result = Java::QtJambi::MetaTypeUtility::analyzeExpectedTemplateName(env, clazz, instantiations)){
            if(Java::Runtime::String::isInstanceOf(env, result)){
                QString templName = qtjambi_cast<QString>(env, result);
                metaType = QMetaType::fromName(templName.toUtf8());
                if(templateName)
                    *templateName = templName;
            }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, result)){
                metaType = QMetaType(Java::QtJambi::QtEnumerator::value(env, result));
            }
        }
        if(metaType.isValid()){// ok so far
        }else if(Java::Runtime::Class::isArray(env, clazz)){
            jclass componentType = Java::Runtime::Class::getComponentType(env, clazz);
            if(Java::Runtime::Integer::isPrimitiveType(env, componentType)){
                return registerMetaType<JIntArrayWrapper>("JIntArrayWrapper");
            }else if(Java::Runtime::Short::isPrimitiveType(env, componentType)){
                return registerMetaType<JShortArrayWrapper>("JShortArrayWrapper");
            }else if(Java::Runtime::Byte::isPrimitiveType(env, componentType)){
                return registerMetaType<JByteArrayWrapper>("JByteArrayWrapper");
            }else if(Java::Runtime::Long::isPrimitiveType(env, componentType)){
                return registerMetaType<JLongArrayWrapper>("JLongArrayWrapper");
            }else if(Java::Runtime::Character::isPrimitiveType(env, componentType)){
                return registerMetaType<JCharArrayWrapper>("JCharArrayWrapper");
            }else if(Java::Runtime::Boolean::isPrimitiveType(env, componentType)){
                return registerMetaType<JBooleanArrayWrapper>("JBooleanArrayWrapper");
            }else if(Java::Runtime::Double::isPrimitiveType(env, componentType)){
                return registerMetaType<JDoubleArrayWrapper>("JDoubleArrayWrapper");
            }else if(Java::Runtime::Float::isPrimitiveType(env, componentType)){
                return registerMetaType<JFloatArrayWrapper>("JFloatArrayWrapper");
            }else{
                return registerMetaType<JObjectArrayWrapper>("JObjectArrayWrapper");
            }
        }else if(Java::Runtime::Integer::isSameClass(env, clazz) || Java::Runtime::Integer::isPrimitiveType(env, clazz)){
            return QMetaType(QMetaType::Int);
        }else if(Java::Runtime::Long::isSameClass(env, clazz) || Java::Runtime::Long::isPrimitiveType(env, clazz)){
            return QMetaType(QMetaType::LongLong);
        }else if(Java::Runtime::Short::isSameClass(env, clazz) || Java::Runtime::Short::isPrimitiveType(env, clazz)){
            return QMetaType(QMetaType::Short);
        }else if(Java::Runtime::Byte::isSameClass(env, clazz) || Java::Runtime::Byte::isPrimitiveType(env, clazz)){
            return QMetaType(QMetaType::SChar);
        }else if(Java::Runtime::Boolean::isSameClass(env, clazz) || Java::Runtime::Boolean::isPrimitiveType(env, clazz)){
            return QMetaType(QMetaType::Bool);
        }else if(Java::Runtime::Character::isSameClass(env, clazz) || Java::Runtime::Character::isPrimitiveType(env, clazz)){
            return QMetaType(QMetaType::QChar);
        }else if(Java::Runtime::Float::isSameClass(env, clazz) || Java::Runtime::Float::isPrimitiveType(env, clazz)){
            return QMetaType(QMetaType::Float);
        }else if(Java::Runtime::Double::isSameClass(env, clazz) || Java::Runtime::Double::isPrimitiveType(env, clazz)){
            return QMetaType(QMetaType::Double);
        }else if(Java::QtCore::QVariant$Null::isSameClass(env, clazz)){
            return QMetaType(QMetaType::Nullptr);
        }else if(Java::QtCore::QVariant::isSameClass(env, clazz)){
            return QMetaType(QMetaType::QVariant);
        }else{
            QString javaClassName = QtJambiAPI::getClassName(env, clazz).replace('.', '/');
            if(!templateName || templateName->isEmpty() || !instantiations || env->GetArrayLength(instantiations)==0){
                if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                    metaType = registeredMetaType(*typeId);
                    if(!metaType.isValid()){
                        QByteArray qtType = getQtName(*typeId);
                        EntryTypes entryType = getEntryType(*typeId);
                        switch(entryType){
                        case EntryTypes::ObjectTypeInfo:
                        case EntryTypes::QObjectTypeInfo:
                        case EntryTypes::InterfaceTypeInfo:
                            {
                                qtType += "*";
                            }
                            break;
                            default: break;
                        }
                        metaType = QMetaType::fromName(qtType);
                    }
                }
            }
            if(!metaType.isValid()){
                metaType = QMetaType::fromName(javaClassName.toUtf8().replace("/", "::").replace("$", "::"));
                if(!metaType.isValid()
                         && !Java::Runtime::Cloneable::isAssignableFrom(env, clazz)
                         && !Java::Runtime::Enum::isAssignableFrom(env, clazz)
                         && !Java::QtJambi::QFlags::isAssignableFrom(env, clazz)){
                metaType = QMetaType::fromName(javaClassName.toUtf8().replace("/", "::").replace("$", "::").append("*"));
                }

            }
        }
    }else{
        metaType = QMetaType(QMetaType::Nullptr);
    }
    return QMetaType{metaType};
}

jclass registeredJavaClassForCustomMetaType(QMetaType metaType);
const char* registeredClassNameForFunctionalMetaType(const QMetaType& metaType);

jclass CoreAPI::getClassForMetaType(JNIEnv *env, const QMetaType& metaType)
{
    QByteArray typeName(metaType.name());
    const char* name = registeredClassNameForFunctionalMetaType(metaType);
    if(name)
        if(jclass cls = JavaAPI::resolveClass(env, name))
            return cls;
    if(const std::type_info* typeId = getTypeByMetaType(metaType)){
        name = getJavaName(*typeId);
    }
    if(!name){
        if(jclass cls = registeredJavaClassForCustomMetaType(metaType))
            return cls;
        QString _name = QtJambiTypeManager::getExternalTypeName(env, QLatin1String(typeName), metaType);
        if(!_name.isEmpty()){
            return JavaAPI::resolveClass(env, qPrintable(_name));
        }
        //
    }
    if(name){
        return JavaAPI::resolveClass(env, name);
    }
    try {
        if(typeName.endsWith("*")){
            typeName.chop(1);
        }
        return JavaAPI::resolveClass(env, typeName.replace("::", "."));
    }catch(const JavaException&){
        QStringList typeNameSplit = QString(QLatin1String(typeName)).split(QStringLiteral("::"));
        QString name;
        while(!typeNameSplit.isEmpty()){
            if(!name.isEmpty())
                name += QStringLiteral("::");
            name += typeNameSplit.takeFirst();
            try {
                JavaAPI::resolveClass(env, qPrintable(name));
                name += QStringLiteral("$")+typeNameSplit.join(QStringLiteral("$"));
                return JavaAPI::resolveClass(env, qPrintable(name));
            }catch(const JavaException&){}
        }
    }
    return nullptr;
}

QVariant CoreAPI::convertCheckedObjectToQVariant(JNIEnv *env, jobject object, const QMetaType& metaType){
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(env->IsSameObject(object, nullptr)){
        return QVariant(metaType, nullptr);
    }
    jclass typeClass = CoreAPI::getClassForMetaType(env, metaType);
    if(typeClass && (env->IsInstanceOf(object, typeClass)
                     || Java::QtCore::QVariant::isSameClass(env, typeClass)
                     || (Java::QtCore::QString::isSameClass(env, typeClass) && Java::Runtime::CharSequence::isInstanceOf(env, object))
                     || (Java::Runtime::String::isSameClass(env, typeClass) && Java::Runtime::CharSequence::isInstanceOf(env, object)))){
        const std::type_info* t;
        QByteArray internalTypeName = metaType.name();
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
        if(!META_TYPE_ACCESS(metaType).iface()->copyCtr){
            Java::Runtime::UnsupportedOperationException::throwNew(env, QStringLiteral("Unable to create value of meta type %1 due to missing copy constructor.").arg(QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
        }
#endif
        if(metaType.flags() & QMetaType::IsPointer){
            if(internalTypeName.endsWith("*"))
                internalTypeName = internalTypeName.chopped(1).trimmed();
            t = getTypeByQtName(internalTypeName);
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object)){
                void* ptr = t ? link->typedPointer(*t) : link->pointer();
                return QVariant(metaType, &ptr);
            }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object))
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }else{
            if ((internalTypeName.startsWith("QSharedPointer<")
                 || internalTypeName.startsWith("QWeakPointer<")
                 || internalTypeName.startsWith("QPointer<")
                 || internalTypeName.startsWith("QScopedPointer<")
                 || internalTypeName.startsWith("QScopedArrayPointer<")
                 || internalTypeName.startsWith("std::unique_ptr<")
                 || internalTypeName.startsWith("std::shared_ptr<")
                 || internalTypeName.startsWith("std::weak_ptr<")) && internalTypeName.endsWith('>')) {
                QString instantiation = internalTypeName.mid(internalTypeName.indexOf(u'<')+1).chopped(1).trimmed();
                t = getTypeByQtName(instantiation);
                bool isQObject = Java::QtCore::QObject::isAssignableFrom(env, typeClass);
                if (internalTypeName.startsWith("QSharedPointer<")) {
                    if(isQObject){
                        QSharedPointer<QObject> sptr = QtJambiAPI::convertJavaObjectToQSharedPointer(env, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }else if(t){
                        QSharedPointer<char> sptr = QtJambiAPI::convertJavaObjectToQSharedPointer(env, t, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }
                }else if (internalTypeName.startsWith("QWeakPointer<")) {
                    if(isQObject){
                        QWeakPointer<QObject> sptr = QtJambiAPI::convertJavaObjectToQWeakPointer(env, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }else if(t){
                        QWeakPointer<char> sptr = QtJambiAPI::convertJavaObjectToQWeakPointer(env, t, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }
                }else if (internalTypeName.startsWith("std::shared_ptr<")) {
                    if(isQObject){
                        std::shared_ptr<QObject> sptr = QtJambiAPI::convertJavaObjectToSharedPtr(env, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }else if(t){
                        std::shared_ptr<char> sptr = QtJambiAPI::convertJavaObjectToSharedPtr(env, t, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }
                }else if (internalTypeName.startsWith("std::weak_ptr<")) {
                    if(isQObject){
                        std::weak_ptr<QObject> sptr = QtJambiAPI::convertJavaObjectToWeakPtr(env, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }else if(t){
                        std::weak_ptr<char> sptr = QtJambiAPI::convertJavaObjectToWeakPtr(env, t, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }
                }else if (internalTypeName.startsWith("QPointer<")) {
                    if(isQObject){
                        QPointer<QObject> sptr = QtJambiAPI::convertJavaObjectToQObject(env, object);
                        return VariantUtility::createVariant(metaType, &sptr);
                    }
                    return QVariant(metaType);
                }else if (internalTypeName.startsWith("QScopedPointer<")) {
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object)){
                        void* ptr = t ? link->typedPointer(*t) : link->pointer();
                        if(link->isSmartPointer())
                            return QVariant(metaType);
                        QScopedPointer<char> sptr;
                        sptr.reset(reinterpret_cast<char*>(ptr));
                        QVariant v(VariantUtility::createVariant(metaType, &sptr));
                        (void)sptr.take();
                        return v;
                    }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object))
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }else if (internalTypeName.startsWith("std::unique_ptr<")) {
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object)){
                        void* ptr = t ? link->typedPointer(*t) : link->pointer();
                        if(link->isSmartPointer())
                            return QVariant(metaType);
                        std::unique_ptr<char> sptr;
                        sptr.reset(reinterpret_cast<char*>(ptr));
                        QVariant v(VariantUtility::createVariant(metaType, &sptr));
                        (void)sptr.release();
                        return v;
                    }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object))
                        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
                }
                return QVariant(metaType);
            }
            t = getTypeByQtName(internalTypeName);
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object)){
                void* ptr = t ? link->typedPointer(*t) : link->pointer();
                return QVariant(metaType, ptr);
            }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object))
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
        }
    }
    QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(env, object);
    if(variant.metaType()!=metaType){
        if(variant.convert(metaType)){
            return variant;
        }
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Object of type %1 incompatible with meta type %2.").arg(QtJambiAPI::getObjectClassName(env, object).replace("$", "."), QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
        return QVariant();
    }else{
        return variant;
    }
}

size_t CoreAPI::computeHash(const QMetaType& metaType, const void* ptr, size_t seed, bool* success)
{
    QtJambiUtils::QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(AbstractContainerAccess::isPointerType(metaType), metaType);
    if(success)
        *success = hashFunction1;
    if(hashFunction1)
        return hashFunction1(ptr, seed);
    return 0;
}

jobject CoreAPI::getExtraSignal(JNIEnv *env, QtJambiNativeID sender__id, QtJambiNativeID method__id)
{
    try{
        if(QSharedPointer<QtJambiLink> sender = QtJambiLink::fromNativeId(sender__id)){
            if(const QMetaMethod *method = QtJambiAPI::objectFromNativeId<QMetaMethod>(method__id)){
                return sender->getExtraSignal(env, *method);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

bool CoreAPI::isJObjectWrappedMetaType(const QMetaType& metaType){
    return ::isJObjectWrappedMetaType(metaType);
}

bool CoreAPI::isNativeWrapperMetaType(const QMetaType& metaType){
    return ::isNativeWrapperMetaType(metaType);
}

ManagedSpan::ManagedSpan() = default;
ManagedSpan::ManagedSpan(const ManagedSpan&) = default;

ManagedSpan::ManagedSpan(QSharedPointer<ManagedSpanData>&& _data)
    : QtJambiSpan{}, d(std::move(_data)) {}

void ManagedSpan::commit(JNIEnv* env){
    ManagedSpanData* data = d.data();
    if(data && data->commitFunction)
        data->commitFunction(data, env);
}

void ManagedSpan::deleter(void* ptr,bool) {
    delete reinterpret_cast<ManagedSpan*>(ptr);
}

void QtJambiSpan::deleter(void* ptr,bool) {
    delete reinterpret_cast<QtJambiSpan*>(ptr);
}
