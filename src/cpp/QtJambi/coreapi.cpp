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

#include <QtCore/QCoreApplication>
#include "coreapi.h"
#include "typemanager_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_cast.h"
#include "qtjambimetaobject_p.h"
#include "containeraccess_p.h"
#include "java_p.h"

#define EXCLUDE_GT_END(strg) strg //.endsWith(">") ? strg+" " : strg

QString CoreAPI::externalTypeTame(JNIEnv *env, const QString& internalName){
    return QtJambiTypeManager::getExternalTypeName(env, internalName);
}

jobject CoreAPI::findObject(JNIEnv *env, const void * pointer)
{
    for(const QSharedPointer<QtJambiLink>& link : QtJambiLink::findLinksForPointer(pointer)){
        if(link)
            return link->getJavaObjectLocalRef(env);
    }
    return nullptr;
}

jobject CoreAPI::javaObject(QtJambiNativeID nativeId, JNIEnv* env)
{
    return !!nativeId ? reinterpret_cast<QtJambiLink *>(nativeId)->getJavaObjectLocalRef(env) : nullptr;
}

jobject CoreAPI::fromDestroyedQObject(JNIEnv *env, QObject* object)
{
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForQObject(object)){
        return link->getJavaObjectLocalRef(env);
    }
    return nullptr;
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
            return qtjambi_cast<jobject>(env, method);
        }else{
            env->SetObjectArrayElement(ok, 0, nullptr);
            return nullptr;
        }
    }
    return nullptr;
}

jobject CoreAPI::invokeMetaMethod(JNIEnv * env, jobject _metaMethod,
                                   jobject _qobject, jobjectArray argClassTypeArray,
                                   jint connection, jobjectArray args)
{
    env->EnsureLocalCapacity(500);
    jvalue result;
    result.l = nullptr;
    QObject* object = QtJambiAPI::convertJavaObjectToQObject(env, _qobject);
    if(object){
        QMetaMethod* method = QtJambiAPI::convertJavaObjectToNative<QMetaMethod>(env, _metaMethod);
        if(method && method->isValid()){
            QtJambiScope scope(env, _qobject);
            bool ok = false;
            const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *method);
            int argsCount = env->GetArrayLength(args);
            const int parameterCount = method->parameterCount();
            if(parameterCount==env->GetArrayLength(argClassTypeArray)-1
                    && parameterCount==argsCount && argsCount<10){
                QGenericArgument val[10];
                QList<QByteArray> parameterTypes = method->parameterTypes();
                ok = true;
                for(int i=0; i<argsCount; ++i){
                    jvalue jval;
                    jval.l = env->GetObjectArrayElement(args, i);
                    void* qtPtr = nullptr;
                    const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                    ok = parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l);
                    if(ok)
                        val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
                    else
                        break;
                }
                if(ok){
                    void* resultPtr = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(method->returnType()!=QMetaType::Void){
                        resultPtr = QMetaType::create(method->returnType());
                        scope.addDeletion(method->returnType(), resultPtr);
                    }
#else
                    if(method->returnMetaType().id()!=QMetaType::Void){
                        resultPtr = method->returnMetaType().create();
                        scope.addDeletion(method->returnMetaType(), resultPtr);
                    }
#endif
                    ok = method->invoke(object,
                                        Qt::ConnectionType(connection),
                                        QGenericReturnArgument(method->typeName(), resultPtr),
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
                    if(ok && resultPtr){
                        ok = parameterTypeInfos[0].convertInternalToExternal(env, nullptr, resultPtr, &result, true);
                    }
                }
            }
            if(!ok){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String(method->methodSignature()) QTJAMBI_STACKTRACEINFO );
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Invalid method") QTJAMBI_STACKTRACEINFO );
        }
    }else if(_qobject){
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, _qobject).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    }else{
        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("QObject must not be null.") QTJAMBI_STACKTRACEINFO );
    }
    return result.l;
}

