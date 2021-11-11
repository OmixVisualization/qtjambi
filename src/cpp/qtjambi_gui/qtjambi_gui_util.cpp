#include <QtGui/QGradient>
#include <QtGui/QGuiApplication>
#include <qtjambi/qtjambi_core.h>
#include "qtjambi_gui_repository.h"

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QPainter_threadCheck)
(JNIEnv *env, jclass, jobject _object)
{
    try{
        QObject* object = qtjambi_to_QObject<QObject>(env, _object);
        qtjambi_check_resource(env, object);
        qtjambi_argument_thread_check(env, "device", object);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6,0,0)

#include <QtGui/QOpenGLContext>
#include <QtGui/qopenglversionfunctions.h>
#include <QtGui/QOpenGLFunctions_ES2>
#include <qtjambi/qtjambi_cast.h>

// QOpenGLContext::versionFunctions<T>() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QOpenGLContext_versionFunctions)
(JNIEnv *__jni_env,
 jobject _this,
 jclass type)
{
    try{
        const QOpenGLContext *__qt_this = qtjambi_to_object<QOpenGLContext>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        QString className = qtjambi_class_name(__jni_env, type);
        if(className == "io.qt.gui.QOpenGLFunctions_ES2"){
#if defined(QT_OPENGL_ES_2)
            return qtjambi_cast<jobject>(__jni_env, __qt_this->versionFunctions<QOpenGLFunctions_ES2>());
#else
        }else if(className.startsWith("io.qt.gui.QOpenGLFunctions_")){
            className = className.mid(27);
            int idx = className.indexOf('_');
            if(idx>0){
                bool ok = false;
                int majorVersion = className.left(idx).toInt(&ok);
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
                        return qtjambi_cast<jobject>(__jni_env, __qt_this->versionFunctions(version));
                    }
                }
            }
#endif
        }
        //if(!__jni_env->IsInstanceOf(__java_return_value, type))
        //    __java_return_value = nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}
#else
#include <QtGui/QPointerEvent>
#include <QtGui/QAction>
#include <qtjambi/qtjambi_cast.h>

// QAction::setMenu(QMenu * menu)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QAction_setMenuObject)
(JNIEnv *__jni_env,
 jobject _this,
 jobject arg__1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QAction::setMenu(QMenu * menu)")
    try{
#if QT_CONFIG(action)
        QAction *__qt_this = qtjambi_to_QObject<QAction>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        static bool iswidgetApplication = QGuiApplication::instance()->inherits("QApplication");
        if(iswidgetApplication && arg__1 && !Java::QtWidgets::QMenu::isInstanceOf(__jni_env, arg__1)){
            JavaException::raiseIllegalArgumentException(__jni_env, "QAction.setMenu(menu) expects a QMenu in widget applications." QTJAMBI_STACKTRACEINFO );
        }
        QObject* __qt_arg__1 = qtjambi_cast<QObject*>(arg__1);
        __qt_this->setMenu(__qt_arg__1);
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(arg__1)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(action)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QAction::menu() const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QAction_menuObject)
(JNIEnv *__jni_env,
 jobject _this)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QAction::menu() const")
    try{
#if QT_CONFIG(action)
        QAction *__qt_this = qtjambi_to_QObject<QAction>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject* __qt_return_value = __qt_this->menu<QObject*>();
        return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
#else
        Q_UNUSED(__this_nativeId)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(action)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QPointerEvent_setPoint)
(JNIEnv *__jni_env,
 jobject _this,
 jlong i0,
 jobject __point)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QPointerEvent::setPoint(qsizetype, QEventPoint)")
    try{
        QPointerEvent *__qt_this = qtjambi_to_object<QPointerEvent>(__jni_env, _this);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(QEventPoint *__qt_point = qtjambi_cast<QEventPoint*>(__jni_env, __point)){
            QEventPoint& point = __qt_this->point(static_cast<qsizetype>(i0));
            point = *__qt_point;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,12,0)
// QGradient::QGradient(QGradient::Preset arg__1)
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QGradient_create)
(JNIEnv *__jni_env, jclass, int preset)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QGradient::QGradient(QGradient::Preset arg__1)")
    try{
        return qtjambi_cast<jobject>(__jni_env, QGradient(QGradient::Preset(preset)));
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;
}
#endif

