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

#include "qtjambiapi.h"
#include "java_p.h"
#include "qtjambi_cast.h"

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_ReflectionUtility_privateLookup
(JNIEnv *env,
 jclass,
 jclass targetClass)
{
    try{
        Q_ASSERT(targetClass);
        static jmethodID constructor = [](JNIEnv *env) -> jmethodID {
            jmethodID result(nullptr);
#ifndef Q_OS_ANDROID
            QString version = qtjambi_cast<QString>(env, Java::Runtime::System::getProperty(env, env->NewStringUTF("java.version"), nullptr));
            if(version.startsWith("1."))
#endif
            {
                result = env->GetMethodID(Java::Runtime::MethodHandles$Lookup::getClass(env), "<init>", "(Ljava/lang/Class;I)V");
                if(env->ExceptionCheck()){
                    env->ExceptionClear();
                }
            }
            if(!result){
                result = env->GetMethodID(Java::Runtime::MethodHandles$Lookup::getClass(env), "<init>", "(Ljava/lang/Class;)V");
                JavaException::check(env);
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

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_ReflectionUtility_writeField
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

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_internal_ReflectionUtility_readField
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