jobject CoreAPI::getMetaMethodParameterClassTypes(JNIEnv * env, jobject _this){
    const QMetaMethod *__qt_this = QtJambiAPI::convertJavaObjectToNative<QMetaMethod>(env, _this);
    jobject result = QtJambiAPI::newJavaArrayList(env, __qt_this->parameterCount());
    if(__qt_this && __qt_this->isValid()){
        const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *__qt_this);
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
        const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *__qt_this);
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


jobject CoreAPI::invokeMetaMethodOnGadget(JNIEnv * env, jobject _metaMethod,
                                           jobject object, jobjectArray argClassTypeArray,
                                           jobjectArray args)
{
    env->EnsureLocalCapacity(500);
    jvalue result;
    result.l = nullptr;
    JObjectWrapper wrapper;
    QMetaMethod* method = QtJambiAPI::convertJavaObjectToNative<QMetaMethod>(env, _metaMethod);
    if(method && method->isValid()){
        QtJambiScope scope;
        void* ptr = nullptr;
        if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object)){
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, object)){
                const std::type_info* typeId = nullptr;
                const QMetaObject *metaObject = method->enclosingMetaObject();
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
            const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, *method);
            int argsCount = env->GetArrayLength(args);
            const int parameterCount = method->parameterCount();
            if(parameterCount==env->GetArrayLength(argClassTypeArray)-1
                    && parameterCount==argsCount && argsCount<10){
                QGenericArgument val[10];
                QList<QByteArray> parameterTypes = method->parameterTypes();
                ok = true;
                for(int i=0; i<argsCount; ++i){
                    jvalue jval;
                    jval.l = env->GetObjectArrayElement(args, i);
                    void* qtPtr = nullptr;
                    const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                    ok = parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l);
                    if(ok)
                        val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
                    else
                        break;
                }
                if(ok){
                    void* resultPtr = nullptr;
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                    if(method->returnType()!=QMetaType::Void){
                        resultPtr = QMetaType::create(method->returnType());
                        scope.addDeletion(method->returnType(), resultPtr);
                    }
#else
                    if(method->returnMetaType().id()!=QMetaType::Void){
                        resultPtr = method->returnMetaType().create();
                        scope.addDeletion(method->returnMetaType(), resultPtr);
                    }
#endif
                    ok = method->invokeOnGadget(ptr,
                                        QGenericReturnArgument(method->typeName(), resultPtr),
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
                    if(ok && resultPtr){
                        ok = parameterTypeInfos[0].convertInternalToExternal(env, nullptr, resultPtr, &result, true);
                    }
                }
            }
            if(!ok){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String(method->methodSignature()) QTJAMBI_STACKTRACEINFO );
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(env, QLatin1String("Invalid method") QTJAMBI_STACKTRACEINFO );
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
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, gadget);
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
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, gadget);
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
                if(InternalToExternalConverter converter = mo->internalToExternalConverterOfProperty(__qt_this->relativePropertyIndex())){
                    jvalue val;
                    val.l = nullptr;
                    QtJambiScope scope(nullptr);
                    if(converter(env, &scope, variant.data(), &val, true))
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
                InternalToExternalConverter converter = QtJambiTypeManager::getInternalToExternalConverter(env, qtName, _type, object_class);
                QtJambiScope scope(nullptr);
                bool result = converter && converter(env, &scope, variant.data(), &val, true);
                if(result)
                    return val.l;
            }else{
                return qtjambi_cast<jobject>(env, variant);
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
                const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, gadget);
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
            int type;
            QT_WARNING_DISABLE_DEPRECATED
            if (__qt_this->type()==QVariant::Invalid) {
                return false;
            }else if(__qt_this->type()==QVariant::UserType){
                type = __qt_this->userType();
            }else{
                type = __qt_this->type();
            }
            QLatin1String qtName(__qt_this->typeName());
            QVariant valueVariant;
            bool success = false;
            if(const QtJambiMetaObject* mo = QtJambiMetaObject::cast(__qt_this->enclosingMetaObject())){
                if(ExternalToInternalConverter converter = mo->externalToInternalConverterOfProperty(__qt_this->relativePropertyIndex())){
                    jvalue val;
                    val.l = value;
                    void *copy = nullptr;
                    QtJambiScope scope(nullptr);
                    if(!(success = converter(env, &scope, val, copy, jValueType::l))){
                        qWarning("Cannot convert external type '%s' to '%s'",
                                 qPrintable(QString(QtJambiAPI::getObjectClassName(env, value)).replace("$", ".")),
                                 qPrintable(qtName));
                        return false;
                    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    valueVariant = QVariant(type, copy);
#else
                    valueVariant = QVariant(QMetaType(type), copy);
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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
                    QMetaType _type(type);
                    ExternalToInternalConverter converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, qtName, _type);
                    void *copy = nullptr;
                    QtJambiScope scope(nullptr);
                    if(!converter(env, &scope, val, copy, jValueType::l)){
                        qWarning("Cannot convert external type '%s' to '%s'",
                                 qPrintable(QString(QLatin1String(fullJavaName)).replace("/", ".").replace("$", ".")),
                                 qPrintable(qtName));
                        return false;
                    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    valueVariant = QVariant(type, copy);
#else
                    valueVariant = QVariant(QMetaType(type), copy);
#endif //QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
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

QReadWriteLock* CoreAPI::objectDataLock()
{
    return QtJambiLinkUserData::lock();
}

jobject CoreAPI::getMetaObjectJavaType(JNIEnv *env, const QMetaObject *metaObject, bool exactOrNull)
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
        const QList<ParameterTypeInfo>& parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, constructor);
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
                    return QtJambiAPI::convertQObjectToJavaObject(env, obj);
                }else{
                    if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(constructor.enclosingMetaObject())){
                        if(Java::QtJambi::QtObjectInterface::isAssignableFrom(env, dynamicMetaObject->javaClass())){
                            return QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, obj, dynamicMetaObject->javaClass());
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
                    if(const char* iid = registeredInterfaceID(*targetTypeId)){
                        targetTypePointer = objectLink->qobject()->qt_metacast(iid);
                    }
                    if(!targetTypePointer){
                        for(const PolymorphicIdHandler* handler : getPolymorphicIdHandlers(typeid(QObject))){
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
                                    for(const PolymorphicIdHandler* handler : getPolymorphicIdHandlers(*sourceTypeId)){
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
                                if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
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
                                if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
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
                        QtJambiLink::registerPointer(newlink);
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
                                if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                                    link->invalidate(env);
                                }else if(env->IsInstanceOf(obj, targetType)){
                                    return obj;
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
                    result = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, interfacePointer, targetType);
                    if(QSharedPointer<QtJambiLink> newlink = QtJambiLink::findLinkForJavaObject(env, result)){
                        objectLink->registerDependentObject(newlink);
                        if(!newlink->isQObject()){
                            QtJambiLink::registerPointer(newlink);
                        }
                    }
                }
            }
        }
    }
    return result;
}

