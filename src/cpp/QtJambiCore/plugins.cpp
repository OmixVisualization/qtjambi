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

#include "pch_p.h"
#include <QtCore/private/qplugin_p.h>
#include <QtCore/QJsonObject>
#include <QtCore/QStaticPlugin>
#include <QtCore/QCborMap>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/CoreAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/FunctionPointer>
#include <QtJambi/Cast>

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
#ifdef QT_MOC_EXPORT_PLUGIN_V2
        rawMetaData.reserve(4 + cborData.size());
#else
        rawMetaData.reserve(16 + cborData.size());
        rawMetaData.append("QTMETADATA !");
#endif
        rawMetaData.append(char(0));
        rawMetaData.append(char(QT_VERSION_MAJOR));
        rawMetaData.append(char(QT_VERSION_MINOR));
        rawMetaData.append(char(qPluginArchRequirements()));
        rawMetaData.append(cborData);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        QtPluginMetaDataFunction rawMetaDataFunction = qtjambi_function_pointer<16,QPluginMetaData()>(
                    [rawMetaData]() -> QPluginMetaData{
                        return QPluginMetaData{reinterpret_cast<const uchar *>(rawMetaData.data()), size_t(rawMetaData.size())};
                    }
                , qHash(rawMetaData));
        QStaticPlugin staticPlugin(instanceFunction, rawMetaDataFunction);
#else
        QStaticPlugin staticPlugin;
        staticPlugin.instance = instanceFunction;
        staticPlugin.rawMetaData = qtjambi_function_pointer<16,const char*()>(
                    [rawMetaData]() -> const char*{
                        return rawMetaData;
                    }
                , qHash(rawMetaData));
#endif
        qRegisterStaticPluginFunction(staticPlugin);
    }
}

