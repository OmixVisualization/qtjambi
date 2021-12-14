/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QVarLengthArray>
#include <QtCore/QPointer>
#include <QtCore/QStringList>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QThread>
#include <QtCore/QStaticPlugin>
#include <QtCore/QCborMap>
#include <QtCore/QCborValue>

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
#include <QtCore/QLinkedList>
#include <QtCore/QVector>
#endif

#include <QtCore/private/qcoreapplication_p.h>
#include <QtCore/private/qabstractfileengine_p.h>
#include <QtCore/private/qplugin_p.h>
#include "qtjambi_core.h"
#include "qtjambi_jobjectwrapper.h"
#include "qtjambivariant_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_thread.h"
#include "qtjambi_thread_p.h"
#include "qtjambi_interfaces_p.h"
#include "qtjambitypemanager_p.h"
#include "qtjambi_functionpointer.h"
#include "qtjambi_registry_p.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_qml.h"
#include "qtjambi_cast.h"
#include "qtjambi_containeraccess_p.h"
#include "qtjambi_application.h"
#include "qtjambi_nativeinterface.h"

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

#define EXCLUDE_GT_END(strg) strg //.endsWith(">") ? strg+" " : strg

JNIEnv* qtjambi_current_environment(bool);

#ifdef Q_OS_WIN
#include <Windows.h>
# endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_AbstractMetaObjectTools_getPropertyForField)
    (JNIEnv *env, jclass, jobject jmetaObject, jobject reflectField)
{
    const QMetaObject* metaObject = qtjambi_cast<const QMetaObject*>(env, jmetaObject);
    if(metaObject){
        jfieldID field = env->FromReflectedField(reflectField);
        for(int i=0; i<metaObject->propertyCount(); ++i){
            QMetaProperty prop = metaObject->property(i);
            if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(prop.enclosingMetaObject())){
                if(field==dynamicMetaObject->getQPropertyField(prop.relativePropertyIndex())){
                    return qtjambi_cast<jobject>(env, prop);
                }
            }
        }
    }
    return qtjambi_cast<jobject>(env, QMetaProperty());
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_AbstractMetaObjectTools_registerPropertyField)
    (JNIEnv *env, jclass, QtJambiNativeID metaPropertyId, jobject reflectField)
{
    QMetaProperty property = qtjambi_value_from_nativeId<QMetaProperty>(metaPropertyId);
    if(property.isValid()){
        if(const QtJambiMetaObject* dynamicMetaObject = QtJambiMetaObject::cast(property.enclosingMetaObject())){
            jfieldID field = env->FromReflectedField(reflectField);
            const_cast<QtJambiMetaObject*>(dynamicMetaObject)->registerQPropertyField(property.relativePropertyIndex(), field);
        }
    }
}
# endif

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_disconnectConnection)
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

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_initializeMultiSignal)
(JNIEnv *env, jclass, jobject multiSignal, jclass declaringClass, jobject reflectedField)
{
    try{
        const QMetaObject* mo = qtjambi_metaobject_for_class(env, declaringClass, nullptr);
        QVector<QtJambiMetaObject::SignalInfo> signalInfos = QtJambiMetaObject::signalInfos(mo, env->FromReflectedField(reflectedField));
        if(!signalInfos.isEmpty()){
            jintArray methodIndexes = env->NewIntArray(jsize(signalInfos.size()));
            jobjectArray signalParameterTypes = env->NewObjectArray(jsize(signalInfos.size()), Java::Runtime::List::getClass(env), nullptr);
            jobjectArray signalObjectTypes = env->NewObjectArray(jsize(signalInfos.size()), Java::Runtime::Class::getClass(env), nullptr);
            {
                JIntArrayPointer methodIndexesPtr(env, methodIndexes, true);
                for(int i=0; i<signalInfos.size(); i++){
                    const QtJambiMetaObject::SignalInfo& info = signalInfos.at(i);
                    env->SetObjectArrayElement(signalObjectTypes, jsize(i), info.signalClass);
                    env->SetObjectArrayElement(signalParameterTypes, jsize(i), info.signalTypes);
                    methodIndexesPtr.pointer()[i] = info.methodIndex;
                }
            }
            Java::QtJambi::QtJambiSignals$AbstractMultiSignal::initializeSignals(env, multiSignal, reflectedField, methodIndexes, signalParameterTypes, signalObjectTypes);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jlong JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_fromReflectedMethod)
(JNIEnv *env, jclass, jobject method)
{
    return jlong(env->FromReflectedMethod(method));
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_signalInfo)
    (JNIEnv *env, jclass, jobject _metaObject, jobject field, jobject emitMethod)
{
    try{
        if(emitMethod){
            QtJambiMetaObject::SignalInfo signalInfo = QtJambiMetaObject::signalInfo(qtjambi_cast<const QMetaObject*>(env, _metaObject), env->FromReflectedField(field), env->FromReflectedMethod(emitMethod));
            return Java::QtJambi::QtJambiSignals$SignalInfo::newInstance(env, signalInfo.methodIndex, signalInfo.signalTypes);
        }else{
            QVector<QtJambiMetaObject::SignalInfo> signalInfos = QtJambiMetaObject::signalInfos(qtjambi_cast<const QMetaObject*>(env, _metaObject), env->FromReflectedField(field));
            QtJambiMetaObject::SignalInfo result;
            for(const QtJambiMetaObject::SignalInfo& signalInfo : signalInfos){
                // in case of default-arg signals take the smallest methodIndex. all subsequent methodIndexes are clones.
                if(result.methodIndex==-1 || result.methodIndex>signalInfo.methodIndex){
                    result = signalInfo;
                }
            }
            return Java::QtJambi::QtJambiSignals$SignalInfo::newInstance(env, result.methodIndex, result.signalTypes);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return nullptr;
    }
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
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024CurrentSenderSetter_setQObjectSender)
(JNIEnv * env, jobject, QtJambiNativeID r, QtJambiNativeID s)
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
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiSignals_00024CurrentSenderSetter_resetQObjectSender)
(JNIEnv *env, jobject, QtJambiNativeID r, jlong keep)
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

typedef QHash<jint,jfieldID> EnumClassValuesHash;
typedef QHash<jint,jboolean> EnumClassExtensibleHash;
Q_GLOBAL_STATIC(EnumClassValuesHash, enumClassValuesFields)
Q_GLOBAL_STATIC(EnumClassExtensibleHash, enumClassExtensible)

jboolean isClassExtensible(JNIEnv *env, jint hashCode, jclass enumClass){
    if(!enumClassExtensible->contains(hashCode)){
        (*enumClassExtensible)[hashCode] = Java::Runtime::Class::isAnnotationPresent(env, enumClass, Java::QtJambi::QtExtensibleEnum::getClass(env));
    }
    return (*enumClassExtensible)[hashCode];
}

jfieldID findValueField(jthrowable& t, JNIEnv *env, jint hashCode, jclass enumClass){
    jfieldID valuesField = enumClassValuesFields->value(hashCode, nullptr);
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
            (*enumClassValuesFields)[hashCode] = valuesField;
    }
    return valuesField;
}

jobjectArray extendEnumValues(JNIEnv *env, jclass enumClass, jobjectArray values, int length, jobject newEntry){
    values = Java::Runtime::Arrays::copyOf(env, values, length+1);
    env->SetObjectArrayElement(values, length, newEntry);
    Java::QtJambi::QtJambiEnums::extendEnum(env, enumClass, values, newEntry);
    return values;
}

QString simpleClassName(JNIEnv *env, jclass enumClass){
    QString className = qtjambi_class_name(env, enumClass);
    auto idx = className.lastIndexOf('$');
    if(idx>=0){
        className = className.mid(idx);
    }
    return className;
}

jobject getRejectedEntries(JNIEnv *env, jclass enumClass){
    jobject rejectedEntries = nullptr;
    if(jobject qtRejectedEntries = Java::Runtime::Class::getAnnotation(env, enumClass, Java::QtJambi::QtRejectedEntries::getClass(env))){
        jobjectArray rejectedValues = Java::QtJambi::QtRejectedEntries::value(env, qtRejectedEntries);
        jobject set = Java::Runtime::HashSet::newInstance(env);
        jsize arrayLength = env->GetArrayLength(rejectedValues);
        for(jsize i=0; i<arrayLength; ++i){
            qtjambi_collection_add(env, set, env->GetObjectArrayElement(rejectedValues, i));
        }
        rejectedEntries = set;
    }
    return rejectedEntries;
}

template<size_t>
struct EnumResolver{
};

template<>
struct EnumResolver<1>{
    constexpr static const char* SIGNATURE = "(Ljava/lang/String;IB)V";
    static jbyte value(JNIEnv *env, jobject entry){
        return Java::QtJambi::QtByteEnumerator::value(env, entry);
    }
};

template<>
struct EnumResolver<2>{
    constexpr static const char* SIGNATURE = "(Ljava/lang/String;IS)V";
    static jshort value(JNIEnv *env, jobject entry){
        return Java::QtJambi::QtShortEnumerator::value(env, entry);
    }
};

template<>
struct EnumResolver<4>{
    constexpr static const char* SIGNATURE = "(Ljava/lang/String;II)V";
    static jint value(JNIEnv *env, jobject entry){
        return Java::QtJambi::QtEnumerator::value(env, entry);
    }
};

template<>
struct EnumResolver<8>{
    constexpr static const char* SIGNATURE = "(Ljava/lang/String;IJ)V";
    static jlong value(JNIEnv *env, jobject entry){
        return Java::QtJambi::QtLongEnumerator::value(env, entry);
    }
};

template<class jtype, template <typename,typename> class Map>
jobject resolveEnum(JNIEnv *env, jint hashCode, jclass enumClass, jtype value, jstring entryName){
    if(Java::Runtime::Enum::isAssignableFrom(env, enumClass)){
        static QReadWriteLock* mutex = new QReadWriteLock();
        QWriteLocker locker(mutex);
        Q_UNUSED(locker)

        static QHash<jint, Map<jtype,jsize> > cachesByEnum = QHash<jint, Map<jtype,jsize> >();
        jobject object = nullptr;
        jthrowable t = nullptr;
        jfieldID valuesField = findValueField(t, env, hashCode, enumClass);
        if(valuesField){
            jobjectArray values = jobjectArray(env->GetStaticObjectField(enumClass, valuesField));
            qtjambi_throw_java_exception(env);
            jsize length = env->GetArrayLength(values);

            if(!cachesByEnum.contains(hashCode)){
                Map<jtype,jsize> indexesByValue;
                jobject rejectedEntries = getRejectedEntries(env, enumClass);
                for(jsize i=0; i<length; ++i){
                    jobject entry = env->GetObjectArrayElement(values, i);
                    if(!rejectedEntries || ![env,rejectedEntries,entry]()->jboolean{
                            jstring entry_name = Java::Runtime::Enum::name(env,entry);
                            return Java::Runtime::Collection::contains(env, rejectedEntries, entry_name);
                        }()){
                        jtype v = EnumResolver<sizeof(jtype)>::value(env,entry);
                        if(!indexesByValue.contains(v))
                            indexesByValue.insert(v, i);
                    }
                }
                cachesByEnum.insert(hashCode, indexesByValue);
            }
            Map<jtype,jsize> &indexesByValue = cachesByEnum[hashCode];
            jsize index = indexesByValue.value(value, -1);
            if(index<0 && isClassExtensible(env, hashCode, enumClass)){
                jmethodID methodId = resolveMethod(env, "<init>", EnumResolver<sizeof(jtype)>::SIGNATURE, enumClass);
                Q_ASSERT(methodId);
                QString className = simpleClassName(env, enumClass);
                object = env->NewObject(enumClass, methodId, entryName ? entryName : qtjambi_from_qstring(env, QString("Custom%1(%2)").arg(className).arg(value)), length, value);
                qtjambi_throw_java_exception(env);
                values = extendEnumValues(env, enumClass, values, length, object);
                indexesByValue.insert(value, length);
                index = length;
                ++length;
                env->SetStaticObjectField(enumClass, valuesField, values);
                qtjambi_throw_java_exception(env);
            }
            if(index>=0 && index<length){
                object = env->GetObjectArrayElement(values, index);
                qtjambi_throw_java_exception(env);
            }
        }else if(t){
            JavaException(env, t).raise( QTJAMBI_STACKTRACEINFO_ENV(env) );
        }
        return object;
    }else{
        return nullptr;
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
        qtjambi_throw_java_exception(env);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
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
            return Java::Runtime::RuntimeException::newInstanceWithMessage(env, exn->what());
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
        static jmethodID constructor = [](JNIEnv *env) -> jmethodID {
            QString version = qtjambi_to_qstring(env, Java::Runtime::System::getProperty(env, env->NewStringUTF("java.version")));
            jmethodID result(nullptr);
            if(version.startsWith("1.")){
                result = env->GetMethodID(Java::Runtime::MethodHandles$Lookup::getClass(env), "<init>", "(Ljava/lang/Class;I)V");
                if(env->ExceptionCheck()){
                    env->ExceptionClear();
                }
            }
            if(!result){
                result = env->GetMethodID(Java::Runtime::MethodHandles$Lookup::getClass(env), "<init>", "(Ljava/lang/Class;)V");
            }
            return result;
        }(env);
        jobject result = env->NewObject(Java::Runtime::MethodHandles$Lookup::getClass(env), constructor, targetClass, 0xf);
        JavaException::check(env);
        return result;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

jobject qtjambi_invoke_method(JNIEnv *env,
                            jobject receiver,
                            jmethodID methodId,
                            jboolean isStatic,
                            jbyte returnType,
                            jobjectArray args,
                            jbyteArray _cnvTypes)
{
    jsize len = env->GetArrayLength(_cnvTypes);
    jbyte *cnvTypes = env->GetByteArrayElements(_cnvTypes, nullptr);
    QVarLengthArray<jvalue> argsArray(len);
    Q_ASSERT(args);
    for (int i=0; i<len; ++i) {
        jobject arg_object = env->GetObjectArrayElement(args, i);
        switch (cnvTypes[i]) {
        case 'L': argsArray[i].l = arg_object; break ;
        case 'Z': argsArray[i].z = qtjambi_to_boolean(env, arg_object); break ;
        case 'J': argsArray[i].j = qtjambi_to_long(env, arg_object); break ;
        case 'I': argsArray[i].i = qtjambi_to_int(env, arg_object); break ;
        case 'F': argsArray[i].f = qtjambi_to_float(env, arg_object); break ;
        case 'D': argsArray[i].d = qtjambi_to_double(env, arg_object); break ;
        case 'S': argsArray[i].s = qtjambi_to_short(env, arg_object); break ;
        case 'B': argsArray[i].b = qtjambi_to_byte(env, arg_object); break ;
        case 'C': argsArray[i].c = qtjambi_to_jchar(env, arg_object); break ;
        default:
            Q_ASSERT_X(false, "qtjambi_invoke_slot", "Error in conversion array");
        }
    }
    env->ReleaseByteArrayElements(_cnvTypes, cnvTypes, JNI_ABORT);
    jobject result = nullptr;
    if(isStatic){
        jclass clazz = jclass(receiver);
        switch (returnType)
        {
        case 'L': result = env->CallStaticObjectMethodA(clazz, methodId, argsArray.data()); break;
        case 'V': env->CallStaticVoidMethodA(clazz, methodId, argsArray.data()); break;
        case 'I': result = qtjambi_from_int(env, env->CallStaticIntMethodA(clazz, methodId, argsArray.data())); break;
        case 'J': result = qtjambi_from_long(env, env->CallStaticLongMethodA(clazz, methodId, argsArray.data())); break;
        case 'S': result = qtjambi_from_short(env, env->CallStaticShortMethodA(clazz, methodId, argsArray.data())); break;
        case 'Z': result = qtjambi_from_boolean(env, env->CallStaticBooleanMethodA(clazz, methodId, argsArray.data())); break;
        case 'F': result = qtjambi_from_float(env, env->CallStaticFloatMethodA(clazz, methodId, argsArray.data())); break;
        case 'D': result = qtjambi_from_double(env, env->CallStaticDoubleMethodA(clazz, methodId, argsArray.data())); break;
        case 'B': result = qtjambi_from_byte(env, env->CallStaticByteMethodA(clazz, methodId, argsArray.data())); break;
        case 'C': result = qtjambi_from_char(env, env->CallStaticCharMethodA(clazz, methodId, argsArray.data())); break;
        default:
            Q_ASSERT_X(false, "qtjambi_invoke_slot", "Invalid return type parameter");
            break;
        }
    }else{
        switch (returnType)
        {
        case 'L': result = env->CallObjectMethodA(receiver, methodId, argsArray.data()); break;
        case 'V': env->CallVoidMethodA(receiver, methodId, argsArray.data()); break;
        case 'I': result = qtjambi_from_int(env, env->CallIntMethodA(receiver, methodId, argsArray.data())); break;
        case 'J': result = qtjambi_from_long(env, env->CallLongMethodA(receiver, methodId, argsArray.data())); break;
        case 'S': result = qtjambi_from_short(env, env->CallShortMethodA(receiver, methodId, argsArray.data())); break;
        case 'Z': result = qtjambi_from_boolean(env, env->CallBooleanMethodA(receiver, methodId, argsArray.data())); break;
        case 'F': result = qtjambi_from_float(env, env->CallFloatMethodA(receiver, methodId, argsArray.data())); break;
        case 'D': result = qtjambi_from_double(env, env->CallDoubleMethodA(receiver, methodId, argsArray.data())); break;
        case 'B': result = qtjambi_from_byte(env, env->CallByteMethodA(receiver, methodId, argsArray.data())); break;
        case 'C': result = qtjambi_from_char(env, env->CallCharMethodA(receiver, methodId, argsArray.data())); break;
        default:
            Q_ASSERT_X(false, "qtjambi_invoke_slot", "Invalid return type parameter");
            break;
        }
    }
    qtjambi_throw_java_exception(env);
    return result;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_invokeMethod)
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
        return qtjambi_invoke_method(env, receiver, methodId, isStatic, returnType, args, cnvTypes);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
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
            QtJambiLink::Ownership ownership = link->ownership();
            return jbyte(ownership);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return -1;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_needsReferenceCounting)
    (JNIEnv *env, jclass, QtJambiNativeID objId)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(objId)) {
            return link->createdByJava() || link->ownership()==QtJambiLink::Ownership::Java;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
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
                }else if(auto containerAccess = link->containerAccess()){
                    const QObject* owner = containerAccess->getOwner(link->pointer());
                    return qtjambi_cast<jobject>(env, owner);
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

void qtjambi_shutdown(JNIEnv * env);

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambi_1LibraryUtilities_shutdown)
(JNIEnv * env, jclass)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QtJambi_LibraryUtilities::shutdown()")
    try{
        if(QThread* mainThread = QCoreApplicationPrivate::theMainThread.loadRelaxed()){
            QThread* currentThread = QThread::currentThread();
            if(mainThread==currentThread){
                if(QCoreApplication::instance() && !Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.no-app-deletion"))){
                    delete QCoreApplication::instance();
                }
            }
        }
        qtjambi_shutdown(env);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_registerQmlListProperty)
(JNIEnv *env, jclass, jstring type)
{
    try{
        QByteArray _type = qtjambi_to_qstring(env, type).toUtf8();
        int id = QMetaType::type("QQmlListProperty<QObject>");
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
        id = QMetaType::registerNormalizedTypedef(_type, id);
#else
        QMetaType t(id);
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

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_internalTypeName)
(JNIEnv *env, jclass, jstring s, jobject classLoader)
{
    try{
        QString signature = qtjambi_to_qstring(env, s);

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

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_internalTypeNameByClass)
(JNIEnv *env, jclass, jclass cls)
{
    try{
        QString result = QtJambiTypeManager::getInternalTypeName(env, cls);
        return qtjambi_from_qstring(env, result);
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

typedef void*(*ConstructorFn)(void*, const void*);
ConstructorFn findConstructor(int size){
    switch(size){
    case 1: return &constructHelper<1>;
    case 2: return &constructHelper<2>;
    case 4: return &constructHelper<4>;
    case 8: return &constructHelper<8>;
    default:
        return nullptr;
    }
}

typedef void(*DestructorFn)(void*);
void destructHelper(void *){}

void * pointerConstructHelper(void * where, const void *pointer)
{
    if(pointer){
        *reinterpret_cast<void**>(where) = *reinterpret_cast<void**>(const_cast<void*>(pointer));
    }
    return where;
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void registerJavaClassForCustomMetaType(int metaType, const QByteArray& javaClass);
#else
void registerJavaClassForCustomMetaType(const QMetaType& metaType, const QByteArray& javaClass);
void registerConverterVariant(JNIEnv *env, const QMetaType& metaType, const QString& qtName, const QString& fullJavaName, jclass clazz);
#endif

int qtjambi_register_metatype(JNIEnv *env, jclass clazz, jboolean isPointer, jboolean isReference)
{
    try{
        if(Java::Runtime::Class::isArray(env, clazz) && !isPointer && !isReference){
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
                return registerMetaType<JObjectWrapper>("JObjectWrapper");
            }
        }else if((Java::Runtime::Integer::isSameClass(env, clazz) || Java::Runtime::Integer::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::Int;
        }else if((Java::Runtime::Long::isSameClass(env, clazz) || Java::Runtime::Long::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::LongLong;
        }else if((Java::Runtime::Short::isSameClass(env, clazz) || Java::Runtime::Short::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::Short;
        }else if((Java::Runtime::Byte::isSameClass(env, clazz) || Java::Runtime::Byte::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::SChar;
        }else if((Java::Runtime::Boolean::isSameClass(env, clazz) || Java::Runtime::Boolean::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::Bool;
        }else if((Java::Runtime::Character::isSameClass(env, clazz) || Java::Runtime::Character::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::QChar;
        }else if((Java::Runtime::Float::isSameClass(env, clazz) || Java::Runtime::Float::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::Float;
        }else if((Java::Runtime::Double::isSameClass(env, clazz) || Java::Runtime::Double::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::Double;
        }else if((Java::Runtime::Void::isPrimitiveType(env, clazz)) && !isPointer && !isReference){
            return QMetaType::Void;
        }else{
            QString javaClassName = qtjambi_class_name(env, clazz).replace('.', '/');
            if(const std::type_info* typeId = getTypeByJavaName(javaClassName)){
                if(!isPointer && !isReference){
                    int id = registeredMetaTypeID(*typeId);
                    if(id!=QMetaType::UnknownType)
                        return id;
                }
                QByteArray qtType = getQtName(*typeId);
                const QMetaObject *meta_object = registeredOriginalMetaObject(*typeId);
                QMetaType::TypeFlags flags = QMetaType::MovableType | QMetaType::NeedsDestruction | QMetaType::NeedsConstruction;
                QByteArray typeName = QMetaObject::normalizedType(qtType);
                EntryTypes entryType = getEntryType(*typeId);
                if(isReference){
                    switch(entryType){
                    case EntryTypes::ObjectTypeInfo:
                    case EntryTypes::QObjectTypeInfo:
                        if(typeName.endsWith("*")){
                            typeName.chop(1);
                        }
                        if(!typeName.endsWith("&")){
                            typeName = QMetaObject::normalizedType(typeName + "&");
                        }
                        entryType = EntryTypes::ValueTypeInfo; break;
                    case EntryTypes::InterfaceTypeInfo:
                        if(typeName.endsWith("*")){
                            typeName.chop(1);
                        }
                        if(!typeName.endsWith("&")){
                            typeName = QMetaObject::normalizedType(typeName + "&");
                        }
                        entryType = EntryTypes::InterfaceValueTypeInfo; break;
                    default: break;
                    }
                }else if(isPointer){
                    switch(entryType){
                    case EntryTypes::ValueTypeInfo:
                        entryType = EntryTypes::ObjectTypeInfo; break;
                    case EntryTypes::InterfaceValueTypeInfo:
                        entryType = EntryTypes::InterfaceTypeInfo; break;
                    default: break;
                    }
                }
                switch(entryType){
                case EntryTypes::ObjectTypeInfo:
                case EntryTypes::QObjectTypeInfo:
                case EntryTypes::InterfaceTypeInfo:
                case EntryTypes::FunctionalTypeInfo: {
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
                    flags |= QMetaType::IsPointer;
#endif
                    if(entryType!=EntryTypes::FunctionalTypeInfo && !typeName.endsWith("*")){
                        typeName = QMetaObject::normalizedType(typeName + "*");
                    }
                    if(Java::QtCore::QObject::isAssignableFrom(env, clazz)){
                        flags |= QMetaType::PointerToQObject;
                    }else if(meta_object){
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
                            meta_object
                        );
                    qtjambi_register_comparator(new QtPrivate::BuiltInComparatorFunction<void*>(), typeId);
                    registerJavaClassForCustomMetaType(typeId, javaClassName.toLatin1());
                    return typeId;
#else
                    QMetaType metaType = createMetaType(typeName,
                                                        true,
                                                        /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                        /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                        /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                        /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                        /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                        /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                        /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                        /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                        /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                        /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                        /*.size=*/ sizeof(void*),
                                                        /*.alignment=*/ alignof(void*),
                                                        /*.typeId=*/ QMetaType::UnknownType,
                                                        /*.flags=*/ QMetaType::TypeFlags(flags),
                                                        meta_object,
                                                        nullptr);
                    metaType.id();
                    registerJavaClassForCustomMetaType(metaType, javaClassName.toLatin1());
                    registerConverterVariant(env, metaType, typeName, javaClassName, clazz);
                    return metaType.id();
#endif
                }
                case EntryTypes::EnumTypeInfo:{
                    flags |= QMetaType::IsEnumeration;
                    if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, clazz)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int typeId = QMetaType::registerNormalizedType(
                                typeName,
                                &destructHelper,
                                findConstructor(sizeof(qint16)),
                                sizeof(sizeof(qint16)),
                                flags,
                                meta_object
                            );
                        qtjambi_register_comparator(new QtPrivate::BuiltInComparatorFunction<qint16>(), typeId);
                        registerJavaClassForCustomMetaType(typeId, javaClassName.toLatin1());
                        return typeId;
#else
                        QMetaType metaType = createMetaType(typeName,
                                                            true,
                                                            /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<qint16>::getDefaultCtr(),
                                                            /*.copyCtr=*/ QtPrivate::QMetaTypeForType<qint16>::getCopyCtr(),
                                                            /*.moveCtr=*/ QtPrivate::QMetaTypeForType<qint16>::getMoveCtr(),
                                                            /*.dtor=*/ QtPrivate::QMetaTypeForType<qint16>::getDtor(),
                                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<qint16>::equals,
                                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<qint16>::lessThan,
                                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<qint16>::debugStream,
                                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<qint16>::dataStreamOut,
                                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<qint16>::dataStreamIn,
                                                            /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<qint16>::getLegacyRegister(),
                                                            /*.size=*/ sizeof(qint16),
                                                            /*.alignment=*/ alignof(qint16),
                                                            /*.typeId=*/ QMetaType::UnknownType,
                                                            /*.flags=*/ QMetaType::TypeFlags(flags),
                                                            meta_object,
                                                            nullptr);
                        int _id = metaType.id();
                        registerJavaClassForCustomMetaType(metaType, javaClassName.toLatin1());
                        return _id;
#endif
                    }else if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, clazz)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int typeId = QMetaType::registerNormalizedType(
                                typeName,
                                &destructHelper,
                                findConstructor(sizeof(qint64)),
                                sizeof(sizeof(qint64)),
                                flags,
                                meta_object
                            );
                        qtjambi_register_comparator(new QtPrivate::BuiltInComparatorFunction<qint64>(), typeId);
                        registerJavaClassForCustomMetaType(typeId, javaClassName.toLatin1());
                        return typeId;
#else
                        QMetaType metaType = createMetaType(typeName,
                                                            true,
                                                            /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<qint64>::getDefaultCtr(),
                                                            /*.copyCtr=*/ QtPrivate::QMetaTypeForType<qint64>::getCopyCtr(),
                                                            /*.moveCtr=*/ QtPrivate::QMetaTypeForType<qint64>::getMoveCtr(),
                                                            /*.dtor=*/ QtPrivate::QMetaTypeForType<qint64>::getDtor(),
                                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<qint64>::equals,
                                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<qint64>::lessThan,
                                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<qint64>::debugStream,
                                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<qint64>::dataStreamOut,
                                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<qint64>::dataStreamIn,
                                                            /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<qint64>::getLegacyRegister(),
                                                            /*.size=*/ sizeof(qint64),
                                                            /*.alignment=*/ alignof(qint64),
                                                            /*.typeId=*/ QMetaType::UnknownType,
                                                            /*.flags=*/ QMetaType::TypeFlags(flags),
                                                            meta_object,
                                                            nullptr);
                        int _id = metaType.id();
                        registerJavaClassForCustomMetaType(metaType, javaClassName.toLatin1());
                        return _id;
#endif
                    }else if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, clazz)){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                        int typeId = QMetaType::registerNormalizedType(
                                typeName,
                                &destructHelper,
                                findConstructor(sizeof(qint8)),
                                sizeof(sizeof(qint8)),
                                flags,
                                meta_object
                            );
                        qtjambi_register_comparator(new QtPrivate::BuiltInComparatorFunction<qint8>(), typeId);
                        registerJavaClassForCustomMetaType(typeId, javaClassName.toLatin1());
                        return typeId;
#else
                        QMetaType metaType = createMetaType(typeName,
                                                            true,
                                                            /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<qint8>::getDefaultCtr(),
                                                            /*.copyCtr=*/ QtPrivate::QMetaTypeForType<qint8>::getCopyCtr(),
                                                            /*.moveCtr=*/ QtPrivate::QMetaTypeForType<qint8>::getMoveCtr(),
                                                            /*.dtor=*/ QtPrivate::QMetaTypeForType<qint8>::getDtor(),
                                                            /*.equals=*/ QtPrivate::QEqualityOperatorForType<qint8>::equals,
                                                            /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<qint8>::lessThan,
                                                            /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<qint8>::debugStream,
                                                            /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<qint8>::dataStreamOut,
                                                            /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<qint8>::dataStreamIn,
                                                            /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<qint8>::getLegacyRegister(),
                                                            /*.size=*/ sizeof(qint8),
                                                            /*.alignment=*/ alignof(qint8),
                                                            /*.typeId=*/ QMetaType::UnknownType,
                                                            /*.flags=*/ QMetaType::TypeFlags(flags),
                                                            meta_object,
                                                            nullptr);
                        int _id = metaType.id();
                        registerJavaClassForCustomMetaType(metaType, javaClassName.toLatin1());
                        return _id;
#endif
                    }else /*if(Java::Runtime::Enum::isAssignableFrom(env, clazz)
                         || Java::QtJambi::QtEnumerator::isAssignableFrom(env, clazz))*/ {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                         int typeId = QMetaType::registerNormalizedType(
                                 typeName,
                                 &destructHelper,
                                 findConstructor(sizeof(qint32)),
                                 sizeof(sizeof(qint32)),
                                 flags,
                                 meta_object
                             );
                         qtjambi_register_comparator(new QtPrivate::BuiltInComparatorFunction<qint32>(), typeId);
                         registerJavaClassForCustomMetaType(typeId, javaClassName.toLatin1());
                         return typeId;
#else
                         QMetaType metaType = createMetaType(typeName,
                                                             true,
                                                             /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<qint32>::getDefaultCtr(),
                                                             /*.copyCtr=*/ QtPrivate::QMetaTypeForType<qint32>::getCopyCtr(),
                                                             /*.moveCtr=*/ QtPrivate::QMetaTypeForType<qint32>::getMoveCtr(),
                                                             /*.dtor=*/ QtPrivate::QMetaTypeForType<qint32>::getDtor(),
                                                             /*.equals=*/ QtPrivate::QEqualityOperatorForType<qint32>::equals,
                                                             /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<qint32>::lessThan,
                                                             /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<qint32>::debugStream,
                                                             /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<qint32>::dataStreamOut,
                                                             /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<qint32>::dataStreamIn,
                                                             /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<qint32>::getLegacyRegister(),
                                                             /*.size=*/ sizeof(qint32),
                                                             /*.alignment=*/ alignof(qint32),
                                                             /*.typeId=*/ QMetaType::UnknownType,
                                                             /*.flags=*/ QMetaType::TypeFlags(flags),
                                                             meta_object,
                                                             nullptr);
                         int _id = metaType.id();
                         registerJavaClassForCustomMetaType(metaType, javaClassName.toLatin1());
                         return _id;
#endif
                    }
                }
                case EntryTypes::FlagsTypeInfo:{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int typeId = QMetaType::registerNormalizedType(
                            typeName,
                            &destructHelper,
                            findConstructor(sizeof(qint32)),
                            sizeof(sizeof(qint32)),
                            flags,
                            meta_object
                        );
                    qtjambi_register_comparator(new QtPrivate::BuiltInComparatorFunction<qint32>(), typeId);
                    registerJavaClassForCustomMetaType(typeId, javaClassName.toLatin1());
                    return typeId;
#else
                    QMetaType metaType = createMetaType(typeName,
                                                        true,
                                                        /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<qint32>::getDefaultCtr(),
                                                        /*.copyCtr=*/ QtPrivate::QMetaTypeForType<qint32>::getCopyCtr(),
                                                        /*.moveCtr=*/ QtPrivate::QMetaTypeForType<qint32>::getMoveCtr(),
                                                        /*.dtor=*/ QtPrivate::QMetaTypeForType<qint32>::getDtor(),
                                                        /*.equals=*/ QtPrivate::QEqualityOperatorForType<qint32>::equals,
                                                        /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<qint32>::lessThan,
                                                        /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<qint32>::debugStream,
                                                        /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<qint32>::dataStreamOut,
                                                        /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<qint32>::dataStreamIn,
                                                        /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<qint32>::getLegacyRegister(),
                                                        /*.size=*/ sizeof(qint32),
                                                        /*.alignment=*/ alignof(qint32),
                                                        /*.typeId=*/ QMetaType::UnknownType,
                                                        /*.flags=*/ QMetaType::TypeFlags(flags),
                                                        meta_object,
                                                        nullptr);
                    int _id = metaType.id();
                    registerJavaClassForCustomMetaType(metaType, javaClassName.toLatin1());
                    return _id;
#endif
                }
                case EntryTypes::InterfaceValueTypeInfo:
                case EntryTypes::ValueTypeInfo:{
                    int id = qRegisterMetaType<JObjectWrapper>("JObjectWrapper");
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int typeId = QMetaType::registerNormalizedTypedef(typeName, id);
                    qtjambi_register_comparator(new QtPrivate::BuiltInEqualsComparatorFunction<JObjectWrapper>(), typeId);
                    registerJavaClassForCustomMetaType(typeId, javaClassName.toLatin1());
                    return typeId;
#else
                    QMetaType::registerNormalizedTypedef(typeName, QMetaType(id));
                    return id;
#endif
                }
                default: break;
                }
            }else{
                const SuperTypeInfos& superTypes = getSuperTypeInfos(env, clazz);
                QByteArray javaName = javaClassName.replace("/", "::").toUtf8();
                if(superTypes.isEmpty()){
                    int id;
                    if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(env, clazz)
                            || Java::QtJambi::QtLongEnumerator::isAssignableFrom(env, clazz)
                            || Java::QtJambi::QtByteEnumerator::isAssignableFrom(env, clazz)
                            || Java::QtJambi::QtEnumerator::isAssignableFrom(env, clazz)
                            || Java::Runtime::Enum::isAssignableFrom(env, clazz)){
                        id = qRegisterMetaType<JEnumWrapper>("JEnumWrapper");
                    }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                         id = qRegisterMetaType<JMapWrapper>("JMapWrapper");
                    }else if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                         id = qRegisterMetaType<JCollectionWrapper>("JCollectionWrapper");
                    }else if(Java::Runtime::Iterator::isAssignableFrom(env, clazz)){
                         id = qRegisterMetaType<JIteratorWrapper>("JIteratorWrapper");
                    }else{
                        id = qRegisterMetaType<JObjectWrapper>("JObjectWrapper");
                    }
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int typeId = QMetaType::registerNormalizedTypedef(javaName, id);
                    qtjambi_register_comparator(new QtPrivate::BuiltInEqualsComparatorFunction<JObjectWrapper>(), typeId);
                    registerJavaClassForCustomMetaType(typeId, javaClassName.toLatin1());
                    return typeId;
#else
                        QMetaType::registerNormalizedTypedef(javaName, QMetaType(id));
                        return id;
#endif
                }else{
                    int id = qtjambi_register_metatype(env, superTypes[0].javaClass(), isPointer, isReference);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    int typeId = QMetaType::registerNormalizedTypedef(javaName, id);
                    qtjambi_register_comparator(new QtPrivate::BuiltInEqualsComparatorFunction<JObjectWrapper>(), typeId);
                    return typeId;
#else
                    QMetaType::registerNormalizedTypedef(javaName, QMetaType(id));
                    return id;
#endif
                }
            }
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return QMetaType::UnknownType;
}

/**
 * new implementation according to Qt5's meta object API
 */
extern "C" Q_DECL_EXPORT int JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1registerMetaType)
(JNIEnv *env, jclass, jclass clazz, jboolean isPointer, jboolean isReference)
{
    return qtjambi_register_metatype(env, clazz, isPointer, isReference);
}

extern "C" Q_DECL_EXPORT int JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1registerMetaType2)
(JNIEnv *, jclass, jint id, jboolean isPointer, jboolean isReference)
{
    QMetaType metaType(id);
    QByteArray typeName(metaType.name());
    if(isPointer){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if(!typeName.endsWith("*")){
#else
        if(!(metaType.flags() & QMetaType::IsPointer)){
#endif
            if(typeName.endsWith("&")){
                typeName.chop(1);
            }
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
            qtjambi_register_comparator(new QtPrivate::BuiltInComparatorFunction<void*>(), typeId);
            return typeId;
#else
            QMetaType _metaType = createMetaType(typeName,
                                                true,
                                                /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                /*.size=*/ sizeof(void*),
                                                /*.alignment=*/ alignof(void*),
                                                /*.typeId=*/ QMetaType::UnknownType,
                                                /*.flags=*/ QMetaType::TypeFlags(flags),
                                                metaType.metaObject(),
                                                nullptr);
            _metaType.id();
            return _metaType.id();
#endif
        }
    }else if(isReference){
        if(!typeName.endsWith("&")){
            if(typeName.endsWith("*")){
                typeName.chop(1);
            }
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
            qtjambi_register_comparator(new QtPrivate::BuiltInComparatorFunction<void*>(), typeId);
            return typeId;
#else
            QMetaType _metaType = createMetaType(typeName,
                                                true,
                                                /*.defaultCtr=*/ QtPrivate::QMetaTypeForType<void*>::getDefaultCtr(),
                                                /*.copyCtr=*/ QtPrivate::QMetaTypeForType<void*>::getCopyCtr(),
                                                /*.moveCtr=*/ QtPrivate::QMetaTypeForType<void*>::getMoveCtr(),
                                                /*.dtor=*/ QtPrivate::QMetaTypeForType<void*>::getDtor(),
                                                /*.equals=*/ QtPrivate::QEqualityOperatorForType<void*>::equals,
                                                /*.lessThan=*/ QtPrivate::QLessThanOperatorForType<void*>::lessThan,
                                                /*.debugStream=*/ QtPrivate::QDebugStreamOperatorForType<void*>::debugStream,
                                                /*.dataStreamOut=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamOut,
                                                /*.dataStreamIn=*/ QtPrivate::QDataStreamOperatorForType<void*>::dataStreamIn,
                                                /*.legacyRegisterOp=*/ QtPrivate::QMetaTypeForType<void*>::getLegacyRegister(),
                                                /*.size=*/ sizeof(void*),
                                                /*.alignment=*/ alignof(void*),
                                                /*.typeId=*/ QMetaType::UnknownType,
                                                /*.flags=*/ QMetaType::TypeFlags(flags),
                                                metaType.metaObject(),
                                                nullptr);
            _metaType.id();
            return _metaType.id();
#endif
        }
    }
    return id;
}

QByteArray registeredJavaClassForCustomMetaType(int metaType);

jclass qtjambi_class_for_metatype(JNIEnv *env, int id)
{
    try {
        QMetaType metaType(id);
        QByteArray typeName(metaType.name());
        const char* name(nullptr);
        if(const std::type_info* typeId = getTypeByMetaType(id)){
            name = getJavaName(*typeId);
        }
        if(!name){
            QByteArray name = registeredJavaClassForCustomMetaType(id);
            if(!name.isEmpty()){
                return resolveClass(env, name);
            }
            QString _name = QtJambiTypeManager::getExternalTypeName(env, QLatin1String(typeName), metaType);
            if(!_name.isEmpty()){
                return resolveClass(env, qPrintable(_name));
            }
            //
        }
        if(name){
            return resolveClass(env, name);
        }
        try {
            if(typeName.endsWith("*")){
                typeName.chop(1);
            }
            return resolveClass(env, typeName.replace("::", "."));
        }catch(const JavaException&){}
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1createMetaType)
(JNIEnv *env, jclass, jint id, jobject copy)
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#define QTJAMBI_METATYPE(id) id
#else
#define QTJAMBI_METATYPE(id) QMetaType(id)
#endif
    if(QByteArray(QMetaType::typeName(id)).endsWith("*")){
        return nullptr;
    }
    if(copy){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, copy)){
            QVariant variant(QTJAMBI_METATYPE(id), link->pointer());
            return qtjambi_from_qvariant(env, variant);
        }else{
            if(Java::QtJambi::QtObjectInterface::isInstanceOf(env, copy))
                Java::QtJambi::QNoNativeResourcesException::throwNew(env, QString("Incomplete object of type: %1").arg(qtjambi_object_class_name(env, copy).replace("$", ".")) QTJAMBI_STACKTRACEINFO );
            QVariant variant = qtjambi_to_qvariant(env, copy);
            if(variant.userType()!=id){
                if(variant.convert(id)){
                    variant = QVariant(QTJAMBI_METATYPE(id), variant.data());
                    return qtjambi_from_qvariant(env, variant);
                }
            }else{
                variant = QVariant(QTJAMBI_METATYPE(id), variant.data());
                return qtjambi_from_qvariant(env, variant);
            }
        }
    }
    QVariant variant(QTJAMBI_METATYPE(id), nullptr);
    return qtjambi_from_qvariant(env, variant);
}

extern "C" Q_DECL_EXPORT jclass JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_javaTypeForMetaTypeId)
(JNIEnv *env, jclass, jint id)
{
    return qtjambi_class_for_metatype(env, id);
}

extern "C" Q_DECL_EXPORT int JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1registerContainerMetaType)
(JNIEnv *env, jclass, jclass containerType, jintArray instantiations)
{
    const JIntArrayWrapper _instantiations(env, instantiations);
    QStringList names;
    const std::type_info* typeId = nullptr;
    switch(_instantiations.length()){
    case 1: {
        AbstractContainerAccess* containerAccess = nullptr;
        ContainerType type;
        if(Java::QtCore::QQueue::isAssignableFrom(env, containerType)){
            type = ContainerType::QQueue;
            typeId = &typeid(QQueue<QVariant>);
            names << "QQueue<%1>";
        }else if(Java::QtCore::QStack::isAssignableFrom(env, containerType)
                 || Java::Runtime::Deque::isAssignableFrom(env, containerType)){
            type = ContainerType::QStack;
            typeId = &typeid(QStack<QVariant>);
            names << "QStack<%1>";
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        }else if(Java::QtCore::QLinkedList::isAssignableFrom(env, containerType)){
            type = ContainerType::QLinkedList;
            typeId = &typeid(QLinkedList<QVariant>);
            names << "QLinkedList<%1>";
        }else if(Java::QtCore::QVector::isAssignableFrom(env, containerType)){
            type = ContainerType::QVector;
            typeId = &typeid(QVector<QVariant>);
            names << "QVector<%1>";
    #endif
        }else if(Java::QtCore::QList::isAssignableFrom(env, containerType)
                 || Java::Runtime::List::isAssignableFrom(env, containerType)){
            type = ContainerType::QList;
            typeId = &typeid(QList<QVariant>);
            names << "QList<%1>";
        }else if(Java::QtCore::QSet::isAssignableFrom(env, containerType)
                 || Java::Runtime::Set::isAssignableFrom(env, containerType)){
            type = ContainerType::QSet;
            typeId = &typeid(QSet<QVariant>);
            names << "QSet<%1>";
        }else{
            return qtjambi_register_metatype(env, containerType, false, false);
        }
        QString elementType = QLatin1String(QMetaType::typeName(_instantiations[0]));
        for(QString& name : names){
            name = name.arg(EXCLUDE_GT_END(elementType));
        }
        switch(_instantiations[0]){
        case QMetaType::Bool:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<bool>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<bool>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<bool>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<bool>::newInstance();
                break;
            }
            break;
        case QMetaType::Char:
        case QMetaType::SChar:
        case QMetaType::UChar:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<qint8>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<qint8>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<qint8>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<qint8>::newInstance();
                break;
            }
            break;
        case QMetaType::Short:
        case QMetaType::UShort:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<qint16>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<qint16>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<qint16>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<qint16>::newInstance();
                break;
            }
            break;
        case QMetaType::Int:
        case QMetaType::UInt:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<qint32>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<qint32>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<qint32>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<qint32>::newInstance();
                break;
            }
            break;
        case QMetaType::LongLong:
        case QMetaType::ULongLong:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<qint64>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<qint64>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<qint64>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<qint64>::newInstance();
                break;
            }
            break;
        case QMetaType::Double:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<double>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<double>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<double>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<double>::newInstance();
                break;
            }
            break;
        case QMetaType::Float:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<float>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<float>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<float>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<float>::newInstance();
                break;
            }
            break;
        case QMetaType::QChar:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<QChar>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<QChar>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<QChar>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<QChar>::newInstance();
                break;
            }
            break;
        case QMetaType::QVariant:
            switch(type){
            case ContainerType::QSet:
                // no qhash for qvariant
                // listAccess = QtJambiPrivate::QSetAccess<QVariant>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<QVariant>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<QVariant>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<QVariant>::newInstance();
                break;
            }
            break;
        case QMetaType::QString:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<QString>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<QString>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<QString>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<QString>::newInstance();
                break;
            }
            break;
        case QMetaType::QObjectStar:
            switch(type){
            case ContainerType::QSet:
                containerAccess = QtJambiPrivate::QSetAccess<QObject*>::newInstance();
                break;
            case ContainerType::QStack:
    #if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            case ContainerType::QVector:
                containerAccess = QtJambiPrivate::QVectorAccess<QObject*>::newInstance();
                break;
            case ContainerType::QLinkedList:
                containerAccess = QtJambiPrivate::QLinkedListAccess<QObject*>::newInstance();
                break;
    #endif
            case ContainerType::QQueue:
            case ContainerType::QList:
                containerAccess = QtJambiPrivate::QListAccess<QObject*>::newInstance();
                break;
            }
            break;
        default: break;
        }
        if(!containerAccess){
            jclass elementClass = qtjambi_class_for_metatype(env, _instantiations[0]);
            bool isPointer = elementType.endsWith("*");
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
                    if(_instantiations[0] != QMetaType::UnknownType){
                        int sz = QMetaType::sizeOf(_instantiations[0]);
                        size = sz<0 ? 0 : size_t(sz);
                    }
                    if(size==0){
                        size = QtJambiTypeManager::getInternalSize(elementType);
                    }
                    align = QtJambiTypeManager::getInternalAlignment(elementType);
                }
            }
            QMetaType metaType1(_instantiations[0]);
            InternalToExternalConverter memberConverter = QtJambiTypeManager::getInternalToExternalConverter(
                                                            env,
                                                            elementType,
                                                            metaType1,
                                                            elementClass);
            ExternalToInternalConverter memberReConverter = QtJambiTypeManager::getExternalToInternalConverter(
                                                            env,
                                                            elementClass,
                                                            elementType, metaType1);

            QHashFunction hashFunction = QtJambiTypeManager::findHashFunction(isPointer, _instantiations[0]);
            if(ContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(type, align, size
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                                                                                                  , isStaticType
#else
                                                                                                  , false
#endif
                                                                                                 )){
                containerAccess = accessFactory(QMetaType(_instantiations[0]),
                                                hashFunction,
                                                memberConverter,
                                                memberReConverter);
            }
        }
        int id = 0;
        for(QString& name : names){
            id = containerAccess->registerContainer(name.toLatin1());
        }
        containerAccess->dispose();
        if(typeId)
            registerMetaTypeID(*typeId, id);
        return id;
    }
    case 2: {
        AbstractContainerAccess* containerAccess = nullptr;
        MapType type;
        if(Java::QtCore::QPair::isAssignableFrom(env, containerType)){
            type = MapType::QPair;
            typeId = &typeid(QPair<QVariant,QVariant>);
            names << "QPair<%1,%2>" << "std::pair<%1,%2>";
        }else if(Java::QtCore::QMultiMap::isAssignableFrom(env, containerType)){
            type = MapType::QMultiMap;
            typeId = &typeid(QMultiMap<QVariant,QVariant>);
            names << "QMultiMap<%1,%2>";
        }else if(Java::QtCore::QMap::isAssignableFrom(env, containerType)
                 || Java::Runtime::NavigableMap::isAssignableFrom(env, containerType)){
            type = MapType::QMap;
            typeId = &typeid(QMap<QVariant,QVariant>);
            names << "QMap<%1,%2>";
        }else if(Java::QtCore::QMultiHash::isAssignableFrom(env, containerType)){
            type = MapType::QMultiHash;
            typeId = &typeid(QMultiHash<QVariant,QVariant>);
            names << "QMultiHash<%1,%2>";
        }else if(Java::QtCore::QHash::isAssignableFrom(env, containerType)
                 || Java::Runtime::Map::isAssignableFrom(env, containerType)){
            type = MapType::QHash;
            typeId = &typeid(QHash<QVariant,QVariant>);
            names << "QHash<%1,%2>";
        }else{
            return qtjambi_register_metatype(env, containerType, false, false);
        }
        QString keyType = QLatin1String(QMetaType::typeName(_instantiations[0]));
        QString valueType = QLatin1String(QMetaType::typeName(_instantiations[1]));
        for(QString& name : names){
            name = name.arg(keyType, EXCLUDE_GT_END(valueType));
        }

        jclass keyClass = qtjambi_class_for_metatype(env, _instantiations[0]);
        jclass valueClass = qtjambi_class_for_metatype(env, _instantiations[1]);
        bool isPointer1 = keyType.endsWith("*");
        size_t size1 = 0;
        size_t align1 = 0;
        bool isPointer2 = valueType.endsWith("*");
        size_t size2 = 0;
        size_t align2 = 0;
        if(!isPointer1){
            if(const std::type_info* t = getTypeByQtName(keyType)){
                size1 = getValueSize(*t);
                align1 = getValueAlignment(*t);
            }else{
                if(_instantiations[0] != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(_instantiations[0]);
                    size1 = sz<0 ? 0 : size_t(sz);
                }
                if(size1==0){
                    size1 = QtJambiTypeManager::getInternalSize(keyType);
                }
                align1 = QtJambiTypeManager::getInternalAlignment(keyType);
            }
        }
        if(!isPointer2){
            if(const std::type_info* t = getTypeByQtName(valueType)){
                size2 = getValueSize(*t);
                align2 = getValueAlignment(*t);
            }else{
                if(_instantiations[1] != QMetaType::UnknownType){
                    int sz = QMetaType::sizeOf(_instantiations[1]);
                    size2 = sz<0 ? 0 : size_t(sz);
                }
                if(size2==0){
                    size2 = QtJambiTypeManager::getInternalSize(valueType);
                }
                align2 = QtJambiTypeManager::getInternalAlignment(valueType);
            }
        }

        QMetaType metaType1(_instantiations[0]);
        QMetaType metaType2(_instantiations[1]);
        InternalToExternalConverter memberConverter1 = QtJambiTypeManager::getInternalToExternalConverter(
                                                        env,
                                                        keyType,
                                                        metaType1,
                                                        keyClass);
        ExternalToInternalConverter memberReConverter1 = QtJambiTypeManager::getExternalToInternalConverter(
                                                        env,
                                                        keyClass,
                                                        keyType, metaType1);

        InternalToExternalConverter memberConverter2 = QtJambiTypeManager::getInternalToExternalConverter(
                                                        env,
                                                        valueType,
                                                        metaType2,
                                                        valueClass);
        ExternalToInternalConverter memberReConverter2 = QtJambiTypeManager::getExternalToInternalConverter(
                                                        env,
                                                        valueClass,
                                                        valueType, metaType2);
        if(BiContainerAccessFactory accessFactory = ContainerAccessFactories::getAccessFactory(type, align1, size1, align2, size2)){
            QHashFunction hashFunction1 = QtJambiTypeManager::findHashFunction(isPointer1, _instantiations[0]);
            QHashFunction hashFunction2 = QtJambiTypeManager::findHashFunction(isPointer2, _instantiations[1]);
            containerAccess = accessFactory(QMetaType(_instantiations[0]),
                                            hashFunction1,
                                            memberConverter1,
                                            memberReConverter1,
                                            QMetaType(_instantiations[1]),
                                            hashFunction2,
                                            memberConverter2,
                                            memberReConverter2);
        }
        int id = 0;
        for(QString& name : names){
            id = containerAccess->registerContainer(name.toLatin1());
        }
        containerAccess->dispose();
        if(typeId)
            registerMetaTypeID(*typeId, id);
        return id;
    }
    }

    return qtjambi_register_metatype(env, containerType, false, false);
}