int qtjambiMetaTypeId(JNIEnv *env, jclass clazz, jobjectArray instantiations, QString* templateName);

struct ResolvingInternalToExternalConverter{
    ResolvingInternalToExternalConverter(jclass elementClass, const QMetaType& metaType)
        : m_elementClass(elementClass),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          m_metaType(metaType.id()),
#else
          m_metaType(metaType),
#endif
          m_converter() {}
    ResolvingInternalToExternalConverter(ResolvingInternalToExternalConverter&& other)
        : m_elementClass(other.m_elementClass),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          m_metaType(other.m_metaType),
#else
          m_metaType(other.m_metaType),
#endif
          m_converter(other.m_converter) {
        other.m_converter = nullptr;
    }
    ResolvingInternalToExternalConverter& operator=(ResolvingInternalToExternalConverter&& other) {
        m_elementClass = other.m_elementClass;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_metaType = other.m_metaType;
#else
        m_metaType = other.m_metaType;
#endif
        m_converter = other.m_converter;
        other.m_converter = nullptr;
        return *this;
    }

    bool operator()(JNIEnv* env, QtJambiScope* scope, const void* in, jvalue* out, bool forceBoxedType)const{
        if(m_elementClass){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QMetaType metaType(m_metaType);
#endif
            m_converter = QtJambiTypeManager::getInternalToExternalConverter(
                                                              env,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                              metaType.name(),
                                                              metaType,
#else
                                                              m_metaType.name(),
                                                              m_metaType,
#endif
                                                              m_elementClass);
            m_elementClass = nullptr;
        }
        return m_converter(env, scope, in, out, forceBoxedType);
    }
    mutable jclass m_elementClass;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int m_metaType;
#else
    QMetaType m_metaType;
#endif
    mutable InternalToExternalConverter m_converter;
};

