#include <QtJambi/QtJambiAPI>
#include <private/qguiapplication_p.h>
#include <qpa/qplatformintegration.h>
#include <QtJambi/qtjambi_cast.h>

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_qpa_QPlatformIntegration_instance)
    (JNIEnv *env, jclass)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        result = qtjambi_cast<jobject>(env, QGuiApplicationPrivate::platformIntegration());
        QtJambiAPI::setCppOwnership(env, result);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
        return result;
}
