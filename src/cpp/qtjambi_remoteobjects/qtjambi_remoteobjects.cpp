/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include <QtRemoteObjects/private/qconnectionfactories_p.h>

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_qRegisterRemoteObjectsServer)
    (JNIEnv *__jni_env,
    jclass,
    jobject id0,
    jclass type1,
    jobject constructor2)
{
    struct ROServerFactory{
        using CreatorFunc = QConnectionAbstractServer * (*)(QObject *);
        QHash<QString, CreatorFunc> m_creatorFuncs;
    };
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterRemoteObjectsServer<T>(const QString & id)")
    Q_UNUSED(__jni_env)
    ROServerFactory *serverFactory = reinterpret_cast<ROServerFactory *>(QtROServerFactory::instance());
    Q_ASSERT(serverFactory);
    const QString&  __qt_id0 = qtjambi_to_qstring(__jni_env, jstring(id0));
    jmethodID constructor = __jni_env->FromReflectedMethod(constructor2);
    type1 = jclass(__jni_env->NewGlobalRef(jobject(type1)));
    uint hash = uint(qtjambi_java_object_hashcode(__jni_env,type1));
    serverFactory->m_creatorFuncs[__qt_id0] = qtjambi_function_pointer<16,QConnectionAbstractServer*(QObject *)>(
        [type1,constructor](QObject * o) -> QConnectionAbstractServer*{
            if(JNIEnv* env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject parent = qtjambi_from_QObject(env, o);
                jobject newObject = env->NewObject(type1, constructor, parent);
                qtjambi_throw_java_exception(env)
                qtjambi_rethrowing(env,
                    return reinterpret_cast<QConnectionAbstractServer*>(qtjambi_to_qobject(env, newObject));
                )
            }
            return nullptr;
        },
        hash);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QtRemoteObjects_qRegisterRemoteObjectsClient)
    (JNIEnv *__jni_env,
    jclass,
    jobject id0,
    jclass type1,
    jobject constructor2)
{
    struct ROClientFactory{
        using CreatorFunc = ClientIoDevice * (*)(QObject *);
        QHash<QString, CreatorFunc> m_creatorFuncs;
    };
    QTJAMBI_DEBUG_METHOD_PRINT("native", "qRegisterRemoteObjectsClient<T>(const QString & id)")
    Q_UNUSED(__jni_env)
    ROClientFactory *factory = reinterpret_cast<ROClientFactory *>(QtROClientFactory::instance());
    Q_ASSERT(factory);
    const QString&  __qt_id0 = qtjambi_to_qstring(__jni_env, jstring(id0));
    jmethodID constructor = __jni_env->FromReflectedMethod(constructor2);
    type1 = jclass(__jni_env->NewGlobalRef(jobject(type1)));
    uint hash = uint(qtjambi_java_object_hashcode(__jni_env, type1));
    factory->m_creatorFuncs[__qt_id0] = qtjambi_function_pointer<16,ClientIoDevice*(QObject *)>(
        [type1,constructor](QObject * o) -> ClientIoDevice*{
            if(JNIEnv* env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 200)
                jobject parent = qtjambi_from_QObject(env, o);
                jobject newObject = env->NewObject(type1, constructor, parent);
                qtjambi_throw_java_exception(env)
                qtjambi_rethrowing(env,
                    return reinterpret_cast<ClientIoDevice*>(qtjambi_to_qobject(env, newObject));
                )
            }
            return nullptr;
        },
        hash);
}

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_remoteobjects_QRemoteObjectNode_acquire)
(JNIEnv *__jni_env, jobject __this, jclass type, jobject constructor, jobject name)
{
    jmethodID constructorID = __jni_env->FromReflectedMethod(constructor);
    Q_ASSERT(constructorID);
    return __jni_env->NewObject(type, constructorID, __this, name);
}