struct ResolvingExternalToInternalConverter{
    ResolvingExternalToInternalConverter(jclass elementClass, const QMetaType& metaType)
        : m_elementClass(elementClass),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          m_metaType(metaType.id()),
#else
          m_metaType(metaType),
#endif
          m_converter() {}
    ResolvingExternalToInternalConverter(ResolvingExternalToInternalConverter&& other)
        : m_elementClass(other.m_elementClass),
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
          m_metaType(other.m_metaType),
#else
          m_metaType(other.m_metaType),
#endif
          m_converter(other.m_converter) {
        other.m_converter = nullptr;
    }
    ResolvingExternalToInternalConverter& operator=(ResolvingExternalToInternalConverter&& other) {
        m_elementClass = other.m_elementClass;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        m_metaType = other.m_metaType;
#else
        m_metaType = other.m_metaType;
#endif
        m_converter = other.m_converter;
        other.m_converter = nullptr;
        return *this;
    }

    bool operator()(JNIEnv* env, QtJambiScope* scope, const jvalue&val, void* &out, jValueType valueType)const{
        if(m_elementClass){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            QMetaType metaType(m_metaType);
#endif
            m_converter = QtJambiTypeManager::getExternalToInternalConverter(
                                                              env,
                                                              m_elementClass,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                              metaType.name(),
                                                              metaType);
#else
                                                              m_metaType.name(),
                                                              m_metaType);
#endif
            m_elementClass = nullptr;
        }
        return m_converter(env, scope, val, out, valueType);
    }
    mutable jclass m_elementClass;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int m_metaType;
#else
    QMetaType m_metaType;
#endif
    mutable ExternalToInternalConverter m_converter;
};