int qtjambi_metaTypeId(JNIEnv *env, jclass clazz)
{
    int id = QMetaType::UnknownType;
    if(clazz){
        if(Java::Runtime::Class::isArray(env, clazz)){
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
                return registerMetaType<JObjectWrapper>("JObjectWrapper");
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
            QString javaClassName = qtjambi_class_name(env, clazz).replace('.', '/');
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
                    id = QMetaType::type(qtType);
                }
            }
            if(id==QMetaType::UnknownType){
                id = QMetaType::type(qPrintable(javaClassName.replace("/", "::")));
            }
            if(id==QMetaType::UnknownType){
                if(Java::Runtime::Enum::isAssignableFrom(env, clazz)){
                    return registerMetaType<JEnumWrapper>("JEnumWrapper");
                }else if(Java::QtJambi::QFlags::isAssignableFrom(env, clazz)){
                    return registerMetaType<JObjectWrapper>("JObjectWrapper");
                }else if(Java::Runtime::Collection::isAssignableFrom(env, clazz)){
                    return registerMetaType<JCollectionWrapper>("JCollectionWrapper");
                }else if(Java::Runtime::Map::isAssignableFrom(env, clazz)){
                    return registerMetaType<JMapWrapper>("JMapWrapper");
                }else if(Java::Runtime::Iterator::isAssignableFrom(env, clazz)){
                    return registerMetaType<JIteratorWrapper>("JIteratorWrapper");
                }
            }
        }
    }else{
        id = QMetaType::Nullptr;
    }
    return id;
}

