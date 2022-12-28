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

#include <QtCore/qcompilerdetection.h>
#include <QtCore/QCborParserError>
#include <QtCore/QCborValue>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QCborMap>
#include <QtCore/QThread>
#include <QtCore/QtPlugin>
#include "qtjambiapi.h"
#include "java_p.h"
#include "exception.h"
#include "qtjambilink_p.h"
#include "qtjambi_cast.h"

#if defined(Q_OS_WIN32)
#include <Windows.h>
#else
#include <stdio.h>
#include <dlfcn.h>
#endif
#include <QtCore/private/qplugin_p.h>

QString getFunctionLibraryPath(QFunctionPointer function){
#ifdef Q_OS_WIN32
#ifdef UNICODE
    QChar data[MAX_PATH];
    HMODULE hm = nullptr;

    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            LPWSTR(function), &hm) != 0)
    {
        if(GetModuleFileNameW(hm, LPWSTR(data), MAX_PATH) != 0){
            return QString(data);
        }
    }
#else
    QByteArray ba;
    ba.fill(0, MAX_PATH);
    HMODULE hm = NULL;

    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            LPSTR(function), &hm) != 0)
    {
        if(GetModuleFileNameA(hm, LPSTR(ba.data()), MAX_PATH) != 0){
            return QString::fromLatin1(ba);
        }
    }
#endif // !UNICODE
#else
    Dl_info info;
    if (dladdr(reinterpret_cast<const void*>(function), &info))
    {
       return QLatin1String(info.dli_fname);
    }
#endif
    return QString();
}

#ifdef QT_MOC_EXPORT_PLUGIN_V2
typedef QPluginMetaData (*QueryMetadata)();
#else
typedef const char* (*QueryMetadata)();
#endif