int CoreAPI::registerMetaType(JNIEnv *env, jclass containerType, jobjectArray instantiations)
{
    QTJAMBI_JNI_LOCAL_FRAME(env, 64);
    using namespace RegistryAPI;
    QString templateName;
    int registeredId = qtjambiMetaTypeId(env, containerType, instantiations, &templateName);
    if(registeredId==RegistryAPI::registerMetaType<JEnumWrapper>("JEnumWrapper")
            || registeredId==RegistryAPI::registerMetaType<JObjectWrapper>("JObjectWrapper")
            || registeredId==RegistryAPI::registerMetaType<JCollectionWrapper>("JCollectionWrapper")
            || registeredId==RegistryAPI::registerMetaType<JMapWrapper>("JMapWrapper")
            || registeredId==RegistryAPI::registerMetaType<JIteratorWrapper>("JIteratorWrapper")){
        registeredId = QMetaType::UnknownType;
    }
    if(registeredId!=QMetaType::UnknownType)
        return registeredId;
    try{
        const JObjectArrayWrapper _instantiations(env, instantiations);
        QStringList names;
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            }else if(templateName.startsWith(QStringLiteral("QLinkedList<"))){
                type = SequentialContainerType::QLinkedList;
                names << templateName;
            }else if(templateName.startsWith(QStringLiteral("QVector<"))){
                type = SequentialContainerType::QVector;
                names << templateName;
#endif
            }else if(templateName.startsWith(QStringLiteral("QList<"))){
                type = SequentialContainerType::QList;
                names << templateName;
            }else if(templateName.startsWith(QStringLiteral("QSet<"))){
                type = SequentialContainerType::QSet;
                names << templateName;
            }else if(Java::QtCore::QQueue::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QQueue;
                names << QStringLiteral("QQueue<%1>");
            }else if(Java::QtCore::QStack::isAssignableFrom(env, containerType)
                     || Java::Runtime::Deque::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QStack;
                names << QStringLiteral("QStack<%1>");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            }else if(Java::QtCore::QLinkedList::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QLinkedList;
                names << QStringLiteral("QLinkedList<%1>");
            }else if(Java::QtCore::QVector::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QVector;
                names << QStringLiteral("QVector<%1>");
#endif
            }else if(Java::QtCore::QList::isAssignableFrom(env, containerType)
                     || Java::Runtime::List::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QList;
                names << QStringLiteral("QList<%1>");
            }else if(Java::QtCore::QSet::isAssignableFrom(env, containerType)
                     || Java::Runtime::Set::isAssignableFrom(env, containerType)){
                type = SequentialContainerType::QSet;
                names << QStringLiteral("QSet<%1>");
            }else{
                return registerMetaType(env, containerType, false, false);
            }
            const QMetaType& metaType1 = qtjambi_cast<const QMetaType&>(env, _instantiations[0].object());
            QString elementType = QLatin1String(metaType1.name());
            for(QString& name : names){
                if(name.contains(QLatin1String("%1")))
                    name = name.arg(EXCLUDE_GT_END(elementType));
            }
            containerAccess = AbstractContainerAccess::create(env, type, metaType1);
            if(!containerAccess){
                jclass elementClass = CoreAPI::getClassForMetaType(env, metaType1);
                bool isPointer = AbstractContainerAccess::isPointerType(metaType1);
                size_t size = 0;
                size_t align = 0;
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                bool isStaticType = true;
    #endif
                if(!isPointer){
                    if(const std::type_info* t = getTypeByQtName(elementType)){
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        if(const QtJambiTypeInfo* typeInfo = getQTypeInfo(*t)){
                            isStaticType = typeInfo->isStatic;
                        }
    #endif
                        size = getValueSize(*t);
                        align = getValueAlignment(*t);
                    }else{
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        isStaticType = QtJambiTypeManager::isStaticType(elementType);
    #endif
                        if(metaType1.id() != QMetaType::UnknownType){
                            int sz = metaType1.sizeOf();
                            size = sz<0 ? 0 : size_t(sz);
                        }
                    }
                    if(size==0)
                        size = QtJambiTypeManager::getInternalSize(elementType);
                    if(align==0)
                        align = QtJambiTypeManager::getInternalAlignment(elementType);
                }

                InternalToExternalConverter memberConverter = QtJambiTypeManager::tryGetInternalToExternalConverter(
                            env,
                            metaType1.name(),
                            metaType1,
                            elementClass);
                if(!memberConverter)
                    memberConverter = ResolvingInternalToExternalConverter(elementClass, metaType1);

                ExternalToInternalConverter memberReConverter = QtJambiTypeManager::tryGetExternalToInternalConverter(
                            env,
                            elementClass,
                            metaType1.name(),
                            metaType1
                        );
                if(!memberReConverter)
                    memberReConverter = ResolvingExternalToInternalConverter(elementClass, metaType1);

                QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, metaType1.id());
                containerAccess = AbstractContainerAccess::create(env, type,
                                                                  metaType1,
                                                                  align, size,
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                  isStaticType,
#endif
                                                                  isPointer,
                                                                  hashFunction,
                                                                  memberConverter,
                                                                  memberReConverter);
            }
            switch(type){
            case SequentialContainerType::QSet:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractSetAccess*>(containerAccess));
                break;
            case SequentialContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case SequentialContainerType::QVector:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractVectorAccess*>(containerAccess));
                break;
            case SequentialContainerType::QLinkedList:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractLinkedListAccess*>(containerAccess));
                break;
    #endif
            case SequentialContainerType::QQueue:
            case SequentialContainerType::QList:
                containerAccess = checkContainerAccess(env, dynamic_cast<AbstractListAccess*>(containerAccess));
                break;
            }
            int id = 0;
            for(QString& name : names){
                id = containerAccess->registerContainer(name.toLatin1());
            }
            containerAccess->dispose();
            return id;
        }
        case 2: {
            QTJAMBI_JNI_LOCAL_FRAME(env, 64);
            const QMetaType& metaType1 = qtjambi_cast<const QMetaType&>(env, _instantiations[0].object());
            const QMetaType& metaType2 = qtjambi_cast<const QMetaType&>(env, _instantiations[1].object());
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                names << QStringLiteral("QPair<%1,%2>");
#else
                names << QStringLiteral("std::pair<%1,%2>");
#endif
            }else if(Java::QtCore::QMultiMap::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QMultiMap;
                names << QStringLiteral("QMultiMap<%1,%2>");
            }else if(Java::QtCore::QMap::isAssignableFrom(env, containerType)
                     || Java::Runtime::NavigableMap::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QMap;
                names << QStringLiteral("QMap<%1,%2>");
            }else if(Java::QtCore::QMultiHash::isAssignableFrom(env, containerType)){
                type = AssociativeContainerType::QMultiHash;
                names << QStringLiteral("QMultiHash<%1,%2>");
            }else if(Java::QtCore::QHash::isAssignableFrom(env, containerType)
                     || Java::Runtime::Map::isAssignableFrom(env, containerType)){
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
            size_t size1 = 0;
            size_t align1 = 0;
            bool isPointer2 = AbstractContainerAccess::isPointerType(metaType2);
            size_t size2 = 0;
            size_t align2 = 0;
            if(!isPointer1){
                if(const std::type_info* t = getTypeByQtName(keyType)){
                    size1 = getValueSize(*t);
                    align1 = getValueAlignment(*t);
                }else{
                    if(metaType1.id() != QMetaType::UnknownType){
                        int sz = metaType1.sizeOf();
                        size1 = sz<0 ? 0 : size_t(sz);
                    }
                }
                if(size1==0)
                    size1 = QtJambiTypeManager::getInternalSize(keyType);
                if(align1==0)
                    align1 = QtJambiTypeManager::getInternalAlignment(keyType);
            }
            if(!isPointer2){
                if(const std::type_info* t = getTypeByQtName(valueType)){
                    size2 = getValueSize(*t);
                    align2 = getValueAlignment(*t);
                }else{
                    if(metaType2.id() != QMetaType::UnknownType){
                        int sz = metaType2.sizeOf();
                        size2 = sz<0 ? 0 : size_t(sz);
                    }
                }
                if(size2==0)
                    size2 = QtJambiTypeManager::getInternalSize(valueType);
                if(align2==0)
                    align2 = QtJambiTypeManager::getInternalAlignment(valueType);
            }

            containerAccess = AbstractContainerAccess::create(env, type, metaType1, metaType2);
            if(!containerAccess){
                QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, metaType1.id());
                QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, metaType2.id());
                InternalToExternalConverter memberConverter1 = QtJambiTypeManager::tryGetInternalToExternalConverter(
                            env,
                            metaType1.name(),
                            metaType1,
                            keyClass);
                if(!memberConverter1)
                    memberConverter1 = ResolvingInternalToExternalConverter(keyClass, metaType1);

                ExternalToInternalConverter memberReConverter1 = QtJambiTypeManager::tryGetExternalToInternalConverter(
                            env,
                            keyClass,
                            metaType1.name(),
                            metaType1
                        );
                if(!memberReConverter1)
                    memberReConverter1 = ResolvingExternalToInternalConverter(keyClass, metaType1);

                InternalToExternalConverter memberConverter2 = QtJambiTypeManager::tryGetInternalToExternalConverter(
                            env,
                            metaType2.name(),
                            metaType2,
                            valueClass);
                if(!memberConverter2)
                    memberConverter2 = ResolvingInternalToExternalConverter(valueClass, metaType2);

                ExternalToInternalConverter memberReConverter2 = QtJambiTypeManager::tryGetExternalToInternalConverter(
                            env,
                            valueClass,
                            metaType2.name(),
                            metaType2
                        );
                if(!memberReConverter2)
                    memberReConverter2 = ResolvingExternalToInternalConverter(valueClass, metaType2);

                containerAccess = AbstractContainerAccess::create(
                                                env, type,
                                                metaType1,
                                                align1, size1,
                                                isPointer1,
                                                hashFunction1,
                                                memberConverter1,
                                                memberReConverter1,
                                                metaType2,
                                                align2, size2,
                                                isPointer2,
                                                hashFunction2,
                                                memberConverter2,
                                                memberReConverter2);
            }
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
            int id = 0;
            for(QString& name : names){
                id = containerAccess->registerContainer(name.toLatin1());
            }
            containerAccess->dispose();
            return id;
        }
        }

        return registerMetaType(env, containerType, false, false);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return 0;
    }
}