extern "C" Q_DECL_EXPORT int JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_findMetaType)
(JNIEnv *env, jclass, jstring name)
{
    const char* _name = env->GetStringUTFChars(name, nullptr);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    int result = QMetaType::type(_name);
#else
    int result = QMetaType::fromName(_name).id();
#endif
    env->ReleaseStringUTFChars(name, _name);
    return result;
}

extern "C" Q_DECL_EXPORT int JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1metaTypeId)
(JNIEnv *env, jclass, jclass clazz)
{
    return qtjambi_metaTypeId(env, clazz);
}

const std::type_info* getTypeByMetaType(int metaType);

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
    QAbstractFileEngine *create(const QString &fileName) const override
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
    ~QClassPathFileEngineHandler() override {}
    QAbstractFileEngine *newClassPathFileEngine(const QString &fileName) const
    {
        if(JNIEnv *env = qtjambi_current_environment()){
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            jstring javaFileName = qtjambi_from_qstring(env, fileName);
            jobject javaFileEngine = Java::QtCore::QClassPathEngine::newInstance(env, javaFileName);
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

void qtjambi_initialize_native_interface(JNIEnv *env, jclass callingClass, jobject object, jobject link, jobject arguments);

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal__1_1qt_1initializeNativeObject)
(JNIEnv *env, jclass, jclass callingClass, jobject object, jobject link, jobject arguments)
{
    try{
        qtjambi_initialize_native_interface(env, callingClass, object, link, arguments);
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

QList<const char*> getInterfaceIIDs(JNIEnv *env, jclass javaType);

void registerPlugin(QtPluginInstanceFunction instanceFunction, const QString& className, const QJsonObject& metaData, const QList<const char*>& iids, QJsonObject pluginInfo = {}){
    for(const char* iid : iids){
        QCborMap cborValue;
        if(iid){
            cborValue.insert(qint64(QtPluginMetaDataKeys::IID), iid);
        }
        cborValue.insert(qint64(QtPluginMetaDataKeys::ClassName), className);
        cborValue.insert(qint64(QtPluginMetaDataKeys::MetaData), QCborMap::fromJsonObject(metaData));
        for(const QString& key : pluginInfo.keys()){
            cborValue.insert(QLatin1String(key.toLatin1()), QCborValue::fromJsonValue(pluginInfo[key]));
        }
        QByteArray cborData = cborValue.toCborValue().toCbor();
        QByteArray rawMetaData;
        rawMetaData.reserve(16 + cborData.size());
        rawMetaData.append("QTMETADATA !");
        rawMetaData.append(char(0));
        rawMetaData.append(char(QT_VERSION_MAJOR));
        rawMetaData.append(char(QT_VERSION_MINOR));
        rawMetaData.append(char(qPluginArchRequirements()));
        rawMetaData.append(cborData);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QtPluginMetaDataFunction rawMetaDataFunction = qtjambi_function_pointer<64,QPluginMetaData()>(
                    [rawMetaData]() -> QPluginMetaData{
                        return QPluginMetaData{reinterpret_cast<const uchar *>(rawMetaData.data()), size_t(rawMetaData.size())};
                    }
                , qHash(rawMetaData));
        QStaticPlugin staticPlugin(instanceFunction, rawMetaDataFunction);
#else
        QStaticPlugin staticPlugin;
        staticPlugin.instance = instanceFunction;
        staticPlugin.rawMetaData = qtjambi_function_pointer<64,const char*()>(
                    [rawMetaData]() -> const char*{
                        return rawMetaData;
                    }
                , qHash(rawMetaData));
#endif
        qRegisterStaticPluginFunction(staticPlugin);
    }
}

// emitting (writeExtraFunctions)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiPlugins_qRegisterStaticPluginFunctionInstance)
(JNIEnv *__jni_env,
 jclass, QtJambiNativeID instance0, QtJambiNativeID metaData1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterStaticPluginFunction(QStaticPlugin)")
    try{
        QSharedPointer<QtJambiLink> instanceLink = QtJambiLink::fromNativeId(instance0);
        qtjambi_check_resource(__jni_env, instanceLink.data(), typeid(QObject));
        Q_ASSERT(instanceLink->isQObject());
        QSharedPointer<QPointer<QObject>> pointer(new QPointer<QObject>(instanceLink->qobject()));
        jobject instance = instanceLink->getJavaObjectLocalRef(__jni_env);
        Q_ASSERT(instance);
        instanceLink->setCppOwnership(__jni_env);
        jclass cls = __jni_env->GetObjectClass(instance);
        Q_ASSERT(cls);
        QString className = qtjambi_class_name(__jni_env, cls);
        QList<const char*> iids = getInterfaceIIDs(__jni_env, cls);
        if(iids.isEmpty()){
            iids << nullptr;
        }
        QJsonObject json = qtjambi_value_from_nativeId<QJsonObject>(metaData1);
        QtPluginInstanceFunction instanceFunction = qtjambi_function_pointer<64,QObject*()>(
                    [pointer]() -> QObject*{
                        return pointer->data();
                    }
                , qHash(className) * 31 + qHash(json.value("Keys")));
        registerPlugin(instanceFunction, className.replace(".", "::"), json, iids);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiPlugins_qRegisterStaticPluginFunctionClass)
(JNIEnv *__jni_env,
 jclass, jclass cls, QtJambiNativeID metaData1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterStaticPluginFunction(QStaticPlugin)")
    try{
        QString className = qtjambi_class_name(__jni_env, cls);
        QList<const char*> iids = getInterfaceIIDs(__jni_env, cls);
        if(iids.isEmpty()){
            iids << nullptr;
        }
        cls = getGlobalClassRef(__jni_env, cls);
        jmethodID constructorHandle = __jni_env->GetMethodID(cls, "<init>", "()V");
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO);
        if(constructorHandle){
            QJsonObject json = qtjambi_value_from_nativeId<QJsonObject>(metaData1);
            QSharedPointer<QPointer<QObject>> pointer(new QPointer<QObject>());
            QtPluginInstanceFunction instanceFunction = qtjambi_function_pointer<64,QObject*()>(
                        [cls, constructorHandle, pointer]() -> QObject*{
                            if(JNIEnv* env = qtjambi_current_environment()){
                                QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                                if(pointer->isNull()){
                                    jobject plugin = env->NewObject(cls, constructorHandle);
                                    JavaException::check(env QTJAMBI_STACKTRACEINFO);
                                    QSharedPointer<QtJambiLink> instanceLink = QtJambiLink::findLinkForJavaObject(env, plugin);
                                    instanceLink->setCppOwnership(env);
                                    QPointer<QObject> p(instanceLink->qobject());
                                    pointer->swap(p);
                                }
                                return pointer->data();
                            }
                            return nullptr;
                        }
                    , qHash(className) * 31 + qHash(json.value("Keys")));
            registerPlugin(instanceFunction, className.replace(".", "::"), json, iids);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiPlugins_qRegisterStaticPluginFunction)
(JNIEnv *__jni_env,
 jclass, jobject classSupplier, jstring _className, jstring jiid, QtJambiNativeID metaData1, QtJambiNativeID pluginInfo1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterStaticPluginFunction(QStaticPlugin)")
    try{
        QString className = qtjambi_to_qstring(__jni_env, _className);
        QByteArray iid = qtjambi_to_qstring(__jni_env, jiid).toLatin1();
        QJsonObject metaData = qtjambi_value_from_nativeId<QJsonObject>(metaData1);
        QJsonObject pluginInfo = qtjambi_value_from_nativeId<QJsonObject>(pluginInfo1);

        struct Functor{
            JObjectWrapper m_classOrSupplier;
            jmethodID m_constructorHandle;
            QPointer<QObject> m_pointer;

            Functor(JNIEnv *env, jobject classSupplier)
                : m_classOrSupplier(env, classSupplier),
                  m_constructorHandle(nullptr),
                  m_pointer() {}

            QObject* operator()(){
                if(m_pointer.isNull()){
                    if(JNIEnv* env = qtjambi_current_environment()){
                        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                        if(!m_constructorHandle && m_classOrSupplier.object()){ // m_classOrSupplier is Supplier
                            jobject classOrSupplier = env->NewLocalRef(m_classOrSupplier.object());
                            m_classOrSupplier = JObjectWrapper();
                            jclass cls = jclass(Java::Runtime::Supplier::get(env, classOrSupplier));
                            if(cls){
                                m_constructorHandle = env->GetMethodID(cls, "<init>", "()V");
                                JavaException::check(env QTJAMBI_STACKTRACEINFO);
                                if(m_constructorHandle){
                                    m_classOrSupplier = JObjectWrapper(env, cls);
                                }
                            }
                        }
                        if(m_constructorHandle){ // m_classOrSupplier is Class
                            jobject plugin = env->NewObject(jclass(m_classOrSupplier.object()), m_constructorHandle);
                            JavaException::check(env QTJAMBI_STACKTRACEINFO);
                            QSharedPointer<QtJambiLink> instanceLink = QtJambiLink::findLinkForJavaObject(env, plugin);
                            instanceLink->setCppOwnership(env);
                            m_pointer = instanceLink->qobject();
                        }
                    }
                }
                return m_pointer.data();
            }
        };

        QtPluginInstanceFunction instanceFunction = qtjambi_function_pointer<64,QObject*()>(
                    Functor(__jni_env, classSupplier), qHash(className) * 31 + qHash(metaData.value("Keys")));
        registerPlugin(instanceFunction, className.replace(".", "::"), metaData, {iid}, pluginInfo);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiPlugins_qRegisterPluginInterface)
(JNIEnv *__jni_env,
 jclass, jclass cls)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterPluginInterface(Class)")
    try{
        (void)registerInterfaceID(__jni_env, cls);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" Q_DECL_EXPORT jclass JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiPlugins_qRegisteredPluginInterface)
(JNIEnv *__jni_env,
 jclass, QtJambiNativeID iid)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisteredPluginInterface(String)")
    try{
        return getInterfaceByIID(__jni_env, qtjambi_value_from_nativeId<QByteArray>(iid));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_00024NativeLink_setHasDisposedSignal)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            link->setHasDisposedSignal();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT bool JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_00024NativeLink_hasDisposedSignal)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            return link->hasDisposedSignal();
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_00024NativeLink_clean)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        jthrowable throwable(nullptr);
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)){
            try{
                link->onClean(env);
            }catch(const JavaException& exn){
                throwable = jthrowable(env->NewLocalRef(exn.object()));
            }
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            {
                QWriteLocker locker(QtJambiLinkUserData::lock());
                link.clear();
            }
        }
        if(throwable)
            env->Throw(throwable);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_00024NativeLink_dispose)
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        jthrowable throwable(nullptr);
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
            try{
                QtJambiExceptionRaiser __exnRaiser;
                link->onDispose(env);
                __exnRaiser.raise(env);
            }catch(const JavaException& exn){
                throwable = jthrowable(env->NewLocalRef(exn.object()));
            }
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            {
                QWriteLocker locker(QtJambiLinkUserData::lock());
                link.clear();
            }
        }
        if(throwable)
            env->Throw(throwable);
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

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiPlugins_getInterfaceIID)
    (JNIEnv *env, jclass, jclass cls)
{
    try{
        const char* iid = getInterfaceIID(env, cls);
        return iid ? env->NewStringUTF(iid) : nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){
    try{
        return qtjambi_get_internal_access(env, cls);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_putenv)
    (JNIEnv *env, jclass, jstring varName, jstring value)
{
    try{
        QString _varName = qtjambi_to_qstring(env, varName);
        QString _value = qtjambi_to_qstring(env, value);
        return qputenv(qPrintable(_varName), qPrintable(_value));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_unsetenv)
    (JNIEnv *env, jclass, jstring varName)
{
    try{
        QString _varName = qtjambi_to_qstring(env, varName);
        return qunsetenv(qPrintable(_varName));
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_QtUtilities_getenv)
    (JNIEnv *env, jclass, jstring varName)
{
    try{
        QString _varName = qtjambi_to_qstring(env, varName);
        if(qEnvironmentVariableIsSet(qPrintable(_varName))){
            QString value = qEnvironmentVariable(qPrintable(_varName));
            return qtjambi_from_qstring(env, value);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}

Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gPointerLock, (QReadWriteLock::Recursive))
typedef QHash<quintptr, QList<JObjectWrapper>> ObjectsByFunctionPointer;
Q_GLOBAL_STATIC(ObjectsByFunctionPointer, gObjectsByFunctionPointer)

static int MAX_NARGS = 256;

struct ffi_type {
    size_t size;
    short alignment;
    short type;
    ffi_type const** elements;
    ffi_type() : size(0), alignment(1), type(13), elements(nullptr) {}
    ffi_type(size_t _size,
            short _alignment,
            short _type,
             ffi_type const** _elements) : size(_size), alignment(_alignment), type(_type), elements(_elements) {}
};

#define FFI_TYPE_VOID       0
#define FFI_TYPE_INT        1
#define FFI_TYPE_FLOAT      2
#define FFI_TYPE_DOUBLE     3
#define FFI_TYPE_UINT8      5
#define FFI_TYPE_SINT8      6
#define FFI_TYPE_UINT16     7
#define FFI_TYPE_SINT16     8
#define FFI_TYPE_UINT32     9
#define FFI_TYPE_SINT32     10
#define FFI_TYPE_UINT64     11
#define FFI_TYPE_SINT64     12
#define FFI_TYPE_STRUCT     13
#define FFI_TYPE_POINTER    14
#define FFI_TYPE_COMPLEX    15

#define FFI_TYPEDEF(name, type, id)\
static const ffi_type ffi_type_##name = { \
  sizeof(type), \
  alignof(type), \
  id, nullptr \
}

static const ffi_type ffi_type_void = {
  1, 1, FFI_TYPE_VOID, nullptr
};

FFI_TYPEDEF(quint8, quint8, FFI_TYPE_UINT8);
FFI_TYPEDEF(qint8, qint8, FFI_TYPE_SINT8);
FFI_TYPEDEF(quint16, quint16, FFI_TYPE_UINT16);
FFI_TYPEDEF(qint16, qint16, FFI_TYPE_SINT16);
FFI_TYPEDEF(quint32, quint32, FFI_TYPE_UINT32);
FFI_TYPEDEF(qint32, qint32, FFI_TYPE_SINT32);
FFI_TYPEDEF(quint64, quint64, FFI_TYPE_UINT64);
FFI_TYPEDEF(qint64, qint64, FFI_TYPE_SINT64);

FFI_TYPEDEF(pointer, void*, FFI_TYPE_POINTER);

FFI_TYPEDEF(float, float, FFI_TYPE_FLOAT);
FFI_TYPEDEF(double, double, FFI_TYPE_DOUBLE);

void qtjambi_funtion_dispatch(JNIEnv *env, void* func, jobjectArray args, ffi_type *return_type, void *presult);

struct Cleanup : QSharedData{
    Cleanup(std::function<void()>&& _task) :QSharedData(), task(std::move(_task)){}
    ~Cleanup(){ if(task) task(); }
    std::function<void()> task;
};

void delete_callback_pointer(void* ptr, bool){
    delete reinterpret_cast<QFunctionPointer*>(ptr);
}

void convertArgumentList(QVector<QSharedDataPointer<Cleanup>>& cleaners, QVector<std::function<void(JNIEnv *)>>& successActions, JNIEnv * __jni_env, jobjectArray arguments, jsize length){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static QMetaType jniEnvType(QMetaType::type("JNIEnv*"));
    static QMetaType jniObjType(QMetaType::type("jobject"));
    static QMetaType stringViewType(QMetaType::type("QStringView"));
#else
    static QMetaType jniEnvType = QMetaType::fromName("JNIEnv*");
    static QMetaType jniObjType = QMetaType::fromName("jobject");
    static QMetaType stringViewType = QMetaType::fromName("QStringView");
#endif
    for(jsize i = 0; i<length; ++i){
        jobject arg = __jni_env->GetObjectArrayElement(arguments, i);
        jobject val = nullptr;
        jint argPointerOrReference = 0;
        const QMetaType* argMetaType = nullptr;
        jclass argClassType = nullptr;
        bool resolved = false;
        bool isReferenceMetaType = false;
        QScopedPointer<const QMetaType> _argMetaType;
        if(Java::QtCore::QGenericArgument::isInstanceOf(__jni_env, arg)){
            argMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, arg));
            if(argMetaType)
                isReferenceMetaType = QByteArray(argMetaType->name()).endsWith("&");
            argPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, arg);
            argClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, arg);
            val = arg = Java::QtCore::QGenericArgument::value(__jni_env, arg);
            if(argMetaType){
                if(argMetaType->id()==QMetaType::Nullptr){
                    arg = Java::JNA::Pointer::newInstance(__jni_env, 0);
                    resolved = true;
                }else if(jniEnvType==*argMetaType){
                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(__jni_env));
                    resolved = true;
                }else if(jniObjType==*argMetaType){
                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(val));
                    resolved = true;
                }
            }
        }else{
            val = arg;
            if(val){
                argClassType = __jni_env->GetObjectClass(val);
                if(!Java::Runtime::Class::isArray(__jni_env, argClassType)){
                    argMetaType = qtjambi_cast<const QMetaType*>(Java::QtCore::QGenericArgument::metaType(__jni_env, val));
                    if(!argMetaType->isValid()){
                        argMetaType = nullptr;
                    }else
                        argClassType = qtjambi_class_for_metatype(__jni_env, argMetaType->id());
                }
            }else{
                arg = Java::JNA::Pointer::newInstance(__jni_env, 0);
                resolved = true;
            }
        }
        if(!resolved && argPointerOrReference==2 && argClassType){
            QFunctionPointer ptr = nullptr;
            if(val){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                    if(DeletablePointerToObjectLink* plink = dynamic_cast<DeletablePointerToObjectLink*>(link.get())){
                        if(plink->pointer() && plink->deleterFunction()==&delete_callback_pointer){
                            ptr = *reinterpret_cast<QFunctionPointer*>(plink->pointer());
                            resolved = true;
                        }
                    }
                }
                if(!resolved){
                    jobject ih = Java::QtCore::QFunctionPointerUtil::createCallbackProxy(__jni_env, Java::JNA::CallbackProxy::getClass(__jni_env), argClassType, val);
                    jobject callbackProxy = Java::QtCore::QFunctionPointerUtil$CppToJavaInvocationHandler::proxy(__jni_env, ih);
                    jobject method = __jni_env->ToReflectedMethod(Java::JNA::CallbackProxy::getClass(__jni_env), Java::JNA::CallbackProxy::callbackMethod_ID(__jni_env), false);
                    jlong peer = Java::JNA::Native::createNativeCallback(__jni_env,
                                                                  callbackProxy,
                                                                  method,
                                                                  Java::JNA::CallbackProxy::getParameterTypes(__jni_env, callbackProxy),
                                                                  Java::JNA::CallbackProxy::getReturnType(__jni_env, callbackProxy),
                                                                  0,
                                                                  0,
                                                                  __jni_env->NewStringUTF("UTF-8"));
                    ptr = *reinterpret_cast<QFunctionPointer*>(peer);
                    QtJambiLink::createLinkForObject(
                            __jni_env,
                            val,
                            new QFunctionPointer(ptr),
                            LINK_NAME_ARG(nullptr)
                            false,
                            false,
                            &delete_callback_pointer
                        )->setJavaOwnership(__jni_env);
                    {
                        QWriteLocker locker(gPointerLock());
                        (*gObjectsByFunctionPointer())[quintptr(ptr)] << JObjectWrapper(__jni_env, val);
                    }
                    Java::QtCore::QFunctionPointerUtil$CppToJavaInvocationHandler::set_peer(__jni_env, ih, peer);
                    Java::QtCore::QFunctionPointerUtil::registerCleanup(__jni_env, val, ih);
                }
            }
            arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
            resolved = true;
        }
        if(!resolved){
            if(Java::Runtime::String::isInstanceOf(__jni_env, val)){
                if(argMetaType){
                    if(argMetaType->id()==QMetaType::Char && argPointerOrReference==-2){
                        const char* chars = __jni_env->GetStringUTFChars(jstring(val), nullptr);
                        arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(chars));
                        resolved = true;
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[val, chars, __jni_env](){ __jni_env->ReleaseStringUTFChars(jstring(val), chars); }}});
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    }else if(argMetaType->id()==QMetaType::Char16 && argPointerOrReference==-2){
                        const jchar* chars = __jni_env->GetStringChars(jstring(val), nullptr);
                        arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(chars));
                        resolved = true;
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[val, chars, __jni_env](){ __jni_env->ReleaseStringChars(jstring(val), chars); }}});
#endif
                    }else if(argMetaType->id()==QMetaType::QByteArray){
                        const char* chars = __jni_env->GetStringUTFChars(jstring(val), nullptr);
                        QByteArray* byteArray = new QByteArray(chars);
                        __jni_env->ReleaseStringUTFChars(jstring(val), chars);
                        void* ptr = byteArray;
                        if(argPointerOrReference==0){
                            arg = __jni_env->NewDirectByteBuffer(ptr, jsize(sizeof(QByteArray)));
                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                        }else{
                            arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(ptr));
                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[byteArray](){ delete byteArray; }}});
                        }
                    }else if(*argMetaType==stringViewType){
                        const jchar* chars = __jni_env->GetStringChars(jstring(val), nullptr);
                        resolved = true;
                        QStringView* strg = new QStringView(chars);
                        void* ptr = strg;
                        if(argPointerOrReference==0){
                            arg = __jni_env->NewDirectByteBuffer(ptr, jsize(sizeof(QStringView)));
                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr, val, chars, __jni_env](){ operator delete (ptr); __jni_env->ReleaseStringChars(jstring(val), chars); }}});
                        }else{
                            arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(ptr));
                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[strg, val, chars, __jni_env](){ delete strg; __jni_env->ReleaseStringChars(jstring(val), chars); }}});
                        }
                    }
                }
                if(!resolved){
                    QString* string = new QString();
                    qtjambi_to_qstring(*string, __jni_env, jstring(val));
                    void* ptr = string;
                    if(argPointerOrReference==0){
                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(sizeof(QString)));
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                    }else{
                        arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(ptr));
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[string](){ delete string; }}});
                    }
                }
            }else if(Java::Runtime::Class::isArray(__jni_env, __jni_env->GetObjectClass(val))
                     && Java::Runtime::String::isSameClass(__jni_env, Java::Runtime::Class::getComponentType(__jni_env, __jni_env->GetObjectClass(val)))
                     && (!argMetaType || QByteArray("QString&")==argMetaType->name() || QByteArray("QString &")==argMetaType->name()
                                      || QByteArray("QString*")==argMetaType->name() || QByteArray("QString *")==argMetaType->name()
                                      || QByteArray("const char**")==argMetaType->name())){
                jsize length = __jni_env->GetArrayLength(jobjectArray(val));
                if(argMetaType && QByteArray("const char**")==argMetaType->name()){
                    const char** ptr = new const char*[size_t(length)];
                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ delete[] ptr; }}});
                    for(jsize i = 0; i<length; ++i){
                        jstring strg = jstring(__jni_env->GetObjectArrayElement(jobjectArray(val), i));
                        const char* chars = __jni_env->GetStringUTFChars(strg, nullptr);
                        ptr[i] = chars;
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[strg, chars, __jni_env](){ __jni_env->ReleaseStringUTFChars(strg, chars); }}});
                    }
                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(ptr));
                    resolved = true;
                }else if(argMetaType && (QByteArray("QString&")==argMetaType->name() || QByteArray("QString &")==argMetaType->name())){
                    if(length!=1){
                        JavaException::raiseIllegalArgumentException(__jni_env, qPrintable(QString("String array of size 1 expected, given: %1").arg(length)) QTJAMBI_STACKTRACEINFO );
                    }else{
                        jstring strg = jstring(__jni_env->GetObjectArrayElement(jobjectArray(val), 0));
                        if(!strg)
                            JavaException::raiseIllegalArgumentException(__jni_env, qPrintable(QString("Argument %1: null not expected as String array entry.").arg(i+1)) QTJAMBI_STACKTRACEINFO );
                        QString* string = new QString;
                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[string](){ delete string; }}});
                        qtjambi_to_qstring(*string, __jni_env, strg);
                        arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(string));
                        resolved = true;
                        successActions.append([string, val](JNIEnv * env){
                            jstring strg = jstring(env->GetObjectArrayElement(jobjectArray(val), 0));
                            jstring newStrg = qtjambi_from_qstring(env, *string);
                            if(!Java::Runtime::Objects::equals(env, strg, newStrg)){
                                env->SetObjectArrayElement(jobjectArray(val), 0, newStrg);
                            }
                        });
                    }
                }else{
                    QString* strings = new QString[size_t(length)];
                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[strings](){ delete[] strings; }}});
                    for(jsize i = 0; i<length; ++i){
                        jstring strg = jstring(__jni_env->GetObjectArrayElement(jobjectArray(val), i));
                        qtjambi_to_qstring(strings[i], __jni_env, strg);
                    }
                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(strings));
                    resolved = true;
                    if(argPointerOrReference!=-2
                            || (argMetaType && (QByteArray("QString*")==argMetaType->name() || QByteArray("QString *")==argMetaType->name()))){
                        successActions.append([strings, length, val](JNIEnv * env){
                            for(jsize i = 0; i<length; ++i){
                                jstring strg = jstring(env->GetObjectArrayElement(jobjectArray(val), i));
                                jstring newStrg = qtjambi_from_qstring(env, strings[i]);
                                if(!Java::Runtime::Objects::equals(env, strg, newStrg)){
                                    env->SetObjectArrayElement(jobjectArray(val), i, newStrg);
                                }
                            }
                        });
                    }
                }
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QFlags::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QFlags::value(__jni_env, val));
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtEnumerator::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QtEnumerator::value(__jni_env, val));
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtByteEnumerator::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Byte::valueOf(__jni_env, Java::QtJambi::QtByteEnumerator::value(__jni_env, val));
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtShortEnumerator::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Short::valueOf(__jni_env, Java::QtJambi::QtShortEnumerator::value(__jni_env, val));
            }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtLongEnumerator::isInstanceOf(__jni_env, val)){
                arg = Java::Runtime::Long::valueOf(__jni_env, Java::QtJambi::QtLongEnumerator::value(__jni_env, val));
            }else if(argPointerOrReference!=0
                    || isReferenceMetaType
                    || (!Java::Runtime::Byte::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Short::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Integer::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Long::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Float::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Double::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Character::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Boolean::isInstanceOf(__jni_env, val)
                     && !Java::Runtime::Buffer::isInstanceOf(__jni_env, val)
                     && !Java::JNA::Pointer::isInstanceOf(__jni_env, val)
                     && !Java::JNA::Structure::isInstanceOf(__jni_env, val)
                     && !(argClassType
                          && Java::Runtime::Class::isArray(__jni_env, argClassType)
                          && Java::Runtime::Class::isPrimitive(__jni_env, Java::Runtime::Class::getComponentType(__jni_env, argClassType))))){
                const std::type_info* typeId = nullptr;
                size_t size = 0;
                if(argPointerOrReference==0
                         && !isReferenceMetaType
                         && Java::Runtime::Class::isArray(__jni_env, __jni_env->GetObjectClass(val))){
                    jsize length = __jni_env->GetArrayLength(jobjectArray(val));
                    jclass argClassType = Java::Runtime::Class::getComponentType(__jni_env, __jni_env->GetObjectClass(val));
                    const QMetaType* argMetaType = nullptr;
                    QScopedPointer<const QMetaType> _argMetaType;
                    for(jsize i = 0; i<length; ++i){
                        jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                        argMetaType = qtjambi_cast<const QMetaType*>(Java::QtCore::QGenericArgument::metaType(__jni_env, entry));
                        if(!argMetaType->isValid()){
                            argClassType = __jni_env->GetObjectClass(entry);
                            argMetaType = nullptr;
                        }else{
                            argClassType = qtjambi_class_for_metatype(__jni_env, argMetaType->id());
                            break;
                        }
                    }
                    if(!argMetaType)
                        _argMetaType.reset(argMetaType = new QMetaType(qtjambi_register_metatype(__jni_env, argClassType, false, false)));
                    if(!argMetaType->isValid())
                        _argMetaType.reset(argMetaType = nullptr);

                    {
                        const SuperTypeInfos& infos = getSuperTypeInfos(__jni_env, argClassType);
                        if(infos.isEmpty()){
                            if(argMetaType){
                                typeId = getTypeByMetaType(*argMetaType);
                                size = size_t(argMetaType->sizeOf());
                            }else{
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            }
                        }else{
                            typeId = &infos.first().typeId();
                            size = infos.first().size();
                        }
                    }
                    resolved = false;
                    if(typeId){
                        bool isValueFunctional = false;
                        switch(getEntryType(*typeId)){
                        case EntryTypes::FunctionalTypeInfo:
                            isValueFunctional = true;
                            if(const QtJambiTypeInfo* info = getQTypeInfo(*typeId)){
                                if(info->isPointer){
                                    void** array = length==0 ? nullptr : new void*[size_t(length)];
                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                    QVector<void*> control;
                                    for(jsize i = 0; i<length; ++i){
                                        jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                        void* ptr = nullptr;
                                        if(!entry){
                                            array[i] = nullptr;
                                        }else if(qtjambi_convert_to_native(__jni_env, *typeId, entry, ptr)){
                                            array[i] = *reinterpret_cast<void**>(ptr);
                                        }else{
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                        }
                                        control << array[i];
                                    }
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                                    successActions.append([array, control, length, val, typeId](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            if(control[i] != array[i]){
                                                env->SetObjectArrayElement(jobjectArray(val), i, qtjambi_from_object(env, array[i], nullptr, *typeId, false, false));
                                            }
                                        }
                                    });
                                    resolved = true;
                                    break;
                                }
                            }
                            Q_FALLTHROUGH();
                        case EntryTypes::ObjectTypeInfo:
                        case EntryTypes::InterfaceTypeInfo:
                        case EntryTypes::QObjectTypeInfo:
                            if(!isValueFunctional && !resolved){
                                resolved = true;
                                QVector<void*> control;
                                void** array = length==0 ? nullptr : new void*[size_t(length)];
                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                for(jsize i = 0; i<length; ++i){
                                    jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                    if(!entry){
                                        array[i] = nullptr;
                                    }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, entry)){
                                        array[i] = link->pointer();
                                    }else if(!qtjambi_convert_to_native(__jni_env, *typeId, entry, array+i)){
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                    control << array[i];
                                }
                                arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                                successActions.append([array, control, length, val, typeId](JNIEnv * env){
                                    for(jsize i = 0; i<length; ++i){
                                        if(control[i] != array[i]){
                                            env->SetObjectArrayElement(jobjectArray(val), i, qtjambi_from_object(env, array[i], nullptr, *typeId, false, false));
                                        }
                                    }
                                });
                                break;
                            }
                            Q_FALLTHROUGH();
                        case EntryTypes::InterfaceValueTypeInfo:
                        case EntryTypes::ValueTypeInfo:
                        case EntryTypes::FlagsTypeInfo:
                        case EntryTypes::SpecialTypeInfo:
                        case EntryTypes::StringTypeInfo:
                        case EntryTypes::PrimitiveTypeInfo:
                        {
                            resolved = true;
                            if(!argMetaType)
                                _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                            if(!argMetaType->isValid())
                                _argMetaType.reset(argMetaType = nullptr);
                            if(!argMetaType){
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                return;
                            }
                            char* array = length==0 ? nullptr : new char[size_t(length) * size];
                            try{
                                for(jsize i = 0; i<length; ++i){
                                    jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                    void * ptr = array+size_t(i)*size;
                                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, entry)){
                                        argMetaType->construct(ptr, link->pointer());
                                    }else{
                                        argMetaType->construct(ptr, nullptr);
                                        if(!qtjambi_convert_to_native(__jni_env, *typeId, entry, ptr)){
                                            if(Java::Runtime::Map::isInstanceOf(__jni_env, entry) || Java::Runtime::Collection::isInstanceOf(__jni_env, entry)){
                                                QVariant variant = qtjambi_to_qvariant(__jni_env, entry);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
#else
                                                if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
#endif
                                                    argMetaType->destruct(ptr);
                                                    argMetaType->construct(ptr, variant.data());
                                                }else{
                                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                }
                                            }else{
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            }
                                        }
                                    }
                                    int metaId = argMetaType->id();
                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[metaId,ptr](){ QMetaType(metaId).destruct(ptr); }}});
                                }
                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                            }catch(...){
                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                throw;
                            }
                            arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                            int metaType = argMetaType->id();
                            successActions.append([array, length, val, typeId, size, metaType](JNIEnv * env){
                                for(jsize i = 0; i<length; ++i){
                                    jobject obj = env->GetObjectArrayElement(jobjectArray(val), i);
                                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, obj)){
                                        if(link->pointer() && !link->isShell()){
                                            QMetaType argMetaType(metaType);
                                            argMetaType.destruct(link->pointer());
                                            argMetaType.construct(link->pointer(), array+size_t(i)*size);
                                            continue;
                                        }
                                    }
                                    jobject newObj = qtjambi_from_object(env, array+size_t(i)*size, nullptr, *typeId, true, false);
                                    if(!Java::Runtime::Objects::equals(env, obj, newObj)){
                                        env->SetObjectArrayElement(jobjectArray(val), i, newObj);
                                    }
                                }
                            });
                        }
                            break;
                        default:
                            break;
                        }
                    }
                    if(!resolved){
                        if(argMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            if(argMetaType->name().endsWith("*")){
#else
                            if(argMetaType->flags() & QMetaType::IsPointer){
#endif
                                QVector<void*> control;
                                void** array = length==0 ? nullptr : new void*[size_t(length)];
                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                for(jsize i = 0; i<length; ++i){
                                    jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                    if(!entry){
                                        array[i] = nullptr;
                                    }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, entry)){
                                        array[i] = link->pointer();
                                    }else if(!typeId || !qtjambi_convert_to_native(__jni_env, *typeId, entry, array+i)){
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                    control << array[i];
                                }
                                arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                                if(typeId){
                                    successActions.append([array, control, length, val, typeId](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            if(control[i] != array[i]){
                                                env->SetObjectArrayElement(jobjectArray(val), i, qtjambi_from_object(env, array[i], nullptr, *typeId, false, false));
                                            }
                                        }
                                    });
                                }else{
                                    successActions.append([array, control, length, val, argClassType](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            if(control[i] != array[i]){
                                                env->SetObjectArrayElement(jobjectArray(val), i, qtjambi_from_object(env, array[i], argClassType, false, false));
                                            }
                                        }
                                    });
                                }
                            }else{
                                resolved = true;
                                if(!argMetaType)
                                    _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                                if(!argMetaType->isValid())
                                    _argMetaType.reset(argMetaType = nullptr);
                                if(!argMetaType){
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    return;
                                }
                                char* array = length==0 ? nullptr : new char[size_t(length) * size];
                                try{
                                    for(jsize i = 0; i<length; ++i){
                                        jobject entry = __jni_env->GetObjectArrayElement(jobjectArray(val), i);
                                        void * ptr = array+size_t(i)*size;
                                        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, entry)){
                                            argMetaType->construct(ptr, link->pointer());
                                        }else{
                                            argMetaType->construct(ptr, nullptr);
                                            if(!typeId || !qtjambi_convert_to_native(__jni_env, *typeId, entry, ptr)){
                                                if(Java::Runtime::Map::isInstanceOf(__jni_env, entry) || Java::Runtime::Collection::isInstanceOf(__jni_env, entry)){
                                                    QVariant variant = qtjambi_to_qvariant(__jni_env, entry);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                    if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
#else
                                                    if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
#endif
                                                        argMetaType->destruct(ptr);
                                                        argMetaType->construct(ptr, variant.data());
                                                    }else{
                                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                    }
                                                }else{
                                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, entry).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                }
                                            }
                                        }
                                        int metaId = argMetaType->id();
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[metaId,ptr](){ QMetaType(metaId).destruct(ptr); }}});
                                    }
                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                }catch(...){
                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[array](){ delete[] array; }}});
                                    throw;
                                }
                                arg = Java::JNA::Pointer::newInstance(__jni_env, jlong(array));
                                int metaType = argMetaType->id();
                                if(typeId){
                                    successActions.append([array, length, val, typeId, size, metaType](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            jobject obj = env->GetObjectArrayElement(jobjectArray(val), i);
                                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, obj)){
                                                if(link->pointer() && !link->isShell()){
                                                    QMetaType argMetaType(metaType);
                                                    argMetaType.destruct(link->pointer());
                                                    argMetaType.construct(link->pointer(), array+size_t(i)*size);
                                                    continue;
                                                }
                                            }
                                            jobject newObj = qtjambi_from_object(env, array+size_t(i)*size, nullptr, *typeId, true, false);
                                            if(!Java::Runtime::Objects::equals(env, obj, newObj)){
                                                env->SetObjectArrayElement(jobjectArray(val), i, newObj);
                                            }
                                        }
                                    });
                                }else{
                                    successActions.append([array, length, val, argClassType, size, metaType](JNIEnv * env){
                                        for(jsize i = 0; i<length; ++i){
                                            jobject obj = env->GetObjectArrayElement(jobjectArray(val), i);
                                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, obj)){
                                                if(link->pointer() && !link->isShell()){
                                                    QMetaType argMetaType(metaType);
                                                    argMetaType.destruct(link->pointer());
                                                    argMetaType.construct(link->pointer(), array+size_t(i)*size);
                                                    continue;
                                                }
                                            }
                                            jobject newObj = qtjambi_from_object(env, array+size_t(i)*size, argClassType, true, false);
                                            if(!Java::Runtime::Objects::equals(env, obj, newObj)){
                                                env->SetObjectArrayElement(jobjectArray(val), i, newObj);
                                            }
                                        }
                                    });
                                }
                            }
                        }else{
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                    resolved = true;
                }else{
                    {
                        const SuperTypeInfos& infos = getSuperTypeInfos(__jni_env, argClassType);
                        if(infos.isEmpty()){
                            if(argMetaType){
                                typeId = getTypeByMetaType(*argMetaType);
                                size = size_t(argMetaType->sizeOf());
                            }else{
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            }
                        }else{
                            typeId = &infos.first().typeId();
                            size = infos.first().size();
                        }
                    }
                    void* ptr = nullptr;
                    if(typeId){
                        bool isValueFunctional = false;
                        switch(getEntryType(*typeId)){
                        case EntryTypes::FunctionalTypeInfo:
                            isValueFunctional = true;
                            if(const QtJambiTypeInfo* info = getQTypeInfo(*typeId)){
                                if(info->isPointer){
                                    if(qtjambi_convert_to_native(__jni_env, *typeId, val, &ptr)){
                                        arg = Java::JNA::Pointer::newInstance(__jni_env, *reinterpret_cast<void**>(ptr));
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                    resolved = true;
                                    break;
                                }
                            }
                            Q_FALLTHROUGH();
                        case EntryTypes::ObjectTypeInfo:
                        case EntryTypes::InterfaceTypeInfo:
                        case EntryTypes::QObjectTypeInfo:
                            if(!isValueFunctional && !resolved){
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                                }else if(qtjambi_convert_to_native(__jni_env, *typeId, val, &ptr)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                }else{
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                }
                                resolved = true;
                                break;
                            }
                            Q_FALLTHROUGH();
                        case EntryTypes::InterfaceValueTypeInfo:
                        case EntryTypes::ValueTypeInfo:
                        {
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                ptr = link->pointer();
                                if(!argMetaType){
                                    if(PointerToObjectLink* plink = dynamic_cast<PointerToObjectLink*>(link.get())){
                                        argMetaType = &plink->metaType();
                                        if(!argMetaType->isValid())
                                            argMetaType = nullptr;
                                    }
                                }
                                if(argPointerOrReference==0 && !isReferenceMetaType){
                                    if(!argMetaType)
                                        _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                                    if(!argMetaType->isValid())
                                        _argMetaType.reset(argMetaType = nullptr);
                                    if(argMetaType){
                                        ptr = argMetaType->create(ptr);
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else{
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                }
                            }else{
                                if(!argMetaType)
                                    _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                                if(!argMetaType->isValid())
                                    _argMetaType.reset(argMetaType = nullptr);
                                if(argMetaType){
                                    ptr = argMetaType->create(nullptr);
                                    if(!qtjambi_convert_to_native(__jni_env, *typeId, val, ptr)){
                                        if(Java::Runtime::Map::isInstanceOf(__jni_env, val) || Java::Runtime::Collection::isInstanceOf(__jni_env, val)){
                                            QVariant variant = qtjambi_to_qvariant(__jni_env, val);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                            if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
#else
                                            if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
#endif
                                                argMetaType->destruct(ptr);
                                                argMetaType->construct(ptr, variant.data());
                                            }else{
                                                argMetaType->destroy(ptr);
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                return;
                                            }
                                        }else{
                                            argMetaType->destroy(ptr);
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            return;
                                        }
                                    }
                                    if(argPointerOrReference==0 && !isReferenceMetaType){
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[argMetaType, ptr](){ argMetaType->destroy(ptr); }}});
                                    }
                                }else{
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                }
                            }
                            resolved = true;
                        }
                        break;
                        case EntryTypes::FlagsTypeInfo:
                        case EntryTypes::SpecialTypeInfo:
                        case EntryTypes::StringTypeInfo:
                        case EntryTypes::PrimitiveTypeInfo:
                            {
                                if(!argMetaType)
                                    _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                                if(!argMetaType->isValid())
                                    _argMetaType.reset(argMetaType = nullptr);
                                if(argPointerOrReference==0 && !isReferenceMetaType){
                                    if(size>0){
                                        if(argMetaType)
                                            ptr = argMetaType->create(nullptr);
                                        else
                                            ptr = operator new(size);
                                        if(qtjambi_convert_to_native(__jni_env, *typeId, val, ptr)){
                                            arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                            cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                        }else{
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                        }
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }else if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                                }else{
                                    if(size>0){
                                        if(argMetaType){
                                            ptr = argMetaType->create(nullptr);
                                            if(qtjambi_convert_to_native(__jni_env, *typeId, val, ptr)){
                                                arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                                cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[argMetaType, ptr](){ argMetaType->destroy(ptr); }}});
                                                if(argPointerOrReference!=-2 || isReferenceMetaType){
                                                    if(Java::QtJambi::QFlags::isInstanceOf(__jni_env, val)){
                                                        successActions.append([arg, val](JNIEnv* env){
                                                            if(int* flag = reinterpret_cast<int*>(Java::JNA::Pointer::peer(env, arg))){
                                                                Java::QtJambi::QFlags::setValue(env, val, *flag);
                                                            }
                                                        });
                                                    }
                                                }
                                            }else{
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            }
                                        }else{
                                            if(PtrDeleterFunction dlt = deleter(*typeId)){
                                                ptr = operator new(size);
                                                if(qtjambi_convert_to_native(__jni_env, *typeId, val, ptr)){
                                                    arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                                    cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[dlt, ptr](){ dlt(ptr,false); }}});
                                                }else{
                                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                }
                                            }else{
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            }
                                        }
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }
                                resolved = true;
                            }
                            break;
                        default:
                            break;
                        }
                    }
                    if(!resolved){
                        if(argPointerOrReference!=0 || isReferenceMetaType){
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                arg = Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                            }else{
                                arg = Java::JNA::Pointer::newInstance(__jni_env, val);
                            }
                        }else if(argMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                            if(argMetaType->name().endsWith("*")){
#else
                            if(argMetaType->flags() & QMetaType::IsPointer){
#endif
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                                }else if(typeId && qtjambi_convert_to_native(__jni_env, *typeId, arg, &ptr)){
                                    arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                }else{
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                }
                            }else{
                                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                    ptr = link->pointer();
                                    if(argPointerOrReference==0 && !isReferenceMetaType){
                                        ptr = argMetaType->create(ptr);
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                    }
                                }else{
                                    ptr = argMetaType->create(nullptr);
                                    if(!typeId || !qtjambi_convert_to_native(__jni_env, *typeId, val, ptr)){
                                        if(Java::Runtime::Map::isInstanceOf(__jni_env, val) || Java::Runtime::Collection::isInstanceOf(__jni_env, val)){
                                            QVariant variant = qtjambi_to_qvariant(__jni_env, val);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                            if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
#else
                                            if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
#endif
                                                argMetaType->destruct(ptr);
                                                argMetaType->construct(ptr, variant.data());
                                            }else{
                                                argMetaType->destroy(ptr);
                                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                                return;
                                            }
                                        }else{
                                            argMetaType->destroy(ptr);
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, val).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                            return;
                                        }
                                    }
                                    if(argPointerOrReference==0 && !isReferenceMetaType){
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        arg = Java::JNA::Pointer::newInstance(__jni_env, ptr);
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[argMetaType, ptr](){ argMetaType->destroy(ptr); }}});
                                    }
                                }
                            }
                        }else{
                            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, val)){
                                if(PointerToObjectLink* plink = dynamic_cast<PointerToObjectLink*>(link.get())){
                                    argMetaType = &plink->metaType();
                                    if(argMetaType->isValid()){
                                        ptr = argMetaType->create(link->pointer());
                                        arg = __jni_env->NewDirectByteBuffer(ptr, jsize(size));
                                        cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[ptr](){ operator delete (ptr); }}});
                                    }else{
                                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }
                            }else{
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            }
                        }
                    }
                }// all others are allowed
            }
        }
        Q_UNUSED(resolved)
        __jni_env->SetObjectArrayElement(arguments, i, arg);
    }
}

