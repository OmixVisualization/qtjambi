/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambi_repository_p.h"
#include "qtjambi_core.h"
#include "qtjambi_cast.h"
#include "qtjambi_thread_p.h"
#include "qtjambi_registry_p.h"
#include "qtjambilink_p.h"
#include "qtjambitypemanager_p.h"
#include "qtdynamicmetaobject_p.h"

#include <cstring>
#include <QtCore/QMetaType>
#include <QtCore/QRect>
#include <QtCore/QTime>
#include <QtCore/QSize>
#include <QtCore/QBasicTimer>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QThreadStorage>

#include <QtGui/QMouseEvent>
#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QCursor>
#include <QtGui/QIcon>
#include <QtGui/QPainter>
#include <QtGui/QPolygon>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/private/qmetaobject_p.h>

#include <QDebug>

static const QSet<QMetaMethod> forbidddenMethods = []() -> QSet<QMetaMethod> {
        QSet<QMetaMethod> methods;
        int idx;
        //idx = QObject::staticMetaObject.indexOfMethod("destroyed(QObject*)");
        //if(idx>=0)
        //    methods.insert(QObject::staticMetaObject.method(idx));
        //idx = QObject::staticMetaObject.indexOfMethod("destroyed()");
        //if(idx>=0)
        //    methods.insert(QObject::staticMetaObject.method(idx));
        //idx = QObject::staticMetaObject.indexOfMethod("deleteLater()");
        //if(idx>=0)
        //    methods.insert(QObject::staticMetaObject.method(idx));
        idx = QObject::staticMetaObject.indexOfMethod("_q_reregisterTimers(void*)");
        if(idx>=0)
            methods.insert(QObject::staticMetaObject.method(idx));
        return methods;
    }();

bool method_is_valid(const QMetaMethod& method){
    return method.isValid() && !forbidddenMethods.contains(method);
}

typedef QHash<const QMetaObject *, jweak> JavaMetaObjectHash;
Q_GLOBAL_STATIC(JavaMetaObjectHash, javaMetaObjects);
Q_GLOBAL_STATIC_WITH_ARGS(QMutex, javaMetaObjectsLock, (QMutex::Recursive));

typedef QHash<uint,const QMetaObject *> MetaObjectByNameHash;
Q_GLOBAL_STATIC(MetaObjectByNameHash, metaObjectsByName);

const QMetaObject * findMetaObject(const char* name){
    const QMetaObject* metaObject = nullptr;
    if(const std::type_info* typeId = getTypeByQtName(name)){
        metaObject = registeredOriginalMetaObject(*typeId);
    }
    if(!metaObject)
        metaObject = metaObjectsByName()->value(qHash(name), nullptr);
    return metaObject;
}

jobject qtjambi_from_QMetaObject(JNIEnv *env, const QMetaObject *metaObject){
    if(qtjambi_metaobject_is_dynamic(metaObject)){
        const QtDynamicMetaObject* dyMetaObject = reinterpret_cast<const QtDynamicMetaObject*>(metaObject);
        jweak javaInstance = QtDynamicMetaObject::javaInstance(dyMetaObject);
        if(env->IsSameObject(javaInstance, nullptr)){
            jobject jmetaObject = Java::Private::QtCore::QMetaObject.newInstance(env, jlong(reinterpret_cast<const void*>(metaObject)));
            QtDynamicMetaObject::setJavaInstance(dyMetaObject, env->NewWeakGlobalRef(jmetaObject));
            return jmetaObject;
        }else{
            return env->NewLocalRef(javaInstance);
        }
    }else{
        QMutexLocker locker(javaMetaObjectsLock());
        Q_UNUSED(locker)
        jweak javaInstance = javaMetaObjects()->value(metaObject, nullptr);
        if(env->IsSameObject(javaInstance, nullptr)){
            jobject jmetaObject = Java::Private::QtCore::QMetaObject.newInstance(env, jlong(reinterpret_cast<const void*>(metaObject)));
            javaMetaObjects()->insert(metaObject, env->NewWeakGlobalRef(jmetaObject));
            QLatin1String javaName;
            if(const std::type_info* typeId = getTypeByMetaObject(metaObject)){
                javaName = QLatin1String(getJavaName(*typeId));
            }
            if(javaName.isEmpty()){
                if(const std::type_info* typeId = getTypeByQtName(metaObject->className()))
                    javaName = QLatin1String(getJavaName(*typeId));
            }
            if(javaName.isEmpty()){
                metaObjectsByName()->insert(qHash(metaObject->className()), metaObject);
            }
            return jmetaObject;
        }else{
            return env->NewLocalRef(javaInstance);
        }
    }
}

jobject qtjambi_from_QMetaMethod(JNIEnv *env, jobject jmetaObject, const QMetaObject *metaObject, const QMetaMethod& method, jobject jreturnType, jobject parameterList){
    if(!method_is_valid(method))
        return nullptr;
    bool isClassList = false;
    jintArray parameterMetaTypes;
    int returnMetaType = QMetaType::UnknownType;
    if(!parameterList){
        QList<QByteArray> __qt_parameterTypes = method.parameterTypes();
        parameterMetaTypes = env->NewIntArray(__qt_parameterTypes.size());
        jint* parameterMetaTypeArray = env->GetIntArrayElements(parameterMetaTypes, nullptr);
        parameterList = qtjambi_arraylist_new(env, __qt_parameterTypes.size());
        for(int i=0; i<__qt_parameterTypes.size(); ++i){
            QString qTypeName = QLatin1String(__qt_parameterTypes.at(i).data());
            int metaType = method.parameterType(i);
            parameterMetaTypeArray[i] = metaType;
            if(qTypeName.isEmpty())
                qTypeName = QLatin1String(QMetaType::typeName(metaType));
            QString paramTypeName = QtJambiTypeManager::getExternalTypeName(env, qTypeName, metaObject, QtJambiTypeManager::ArgumentType, metaType);
            if(paramTypeName.isEmpty()){
                if(metaType!=QMetaType::UnknownType && metaType!=QMetaType::Void && metaType!=QMetaType::Nullptr){
                    if(qTypeName.endsWith("*")){
                        paramTypeName = QLatin1String("io/qt/QNativePointer");
                        if(QMetaType(metaType).flags() & QMetaType::PointerToQObject
                                || QMetaType(metaType).flags() & QMetaType::SharedPointerToQObject
                                || QMetaType(metaType).flags() & QMetaType::WeakPointerToQObject){
                            paramTypeName = QLatin1String("io/qt/core/QObject");
                        }else if(QMetaType(metaType).flags() & QMetaType::PointerToGadget){
                             paramTypeName = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                        }
                    }else{
                        paramTypeName = QLatin1String("io/qt/core/QMetaType$GenericValue");
                    }
                }else{
                    paramTypeName = QLatin1String("java/lang/Object");
                }
            }
            qtjambi_collection_add(env, parameterList, qtjambi_from_qstring(env, paramTypeName));
        }
        env->ReleaseIntArrayElements(parameterMetaTypes, parameterMetaTypeArray, 0);
        QString returnTypeName;
        if(method.methodType()!=QMetaMethod::Constructor){
            QString qTypeName = QLatin1String(method.typeName());
            returnMetaType = method.returnType();
            if(qTypeName.isEmpty())
                qTypeName = QLatin1String(QMetaType::typeName(returnMetaType));
            returnTypeName = QtJambiTypeManager::getExternalTypeName(env, qTypeName, metaObject, QtJambiTypeManager::ReturnType, returnMetaType);
            if(returnTypeName.isEmpty()){
                if(returnMetaType!=QMetaType::UnknownType && returnMetaType!=QMetaType::Void && returnMetaType!=QMetaType::Nullptr){
                    if(qTypeName.endsWith("*")){
                        returnTypeName = QLatin1String("io/qt/QNativePointer");
                        if(QMetaType(returnMetaType).flags() & QMetaType::PointerToQObject
                                || QMetaType(returnMetaType).flags() & QMetaType::SharedPointerToQObject
                                || QMetaType(returnMetaType).flags() & QMetaType::WeakPointerToQObject){
                            returnTypeName = QLatin1String("io/qt/core/QObject");
                        }else if(QMetaType(returnMetaType).flags() & QMetaType::PointerToGadget){
                             returnTypeName = QLatin1String("io/qt/core/QMetaType$GenericGadget");
                        }
                    }else{
                        returnTypeName = QLatin1String("io/qt/core/QMetaType$GenericValue");
                    }
                }else{
                    returnTypeName = QLatin1String("java/lang/Object");
                }
            }
        }
        jreturnType = qtjambi_from_qstring(env, returnTypeName);
    }else{
        parameterMetaTypes = env->NewIntArray(method.parameterCount());
        jint* parameterMetaTypeArray = env->GetIntArrayElements(parameterMetaTypes, nullptr);
        for(int i=0; i<method.parameterCount(); ++i){
            parameterMetaTypeArray[i] = method.parameterType(i);
        }
        env->ReleaseIntArrayElements(parameterMetaTypes, parameterMetaTypeArray, 0);
        returnMetaType = method.methodType()==QMetaMethod::Constructor ? QMetaType::Void : method.returnType();
        isClassList = true;
    }
    if(!jmetaObject){
        jmetaObject = qtjambi_from_QMetaObject(env, metaObject);
    }
    jobject enclosingMetaObject = nullptr;
    if(method.enclosingMetaObject()==metaObject){
        enclosingMetaObject = jmetaObject;
    }else if(method.enclosingMetaObject()){
        enclosingMetaObject = qtjambi_from_QMetaObject(env, method.enclosingMetaObject());
    }
    jobject result;
    QString methodName = method.name();
    if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
        if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
            methodName = QLatin1String(newName);
        }
    }
    if(isClassList)
        result = Java::Private::QtCore::QMetaMethod.newInstance2(env,
                          enclosingMetaObject,
                          qtjambi_from_qstring(env, methodName),
                          jreturnType,
                          parameterList,
                          returnMetaType,
                          parameterMetaTypes,
                          qtjambi_from_qstring(env, method.tag()),
                          method.methodIndex(),
                          jint(method.access()),
                          jint(method.methodType())
                    );
    else
        result = Java::Private::QtCore::QMetaMethod.newInstance(env,
                          enclosingMetaObject,
                          qtjambi_from_qstring(env, methodName),
                          jreturnType,
                          parameterList,
                          returnMetaType,
                          parameterMetaTypes,
                          qtjambi_from_qstring(env, method.tag()),
                          method.methodIndex(),
                          jint(method.access()),
                          jint(method.methodType())
                    );
    return result;
}

