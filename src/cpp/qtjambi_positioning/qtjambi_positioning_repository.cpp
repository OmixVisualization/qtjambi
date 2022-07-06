#include <QtCore/QMutex>
#include "qtjambi_positioning_repository.h"

namespace Java{
namespace QtPositioning {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/positioning,QNmeaPositionInfoSource$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/positioning/QGeoPositionInfo;Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(info,()Lio/qt/positioning/QGeoPositionInfo;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hasFix,()Z)
)
}
namespace QtJambi {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,QtJambiSignals$AbstractSignal,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(containingObject,()Lio/qt/QtSignalEmitterInterface;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(methodIndex,()I)
)
}
namespace QtCore {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaMethod,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toSignal,(Lio/qt/core/QObject;)Lio/qt/core/QMetaObject$AbstractSignal;)
)
}
}

void initialize_meta_info_QtPositioning(){
#if defined(Q_OS_ANDROID)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define ORG_QTPROJECT_QT "org/qtproject/qt/"
#else
#define ORG_QTPROJECT_QT "org/qtproject/qt5/"
#endif
    if(JNIEnv* env = qtjambi_current_environment()){
        jclass cls = env->FindClass(ORG_QTPROJECT_QT "android/positioning/QtPositioning");
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