int CoreAPI::metaTypeId(JNIEnv *env, jclass clazz, jobjectArray instantiations){
    return qtjambiMetaTypeId(env, clazz, instantiations, nullptr);
}

int qtjambiMetaTypeId(JNIEnv *env, jclass clazz, jobjectArray instantiations, QString* templateName)
{
    using namespace RegistryAPI;
    int id = QMetaType::UnknownType;
    if(clazz){
        if(jobject result = Java::QtJambi::MetaTypeUtility::analyzeExpectedTemplateName(env, clazz, instantiations)){
            if(Java::Runtime::String::isInstanceOf(env, result)){
                QString templName = qtjambi_cast<QString>(env, result);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                id = QMetaType::type(qPrintable(templName));
#else
                id = QMetaType::fromName(templName.toUtf8()).id();
#endif
                if(templateName)
                    *templateName = templName;
            }else if(Java::QtJambi::QtEnumerator::isInstanceOf(env, result)){
                id = Java::QtJambi::QtEnumerator::value(env, result);
            }
        }
        if(id!=QMetaType::UnknownType){// ok so far
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
            return QMetaType::Int;
        }else if(Java::Runtime::Long::isSameClass(env, clazz) || Java::Runtime::Long::isPrimitiveType(env, clazz)){
            return QMetaType::LongLong;
        }else if(Java::Runtime::Short::isSameClass(env, clazz) || Java::Runtime::Short::isPrimitiveType(env, clazz)){
            return QMetaType::Short;
        }else if(Java::Runtime::Byte::isSameClass(env, clazz) || Java::Runtime::Byte::isPrimitiveType(env, clazz)){
            return QMetaType::SChar;
        }else if(Java::Runtime::Boolean::isSameClass(env, clazz) || Java::Runtime::Boolean::isPrimitiveType(env, clazz)){
            return QMetaType::Bool;
        }else if(Java::Runtime::Character::isSameClass(env, clazz) || Java::Runtime::Character::isPrimitiveType(env, clazz)){
            return QMetaType::QChar;
        }else if(Java::Runtime::Float::isSameClass(env, clazz) || Java::Runtime::Float::isPrimitiveType(env, clazz)){
            return QMetaType::Float;
        }else if(Java::Runtime::Double::isSameClass(env, clazz) || Java::Runtime::Double::isPrimitiveType(env, clazz)){
            return QMetaType::Double;
        }else{
            QString javaClassName = QtJambiAPI::getClassName(env, clazz).replace('.', '/');
            if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                id = registeredMetaTypeID(*typeId);
                if(id==QMetaType::UnknownType){
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
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    id = QMetaType::type(qtType);
#else
                    id = QMetaType::fromName(qtType).id();
#endif
                }
            }
            if(id==QMetaType::UnknownType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                id = QMetaType::type(qPrintable(javaClassName.replace("/", "::").replace("$", "::")));
#else
                id = QMetaType::fromName(javaClassName.toUtf8().replace("/", "::").replace("$", "::")).id();
#endif
                if(id==QMetaType::UnknownType
                         && !Java::Runtime::Cloneable::isAssignableFrom(env, clazz)
                         && !Java::Runtime::Enum::isAssignableFrom(env, clazz)
                         && !Java::QtJambi::QFlags::isAssignableFrom(env, clazz)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                id = QMetaType::type(qPrintable(javaClassName.replace("/", "::").replace("$", "::").append("*")));
#else
                id = QMetaType::fromName(javaClassName.toUtf8().replace("/", "::").replace("$", "::").append("*")).id();
#endif
                }

            }
        }
    }else{
        id = QMetaType::Nullptr;
    }
    return id;
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
jclass registeredJavaClassForCustomMetaType(QMetaType metaType);
#else
jclass registeredJavaClassForCustomMetaType(const QMetaType& metaType);
#endif

