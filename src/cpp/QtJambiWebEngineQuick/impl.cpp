/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#if __has_include(<QtWebEngineQuick/private/qquickwebenginesettings_p.h>)
#include <QtWebEngineQuick/private/qquickwebenginesettings_p.h>
#define QUICK_WEBENGINE_SETTINGS
#endif
#if __has_include(<QtWebEngineQuick/private/qquickwebenginescriptcollection_p.h>)
#include <QtWebEngineQuick/private/qquickwebenginescriptcollection_p.h>
#define QUICK_WEBENGINE_SCRIPTCOLLECTION
#endif
#if __has_include(<QtWebEngineQuick/private/qquickwebenginescriptcollection_p_p.h>)
#include <QtWebEngineQuick/private/qquickwebenginescriptcollection_p_p.h>
#define QUICK_WEBENGINE_SCRIPTCOLLECTION
#endif
#include <QtWebEngineCore/QWebEngineSettings>
#include <QtWebEngineCore/QWebEngineScriptCollection>

#include <QtJambi/QtJambiAPI>
#include <QtJambi/CoreAPI>
#include <QtJambi/Cast>

#if !defined(QUICK_WEBENGINE_SETTINGS)
struct QQuickWebEngineSettings : QObject { QScopedPointer<QWebEngineSettings> d_ptr; };
#endif

#if !defined(QUICK_WEBENGINE_SCRIPTCOLLECTION)
struct QWebEngineScriptCollection : QObject { QScopedPointer<QWebEngineScriptCollection> d; };
#endif

class QQuickWebEngineViewPrivate{
public:
    static QWebEngineSettings* getWebEngineSettings(QObject* settings){
        return static_cast<QQuickWebEngineSettings*>(settings)->d_ptr.data();
    }

    static QWebEngineScriptCollection* getWebEngineScriptCollection(QObject* collection){
        return static_cast<QQuickWebEngineScriptCollection*>(collection)->d.data();
    }
};

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_webengine_quick_QtWebEngineQuick_toWebEngineSettings(JNIEnv *env, jclass, jobject object){
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QObject *_object = qtjambi_cast<QObject*>(env, object);
        if(_object->inherits("QQuickWebEngineSettings")){
            QWebEngineSettings* wSettings = QQuickWebEngineViewPrivate::getWebEngineSettings(_object);
            __java_return_value = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, wSettings);
            CoreAPI::registerDependentObject(env, __java_return_value, object);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_webengine_quick_QtWebEngineQuick_toWebEngineScriptCollection(JNIEnv *env, jclass, jobject object){
    jobject __java_return_value{0};
    QTJAMBI_TRY {
        QObject *_object = qtjambi_cast<QObject*>(env, object);
        if(_object->inherits("QQuickWebEngineScriptCollection")){
            QWebEngineScriptCollection* wCollection = QQuickWebEngineViewPrivate::getWebEngineScriptCollection(_object);
            __java_return_value = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, wCollection);
            CoreAPI::registerDependentObject(env, __java_return_value, object);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}
