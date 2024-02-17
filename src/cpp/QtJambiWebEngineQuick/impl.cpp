/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtWebEngineQuick/private/qquickwebenginesettings_p.h>
#include <QtWebEngineQuick/private/qquickwebenginescriptcollection_p.h>
#include <QtWebEngineQuick/private/qquickwebenginescriptcollection_p_p.h>
#include <QtWebEngineCore/QWebEngineSettings>
#include <QtWebEngineCore/QWebEngineScriptCollection>

#include <QtJambi/QtJambiAPI>
#include <QtJambi/CoreAPI>
#include <QtJambi/Cast>

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
            __java_return_value = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, wSettings);
            CoreAPI::registerDependentObject(env, __java_return_value, object);
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
            __java_return_value = QtJambiAPI::convertNativeToJavaObjectAsWrapper(env, wCollection);
            CoreAPI::registerDependentObject(env, __java_return_value, object);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return __java_return_value;
}