// emitting (writeExtraFunctions)
extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QPluginLoader_qRegisterStaticPluginFunctionInstance(JNIEnv *__jni_env, jclass, jobject jinstance, QtJambiNativeID metaData1){
    QTJAMBI_NATIVE_METHOD_CALL("qRegisterStaticPluginFunction(QStaticPlugin)")
    try{
        QObject* instance = qtjambi_cast<QObject*>(__jni_env, jinstance);
        QtJambiAPI::checkNullPointer(__jni_env, instance);
        QtJambiAPI::setCppOwnership(__jni_env, jinstance);
        jclass cls = __jni_env->GetObjectClass(jinstance);
        Q_ASSERT(cls);
        QString className = QtJambiAPI::getClassName(__jni_env, cls);
        QList<const char*> iids = CoreAPI::getInterfaceIIDs(__jni_env, cls);
        if(iids.isEmpty()){
            iids << nullptr;
        }
        QJsonObject json = QtJambiAPI::valueReferenceFromNativeId<QJsonObject>(metaData1);
        QtPluginInstanceFunction instanceFunction = qtjambi_function_pointer<16,QObject*()>(
                    [pointer = QPointer<QObject>(instance)]() -> QObject*{
                        return pointer.data();
                    }
                , qHash(className) * 31 + qHash(json.value("Keys")));
        registerPlugin(instanceFunction, className.replace(".", "::").replace("$", "::"), json, iids);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QPluginLoader_qRegisterStaticPluginFunctionClass(JNIEnv *__jni_env, jclass, jclass cls, QtJambiNativeID metaData1){
    QTJAMBI_NATIVE_METHOD_CALL("qRegisterStaticPluginFunction(QStaticPlugin)")
    try{
        QString className = QtJambiAPI::getClassName(__jni_env, cls);
        QList<const char*> iids = CoreAPI::getInterfaceIIDs(__jni_env, cls);
        if(iids.isEmpty()){
            iids << nullptr;
        }
        cls = JavaAPI::toGlobalReference(__jni_env, cls);
        jmethodID constructorHandle = __jni_env->GetMethodID(cls, "<init>", "()V");
        JavaException::check(__jni_env QTJAMBI_STACKTRACEINFO);
        if(constructorHandle){
            QJsonObject json = QtJambiAPI::valueReferenceFromNativeId<QJsonObject>(metaData1);
            QtPluginInstanceFunction instanceFunction = qtjambi_function_pointer<16,QObject*()>(
                        [cls, constructorHandle, pointer = QPointer<QObject>()]() mutable -> QObject* {
                            if(JniEnvironment env{500}){
                               QTJAMBI_TRY{
                                    if(pointer.isNull()){
                                        jobject plugin = env->NewObject(cls, constructorHandle);
                                        JavaException::check(env QTJAMBI_STACKTRACEINFO);
                                        QtJambiAPI::setCppOwnership(env, plugin);
                                        pointer = qtjambi_cast<QObject*>(env, plugin);
                                    }
                                    return pointer.data();
                                }QTJAMBI_CATCH(const JavaException& exn){
                                    exn.report(env);
                                }QTJAMBI_TRY_END
                            }
                            return nullptr;
                        }
                    , qHash(className) * 31 + qHash(json.value("Keys")));
            registerPlugin(instanceFunction, className.replace(".", "::").replace("$", "::"), json, iids);
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QPluginLoader_qRegisterStaticPluginFunction
(JNIEnv *__jni_env,
 jclass,
 jobject classSupplier,
 jstring _className,
 jstring jiid,
 QtJambiNativeID metaData1,
 QtJambiNativeID pluginInfo1)
{
    QTJAMBI_NATIVE_METHOD_CALL("qRegisterStaticPluginFunction(QStaticPlugin)")
    try{
        QString className = qtjambi_cast<QString>(__jni_env, _className);
        QByteArray iid = qtjambi_cast<QByteArray>(__jni_env, jiid);
        QJsonObject metaData = QtJambiAPI::valueReferenceFromNativeId<QJsonObject>(metaData1);
        QJsonObject pluginInfo = QtJambiAPI::valueReferenceFromNativeId<QJsonObject>(pluginInfo1);

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
                    if(JniEnvironment env{200}){
                        QTJAMBI_TRY{
                            jobject classOrSupplier;
                            if(!m_constructorHandle && (classOrSupplier = m_classOrSupplier.object(env))){ // m_classOrSupplier is Supplier
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
                                jobject plugin = env->NewObject(jclass(m_classOrSupplier.object(env)), m_constructorHandle);
                                JavaException::check(env QTJAMBI_STACKTRACEINFO);
                                m_pointer = qtjambi_cast<QObject*>(env, plugin);
                                QtJambiAPI::setCppOwnership(env, plugin);
                            }
                        }QTJAMBI_CATCH(const JavaException& exn){
                            exn.report(env);
                        }QTJAMBI_TRY_END
                    }
                }
                return m_pointer.data();
            }
        };

        QtPluginInstanceFunction instanceFunction = qtjambi_function_pointer<16,QObject*()>(
                    Functor(__jni_env, classSupplier), qHash(className) * 31 + qHash(metaData.value("Keys")));
        registerPlugin(instanceFunction, className.replace(".", "::").replace("$", "::"), metaData, {iid}, pluginInfo);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QPluginLoader_qRegisterPluginInterface(JNIEnv *__jni_env, jclass, jclass cls){
    QTJAMBI_NATIVE_METHOD_CALL("qRegisterPluginInterface(Class)")
    try{
        (void)RegistryAPI::registerInterfaceID(__jni_env, cls);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

extern "C" JNIEXPORT jclass JNICALL Java_io_qt_core_internal_QFactoryLoader_registeredPluginInterface(JNIEnv *__jni_env, jclass, jstring iid){
    QTJAMBI_NATIVE_METHOD_CALL("registeredPluginInterface(String)")
    try{
        QtJambiScope scope;
        return CoreAPI::getInterfaceByIID(__jni_env, qtjambi_cast<const char*>(__jni_env, scope, iid));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}
