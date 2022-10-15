#include <qtjambi/qtjambi_core.h>

#include <QtOpenGL/QOpenGLVersionFunctionsFactory>
#include <QtOpenGL/qopenglversionfunctions.h>
#include <QtOpenGL/QOpenGLFunctions_ES2>
#include <qtjambi/qtjambi_cast.h>

QAbstractOpenGLFunctions* qtjambi_QOpenGLVersionFunctionsFactory_get(JNIEnv *__jni_env, jclass type, QOpenGLContext* context){
    //QAbstractOpenGLFunctions* __qt_return_value = nullptr;
    QString className = qtjambi_class_name(__jni_env, type);
    if(className == "io.qt.opengl.QOpenGLFunctions_ES2"){
#if defined(QT_OPENGL_ES_2)
        return QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_ES2>(context);
#else
    }else if(className.startsWith("io.qt.opengl.QOpenGLFunctions_")){
        className = className.mid(30);
        auto idx = className.indexOf('_');
        if(idx>0){
            bool ok = false;
            int majorVersion = QStringView(className).left(idx).toInt(&ok);
            if(ok){
                className = className.mid(idx+1);
                QSurfaceFormat::OpenGLContextProfile profile = QSurfaceFormat::NoProfile;
                if(className.endsWith("_Core")){
                    className = className.chopped(5);
                    profile = QSurfaceFormat::CoreProfile;
                }else if(className.endsWith("_Compatibility")){
                    className = className.chopped(14);
                    profile = QSurfaceFormat::CompatibilityProfile;
                }
                ok = false;
                int minorVersion = className.toInt(&ok);
                if(ok){
                    QOpenGLVersionProfile version;
                    version.setProfile(profile);
                    version.setVersion(majorVersion, minorVersion);
                    return QOpenGLVersionFunctionsFactory::get(version, context);
                }
            }
        }
#endif
    }
    return nullptr;
}

