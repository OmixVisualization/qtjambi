/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QMutex>
#include "registryutil_p.h"
#include "java_p.h"
#include "utils_p.h"

#include "qtjambi_cast.h"

typedef QHash<jint,jfieldID> EnumClassValuesHash;
typedef QHash<jint,jboolean> EnumClassExtensibleHash;
Q_GLOBAL_STATIC(EnumClassValuesHash, enumClassValuesFields)
Q_GLOBAL_STATIC(EnumClassExtensibleHash, enumClassExtensible)
Q_GLOBAL_STATIC(QMutex, gEnumLock)

jboolean isClassExtensible(JNIEnv *env, jint hashCode, jclass enumClass){
    if(!enumClassExtensible->contains(hashCode)){
        (*enumClassExtensible)[hashCode] = Java::Runtime::Class::isAnnotationPresent(env, enumClass, Java::QtJambi::QtExtensibleEnum::getClass(env));
    }
    return (*enumClassExtensible)[hashCode];
}

jfieldID findValueField(jthrowable& t, JNIEnv *env, jint hashCode, jclass enumClass){
    jfieldID valuesField = enumClassValuesFields->value(hashCode, nullptr);
    if(!valuesField){
        QString signature = QString("[L%1;").arg(QtJambiAPI::getClassName(env, enumClass).replace(QLatin1Char('.'), QLatin1Char('/')));
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
    static bool extendEnum = true;
    values = Java::Runtime::Arrays::copyOf(env, values, length+1);
    env->SetObjectArrayElement(values, length, newEntry);
    if(extendEnum)
         extendEnum = Java::QtJambi::EnumUtility::extendEnum(env, enumClass, values, newEntry);
    return values;
}

QString simpleClassName(JNIEnv *env, jclass enumClass){
    QString className = QtJambiAPI::getClassName(env, enumClass);
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
            QtJambiAPI::addToJavaCollection(env, set, env->GetObjectArrayElement(rejectedValues, i));
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
        QMutexLocker locker(gEnumLock());

        static QHash<jint, Map<jtype,jsize> > cachesByEnum = QHash<jint, Map<jtype,jsize> >();
        jobject object = nullptr;
        jthrowable t = nullptr;
        jfieldID valuesField = findValueField(t, env, hashCode, enumClass);
        if(valuesField){
            jobjectArray values = jobjectArray(env->GetStaticObjectField(enumClass, valuesField));
            JavaException::check(env QTJAMBI_STACKTRACEINFO );
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
                t = nullptr;
                jmethodID methodId = JavaAPI::resolveMethod(env, "<init>", EnumResolver<sizeof(jtype)>::SIGNATURE, enumClass, false, &t);
                if(t)
                    JavaException(env, t).raise();
                Q_ASSERT(methodId);
                QString className = simpleClassName(env, enumClass);
                object = env->NewObject(enumClass, methodId, entryName ? entryName : qtjambi_cast<jstring>(env, QString("Custom%1(%2)").arg(className).arg(value)), length, value);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                values = extendEnumValues(env, enumClass, values, length, object);
                indexesByValue.insert(value, length);
                index = length;
                ++length;
                env->SetStaticObjectField(enumClass, valuesField, values);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
            }
            if(index>=0 && index<length){
                object = env->GetObjectArrayElement(values, index);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
            }
        }else if(t){
            JavaException(env, t).raise();
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
