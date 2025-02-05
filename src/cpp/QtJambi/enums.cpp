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

#include <QtCore/QReadWriteLock>
#include "registryutil_p.h"
#include "java_p.h"
#include "utils_p.h"

#include "qtjambi_cast.h"

struct SwitchTable{
    jfieldID field;
    jclass clazz;
    bool isSwitchTable;
};

Q_GLOBAL_STATIC(QReadWriteLock, gEnumLock)
typedef SecureContainer<QHash<jint,jfieldID>,gEnumLock> EnumClassValuesHash;
typedef SecureContainer<QHash<jint,bool>,gEnumLock> EnumClassExtensibleHash;
typedef SecureContainer<QHash<QString,QList<SwitchTable>>,gEnumLock> SwitchTableFieldHash;
Q_GLOBAL_STATIC(EnumClassValuesHash, enumClassValuesFields)
Q_GLOBAL_STATIC(EnumClassExtensibleHash, enumClassExtensible)
Q_GLOBAL_STATIC(SwitchTableFieldHash, gSwitchTableFields)

bool isClassExtensible(JNIEnv *env, jint hashCode, jclass enumClass){
    {
        QReadLocker locker(gEnumLock());
        if(enumClassExtensible->contains(hashCode)){
            return (*enumClassExtensible)[hashCode];
        }
    }
    bool result = Java::Runtime::Class::isAnnotationPresent(env, enumClass, Java::QtJambi::QtExtensibleEnum::getClass(env));
    QWriteLocker locker(gEnumLock());
    (*enumClassExtensible)[hashCode] = result;
    return (*enumClassExtensible)[hashCode];
}

jfieldID findValueField(jthrowable& t, JNIEnv *env, jint hashCode, jclass enumClass){
    jfieldID valuesField;
    {
        QReadLocker locker(gEnumLock());
        valuesField = enumClassValuesFields->value(hashCode, nullptr);
    }
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
        if(valuesField){
            QWriteLocker locker(gEnumLock());
            (*enumClassValuesFields)[hashCode] = valuesField;
        }
    }
    return valuesField;
}