jclass CoreAPI::getClassForMetaType(JNIEnv *env, const QMetaType& metaType)
{
    QByteArray typeName(metaType.name());
    const char* name(nullptr);
    if(const std::type_info* typeId = getTypeByMetaType(metaType.id())){
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
    if(env->IsSameObject(object, nullptr)){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        return QVariant(metaType, nullptr);
#else
        return QVariant(metaType.id(), nullptr);
#endif
    }
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, object)){
        jclass typeClass = CoreAPI::getClassForMetaType(env, metaType);
        if(typeClass && env->IsInstanceOf(object, typeClass)){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
            if(!QMetaType(metaType).iface()->copyCtr){
                Java::Runtime::UnsupportedOperationException::throwNew(env, QStringLiteral("Unable to create value of meta type %1 due to missing copy constructor.").arg(QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
            }
#endif
            if(metaType.flags() & QMetaType::IsPointer){
                void* ptr = link->pointer();
                return QVariant(metaType, &ptr);
            }else{
                return QVariant(metaType, link->pointer());
            }
#else
            if(metaType.name().endsWith("*")){
                void* ptr = link->pointer();
                return QVariant(metaType.id(), &ptr);
            }else{
                return QVariant(metaType.id(), link->pointer());
            }
#endif
        }
    }else if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, object))
        Java::QtJambi::QNoNativeResourcesException::throwNew(env, QStringLiteral("Incomplete object of type: %1").arg(QtJambiAPI::getObjectClassName(env, object).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
    QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(env, object);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if(variant.metaType()!=metaType){
        if(variant.convert(metaType)){
#else
    if(variant.userType()!=metaType.id()){
        if(variant.convert(metaType.id())){
#endif
            return variant;
        }
        Java::Runtime::IllegalArgumentException::throwNew(env, QStringLiteral("Object of type %1 incompatible with meta type %2.").arg(QtJambiAPI::getObjectClassName(env, object).replace("$", "."), QLatin1String(metaType.name())) QTJAMBI_STACKTRACEINFO );
        return QVariant();
    }else{
        return variant;
    }
}

hash_type CoreAPI::computeHash(const QMetaType& metaType, const void* ptr, hash_type seed, bool* success)
{
    QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(AbstractContainerAccess::isPointerType(metaType), metaType.id());
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
                if(sender->isSmartPointer()){
                    QSharedPointerToQObjectLink* slink = static_cast<QSharedPointerToQObjectLink*>(sender.data());
                    return slink->getExtraSignal(env, *method);
                }else{
                    PointerToQObjectLink* plink = static_cast<PointerToQObjectLink*>(sender.data());
                    return plink->getExtraSignal(env, *method);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