#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) env QTJAMBI_STACKTRACEINFO
#else
#define QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env)
#endif

jobject qtjambi_invoke_function_pointer(JNIEnv * __jni_env, QFunctionPointer __qt_this, jobject returnType, jobjectArray arguments){
    jsize length = __jni_env->IsSameObject(arguments, nullptr) ? 0 : __jni_env->GetArrayLength(arguments);
    QMetaType* returnMetaType = nullptr;
    jint returnPointerOrReference = 0;
    jclass returnClassType = nullptr;
    const std::type_info* returnTypeId = nullptr;
    jclass jnaReturnType = nullptr;
    bool isFunctionPointer = false;
    bool isReferenceMetaType = false;
    if(Java::QtCore::QGenericArgumentType::isInstanceOf(__jni_env, returnType)){
        returnMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, returnType));
        if(returnMetaType && !returnMetaType->isValid())
            returnMetaType = nullptr;
        if(returnMetaType)
            isReferenceMetaType = QByteArray(returnMetaType->name()).endsWith("&");
        returnPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, returnType);
        returnClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, returnType);
    }else if(Java::Runtime::Class::isInstanceOf(__jni_env, returnType)){
        returnClassType = jclass(returnType);
    }else{
        returnType = nullptr;
    }
    bool returnValueResolved = false;
    if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, nullptr)
            || __jni_env->IsSameObject(returnClassType, Java::Runtime::Void::getClass(__jni_env))
            || __jni_env->IsSameObject(returnClassType, Java::Runtime::Void::primitiveType(__jni_env))))
            || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Void)){
        if(length==0){
            (*__qt_this)();
            return nullptr;
        }
        jnaReturnType = Java::Runtime::Void::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Integer::getClass(__jni_env))
                                || __jni_env->IsSameObject(returnClassType, Java::Runtime::Integer::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Int
                                                                    || returnMetaType->id()==QMetaType::UInt
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                                    || returnMetaType->id()==QMetaType::Char32
#endif
                                                                    ))){
        if(length==0)
            return qtjambi_from_int(__jni_env, reinterpret_cast<jint(*)()>(*__qt_this)());
        jnaReturnType = Java::Runtime::Integer::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if(!returnMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(length==0)
            return qtjambi_from_enum(__jni_env, reinterpret_cast<qint32(*)()>(*__qt_this)(), returnClassType);
        jnaReturnType = Java::Runtime::Integer::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if(!returnMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(length==0)
            return qtjambi_from_enum(__jni_env, reinterpret_cast<qint8(*)()>(*__qt_this)(), returnClassType);
        jnaReturnType = Java::Runtime::Byte::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if(!returnMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(length==0)
            return qtjambi_from_enum(__jni_env, reinterpret_cast<qint16(*)()>(*__qt_this)(), returnClassType);
        jnaReturnType = Java::Runtime::Short::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if(!returnMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(length==0)
            return qtjambi_from_enum(__jni_env, reinterpret_cast<qint64(*)()>(*__qt_this)(), returnClassType);
        jnaReturnType = Java::Runtime::Long::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Byte::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Byte::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Char
                                                                    || returnMetaType->id()==QMetaType::UChar
                                                                    || returnMetaType->id()==QMetaType::SChar ))){
        if(length==0)
            return qtjambi_from_byte(__jni_env, reinterpret_cast<jbyte(*)()>(*__qt_this)());
        jnaReturnType = Java::Runtime::Byte::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Short::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Short::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Short
                                                                    || returnMetaType->id()==QMetaType::UShort))){
        if(length==0)
            return qtjambi_from_short(__jni_env, reinterpret_cast<jshort(*)()>(*__qt_this)());
        jnaReturnType = Java::Runtime::Short::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Long::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Long::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::LongLong
                                                                    || returnMetaType->id()==QMetaType::ULongLong))){
        if(length==0)
            return qtjambi_from_long(__jni_env, reinterpret_cast<jlong(*)()>(*__qt_this)());
        jnaReturnType = Java::Runtime::Long::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Float::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Float::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Float)){
        if(length==0)
            return qtjambi_from_float(__jni_env, reinterpret_cast<jfloat(*)()>(*__qt_this)());
        jnaReturnType = Java::Runtime::Float::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Double::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Double::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Double)){
        if(length==0)
            return qtjambi_from_double(__jni_env, reinterpret_cast<jdouble(*)()>(*__qt_this)());
        jnaReturnType = Java::Runtime::Double::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Boolean::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Boolean::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Bool)){
        if(length==0)
            return qtjambi_from_boolean(__jni_env, reinterpret_cast<jboolean(*)()>(*__qt_this)());
        jnaReturnType = Java::Runtime::Boolean::primitiveType(__jni_env);
        returnValueResolved = true;
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Character::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Character::primitiveType(__jni_env))))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16)
#endif
             ){
        if(length==0)
            return qtjambi_from_char(__jni_env, reinterpret_cast<jchar(*)()>(*__qt_this)());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16){
            jnaReturnType = Java::Runtime::Character::primitiveType(__jni_env);
            returnValueResolved = true;
        }
