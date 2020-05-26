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

#include "qtjambi_core.h"
#include "qtjambi_cast.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambivariant_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_thread.h"
#include "qtjambi_thread_p.h"
#include "qtjambi_interfaces.h"
#include "qtjambitypemanager_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_repository_p.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QVarLengthArray>
#include <QtCore/QPointer>
#include <QtCore/QStringList>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QThread>
#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/private/qabstractfileengine_p.h>
#include <QtCore/private/qplugin_p.h>

#ifdef QTJAMBI_SANITY_CHECK
#include <QtCore/QObject>
#include <QtCore/private/qobject_p.h>
#endif

#include <QtCore/private/qobject_p.h>
#include <QtCore/private/qthread_p.h>
#include <QtCore/private/qcoreapplication_p.h>

#ifdef Q_OS_DARWIN
#include <pthread.h>
#endif

QT_WARNING_DISABLE_DEPRECATED

JNIEnv* qtjambi_current_environment(bool);

#ifdef Q_OS_WIN
#include <windows.h>
# endif

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_disconnectConnection)
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

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_isSharedPointer)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID nativeId)
{
    try{
        QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId);
        return link && link->isSharedPointer();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;

}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_setQObjectSender)
(JNIEnv * env, jclass, QtJambiNativeID r, QtJambiNativeID s)
{
    try{
        QObject *the_receiver = qtjambi_object_from_nativeId<QObject>(r);
        QObject *the_sender = qtjambi_object_from_nativeId<QObject>(s);
        if (the_receiver == nullptr)
            return 0;

#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        void **keep = new void*[2];
        {
            QObjectPrivate::Sender *sender = new QObjectPrivate::Sender;
            sender->sender = the_sender;
            sender->signal = -1;
            sender->ref = 1;

            // Store the old sender as "return value"
            keep[0] = QObjectPrivate::setCurrentSender(the_receiver, sender);
            keep[1] = sender;
        }
        return jlong(keep);
#else
        return jlong(new QObjectPrivate::Sender(the_receiver, the_sender, -1));
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}


extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_resetQObjectSender)
(JNIEnv *env, jclass, QtJambiNativeID r, jlong keep)
{
    try{
#if QT_VERSION < QT_VERSION_CHECK(5,14,0)
        QObject *receiver = qtjambi_object_from_nativeId<QObject>(r);
        void **senders = reinterpret_cast<void **>(keep);

        if(senders){
            QObjectPrivate::Sender *oldSender = reinterpret_cast<QObjectPrivate::Sender *>(senders[0]);
            QObjectPrivate::Sender *sender = reinterpret_cast<QObjectPrivate::Sender *>(senders[1]);
            if(receiver){
                QObjectPrivate::resetCurrentSender(receiver, sender, oldSender);
            }
            delete sender;
            delete [] senders;
        }
#else
        Q_UNUSED(r)
        if(QObjectPrivate::Sender* sender = reinterpret_cast<QObjectPrivate::Sender*>(keep)){
            delete sender;
        }
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

static QHash<jint,jfieldID> enumClassValuesFields = QHash<jint,jfieldID>();
static QHash<jint,jboolean> enumClassExtensible = QHash<jint,jboolean>();

jboolean isClassExtensible(JNIEnv *env, jint hashCode, jclass enumClass){
    if(!enumClassExtensible.contains(hashCode)){
        enumClassExtensible[hashCode] = Java::Private::Runtime::Class.isAnnotationPresent(env, enumClass, Java::Private::QtJambi::QtExtensibleEnum.getClass(env));
    }
    return enumClassExtensible[hashCode];
}

jfieldID findValueField(jthrowable& t, JNIEnv *env, jint hashCode, jclass enumClass){
    jfieldID valuesField = enumClassValuesFields.value(hashCode, nullptr);
    if(!valuesField){
        QString signature = QString("[L%1;").arg(qtjambi_class_name(env, enumClass).replace(QLatin1Char('.'), QLatin1Char('/')));
        valuesField = resolveField(env, "$VALUES", qPrintable(signature), enumClass, true);
        if(!valuesField){
            if(env->ExceptionCheck()){
                t = env->ExceptionOccurred();
                env->ExceptionClear();
            }
        }
        if(jfieldID enumValuesField = resolveField(env, "ENUM$VALUES", qPrintable(signature), enumClass, true)){
            valuesField = enumValuesField;
        }else if(env->ExceptionCheck()){
            env->ExceptionClear();
        }
        for (int i=0; true; ++i) {
            if(jfieldID enumValuesNField = resolveField(env, qPrintable(QString("ENUM$VALUES_%1").arg(i)), qPrintable(signature), enumClass, true)){
                valuesField = enumValuesNField;
            }else{
                if(env->ExceptionCheck())
                    env->ExceptionClear();
                break;
            }
        }
        if(valuesField)
            enumClassValuesFields[hashCode] = valuesField;
    }
    return valuesField;
}

jobjectArray extendEnumValues(JNIEnv *env, jclass enumClass, jobjectArray values, int length, jobject newEntry){
    values = Java::Private::Runtime::Arrays.copyOf(env, values, length+1);
    env->SetObjectArrayElement(values, length, newEntry);
    Java::Private::QtJambi::QtJambiInternal.extendEnum(env, enumClass, values, newEntry);
    return values;
}

QString simpleClassName(JNIEnv *env, jclass enumClass){
    QString className = qtjambi_class_name(env, enumClass);
    int idx = className.lastIndexOf('$');
    if(idx>=0){
        className = className.mid(idx);
    }
    return className;
}

jobject getRejectedEntries(JNIEnv *env, jclass enumClass){
    jobject rejectedEntries = nullptr;
    if(jobject qtRejectedEntries = Java::Private::Runtime::Class.getAnnotation(env, enumClass, Java::Private::QtJambi::QtRejectedEntries.getClass(env))){
        jobject rejectedValues = Java::Private::QtJambi::QtRejectedEntries.value(env, qtRejectedEntries);
        rejectedEntries = Java::Private::Runtime::Set.of(env, rejectedValues);
    }
    return rejectedEntries;
}

template<class jtype, template <typename,typename> class Map>
jobject resolveEnum(JNIEnv *env, jint hashCode, jclass enumClass, jtype value, jstring entryName){
    if(Java::Private::Runtime::Enum.isAssignableFrom(env, enumClass)){
        static QReadWriteLock* mutex = new QReadWriteLock();
        QWriteLocker locker(mutex);
        Q_UNUSED(locker)

        static QHash<jint, Map<jtype,jsize> > cachesByEnum = QHash<jint, Map<jtype,jsize> >();
        jobject object = nullptr;
        jthrowable t = nullptr;
        jfieldID valuesField = findValueField(t, env, hashCode, enumClass);
        if(valuesField){
            jobjectArray values = jobjectArray(env->GetStaticObjectField(enumClass, valuesField));
            qtjambi_throw_java_exception(env)
            jsize length = env->GetArrayLength(values);

            if(!cachesByEnum.contains(hashCode)){
                Map<jtype,jsize> indexesByValue;
                jobject rejectedEntries = getRejectedEntries(env, enumClass);
                for(jsize i=0; i<length; ++i){
                    jobject entry = env->GetObjectArrayElement(values, i);
                    if(!rejectedEntries || ![env,rejectedEntries,entry]()->jboolean{
                            jstring entry_name = Java::Private::Runtime::Enum.name(env,entry);
                            return Java::Private::Runtime::Collection.contains(env, rejectedEntries, entry_name);
                        }()){
                        jtype v = qtjambi_to_enumerator(env,entry).value<jtype>();
                        if(!indexesByValue.contains(v))
                            indexesByValue.insert(v, i);
                    }
                }
                cachesByEnum.insert(hashCode, indexesByValue);
            }
            Map<jtype,jsize> &indexesByValue = cachesByEnum[hashCode];
            jsize index = indexesByValue.value(value, -1);
            if(index<0 && isClassExtensible(env, hashCode, enumClass)){
                const char* sig;
                switch ( sizeof(jtype) ) {
                case 1:  sig = "(Ljava/lang/String;IB)V"; break;
                case 2:  sig = "(Ljava/lang/String;IS)V"; break;
                case 8:  sig = "(Ljava/lang/String;IJ)V"; break;
                default: sig = "(Ljava/lang/String;II)V"; break;
                }
                jmethodID methodId = resolveMethod(env, "<init>", sig, enumClass);
                qtjambi_throw_java_exception(env)
                Q_ASSERT(methodId);
                QString className = simpleClassName(env, enumClass);
                object = env->NewObject(enumClass, methodId, entryName ? entryName : qtjambi_from_qstring(env, QString("Custom%1(%2)").arg(className).arg(value)), length, value);
                qtjambi_throw_java_exception(env)
                values = extendEnumValues(env, enumClass, values, length, object);
                indexesByValue.insert(value, length);
                index = length;
                ++length;
                env->SetStaticObjectField(enumClass, valuesField, values);
                qtjambi_throw_java_exception(env)
            }
            if(index>=0 && index<length){
                object = env->GetObjectArrayElement(values, index);
                qtjambi_throw_java_exception(env)
            }
        }else if(t){
            JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
        }
        return object;
    }else{
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1threadCheck)
(JNIEnv * __jni_env, jclass, QtJambiNativeID __this_nativeId, jstring message)
{
    try{
        QObject *__qt_this = qtjambi_object_from_nativeId<QObject>(__this_nativeId);
        Q_ASSERT(__qt_this);
        QThread* objectThread = __qt_this->thread();
        QThread* currentThread = QThread::currentThread();
        if (objectThread && objectThread != currentThread) {
            if(QThread* this_thread = qobject_cast<QThread*>(__qt_this)){
                objectThread = this_thread;
                if(objectThread == currentThread)
                    return;
            }
            jobject obj = qtjambi_java_object_reference(__this_nativeId, __jni_env);
            if(message){
                jthrowable t = Java::Private::QtJambi::QThreadAffinityException.newInstance(__jni_env,
                                                               message,
                                                               obj,
#ifdef QT_QTJAMBI_PORT
                                                               qtjambi_from_thread(__jni_env, objectThread),
                                                               qtjambi_from_thread(__jni_env, currentThread)
#else
                                                               qtjambi_cast<jobject>(__jni_env, objectThread),
                                                               qtjambi_cast<jobject>(__jni_env, currentThread)
#endif
                                                    );
                __jni_env->Throw(t);
            }else{
                JavaException::raiseQThreadAffinityException(__jni_env, "QObject used from outside its own thread" QTJAMBI_STACKTRACEINFO ,
                                                             obj,
                                                             objectThread, currentThread);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

jobject resolveLongEnum(JNIEnv *env, jint hashCode, jclass enumClass, jlong value, jstring entryName){
    return resolveEnum<jlong,QMap>(env, hashCode, enumClass, value, entryName);
}

jobject resolveByteEnum(JNIEnv *env, jint hashCode, jclass enumClass, jbyte value, jstring entryName){
    return resolveEnum<jbyte,QHash>(env, hashCode, enumClass, value, entryName);
}

jobject resolveShortEnum(JNIEnv *env, jint hashCode, jclass enumClass, jshort value, jstring entryName){
    return resolveEnum<jshort,QHash>(env, hashCode, enumClass, value, entryName);
}

jobject resolveIntEnum(JNIEnv *env, jint hashCode, jclass enumClass, jint value, jstring entryName){
    return resolveEnum<jint,QHash>(env, hashCode, enumClass, value, entryName);
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_resolveLongEnum)
(JNIEnv *env, jclass, jint hashCode, jclass enumClass, jlong value, jstring entryName)
{
    try{
        return resolveLongEnum(env, hashCode, enumClass, value, entryName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_resolveByteEnum)
(JNIEnv *env, jclass, jint hashCode, jclass enumClass, jbyte value, jstring entryName)
{
    try{
        return resolveByteEnum(env, hashCode, enumClass, value, entryName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_resolveShortEnum)
(JNIEnv *env, jclass, jint hashCode, jclass enumClass, jshort value, jstring entryName)
{
    try{
        return resolveShortEnum(env, hashCode, enumClass, value, entryName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_resolveIntEnum)
(JNIEnv *env, jclass, jint hashCode, jclass enumClass, jint value, jstring entryName)
{
    try{
        return resolveIntEnum(env, hashCode, enumClass, value, entryName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_setField)
(JNIEnv *env,
 jclass,
 jobject _this,
 jobject field,
 jobject newValue)
{
    try{
        jfieldID fieldId = env->FromReflectedField(field);
        Q_ASSERT(fieldId != nullptr);

        env->SetObjectField(_this, fieldId, newValue);
        qtjambi_throw_java_exception(env)
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_fetchSignal)
(JNIEnv *env,
 jclass,
 jobject java_object,
 jobject field,
 jboolean isStatic)
{
    try{
        if (field == nullptr)
            return nullptr;
        jfieldID fieldId = env->FromReflectedField(field);
        qtjambi_throw_java_exception(env)
        jobject signal;
        if(isStatic)
            signal = env->GetStaticObjectField(jclass(java_object), fieldId);
        else
            signal = env->GetObjectField(java_object, fieldId);
        qtjambi_throw_java_exception(env)
        return signal;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_convertNativeException)
(JNIEnv *env,
 jclass,
 jlong exception)
{
    if(exception){
        const std::exception* exn = reinterpret_cast<const std::exception*>(exception);
        if(typeid(*exn)==typeid(JavaException)){
            const JavaException* jexn = reinterpret_cast<const JavaException*>(exception);
            return jexn->object();
        }else{
            return Java::Private::Runtime::RuntimeException.newInstanceWithMessage(env, exn->what());
        }
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_privateLookup)
(JNIEnv *env,
 jclass,
 jclass targetClass)
{
    try{
        Q_ASSERT(targetClass);
        return Java::Private::Runtime::MethodHandles$Lookup.newInstance(env, targetClass);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_resolveSlot)
(JNIEnv *env,
 jclass,
 jobject method)
{
    try{
        Q_ASSERT(method);
        return reinterpret_cast<jlong>(env->FromReflectedMethod(method));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_invokeSlot)
(JNIEnv *env,
 jclass,
 jobject receiver,
 jlong m,
 jbyte returnType,
 jobjectArray args,
 jintArray cnvTypes)
{
    try{
        Q_ASSERT(receiver != nullptr);
        Q_ASSERT(m != 0);

        jmethodID methodId = reinterpret_cast<jmethodID>(m);
        qtjambi_invoke_method(env, receiver, methodId, returnType, qtjambi_from_jobjectArray(env, args, cnvTypes));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_invokeStaticSlot)
(JNIEnv *env,
 jclass,
 jclass clazz,
 jlong m,
 jbyte returnType,
 jobjectArray args,
 jintArray cnvTypes)
{
    try{
        Q_ASSERT(clazz != nullptr);
        Q_ASSERT(m != 0);

        jmethodID methodId = reinterpret_cast<jmethodID>(m);
        qtjambi_invoke_static_method(env, clazz, methodId, returnType, qtjambi_from_jobjectArray(env, args, cnvTypes));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_setFieldNative)
(JNIEnv *env,
 jclass,
 jobject owner,
 jobject field,
 jboolean isStatic,
 jobject newValue)
{
    try{
        if (owner == nullptr || field == nullptr) return false;

        jfieldID id = env->FromReflectedField(field);
        if (id == nullptr) return false;

        if(isStatic)
            env->SetStaticObjectField(jclass(owner), id, newValue);
        else
            env->SetObjectField(owner, id, newValue);
        return true;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_fetchFieldNative)
(JNIEnv *env,
 jclass,
 jobject owner,
 jobject field,
 jboolean isStatic)
{
    try{
        if (owner == nullptr || field == nullptr) return nullptr;

        jfieldID id = env->FromReflectedField(field);
        if (id == nullptr) return nullptr;

        if(isStatic)
            return env->GetStaticObjectField(jclass(owner), id);
        else
            return env->GetObjectField(owner, id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_invalidateObject)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            if(!link->isShell() && !link->isQObject()){
                link->invalidate(env);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_setCppOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        qtjambi_set_cpp_ownership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_setDefaultOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        qtjambi_set_default_ownership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_setJavaOwnership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        qtjambi_set_java_ownership(env, native_id);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jbyte JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_ownership)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            return jbyte(link->ownership());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_hasOwnerFunction)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            if(!link->isQObject()){
                PtrOwnerFunction ownerFunction;
                if(link->isSharedPointer()){
                    ownerFunction = reinterpret_cast<QSharedPointerToObjectLink*>(link.data())->ownerFunction();
                }else{
                    ownerFunction = reinterpret_cast<PointerToObjectLink*>(link.data())->ownerFunction();
                }
                return ownerFunction!=nullptr;
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_owner)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            if(!link->isQObject()){
                PtrOwnerFunction ownerFunction;
                if(link->isSharedPointer()){
                    ownerFunction = reinterpret_cast<QSharedPointerToObjectLink*>(link.data())->ownerFunction();
                }else{
                    ownerFunction = reinterpret_cast<PointerToObjectLink*>(link.data())->ownerFunction();
                }
                if(ownerFunction){
                    const QObject* owner = ownerFunction(link->pointer());
                    return qtjambi_cast<jobject>(env, owner);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1reassignLink)
    (JNIEnv *env, jclass, QtJambiNativeID old_native_id, jclass clazz, jobject constructor)
{
    try{
        QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(old_native_id);
        Q_ASSERT(link);

        jmethodID methodId = env->FromReflectedMethod(constructor);
        Q_ASSERT(methodId);

        jobject new_object = env->NewObject(clazz, methodId, 0);
        Q_ASSERT(new_object);

        bool createdByJava = link->createdByJava();
        bool is_shell = link->isShell();
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        const char* className =  link->qtTypeName();
#endif
        if (link->isQObject()) {
            QObject *qobject = link->qobject();
            QtJambiLink::Ownership ownership = link->ownership();
            link->invalidate(env);
            link = QSharedPointer<QtJambiLink>();
            const QSharedPointer<QtJambiLink>& new_link = QtJambiLink::createLinkForQObject(env, new_object, qobject, createdByJava, is_shell);
            switch (ownership) {
            case QtJambiLink::Ownership::Java:
                new_link->setJavaOwnership(env);
                break;
            case QtJambiLink::Ownership::Split:
                new_link->setSplitOwnership(env);
                break;
            default: // default is cpp ownership for qobjects
                new_link->setCppOwnership(env);
                break;
            }
        } else {
            PtrDeleterFunction deleter = nullptr;
            PtrOwnerFunction registeredOwnerFunction = nullptr;
            if(link->isSharedPointer()){
                QSharedPointerToObjectLink* polink = static_cast<QSharedPointerToObjectLink*>(link.data());
                deleter = polink->destructorFunction();
                registeredOwnerFunction = polink->ownerFunction();
            }else{
                PointerToObjectLink* polink = static_cast<PointerToObjectLink*>(link.data());
                deleter = polink->destructorFunction();
                registeredOwnerFunction = polink->ownerFunction();
            }
            void *ptr = link->pointer();
            int metatype = link->metaType();
            link->invalidate(env);
            link = nullptr;

            // Create new link.
            QtJambiLink::createLinkForObject(env,
                                                 new_object,
                                                 ptr,
                                                 metatype,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
                                                 className,
#endif
                                                 createdByJava,
                                                 is_shell,
                                                 deleter,
                                                 registeredOwnerFunction);
        }
        return new_object;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

void qtjambi_shutdown(JNIEnv * env);

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambi_1LibraryShutdown_shutdown)
(JNIEnv * env, jclass)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi_LibraryShutdown::shutdownAll()")
    try{
        qtjambi_shutdown(env);	// NULL the JavaVM handle
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_MetaObjectTools_registerQmlListProperty)
(JNIEnv *env, jclass, jstring type)
{
    try{
        QString _type = qtjambi_to_qstring(env, type);
        return QMetaType::registerNormalizedTypedef(_type.toUtf8(), QMetaType::type("QQmlListProperty<QObject>"));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return QMetaType::UnknownType;
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_MetaObjectTools_internalMethodName)
(JNIEnv *env, jclass, jobject method)
{
    QMetaMethod metaMethod = qtjambi_cast<QMetaMethod>(env, method);
    return qtjambi_from_qstring(env, QLatin1String(metaMethod.name()));
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_MetaObjectTools_cppMethodSignature)
(JNIEnv *env, jclass, jobject method)
{
    QMetaMethod metaMethod = qtjambi_cast<QMetaMethod>(env, method);
    return qtjambi_from_qstring(env, QLatin1String(metaMethod.methodSignature()));
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_MetaObjectTools_registerClass)
(JNIEnv *env, jclass, jstring s, jclass cls)
{
    const char* c = env->GetStringUTFChars(s, nullptr);
    getGlobalClassRef(env, cls, c);
    env->ReleaseStringUTFChars(s, c);
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_MetaObjectTools_internalTypeName)
(JNIEnv *env, jclass, jstring s, jobject classLoader)
{
    try{
        QString signature = qtjambi_to_qstring(env, s);

        int prefix_end = signature.indexOf("(");
        QString prefix;
        if (prefix_end >= 0) {
            prefix = signature.mid(0, prefix_end+1);
            signature = signature.mid(prefix_end+1);
        }

        int postfix_start = signature.lastIndexOf(")");
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
                    return qtjambi_from_qstring(env, "");
                }
            }
        }

        return qtjambi_from_qstring(env, prefix + allArgs.join(",") + postfix);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

void __savePointer(QDataStream &stream, const void * pointer){
#if QT_POINTER_SIZE==8
    qint64 value = qint64(pointer);
#else
    qint32 value = qint32(pointer);
#endif
    stream << value;
    qWarning() << "write pointer: " << value;
}

void __loadPointer(QDataStream & stream, void * pointer){
    Q_UNUSED(pointer)
#if QT_POINTER_SIZE==8
    qint64 value;
#else
    qint32 value;
#endif
    stream >> value;
    pointer = reinterpret_cast<void*>(value);
    qWarning() << "read pointer: " << value;
}

template<size_t SIZE>
void * constructHelper(void * where, const void *pointer)
{
    memcpy(where, pointer, SIZE);
    return where;
}

QMetaType::Constructor findConstructor(int size){
    switch(size){
    case 1: return &constructHelper<1>;
    case 2: return &constructHelper<2>;
    case 4: return &constructHelper<4>;
    case 8: return &constructHelper<8>;
    default:
        return nullptr;
    }
}

void destructHelper(void *){}

void * pointerConstructHelper(void * where, const void *pointer)
{
    if(pointer){
        *reinterpret_cast<void**>(where) = *reinterpret_cast<void**>(const_cast<void*>(pointer));
    }
    return where;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_constructorThreadCheckApplication)
    (JNIEnv *__jni_env, jclass, jobject obj)
{
    try{
        if(QThread::currentThread()!=QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            JavaException::raiseQThreadAffinityException(__jni_env, "QCoreApplication created outside main thread" QTJAMBI_STACKTRACEINFO, obj,
                                                        QThread::currentThread(), QCoreApplicationPrivate::theMainThread.loadRelaxed() );
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_constructorThreadCheckWindow)
    (JNIEnv *__jni_env, jclass, jobject)
{
    try{
        QCoreApplication* instance = QCoreApplication::instance();
        if(!instance) {
            JavaException::raiseIllegalStateException(__jni_env, "Cannot create a QWindow before initializing QGuiApplication." QTJAMBI_STACKTRACEINFO);
        }else if(!instance->inherits("QGuiApplication")){
            JavaException::raiseIllegalStateException(__jni_env, qPrintable(QString("Cannot create a QWindow without QGuiApplication (instead of %1).").arg(instance->metaObject()->className())) QTJAMBI_STACKTRACEINFO);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_constructorThreadCheckWidget)
    (JNIEnv *__jni_env, jclass, jobject obj)
{
    try{
        QCoreApplication* instance = QCoreApplication::instance();
        if(!instance) {
            JavaException::raiseIllegalStateException(__jni_env, "Cannot create a QWidget before initializing QApplication." QTJAMBI_STACKTRACEINFO);
        }else if(!instance->inherits("QApplication")){
            JavaException::raiseIllegalStateException(__jni_env, qPrintable(QString("Cannot create a QWidget without QApplication (instead of %1).").arg(instance->metaObject()->className())) QTJAMBI_STACKTRACEINFO);
        }else if (instance->thread() != QThread::currentThread()) {
            JavaException::raiseQThreadAffinityException(__jni_env, "QWidget created outside main thread" QTJAMBI_STACKTRACEINFO,
                                               obj,
                                               instance->thread(),
                                               QThread::currentThread());
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

/**
 * new implementation according to Qt5's meta object API
 */
extern "C" Q_DECL_EXPORT int JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1registerMetaType)
(JNIEnv *env, jclass, jclass clazz)
{
    try{
        QString javaClassName = qtjambi_class_name(env, clazz).replace('.', '/');

        QString qtName = QtJambiTypeManager::getInternalTypeName(env, javaClassName);

        QString javaName = javaClassName.replace("/", "::");
        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
            int metaId = registeredMetaTypeID(*typeId);
            if(metaId!=QMetaType::UnknownType)
                return metaId;
            if(qtName.endsWith("*")){
                int id = QMetaType::type(qtName.toLatin1().constData());
                if(id==QMetaType::UnknownType){
                    QMetaType::TypeFlags flags = QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction;
                    const QMetaObject *meta_object = nullptr;
                    QMetaType::Destructor destructor = &destructHelper;
                    QMetaType::Constructor constructor = &pointerConstructHelper;
                    int size = sizeof(void*);
                    if(Java::Private::QtCore::QObject.isAssignableFrom(env, clazz)){
                        meta_object = registeredOriginalMetaObject(*typeId);
                        flags |= QMetaType::PointerToQObject;
                    }
                    id = QMetaType::registerNormalizedType(
                            QMetaObject::normalizedType(qtName.toLatin1().constData()),
                            destructor,
                            constructor,
                            size,
                            flags,
                            meta_object
                        );
                    Q_ASSERT(id!=0);
                }
                return QMetaType::registerTypedef(javaName.toLatin1(), id);
            }else if(qtName==QLatin1String("JEnumWrapper")){
                int last = javaName.lastIndexOf('$');
                if(last>0){
                    javaName.replace(last, 1, "::");
                }
                int id = qRegisterMetaType<JEnumWrapper>("JEnumWrapper");
                return QMetaType::registerTypedef(javaName.toLatin1(), id);
            }else if(Java::Private::Runtime::Enum.isAssignableFrom(env, clazz)){
                int last = javaName.lastIndexOf('$');
                if(last>0){
                    javaName.replace(last, 1, "::");
                }
                int id = qRegisterMetaType<JEnumWrapper>("JEnumWrapper");
                if(!qtName.isEmpty()){
                    QMetaType::registerNormalizedTypedef(qtName.toLatin1(), id);
                }
                return QMetaType::registerTypedef(javaName.toLatin1(), id);
            }else if(Java::Private::Runtime::Collection.isAssignableFrom(env, clazz)){
                int id = qRegisterMetaType<JCollectionWrapper>("JCollectionWrapper");
                return QMetaType::registerTypedef(javaName.toLatin1(), id);
            }else if(Java::Private::Runtime::Map.isAssignableFrom(env, clazz)){
                int id = qRegisterMetaType<JMapWrapper>("JMapWrapper");
                return QMetaType::registerTypedef(javaName.toLatin1(), id);
            }else if(Java::Private::Runtime::Iterator.isAssignableFrom(env, clazz)){
                int id = qRegisterMetaType<JIteratorWrapper>("JIteratorWrapper");
                return QMetaType::registerTypedef(javaName.toLatin1(), id);
            }else{
                int id = qRegisterMetaType<JObjectWrapper>("JObjectWrapper");
                return QMetaType::registerTypedef(javaName.toLatin1(), id);
            }
        }else{
            QString typeName;
            if(qtName.isEmpty())
                typeName = javaName;
            else
                typeName = qtName;
            int id = QMetaType::type(typeName.toLatin1());
            if(id==QMetaType::UnknownType){
                QMetaType::TypeFlags flags = QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction;
                const QMetaObject *meta_object = nullptr;
                const SuperTypeInfos& superTypeInfos = getSuperTypeInfos(env, clazz);
                if(!superTypeInfos.isEmpty() && Java::Private::QtCore::QObject.isAssignableFrom(env, superTypeInfos.first().javaClass)){
                    const QMetaObject *original_meta_object = registeredOriginalMetaObject(superTypeInfos.first().typeId());
                    meta_object = qtjambi_metaobject_for_class(env, clazz, original_meta_object);
                    flags |= QMetaType::PointerToQObject;
                }
                if(!qtName.isEmpty() && Java::Private::Runtime::Enum.isAssignableFrom(env, clazz)){
                    flags |= QMetaType::IsEnumeration;
                    int size = sizeof(void*);
                    if(size<=0){
                        enum E{};
                        size = sizeof(E);
                    }
                    return QMetaType::registerNormalizedType(
                            QMetaObject::normalizedType(typeName.toLatin1().constData()),
                            &destructHelper,
                            findConstructor(size),
                            size,
                            flags,
                            meta_object
                        );
                }else if(!qtName.isEmpty() && Java::Private::QtJambi::QFlags.isAssignableFrom(env, clazz)){
                    flags |= QMetaType::WasDeclaredAsMetaType;
                    int size = sizeof(void*);
                    if(size<=0){
                        enum E{};
                        size = sizeof(QFlags<E>);
                    }
                    return QMetaType::registerNormalizedType(
                            QMetaObject::normalizedType(typeName.toLatin1().constData()),
                            &destructHelper,
                            findConstructor(size),
                            size,
                            flags,
                            meta_object
                        );
                }else if(Java::Private::Runtime::Enum.isAssignableFrom(env, clazz)){
                    int last = javaName.lastIndexOf('$');
                    if(last>0){
                        javaName.replace(last, 1, "::");
                    }
                    int id = qRegisterMetaType<JEnumWrapper>("JEnumWrapper");
                    QMetaType::registerNormalizedTypedef(javaName.toLatin1(), id);
    //                if(!qtName.isEmpty()){
    //                    QMetaType::registerNormalizedTypedef(qtName.toLatin1(), id);
    //                }
                    return id;
                }
                int size = sizeof(void*);

                id = QMetaType::registerNormalizedType(
                        QMetaObject::normalizedType(typeName.toLatin1().constData()),
                        &destructHelper,
                        &pointerConstructHelper,
                        size,
                        flags,
                        meta_object
                    );
                if(typeName!=javaName)
                    id = QMetaType::registerTypedef(javaName.toLatin1(), id);
                Q_ASSERT(id!=0);
            }
            return id;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return 0;
}

extern "C" Q_DECL_EXPORT int JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1metaTypeId)
(JNIEnv *env, jclass, jclass clazz)
{
    int id = QMetaType::UnknownType;
    if(clazz){
        QString javaClassName = qtjambi_class_name(env, clazz).replace('.', '/');
        if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
            id = registeredMetaTypeID(*typeId);
        }
        if(id==QMetaType::UnknownType){
            QString qtName = QtJambiTypeManager::getInternalTypeName(env, javaClassName);
            id = QMetaType::type(qPrintable(qtName));
            if(id==QMetaType::UnknownType){
                id = QMetaType::type(qPrintable(javaClassName.replace("/", "::")));
            }
        }
    }else{
        id = QMetaType::Nullptr;
    }
    return id;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_isGeneratedClass)
(JNIEnv *env,
 jclass,
 jstring className)
{
    try{
        return getTypeByJavaName(qtjambi_to_qstring(env, className).replace(".", "/"))!=nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_lessThan)
(JNIEnv *env,
 jclass,
 jlong lessThanFunction,
 jobject k1,
 jobject k2)
{
    try{
        if (lessThanFunction){
            LessThanFunction lessThanFct = reinterpret_cast<LessThanFunction>(lessThanFunction);
            Q_ASSERT(lessThanFct);
            return lessThanFct(env, k1, k2);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_monitorEnter)
(JNIEnv *env,
 jclass,
 jobject object)
{
    try{
        return object && env->MonitorEnter(object)==0;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_monitorExit)
(JNIEnv *env,
 jclass,
 jobject object)
{
    try{
        return object && env->MonitorExit(object)==0;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_getListOfExtraSignal)
    (JNIEnv *env, jclass, QtJambiNativeID sender__id)
{
    try{
        if(QSharedPointer<QtJambiLink> sender = QtJambiLink::fromNativeId(sender__id)){
            if(sender->isSharedPointer()){
                QSharedPointerToQObjectLink* slink = static_cast<QSharedPointerToQObjectLink*>(sender.data());
                return slink->pointerContainer()->getListOfExtraSignal(env);
            }else{
                PointerToQObjectLink* plink = static_cast<PointerToQObjectLink*>(sender.data());
                return plink->getListOfExtraSignal(env);
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

class QClassPathFileEngineHandler: public QAbstractFileEngineHandler
{
public:
    QAbstractFileEngine *create(const QString &fileName) const
    {
        QAbstractFileEngine *rv;
        if (fileName.startsWith("classpath:"))
            rv = newClassPathFileEngine(fileName);
        else if (fileName.startsWith("/:classpath:"))
            rv = newClassPathFileEngine(fileName.mid(2));
        else
            rv = nullptr;
        return rv;
    }
private:
    QClassPathFileEngineHandler(){}
    ~QClassPathFileEngineHandler(){}
    QAbstractFileEngine *newClassPathFileEngine(const QString &fileName) const
    {
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            jstring javaFileName = qtjambi_from_qstring(env, fileName);
            jobject javaFileEngine = Java::Private::QtCore::QClassPathEngine.newInstance(env, javaFileName);
            QAbstractFileEngine * fileEngine = nullptr;
            QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, javaFileEngine);
            Q_ASSERT(link);
            if(link){
                link->setCppOwnership(env);
                fileEngine = reinterpret_cast<QAbstractFileEngine *>(link->pointer());
            }
            return fileEngine;
        }else return nullptr;
    }
    static QClassPathFileEngineHandler INSTANCE;
};

QClassPathFileEngineHandler QClassPathFileEngineHandler::INSTANCE;

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_objectCount)
(JNIEnv * env,
 jclass)
{
#if defined(QTJAMBI_DEBUG_TOOLS)
    return QtJambiLink::dumpLinks(env);
#else
    Q_UNUSED(env)
    return 0;
#endif
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_hasDeleteLaterEvents)
(JNIEnv *,
 jclass)
{
#if defined(QTJAMBI_DEBUG_TOOLS)
    return QtJambiLink::hasDeleteLaterEvents();
#else
    return false;
#endif
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_hasDebugTools)
(JNIEnv *,
 jclass)
{
#if defined(QTJAMBI_DEBUG_TOOLS) || !defined (QT_NO_DEBUG)
    return true;
#else
    return false;
#endif
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_flushOut)
(JNIEnv *,
 jclass)
{
    fflush(stdout);
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_flushErr)
(JNIEnv *,
 jclass)
{
    fflush(stderr);
}

int qtjambi_object_cache_mode_get();
void qtjambi_object_cache_mode_set(int object_cache_mode);

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_getObjectCacheMode)
(JNIEnv *,
 jclass)
{
    return qtjambi_object_cache_mode_get();
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_setObjectCacheMode)
(JNIEnv *,
 jclass,
 jint object_cache_mode)
{
    qtjambi_object_cache_mode_set(object_cache_mode);
}

void qtjambi_initialize_native_object(JNIEnv *env, jclass callingClass, jobject object, jobject link, jobject arguments);

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1initializeNativeObject)
(JNIEnv *env, jclass, jclass callingClass, jobject object, jobject link, jobject arguments)
{
    try{
        qtjambi_initialize_native_object(env, callingClass, object, link, arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_unregisterDependentObject)
(JNIEnv *, jclass, QtJambiNativeID dependentObject, QtJambiNativeID owner)
{
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->unregisterDependentObject(_dependentLink);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_registerDependentObject)
(JNIEnv *, jclass, QtJambiNativeID dependentObject, QtJambiNativeID owner)
{
    QSharedPointer<QtJambiLink> _dependentLink = QtJambiLink::fromNativeId(dependentObject);
    QSharedPointer<QtJambiLink> _ownerLink = QtJambiLink::fromNativeId(owner);
    if(_dependentLink && _ownerLink){
        _ownerLink->registerDependentObject(_dependentLink);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_getInterfaces)
(JNIEnv *env, jclass, jclass cls)
{
    try{
        return getSuperTypeInfos(env, cls).interfaceList(env);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return nullptr;
    }
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_countEventLoops)
(JNIEnv *env, jclass, QtJambiNativeID __thread_nativeId)
{
    try{
        QThread *__qt_this = qtjambi_object_from_nativeId<QThread>(__thread_nativeId);
        Q_ASSERT(__qt_this);
        return QThreadData::get2(__qt_this)->eventLoops.size();
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return 0;
    }
}

QObject* find_instance(const QWeakPointer<QtJambiLink>& weakPtr){
    if(QSharedPointer<QtJambiLink> instanceLink = weakPtr.toStrongRef()){
        return instanceLink->qobject();
    }
    return nullptr;
}

// emitting (writeExtraFunctions)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_qRegisterStaticPluginFunction)
(JNIEnv *__jni_env,
 jclass, QtJambiNativeID instance0, jobject metaData1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterStaticPluginFunction(QStaticPlugin)")
    try{
        QStaticPlugin staticPlugin;
        QSharedPointer<QtJambiLink> instanceLink = QtJambiLink::fromNativeId(instance0);
        Q_ASSERT(instanceLink);
        Q_ASSERT(instanceLink->isQObject());
        QWeakPointer<QtJambiLink> weakPtr = instanceLink.toWeakRef();
        jobject instance = instanceLink->getJavaObjectLocalRef(__jni_env);
        Q_ASSERT(instance);
        jclass cls = __jni_env->GetObjectClass(instance);
        Q_ASSERT(cls);
        QString className = qtjambi_class_name(__jni_env, cls);
        uint hash = uint(Java::Private::Runtime::System.identityHashCode(__jni_env,instance));
        staticPlugin.instance = qtjambi_function_pointer<16 /* = 1024 options */,QObject*()>(
                    [weakPtr]() -> QObject*{
                        return find_instance(weakPtr);
                    }
                , hash);
        QJsonObject json = qtjambi_cast<QJsonObject>(__jni_env, metaData1);
        QCborMap cborValue;
        if(const char* iid = getInterfaceIID(__jni_env, cls)){
            cborValue.insert(qint64(QtPluginMetaDataKeys::IID), iid);
        }
        cborValue.insert(qint64(QtPluginMetaDataKeys::ClassName), className);
        cborValue.insert(qint64(QtPluginMetaDataKeys::MetaData), QCborMap::fromJsonObject(json));
        QByteArray cborData = cborValue.toCborValue().toCbor();
        QByteArray rawMetaData;
        rawMetaData.reserve(16 + cborData.size());
        rawMetaData.append("QTMETADATA !");
        rawMetaData.append(char(0));
        rawMetaData.append(char(QT_VERSION_MAJOR));
        rawMetaData.append(char(QT_VERSION_MINOR));
        rawMetaData.append(char(qPluginArchRequirements()));
        rawMetaData.append(cborData);
        staticPlugin.rawMetaData = qtjambi_function_pointer<16 /* = 1024 options */,const char*()>(
                    [rawMetaData]() -> const char*{
                        return rawMetaData;
                    }
                , qHash(rawMetaData));
        qRegisterStaticPluginFunction(staticPlugin);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_00024NativeLink_clean)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            link->onClean(env);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_00024NativeLink_dispose)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            link->onDispose(env);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_00024NativeLink_qtTypeName)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME) || !defined(QT_NO_DEBUG)
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            return env->NewStringUTF(link->qtTypeName());
        }
#else
        Q_UNUSED(native_id)
#endif
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
