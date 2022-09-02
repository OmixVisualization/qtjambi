
#include <QtWebEngineQuick/private/qquickwebenginesettings_p.h>
#include <QtWebEngineQuick/private/qquickwebenginescriptcollection_p.h>
#include <QtWebEngineQuick/private/qquickwebenginescriptcollection_p_p.h>
#include <QtWebEngineCore/QWebEngineSettings>
#include <QtWebEngineCore/QWebEngineScriptCollection>

#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cast.h>

class QQuickWebEngineViewPrivate{
public:
    static QWebEngineSettings* getWebEngineSettings(QQuickWebEngineSettings* settings){
        return settings->d_ptr.data();
    }

    static QWebEngineScriptCollection* getWebEngineScriptCollection(QQuickWebEngineScriptCollection* collection){
        return collection->d.data();
    }
};

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_webengine_quick_QtWebEngineQuick_toWebEngineSettings)
(JNIEnv *env, jclass, jobject object)
{
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QObject *_object = qtjambi_cast<QObject*>(env, object);
        if(QQuickWebEngineSettings* settings = qobject_cast<QQuickWebEngineSettings*>(_object)){
            QWebEngineSettings* wSettings = QQuickWebEngineViewPrivate::getWebEngineSettings(settings);
            __java_return_value = qtjambi_from_object(env, wSettings, false, true);
            qtjambi_register_dependent_object(env, __java_return_value, object);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_webengine_quick_QtWebEngineQuick_toWebEngineScriptCollection)
(JNIEnv *env, jclass, jobject object)
{
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QObject *_object = qtjambi_cast<QObject*>(env, object);
        if(QQuickWebEngineScriptCollection* collection = qobject_cast<QQuickWebEngineScriptCollection*>(_object)){
            QWebEngineScriptCollection* wCollection = QQuickWebEngineViewPrivate::getWebEngineScriptCollection(collection);
            __java_return_value = qtjambi_from_object(env, wCollection, false, true);
            qtjambi_register_dependent_object(env, __java_return_value, object);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}