#endif
    }
    try{
        Java::JNA::Native::getClass(__jni_env);
    }catch(const JavaException& exn){
        JavaException exn2(__jni_env, Java::QtJambi::QUnsuccessfulInvocationException::newInstanceWithMessage(__jni_env, "Unable to load Java Native Access library. Please add JNA to class path."));
        exn2.addSuppressed(__jni_env, exn);
        exn2.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(__jni_env) );
    }
    ffi_type __ffi_type;

    QScopedPointer<QMetaType> _returnMetaType;
    if(!returnValueResolved){
        size_t size = 0;
        short alignment = 1;
        const SuperTypeInfos& infos = getSuperTypeInfos(__jni_env, returnClassType);
        if(infos.isEmpty()){
            if(!returnMetaType)
                _returnMetaType.reset(returnMetaType = new QMetaType(qtjambi_register_metatype(__jni_env, returnClassType, false, false)));
            if(!returnMetaType->isValid())
                _returnMetaType.reset(returnMetaType = nullptr);
            if(returnMetaType){
                returnTypeId = getTypeByMetaType(*returnMetaType);
                size = size_t(returnMetaType->sizeOf());
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                alignment = short(returnMetaType->alignOf());
#endif
            }
        }else{
            returnTypeId = &infos.first().typeId();
            size = infos.first().size();
            alignment = short(getValueAlignment(*returnTypeId));
        }
        if(returnTypeId){
            switch(getEntryType(*returnTypeId)){
            case EntryTypes::FunctionalTypeInfo:
                if(const QtJambiTypeInfo* info = getQTypeInfo(*returnTypeId)){
                    if(info->isPointer){
                        jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                        isFunctionPointer = true;
                    }else{
                        if(returnPointerOrReference==0 && !isReferenceMetaType){
                            if(size==0)
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            __ffi_type.size = size;
                            __ffi_type.alignment = alignment;
                            if(!returnMetaType)
                                _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                            if(!returnMetaType->isValid())
                                _returnMetaType.reset(returnMetaType = nullptr);
                        }else{
                            jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                        }
                    }
                }else{
                    jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                }
                break;
            case EntryTypes::ObjectTypeInfo:
            case EntryTypes::InterfaceTypeInfo:
            case EntryTypes::QObjectTypeInfo:
                jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                break;
            case EntryTypes::FlagsTypeInfo:
            case EntryTypes::InterfaceValueTypeInfo:
            case EntryTypes::ValueTypeInfo:
            case EntryTypes::SpecialTypeInfo:
            case EntryTypes::StringTypeInfo:
            case EntryTypes::PrimitiveTypeInfo:
                if(returnPointerOrReference==0 && !isReferenceMetaType){
                    if(size==0)
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    __ffi_type.size = size;
                    __ffi_type.alignment = alignment;
                    if(!returnMetaType)
                        _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                    if(!returnMetaType->isValid())
                        _returnMetaType.reset(returnMetaType = nullptr);
                }else{
                    jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                }
                break;
            default:
                if(!returnMetaType)
                    _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                if(!returnMetaType->isValid())
                    _returnMetaType.reset(returnMetaType = nullptr);
                if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if(returnMetaType->name().endsWith("*")){
#else
                    if(returnMetaType->flags() & QMetaType::IsPointer){
#endif
                        jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
                    }else{
                        __ffi_type.size = size;
                        __ffi_type.alignment = alignment;
                    }
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
                break;
            }
        }else if(returnPointerOrReference!=0 || isReferenceMetaType){
            jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
        }else if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            if(returnMetaType->name().endsWith("*")){
#else
            if(returnMetaType->flags() & QMetaType::IsPointer){
#endif
                jnaReturnType = Java::JNA::Pointer::getClass(__jni_env);
            }else{
                __ffi_type.size = size;
                __ffi_type.alignment = alignment;
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
            return nullptr;
        }
    }
    QVector<QSharedDataPointer<Cleanup>> cleaners;
    QVector<std::function<void(JNIEnv *)>> successActions;
    convertArgumentList(cleaners, successActions, __jni_env, arguments, length);
    jobject jnaFunction = Java::JNA::Function::getFunction(__jni_env, Java::JNA::Pointer::newInstance(__jni_env, jlong(__qt_this)));
    jobject result = nullptr;
    if(__ffi_type.size>0){
        if(__ffi_type.alignment == 0)
            __ffi_type.alignment = 1;
        const ffi_type* element = nullptr;
        __ffi_type.elements = &element;
        void* ptr;
        bool isAllocated = false;
#ifdef Q_OS_WINDOWS
        __ffi_type.size += (__ffi_type.size % 8) + 1;
        ptr = operator new(__ffi_type.size);
        isAllocated = true;
        memset(ptr, 0, __ffi_type.size);
        //qtjambi_funtion_dispatch(__jni_env, reinterpret_cast<void*>(__qt_this), arguments, &__ffi_type, ptr);
        Java::JNA::Native::invokeStructure(__jni_env, jnaFunction, jlong(__qt_this), 0, arguments, jlong(ptr), jlong(&__ffi_type));
#else
        jvalue val;
        switch(__ffi_type.size){
        case 1:
            val.b = Java::Runtime::Number::byteValue(__jni_env, Java::JNA::Function::invoke(__jni_env, jnaFunction, Java::Runtime::Byte::primitiveType(__jni_env), arguments));
            ptr = &val.b;
            break;
        case 2:
            val.s = Java::Runtime::Number::shortValue(__jni_env, Java::JNA::Function::invoke(__jni_env, jnaFunction, Java::Runtime::Short::primitiveType(__jni_env), arguments));
            ptr = &val.s;
            break;
        case 4:
            val.i = Java::Runtime::Number::intValue(__jni_env, Java::JNA::Function::invoke(__jni_env, jnaFunction, Java::Runtime::Integer::primitiveType(__jni_env), arguments));
            ptr = &val.i;
            break;
        case 8:
            val.j = Java::Runtime::Number::longValue(__jni_env, Java::JNA::Function::invoke(__jni_env, jnaFunction, Java::Runtime::Long::primitiveType(__jni_env), arguments));
            ptr = &val.j;
            break;
        default:
            if(returnMetaType){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported as return type.").arg(QLatin1String(returnMetaType->name())) QTJAMBI_STACKTRACEINFO );
            }else{
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported as return type.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
            }
            ptr = operator new(__ffi_type.size);
            isAllocated = true;
            memset(ptr, 0, __ffi_type.size);
            //qtjambi_funtion_dispatch(__jni_env, reinterpret_cast<void*>(__qt_this), arguments, &__ffi_type, ptr);
            Java::JNA::Native::invokeStructure(__jni_env, jnaFunction, jlong(__qt_this), 0, arguments, jlong(ptr), jlong(&__ffi_type));
            break;
        }
#endif
        if(returnTypeId){
            result = qtjambi_from_object(__jni_env, ptr, nullptr, *returnTypeId, !isAllocated, false);
        }else {
            result = qtjambi_from_object(__jni_env, ptr, returnClassType, !isAllocated, false);
        }
        if(result && returnClassType && __jni_env->IsInstanceOf(result, returnClassType)){
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
                if(link->pointer()!=ptr){
                    if(returnMetaType)
                        returnMetaType->destruct(ptr);
                    if(isAllocated)
                        operator delete(ptr);
                }
            }else{
                if(returnMetaType)
                    returnMetaType->destruct(ptr);
                if(isAllocated)
                    operator delete(ptr);
            }
        }else if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
            result = qtjambi_from_qvariant(__jni_env, QVariant(returnMetaType->id(), ptr));
#else
            result = qtjambi_from_qvariant(__jni_env, QVariant(*returnMetaType, ptr));
#endif
        }
    }else{
        result = Java::JNA::Function::invoke(__jni_env, jnaFunction, jnaReturnType, arguments);
        if(Java::JNA::Pointer::isSameClass(__jni_env, jnaReturnType)){
            if(Java::JNA::Pointer::isInstanceOf(__jni_env, result)){
                void* ptr = reinterpret_cast<void*>(Java::JNA::Pointer::peer(__jni_env, result));
                if(returnPointerOrReference==2 && returnClassType){
                    QFunctionPointer fp = reinterpret_cast<QFunctionPointer>(ptr);
                    bool resolved = false;
                    {
                        QReadLocker locker(gPointerLock());
                        if(gObjectsByFunctionPointer->contains(quintptr(fp))){
                            QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(fp)];
                            for(JObjectWrapper& wrapper : objects){
                                if(__jni_env->IsInstanceOf(wrapper.object(), returnClassType)){
                                    result = __jni_env->NewLocalRef(wrapper.object());
                                    resolved = true;
                                }
                            }
                        }
                    }
                    if(!resolved){
                        result = Java::QtCore::QFunctionPointerUtil::createProxy(__jni_env, returnClassType);
                        QtJambiLink::createLinkForObject(
                                __jni_env,
                                result,
                                new QFunctionPointer(fp),
                                LINK_NAME_ARG(nullptr)
                                false,
                                false,
                                &delete_callback_pointer
                            )->setJavaOwnership(__jni_env);
                        QWriteLocker locker(gPointerLock());
                        (*gObjectsByFunctionPointer())[quintptr(fp)] << JObjectWrapper(__jni_env, result);
                    }
                }else{
                    void* _ptr = ptr;
                    if(isFunctionPointer){
                        _ptr = &ptr;
                    }
                    if(returnTypeId){
                        result = qtjambi_from_object(__jni_env, _ptr, nullptr, *returnTypeId, isFunctionPointer || (!isReferenceMetaType && (returnPointerOrReference==0 || returnPointerOrReference==-2)), false);
                    }else {
                        result = qtjambi_from_object(__jni_env, _ptr, returnClassType, isFunctionPointer || (!isReferenceMetaType && (returnPointerOrReference==0 || returnPointerOrReference==-2)), false);
                    }
                }
            }
        }else if(!returnMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, returnClassType)){
            if(Java::Runtime::Number::isInstanceOf(__jni_env, result)){
                result = qtjambi_from_enum(__jni_env, qint32(Java::Runtime::Number::intValue(__jni_env, result)), returnClassType);
            }else
                result = nullptr;
        }else if(!returnMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, returnClassType)){
            if(Java::Runtime::Number::isInstanceOf(__jni_env, result)){
                result = qtjambi_from_enum(__jni_env, qint16(Java::Runtime::Number::shortValue(__jni_env, result)), returnClassType);
            }else
                result = nullptr;
        }else if(!returnMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, returnClassType)){
            if(Java::Runtime::Number::isInstanceOf(__jni_env, result)){
                result = qtjambi_from_enum(__jni_env, qint8(Java::Runtime::Number::byteValue(__jni_env, result)), returnClassType);
            }else
                result = nullptr;
        }else if(!returnMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, returnClassType)){
            if(Java::Runtime::Number::isInstanceOf(__jni_env, result)){
                result = qtjambi_from_enum(__jni_env, qint64(Java::Runtime::Number::longValue(__jni_env, result)), returnClassType);
            }else
                result = nullptr;
        }
    }
    for(auto action : successActions){
        action(__jni_env);
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO );
    }
    return result;
}

