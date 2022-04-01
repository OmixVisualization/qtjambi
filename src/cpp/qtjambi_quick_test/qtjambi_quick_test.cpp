#include <QtQuickTest/QtQuickTest>

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cast.h>
#include <QtCore/private/qcoreapplication_p.h>

extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_quick_test_QQuickTest_quickTestMainWithSetup)
(JNIEnv *env, jclass, jobjectArray args, jstring name, jstring sourceDir, QtJambiNativeID setup)
{
    J2CStringBuffer _name(env, name);
    J2CStringBuffer _sourceDir(env, sourceDir);
    int argc;
    char** argv;
    QByteArray appName;
    QVector<QByteArray> argsBuffers;
    QCoreApplication* app = QCoreApplication::instance();
    if (QCoreApplicationPrivate* pvt = app ? dynamic_cast<QCoreApplicationPrivate*>(QCoreApplicationPrivate::get(app)) : nullptr){
        argc = pvt->argc;
        argv = pvt->argv;
    }else{
        argc = int(env->GetArrayLength(args));
        argv = new char*[size_t(argc+1)];
        appName = qAppName().toUtf8();
        if(appName.isEmpty())
            appName = _name.data();
        argv[0] = appName.data();
        argsBuffers.resize(argc);
        for(jsize i=0; i<argc; ++i){
            jstring strg = jstring(env->GetObjectArrayElement(args, i));
            const char* data = strg && env->GetStringUTFLength(strg)>0 ? env->GetStringUTFChars(strg, nullptr) : nullptr;
            argsBuffers[i] = data;
            argv[i+1] = argsBuffers[i].data();
            if(data)
                env->ReleaseStringUTFChars(strg, data);
        }
    }
    QObject* _setup = qtjambi_object_from_nativeId<QObject>(setup);
    return quick_test_main_with_setup(argc, argv, _name.data(), _sourceDir.data(), _setup);
}
