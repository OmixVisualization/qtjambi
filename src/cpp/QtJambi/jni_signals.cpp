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

#include <QtCore/qcompilerdetection.h>

#if QT_VERSION >= QT_VERSION_CHECK(6,4,0)
#include <QtCore/private/qobject_p_p.h>
#endif

QT_WARNING_DISABLE_DEPRECATED
#include "qtjambiapi.h"
#include "java_p.h"
#include "threadutils_p.h"
#include "registryapi.h"
#include "registryutil_p.h"
#include "qtjambilink_p.h"
#include "typemanager_p.h"
#include "qtjambimetaobject_p.h"
#include "coreapi.h"

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
#include "qtjambi_cast.h"

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define qAsConst std::as_const
#endif

class ConnectionAccess{
public: QObjectPrivate::Connection *d_ptr;
};

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_isConnected
    (JNIEnv *env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        return *connection ? true : false;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_equals
    (JNIEnv *env, jclass, QtJambiNativeID nativeId, QtJambiNativeID otherNativeId)
{
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        QMetaObject::Connection* otherConnection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(otherNativeId);
        if(connection && otherConnection){
            QObjectPrivate::Connection* ptr1 = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            QObjectPrivate::Connection* ptr2 = reinterpret_cast<ConnectionAccess*>(otherConnection)->d_ptr;
            return ptr1==ptr2;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

QObject* connectionSender(const QMetaObject::Connection* connection){
    if(connection && *connection){
        QObjectPrivate::Connection* ptr = reinterpret_cast<const ConnectionAccess*>(connection)->d_ptr;
        return ptr->sender;
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_sender
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        return QtJambiAPI::convertQObjectToJavaObject(env, connectionSender(connection));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_receiver
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        if(*connection){
            QObjectPrivate::Connection* ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            QObject* receiver = ptr->receiver;
            return QtJambiAPI::convertQObjectToJavaObject(env, receiver);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_signal
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaMethod signal;
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        if(*connection){
            env->EnsureLocalCapacity(100);
            QObjectPrivate::Connection* ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            QObject* sender = ptr->sender;
            signal = QMetaObjectPrivate::signal(sender->metaObject(), ptr->signal_index);
        }
        return qtjambi_cast<jobject>(env, signal);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_method
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        QMetaMethod method;
        if(*connection){
            env->EnsureLocalCapacity(100);
            QObjectPrivate::Connection* ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            if(!ptr->isSlotObject){
                QObject* receiver = ptr->receiver;
                method = receiver->metaObject()->method(ptr->method());
            }
        }
        return qtjambi_cast<jobject>(env, method);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_isSlotObject
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        if(*connection){
            QObjectPrivate::Connection* ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
            return ptr->isSlotObject;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_clone
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        return qtjambi_cast<jobject>(env, QMetaObject::Connection(*connection));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_hashCode
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    jint __java_return_value{0};
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        QObjectPrivate::Connection* ptr = reinterpret_cast<ConnectionAccess*>(connection)->d_ptr;
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        hash_type hashValue = qHash(ptr);
        __java_return_value = jint(quint64(hashValue) ^ quint64(hashValue) >> 32);
#else
        __java_return_value = qHash(ptr);
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return __java_return_value;
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_SignalUtility_00024NativeConnection_toString
    (JNIEnv * env, jclass, QtJambiNativeID nativeId)
{
    jstring __java_return_value{nullptr};
    try{
        QMetaObject::Connection* connection = QtJambiAPI::objectFromNativeId<QMetaObject::Connection>(nativeId);
        QString res;
        {
            QDebug dbg(&res);
            QObjectPrivate::Connection* ptr = reinterpret_cast<const ConnectionAccess*>(connection)->d_ptr;
            dbg.noquote().nospace() << "QMetaObject::Connection(" << ptr << ")";
        }
        if(!res.isEmpty() && res.back().isSpace())
            res = res.trimmed();
        __java_return_value = qtjambi_cast<jstring>(env, res);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return __java_return_value;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_SignalUtility_00024AbstractConnection_invokeOnLock
(JNIEnv * env, jobject connection, QtJambiNativeID s, QtJambiNativeID r, jobjectArray args)
{
    try{
        QObject *the_sender = QtJambiAPI::objectFromNativeId<QObject>(s);
        QObject *the_receiver = QtJambiAPI::objectFromNativeId<QObject>(r);
        Q_ASSERT(the_receiver);
        // we need to use meta call because that's the only way to get signal lock which is QtCore-internal.
        struct Call : QAbstractMetaCallEvent{
            JNIEnv * env;
            jobject connection;
            jobjectArray args;
            Call(const QObject *sender, JNIEnv * _env, jobject _connection, jobjectArray _args)
                : QAbstractMetaCallEvent(sender, -1),
                env(_env),
                connection(_connection),
                args(_args)
            {}
            void placeMetaCall(QObject *){
                try{
                    Java::QtJambi::SignalUtility$AbstractConnection::invoke(env, connection, args);
                }catch(const JavaException& exn){
                    exn.raiseInJava(env);
                }
            }
        } call(the_sender, env, connection, args);
        the_receiver->QObject::event(&call);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_SignalUtility_disconnectConnection
    (JNIEnv *env, jclass, QtJambiNativeID connection__id)
{
    bool result = false;
    try{
        QSharedPointer<QtJambiLink> connectionLink = QtJambiLink::fromNativeId(connection__id);
        if(connectionLink && !connectionLink->isQObject()){
            if(QMetaObject::Connection* connection = static_cast<QMetaObject::Connection*>(connectionLink->pointer())){
                if(*connection)
                    result = QObject::disconnect(*connection);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return result;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_SignalUtility_initializeMultiSignal
(JNIEnv *env, jclass, jobject multiSignal, jclass declaringClass, jobject reflectedField)
{
    try{
        const QMetaObject* mo = CoreAPI::metaObjectForClass(env, declaringClass);
        QVector<QtJambiMetaObject::SignalInfo> signalInfos = QtJambiMetaObject::signalInfos(env, mo, env->FromReflectedField(reflectedField));
        if(!signalInfos.isEmpty()){
            jintArray methodIndexes = env->NewIntArray(jsize(signalInfos.size()));
            jlongArray metaObjects = env->NewLongArray(jsize(signalInfos.size()));
            jobjectArray signalParameterTypes = env->NewObjectArray(jsize(signalInfos.size()), Java::Runtime::List::getClass(env), nullptr);
            jobjectArray signalObjectTypes = env->NewObjectArray(jsize(signalInfos.size()), Java::Runtime::Class::getClass(env), nullptr);
            {
                JIntArrayPointer methodIndexesPtr(env, methodIndexes);
                JLongArrayPointer metaObjectsPtr(env, metaObjects);
                for(int i=0; i<signalInfos.size(); i++){
                    const QtJambiMetaObject::SignalInfo& info = signalInfos.at(i);
                    env->SetObjectArrayElement(signalObjectTypes, jsize(i), info.signalClass);
                    env->SetObjectArrayElement(signalParameterTypes, jsize(i), info.signalTypes.object(env));
                    methodIndexesPtr.pointer()[i] = info.methodIndex;
                    metaObjectsPtr.pointer()[i] = jlong(mo);
                }
            }
            Java::QtJambi::SignalUtility$AbstractMultiSignal::initializeSignals(env, multiSignal, reflectedField, methodIndexes, metaObjects, signalParameterTypes, signalObjectTypes);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" JNIEXPORT jlong JNICALL Java_io_qt_internal_SignalUtility_fromReflectedMethod(JNIEnv *env, jclass, jobject method){
    return jlong(env->FromReflectedMethod(method));
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_signalInfo
    (JNIEnv *env, jclass, jobject containingObject, jobject _metaObject, jobject field, jobject emitMethod)
{
    try{
        const QMetaObject* mo = qtjambi_cast<const QMetaObject*>(env, _metaObject);
        QtJambiMetaObject::SignalInfo result;
        if(emitMethod){
            result = QtJambiMetaObject::signalInfo(env, mo, env->FromReflectedField(field), env->FromReflectedMethod(emitMethod));
        }else{
            QVector<QtJambiMetaObject::SignalInfo> signalInfos = QtJambiMetaObject::signalInfos(env, mo, env->FromReflectedField(field));
            for(const QtJambiMetaObject::SignalInfo& signalInfo : signalInfos){
                // in case of default-arg signals take the smallest methodIndex. all subsequent methodIndexes are clones.
                if(result.methodIndex==-1 || result.methodIndex>signalInfo.methodIndex){
                    result = signalInfo;
                }
            }
        }
        if(result.methodIndex==-1 && !result.signalTypes && Java::QtCore::QObject::isInstanceOf(env, containingObject)){
            jclass declaringClass = Java::Runtime::Field::getDeclaringClass(env, field);
            if(const std::type_info* typeId = getTypeByJavaName(QtJambiAPI::getClassName(env, declaringClass).replace('.', '/'))){
                if(hasCustomMetaObject(*typeId)){
                    if(QObject* object = qtjambi_cast<QObject*>(env, containingObject)){
                        QByteArray signalName = qtjambi_cast<QString>(env, Java::Runtime::Field::getName(env, field)).toUtf8();
                        const QMetaObject* containingObjectClass = object->metaObject();
                        QMap<QByteArray,QMetaMethod> matchingSignals;
                        for(int i=containingObjectClass->methodOffset(), length = containingObjectClass->methodCount(); i<length; ++i){
                            QMetaMethod method = containingObjectClass->method(i);
                            if(method.methodType()==QMetaMethod::Signal
                                    && signalName==method.name()
                                    && !matchingSignals.contains(method.methodSignature())){
                                matchingSignals[method.methodSignature()] = method;
                            }
                        }
                        if(matchingSignals.size()==1){
                            result.metaObject = matchingSignals.first().enclosingMetaObject();
                            result.methodIndex = matchingSignals.first().methodIndex();
                        }
                    }
                }
            }
        }
        return Java::QtJambi::SignalUtility$SignalInfo::newInstance(env, jlong(result.metaObject), result.methodIndex, result.signalTypes.object(env));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return nullptr;
    }
}


extern "C" JNIEXPORT jlong JNICALL Java_io_qt_internal_SignalUtility_metaObjectId
(JNIEnv *env, jclass, jobject mo){
    try{
        return mo ? Java::QtCore::QMetaObject::metaObjectPointer(env, mo) : 0;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return 0;
    }
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_SignalUtility_isDynamic
    (JNIEnv *env, jclass, jobject mo){
    try{
        jlong metaObjectId = mo ? Java::QtCore::QMetaObject::metaObjectPointer(env, mo) : 0;
        return QtJambiMetaObject::isInstance(reinterpret_cast<const QMetaObject*>(metaObjectId));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return false;
    }
}

extern "C" JNIEXPORT bool JNICALL Java_io_qt_internal_SignalUtility_isSignalConnected
    (JNIEnv *env,
     jclass,
     QtJambiNativeID senderId,
     jint methodIndex,
     jlong senderMetaObjectId)
{
    QTJAMBI_NATIVE_METHOD_CALL("SignalUtility::isSignalConnected(...)")
    JniLocalFrame __jniLocalFrame(env, 1024);
    try{
        QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(senderId);
        Q_ASSERT(link && link->isQObject());
        const QMetaObject* metaObject = reinterpret_cast<const QMetaObject*>(senderMetaObjectId);
        QMetaMethod method = metaObject->method(methodIndex);
        if(method.isValid()){
            if (QObject *o = link->qobject()) {
                struct SignalAccess : QObject{
                    bool isSignalConnected(const QMetaMethod &signal) const{
                        return QObject::isSignalConnected(signal);
                    }
                };
                return reinterpret_cast<const SignalAccess*>(o)->isSignalConnected(method);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT int JNICALL Java_io_qt_internal_SignalUtility_signalReceivers
    (JNIEnv *env,
     jclass,
     QtJambiNativeID senderId,
     jint methodIndex,
     jlong senderMetaObjectId)
{
    QTJAMBI_NATIVE_METHOD_CALL("SignalUtility::receivers(...)")
    JniLocalFrame __jniLocalFrame(env, 1024);
    try{
        QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(senderId);
        Q_ASSERT(link && link->isQObject());
        const QMetaObject* metaObject = reinterpret_cast<const QMetaObject*>(senderMetaObjectId);
        QMetaMethod method = metaObject->method(methodIndex);
        if(method.isValid()){
            if (QObject *o = link->qobject()) {
                struct SignalAccess : QObject{
                    int receivers(const QMetaMethod &signal) const{
                        return QObject::receivers("2"+signal.methodSignature());
                    }
                };
                return reinterpret_cast<const SignalAccess*>(o)->receivers(method);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

bool checkThreadOnSignalEmit(QObject* object);

Q_LOGGING_CATEGORY(internalSignalCategory, "io.qtjambi.internal.signal")

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gSignalEmitThreadCheckHandlerLock, (QReadWriteLock::Recursive));
Q_GLOBAL_STATIC(JObjectWrapper, gSignalEmitThreadCheckHandler)

void installSignalEmitThreadCheckHandler(JNIEnv *env, jobject handler){
    QWriteLocker locker(gSignalEmitThreadCheckHandlerLock());
    if(handler)
        *gSignalEmitThreadCheckHandler = JObjectWrapper(env, handler);
    else
        gSignalEmitThreadCheckHandler->clear(env);
}

void clearInstallSignalEmitThreadCheckHandler(JNIEnv *env){
    if(!gSignalEmitThreadCheckHandler.isDestroyed() && env){
        QWriteLocker locker(gSignalEmitThreadCheckHandlerLock());
        gSignalEmitThreadCheckHandler->clear(env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_SignalUtility_emitNativeSignal
    (JNIEnv *env,
     jclass,
     jobject signalObject,
     QtJambiNativeID senderId,
     jint methodIndex,
     jlong senderMetaObjectId,
     jint defaults,
     jobjectArray args)
{
    QTJAMBI_NATIVE_METHOD_CALL("SignalUtility::emitNativeSignal(...)")
    JniLocalFrame __jniLocalFrame(env, 1024);
    try{
        QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(senderId);
        Q_ASSERT(link && link->isQObject());
        if (QObject *o = link->qobject()) {
            QtJambiLinkScope scope(link);
            const QMetaObject* metaObject = reinterpret_cast<const QMetaObject*>(senderMetaObjectId);
            QMetaMethod method = metaObject->method(methodIndex + defaults);
            if(method.isValid()){
                const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, method);
                QVector<void *> convertedArguments;
                bool failed = false;
                int size = args ? env->GetArrayLength(args) : 0;
                convertedArguments.fill(nullptr, size+1);
                for (int i = 0; i < size; ++i) {
                    const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                    jvalue jv;
                    jv.l = env->GetObjectArrayElement(args, i);
                    if(!parameterTypeInfo.convertExternalToInternal(env, &scope, jv, convertedArguments[i+1], jValueType::l)){
                        failed = true;
                        break;
                    }
                }
                if (!failed) {
                    if(!checkThreadOnSignalEmit(o)){
                        jobject signalEmitThreadCheckHandler;
                        {
                            QReadLocker locker(gSignalEmitThreadCheckHandlerLock());
                            signalEmitThreadCheckHandler = gSignalEmitThreadCheckHandler->object(env);
                        }
                        if(signalEmitThreadCheckHandler){
                            jobject qobject = link->getJavaObjectLocalRef(env);
                            if(!signalObject)
                                signalObject = Java::QtCore::QMetaMethod::toSignal(env, qtjambi_cast<jobject>(env, method), qobject);
                            Java::Runtime::BiConsumer::accept(env, signalEmitThreadCheckHandler, qobject, signalObject);
                        }else{
                            qCWarning(internalSignalCategory).noquote().nospace() << "Emitting signal "
                                                                                  << metaObject->className() << "::" << method.methodSignature() << " in thread " << QThread::currentThread()
                                                                                  << " on object " << o << " with different thread affinity " << o->thread();
                        }
                    }
                    if(defaults==0){
                        int signalIndex = QMetaObjectPrivate::signalIndex(method);
                        Q_ASSERT(signalIndex>=0);
                        if(signalIndex>=0){
                            QMetaObject::activate(o, 0, signalIndex, convertedArguments.data());
                            return;
                        }
                    }
                    metaObject->metacall(o, QMetaObject::InvokeMetaMethod, method.methodIndex(), convertedArguments.data());
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

struct NativeSlotObject : public QtPrivate::QSlotObjectBase
{
    NativeSlotObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link, bool noJNIInitialization, const QMetaMethod& signal, jobject connection, jint argumentCount, jint connectionType, bool nothrow, QObject* deletable = nullptr);
    ~NativeSlotObject();
    int* types();
    static void impl(int which, QSlotObjectBase *this_, QObject *r, void **a, bool *ret);
    bool m_noJNIInitialization;
    QMetaMethod m_signal;
    QWeakPointer<QtJambiLink> m_link;
    JObjectWrapper m_connection;
    int m_argumentCount;
    int* const m_types;
    QObject* m_deletable;
    bool m_nothrow;
};

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_connectNative
    (JNIEnv * env, jclass, QtJambiNativeID senderObjectId, jint signal, jlong senderMetaObjectId, QtJambiNativeID contextNativeId, jobject connection, jint argumentCount, jint connectionType)
{
    try{
        QSharedPointer<QtJambiLink> senderLink = QtJambiLink::fromNativeId(senderObjectId);
        NativeSlotObject *slotObj;
        QObject* sender = senderLink->qobject();
        Q_ASSERT(senderMetaObjectId);
        const QMetaObject* senderMetaObject = reinterpret_cast<const QMetaObject*>(senderMetaObjectId);
        QMetaMethod qt_signalMethod = senderMetaObject->method(signal);
        QObject* deletable = nullptr;
        QObject* context = QtJambiAPI::objectFromNativeId<QObject>(contextNativeId);
        int reducedConnectionType = (connectionType & ~Qt::UniqueConnection)
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                                    & ~Qt::SingleShotConnection
#endif
            ;
        QThread* senderAsThread = dynamic_cast<QThread*>(sender);
        if(reducedConnectionType==Qt::AutoConnection || reducedConnectionType==Qt::DirectConnection){
            if(senderAsThread && !context){
                connectionType = Qt::DirectConnection;
                deletable = new QObject();
                deletable->moveToThread(nullptr);
                context = deletable;
            }
        }else if(senderAsThread && QThreadData::get2(senderAsThread)->isAdopted){
            QString _connectionType;
            if(reducedConnectionType==Qt::BlockingQueuedConnection){
                _connectionType = "BlockingQueuedConnection";
            }else{
                _connectionType = "QueuedConnection";
            }
            Java::Runtime::IllegalStateException::throwNew(env, QStringLiteral("Cannot use signal QThread::%1 from adopted thread with connection type %2.").arg(qt_signalMethod.methodSignature().data()).arg(_connectionType) QTJAMBI_STACKTRACEINFO);
            return nullptr;
        }
        if(!context)
            context = sender;
        int signalIndex = QMetaObjectPrivate::signalIndex(qt_signalMethod);
        slotObj = new NativeSlotObject(env, senderLink, senderMetaObject==&QThread::staticMetaObject && qt_signalMethod.name()=="finished", qt_signalMethod, connection, argumentCount, connectionType, signalIndex==0, deletable);
        QMetaObject::Connection c = QObjectPrivate::connectImpl(sender,
                                                                signalIndex,
                                                                context,
                                                                nullptr,
                                                                slotObj,
                                                                Qt::ConnectionType(connectionType),
                                                                slotObj->types(),
                                                                qt_signalMethod.enclosingMetaObject());
        return qtjambi_cast<jobject>(env, c);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

#if QT_VERSION < QT_VERSION_CHECK(6,5,0)
struct SlotObjectBase {
    typedef void (*ImplFn)(int which, QtPrivate::QSlotObjectBase* this_, QObject *receiver, void **args, bool *ret);
    QAtomicInt m_ref;
    const ImplFn m_impl;
    bool isImpl(ImplFn impl) {
        return m_impl==impl;
    }
};
#define SLOT_OBJECT_CAST(A) reinterpret_cast<SlotObjectBase*>(A)
#else
#define SLOT_OBJECT_CAST(A) A
#endif

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_SignalUtility_disconnectNative__JIJLjava_util_function_Predicate_2
    (JNIEnv * env, jclass, QtJambiNativeID senderObjectId, jint signal, jlong senderMetaObjectId, jobject predicate)
{
    QList<QMetaObject::Connection> list;
    {
        QSharedPointer<QtJambiLink> senderLink = QtJambiLink::fromNativeId(senderObjectId);
        QObject* sender = senderLink->qobject();
        Q_ASSERT(senderMetaObjectId);
        QMetaMethod qt_signalMethod = reinterpret_cast<const QMetaObject*>(senderMetaObjectId)->method(signal);
        int signalIndex = QMetaObjectPrivate::signalIndex(qt_signalMethod);

        // we need to use meta call because that's the only way to get signal lock which is QtCore-internal.
        struct Call : QAbstractMetaCallEvent{
            JNIEnv * env;
            jobject predicate;
            int signalIndex;
            QList<QMetaObject::Connection> &list;
            Call(const QObject *sender, JNIEnv * _env, jobject _predicate, int _signalIndex, QList<QMetaObject::Connection> &_list)
                : QAbstractMetaCallEvent(sender, -1),
                env(_env),
                predicate(_predicate),
                signalIndex(_signalIndex),
                list(_list)
            {}
            void placeMetaCall(QObject *sender){
                try{
                    QObjectPrivate* objectp = QObjectPrivate::get(sender);
                    QObjectPrivate::ConnectionData* cd = objectp->connections.loadRelaxed();
                    if (cd) {
                        QObjectPrivate::Connection *c = cd->senders;
                        while (c) {
                            if(c->sender==sender
                                && c->signal_index==signalIndex
                                && c->isSlotObject
                                && SLOT_OBJECT_CAST(c->slotObj)->isImpl(&NativeSlotObject::impl)){
                                NativeSlotObject* slotObj = static_cast<NativeSlotObject*>(c->slotObj);
                                if(Java::Runtime::Predicate::test(env, predicate, slotObj->m_connection.object(env))){
                                    list << *reinterpret_cast<QMetaObject::Connection*>(&c);
                                }
                            }
                            c = c->next;
                        }
                    }
                }catch(const JavaException& exn){
                    exn.raiseInJava(env);
                }
            }
        } call(sender, env, predicate, signalIndex, list);
        sender->QObject::event(&call);
    }

    bool result = false;
    for(const QMetaObject::Connection& connection : qAsConst(list)){
        result |= QObject::disconnect(connection);
    }
    return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_connectNativeToMetaMethod
    (JNIEnv * env, jclass, QtJambiNativeID senderObjectId, jint signal, jlong senderMetaObjectId, QtJambiNativeID receiverObjectId, QtJambiNativeID slotId, jint connectionType)
{
    try{
        QSharedPointer<QtJambiLink> senderLink = QtJambiLink::fromNativeId(senderObjectId);
        QSharedPointer<QtJambiLink> receiverLink = QtJambiLink::fromNativeId(receiverObjectId);
        QObject* sender = senderLink ? senderLink->qobject() : nullptr;
        QMetaMethod signalMethod;
        if(senderMetaObjectId)
            signalMethod = reinterpret_cast<const QMetaObject*>(senderMetaObjectId)->method(signal);
        QObject* receiver = receiverLink ? receiverLink->qobject() : nullptr;
        QMetaMethod slotMethod = QtJambiAPI::valueReferenceFromNativeId<QMetaMethod>(slotId);
        if(!QMetaObject::checkConnectArgs(signalMethod, slotMethod)){
            QString message("Signal and slot signatures misfit: %1 != %2");
            message = message.arg(QLatin1String(signalMethod.methodSignature())).arg(QLatin1String(slotMethod.methodSignature()));
            jthrowable t = Java::QtJambi::QMisfittingSignatureException::newInstance(env, qtjambi_cast<jstring>(env, message));
            env->Throw(t);
            return nullptr;
        }
        QMetaObject::Connection c = QObject::connect(sender, signalMethod, receiver, slotMethod, Qt::ConnectionType(connectionType));
        return qtjambi_cast<jobject>(env, c);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_SignalUtility_disconnectNative__JIJJJ
    (JNIEnv *env, jclass, QtJambiNativeID senderObjectId, jint signal, jlong senderMetaObjectId, QtJambiNativeID receiverObjectId, QtJambiNativeID slotId)
{
    try{
        QSharedPointer<QtJambiLink> senderLink = QtJambiLink::fromNativeId(senderObjectId);
        QSharedPointer<QtJambiLink> receiverLink = QtJambiLink::fromNativeId(receiverObjectId);
        QObject* sender = senderLink ? senderLink->qobject() : nullptr;
        QMetaMethod signalMethod;
        if(senderMetaObjectId)
            signalMethod = reinterpret_cast<const QMetaObject*>(senderMetaObjectId)->method(signal);
        QObject* receiver = receiverLink ? receiverLink->qobject() : nullptr;
        QMetaMethod slotMethod = QtJambiAPI::valueReferenceFromNativeId<QMetaMethod>(slotId);
        return QObject::disconnect(sender, signalMethod, receiver, slotMethod);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_signalMethod
    (JNIEnv * env, jclass, jint signal, jlong senderMetaObjectId)
{
    try{
        QMetaMethod signalMethod;
        if(senderMetaObjectId)
            signalMethod = reinterpret_cast<const QMetaObject*>(senderMetaObjectId)->method(signal);
        return qtjambi_cast<jobject>(env, signalMethod);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_SignalUtility_signalName
    (JNIEnv * env, jclass, jint signal, jlong senderMetaObjectId)
{
    try{
        QMetaMethod signalMethod;
        if(senderMetaObjectId)
            signalMethod = reinterpret_cast<const QMetaObject*>(senderMetaObjectId)->method(signal);
        return signalMethod.isValid() ? env->NewStringUTF(signalMethod.name().constData()) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_SignalUtility_signalSignature
    (JNIEnv * env, jclass, jint signal, jlong senderMetaObjectId)
{
    try{
        QMetaMethod signalMethod;
        if(senderMetaObjectId)
            signalMethod = reinterpret_cast<const QMetaObject*>(senderMetaObjectId)->method(signal);
        return signalMethod.isValid() ? env->NewStringUTF(signalMethod.methodSignature().constData()) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" JNIEXPORT jstring JNICALL Java_io_qt_internal_SignalUtility_fullSignalSignature
    (JNIEnv * env, jclass, jint signal, jlong senderMetaObjectId)
{
    try{
        if(const QMetaObject* mo = reinterpret_cast<const QMetaObject*>(senderMetaObjectId)){
            QMetaMethod signalMethod = mo->method(signal);
            return signalMethod.isValid() ? env->NewStringUTF(QStringLiteral(u"%1::%2").arg(mo->className(), signalMethod.methodSignature()).toUtf8()) : nullptr;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

jobject invokeReflectiveMethod(JNIEnv *env,
                            jobject receiver,
                            jmethodID methodId,
                            jboolean isStatic,
                            jbyte returnType,
                            jobjectArray args,
                            jbyteArray _cnvTypes)
{
    jsize len = env->GetArrayLength(_cnvTypes);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    jbyte *cnvTypes = env->GetByteArrayElements(_cnvTypes, nullptr);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    QVarLengthArray<jvalue> argsArray(len);
    Q_ASSERT(args);
    for (int i=0; i<len; ++i) {
        jobject arg_object = env->GetObjectArrayElement(args, i);
        switch (cnvTypes[i]) {
        case 'L': argsArray[i].l = arg_object; break ;
        case 'Z': argsArray[i].z = QtJambiAPI::fromJavaBooleanObject(env, arg_object); break ;
        case 'J': argsArray[i].j = QtJambiAPI::fromJavaLongObject(env, arg_object); break ;
        case 'I': argsArray[i].i = QtJambiAPI::fromJavaIntegerObject(env, arg_object); break ;
        case 'F': argsArray[i].f = QtJambiAPI::fromJavaFloatObject(env, arg_object); break ;
        case 'D': argsArray[i].d = QtJambiAPI::fromJavaDoubleObject(env, arg_object); break ;
        case 'S': argsArray[i].s = QtJambiAPI::fromJavaShortObject(env, arg_object); break ;
        case 'B': argsArray[i].b = QtJambiAPI::fromJavaByteObject(env, arg_object); break ;
        case 'C': argsArray[i].c = QtJambiAPI::fromJavaCharacterObject(env, arg_object); break ;
        default:
            Q_ASSERT_X(false, "qtjambi_invoke_slot", "Error in conversion array");
        }
    }
    env->ReleaseByteArrayElements(_cnvTypes, cnvTypes, JNI_ABORT);
    jobject result = nullptr;
    if(env->ExceptionCheck()){
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
    if(isStatic){
        jclass clazz = jclass(receiver);
        switch (returnType)
        {
        case 'L': result = env->CallStaticObjectMethodA(clazz, methodId, argsArray.data()); break;
        case 'V': env->CallStaticVoidMethodA(clazz, methodId, argsArray.data()); break;
        case 'I': result = QtJambiAPI::toJavaIntegerObject(env, env->CallStaticIntMethodA(clazz, methodId, argsArray.data())); break;
        case 'J': result = QtJambiAPI::toJavaLongObject(env, env->CallStaticLongMethodA(clazz, methodId, argsArray.data())); break;
        case 'S': result = QtJambiAPI::toJavaShortObject(env, env->CallStaticShortMethodA(clazz, methodId, argsArray.data())); break;
        case 'Z': result = QtJambiAPI::toJavaBooleanObject(env, env->CallStaticBooleanMethodA(clazz, methodId, argsArray.data())); break;
        case 'F': result = QtJambiAPI::toJavaFloatObject(env, env->CallStaticFloatMethodA(clazz, methodId, argsArray.data())); break;
        case 'D': result = QtJambiAPI::toJavaDoubleObject(env, env->CallStaticDoubleMethodA(clazz, methodId, argsArray.data())); break;
        case 'B': result = QtJambiAPI::toJavaByteObject(env, env->CallStaticByteMethodA(clazz, methodId, argsArray.data())); break;
        case 'C': result = QtJambiAPI::toJavaCharacterObject(env, env->CallStaticCharMethodA(clazz, methodId, argsArray.data())); break;
        default:
            Q_ASSERT_X(false, "qtjambi_invoke_slot", "Invalid return type parameter");
            break;
        }
    }else{
        switch (returnType)
        {
        case 'L': result = env->CallObjectMethodA(receiver, methodId, argsArray.data()); break;
        case 'V': env->CallVoidMethodA(receiver, methodId, argsArray.data()); break;
        case 'I': result = QtJambiAPI::toJavaIntegerObject(env, env->CallIntMethodA(receiver, methodId, argsArray.data())); break;
        case 'J': result = QtJambiAPI::toJavaLongObject(env, env->CallLongMethodA(receiver, methodId, argsArray.data())); break;
        case 'S': result = QtJambiAPI::toJavaShortObject(env, env->CallShortMethodA(receiver, methodId, argsArray.data())); break;
        case 'Z': result = QtJambiAPI::toJavaBooleanObject(env, env->CallBooleanMethodA(receiver, methodId, argsArray.data())); break;
        case 'F': result = QtJambiAPI::toJavaFloatObject(env, env->CallFloatMethodA(receiver, methodId, argsArray.data())); break;
        case 'D': result = QtJambiAPI::toJavaDoubleObject(env, env->CallDoubleMethodA(receiver, methodId, argsArray.data())); break;
        case 'B': result = QtJambiAPI::toJavaByteObject(env, env->CallByteMethodA(receiver, methodId, argsArray.data())); break;
        case 'C': result = QtJambiAPI::toJavaCharacterObject(env, env->CallCharMethodA(receiver, methodId, argsArray.data())); break;
        default:
            Q_ASSERT_X(false, "qtjambi_invoke_slot", "Invalid return type parameter");
            break;
        }
    }
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_SignalUtility_invokeReflectiveMethod
    (JNIEnv *env,
     jclass,
     jobject receiver,
     jobject method,
     jboolean isStatic,
     jbyte returnType,
     jobjectArray args,
     jbyteArray cnvTypes)
{
    try{
        Q_ASSERT(receiver != nullptr);
        jmethodID methodId = env->FromReflectedMethod(method);
        Q_ASSERT(methodId);
        return invokeReflectiveMethod(env, receiver, methodId, isStatic, returnType, args, cnvTypes);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

NativeSlotObject::NativeSlotObject(JNIEnv * env, const QSharedPointer<QtJambiLink>& link, bool noJNIInitialization, const QMetaMethod& signal, jobject connection, jint argumentCount, jint connectionType, bool nothrow, QObject* deletable)
    : QSlotObjectBase(&NativeSlotObject::impl),
      m_noJNIInitialization(noJNIInitialization),
      m_signal(signal),
      m_link(link),
      m_connection(env, connection),
      m_argumentCount(argumentCount),
      m_types(
          [](JNIEnv * env, const QMetaMethod& signal, jint connectionType)->int*{
          if (connectionType == Qt::QueuedConnection || connectionType == Qt::BlockingQueuedConnection){
              const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, signal);
              if(parameterTypeInfos.size()>1){
                  int *types = new int[size_t(parameterTypeInfos.size())];
                  for (int i = 0; i < parameterTypeInfos.size(); ++i) {
                      if(i == parameterTypeInfos.size()-1){
                          types[i] = 0;
                      }else{
                          types[i] = parameterTypeInfos[i+1].metaType();
                      }
                  }
                  return types;
              }
            }
            return nullptr;
          }(env, signal, connectionType)
      ),
      m_deletable(deletable),
      m_nothrow(nothrow)
{
}

NativeSlotObject::~NativeSlotObject()
{
    if(m_connection){
        if(JniEnvironment env{200}){
            try{
                Java::QtJambi::SignalUtility$AbstractConnection::onDisconnect(env,m_connection.object(env));
                m_connection.clear(env);
            } catch (const JavaException& exn) {
                exn.report(env);
            }
        }
    }
    if(m_types)
        delete[] m_types;
    if(m_deletable)
        delete m_deletable;
}

int* NativeSlotObject::types() { return m_types; }

struct SlotJniEnvironment : JniEnvironment{
    SlotJniEnvironment(bool initializeJavaThread, int capacity) : JniEnvironment(int(initializeJavaThread ? JniEnvironmentFlag::Default : JniEnvironmentFlag::NoInitializeJavaThread), capacity){}
};

void NativeSlotObject::impl(int which, QSlotObjectBase *this_, QObject *, void **a, bool *ret){
    switch (which) {
    case Destroy:
        delete static_cast<NativeSlotObject*>(this_);
        break;
    case Call:{
            NativeSlotObject* _this = static_cast<NativeSlotObject*>(this_);
            QSharedPointer<QtJambiLink> link = _this->m_link.toStrongRef();
            if(link && link->isQObject()){
                if(SlotJniEnvironment env{!_this->m_noJNIInitialization, 300}){
                    QtJambiExceptionHandler __qt_exnhandler;
                    try{
                        QtJambiLinkScope scope(link);
                        const QVector<ParameterTypeInfo> parameterTypeInfos = QtJambiMetaObject::methodParameterInfo(env, _this->m_signal);
                        if(parameterTypeInfos.size()){
                            QVector<jvalue> converted_arguments(_this->m_argumentCount<0 ? parameterTypeInfos.size()-1 : qMin(_this->m_argumentCount, parameterTypeInfos.size()-1));

                            bool success = true;
                            for (int i = 0; i < converted_arguments.size(); ++i) {
                                const ParameterTypeInfo& parameterTypeInfo = parameterTypeInfos[i+1];
                                converted_arguments[i].l = nullptr;
                                if(!parameterTypeInfo.convertInternalToExternal(env, &scope, a[i+1], converted_arguments[i], true)){
                                    success = false;
                                    break;
                                }
                            }
                            if (success) {
                                jobjectArray args = Java::Runtime::Object::newArray(env, jsize(converted_arguments.size()));
                                for (int i=0; i<converted_arguments.size();++i) {
                                    env->SetObjectArrayElement(args, i, converted_arguments[i].l);
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO );
                                }
                                Java::QtJambi::SignalUtility$AbstractConnection::invoke(env,_this->m_connection.object(env), args);
                            } else {
                                qCWarning(DebugAPI::internalCategory) << "SlotObject::CallSignal: Failed to convert arguments";
                            }
                        }else{
                            qCWarning(DebugAPI::internalCategory) << "SlotObject::CallSignal: Failed to convert method types for signal " << _this->m_signal.methodSignature();
                        }
                    } catch (const JavaException& exn) {
                        if(noExceptionForwarding(env)){
                            QtJambiExceptionBlocker __blocker;
                            {
                                QtJambiExceptionHandler __handler;
                                __handler.handle(env, exn, "signal connection");
                            }
                            __blocker.release(env);
                        }else if(_this->m_nothrow){
                            exn.report(env);
                        }else{
                            //QString message("connection to %1::%2");
                            //message = message.arg(QLatin1String(_this->m_signal.enclosingMetaObject()->className()), QLatin1String(_this->m_signal.methodSignature()));
                            __qt_exnhandler.handle(env, exn, "signal connection");
                        }
                    }
                }
            }
        }
        break;
    case Compare:
        *ret = false;
        {
            QSlotObjectBase* other = reinterpret_cast<QSlotObjectBase*>(a[0]);
            if(SLOT_OBJECT_CAST(other)->isImpl(&NativeSlotObject::impl)){
                NativeSlotObject* _this = static_cast<NativeSlotObject*>(this_);
                NativeSlotObject* _other = static_cast<NativeSlotObject*>(other);
                QSharedPointer<QtJambiLink> link1 = _this->m_link.toStrongRef();
                QSharedPointer<QtJambiLink> link2 = _other->m_link.toStrongRef();
            }
        }
        break;
    case NumOperations: ;
    }
}


#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)

typedef QVarLengthArray<QArgumentType, 10> QArgumentTypeArray;

namespace QtJambiAPI{

    class ConvertingSlotObject : public QtPrivate::QSlotObjectBase
    {
    public:
        ConvertingSlotObject(const QMetaMethod& signal, const QMetaMethod& method, Qt::ConnectionType type, QSet<int>&& converter);
        ~ConvertingSlotObject();
        int* types();
    private:
        static void impl(int which, QSlotObjectBase *this_, QObject *r, void **a, bool *ret);
        QMetaMethod m_signal;
        QMetaMethod m_method;
        QSet<int> m_converter;
        int* const m_types;
    };

    ConvertingSlotObject::ConvertingSlotObject(const QMetaMethod& _signal, const QMetaMethod& method, Qt::ConnectionType type, QSet<int>&& converter)
        : QSlotObjectBase(&ConvertingSlotObject::impl),
          m_signal(_signal),
          m_method(method),
          m_converter(std::move(converter)),
          m_types(
              [](const QMetaMethod& signal, jint connectionType)->int*{
              if (connectionType == Qt::QueuedConnection || connectionType == Qt::BlockingQueuedConnection){
                      int *types = new int[size_t(signal.parameterCount())];
                      for (int i = 0; i < signal.parameterCount(); ++i) {
                          if(i == signal.parameterCount()-1){
                              types[i] = 0;
                          }else{
                              types[i] = signal.parameterType(i);
                          }
                      }
                      return types;
                }
                return nullptr;
              }(_signal, type)
          )
    {
    }

    ConvertingSlotObject::~ConvertingSlotObject()
    {
        if(m_types)
            delete[] m_types;
    }

    int* ConvertingSlotObject::types() { return m_types; }

    void ConvertingSlotObject::impl(int which, QSlotObjectBase *this_, QObject *receiver, void **a, bool *ret){
        switch (which) {
        case Destroy:
            delete static_cast<ConvertingSlotObject*>(this_);
            break;
        case Call:{
                ConvertingSlotObject* _this = static_cast<ConvertingSlotObject*>(this_);
                QList<QVariant> variants;
                QList<void*> newArgs;
                if(!_this->m_converter.isEmpty()){
                    newArgs.resize(_this->m_method.parameterCount()+1);
                    variants.resize(_this->m_method.parameterCount());
                    newArgs[0] = a[0];
                    for(int i=0; i<_this->m_method.parameterCount(); ++i){
                        if(_this->m_converter.contains(i)){
                            QMetaType sourceType = _this->m_signal.parameterMetaType(i);
                            QMetaType targetType = _this->m_method.parameterMetaType(i);
                            variants[i] = QVariant(sourceType, a[i+1]);
                            variants[i].convert(targetType);
                            newArgs[i+1] = variants[i].data();
                        }else{
                            newArgs[i+1] = a[i+1];
                        }
                    }
                    a = newArgs.data();
                }
                QObjectPrivate::StaticMetaCallFunction callFunction = _this->m_method.enclosingMetaObject()->d.static_metacall;
                if (callFunction) {
                    callFunction(receiver, QMetaObject::InvokeMetaMethod, _this->m_method.methodIndex() - _this->m_method.enclosingMetaObject()->methodOffset(), a);
                } else {
                    QMetaObject::metacall(receiver, QMetaObject::InvokeMetaMethod, _this->m_method.methodIndex(), a);
                }
            }
            break;
        case Compare:{
            ConvertingSlotObject* _this = static_cast<ConvertingSlotObject*>(this_);
            ConvertingSlotObject* other = static_cast<ConvertingSlotObject*>(a[0]);
            *ret = other->m_method==_this->m_method && other->m_signal==_this->m_signal;
        }
            break;
        case NumOperations: ;
        }
    }

    inline int extract_code(const char *member)
    {
        // extract code, ensure QMETHOD_CODE <= code <= QSIGNAL_CODE
        return (((int)(*member) - '0') & 0x3);
    }

    bool check_signal_macro(const char *signal)
    {
        int sigcode = extract_code(signal);
        if (sigcode != QSIGNAL_CODE) {
            return false;
        }
        return true;
    }

    int *queuedConnectionTypes(const QMetaMethod &method)
    {
        const auto parameterCount = method.parameterCount();
        int *typeIds = new int[parameterCount + 1];
        Q_CHECK_PTR(typeIds);
        for (int i = 0; i < parameterCount; ++i) {
            const QMetaType metaType = method.parameterMetaType(i);
            if (metaType.flags() & QMetaType::IsPointer)
                typeIds[i] = QMetaType::VoidStar;
            else
                typeIds[i] = metaType.id();
            if (!typeIds[i]) {
                delete[] typeIds;
                return nullptr;
            }
        }
        typeIds[parameterCount] = 0;

        return typeIds;
    }

    int *queuedConnectionTypes(const QArgumentType *argumentTypes, int argc)
    {
        auto types = std::make_unique<int[]>(argc + 1);
        for (int i = 0; i < argc; ++i) {
            const QArgumentType &type = argumentTypes[i];
#if QT_VERSION < QT_VERSION_CHECK(6, 9, 0)
            if (type.type())
                types[i] = type.type();
            else if (type.name().endsWith('*'))
                types[i] = QMetaType::VoidStar;
            else
                types[i] = QMetaType::fromName(type.name()).id();
#else
            if (type.metaType().isValid())
                types[i] = type.metaType().id();
#endif
            if (!types[i]) {
                return nullptr;
            }
        }
        types[argc] = 0;

        return types.release();
    }

    bool checkConnectArgs(const QMetaMethod &signal,
                          const QMetaMethod &method, QSet<int>& converter){
        if(method.parameterCount()<=signal.parameterCount()){
            for(int i=0; i<method.parameterCount(); ++i){
                QMetaType sourceType = signal.parameterMetaType(i);
                QMetaType targetType = method.parameterMetaType(i);
                if(targetType==QMetaType::fromType<JObjectWrapper>()){
                    if(JniEnvironment env{200}){
                        jclass clazz = CoreAPI::getClassForMetaType(env, sourceType);
                        if(Java::QtJambi::QFlags::isAssignableFrom(env, clazz)){
                            converter.insert(i);
                            continue;
                        }
                    }
                    if(!isJObjectWrappedMetaType(sourceType) && !JObjectValueWrapper::isValueType(sourceType)){
                        return false;
                    }
                }else if(targetType==QMetaType::fromType<JCollectionWrapper>()
                         || targetType==QMetaType::fromType<JMapWrapper>()
                         || targetType==QMetaType::fromType<JIteratorWrapper>()){
                    if(!isJObjectWrappedMetaType(sourceType) && !JObjectValueWrapper::isValueType(sourceType)){
                        return false;
                    }
                }else if(targetType==QMetaType::fromType<JEnumWrapper>()){
                    converter.insert(i);
                }
            }
            return true;
        }
        return false;
    }

    QMetaObject::Connection connect(const QObject *sender, const char *signal,
                                                   const QObject *receiver, const char *method,
                                                   Qt::ConnectionType type){
        if (sender == nullptr || receiver == nullptr || signal == nullptr || method == nullptr) {
            return QObject::connect(sender, signal, receiver, method, type);
        }
        QByteArray tmp_signal_name;

        if (!check_signal_macro(signal))
            return QObject::connect(sender, signal, receiver, method, type);
        const QMetaObject *smeta = sender->metaObject();
        ++signal; // skip code
        QArgumentTypeArray signalTypes;
        Q_ASSERT(QMetaObjectPrivate::get(smeta)->revision >= 7);
        int signal_index = smeta->indexOfMethod(signal);
        if (signal_index < 0) {
            // check for normalized signatures
            tmp_signal_name = QMetaObject::normalizedSignature(signal - 1);
            signal = tmp_signal_name.constData() + 1;
            signalTypes.clear();
            smeta = sender->metaObject();
            signal_index = smeta->indexOfMethod(signal);
        }
        if (signal_index < 0) {
            return QObject::connect(sender, signal, receiver, method, type);
        }
        QMetaMethod msignal = smeta->method(signal_index);

        QByteArray tmp_method_name;
        int membcode = extract_code(method);

        if (membcode != QSLOT_CODE && membcode != QSIGNAL_CODE)
            return QObject::connect(sender, signal, receiver, method, type);
        ++method; // skip code

        const QMetaObject *rmeta = receiver->metaObject();
        int method_index = rmeta->indexOfMethod(method);
        if (method_index < 0) {
            // check for normalized methods
            tmp_method_name = QMetaObject::normalizedSignature(method);
            method = tmp_method_name.constData();
            method_index = rmeta->indexOfMethod(method);
        }

        if (method_index < 0) {
            return QObject::connect(sender, signal, receiver, method, type);
        }
        QMetaMethod mmethod = rmeta->method(method_index);
        return connect(sender, msignal, receiver, mmethod, type);
    }

    QMetaObject::Connection connect(const QObject *sender, const QMetaMethod &signal,
                                                   const QObject *receiver, const QMetaMethod &method,
                                                   Qt::ConnectionType type){
        if (sender == nullptr
                || receiver == nullptr
                || signal.methodType() != QMetaMethod::Signal
                || method.methodType() == QMetaMethod::Constructor) {
            return QObject::connect(sender, signal, receiver, method, type);
        }

        int signal_index = QMetaObjectPrivate::signalIndex(signal);
        int method_index = method.methodIndex();

        if (signal_index == -1 || method_index == -1) {
            return QObject::connect(sender, signal, receiver, method, type);
        }

        if (!QMetaObject::checkConnectArgs(signal.methodSignature().constData(),
                                           method.methodSignature().constData())) {
            QSet<int> converter;
            if(checkConnectArgs(signal, method, converter)){
                ConvertingSlotObject *slotObj = new ConvertingSlotObject(signal, method, type, std::move(converter));
                return QObjectPrivate::connectImpl(sender,
                                                    signal_index,
                                                    receiver,
                                                    nullptr,
                                                    slotObj,
                                                    type,
                                                    slotObj->types(),
                                                    signal.enclosingMetaObject());
            }
        }
        return QObject::connect(sender, signal, receiver, method, type);
    }
}

#endif
