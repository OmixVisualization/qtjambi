#include <QtCore/qcompilerdetection.h>
#if QT_VERSION >= QT_VERSION_CHECK(6,4,0)
#  define QT_CORE_INLINE_SINCE(major, minor) inline
#  define QT_CORE_INLINE_IMPL_SINCE(major, minor) 1
#endif
#include <QtCore/QCborParserError>
#include <QtCore/QCborValue>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QMutex>
#include <QtCore/QCborMap>
#include <QtCore/QThread>
#include "qtjambi_core.h"
#include "qtjambi_plugin.h"
#include "qtjambi_repository_p.h"
#include "qtjambilink_p.h"
#include "qtjambi_cast.h"

#if defined(Q_OS_WIN32)
#include <Windows.h>
#else
#include <stdio.h>
#include <dlfcn.h>
#endif
#include <QtCore/private/qplugin_p.h>

QString qtjambi_function_library_path(const void* qt_plugin_query_metadata){
#ifdef Q_OS_WIN32
#ifdef UNICODE
    QChar data[MAX_PATH];
    HMODULE hm = nullptr;

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
    if (dladdr(qt_plugin_query_metadata, &info))
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

QTJAMBI_EXPORT QObject* qtjambi_plugin_instance(QueryMetadata qt_plugin_query_metadata){
    Q_ASSERT(qt_plugin_query_metadata);
    QString libPath = qtjambi_function_library_path(reinterpret_cast<const void*>(qt_plugin_query_metadata));
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
            if(JNIEnv* env =  qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 500)
                jobject result = Java::QtJambi::QtJambiPlugins::loadPluginInstance(
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

void clear_pluginimporter_at_shutdown(JNIEnv *){
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

extern "C" Q_DECL_EXPORT QObject* qtjambi_qml_create_jarimport(QueryMetadata ptr){
    quintptr val = quintptr(ptr);
    if(!gJarimports.isDestroyed()){
        QObject* result;
        {
            QReadLocker rlocker(gJarimportsLock());
            result = gJarimports->value(val);
        }
        if(!result){
            QString path = qtjambi_function_library_path(reinterpret_cast<const void*>(ptr));
            if(JNIEnv* env = qtjambi_current_environment()){
                QTJAMBI_JNI_LOCAL_FRAME(env, 300)
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
                QObject*(*ptr)(QueryMetadata) = &qtjambi_plugin_instance;
                return reinterpret_cast<void*>(ptr);
            }else if(QLatin1String(type)==QLatin1String("CreateJarImportFunction")){
                QObject*(*ptr)(QueryMetadata) = &qtjambi_qml_create_jarimport;
                return reinterpret_cast<void*>(ptr);
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
