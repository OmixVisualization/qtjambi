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
QT_WARNING_DISABLE_DEPRECATED
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
        enumClassExtensible[hashCode] = Java::Runtime::Class::isAnnotationPresent(env, enumClass, Java::QtJambi::QtExtensibleEnum::getClass(env));
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
    values = Java::Runtime::Arrays::copyOf(env, values, length+1);
    env->SetObjectArrayElement(values, length, newEntry);
    Java::QtJambi::QtJambiInternal::extendEnum(env, enumClass, values, newEntry);
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
            qtjambi_throw_java_exception(env)
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
    qtjambi_throw_java_exception(env)
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
    (JNIEnv *env, jclass, QtJambiNativeID native_id)
{
    try{
        if (QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(native_id)) {
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
        if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, copy)){
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
            QT_WARNING_DISABLE_DEPRECATED
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
            name = name.arg(keyType).arg(EXCLUDE_GT_END(valueType));
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

extern "C" Q_DECL_EXPORT jint JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_countEventLoops)
(JNIEnv *env, jclass, QtJambiNativeID __thread_nativeId)
{
    try{
        QThread *__qt_this = qtjambi_object_from_nativeId<QThread>(__thread_nativeId);
        Q_ASSERT(__qt_this);
        return jint(QThreadData::get2(__qt_this)->eventLoops.size());
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
        return 0;
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
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_qRegisterStaticPluginFunctionInstance)
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

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_qRegisterStaticPluginFunctionClass)
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

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_qRegisterStaticPluginFunction)
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

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_qRegisterPluginInterface)
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

extern "C" Q_DECL_EXPORT jclass JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_qRegisteredPluginInterface)
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
                link->onDispose(env);
            }catch(const JavaException& exn){
                throwable = jthrowable(env->NewLocalRef(exn.object()));
            }
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
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
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiInternal_getInterfaceIID)
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
