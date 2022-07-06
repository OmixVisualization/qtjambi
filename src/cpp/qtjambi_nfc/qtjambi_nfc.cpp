#include "qtjambi_nfc.h"
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_repository.h>

void initialize_meta_info_QtNfc(){
#if defined(Q_OS_ANDROID)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define ORG_QTPROJECT_QT "org/qtproject/qt/"
#else
#define ORG_QTPROJECT_QT "org/qtproject/qt5/"
#endif
    if(JNIEnv* env = qtjambi_current_environment()){
        jclass cls = env->FindClass(ORG_QTPROJECT_QT "android/nfc/QtNfc");
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
