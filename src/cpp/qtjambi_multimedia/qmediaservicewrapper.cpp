#include "qmediaservicewrapper.h"
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_global.h>
#include <qtjambi/qtjambitypemanager_p.h>
#include <qmediaservice.h>
#include <qobject.h>

jobject Java_org_qtjambi_qt_multimedia_QMediaService_requestControl
  (JNIEnv * env, jobject this_object, jclass type){
    QMediaService*  mediaService = (QMediaService* ) qtjambi_to_qobject(env, this_object);
    QString javaName = qtjambi_class_name(env, type).replace('.', '/');
    QtJambiTypeManager manager(env, true, QtJambiTypeManager::DynamicMetaObjectMode);
    QString qtName = manager.getInternalTypeName(javaName, QtJambiTypeManager::ArgumentType);

    javaName = javaName.replace("/", "::");
    StaticCache *sc = StaticCache::instance();
    sc->resolveQtJambiObject();
    sc->resolveQtJambiInternal();
    if(env->CallStaticBooleanMethod(sc->QtJambiInternal.class_ref, sc->QtJambiInternal.isGeneratedClass, type)){
        if(qtName.endsWith("*")){
            qtName = qtName.replace("*", "");
        }
        QMediaControl *control = mediaService->requestControl(qtName.toLatin1().constData());
        return qtjambi_from_QObject(env, control);
    }else{
        return 0;
    }
}