jobjectArray extendEnumValues(JNIEnv *env, jclass enumClass, jobjectArray values, int length, jobject newEntry){
    values = Java::Runtime::Arrays::copyOf(env, values, length+1);
    env->SetObjectArrayElement(values, length, newEntry);
    static std::function<void(JNIEnv *, jclass, jobjectArray, jobject)> extendEnumFn = [](JNIEnv *env) -> std::function<void(JNIEnv *, jclass, jobjectArray, jobject)>{
#if defined(Q_OS_ANDROID)
        // this is Android JDK
        jfieldID sharedConstantsCache = resolveField(env, "sharedConstantsCache", "Llibcore/util/BasicLruCache;", Java::Runtime::Enum::getClass(env), true);
        if(!sharedConstantsCache){
            jobject sharedConstantsCacheField = Java::Runtime::Class::tryGetDeclaredField(env, Java::Runtime::Enum::getClass(env), env->NewStringUTF("sharedConstantsCache"));
            if(env->ExceptionCheck()){
                env->ExceptionClear();
            }
            if(sharedConstantsCacheField){
                sharedConstantsCache = env->FromReflectedField(sharedConstantsCacheField);
            }
        }
        if(sharedConstantsCache){
            if(jobject cache = env->GetStaticObjectField(Java::Runtime::Enum::getClass(env), sharedConstantsCache)){
                jclass cacheClass = env->GetObjectClass(cache);
                jmethodID evictAll = JavaAPI::resolveMethod(env, "evictAll", "()V", cacheClass, false);
                if(evictAll){
                    JObjectWrapper cacheWrapper(env, cache);
                    return [cacheWrapper, evictAll](JNIEnv *env, jclass, jobjectArray, jobject){
                        env->CallVoidMethod(cacheWrapper, evictAll);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    };
                }
            }
        }
#endif
        // this is OpenJDK
        jfieldID enumConstants = resolveField(env, "enumConstants", "[Ljava/lang/Object;", Java::Runtime::Class::getClass(env));
        if(env->ExceptionCheck())
            env->ExceptionClear();
        if(!enumConstants){
            jobject enumConstantsField = Java::Runtime::Class::tryGetDeclaredField(env, Java::Runtime::Class::getClass(env), env->NewStringUTF("enumConstants"));
            if(env->ExceptionCheck())
                env->ExceptionClear();
            if(enumConstantsField)
                enumConstants = env->FromReflectedField(enumConstantsField);
        }
        jfieldID enumConstantDirectory = resolveField(env, "enumConstantDirectory", "Ljava/util/Map;", Java::Runtime::Class::getClass(env));
        if(env->ExceptionCheck())
            env->ExceptionClear();
        if(!enumConstantDirectory){
            jobject enumConstantDirectoryField = Java::Runtime::Class::tryGetDeclaredField(env, Java::Runtime::Class::getClass(env), env->NewStringUTF("enumConstantDirectory"));
            if(env->ExceptionCheck())
                env->ExceptionClear();
            if(enumConstantDirectoryField)
                enumConstantDirectory = env->FromReflectedField(enumConstantDirectoryField);
        }
        if(enumConstants && enumConstantDirectory){
            return [enumConstants, enumConstantDirectory](JNIEnv *env, jclass enumClass, jobjectArray values, jobject newEntry){
                env->SetObjectField(enumClass, enumConstants, values);
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                jobject directory = env->GetObjectField(enumClass, enumConstantDirectory);
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                if(directory){
                    Java::Runtime::Map::put(env, directory, Java::Runtime::Enum::name(env, newEntry), newEntry);
                }
            };
        }
        // this is openj9 JDK
        jmethodID getEnumVars = JavaAPI::resolveMethod(env, "getEnumVars", "()Ljava/lang/Class$EnumVars;", Java::Runtime::Class::getClass(env), false);
        jclass enumVarsClass{nullptr};
        if(env->ExceptionCheck()){
            env->ExceptionClear();
        }
        if(!getEnumVars){
            jobject getEnumVarsMethod = Java::Runtime::Class::tryGetDeclaredMethod(env, Java::Runtime::Class::getClass(env), env->NewStringUTF("getEnumVars"), env->NewObjectArray(0, Java::Runtime::Class::getClass(env), nullptr));
            if(env->ExceptionCheck()){
                env->ExceptionClear();
            }
            if(getEnumVarsMethod){
                getEnumVars = env->FromReflectedMethod(getEnumVarsMethod);
                enumVarsClass = Java::Runtime::Method::getReturnType(env, getEnumVarsMethod);
            }
        }else{
            enumVarsClass = JavaAPI::resolveClass(env, "java/lang/Class$EnumVars");
        }
        if(getEnumVars && enumVarsClass){
            jobject cachedEnumConstantsField = Java::Runtime::Class::tryGetDeclaredField(env, enumVarsClass, env->NewStringUTF("cachedEnumConstants"));
            jobject cachedEnumConstantDirectoryField = Java::Runtime::Class::tryGetDeclaredField(env, enumVarsClass, env->NewStringUTF("cachedEnumConstantDirectory"));
            if(cachedEnumConstantsField && cachedEnumConstantDirectoryField){
                jfieldID cachedEnumConstants = env->FromReflectedField(cachedEnumConstantsField);
                jfieldID cachedEnumConstantDirectory = env->FromReflectedField(cachedEnumConstantDirectoryField);
                return [getEnumVars, cachedEnumConstants, cachedEnumConstantDirectory](JNIEnv *env, jclass enumClass, jobjectArray values, jobject newEntry){
                    jobject enumVarsObject = env->CallObjectMethod(enumClass, getEnumVars);
                    if(env->ExceptionCheck()){
                        env->ExceptionDescribe();
                        env->ExceptionClear();
                    }
                    if(enumVarsObject){
                        env->SetObjectField(enumVarsObject, cachedEnumConstants, values);
                        if(env->ExceptionCheck()){
                            env->ExceptionDescribe();
                            env->ExceptionClear();
                        }
                        jobject directory = env->GetObjectField(enumVarsObject, cachedEnumConstantDirectory);
                        if(env->ExceptionCheck()){
                            env->ExceptionDescribe();
                            env->ExceptionClear();
                        }
                        if(directory){
                            Java::Runtime::Map::put(env, directory, Java::Runtime::Enum::name(env, newEntry), newEntry);
                        }
                    }
                };
            }
        }
        return [](JNIEnv *, jclass, jobjectArray, jobject){};
    }(env);
    extendEnumFn(env, enumClass, values, newEntry);
    return values;
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
                QString className = QtJambiAPI::getClassName(env, enumClass);
                auto idx = className.lastIndexOf('$');
                QString simpleClassName = className;
                if(idx>=0){
                    simpleClassName = simpleClassName.mid(idx+1);
                }
                object = env->NewObject(enumClass, methodId, entryName ? entryName : qtjambi_cast<jstring>(env, QString("Custom%1(%2)").arg(simpleClassName).arg(value)), length, value);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                values = extendEnumValues(env, enumClass, values, length, object);
                indexesByValue.insert(value, length);
                index = length;
                ++length;
                env->SetStaticObjectField(enumClass, valuesField, values);
                JavaException::check(env QTJAMBI_STACKTRACEINFO );
                className = className.replace(".", "$").replace("/", "$");
                QReadLocker locker(gEnumLock());
                const SwitchTableFieldHash& switchTableFields = *gSwitchTableFields;
                for(const SwitchTable& st : switchTableFields[className]){
                    jintArray array = jintArray(env->GetStaticObjectField(st.clazz, st.field));
                    if(array){
                        array = Java::Runtime::Arrays::copyOfIntArray(env, array, length);
                        /*if(st.isSwitchTable){
                            JIntArrayWrapper wrapper(env, array);
                            wrapper[length] = index + 1;
                        }*/
                        env->SetStaticObjectField(st.clazz, st.field, array);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    }
                }
                for(const SwitchTable& st : switchTableFields[QStringLiteral(u"$EnumSwitchMapping$")]){
                    jintArray array = jintArray(env->GetStaticObjectField(st.clazz, st.field));
                    if(array && env->GetArrayLength(array)==length-1){
                        array = Java::Runtime::Arrays::copyOfIntArray(env, array, length);
                        env->SetStaticObjectField(st.clazz, st.field, array);
                        JavaException::check(env QTJAMBI_STACKTRACEINFO );
                    }
                }
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

void registerSwitchTableFields(JNIEnv *env, jobject switchTableFields){
    for(jsize i=0, l = Java::Runtime::Collection::size(env, switchTableFields); i<l; ++i){
        jobject field = Java::Runtime::List::get(env, switchTableFields, i);
        QString name = qtjambi_cast<QString>(env, Java::Runtime::Field::getName(env, field));
        bool table = false;
        bool isEnumSwitchMapping = false;
        if((table = name.startsWith(QStringLiteral(u"$SWITCH_TABLE$")))
            || name.startsWith(QStringLiteral(u"$SwitchMap$"))
            || (isEnumSwitchMapping = name.startsWith(QStringLiteral(u"$EnumSwitchMapping$")))){
            jclass declaringClass = Java::Runtime::Field::getDeclaringClass(env, field);
            declaringClass = getGlobalClassRef(env, declaringClass);
            SwitchTableFieldHash& switchTableFields = *gSwitchTableFields;
            QWriteLocker locker(gEnumLock());
            if(isEnumSwitchMapping)
                switchTableFields[QStringLiteral(u"$EnumSwitchMapping$")].append({env->FromReflectedField(field), declaringClass, table});
            else
                switchTableFields[name.mid(table ? 14 : 11)].append({env->FromReflectedField(field), declaringClass, table});
        }
    }
}