jclass qtjambi_function_pointer_return_type(JNIEnv * __jni_env, jobject returnType){
    QMetaType* returnMetaType = nullptr;
    jint returnPointerOrReference = 0;
    jclass returnClassType = nullptr;
    const std::type_info* returnTypeId = nullptr;
    bool isReferenceMetaType = false;
    returnMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, returnType));
    if(returnMetaType && !returnMetaType->isValid())
        returnMetaType = nullptr;
    if(returnMetaType)
        isReferenceMetaType = QByteArray(returnMetaType->name()).endsWith("&");
    returnPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, returnType);
    returnClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, returnType);
    if(returnPointerOrReference==2 && returnClassType){
        return Java::JNA::Pointer::getClass(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, nullptr)
            || __jni_env->IsSameObject(returnClassType, Java::Runtime::Void::getClass(__jni_env))
            || __jni_env->IsSameObject(returnClassType, Java::Runtime::Void::primitiveType(__jni_env))))
            || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Void)){
        return Java::Runtime::Void::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Integer::getClass(__jni_env))
                                || __jni_env->IsSameObject(returnClassType, Java::Runtime::Integer::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Int
                                                                    || returnMetaType->id()==QMetaType::UInt
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                                    || returnMetaType->id()==QMetaType::Char32
#endif
                                                                    ))){
        return Java::Runtime::Integer::primitiveType(__jni_env);
    }else if(!returnMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        return Java::Runtime::Integer::primitiveType(__jni_env);
    }else if(!returnMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        return Java::Runtime::Byte::primitiveType(__jni_env);
    }else if(!returnMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        return Java::Runtime::Short::primitiveType(__jni_env);
    }else if(!returnMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        return Java::Runtime::Long::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Byte::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Byte::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Char
                                                                    || returnMetaType->id()==QMetaType::UChar
                                                                    || returnMetaType->id()==QMetaType::SChar ))){
        return Java::Runtime::Byte::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Short::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Short::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Short
                                                                    || returnMetaType->id()==QMetaType::UShort))){
        return Java::Runtime::Short::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Long::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Long::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::LongLong
                                                                    || returnMetaType->id()==QMetaType::ULongLong))){
        return Java::Runtime::Long::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Float::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Float::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Float)){
        return Java::Runtime::Float::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Double::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Double::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Double)){
        return Java::Runtime::Double::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Boolean::getClass(__jni_env))
                                     || __jni_env->IsSameObject(returnClassType, Java::Runtime::Boolean::primitiveType(__jni_env))))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Bool)){
        return Java::Runtime::Boolean::primitiveType(__jni_env);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, Java::Runtime::Character::getClass(__jni_env))
                                    || __jni_env->IsSameObject(returnClassType, Java::Runtime::Character::primitiveType(__jni_env))))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16)
#endif
             ){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16){
            return Java::Runtime::Character::primitiveType(__jni_env);
        }