jobject qtjambi_from_QMetaProperty(JNIEnv *env, jobject jmetaObject, const QMetaProperty& property){
    if(!property.isValid())
        return nullptr;
    int metaType = property.type()==QVariant::UserType ? property.userType() : int(property.type());
    jclass propertyType = nullptr;
    if(QtDynamicMetaObject::isDynamic(property.enclosingMetaObject())){
        const QtDynamicMetaObject* metaObject = static_cast<const QtDynamicMetaObject*>(property.enclosingMetaObject());
        propertyType = metaObject->typeOfProperty(property.propertyIndex() - metaObject->propertyOffset());
    }else{
        QString typeName = QtJambiTypeManager::getExternalTypeName(env, property.typeName(), QtJambiTypeManager::ArgumentType, metaType);
        if(typeName.isEmpty()){
            if(metaType!=QMetaType::UnknownType && metaType!=QMetaType::Void && metaType!=QMetaType::Nullptr){
                if(QByteArray(property.typeName()).endsWith("*")){
                    propertyType = Java::Private::QtJambi::QNativePointer.getClass(env);
                    if(QMetaType(metaType).flags() & QMetaType::PointerToQObject
                            || QMetaType(metaType).flags() & QMetaType::SharedPointerToQObject
                            || QMetaType(metaType).flags() & QMetaType::WeakPointerToQObject){
                        propertyType = Java::Private::QtCore::QObject.getClass(env);
                    }else if(QMetaType(metaType).flags() & QMetaType::PointerToGadget){
                         propertyType = Java::Private::QtCore::QMetaType$GenericGadget.getClass(env);
                    }
                }else{
                    propertyType = Java::Private::QtCore::QMetaType$GenericValue.getClass(env);
                }
            }else{
                propertyType = Java::Private::Runtime::Object.getClass(env);
            }
        }
        if(!typeName.isEmpty()){
            if(typeName=="int"){
                propertyType = Java::Runtime::Integer.primitiveType(env);
            }else if(typeName=="short"){
                propertyType = Java::Runtime::Short.primitiveType(env);
            }else if(typeName=="long"){
                propertyType = Java::Runtime::Long.primitiveType(env);
            }else if(typeName=="byte"){
                propertyType = Java::Runtime::Byte.primitiveType(env);
            }else if(typeName=="boolean"){
                propertyType = Java::Runtime::Boolean.primitiveType(env);
            }else if(typeName=="double"){
                propertyType = Java::Runtime::Double.primitiveType(env);
            }else if(typeName=="char"){
                propertyType = Java::Runtime::Character.primitiveType(env);
            }else if(typeName=="float"){
                propertyType = Java::Runtime::Float.primitiveType(env);
            }else{
                propertyType = resolveClass(env, qPrintable(typeName));
            }
        }
    }
    if(!propertyType)
        propertyType = Java::Private::Runtime::Object.getClass(env);
    jint flags = Invalid;
    if(property.isReadable())
        flags |= Readable;
    if(property.isWritable())
        flags |= Writable;
    if(property.isScriptable())
        flags |= Scriptable;
    if(property.isEditable())
        flags |= Editable;
    if(property.isStored())
        flags |= Stored;
    if(property.isDesignable())
        flags |= Designable;
    if(property.isResettable())
        flags |= Resettable;
    if(property.hasNotifySignal())
        flags |= Notify;
    if(property.isConstant())
        flags |= Constant;
    if(property.isFinal())
        flags |= Final;
    if(property.isUser())
        flags |= User;
    if(property.hasStdCppSet())
        flags |= StdCppSet;
    jobject result = Java::Private::QtCore::QMetaProperty.newInstance(env,
                          jmetaObject,
                          property.propertyIndex(),
                          qtjambi_from_qstring(env, property.name()),
                          propertyType,
                          flags
                        );
    return result;
}