QObject* createPluginInstance(QueryMetadata qt_plugin_query_metadata){
    Q_ASSERT(qt_plugin_query_metadata);
    QString libPath = getFunctionLibraryPath(reinterpret_cast<QFunctionPointer>(qt_plugin_query_metadata));
#ifdef QT_MOC_EXPORT_PLUGIN_V2
    QPluginMetaData _metadata = qt_plugin_query_metadata();
    if(const char* qt_plugin_metadata = reinterpret_cast<const char*>(_metadata.data)){
        QPluginMetaData::Header header;
        memcpy(&header, qt_plugin_metadata, sizeof(header));
        qt_plugin_metadata += sizeof(header);
#else
    if(const char* qt_plugin_metadata = qt_plugin_query_metadata()){
        qt_plugin_metadata += 16;
#endif
        QByteArray ba = QByteArray::fromRawData(qt_plugin_metadata, INT_MAX);
        QCborParserError err;
        QCborValue metadata = QCborValue::fromCbor(ba, &err);
        QJsonObject pluginMetaData;
        if (err.error == QCborError::NoError && metadata.isMap()) {
            QCborValue cbClassName = metadata[qint64(QtPluginMetaDataKeys::ClassName)];
            QCborValue cbPluginName = metadata[qint64(0x0CAFEBABE0L)];
            QString className = cbClassName.toString().replace("::", ".");
            QString pluginName = cbPluginName.toString();
            if(JniEnvironment env{500}){
                jobject result = Java::QtCore::QPluginLoader::loadPluginInstance(
                            env,
                            qtjambi_cast<jstring>(env, libPath),
                            qtjambi_cast<jstring>(env, className),
                            qtjambi_cast<jstring>(env, pluginName)
                        );
                return qtjambi_cast<QObject*>(env, result);
            }
        }
    }
    return nullptr;
}

typedef QHash<quintptr,QObject*> JarimportHash;
Q_GLOBAL_STATIC(JarimportHash, gJarimports)
Q_GLOBAL_STATIC_WITH_ARGS(QReadWriteLock, gJarimportsLock, (QReadWriteLock::Recursive))

void clearJarImportersAtShutdown(JNIEnv *){
    QHash<quintptr,QObject*> jarimports;
    if(!gJarimports.isDestroyed()){
        QWriteLocker wlocker(gJarimportsLock());
        gJarimports->swap(jarimports);
    }
    for(QObject* o : jarimports.values()){
        if(!o->thread() || o->thread()==QThread::currentThread())
            delete o;
        else
            o->deleteLater();
    }
    jarimports.clear();
}

QObject* createJarImport(QueryMetadata ptr){
    quintptr val = quintptr(ptr);
    if(!gJarimports.isDestroyed()){
        QObject* result;
        {
            QReadLocker rlocker(gJarimportsLock());
            result = gJarimports->value(val);
        }
        if(!result){
            QString path = getFunctionLibraryPath(reinterpret_cast<QFunctionPointer>(ptr));
            if(JniEnvironment env{300}){
                try{
                    jobject obj = Java::QtQml::Internal::QmlTypes$JarImport::newInstance(env, qtjambi_cast<jstring>(env, path));
                    if(QSharedPointer<QtJambiLink> link = QtJambiLink::findLinkForJavaObject(env, obj)){
                        if(link->isQObject()){
                            link->setCppOwnership(env);
                            result = link->qobject();
                            {
                                QWriteLocker wlocker(gJarimportsLock());
                                QObject* _result = gJarimports->value(val);
                                if(_result){
                                    delete result;
                                    return _result;
                                }else{
                                    gJarimports->insert(val, result);
                                }
                            }
                        }
                    }
                }catch(const JavaException& exn){
                    exn.report(env);
                }
            }
        }
        return result;
    }
    return nullptr;
}

void registerPluginImporter(){
    class Importer final : public QObject{
    public:
        Importer()
            : QObject()
        {
            moveToThread(nullptr);
        }

        void *qt_metacast(const char *type) override
        {
            if(QLatin1String(type)==QLatin1String("CreatePluginInstanceFunction")){
                QObject*(*ptr)(QueryMetadata) = &createPluginInstance;
                return reinterpret_cast<void*>(ptr);
            }else if(QLatin1String(type)==QLatin1String("CreateJarImportFunction")){
                QObject*(*ptr)(QueryMetadata) = &createJarImport;
                return reinterpret_cast<void*>(ptr);
            }else if(QLatin1String(type)==QLatin1String("CoreAPI::preExit")){
                // called from QtJambiGenerator
                return reinterpret_cast<void*>(&CoreAPI::preExit);
            }else if(QLatin1String(type)==QLatin1String("CoreAPI::unexit")){
                // called from QtJambiGenerator
                return reinterpret_cast<void*>(&CoreAPI::unexit);
            }
            return nullptr;
        }
    };
    static Importer instance;
    QtPluginInstanceFunction i = []() -> QObject* {
        return &instance;
    };
    static QByteArray metaData = []()->QByteArray{
        QByteArray result;
#ifndef QT_MOC_EXPORT_PLUGIN_V2
        result += 'Q';
        result += 'T';
        result += 'M';
        result += 'E';
        result += 'T';
        result += 'A';
        result += 'D';
        result += 'A';
        result += 'T';
        result += 'A';
        result += ' ';
        result += '!';
#endif
        result += char(0);
        result += char(QT_VERSION_MAJOR);
        result += char(QT_VERSION_MINOR);
        result += char(qPluginArchRequirements()
#ifdef QTJAMBI_NO_DEBUG_PLUGINS
                                                  & ~1
#endif
                  );

        result += QCborMap({{qint64(QtPluginMetaDataKeys::IID), QCborValue("io.qtjambi.PluginImporter")}}).toCborValue().toCbor();
        return result;
    }();
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    QtPluginMetaDataFunction m = []()->const char *{
        return metaData.constData();
    };
#else
    QtPluginMetaDataFunction m = []()->QPluginMetaData{
        return {reinterpret_cast<decltype(std::declval<QPluginMetaData>().data)>(metaData.constData()), size_t(metaData.size())};
    };
#endif
    QStaticPlugin sp{i, m};
    sp.metaData();
    qRegisterStaticPluginFunction(sp);
}