#endif
    }

    QScopedPointer<QMetaType> _returnMetaType;
    size_t size = 0;
    const SuperTypeInfos& infos = getSuperTypeInfos(__jni_env, returnClassType);
    if(infos.isEmpty()){
        if(!returnMetaType)
            _returnMetaType.reset(returnMetaType = new QMetaType(qtjambi_register_metatype(__jni_env, returnClassType, false, false)));
        if(!returnMetaType->isValid())
            _returnMetaType.reset(returnMetaType = nullptr);
        if(returnMetaType){
            returnTypeId = getTypeByMetaType(*returnMetaType);
            size = size_t(returnMetaType->sizeOf());
        }
    }else{
        returnTypeId = &infos.first().typeId();
        size = infos.first().size();
    }
    if(returnTypeId){
        switch(getEntryType(*returnTypeId)){
        case EntryTypes::FunctionalTypeInfo:
            if(const QtJambiTypeInfo* info = getQTypeInfo(*returnTypeId)){
                if(info->isPointer){
                    return Java::JNA::Pointer::getClass(__jni_env);
                }else{
                    if(returnPointerOrReference==0 && !isReferenceMetaType){
                        switch(size){
                        case sizeof(jbyte):
                            return Java::Runtime::Byte::primitiveType(__jni_env);
                        case sizeof(jshort):
                            return Java::Runtime::Short::primitiveType(__jni_env);
                        case sizeof(jint):
                            return Java::Runtime::Integer::primitiveType(__jni_env);
                        case sizeof(jlong):
                            return Java::Runtime::Long::primitiveType(__jni_env);
                        default:
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            return nullptr;
                        }
                    }else{
                        return Java::JNA::Pointer::getClass(__jni_env);
                    }
                }
            }else{
                return Java::JNA::Pointer::getClass(__jni_env);
            }
        case EntryTypes::ObjectTypeInfo:
        case EntryTypes::InterfaceTypeInfo:
        case EntryTypes::QObjectTypeInfo:
            return Java::JNA::Pointer::getClass(__jni_env);
        case EntryTypes::FlagsTypeInfo:
        case EntryTypes::InterfaceValueTypeInfo:
        case EntryTypes::ValueTypeInfo:
        case EntryTypes::SpecialTypeInfo:
        case EntryTypes::StringTypeInfo:
        case EntryTypes::PrimitiveTypeInfo:
            if(returnPointerOrReference==0 && !isReferenceMetaType){
                switch(size){
                case sizeof(jbyte):
                    return Java::Runtime::Byte::primitiveType(__jni_env);
                case sizeof(jshort):
                    return Java::Runtime::Short::primitiveType(__jni_env);
                case sizeof(jint):
                    return Java::Runtime::Integer::primitiveType(__jni_env);
                case sizeof(jlong):
                    return Java::Runtime::Long::primitiveType(__jni_env);
                default:
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }else{
                return Java::JNA::Pointer::getClass(__jni_env);
            }
        default:
            if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                if(returnMetaType->name().endsWith("*")){
#else
                if(returnMetaType->flags() & QMetaType::IsPointer){
#endif
                    return Java::JNA::Pointer::getClass(__jni_env);
                }else{
                    switch(returnMetaType->sizeOf()){
                    case sizeof(jbyte):
                        return Java::Runtime::Byte::primitiveType(__jni_env);
                    case sizeof(jshort):
                        return Java::Runtime::Short::primitiveType(__jni_env);
                    case sizeof(jint):
                        return Java::Runtime::Integer::primitiveType(__jni_env);
                    case sizeof(jlong):
                        return Java::Runtime::Long::primitiveType(__jni_env);
                    default:
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }
            }else{
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
        }
    }else if(returnPointerOrReference!=0 || isReferenceMetaType){
        return Java::JNA::Pointer::getClass(__jni_env);
    }else if(returnMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
        if(returnMetaType->name().endsWith("*")){
#else
        if(returnMetaType->flags() & QMetaType::IsPointer){
#endif
            return Java::JNA::Pointer::getClass(__jni_env);
        }else{
            switch(returnMetaType->sizeOf()){
            case sizeof(jbyte):
                return Java::Runtime::Byte::primitiveType(__jni_env);
            case sizeof(jshort):
                return Java::Runtime::Short::primitiveType(__jni_env);
            case sizeof(jint):
                return Java::Runtime::Integer::primitiveType(__jni_env);
            case sizeof(jlong):
                return Java::Runtime::Long::primitiveType(__jni_env);
            default:
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                return nullptr;
            }
        }
    }else{
        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        return nullptr;
    }
}

jobject qtjambi_function_pointer_convert_return(JNIEnv * __jni_env, jobject returnType, jobject result){
    const QMetaType* returnMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, returnType));
    if(returnMetaType && !returnMetaType->isValid())
        returnMetaType = nullptr;
    bool isReferenceMetaType = false;
    if(returnMetaType)
        isReferenceMetaType = QByteArray(returnMetaType->name()).endsWith("&");
    jint returnPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, returnType);
    jclass returnClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, returnType);
    if(returnPointerOrReference==2 && returnClassType){
        QFunctionPointer ptr = nullptr;
        if(result){
            bool resolved = false;
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
                if(DeletablePointerToObjectLink* plink = dynamic_cast<DeletablePointerToObjectLink*>(link.get())){
                    if(plink->pointer() && plink->deleterFunction()==&delete_callback_pointer){
                        ptr = *reinterpret_cast<QFunctionPointer*>(plink->pointer());
                        resolved = true;
                    }
                }
            }
            if(!resolved){
                jobject ih = Java::QtCore::QFunctionPointerUtil::createCallbackProxy(__jni_env, Java::JNA::CallbackProxy::getClass(__jni_env), returnClassType, result);
                jobject callbackProxy = Java::QtCore::QFunctionPointerUtil$CppToJavaInvocationHandler::proxy(__jni_env, ih);
                jobject method = __jni_env->ToReflectedMethod(Java::JNA::CallbackProxy::getClass(__jni_env), Java::JNA::CallbackProxy::callbackMethod_ID(__jni_env), false);
                jlong peer = Java::JNA::Native::createNativeCallback(__jni_env,
                                                              callbackProxy,
                                                              method,
                                                              Java::JNA::CallbackProxy::getParameterTypes(__jni_env, callbackProxy),
                                                              Java::JNA::CallbackProxy::getReturnType(__jni_env, callbackProxy),
                                                              0,
                                                              0,
                                                              __jni_env->NewStringUTF("UTF-8"));
                ptr = *reinterpret_cast<QFunctionPointer*>(peer);
                QFunctionPointer fp = reinterpret_cast<QFunctionPointer>(ptr);
                QtJambiLink::createLinkForObject(
                        __jni_env,
                        result,
                        new QFunctionPointer(ptr),
                        LINK_NAME_ARG(nullptr)
                        false,
                        false,
                        &delete_callback_pointer
                    )->setJavaOwnership(__jni_env);
                {
                    QWriteLocker locker(gPointerLock());
                    (*gObjectsByFunctionPointer())[quintptr(fp)] << JObjectWrapper(__jni_env, result);
                }
                Java::QtCore::QFunctionPointerUtil$CppToJavaInvocationHandler::set_peer(__jni_env, ih, peer);
                Java::QtCore::QFunctionPointerUtil::registerCleanup(__jni_env, result, ih);
            }
        }
        return Java::JNA::Pointer::newInstance(__jni_env, ptr);
    }else if((!returnMetaType && (__jni_env->IsSameObject(returnClassType, nullptr)
            || Java::Runtime::Void::isSameClass(__jni_env, returnClassType)
            || Java::Runtime::Void::isPrimitiveType(__jni_env, returnClassType)))
            || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Void)){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Integer::isSameClass(__jni_env, returnClassType)
                                || Java::Runtime::Integer::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Int
                                                                    || returnMetaType->id()==QMetaType::UInt
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                                                                    || returnMetaType->id()==QMetaType::Char32
#endif
                                                                    ))){
        return result;
    }else if(!returnMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(Java::QtJambi::QtEnumerator::isInstanceOf(__jni_env, result)){
            return Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QtEnumerator::value(__jni_env, result));
        }else
            return nullptr;
    }else if(!returnMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(Java::QtJambi::QtShortEnumerator::isInstanceOf(__jni_env, result)){
            return Java::Runtime::Short::valueOf(__jni_env, Java::QtJambi::QtShortEnumerator::value(__jni_env, result));
        }else
            return nullptr;
    }else if(!returnMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(Java::QtJambi::QtByteEnumerator::isInstanceOf(__jni_env, result)){
            return Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QtByteEnumerator::value(__jni_env, result));
        }else
            return nullptr;
    }else if(!returnMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, returnClassType)){
        if(Java::QtJambi::QtLongEnumerator::isInstanceOf(__jni_env, result)){
            return Java::Runtime::Integer::valueOf(__jni_env, Java::QtJambi::QtLongEnumerator::value(__jni_env, result));
        }else
            return nullptr;
    }else if((!returnMetaType && (Java::Runtime::Byte::isSameClass(__jni_env, returnClassType)
                                    || Java::Runtime::Byte::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Char
                                                                    || returnMetaType->id()==QMetaType::UChar
                                                                    || returnMetaType->id()==QMetaType::SChar ))){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Short::isSameClass(__jni_env, returnClassType)
                                    || Java::Runtime::Short::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::Short
                                                                    || returnMetaType->id()==QMetaType::UShort))){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Long::isSameClass(__jni_env, returnClassType)
                                     || Java::Runtime::Long::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && ( returnMetaType->id()==QMetaType::LongLong
                                                                    || returnMetaType->id()==QMetaType::ULongLong))){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Float::isSameClass(__jni_env, returnClassType)
                                    || Java::Runtime::Float::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Float)){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Double::isSameClass(__jni_env, returnClassType)
                                     || Java::Runtime::Double::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Double)){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Boolean::isSameClass(__jni_env, returnClassType)
                                     || Java::Runtime::Boolean::isPrimitiveType(__jni_env, returnClassType)))
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Bool)){
        return result;
    }else if((!returnMetaType && (Java::Runtime::Character::isSameClass(__jni_env, returnClassType)
                                    || Java::Runtime::Character::isPrimitiveType(__jni_env, returnClassType)))
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
             || (returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16)
#endif
             ){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if(returnMetaType && returnPointerOrReference==0 && !isReferenceMetaType && returnMetaType->id()==QMetaType::Char16){
            return result;
        }
#endif
    }
    const std::type_info* returnTypeId = nullptr;

    QScopedPointer<const QMetaType> _returnMetaType;
    const SuperTypeInfos& infos = getSuperTypeInfos(__jni_env, returnClassType);
    if(infos.isEmpty()){
        if(!returnMetaType)
            _returnMetaType.reset(returnMetaType = new QMetaType(qtjambi_register_metatype(__jni_env, returnClassType, false, false)));
        if(!returnMetaType->isValid())
            _returnMetaType.reset(returnMetaType = nullptr);
        if(returnMetaType){
            returnTypeId = getTypeByMetaType(*returnMetaType);
        }
    }else{
        returnTypeId = &infos.first().typeId();
    }
    void* ptr = nullptr;
    if(returnTypeId){
        bool isValueFunctional = false;
        switch(getEntryType(*returnTypeId)){
        case EntryTypes::FunctionalTypeInfo:
            isValueFunctional = true;
            if(const QtJambiTypeInfo* info = getQTypeInfo(*returnTypeId)){
                if(info->isPointer){
                    if(qtjambi_convert_to_native(__jni_env, *returnTypeId, result, &ptr)){
                        return Java::JNA::Pointer::newInstance(__jni_env, *reinterpret_cast<void**>(ptr));
                    }else{
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }
            }
            Q_FALLTHROUGH();
        case EntryTypes::ObjectTypeInfo:
        case EntryTypes::InterfaceTypeInfo:
        case EntryTypes::QObjectTypeInfo:
            if(!isValueFunctional){
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
                    return Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
                }else if(qtjambi_convert_to_native(__jni_env, *returnTypeId, result, &ptr)){
                    return Java::JNA::Pointer::newInstance(__jni_env, ptr);
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }
            Q_FALLTHROUGH();
        case EntryTypes::FlagsTypeInfo:
        case EntryTypes::InterfaceValueTypeInfo:
        case EntryTypes::ValueTypeInfo:
        case EntryTypes::SpecialTypeInfo:
        case EntryTypes::StringTypeInfo:
        case EntryTypes::PrimitiveTypeInfo:
            if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
                ptr = link->pointer();
                if(!returnMetaType){
                    if(PointerToObjectLink* plink = dynamic_cast<PointerToObjectLink*>(link.get())){
                        returnMetaType = &plink->metaType();
                        if(!returnMetaType->isValid())
                            returnMetaType = nullptr;
                    }
                }
                if(returnPointerOrReference==0 && !isReferenceMetaType){
                    if(!returnMetaType)
                        _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                    if(!returnMetaType->isValid())
                        _returnMetaType.reset(returnMetaType = nullptr);
                    if(returnMetaType){
                        switch(returnMetaType->sizeOf()){
                        case sizeof(jbyte):{
                                jbyte data = 0;
                                returnMetaType->construct(&data, ptr);
                                return Java::Runtime::Byte::valueOf(__jni_env, data);
                            }
                        case sizeof(jshort):{
                                jshort data = 0;
                                returnMetaType->construct(&data, ptr);
                                return Java::Runtime::Short::valueOf(__jni_env, data);
                            }
                        case sizeof(jint):{
                                jint data = 0;
                                returnMetaType->construct(&data, ptr);
                                return Java::Runtime::Integer::valueOf(__jni_env, data);
                            }
                        case sizeof(jlong):{
                                jlong data = 0;
                                returnMetaType->construct(&data, ptr);
                                return Java::Runtime::Long::valueOf(__jni_env, data);
                            }
                        default:
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    return Java::JNA::Pointer::newInstance(__jni_env, ptr);
                }
            }else{
                if(!returnMetaType)
                    _returnMetaType.reset(returnMetaType = new QMetaType(registeredMetaTypeID(*returnTypeId)));
                if(!returnMetaType->isValid())
                    _returnMetaType.reset(returnMetaType = nullptr);
                if(returnMetaType){
                    ptr = returnMetaType->create(nullptr);
                    if(!qtjambi_convert_to_native(__jni_env, *returnTypeId, result, ptr)){
                        if(Java::Runtime::Map::isInstanceOf(__jni_env, result) || Java::Runtime::Collection::isInstanceOf(__jni_env, result)){
                            QVariant variant = qtjambi_to_qvariant(__jni_env, result);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                            if(variant.metaType()==*returnMetaType || variant.convert(*returnMetaType)){
#else
                            if(variant.userType()==returnMetaType->id() || variant.convert(returnMetaType->id())){
#endif
                                returnMetaType->destruct(ptr);
                                returnMetaType->construct(ptr, variant.data());
                            }else{
                                returnMetaType->destroy(ptr);
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                return nullptr;
                            }
                        }else{
                            returnMetaType->destroy(ptr);
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            return nullptr;
                        }
                    }
                    if(returnPointerOrReference==0 && !isReferenceMetaType){
                        switch(returnMetaType->sizeOf()){
                        case sizeof(jbyte):{
                                jbyte data = 0;
                                returnMetaType->construct(&data, ptr);
                                returnMetaType->destroy(ptr);
                                return Java::Runtime::Byte::valueOf(__jni_env, data);
                            }
                        case sizeof(jshort):{
                                jshort data = 0;
                                returnMetaType->construct(&data, ptr);
                                returnMetaType->destroy(ptr);
                                return Java::Runtime::Short::valueOf(__jni_env, data);
                            }
                        case sizeof(jint):{
                                jint data = 0;
                                returnMetaType->construct(&data, ptr);
                                returnMetaType->destroy(ptr);
                                return Java::Runtime::Integer::valueOf(__jni_env, data);
                            }
                        case sizeof(jlong):{
                                jlong data = 0;
                                returnMetaType->construct(&data, ptr);
                                returnMetaType->destroy(ptr);
                                return Java::Runtime::Long::valueOf(__jni_env, data);
                            }
                        default:
                            returnMetaType->destroy(ptr);
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        return Java::JNA::Pointer::newInstance(__jni_env, ptr);
                        //cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[returnMetaType, ptr](){ returnMetaType->destroy(ptr); }}});
                    }
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }
            return nullptr;
        default:
            break;
        }
    }else if(returnPointerOrReference!=0 || isReferenceMetaType){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
            return Java::JNA::Pointer::newInstance(__jni_env, link->pointer());
        }else{
            return Java::JNA::Pointer::newInstance(__jni_env, result);
        }
    }else if(returnMetaType){
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
            ptr = link->pointer();
            if(returnPointerOrReference==0 && !isReferenceMetaType){
                switch(returnMetaType->sizeOf()){
                case sizeof(jbyte):{
                        jbyte data = 0;
                        returnMetaType->construct(&data, ptr);
                        return Java::Runtime::Byte::valueOf(__jni_env, data);
                    }
                case sizeof(jshort):{
                        jshort data = 0;
                        returnMetaType->construct(&data, ptr);
                        return Java::Runtime::Short::valueOf(__jni_env, data);
                    }
                case sizeof(jint):{
                        jint data = 0;
                        returnMetaType->construct(&data, ptr);
                        return Java::Runtime::Integer::valueOf(__jni_env, data);
                    }
                case sizeof(jlong):{
                        jlong data = 0;
                        returnMetaType->construct(&data, ptr);
                        return Java::Runtime::Long::valueOf(__jni_env, data);
                    }
                default:
                    returnMetaType->destroy(ptr);
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                return Java::JNA::Pointer::newInstance(__jni_env, ptr);
            }
        }else{
            ptr = returnMetaType->create(nullptr);
            if(!returnTypeId || !qtjambi_convert_to_native(__jni_env, *returnTypeId, result, ptr)){
                if(Java::Runtime::Map::isInstanceOf(__jni_env, result) || Java::Runtime::Collection::isInstanceOf(__jni_env, result)){
                    QVariant variant = qtjambi_to_qvariant(__jni_env, result);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    if(variant.metaType()==*returnMetaType || variant.convert(*returnMetaType)){
#else
                    if(variant.userType()==returnMetaType->id() || variant.convert(returnMetaType->id())){
#endif
                        returnMetaType->destruct(ptr);
                        returnMetaType->construct(ptr, variant.data());
                    }else{
                        returnMetaType->destroy(ptr);
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        return nullptr;
                    }
                }else{
                    returnMetaType->destroy(ptr);
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, result).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }
            if(returnPointerOrReference==0 && !isReferenceMetaType){
                switch(returnMetaType->sizeOf()){
                case sizeof(jbyte):{
                        jbyte data = 0;
                        returnMetaType->construct(&data, ptr);
                        returnMetaType->destroy(ptr);
                        return Java::Runtime::Byte::valueOf(__jni_env, data);
                    }
                case sizeof(jshort):{
                        jshort data = 0;
                        returnMetaType->construct(&data, ptr);
                        returnMetaType->destroy(ptr);
                        return Java::Runtime::Short::valueOf(__jni_env, data);
                    }
                case sizeof(jint):{
                        jint data = 0;
                        returnMetaType->construct(&data, ptr);
                        returnMetaType->destroy(ptr);
                        return Java::Runtime::Integer::valueOf(__jni_env, data);
                    }
                case sizeof(jlong):{
                        jlong data = 0;
                        returnMetaType->construct(&data, ptr);
                        returnMetaType->destroy(ptr);
                        return Java::Runtime::Long::valueOf(__jni_env, data);
                    }
                default:
                    returnMetaType->destroy(ptr);
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                return Java::JNA::Pointer::newInstance(__jni_env, ptr);
                //cleaners.append(QSharedDataPointer<Cleanup>{new Cleanup{[returnMetaType, ptr](){ returnMetaType->destroy(ptr); }}});
            }
        }
    }else{
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, result)){
            if(PointerToObjectLink* plink = dynamic_cast<PointerToObjectLink*>(link.get())){
                returnMetaType = &plink->metaType();
                if(returnMetaType->isValid()){
                    switch(returnMetaType->sizeOf()){
                    case sizeof(jbyte):{
                            jbyte data = 0;
                            returnMetaType->construct(&data, plink->pointer());
                            return Java::Runtime::Byte::valueOf(__jni_env, data);
                        }
                    case sizeof(jshort):{
                            jshort data = 0;
                            returnMetaType->construct(&data, plink->pointer());
                            return Java::Runtime::Short::valueOf(__jni_env, data);
                        }
                    case sizeof(jint):{
                            jint data = 0;
                            returnMetaType->construct(&data, plink->pointer());
                            return Java::Runtime::Integer::valueOf(__jni_env, data);
                        }
                    case sizeof(jlong):{
                            jlong data = 0;
                            returnMetaType->construct(&data, plink->pointer());
                            return Java::Runtime::Long::valueOf(__jni_env, data);
                        }
                    default:
                        Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                    }
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }
        }else{
            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, returnClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
        }
    }
    return result;
}

void qtjambi_function_pointer_get_parameter_types(JNIEnv * __jni_env, jobjectArray argumentTypes, jobjectArray parameterTypes, jboolean){
    jsize length = __jni_env->GetArrayLength(argumentTypes);
    for(jsize i = 0; i<length; ++i){
        jobject argType = __jni_env->GetObjectArrayElement(argumentTypes, i);
        jint argPointerOrReference = 0;
        const QMetaType* argMetaType = nullptr;
        jclass argClassType = nullptr;
        bool isReferenceMetaType = false;
        QScopedPointer<const QMetaType> _argMetaType;
        argMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, argType));
        if(argMetaType)
            isReferenceMetaType = QByteArray(argMetaType->name()).endsWith("&");
        argPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, argType);
        argClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, argType);
        if(argMetaType && argMetaType->id()==QMetaType::Nullptr){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
            continue;
        }
        if(argPointerOrReference==2 && argClassType){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
            continue;
        }
        if(Java::Runtime::String::isSameClass(__jni_env, argClassType)){
            if(argMetaType){
                if(argMetaType->id()==QMetaType::Char && argPointerOrReference==-2){
                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    continue;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                }else if(argMetaType->id()==QMetaType::Char16 && argPointerOrReference==-2){
                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    continue;
#endif
                }
            }
            if(argPointerOrReference==0){
                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
            }else{
                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
            }
            continue;
        }else if(Java::Runtime::Class::isArray(__jni_env, argClassType)
                 && Java::Runtime::String::isSameClass(__jni_env, Java::Runtime::Class::getComponentType(__jni_env, argClassType))
                 && (!argMetaType || QByteArray("QString&")==argMetaType->name() || QByteArray("QString &")==argMetaType->name()
                                  || QByteArray("QString*")==argMetaType->name() || QByteArray("QString *")==argMetaType->name()
                                  || QByteArray("const char**")==argMetaType->name())){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QFlags::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Byte::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Short::getClass(__jni_env));
            continue;
        }else if(argPointerOrReference==0 && !isReferenceMetaType && Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, argClassType)){
            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Long::getClass(__jni_env));
            continue;
        }else if((argPointerOrReference==0
                  && !isReferenceMetaType
                  && (Java::Runtime::Byte::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Short::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Integer::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Long::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Float::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Double::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Character::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Boolean::isAssignableFrom(__jni_env, argClassType)
                   || Java::Runtime::Byte::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Short::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Integer::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Long::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Float::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Double::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Character::isPrimitiveType(__jni_env, argClassType)
                   || Java::Runtime::Boolean::isPrimitiveType(__jni_env, argClassType)
                   || Java::JNA::Pointer::isAssignableFrom(__jni_env, argClassType)
                   || Java::JNA::Structure::isAssignableFrom(__jni_env, argClassType)))
                ||
                 (argClassType
                 && Java::Runtime::Class::isArray(__jni_env, argClassType)
                 && Java::Runtime::Class::isPrimitive(__jni_env, Java::Runtime::Class::getComponentType(__jni_env, argClassType)))){
            __jni_env->SetObjectArrayElement(parameterTypes, i, argClassType);
            continue;
        }else{
            const std::type_info* typeId = nullptr;
            if(argPointerOrReference==0
                     && !isReferenceMetaType
                     && Java::Runtime::Class::isArray(__jni_env, argClassType)){
                jclass _argClassType = argClassType;
                jclass argClassType = Java::Runtime::Class::getComponentType(__jni_env, _argClassType);
                const QMetaType* argMetaType = nullptr;
                QScopedPointer<const QMetaType> _argMetaType;
                if(!argMetaType)
                    _argMetaType.reset(argMetaType = new QMetaType(qtjambi_register_metatype(__jni_env, argClassType, false, false)));
                if(!argMetaType->isValid())
                    _argMetaType.reset(argMetaType = nullptr);

                {
                    const SuperTypeInfos& infos = getSuperTypeInfos(__jni_env, argClassType);
                    if(infos.isEmpty()){
                        if(argMetaType){
                            typeId = getTypeByMetaType(*argMetaType);
                        }else{
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        typeId = &infos.first().typeId();
                    }
                }
                if(typeId){
                    bool isValueFunctional = false;
                    switch(getEntryType(*typeId)){
                    case EntryTypes::FunctionalTypeInfo:
                        isValueFunctional = true;
                        if(const QtJambiTypeInfo* info = getQTypeInfo(*typeId)){
                            if(info->isPointer){
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                continue;
                            }
                        }
                        Q_FALLTHROUGH();
                    case EntryTypes::ObjectTypeInfo:
                    case EntryTypes::InterfaceTypeInfo:
                    case EntryTypes::QObjectTypeInfo:
                        if(!isValueFunctional){
                            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                            continue;
                        }
                        Q_FALLTHROUGH();
                    case EntryTypes::InterfaceValueTypeInfo:
                    case EntryTypes::ValueTypeInfo:
                    case EntryTypes::FlagsTypeInfo:
                    case EntryTypes::SpecialTypeInfo:
                    case EntryTypes::StringTypeInfo:
                    case EntryTypes::PrimitiveTypeInfo:
                        if(!argMetaType)
                            _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                        if(!argMetaType->isValid())
                            _argMetaType.reset(argMetaType = nullptr);
                        if(!argMetaType)
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                        continue;
                    default:
                        break;
                    }
                }
                if(argMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if(argMetaType->name().endsWith("*")){
#else
                    if(argMetaType->flags() & QMetaType::IsPointer){
#endif
                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    }else{
                        if(!argMetaType)
                            _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                        if(!argMetaType->isValid())
                            _argMetaType.reset(argMetaType = nullptr);
                        if(!argMetaType)
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    }
                    continue;
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
            }else{
                size_t size = 0;
                {
                    const SuperTypeInfos& infos = getSuperTypeInfos(__jni_env, argClassType);
                    if(infos.isEmpty()){
                        if(argMetaType){
                            typeId = getTypeByMetaType(*argMetaType);
                            size = size_t(argMetaType->sizeOf());
                        }
                    }else{
                        typeId = &infos.first().typeId();
                        size = infos.first().size();
                    }
                }
                if(typeId){
                    bool isValueFunctional = false;
                    switch(getEntryType(*typeId)){
                    case EntryTypes::FunctionalTypeInfo:
                        isValueFunctional = true;
                        if(const QtJambiTypeInfo* info = getQTypeInfo(*typeId)){
                            if(info->isPointer){
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                continue;
                            }
                        }
                        Q_FALLTHROUGH();
                    case EntryTypes::ObjectTypeInfo:
                    case EntryTypes::InterfaceTypeInfo:
                    case EntryTypes::QObjectTypeInfo:
                        if(!isValueFunctional){
                            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                            continue;
                        }
                        Q_FALLTHROUGH();
                    case EntryTypes::InterfaceValueTypeInfo:
                    case EntryTypes::ValueTypeInfo:
                        if(!argMetaType)
                            _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                        if(!argMetaType->isValid())
                            _argMetaType.reset(argMetaType = nullptr);
                        if(argMetaType){
                            if(argPointerOrReference==0 && !isReferenceMetaType){
                                switch(argMetaType->sizeOf()){
                                case sizeof(jbyte):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Byte::primitiveType(__jni_env));
                                    break;
                                case sizeof(jshort):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Short::primitiveType(__jni_env));
                                    break;
                                case sizeof(jint):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::primitiveType(__jni_env));
                                    break;
                                case sizeof(jlong):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Long::primitiveType(__jni_env));
                                    break;
                                default:
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    break;
                                }
                                continue;
                            }else{
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                continue;
                            }
                        }else{
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    break;
                    case EntryTypes::FlagsTypeInfo:
                    case EntryTypes::SpecialTypeInfo:
                    case EntryTypes::StringTypeInfo:
                    case EntryTypes::PrimitiveTypeInfo:
                        {
                            if(!argMetaType)
                                _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                            if(!argMetaType->isValid())
                                _argMetaType.reset(argMetaType = nullptr);
                            if(argPointerOrReference==0 && !isReferenceMetaType){
                                switch(argMetaType->sizeOf()){
                                case sizeof(jbyte):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Byte::primitiveType(__jni_env));
                                    break;
                                case sizeof(jshort):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Short::primitiveType(__jni_env));
                                    break;
                                case sizeof(jint):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::primitiveType(__jni_env));
                                    break;
                                case sizeof(jlong):
                                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Long::primitiveType(__jni_env));
                                    break;
                                default:
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                    break;
                                }
                                continue;
                            }else{
                                if(size>0){
                                    if(argMetaType){
                                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                        continue;
                                    }else{
                                        if(deleter(*typeId)!=nullptr){
                                            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                                            continue;
                                        }else{
                                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                        }
                                    }
                                }else{
                                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                }
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
                if(argMetaType){
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
                    if(argMetaType->name().endsWith("*")){
#else
                    if(argMetaType->flags() & QMetaType::IsPointer){
#endif
                        __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                    }else{
                        if(argPointerOrReference==0 && !isReferenceMetaType){
                            switch(argMetaType->sizeOf()){
                            case sizeof(jbyte):
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Byte::primitiveType(__jni_env));
                                break;
                            case sizeof(jshort):
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Short::primitiveType(__jni_env));
                                break;
                            case sizeof(jint):
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Integer::primitiveType(__jni_env));
                                break;
                            case sizeof(jlong):
                                __jni_env->SetObjectArrayElement(parameterTypes, i, Java::Runtime::Long::primitiveType(__jni_env));
                                break;
                            default:
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                                break;
                            }
                        }else{
                            __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                        }
                    }
                }else if(argPointerOrReference!=0 || isReferenceMetaType){
                    __jni_env->SetObjectArrayElement(parameterTypes, i, Java::JNA::Pointer::getClass(__jni_env));
                }else{
                    Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported without pointer or reference arithmetic.").arg(qtjambi_class_name(__jni_env, argClassType).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                }
                continue;
            }// all others are allowed
        }
        __jni_env->SetObjectArrayElement(parameterTypes, i, argClassType);
    }
}

