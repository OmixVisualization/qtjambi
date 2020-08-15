#include "qtjambi_plugin.h"
#include "qtjambi_repository_p.h"
#include "qtjambi_cast.h"

#if defined(Q_OS_WIN32)
#include <Windows.h>
#else
#include <stdio.h>
#include <dlfcn.h>
#endif
#include <QtCore/private/qplugin_p.h>

QString libraryPath(QueryMetadata qt_plugin_query_metadata){
#ifdef Q_OS_WIN32
#ifdef UNICODE
    QChar data[MAX_PATH];
    HMODULE hm = NULL;

    if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            LPWSTR(qt_plugin_query_metadata), &hm) != 0)
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
            LPSTR(qt_plugin_query_metadata), &hm) != 0)
    {
        if(GetModuleFileNameA(hm, LPSTR(ba.data()), MAX_PATH) != 0){
            return QString::fromLatin1(ba);
        }
    }
#endif // !UNICODE
#else
    Dl_info info;
    if (dladdr(reinterpret_cast<const void*>(qt_plugin_query_metadata), &info))
    {
       return QLatin1String(info.dli_fname);
    }
#endif
    return QString();
}


QObject* qtjambi_plugin_instance(QueryMetadata qt_plugin_query_metadata){
    Q_ASSERT(qt_plugin_query_metadata);
    QString libPath = libraryPath(qt_plugin_query_metadata);
    const char* qt_plugin_metadata = qt_plugin_query_metadata();
    if(qt_plugin_metadata){
        qt_plugin_metadata += 16;
        QByteArray ba = QByteArray::fromRawData(qt_plugin_metadata, INT_MAX);
        QCborParserError err;
        QCborValue metadata = QCborValue::fromCbor(ba, &err);
        QJsonObject pluginMetaData;
        if (err.error == QCborError::NoError && metadata.isMap()) {
            QCborValue cbClassName = metadata[qint64(QtPluginMetaDataKeys::ClassName)];
            QCborValue cbPluginName = metadata[qint64(0x0CAFEBABE0L)];
            QString className = cbClassName.toString().replace("::", ".");
            QString pluginName = cbPluginName.toString();
            if(JNIEnv* env =  qtjambi_current_environment()){
                jobject result = Java::Private::QtJambi::QtJambiInternal.loadPluginInstance(
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