jobject qtjambi_from_QMetaEnum(JNIEnv *env, const QMetaEnum& enumerator, int enumeratorIdx)
{
    if(enumerator.isValid()){
        jobject metaObject = qtjambi_from_QMetaObject(env, enumerator.enclosingMetaObject());
        jobject result = Java::Private::QtCore::QMetaEnum.newInstance(env,
                              metaObject,
                              enumeratorIdx
                            );
        return result;
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_userProperty)
    (JNIEnv *env, jobject jmetaObject, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            Q_ASSERT(metaObject != nullptr);
            return qtjambi_from_QMetaProperty(env, jmetaObject, metaObject->userProperty());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_properties)
    (JNIEnv *env, jobject jmetaObject, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int count = metaObject->propertyCount();
            jobject propertyList = qtjambi_arraylist_new(env, count);
            Q_ASSERT(propertyList != nullptr);

            for (int i=0; i<count; ++i) {
                jobject javaProperty = qtjambi_from_QMetaProperty(env, jmetaObject, metaObject->property(i));
                Q_ASSERT(javaProperty != nullptr);
                qtjambi_collection_add(env, propertyList, javaProperty);
            }

            return propertyList;
        }
        return qtjambi_arraylist_new(env, 0);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_property)
    (JNIEnv *env, jobject jmetaObject, jlong metaObjectPointer, jobject name)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int i = metaObject->indexOfProperty(qPrintable(qtjambi_to_qstring(env, jstring(name))));
            return qtjambi_from_QMetaProperty(env, jmetaObject, metaObject->property(i));
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_connect)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id, jstring signalName, QtJambiNativeID receiver__id, jstring slotName, jbyte connectionType)
{
    jobject result = nullptr;
    try{
        QSharedPointer<QtJambiLink> sender = QtJambiLink::fromNativeId(sender__id);
        QSharedPointer<QtJambiLink> receiver = QtJambiLink::fromNativeId(receiver__id);
        if(sender && sender->isQObject() && receiver && receiver->isQObject()){
            QByteArray normalizedSignal = QMetaObject::normalizedSignature(qPrintable(qtjambi_to_qstring(env, signalName)));
            QByteArray normalizedSlot = QMetaObject::normalizedSignature(qPrintable(qtjambi_to_qstring(env, slotName)));
            QMetaMethod signalMethod = sender->qobject()->metaObject()->method(sender->qobject()->metaObject()->indexOfMethod(normalizedSignal.constData()));
            QMetaMethod slotMethod = receiver->qobject()->metaObject()->method(receiver->qobject()->metaObject()->indexOfMethod(normalizedSlot.constData()));
            if(!signalMethod.isValid()){
                for(int i=0; i<sender->qobject()->metaObject()->methodCount(); ++i){
                    QMetaMethod method = sender->qobject()->metaObject()->method(i);
                    if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
                        if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
                            QByteArray methodSignature = method.methodSignature();
                            int idxFirstParen = methodSignature.indexOf('(');
                            if(idxFirstParen>=0){
                                methodSignature = methodSignature.remove(0, idxFirstParen + 1);
                            }
                            methodSignature.prepend(newName);
                            if(normalizedSignal==methodSignature){
                                signalMethod = method;
                                break;
                            }
                        }
                    }
                }
            }
            if(!slotMethod.isValid()){
                for(int i=0; i<receiver->qobject()->metaObject()->methodCount(); ++i){
                    QMetaMethod method = receiver->qobject()->metaObject()->method(i);
                    if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
                        if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
                            QByteArray methodSignature = method.methodSignature();
                            int idxFirstParen = methodSignature.indexOf('(');
                            if(idxFirstParen>=0){
                                methodSignature = methodSignature.remove(0, idxFirstParen + 1);
                            }
                            methodSignature.prepend(newName);
                            if(normalizedSlot==methodSignature){
                                slotMethod = method;
                                break;
                            }
                        }
                    }
                }
            }
            if(method_is_valid(signalMethod) && method_is_valid(slotMethod)){
                QMetaObject::Connection connection = QObject::connect(sender->qobject(), signalMethod, receiver->qobject(), slotMethod, Qt::ConnectionType(connectionType));
                if(connection){
                    result = qtjambi_cast<jobject>(env, connection);
                    qtjambi_set_java_ownership(env, result);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_connectMethods)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id, jint signalIdx, jlong signalEnclosingMetaObjectPointer, QtJambiNativeID receiver__id, jint slotIdx, jlong receiverEnclosingMetaObjectPointer, jbyte connectionType)
{
    jobject result = nullptr;
    try{
        QSharedPointer<QtJambiLink> sender = QtJambiLink::fromNativeId(sender__id);
        QSharedPointer<QtJambiLink> receiver = QtJambiLink::fromNativeId(receiver__id);
        const QMetaObject *signalEnclosingMetaObject = reinterpret_cast<const QMetaObject *>(signalEnclosingMetaObjectPointer);
        const QMetaObject *receiverEnclosingMetaObject = reinterpret_cast<const QMetaObject *>(receiverEnclosingMetaObjectPointer);
        if(signalEnclosingMetaObject && receiverEnclosingMetaObjectPointer && sender && sender->isQObject() && receiver && receiver->isQObject()){
            QMetaMethod signalMethod = signalEnclosingMetaObject->method(signalIdx);
            QMetaMethod slotMethod = receiverEnclosingMetaObject->method(slotIdx);
            if(method_is_valid(signalMethod) && method_is_valid(slotMethod)){
                QMetaObject::Connection connection = QObject::connect(sender->qobject(), signalMethod, receiver->qobject(), slotMethod, Qt::ConnectionType(connectionType));
                if(connection){
                    result = qtjambi_cast<jobject>(env, connection);
                    qtjambi_set_java_ownership(env, result);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_disconnect)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id, jstring signalName, QtJambiNativeID receiver__id, jstring slotName)
{
    bool result = false;
    try{
        QSharedPointer<QtJambiLink> sender = QtJambiLink::fromNativeId(sender__id);
        if(sender && sender->isQObject() && sender->qobject()){
            QSharedPointer<QtJambiLink> receiverLink = QtJambiLink::fromNativeId(receiver__id);
            QString qsignalName = qtjambi_to_qstring(env, signalName);
            QString qslotName = qtjambi_to_qstring(env, slotName);
            QByteArray normalizedSignal = QMetaObject::normalizedSignature(qPrintable(qsignalName));
            QByteArray normalizedSlot = QMetaObject::normalizedSignature(qPrintable(qslotName));
            QMetaMethod signalMethod;
            if(!normalizedSignal.isEmpty()){
                signalMethod = sender->qobject()->metaObject()->method(sender->qobject()->metaObject()->indexOfMethod(normalizedSignal.constData()));
                if(!signalMethod.isValid()){
                    for(int i=0; i<sender->qobject()->metaObject()->methodCount(); ++i){
                        QMetaMethod method = sender->qobject()->metaObject()->method(i);
                        if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
                            if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
                                QByteArray methodSignature = method.methodSignature();
                                int idxFirstParen = methodSignature.indexOf('(');
                                if(idxFirstParen>=0){
                                    methodSignature = methodSignature.remove(0, idxFirstParen + 1);
                                }
                                methodSignature.prepend(newName);
                                if(normalizedSignal==methodSignature){
                                    signalMethod = method;
                                    normalizedSignal = methodSignature;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            QObject* receiver = nullptr;
            QMetaMethod slotMethod;
            if(receiverLink && receiverLink->isQObject()){
                receiver = receiverLink->qobject();
                if(receiver){
                    slotMethod = receiver->metaObject()->method(receiver->metaObject()->indexOfMethod(normalizedSlot.constData()));
                    if(!slotMethod.isValid()){
                        for(int i=0; i<receiver->metaObject()->methodCount(); ++i){
                            QMetaMethod method = receiver->metaObject()->method(i);
                            if(const QHash<int,const char*>* _renamedMethods = renamedMethods(method.enclosingMetaObject())){
                                if(const char* newName = _renamedMethods->value(method.methodIndex(), nullptr)){
                                    QByteArray methodSignature = method.methodSignature();
                                    int idxFirstParen = methodSignature.indexOf('(');
                                    if(idxFirstParen>=0){
                                        methodSignature = methodSignature.remove(0, idxFirstParen + 1);
                                    }
                                    methodSignature.prepend(newName);
                                    if(normalizedSlot==methodSignature){
                                        slotMethod = method;
                                        normalizedSlot = methodSignature;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if((!signalMethod.isValid() || method_is_valid(signalMethod))
                    && (!slotMethod.isValid() ||method_is_valid(slotMethod))){
                result = QObject::disconnect(sender->qobject(), signalMethod, receiver, slotMethod);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_disconnectMethods)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id, jint signalIdx, jlong signalEnclosingMetaObjectPointer, QtJambiNativeID receiver__id, jint slotIdx, jlong receiverEnclosingMetaObjectPointer)
{
    bool result = false;
    try{
        QSharedPointer<QtJambiLink> senderLink = QtJambiLink::fromNativeId(sender__id);
        if(senderLink && senderLink->isQObject()){
            QSharedPointer<QtJambiLink> receiverLink = QtJambiLink::fromNativeId(receiver__id);
            const QMetaObject *signalEnclosingMetaObject = reinterpret_cast<const QMetaObject *>(signalEnclosingMetaObjectPointer);
            const QMetaObject *receiverEnclosingMetaObject = reinterpret_cast<const QMetaObject *>(receiverEnclosingMetaObjectPointer);
            QObject* receiver = nullptr;
            if(receiverLink && receiverLink->isQObject()){
                receiver = receiverLink->qobject();
            }
            QMetaMethod signalMethod;
            QMetaMethod slotMethod;
            if(signalEnclosingMetaObject && signalIdx>=0){
                signalMethod = signalEnclosingMetaObject->method(signalIdx);
            }
            if(receiverEnclosingMetaObject && slotIdx>=0){
                slotMethod = receiverEnclosingMetaObject->method(slotIdx);
            }
            if((!signalMethod.isValid() || method_is_valid(signalMethod))
                    && (!slotMethod.isValid() ||method_is_valid(slotMethod))){
                result = QObject::disconnect(senderLink->qobject(), signalMethod, receiver, slotMethod);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_cast)
    (JNIEnv *env, jclass, QtJambiNativeID object__id, jclass targetType)
{
    jobject result = nullptr;
    try{
        QSharedPointer<QtJambiLink> objectLink = QtJambiLink::fromNativeId(object__id);
        if(objectLink){
            QString javaName = qtjambi_class_name(env, targetType).replace(QLatin1Char('.'), QLatin1Char('/'));
            if(const std::type_info* typeId = getTypeByJavaName(javaName)){
                void* basicPointer = objectLink->pointer();
                void* interfacePointer = nullptr;
                if(objectLink->isQObject()){
                    interfacePointer = objectLink->qobject()->qt_metacast(getQtName(*typeId));
                    if(!interfacePointer){
                        interfacePointer = objectLink->typedPointer(*typeId);
                    }
                }else{
                    interfacePointer = objectLink->typedPointer(*typeId);
                }
                if(interfacePointer && interfacePointer!=basicPointer){
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(interfacePointer)){
                        jobject obj = link->getJavaObjectLocalRef(env);
                        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                            link->invalidate(env);
                        }else{
                            return obj;
                        }
                    }
                    if(const QtJambiTypeEntry* typeEntry = QtJambiTypeEntry::getTypeEntry(env, *typeId)){
                        result = env->NewObject(typeEntry->creatableClass(), typeEntry->creatorMethod(), 0);
                        qtjambi_throw_java_exception(env)
                        QtJambiLink::createLinkForInterface(env, result, interfacePointer,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                            getQtName(*typeId),
#endif
                                                            objectLink);
                    }
                }
            }else{
                void* basicPointer = objectLink->pointer();
                void* interfacePointer = nullptr;
                if(objectLink->isQObject()){
                    interfacePointer = objectLink->qobject()->qt_metacast(qPrintable(javaName.replace("/", "::").replace("$", "::")));
                }
                if(interfacePointer && interfacePointer!=basicPointer){
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForPointer(interfacePointer)){
                        jobject obj = link->getJavaObjectLocalRef(env);
                        if(!obj && link->ownership()==QtJambiLink::Ownership::Split){
                            link->invalidate(env);
                        }else{
                            return obj;
                        }
                    }
                    result = qtjambi_from_object(env, interfacePointer, targetType, false, false);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_query)
    (JNIEnv *env, jclass, QtJambiNativeID object__id, jint propertyAbsoluteIndex, jint call)
{
    bool b = false;
    try{
        QSharedPointer<QtJambiLink> objectLink = QtJambiLink::fromNativeId(object__id);
        if(objectLink && objectLink->isQObject()){
            void *argv[] = { &b };
            QMetaObject::metacall(objectLink->qobject(), QMetaObject::Call(call), propertyAbsoluteIndex, argv);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return b;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_reset)
    (JNIEnv *env, jclass, QtJambiNativeID object__id, jlong metaObjectPointer, jint propertyIndex)
{
    try{
        QSharedPointer<QtJambiLink> objectLink = QtJambiLink::fromNativeId(object__id);
        if(objectLink && objectLink->isQObject()){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            return metaObject->property(propertyIndex).reset(objectLink->qobject());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_resetOnGadget)
    (JNIEnv *env, jclass, jobject object, jlong metaObjectPointer, jint propertyIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env, object)){
            const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, object);
            void* ptr = nullptr;
            const std::type_info* typeId = nullptr;
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
            qtjambi_check_resource(env, ptr, *typeId);
            return metaObject->property(propertyIndex).resetOnGadget(ptr);
        }else{
            return metaObject->property(propertyIndex).resetOnGadget(object);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_write)
    (JNIEnv * env, jclass, QtJambiNativeID object__id, jlong metaObjectPointer, jint propertyIndex, jobject value)
{
    try{
        QSharedPointer<QtJambiLink> objectLink = QtJambiLink::fromNativeId(object__id);
        if(objectLink && objectLink->isQObject()){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaProperty property = metaObject->property(propertyIndex);
            int type;
            if (property.type()==QVariant::Invalid) {
                return false;
            }else if(property.type()==QVariant::UserType){
                type = property.userType();
            }else{
                type = property.type();
            }
            QLatin1String qtName(property.typeName());
            if(const std::type_info* typeId = getTypeByQtName(qtName)){
                const char* fullJavaName = getJavaName(*typeId);
                jclass object_class = resolveClass(env, fullJavaName);
                Q_ASSERT(object_class);
                if(value && !env->IsInstanceOf(value, object_class)){
                    return false;
                }
                jvalue val;
                val.l = value;
                qtjambi_throw_java_exception(env)
                jobject object_classLoader = nullptr;
                void *copy = nullptr;
                QtJambiTypeManager::TypePattern internalType = QtJambiTypeManager::typeIdOfInternal(env, fullJavaName, qtName, type, object_classLoader);
                const ExternalToInternalConverter& converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, internalType, qtName, type);
                QtJambiScope scope(nullptr);
                ConvertResponse result = converter(env, &scope, val, copy, jValueType::l);
                switch(result){
                case ConvertResponse::NotSuccessful:
                    qWarning("Cannot convert external type '%s' to '%s'",
                             qPrintable(QString(QLatin1String(fullJavaName)).replace("/", ".").replace("$", ".")),
                             qPrintable(qtName));
                    type = QVariant::Invalid;
                    copy= nullptr;
                    return false;
                case ConvertResponse::JavaObjectNotLinked:
                    qWarning("Java object of type '%s' not linked to C++ object",
                             qPrintable(QString(QLatin1String(fullJavaName)).replace("/", ".").replace("$", ".")));
                    type = QVariant::Invalid;
                    copy= nullptr;
                    return false;
                default:
                    break;
                }
                return property.write(objectLink->qobject(), QVariant(type, copy));
            }
            return false;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_writeOnGadget)
    (JNIEnv * env, jclass, jobject object, jlong metaObjectPointer, jint propertyIndex, jobject value)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env, object)){
            const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, object);
            void* ptr = nullptr;
            const std::type_info* typeId = nullptr;
            do{
                typeId = getTypeByQtName(metaObject->className());
                if(typeId)
                    break;
                metaObject = metaObject->superClass();
            }while(metaObject);
            if(!typeId){
                if(link){
                    ptr = link->typedPointer(*typeId);
                }
            }else{
                typeId = &typeid(JObjectWrapper);
                if(link){
                    ptr = link->pointer();
                }
            }
            qtjambi_check_resource(env, ptr, *typeId);
            QMetaProperty property = metaObject->property(propertyIndex);
            int type;
            if (property.type()==QVariant::Invalid) {
                return false;
            }else if(property.type()==QVariant::UserType){
                type = property.userType();
            }else{
                type = property.type();
            }
            QLatin1String qtName(property.typeName());
            if(const std::type_info* typeId = getTypeByQtName(qtName)){
                const char* fullJavaName = getJavaName(*typeId);
                jclass object_class = resolveClass(env, fullJavaName);
                Q_ASSERT(object_class);
                if(value && !env->IsInstanceOf(value, object_class)){
                    return false;
                }
                jvalue val;
                val.l = value;
                qtjambi_throw_java_exception(env)
                jobject object_classLoader = nullptr;
                void *copy = nullptr;
                QtJambiTypeManager::TypePattern internalType = QtJambiTypeManager::typeIdOfInternal(env, fullJavaName, qtName, type, object_classLoader);
                const ExternalToInternalConverter& converter = QtJambiTypeManager::getExternalToInternalConverter(env, object_class, internalType, qtName, type);
                QtJambiScope scope(nullptr);
                ConvertResponse result = converter(env, &scope, val, copy, jValueType::l);
                switch(result){
                case ConvertResponse::NotSuccessful:
                    qWarning("Cannot convert external type '%s' to '%s'",
                             qPrintable(QString(QLatin1String(fullJavaName)).replace("/", ".").replace("$", ".")),
                             qPrintable(qtName));
                    type = QVariant::Invalid;
                    return false;
                case ConvertResponse::JavaObjectNotLinked:
                    qWarning("Java object of type '%s' not linked to C++ object",
                             qPrintable(QString(QLatin1String(fullJavaName)).replace("/", ".").replace("$", ".")));
                    type = QVariant::Invalid;
                    return false;
                default:
                    break;
                }
                return property.writeOnGadget(ptr, QVariant(type, copy));
            }
            return false;
        }else{
            QVariant __qt_value = qtjambi_to_qvariant(env, value);
            return metaObject->property(propertyIndex).writeOnGadget(object, __qt_value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_read)
    (JNIEnv * __jni_env, jclass, QtJambiNativeID object__id, jlong metaObjectPointer, jint propertyIndex)
{
    try{
        QSharedPointer<QtJambiLink> objectLink = QtJambiLink::fromNativeId(object__id);
        if(objectLink && objectLink->isQObject()){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QVariant result = metaObject->property(propertyIndex).read(objectLink->qobject());
            return qtjambi_from_qvariant(__jni_env, result);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_readOnGadget)
    (JNIEnv * env, jclass, jobject object, jlong metaObjectPointer, jint propertyIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        QVariant result;
        if(Java::Private::QtJambi::QtObjectInterface.isInstanceOf(env, object)){
            const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaObject(env, object);
            void* ptr = nullptr;
            const std::type_info* typeId = nullptr;
            do{
                typeId = getTypeByQtName(metaObject->className());
                if(typeId)
                    break;
                metaObject = metaObject->superClass();
            }while(metaObject);
            if(!typeId){
                if(link){
                    ptr = link->typedPointer(*typeId);
                }
            }else{
                typeId = &typeid(JObjectWrapper);
                if(link){
                    ptr = link->pointer();
                }
            }
            qtjambi_check_resource(env, ptr, *typeId);
            result = metaObject->property(propertyIndex).readOnGadget(ptr);
        }else{
            result = metaObject->property(propertyIndex).readOnGadget(object);
        }
        return qtjambi_from_qvariant(env, result);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaProperty_notifySignal)
    (JNIEnv * env, jclass, jobject jmetaObject, jlong metaObjectPointer, jint propertyIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        QMetaMethod notifySignal = metaObject->property(propertyIndex).notifySignal();
        if(notifySignal.isValid() && notifySignal.methodType()==QMetaMethod::Signal){
            env->EnsureLocalCapacity(100);
            jobject jreturnType = nullptr;
            jobject jparameters = nullptr;
            const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, notifySignal);
            Q_ASSERT(parameterTypeInfos);
            jreturnType = parameterTypeInfos->at(0).javaClass();
            jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
            for(int i=1; i<parameterTypeInfos->size(); ++i){
                qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
            }
            return qtjambi_from_QMetaMethod(env, jmetaObject, metaObject, notifySignal, jreturnType, jparameters);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_method)
    (JNIEnv * env, jobject jmetaObject, jlong metaObjectPointer, jobject normalizedSignature)
{
    try{
        if(metaObjectPointer){
            QString sig = qtjambi_to_qstring(env, jstring(normalizedSignature));
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->method(metaObject->indexOfMethod(qPrintable(sig)));
            if(!method.isValid()){
                QString methodName;
                QVector<QString> arguments = QtJambiTypeManager::parseSignature(sig, &methodName);
                const int methodCount = metaObject->methodCount();
                for(int i=0; i<methodCount; ++i){
                    QMetaMethod _method = metaObject->method(i);
                    QString qmethodName = _method.name();
                    if(const QHash<int,const char*>* _renamedMethods = renamedMethods(_method.enclosingMetaObject())){
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
            if(method.isValid()){
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                jobject jreturnType = parameterTypeInfos->at(0).javaClass();
                jobject jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
                for(int i=1; i<parameterTypeInfos->size(); ++i){
                    qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
                }
                return qtjambi_from_QMetaMethod(env, jmetaObject, metaObject, method, jreturnType, jparameters);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_findEnumForFlags)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        QMetaEnum enumerator = metaObject->enumerator(enumeratorIndex);
        QStringList keys;
        for (int i=0;i<enumerator.keyCount();++i) {
            keys << QLatin1String(enumerator.key(i));
        }
        for(int e=0; e<metaObject->enumeratorCount(); ++e){
            if(e!=enumeratorIndex){
                QMetaEnum enm = metaObject->enumerator(enumeratorIndex);
                if(!enm.isFlag()){
                    QStringList keys2;
                    for (int i=0;i<enm.keyCount();++i) {
                        keys2 << QLatin1String(enm.key(i));
                    }
                    if(keys==keys2){
                        return qtjambi_from_QMetaEnum(__jni_env, enumerator, enumeratorIndex);
                    }
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_name)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        const char* result = metaObject->enumerator(enumeratorIndex).name();
        return result ? __jni_env->NewStringUTF(result) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_isFlag)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        return metaObject->enumerator(enumeratorIndex).isFlag();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_isScoped)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        return metaObject->enumerator(enumeratorIndex).isScoped();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_keyCount)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        return metaObject->enumerator(enumeratorIndex).keyCount();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_scope)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        const char* result = metaObject->enumerator(enumeratorIndex).scope();
        return result ? __jni_env->NewStringUTF(result) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_key)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex, jint index)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        const char* result = metaObject->enumerator(enumeratorIndex).key(index);
        return result ? __jni_env->NewStringUTF(result) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_value)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex, jint index)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        return metaObject->enumerator(enumeratorIndex).value(index);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_keyToValue)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex, jstring key)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        QString __qt_key = qtjambi_to_qstring(__jni_env, key);
        bool ok = false;
        int result = metaObject->enumerator(enumeratorIndex).keyToValue(qPrintable(__qt_key), &ok);
        return ok ? qtjambi_from_int(__jni_env, result) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_valueToKey)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex, jint value)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        const char* result = metaObject->enumerator(enumeratorIndex).valueToKey(value);
        return result ? __jni_env->NewStringUTF(result) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_keysToValue)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jint enumeratorIndex, jobject keys)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        QStringList __qt_keys;
        for(jobject iterator = qtjambi_collection_iterator(env, keys);
            qtjambi_iterator_has_next(env, iterator);){
            jobject next = qtjambi_iterator_next(env, iterator);
            __qt_keys << qtjambi_to_qstring(env, jstring(next));
        }
        bool ok = false;
        int result = metaObject->enumerator(enumeratorIndex).keysToValue(qPrintable(__qt_keys.join("|")), &ok);
        return ok ? qtjambi_from_int(env, result) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_valueToKeys)
    (JNIEnv * __jni_env, jclass, jlong metaObjectPointer, jint enumeratorIndex, jint value)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        QList<QByteArray> keys = metaObject->enumerator(enumeratorIndex).valueToKeys(value).split('|');
        jobject result = qtjambi_arraylist_new(__jni_env, keys.size());
        for (int i=0; i<keys.size(); ++i) {
            jobject key = qtjambi_from_qstring(__jni_env, keys[i]);
            qtjambi_collection_add(__jni_env, result, key);
        }
        return result;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumerator)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jobject name)
{
    try{
        if(metaObjectPointer){
            QString enumName = qtjambi_to_qstring(env, jstring(name));
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int idx = metaObject->indexOfEnumerator(qPrintable(enumName));
            QMetaEnum enumerator = metaObject->enumerator(idx);
            return qtjambi_from_QMetaEnum(env, enumerator, idx);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_resolveEntry)
    (JNIEnv * env, jclass, jclass enumClass, jint value, jbyte bitSize)
{
    try{
        switch ( bitSize ) {
        case 1:  return qtjambi_from_enum(env, qint8(value), enumClass);
        case 2:  return qtjambi_from_enum(env, qint16(value), enumClass);
        case 8:  return qtjambi_from_enum(env, qint64(value), enumClass);
        default: return qtjambi_from_enum(env, qint32(value), enumClass);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaEnum_flags)
    (JNIEnv * env, jclass, jclass flagsClass, jint value)
{
    try{
        return qtjambi_from_flags(env, value, qPrintable(qtjambi_class_name(env, flagsClass).replace(".", "/")));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumeratorCount)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer)){
            return metaObject->enumeratorCount();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_enumerators)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int count = metaObject->enumeratorCount();
            jobject javaEnums = qtjambi_arraylist_new(env, count);
            for (int i=0; i<count; ++i) {
                QMetaEnum enumerator = metaObject->enumerator(i);
                jobject javaEnum = qtjambi_from_QMetaEnum(env, enumerator, i);
                qtjambi_collection_add(env, javaEnums, javaEnum);
            }
            return javaEnums;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_methodFromMethod)
    (JNIEnv * env, jclass, jobject jmetaObject, jlong metaObjectPointer, jobjectArray ok)
{
    try{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            jobject reflect_method = env->GetObjectArrayElement(ok, 0);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            int index = QtDynamicMetaObject::methodFromJMethod(metaObject, env->FromReflectedMethod(reflect_method));
            QMetaMethod method = metaObject->method(index);
            if(method.isValid()){
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                jobject returnType = parameterTypeInfos->at(0).javaClass();
                jobject parameterTypes = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
                for(int i=1; i<parameterTypeInfos->size(); ++i){
                    qtjambi_collection_add(env, parameterTypes, parameterTypeInfos->at(i).javaClass());
                }
                return qtjambi_from_QMetaMethod(env, jmetaObject, metaObject, metaObject->method(index), returnType, parameterTypes);
            }else{
                env->SetObjectArrayElement(ok, 0, nullptr);
                return nullptr;
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_methodByIndex)
    (JNIEnv * env, jobject jmetaObject, jlong metaObjectPointer, jint index)
{
    try{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->method(index);
            if(method.isValid()){
                jobject jreturnType = nullptr;
                jobject jparameters = nullptr;
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                jreturnType = parameterTypeInfos->at(0).javaClass();
                jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
                for(int i=1; i<parameterTypeInfos->size(); ++i){
                    qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
                }
                return qtjambi_from_QMetaMethod(env, jmetaObject, metaObject, method, jreturnType, jparameters);
            }else{
                JavaException::raiseIndexOutOfBoundsException(env, qPrintable(QString("Index %1 is not a valid method of class %2").arg(index).arg(metaObject->className())) QTJAMBI_STACKTRACEINFO );
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_methods)
    (JNIEnv * env, jobject jmetaObject, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(500);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            const int methodCount = metaObject->methodCount();
            jobject list = qtjambi_arraylist_new(env, methodCount);
            for(int i=0; i<methodCount; ++i){
                QMetaMethod method = metaObject->method(i);
                jobject jreturnType = nullptr;
                jobject jparameters = nullptr;
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                jreturnType = parameterTypeInfos->at(0).javaClass();
                jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
                for(int i=1; i<parameterTypeInfos->size(); ++i){
                    qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
                }
                jobject mobject = qtjambi_from_QMetaMethod(env, jmetaObject, metaObject, method, jreturnType, jparameters);
                if(mobject)
                    qtjambi_collection_add(env, list, mobject);
            }
            return list;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructor)
    (JNIEnv * env, jobject jmetaObject, jlong metaObjectPointer, jobject normalizedSignature)
{
    try{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->constructor(metaObject->indexOfConstructor(qPrintable(qtjambi_to_qstring(env, jstring(normalizedSignature)))));
            if(method.isValid() && method.methodType()==QMetaMethod::Constructor){
                jobject jreturnType = nullptr;
                jobject jparameters = nullptr;
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                jreturnType = parameterTypeInfos->at(0).javaClass();
                jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
                for(int i=1; i<parameterTypeInfos->size(); ++i){
                    qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
                }
                return qtjambi_from_QMetaMethod(env, jmetaObject, metaObject, method, jreturnType, jparameters);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_constructors)
    (JNIEnv * env, jobject jmetaObject, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            env->EnsureLocalCapacity(100);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            jobject list = qtjambi_arraylist_new(env, metaObject->constructorCount());
            for(int i=0; i<metaObject->constructorCount(); ++i){
                QMetaMethod method = metaObject->constructor(i);
                jobject jreturnType = nullptr;
                jobject jparameters = nullptr;
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                jreturnType = parameterTypeInfos->at(0).javaClass();
                jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
                for(int i=1; i<parameterTypeInfos->size(); ++i){
                    qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
                }
                jobject mobject = qtjambi_from_QMetaMethod(env, jmetaObject, metaObject, method, jreturnType, jparameters);
                if(mobject)
                    qtjambi_collection_add(env, list, mobject);
            }
            return list;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_forQObject)
    (JNIEnv * env, jclass, QtJambiNativeID native__id)
{
    try{
        const QMetaObject *metaObject = nullptr;
        if(QSharedPointer<QtJambiLink> objectLink = QtJambiLink::fromNativeId(native__id)){
            if(objectLink->isQObject()){
                if(QObject* obj = objectLink->qobject()){
                    metaObject = obj->metaObject();
                }else if(jobject ref = objectLink->getJavaObjectLocalRef(env)){
                    metaObject = qtjambi_metaobject_for_class(env, env->GetObjectClass(ref), nullptr);
                }
            }
        }
        if(metaObject){
            return qtjambi_from_QMetaObject(env, metaObject);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject__1_1qt_1forType)
    (JNIEnv * env, jclass, jclass type)
{
    try{
        const QMetaObject *metaObject = qtjambi_metaobject_for_class(env, type, nullptr);
        return qtjambi_from_QMetaObject(env, metaObject);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_type)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            return QtDynamicMetaObject::javaClass(env, metaObject);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_exactType)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        if(metaObjectPointer){
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            return QtDynamicMetaObject::javaClass(env, metaObject, true);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_superClass)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject && metaObject->superClass()){
            return qtjambi_from_QMetaObject(env, metaObject->superClass());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_className)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            return qtjambi_from_qstring(env, QLatin1String(metaObject->className()));
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_classInfos)
    (JNIEnv * env, jclass, jlong metaObjectPointer)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            jobject list = qtjambi_arraylist_new(env, metaObject->classInfoCount());
            for (int var = 0; var < metaObject->classInfoCount(); ++var) {
                QString name = QLatin1String(metaObject->classInfo(var).name());
                if(name!="__qt__binding_shell_language"){
                    jstring _name = qtjambi_from_qstring(env, name);
                    jstring value = qtjambi_from_qstring(env, QLatin1String(metaObject->classInfo(var).value()));
                    qtjambi_collection_add(env, list, qtjambi_pair_new(env, _name, value));
                }
            }
            return list;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_classInfo)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jstring name)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            int var = metaObject->indexOfClassInfo(qPrintable(qtjambi_to_qstring(env, name)));
            if(var){
                return qtjambi_from_qstring(env, QLatin1String(metaObject->classInfo(var).value()));
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_inherits)
    (JNIEnv *env, jclass, jlong metaObjectPointer, jlong other)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            return metaObject->inherits(reinterpret_cast<const QMetaObject *>(other));
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_checkConnectArgsString)
    (JNIEnv * env, jclass, jobject signal, jobject method)
{
    try{
        return QMetaObject::checkConnectArgs(qPrintable(qtjambi_to_qstring(env, jstring(signal))), qPrintable(qtjambi_to_qstring(env, jstring(method))));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_checkConnectArgsMethods)
    (JNIEnv *env, jclass, jlong signalMetaObjectPointer, jint signalMethodIndex, jlong methodMetaObjectPointer, jint methodMethodIndex)
{
    try{
        const QMetaObject *signalMetaObject = reinterpret_cast<const QMetaObject *>(signalMetaObjectPointer);
        const QMetaObject *methodMetaObject = reinterpret_cast<const QMetaObject *>(methodMetaObjectPointer);
        return QMetaObject::checkConnectArgs(signalMetaObject->method(signalMethodIndex), methodMetaObject->method(methodMethodIndex));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod__1invoke)
    (JNIEnv * env, jclass, QtJambiNativeID object__id, jlong metaObjectPointer, jint methodIndex, jobjectArray argClassTypeArray, jint connection, jobjectArray args)
{
    jobject result = nullptr;
    bool ok = false;
    try{
        QSharedPointer<QtJambiLink> objectLink = QtJambiLink::fromNativeId(object__id);
        if(objectLink && objectLink->isQObject()){
            QtJambiScope scope(object__id);
            env->EnsureLocalCapacity(500);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->method(methodIndex);
            if(method.isValid()){
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                int argsCount = env->GetArrayLength(args);
                const int parameterCount = method.parameterCount();
                if(parameterCount==env->GetArrayLength(argClassTypeArray)-1
                        && parameterCount==argsCount && argsCount<10){
                    QGenericArgument val[10];
                    QList<QByteArray> parameterTypes = method.parameterTypes();
                    bool failed = false;
                    for(int i=0; i<argsCount; ++i){
                        jvalue jval;
                        jval.l = env->GetObjectArrayElement(args, i);
                        void* qtPtr = nullptr;
                        const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos->at(i+1);
                        ConvertResponse resp = parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l);
                        switch(resp){
                        case ConvertResponse::NotSuccessful:
                            failed = true;
                            break;
                        case ConvertResponse::JavaObjectNotLinked:
                            failed = true;
                            break;
                        default:
                            break;
                        }
                        if(failed)
                            break;
                        val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
                    }
                    if(failed){
                        ok = false;
                    }else{
                        void* resultPtr = method.returnType()==QMetaType::Void ? nullptr : QMetaType::create(method.returnType());
                        ok = method.invoke(objectLink->qobject(),
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
                        if(resultPtr){
                            if(ok){
                                jvalue jval;
                                jval.l = nullptr;
                                if(parameterTypeInfos->at(0).convertInternalToExternal(env, &scope, resultPtr, &jval, true)){
                                    result = jval.l;
                                }else{
                                    ok = false;
                                }
                            }
                            QMetaType::destroy(method.returnType(), resultPtr);
                        }
                    }
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    if(!ok){
        return Java::Private::QtCore::QMetaMethod.UNSUPPORTED(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod__1invokeOnGadget)
    (JNIEnv * env, jclass, QtJambiNativeID object_id, jobject object, jlong metaObjectPointer, jint methodIndex, jobjectArray argClassTypeArray, jobjectArray args)
{
    bool ok = false;
    jobject result = nullptr;
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        void* ptr = nullptr;
        if(!!object_id){
            const std::type_info* typeId = nullptr;
            do{
                typeId = getTypeByQtName(metaObject->className());
                if(typeId)
                    break;
                metaObject = metaObject->superClass();
            }while(metaObject);
            if(!typeId){
                ptr = reinterpret_cast<QtJambiLink *>(object_id)->typedPointer(*typeId);
            }else{
                typeId = &typeid(JObjectWrapper);
                ptr = reinterpret_cast<QtJambiLink *>(object_id)->pointer();
            }
            qtjambi_check_resource(env, ptr, *typeId);
        }else{
            ptr = object;
        }
        if(ptr){
            QtJambiScope scope(nullptr);
            env->EnsureLocalCapacity(500);
            const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
            QMetaMethod method = metaObject->method(methodIndex);
            if(method.isValid()){
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                int argsCount = env->GetArrayLength(args);
                const int parameterCount = method.parameterCount();
                if(parameterCount==env->GetArrayLength(argClassTypeArray)-1
                        && parameterCount==argsCount && argsCount<10){
                    QGenericArgument val[10];
                    QList<QByteArray> parameterTypes = method.parameterTypes();
                    bool failed = false;
                    for(int i=0; i<argsCount; ++i){
                        jvalue jval;
                        jval.l = env->GetObjectArrayElement(args, i);
                        void* qtPtr = nullptr;
                        const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos->at(i+1);
                        ConvertResponse resp = parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l);
                        switch(resp){
                        case ConvertResponse::NotSuccessful:
                            failed = true;
                            break;
                        case ConvertResponse::JavaObjectNotLinked:
                            failed = true;
                            break;
                        default:
                            break;
                        }
                        if(failed)
                            break;
                        val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
                    }
                    if(failed){
                        ok = false;
                    }else{
                        void* resultPtr = method.returnType()==QMetaType::Void ? nullptr : QMetaType::create(method.returnType());
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
                        if(resultPtr){
                            if(ok){
                                jvalue jval;
                                jval.l = nullptr;
                                if(parameterTypeInfos->at(0).convertInternalToExternal(env, &scope, resultPtr, &jval, true)){
                                    result = jval.l;
                                }else{
                                    ok = false;
                                }
                            }
                            QMetaType::destroy(method.returnType(), resultPtr);
                        }
                    }
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    if(!ok){
        return Java::Private::QtCore::QMetaMethod.UNSUPPORTED(env);
    }
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaObject_newInstanceImpl)
    (JNIEnv * env, jclass, jlong metaObjectPointer, jint methodIndex, jobject argTypes, jobjectArray args)
{
    try{
        const QMetaObject *metaObject = reinterpret_cast<const QMetaObject *>(metaObjectPointer);
        if(metaObject){
            QtJambiScope scope(nullptr);
            env->EnsureLocalCapacity(500);
            QMetaMethod method = metaObject->method(methodIndex);
            if(!method.isValid()){
                for(int i=0; i<metaObject->constructorCount(); ++i){
                    QMetaMethod constr = metaObject->constructor(i);
                    if(constr.methodIndex()==methodIndex){
                        method = constr;
                        break;
                    }
                }
            }
            if(method.isValid()){
                const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                Q_ASSERT(parameterTypeInfos);
                jobjectArray argTypesArray = jobjectArray(argTypes);
                int argsCount = env->GetArrayLength(args);
                const int parameterCount = method.parameterCount();
                if(parameterCount==env->GetArrayLength(argTypesArray) && parameterCount==argsCount && argsCount<10){
                    QGenericArgument val[10];
                    QList<QByteArray> parameterTypes = method.parameterTypes();
                    bool failed = false;
                    for(int i=0; i<argsCount; ++i){
                        jvalue jval;
                        jval.l = env->GetObjectArrayElement(args, i);
                        void* qtPtr = nullptr;
                        const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos->at(i+1);
                        switch(parameterTypeInfo.convertExternalToInternal(env, &scope, jval, qtPtr, jValueType::l)){
                        case ConvertResponse::NotSuccessful:
                        case ConvertResponse::JavaObjectNotLinked:
                            failed = true;
                            break;
                        default:
                            break;
                        }
                        if(failed)
                            break;
                        val[i] = QGenericArgument(parameterTypes[i].constData(), qtPtr);
                    }
                    QObject* obj = nullptr;
                    if(!failed){
                        obj = metaObject->newInstance(
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
                    }
                    if(obj){
                        if(metaObject->inherits(&QObject::staticMetaObject)){
                            return qtjambi_from_QObject(env, obj);
                        }else{
                            if(QtDynamicMetaObject::isDynamic(metaObject)){
                                const QtDynamicMetaObject* dynamicMetaObject = reinterpret_cast<const QtDynamicMetaObject*>(metaObject);
                                if(Java::Private::QtJambi::QtObjectInterface.isAssignableFrom(env, dynamicMetaObject->javaClass())){
                                    return qtjambi_from_object(env, obj, dynamicMetaObject->javaClass(), false, false);
                                }else{
                                    return reinterpret_cast<jobject>(obj);
                                }
                            }else if(const std::type_info* typeId = getTypeByQtName(metaObject->className())){
                                return qtjambi_from_object(env, obj, *typeId, false, false);
                            }
                        }
                    }
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

class ConnectionAccess{
public: void *d_ptr;
};

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024NativeConnection_isConnected)
    (JNIEnv *env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = qtjambi_object_from_nativeId<QMetaObject::Connection>(nativeId);
        if(*connection)
            return true;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024NativeConnection_equals)
    (JNIEnv *env, jclass, QtJambiNativeID nativeId, QtJambiNativeID otherNativeId)
{
    try{
        QMetaObject::Connection* connection = qtjambi_object_from_nativeId<QMetaObject::Connection>(nativeId);
        QMetaObject::Connection* otherConnection = qtjambi_object_from_nativeId<QMetaObject::Connection>(otherNativeId);
        if(connection && otherConnection){
            void * ptr1 = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            void * ptr2 = reinterpret_cast<ConnectionAccess*>(otherConnection)->d_ptr;
            QObjectPrivate::Connection* _ptr1 = reinterpret_cast<QObjectPrivate::Connection*>(ptr1);
            QObjectPrivate::Connection* _ptr2 = reinterpret_cast<QObjectPrivate::Connection*>(ptr2);
            return _ptr1==_ptr2;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

QObject* connectionSender(const QMetaObject::Connection* connection){
    if(connection && *connection){
        void * ptr = reinterpret_cast<const ConnectionAccess*>(connection)->d_ptr;
        return reinterpret_cast<QObjectPrivate::Connection*>(ptr)->sender;
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024NativeConnection_sender)
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = qtjambi_object_from_nativeId<QMetaObject::Connection>(nativeId);
        return qtjambi_from_QObject(env, connectionSender(connection));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024NativeConnection_receiver)
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = qtjambi_object_from_nativeId<QMetaObject::Connection>(nativeId);
        if(*connection){
            void * ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            QObjectPrivate::Connection* _ptr = reinterpret_cast<QObjectPrivate::Connection*>(ptr);
            QObject* receiver = _ptr->receiver;
            return qtjambi_from_QObject(env, receiver);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024NativeConnection_signal)
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = qtjambi_object_from_nativeId<QMetaObject::Connection>(nativeId);
        if(*connection){
            env->EnsureLocalCapacity(100);
            void * ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            QObject* sender = reinterpret_cast<QObjectPrivate::Connection*>(ptr)->sender;
            QMetaMethod signal = QMetaObjectPrivate::signal(sender->metaObject(), reinterpret_cast<QObjectPrivate::Connection*>(ptr)->signal_index);
            jobject jreturnType = nullptr;
            jobject jparameters = nullptr;
            const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, signal);
            Q_ASSERT(parameterTypeInfos);
            jreturnType = parameterTypeInfos->at(0).javaClass();
            jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
            for(int i=1; i<parameterTypeInfos->size(); ++i){
                qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
            }
            return qtjambi_from_QMetaMethod(env, nullptr, sender->metaObject(), signal, jreturnType, jparameters);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024NativeConnection_method)
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = qtjambi_object_from_nativeId<QMetaObject::Connection>(nativeId);
        if(*connection){
            env->EnsureLocalCapacity(100);
            void * ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            QObjectPrivate::Connection* cnn = reinterpret_cast<QObjectPrivate::Connection*>(ptr);
            if(!cnn->isSlotObject){
                QObject* receiver = cnn->receiver;
                QMetaMethod method = receiver->metaObject()->method(reinterpret_cast<QObjectPrivate::Connection*>(ptr)->method());
                if(method.isValid()){
                    jobject jreturnType = nullptr;
                    jobject jparameters = nullptr;
                    const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                    Q_ASSERT(parameterTypeInfos);
                    jreturnType = parameterTypeInfos->at(0).javaClass();
                    jparameters = qtjambi_arraylist_new(env, parameterTypeInfos->size()-1);
                    for(int i=1; i<parameterTypeInfos->size(); ++i){
                        qtjambi_collection_add(env, jparameters, parameterTypeInfos->at(i).javaClass());
                    }
                    return qtjambi_from_QMetaMethod(env, nullptr, receiver->metaObject(), method, jreturnType, jparameters);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024NativeConnection_isSlotObject)
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = qtjambi_object_from_nativeId<QMetaObject::Connection>(nativeId);
        if(*connection){
            void * ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            QObjectPrivate::Connection* _ptr = reinterpret_cast<QObjectPrivate::Connection*>(ptr);
            return _ptr->isSlotObject;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_emitNativeSignal)
(JNIEnv *env,
 jclass,
 QtJambiNativeID senderNativeId,
 jint methodIndex,
 jobjectArray args)
{
    QTJAMBI_DEBUG_METHOD_PRINT("java", "QtJambiSignals::emitNativeSignal(...)")
    JniLocalFrame __jniLocalFrame(env, 1000);
    Q_UNUSED(__jniLocalFrame)
    try{
        QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(senderNativeId);
        if (link && link->isQObject()) {
            if(QObject *o = link->qobject()){
                QMetaMethod method = o->metaObject()->method(methodIndex);
                if(method.isValid()){
                    QtJambiScope scope(senderNativeId);
                    const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, method);
                    Q_ASSERT(parameterTypeInfos);
                    QVector<void *> convertedArguments;
                    bool failed = false;
                    int size = env->GetArrayLength(args);
                    convertedArguments.fill(nullptr, size+1);
                    for (int i = 0; i < size; ++i) {
                        const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos->at(i+1);
                        jvalue jv;
                        jv.l = env->GetObjectArrayElement(args, i);
                        switch(parameterTypeInfo.convertExternalToInternal(env, &scope, jv, convertedArguments[i+1], jValueType::l)){
                        case ConvertResponse::NotSuccessful:
                        case ConvertResponse::JavaObjectNotLinked:
                            failed = true;
                            break;
                        default:
                            break;
                        }
                        if(failed)
                            break;
                    }
                    if (!failed) {
                        Q_ASSERT(method.isValid());
                        int signalIndex = QMetaObjectPrivate::signalIndex(method);
                        Q_ASSERT(signalIndex>=0);
                        if(signalIndex>=0)
                            QMetaObject::activate(o, 0, signalIndex, convertedArguments.data());
                    }
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMetaMethod_methodByIndex)
    (JNIEnv * env, jclass, jobject _metaObject, jint methodIndex)
{
    if(const QMetaObject* metaObject = qtjambi_cast<const QMetaObject*>(env, _metaObject)){
        try{
            return qtjambi_cast<jobject>(env, metaObject->method(methodIndex));
        }catch(const JavaException& exn){
            exn.raiseInJava(env);
        }
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_indexOfMethod)
    (JNIEnv * env, jclass, jobject method)
{
    if(method){
        try{
            return Java::Private::QtCore::QMetaMethod.methodIndex(env, method);
        }catch(const JavaException& exn){
            exn.raiseInJava(env);
        }
    }
    return -1;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_isQtPatched)
    (JNIEnv *, jclass)
{
#ifdef QT_QTJAMBI_PORT
    return true;
#else
    return false;
#endif
}

class NativeSlotObject : public QtPrivate::QSlotObjectBase
{
public:
    NativeSlotObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link, const QMetaMethod& signal, jobject connection, jint argumentCount, jint connectionType, bool nothrow, QObject* receiver = nullptr);
    ~NativeSlotObject();
    int* types();
private:
    static void impl(int which, QSlotObjectBase *this_, QObject *r, void **a, bool *ret);
    QMetaMethod m_signal;
    QWeakPointer<QtJambiLink> m_link;
    JObjectWrapper m_connection;
    int m_argumentCount;
    int* const m_types;
    QObject* m_receiver;
    bool m_nothrow;
};

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_connectNative)
    (JNIEnv * env, jclass, QtJambiNativeID senderObjectId, jint signal, jobject connection, jint argumentCount, jint connectionType)
{
    try{
        QSharedPointer<QtJambiLink> senderLink = QtJambiLink::fromNativeId(senderObjectId);
        NativeSlotObject *slotObj;
        QObject* sender = senderLink->qobject();
        QMetaMethod qt_signalMethod = sender->metaObject()->method(signal);
        QObject* receiver = sender;
        connectionType = connectionType & ~Qt::UniqueConnection;
        if(connectionType==Qt::AutoConnection || connectionType==Qt::DirectConnection){
            connectionType = Qt::DirectConnection;
            receiver = new QObject();
            receiver->moveToThread(nullptr);
        }else if(qt_signalMethod.enclosingMetaObject()==&QThread::staticMetaObject && QThreadData::get2(static_cast<QThread*>(sender))->isAdopted){
            QString _connectionType;
            if(connectionType==Qt::BlockingQueuedConnection){
                _connectionType = "BlockingQueuedConnection";
            }else{
                _connectionType = "QueuedConnection";
            }
            JavaException::raiseIllegalStateException(env, qPrintable(QString("Cannot use signal QThread::%1 from adopted thread with connection type %2.").arg(qt_signalMethod.methodSignature().data()).arg(_connectionType)) QTJAMBI_STACKTRACEINFO);
            return nullptr;
        }
        int signalIndex = QMetaObjectPrivate::signalIndex(qt_signalMethod);
        slotObj = new NativeSlotObject(env, senderLink, qt_signalMethod, connection, argumentCount, connectionType, signalIndex==0, receiver == sender ? nullptr : receiver);
        QMetaObject::Connection c = QObjectPrivate::connectImpl(sender,
                                                                signalIndex,
                                                                receiver,
                                                                nullptr,
                                                                slotObj,
                                                                Qt::ConnectionType(connectionType),
                                                                slotObj->types(),
                                                                qt_signalMethod.enclosingMetaObject());
        return c ? qtjambi_cast<jobject>(env, c) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_connectNativeToMetaMethod)
    (JNIEnv * env, jclass, QtJambiNativeID senderObjectId, jint signal, QtJambiNativeID receiverObjectId, jint slot, jint connectionType)
{
    try{
        QSharedPointer<QtJambiLink> senderLink = QtJambiLink::fromNativeId(senderObjectId);
        QSharedPointer<QtJambiLink> receiverLink = QtJambiLink::fromNativeId(receiverObjectId);
        QObject* sender = senderLink ? senderLink->qobject() : nullptr;
        QMetaMethod signalMethod;
        if(signal)
            signalMethod = sender->metaObject()->method(signal);
        QObject* receiver = receiverLink ? receiverLink->qobject() : nullptr;
        QMetaMethod slotMethod;
        if(receiver)
            slotMethod = receiver->metaObject()->method(slot);
        if(!QMetaObject::checkConnectArgs(signalMethod, slotMethod)){
            QString message("Signal and slot signatures misfit: %1 != %2");
            message = message.arg(QLatin1String(signalMethod.methodSignature())).arg(QLatin1String(slotMethod.methodSignature()));
            jthrowable t = Java::Private::QtJambi::QMisfittingSignatureException.newInstance(env, qtjambi_from_qstring(env, message));
            env->Throw(t);
            return nullptr;
        }
        QMetaObject::Connection c = QObject::connect(sender, signalMethod, receiver, slotMethod, Qt::ConnectionType(connectionType));
        if(c){
            return qtjambi_cast<jobject>(env, c);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_disconnectNative)
    (JNIEnv *env, jclass, QtJambiNativeID senderObjectId, jint signal, QtJambiNativeID receiverObjectId, jint slot)
{
    try{
        QSharedPointer<QtJambiLink> senderLink = QtJambiLink::fromNativeId(senderObjectId);
        QSharedPointer<QtJambiLink> receiverLink = QtJambiLink::fromNativeId(receiverObjectId);
        QObject* sender = senderLink ? senderLink->qobject() : nullptr;
        QMetaMethod signalMethod;
        if(signal)
            signalMethod = sender->metaObject()->method(signal);
        QObject* receiver = receiverLink ? receiverLink->qobject() : nullptr;
        QMetaMethod slotMethod;
        if(receiver)
            slotMethod = receiver->metaObject()->method(slot);
        return QObject::disconnect(sender, signalMethod, receiver, slotMethod);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

NativeSlotObject::NativeSlotObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link, const QMetaMethod& signal, jobject connection, jint argumentCount, jint connectionType, bool nothrow, QObject* receiver)
    : QSlotObjectBase(&NativeSlotObject::impl),
      m_signal(signal),
      m_link(link),
      m_connection(env, connection),
      m_argumentCount(argumentCount),
      m_types(
          [env,signal,connectionType]()->int*{
          if (connectionType == Qt::QueuedConnection || connectionType == Qt::BlockingQueuedConnection){
              const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, signal);
              Q_ASSERT(parameterTypeInfos);
              if(parameterTypeInfos && parameterTypeInfos->size()>1){
                  int *types = new int[size_t(parameterTypeInfos->size())];
                  for (int i = 0; i < parameterTypeInfos->size(); ++i) {
                      if(i == parameterTypeInfos->size()-1){
                          types[i] = 0;
                      }else{
                          types[i] = (*parameterTypeInfos)[i+1].metaType();
                      }
                  }
                  return types;
              }
            }
            return nullptr;
          }()
      ),
      m_receiver(receiver),
      m_nothrow(nothrow)
{
}

NativeSlotObject::~NativeSlotObject()
{
    if(m_types)
        delete[] m_types;
    if(m_receiver)
        delete m_receiver;
}

int* NativeSlotObject::types() { return m_types; }

void NativeSlotObject::impl(int which, QSlotObjectBase *this_, QObject *, void **a, bool *ret){
    switch (which) {
    case Destroy:
        delete static_cast<NativeSlotObject*>(this_);
        break;
    case Call:{
            NativeSlotObject* _this = static_cast<NativeSlotObject*>(this_);
            if(JNIEnv* env = qtjambi_current_environment()){
                try{
                    QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                    if(QSharedPointer<QtJambiLink> link = _this->m_link.toStrongRef()){
                        QtJambiLinkScope scope(link);
                        if(link->isQObject()){
                            const QVector<ParameterTypeInfo>* parameterTypeInfos = QtDynamicMetaObject::methodParameterInfo(env, _this->m_signal);
                            Q_ASSERT(parameterTypeInfos);
                            if(parameterTypeInfos->size()){
                                QVector<jvalue> converted_arguments(_this->m_argumentCount<0 ? parameterTypeInfos->size()-1 : qMin(_this->m_argumentCount, parameterTypeInfos->size()-1));

                                bool success = true;
                                for (int i = 0; i < converted_arguments.size(); ++i) {
                                    const ParameterTypeInfo& parameterTypeInfo = (*parameterTypeInfos)[i+1];
                                    converted_arguments[i].l = nullptr;
                                    if(!parameterTypeInfo.convertInternalToExternal(env, &scope, a[i+1], &converted_arguments[i], true)){
                                        success = false;
                                        break;
                                    }
                                }
                                if (success) {
                                    jobjectArray args = Java::Private::Runtime::Object.newArray(env, converted_arguments.size());
                                    for (int i=0; i<converted_arguments.size();++i) {
                                        env->SetObjectArrayElement(args, i, converted_arguments[i].l);
                                        qtjambi_throw_java_exception(env)
                                    }
                                    Java::Private::QtJambi::QtJambiSignals$AbstractConnection.invoke(env,_this->m_connection.object(), args);
                                } else {
                                    qWarning("SlotObject::CallSignal: Failed to convert arguments");
                                }
                            }else{
                                qWarning()<< "SlotObject::CallSignal: Failed to convert method types for signal " << _this->m_signal.methodSignature();
                            }
                        }
                    }
                } catch (const JavaException& exn) {
                    if(_this->m_nothrow){
                        exn.report(env);
                    }else{
                        throw;
                    }
                }
            }
        }
        break;
    case Compare:
        *ret = false;
        break;
    case NumOperations: ;
    }
}