void qtjambi_function_pointer_convert_parameters(JNIEnv * __jni_env, jobjectArray argumentTypes, jobjectArray args, jobjectArray convertedArgs, jboolean isVarArgs, jboolean forward){
    jsize length = __jni_env->GetArrayLength(argumentTypes);
    jsize varArgLength = isVarArgs ? __jni_env->GetArrayLength(args) : length;
    jobjectArray varArgument = nullptr;
    jsize j = 0;
    for(jsize i = 0; i<varArgLength; ++i){
        jobject argType = __jni_env->GetObjectArrayElement(argumentTypes, qMin(i, length-1));
        QScopedPointer<const QMetaType> _argMetaType;
        const QMetaType* argMetaType = qtjambi_cast<QMetaType*>(__jni_env, Java::QtCore::QGenericArgumentType::metaType(__jni_env, argType));
        bool isReferenceMetaType = false;
        if(argMetaType)
            isReferenceMetaType = QByteArray(argMetaType->name()).endsWith("&");
        jint argPointerOrReference = Java::QtCore::QGenericArgumentType::pointerOrReference(__jni_env, argType);
        jclass argClassType = Java::QtCore::QGenericArgumentType::classType(__jni_env, argType);
        jobject oldValue = __jni_env->GetObjectArrayElement(args, i);

        jobject convertedValue = nullptr;
        const std::type_info* typeId = nullptr;
        bool isFunctionPointer = false;
        const SuperTypeInfos& infos = getSuperTypeInfos(__jni_env, argClassType);
        if(infos.isEmpty()){
            if(!argMetaType)
                _argMetaType.reset(argMetaType = new QMetaType(qtjambi_register_metatype(__jni_env, argClassType, false, false)));
            if(!argMetaType->isValid())
                _argMetaType.reset(argMetaType = nullptr);
            if(argMetaType){
                typeId = getTypeByMetaType(*argMetaType);
            }
        }else{
            typeId = &infos.first().typeId();
        }
        bool isPrimitive = false;
        if(typeId){
            isPrimitive = getEntryType(*typeId)==EntryTypes::PrimitiveTypeInfo;
            switch(getEntryType(*typeId)){
            case EntryTypes::FunctionalTypeInfo:
                if(const QtJambiTypeInfo* info = getQTypeInfo(*typeId)){
                    if(info->isPointer){
                        isFunctionPointer = true;
                    }
                }
                break;
            case EntryTypes::InterfaceValueTypeInfo:
            case EntryTypes::ValueTypeInfo:
            case EntryTypes::FlagsTypeInfo:
            case EntryTypes::SpecialTypeInfo:
            case EntryTypes::StringTypeInfo:
            case EntryTypes::PrimitiveTypeInfo:
                if(!argMetaType)
                    _argMetaType.reset(argMetaType = new QMetaType(registeredMetaTypeID(*typeId)));
                if(!argMetaType->isValid())
                    _argMetaType.reset(argMetaType = nullptr);
                break;
            default: break;
            }
        }else if(argMetaType){
            isPrimitive = argMetaType->id()==QMetaType::Int
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                    || argMetaType->id()==QMetaType::Char16
                    || argMetaType->id()==QMetaType::Char32
#endif
                    || argMetaType->id()==QMetaType::Char
                    || argMetaType->id()==QMetaType::LongLong
                    || argMetaType->id()==QMetaType::Short
                    || argMetaType->id()==QMetaType::UInt
                    || argMetaType->id()==QMetaType::UChar
                    || argMetaType->id()==QMetaType::SChar
                    || argMetaType->id()==QMetaType::ULongLong
                    || argMetaType->id()==QMetaType::UShort
                    || argMetaType->id()==QMetaType::Bool
                    || argMetaType->id()==QMetaType::QChar;
        }
        if(forward){
            if(!__jni_env->IsSameObject(oldValue, nullptr)){
                if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                    void* ptr = reinterpret_cast<void*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                    if(argPointerOrReference==2 && argClassType){
                        QFunctionPointer fp = reinterpret_cast<QFunctionPointer>(ptr);
                        bool resolved = false;
                        {
                            QReadLocker locker(gPointerLock());
                            if(gObjectsByFunctionPointer->contains(quintptr(fp))){
                                QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(fp)];
                                for(JObjectWrapper& wrapper : objects){
                                    if(__jni_env->IsInstanceOf(wrapper.object(), argClassType)){
                                        convertedValue = __jni_env->NewLocalRef(wrapper.object());
                                        resolved = true;
                                    }
                                }
                            }
                        }
                        if(!resolved){
                            convertedValue = Java::QtCore::QFunctionPointerUtil::createProxy(__jni_env, argClassType);
                            QtJambiLink::createLinkForObject(
                                    __jni_env,
                                    convertedValue,
                                    new QFunctionPointer(fp),
                                    LINK_NAME_ARG(nullptr)
                                    false,
                                    false,
                                    &delete_callback_pointer
                                )->setJavaOwnership(__jni_env);
                            QWriteLocker locker(gPointerLock());
                            (*gObjectsByFunctionPointer())[quintptr(fp)] << JObjectWrapper(__jni_env, convertedValue);
                        }
                    }else{
                        void* _ptr = ptr;
                        if(isFunctionPointer){
                            _ptr = &ptr;
                        }
                        if(typeId){
                            convertedValue = qtjambi_from_object(__jni_env, _ptr, nullptr, *typeId, isFunctionPointer || (!isReferenceMetaType && (argPointerOrReference==0 || argPointerOrReference==-2)), false);
                        }else {
                            convertedValue = qtjambi_from_object(__jni_env, _ptr, argClassType, isFunctionPointer || (!isReferenceMetaType && (argPointerOrReference==0 || argPointerOrReference==-2)), false);
                        }
                    }
                }else if(Java::QtJambi::QtEnumerator::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = qtjambi_from_enum(__jni_env, qint32(Java::Runtime::Number::intValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint32* ptr = reinterpret_cast<qint32*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = qtjambi_from_enum(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(Java::QtJambi::QFlags::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = qtjambi_from_flags(__jni_env, qint32(Java::Runtime::Number::intValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint32* ptr = reinterpret_cast<qint32*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = qtjambi_from_flags(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(Java::QtJambi::QtShortEnumerator::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = qtjambi_from_enum(__jni_env, qint16(Java::Runtime::Number::shortValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint16* ptr = reinterpret_cast<qint16*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = qtjambi_from_enum(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(Java::QtJambi::QtByteEnumerator::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = qtjambi_from_enum(__jni_env, qint8(Java::Runtime::Number::byteValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint8* ptr = reinterpret_cast<qint8*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = qtjambi_from_enum(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(Java::QtJambi::QtLongEnumerator::isAssignableFrom(__jni_env, argClassType)){
                    if(Java::Runtime::Number::isInstanceOf(__jni_env, oldValue)){
                        convertedValue = qtjambi_from_enum(__jni_env, qint64(Java::Runtime::Number::longValue(__jni_env, oldValue)), argClassType);
                    }else if(Java::JNA::Pointer::isInstanceOf(__jni_env, oldValue)){
                        qint64* ptr = reinterpret_cast<qint64*>(Java::JNA::Pointer::peer(__jni_env, oldValue));
                        convertedValue = qtjambi_from_enum(__jni_env, *ptr, argClassType);
                    }else
                        convertedValue = nullptr;
                }else if(isPrimitive && (Java::Runtime::Byte::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Short::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Integer::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Long::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Float::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Double::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Boolean::isInstanceOf(__jni_env, oldValue)
                                         || Java::Runtime::Character::isInstanceOf(__jni_env, oldValue))){
                    convertedValue = oldValue;
                }else if(argMetaType){
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(__jni_env, oldValue)){
                        switch(argMetaType->sizeOf()){
                        case sizeof(jbyte):{
                                jbyte data = 0;
                                argMetaType->construct(&data, link->pointer());
                                convertedValue = Java::Runtime::Byte::valueOf(__jni_env, data);
                                break;
                            }
                        case sizeof(jshort):{
                                jshort data = 0;
                                argMetaType->construct(&data, link->pointer());
                                convertedValue = Java::Runtime::Short::valueOf(__jni_env, data);
                                break;
                            }
                        case sizeof(jint):{
                                jint data = 0;
                                argMetaType->construct(&data, link->pointer());
                                convertedValue = Java::Runtime::Integer::valueOf(__jni_env, data);
                                break;
                            }
                        case sizeof(jlong):{
                                jlong data = 0;
                                argMetaType->construct(&data, link->pointer());
                                convertedValue = Java::Runtime::Long::valueOf(__jni_env, data);
                                break;
                            }
                        default:
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, oldValue).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }else{
                        QVariant variant = qtjambi_to_qvariant(__jni_env, oldValue);
    #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
                        if(variant.metaType()==*argMetaType || variant.convert(*argMetaType)){
    #else
                        if(variant.userType()==argMetaType->id() || variant.convert(argMetaType->id())){
    #endif
                            switch(argMetaType->sizeOf()){
                            case sizeof(jbyte):{
                                    jbyte data = 0;
                                    argMetaType->construct(&data, variant.data());
                                    convertedValue = Java::Runtime::Byte::valueOf(__jni_env, data);
                                    break;
                                }
                            case sizeof(jshort):{
                                    jshort data = 0;
                                    argMetaType->construct(&data, variant.data());
                                    convertedValue = Java::Runtime::Short::valueOf(__jni_env, data);
                                    break;
                                }
                            case sizeof(jint):{
                                    jint data = 0;
                                    argMetaType->construct(&data, variant.data());
                                    convertedValue = Java::Runtime::Integer::valueOf(__jni_env, data);
                                    break;
                                }
                            case sizeof(jlong):{
                                    jlong data = 0;
                                    argMetaType->construct(&data, variant.data());
                                    convertedValue = Java::Runtime::Long::valueOf(__jni_env, data);
                                    break;
                                }
                            default:
                                Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, oldValue).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                            }
                        }else{
                            Java::QtJambi::QUnsuccessfulInvocationException::throwNew(__jni_env, QString("Type %1 not supported.").arg(qtjambi_object_class_name(__jni_env, oldValue).replace(QLatin1Char('$'), QLatin1Char('.'))) QTJAMBI_STACKTRACEINFO );
                        }
                    }
                }else{
                    // missing: check primitives, primitive arrays and convert object arrays!
                    convertedValue = oldValue;
                }
            }
            if(i<length-1 || !isVarArgs){
                __jni_env->SetObjectArrayElement(convertedArgs, i+1, convertedValue);
            }else if(i==length-1){
                varArgument = __jni_env->NewObjectArray(varArgLength-i, argClassType, nullptr);
                __jni_env->SetObjectArrayElement(convertedArgs, i+1, varArgument);
                __jni_env->SetObjectArrayElement(varArgument, j++, convertedValue);
            }else{
                __jni_env->SetObjectArrayElement(varArgument, j++, convertedValue);
            }
        }
    }
}

void qtjambi_function_pointer_dispose(JNIEnv * __jni_env, jlong peer){
    Java::JNA::Native::freeNativeCallback(__jni_env, peer);
}

jobject qtjambi_function_pointer_cast(JNIEnv * env, jobject function, jclass functionalInterface){
    if(env->IsInstanceOf(function, functionalInterface))
        return function;
    jclass sourceClass = env->GetObjectClass(function);
    const std::type_info* sourceTypeId = nullptr;
    const std::type_info* targetTypeId = nullptr;
    {
        const SuperTypeInfos& infos = getSuperTypeInfos(env, functionalInterface);
        if(infos.isEmpty()){
            targetTypeId = getTypeByJavaName(qtjambi_class_name(env, functionalInterface).replace('.', '/'));
        }else{
            targetTypeId = &infos.first().typeId();
        }
    }
    {
        const SuperTypeInfos& infos = getSuperTypeInfos(env, sourceClass);
        if(infos.isEmpty()){
            sourceTypeId = getTypeByJavaName(qtjambi_class_name(env, sourceClass).replace('.', '/'));
        }else{
            sourceTypeId = &infos.first().typeId();
        }
    }
    if(targetTypeId){
        if(isFunctional(*targetTypeId)){
            if(const QtJambiTypeInfo* info = getQTypeInfo(*targetTypeId)){
                if(info->isPointer){
                    if(sourceTypeId){
                        if(isFunctional(*sourceTypeId)){
                            if(const QtJambiTypeInfo* info = getQTypeInfo(*sourceTypeId)){
                                if(info->isPointer){
                                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, function)){
                                        QFunctionPointer ptr = nullptr;
                                        if(qtjambi_convert_to_native(env, *sourceTypeId, function, &ptr)){
                                            {
                                                QReadLocker locker(gPointerLock());
                                                if(gObjectsByFunctionPointer->contains(quintptr(ptr))){
                                                    QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(ptr)];
                                                    for(JObjectWrapper& wrapper : objects){
                                                        if(env->IsInstanceOf(wrapper.object(), functionalInterface))
                                                            return env->NewLocalRef(wrapper.object());
                                                    }
                                                }
                                            }
                                            jobject result = qtjambi_from_object(env, &ptr, nullptr, *targetTypeId, true, false);
                                            QWriteLocker locker(gPointerLock());
                                            (*gObjectsByFunctionPointer())[quintptr(ptr)] << JObjectWrapper(env, result);
                                            return result;
                                        }else{
                                            Java::Runtime::ClassCastException::throwNew(env, qPrintable(QString("Unable to convert object of type %1 to function pointer.").arg(qtjambi_object_class_name(env, function))) QTJAMBI_STACKTRACEINFO );
                                            return nullptr;
                                        }
                                    }else{
                                        JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, sourceClass))) QTJAMBI_STACKTRACEINFO );
                                    }
                                }
                            }
                        }
                    }
                    //JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Object of type %1 is not a function pointer type.").arg(qtjambi_class_name(env, sourceClass).replace('$', '.'))) QTJAMBI_STACKTRACEINFO );
                }
            }
        }
        //JavaException::raiseIllegalArgumentException(env, qPrintable(QString("%1 is not a function pointer type.").arg(qtjambi_type_name(*targetTypeId))) QTJAMBI_STACKTRACEINFO );
    }

    if(sourceTypeId){
        if(isFunctional(*sourceTypeId)){
            if(const QtJambiTypeInfo* info = getQTypeInfo(*sourceTypeId)){
                if(info->isPointer){
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, function)){
                        if(env->IsInstanceOf(link->getJavaObjectLocalRef(env), functionalInterface)){
                            return function;
                        }else{
                            QFunctionPointer* ptr = nullptr;
                            if(qtjambi_convert_to_native(env, *sourceTypeId, function, &ptr) && ptr && *ptr){
                                if(Java::QtCore::QFunctionPointer::isSameClass(env, functionalInterface)){
                                    return qtjambi_cast<jobject>(env, ptr, "QFunctionPointer");
                                }else{
                                    {
                                        QReadLocker locker(gPointerLock());
                                        if(gObjectsByFunctionPointer->contains(quintptr(*ptr))){
                                            QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(*ptr)];
                                            for(JObjectWrapper& wrapper : objects){
                                                if(env->IsInstanceOf(wrapper.object(), functionalInterface))
                                                    return env->NewLocalRef(wrapper.object());
                                            }
                                        }
                                    }
                                    try{
                                        Java::JNA::Native::getClass(env);
                                    }catch(const JavaException& exn){
                                        JavaException exn2(env, Java::QtJambi::QUnsuccessfulInvocationException::newInstanceWithMessage(env, "Unable to load Java Native Access library. Please add JNA to class path."));
                                        exn2.addSuppressed(env, exn);
                                        exn2.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
                                    }
                                    jobject result = Java::QtCore::QFunctionPointerUtil::createProxy(env, functionalInterface);
                                    QtJambiLink::createLinkForObject(
                                            env,
                                            result,
                                            new QFunctionPointer(*ptr),
                                            LINK_NAME_ARG(nullptr)
                                            false,
                                            false,
                                            &delete_callback_pointer
                                        )->setJavaOwnership(env);
                                    QWriteLocker locker(gPointerLock());
                                    (*gObjectsByFunctionPointer())[quintptr(*ptr)] << JObjectWrapper(env, result);
                                    return result;
                                }
                            }else{
                                Java::Runtime::ClassCastException::throwNew(env, qPrintable(QString("Unable to convert object of type %1 to function pointer.").arg(qtjambi_object_class_name(env, function))) QTJAMBI_STACKTRACEINFO );
                                return nullptr;
                            }
                        }
                    }else{
                        JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, sourceClass))) QTJAMBI_STACKTRACEINFO );
                    }
                }
            }
        }
    }
    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaInterface(env, function)){
        if(DeletablePointerToObjectLink* plink = dynamic_cast<DeletablePointerToObjectLink*>(link.get())){
            if(plink->deleterFunction()==&delete_callback_pointer){
                QFunctionPointer* ptr = nullptr;
                if(const QSharedPointer<QtJambiLink>& link = QtJambiLink::findLinkForJavaInterface(env, function)){
                    ptr = reinterpret_cast<QFunctionPointer*>(link->pointer());
                    if(ptr && *ptr){
                        if(Java::QtCore::QFunctionPointer::isSameClass(env, functionalInterface)){
                            return qtjambi_cast<jobject>(env, *ptr, "QFunctionPointer");
                        }else{
                            {
                                QReadLocker locker(gPointerLock());
                                if(gObjectsByFunctionPointer->contains(quintptr(*ptr))){
                                    QList<JObjectWrapper>& objects = (*gObjectsByFunctionPointer())[quintptr(*ptr)];
                                    for(JObjectWrapper& wrapper : objects){
                                        if(env->IsInstanceOf(wrapper.object(), functionalInterface))
                                            return env->NewLocalRef(wrapper.object());
                                    }
                                }
                            }
                            try{
                                Java::JNA::Native::getClass(env);
                            }catch(const JavaException& exn){
                                JavaException exn2(env, Java::QtJambi::QUnsuccessfulInvocationException::newInstanceWithMessage(env, "Unable to load Java Native Access library. Please add JNA to class path."));
                                exn2.addSuppressed(env, exn);
                                exn2.raise( QTJAMBI_STACKTRACEINFO_DECL_USE_ENV(env) );
                            }
                            jobject result = Java::QtCore::QFunctionPointerUtil::createProxy(env, functionalInterface);
                            QtJambiLink::createLinkForObject(
                                    env,
                                    result,
                                    new QFunctionPointer(*ptr),
                                    LINK_NAME_ARG(nullptr)
                                    false,
                                    false,
                                    &delete_callback_pointer
                                )->setJavaOwnership(env);
                            QWriteLocker locker(gPointerLock());
                            (*gObjectsByFunctionPointer())[quintptr(*ptr)] << JObjectWrapper(env, result);
                            return result;
                        }
                    }
                }else{
                    Java::Runtime::ClassCastException::throwNew(env, qPrintable(QString("Unable to convert object of type %1 to function pointer.").arg(qtjambi_object_class_name(env, function))) QTJAMBI_STACKTRACEINFO );
                    return nullptr;
                }
            }
        }
        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Object of type %1 is not a function pointer type.").arg(qtjambi_class_name(env, sourceClass).replace('$', '.'))) QTJAMBI_STACKTRACEINFO );
    }else{
        JavaException::raiseQNoNativeResourcesException(env, qPrintable(QString("Incomplete object of type: %1").arg(qtjambi_class_name(env, sourceClass))) QTJAMBI_STACKTRACEINFO );
    }
    return nullptr;
}

enum ffi_status {
  FFI_OK = 0,
  FFI_BAD_TYPEDEF,
  FFI_BAD_ABI,
  FFI_BAD_ARGTYPE
};

struct ffi_cif {
  jint abi;
  unsigned nargs;
  ffi_type **arg_types;
  ffi_type *rtype;
  unsigned bytes;
  unsigned flags;
};

void qtjambi_funtion_dispatch(JNIEnv *env, void* func, jobjectArray args, ffi_type *return_type, void *presult)
{
    Q_UNUSED(ffi_type_void)
    Q_UNUSED(ffi_type_quint8)
    Q_UNUSED(ffi_type_quint64)
    int i;
    jsize nargs;
    QScopedArrayPointer<jvalue> c_args;
    QScopedArrayPointer<ffi_type const*> arg_types;
    QScopedArrayPointer<void*> arg_values;
//  int fixed_args = (flags & USE_VARARGS) >> 7;

    nargs = env->GetArrayLength(args);

    if (nargs > MAX_NARGS) {
        //snprintf(msg, sizeof(msg), "Too many arguments (max %ld)", MAX_NARGS);
        //throwByName(env, EUnsupportedOperation, msg);
        return;
    }

    if(nargs>0){
        c_args.reset(new jvalue[size_t(nargs)]);
        arg_types.reset(new ffi_type const*[size_t(nargs)]);
        arg_values.reset(new void*[size_t(nargs)]);
    }

    for (i = 0; i < nargs; i++) {
        jobject arg = env->GetObjectArrayElement(args, i);

        if (arg == nullptr) {
            c_args[i].l = nullptr;
            arg_types[i] = &ffi_type_pointer;
            arg_values[i] = &c_args[i].l;
        }
        else if (Java::Runtime::Boolean::isInstanceOf(env, arg)) {
            c_args[i].i = Java::Runtime::Boolean::booleanValue(env, arg);
            arg_types[i] = &ffi_type_quint32;
            arg_values[i] = &c_args[i].i;
        }
        else if (Java::Runtime::Byte::isInstanceOf(env, arg)) {
            c_args[i].b = Java::Runtime::Number::byteValue(env, arg);
            arg_types[i] = &ffi_type_qint8;
            arg_values[i] = &c_args[i].b;
        }
        else if (Java::Runtime::Short::isInstanceOf(env, arg)) {
            c_args[i].s = Java::Runtime::Number::shortValue(env, arg);
            arg_types[i] = &ffi_type_qint16;
            arg_values[i] = &c_args[i].s;
        }
        else if (Java::Runtime::Character::isInstanceOf(env, arg)) {
            if (sizeof(wchar_t) == 2) {
                c_args[i].c = Java::Runtime::Character::charValue(env, arg);
                arg_types[i] = &ffi_type_quint16;
                arg_values[i] = &c_args[i].c;
            }
            else if (sizeof(wchar_t) == 4) {
                c_args[i].i = Java::Runtime::Character::charValue(env, arg);
                arg_types[i] = &ffi_type_quint32;
                arg_values[i] = &c_args[i].i;
            }
            else {
                Java::Runtime::UnsupportedOperationException::throwNew(env, QString("Unsupported wchar_t size (%1)").arg(sizeof(wchar_t)) QTJAMBI_STACKTRACEINFO );
                return;
            }
        }
        else if (Java::Runtime::Integer::isInstanceOf(env, arg)) {
            c_args[i].i = Java::Runtime::Number::intValue(env, arg);
            arg_types[i] = &ffi_type_qint32;
            arg_values[i] = &c_args[i].i;
        }
        else if (Java::Runtime::Long::isInstanceOf(env, arg)) {
            c_args[i].j = Java::Runtime::Number::longValue(env, arg);
            arg_types[i] = &ffi_type_qint64;
            arg_values[i] = &c_args[i].j;
        }
        else if (Java::Runtime::Float::isInstanceOf(env, arg)) {
            c_args[i].f = Java::Runtime::Number::floatValue(env, arg);
            arg_types[i] = &ffi_type_float;
            arg_values[i] = &c_args[i].f;
        }
        else if (Java::Runtime::Double::isInstanceOf(env, arg)) {
            c_args[i].d = Java::Runtime::Number::doubleValue(env, arg);
            arg_types[i] = &ffi_type_double;
            arg_values[i] = &c_args[i].d;
        }
        else if (Java::JNA::Pointer::isInstanceOf(env, arg)) {
            c_args[i].l = jobject(Java::JNA::Pointer::peer(env, arg));
            arg_types[i] = &ffi_type_pointer;
            arg_values[i] = &c_args[i].l;
        }
        else if (Java::Runtime::Buffer::isInstanceOf(env, arg)) {
            c_args[i].l = jobject(env->GetDirectBufferAddress(arg));
            arg_types[i] = &ffi_type_pointer;
            arg_values[i] = &c_args[i].l;
        /*
        if (c_args[i].l == nullptr) {
        c_args[i].l =
        getBufferArray(arg, &array_elements[array_count].array,
        &array_elements[array_count].elems,
        (void**)&array_elements[array_count].release);
        if (c_args[i].l == nullptr) {
        throw_type = EIllegalArgument;
        throw_msg = "Buffer arguments must be direct or have a primitive backing array";
        goto cleanup;
        }
        ++array_count;
        }*/
        }
        /*
        else if ((array_pt = getArrayComponentType(arg)) != 0
        && array_pt != 'L') {
        void *ptr = nullptr;
        release_t release = nullptr;

        #define GET_ELEMS(TYPE) do {ptr=env->Get##TYPE##ArrayElements(env,arg,nullptr); release=(void*)env->Release##TYPE##ArrayElements; }while(0)
        switch(array_pt) {
        case 'Z': GET_ELEMS(Boolean); break;
        case 'B': GET_ELEMS(Byte); break;
        case 'C': GET_ELEMS(Char); break;
        case 'S': GET_ELEMS(Short); break;
        case 'I': GET_ELEMS(Int); break;
        case 'J': GET_ELEMS(Long); break;
        case 'F': GET_ELEMS(Float); break;
        case 'D': GET_ELEMS(Double); break;
        }
        if (!ptr) {
        throw_type = EOutOfMemory;
        throw_msg = "Could not obtain memory for primitive buffer";
        goto cleanup;
        }
        c_args[i].l = ptr;
        arg_types[i] = &ffi_type_pointer;
        arg_values[i] = &c_args[i].l;
        array_elements[array_count].array = arg;
        array_elements[array_count].elems = ptr;
        array_elements[array_count++].release = release;
        }*/
        else {
            // Anything else, pass directly as a pointer
            c_args[i].l = jobject(arg);
            arg_types[i] = &ffi_type_pointer;
            arg_values[i] = &c_args[i].l;
        }
    }

  /*switch (callconv) {
  case CALLCONV_C:
    abi = FFI_DEFAULT_ABI;
    break;
#ifdef _WIN32
  case CALLCONV_STDCALL:
#if defined(_WIN64) || defined(_WIN32_WCE)
    // Ignore requests for stdcall on win64/wince
    abi = FFI_DEFAULT_ABI;
#else
    abi = FFI_STDCALL;
#endif
    break;
#endif // _WIN32
  default:
    abi = (int)callconv;
    if (!(abi > FFI_FIRST_ABI && abi < FFI_LAST_ABI)) {
      snprintf(msg, sizeof(msg),
               "Unrecognized calling convention: %d", abi);
      throw_type = EIllegalArgument;
      throw_msg = msg;
      goto cleanup;
    }
    break;
  }*/

    jlong cif = Java::JNA::Native::ffi_prep_cif(env, 0, nargs, return_type, arg_types.get());
    if(cif==0){
        Java::Runtime::RuntimeException::throwNew(env, "Error during native call setup." QTJAMBI_STACKTRACEINFO );
    }else{
        Java::JNA::Native::ffi_call(env, cif, func, presult, arg_values.get());
        Java::JNA::Native::free(env, cif);
    }
//  status =
//          fixed_args
//    ? ffi_prep_cif_var(&cif, abi, fixed_args, nargs, return_type, arg_types)
//    :
//  if (!ffi_error("Native call setup", status)) {
//    PSTART();
//    if ((flags & THROW_LAST_ERROR) != 0) {
//      SET_LAST_ERROR(0);
//    }
//    ffi_call(&cif, FFI_FN(func), presult, arg_values);
//    {
//      int err = GET_LAST_ERROR();
//      JNA_set_last_error(err);
//      if ((flags & THROW_LAST_ERROR) && err) {
//        char emsg[MSG_SIZE - 3 /* literal characters */ - 10 /* max length of %d */];
//        snprintf(msg, sizeof(msg), "[%d] %s", err, STR_ERROR(err, emsg, sizeof(emsg)));
//        throw_type = ELastError;
//        throw_msg = msg;
//      }
//    }

//    PROTECTED_END(do { throw_type=EError;throw_msg="Invalid memory access";} while(0));
//  }

// cleanup:

//  // Release array elements
//  for (i=0;i < array_count;i++) {
//    array_elements[i].release(array_elements[i].array,
//                              array_elements[i].elems, 0);
//  }

//  // Must raise any exception *after* all other JNI operations
//  if (throw_type) {
//    throwByName(throw_type, throw_msg);
//  }
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool registeredNativeInterface(JNIEnv* env, jclass cls, QPair<const char*, int>& nameAndRevision);

NITypeInfo qtjambi_get_native_interface_info(JNIEnv * env, jclass cls){
    if(!cls)
        JavaException::raiseNullPointerException(env, "Class must not be null." QTJAMBI_STACKTRACEINFO );
    QPair<const char*, int> nameAndRevision;
    NITypeInfo info;
    if(registeredNativeInterface(env, cls, nameAndRevision)){
        info.name = nameAndRevision.first;
        info.revision = nameAndRevision.second;
    }else{
        JavaException::raiseIllegalArgumentException(env, qPrintable(QString("Class %1 is not a native interface type.").arg(qtjambi_class_name(env, cls).replace('$', '.'))) QTJAMBI_STACKTRACEINFO );
    }
    return info;
}
#endif
