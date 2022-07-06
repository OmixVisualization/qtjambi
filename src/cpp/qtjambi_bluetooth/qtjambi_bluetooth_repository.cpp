#include <QtCore/QMutex>
#include "qtjambi_bluetooth_repository.h"

namespace Java{
namespace QtBluetooth{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/bluetooth,QBluetoothDeviceInfo$ServiceUuids,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/util/List;Lio/qt/bluetooth/QBluetoothDeviceInfo$DataCompleteness;)
)
}
}

void initialize_meta_info_QtBluetooth(){
#if defined(Q_OS_ANDROID)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define ORG_QTPROJECT_QT "org/qtproject/qt/"
#else
#define ORG_QTPROJECT_QT "org/qtproject/qt5/"
#endif
    if(JNIEnv* env = qtjambi_current_environment()){
        jclass cls = env->FindClass(ORG_QTPROJECT_QT "android/bluetooth/QtBluetoothBroadcastReceiver");
        if(env->ExceptionCheck())
            env->ExceptionClear();
        if(cls){
            jmethodID method = env->GetStaticMethodID(cls, "setContext", "(Landroid/content/Context;)V");
            if(env->ExceptionCheck())
                env->ExceptionClear();
            if(method){
                jobject activity = nullptr;
                try{
                    activity = Java::Android::QtNative::activity(env);
                }catch(...){}
                env->CallStaticVoidMethod(cls, method, activity);
                if(env->ExceptionCheck())
                    env->ExceptionClear();
            }
        }
    }
#endif
}
