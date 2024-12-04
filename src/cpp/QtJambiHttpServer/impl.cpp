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

#include <QtCore/QtGlobal>
#include <QtHttpServer/QHttpServer>
#include <QtHttpServer/private/qhttpserver_p.h>
#include <QtHttpServer/private/qhttpserverrouter_p.h>
#include <QtCore/private/qobject_p.h>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/JavaAPI>
#include <QtJambi/qtjambi_cast.h>

struct HttpServer : QAbstractHttpServer{
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
    inline void sendResponse(QHttpServerResponse &&response,
                                   const QHttpServerRequest &request,
                                   QTcpSocket *socket)
    {
        QHttpServerPrivate* d = reinterpret_cast<QHttpServerPrivate*>(QObjectPrivate::get(this));
        for (auto afterRequestHandler : d->afterRequestHandlers)
            response = afterRequestHandler(std::move(response), request);
        response.write(makeResponder(request, socket));
    }

    bool superhandleRequest(const QHttpServerRequest& request0, QTcpSocket* socket1){
        return handleRequest(request0, socket1);
    }
    void superMissingHandler(const QHttpServerRequest& request0, QTcpSocket* socket1){
        missingHandler(request0, socket1);
    }
#elif QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
    inline void afterRequestImpl(std::function<QHttpServerResponse(QHttpServerResponse &&response,
                                                                   const QHttpServerRequest &request)> afterRequestHandler)
    {
        QHttpServerPrivate* d = reinterpret_cast<QHttpServerPrivate*>(QObjectPrivate::get(this));
        d->afterRequestHandlers.push_back(std::move(afterRequestHandler));
    }

    inline void sendResponse(QHttpServerResponse &&response,
                             const QHttpServerRequest &request,
                             QHttpServerResponder &&responder)
    {
        QHttpServerPrivate* d = reinterpret_cast<QHttpServerPrivate*>(QObjectPrivate::get(this));
        for (auto afterRequestHandler : d->afterRequestHandlers)
            response = afterRequestHandler(std::move(response), request);
        responder.sendResponse(response);
    }

    inline void sendResponse(QFuture<QHttpServerResponse> &&response,
                             const QHttpServerRequest &request,
                             QHttpServerResponder &&responder)
    {
        response.then(this,
                      [this, &request,
                       responder = std::move(responder)](QHttpServerResponse &&response) mutable {
                          sendResponse(std::move(response), request, std::move(responder));
                      });
    }

    bool superhandleRequest(const QHttpServerRequest& request0, QHttpServerResponder &responder){
        return handleRequest(request0, responder);
    }
    void superMissingHandler(const QHttpServerRequest& request0, QHttpServerResponder &&responder){
        missingHandler(request0, std::move(responder));
    }
#else
    bool superhandleRequest(const QHttpServerRequest& request0, QHttpServerResponder &responder){
        return handleRequest(request0, responder);
    }
    void superMissingHandler(const QHttpServerRequest& request0, QHttpServerResponder &responder){
        missingHandler(request0, responder);
    }
#endif
};

struct HttpServerRouterRule : QHttpServerRouterRule{
    bool hasValidMethods() const{ return QHttpServerRouterRule::hasValidMethods(); }

    bool createPathRegexp(std::initializer_list<QMetaType> metaTypes,
                          const QHash<QMetaType, QString> &converters){
        return QHttpServerRouterRule::createPathRegexp(metaTypes, converters);
    }
};

struct HttpServerRouter{
    bool addRuleImpl(std::unique_ptr<QHttpServerRouterRule> rule,
                                        std::initializer_list<QMetaType> metaTypes)
    {
        Q_D(QHttpServerRouter);

        if (!reinterpret_cast<HttpServerRouterRule*>(rule.get())->hasValidMethods() || !reinterpret_cast<HttpServerRouterRule*>(rule.get())->createPathRegexp(metaTypes, d->converters)) {
            return false;
        }

        d->rules.push_back(std::move(rule));
        return true;
    }

    Q_DECLARE_PRIVATE(QHttpServerRouter)
    QScopedPointer<QHttpServerRouter> d_ptr;
};

#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)

extern "C" Q_DECL_EXPORT bool JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServerRouter_addRuleImpl)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID __rule,
 QtJambiNativeID __metaTypes)
{
    Q_UNUSED(__this)
    bool result = false;
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QHttpServer::addRuleImpl(std::unique_ptr<QHttpServerRouterRule>, std::initializer_list<QMetaType>)", __qt_this)
        QHttpServerRouterRule *rule = QtJambiAPI::objectFromNativeId<QHttpServerRouterRule>(__rule);
        QtJambiAPI::checkNullPointer(__jni_env, rule);
        QtJambiAPI::setCppOwnership(__jni_env, __rule);
        QList<QMetaType>* metaTypes = QtJambiAPI::objectFromNativeId<QList<QMetaType>>(__metaTypes);
        QtJambiAPI::checkNullPointer(__jni_env, metaTypes);
        result = reinterpret_cast<HttpServerRouter*>(__qt_this)->addRuleImpl(std::unique_ptr<QHttpServerRouterRule>(rule), QtJambiAPI::createIterable<std::initializer_list<QMetaType>>(metaTypes->data(), metaTypes->size()));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServer_sendResponse)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID __response,
 QtJambiNativeID __request,
 QtJambiNativeID __argX)
{
    Q_UNUSED(__this)
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QHttpServer::sendResponse(QHttpServerResponse &&response, const QHttpServerRequest &request, QHttpServerResponder &&responder)", __qt_this)
        QHttpServerResponse *response = QtJambiAPI::objectFromNativeId<QHttpServerResponse>(__response);
        QtJambiAPI::checkNullPointer(__jni_env, response);
        const QHttpServerRequest *request = QtJambiAPI::objectFromNativeId<QHttpServerRequest>(__request);
        QtJambiAPI::checkNullPointer(__jni_env, request);
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
        QTcpSocket *argX = QtJambiAPI::objectFromNativeId<QTcpSocket>(__argX);
#else
        QHttpServerResponder& argX = QtJambiAPI::objectReferenceFromNativeId<QHttpServerResponder>(__jni_env, __argX);
#endif
        reinterpret_cast<HttpServer*>(__qt_this)->sendResponse(std::move(*response), *request, std::move(argX));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServer_sendResponseFuture)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 jobject __response,
 QtJambiNativeID __request,
 QtJambiNativeID __argX)
{
    QtJambiScope scope;
    Q_UNUSED(__this)
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QHttpServer::sendResponse(QFuture<QHttpServerResponse> &&response, const QHttpServerRequest &request, QHttpServerResponder &&responder)", __qt_this)
        QFuture<QHttpServerResponse>& response = qtjambi_cast<QFuture<QHttpServerResponse>&>(__jni_env, scope, __response);
        const QHttpServerRequest *request = QtJambiAPI::objectFromNativeId<QHttpServerRequest>(__request);
        QtJambiAPI::checkNullPointer(__jni_env, request);
        QHttpServerResponder& argX = QtJambiAPI::objectReferenceFromNativeId<QHttpServerResponder>(__jni_env, __argX);
        reinterpret_cast<HttpServer*>(__qt_this)->sendResponse(std::move(response), *request, std::move(argX));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
#endif

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServer_afterRequest)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 jboolean isBi,
 jobject _action)
{
    Q_UNUSED(__this)
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QHttpServer::afterRequest()", __qt_this)
        if(isBi){
            __qt_this->afterRequest([action = JObjectWrapper(__jni_env, _action)](QHttpServerResponse &&resp, const QHttpServerRequest &request){
                if(JniEnvironment env{300}){
                    jobject _resp = qtjambi_cast<jobject>(env, &resp);
                    jobject _request = qtjambi_cast<jobject>(env, &request);
                    jobject result = Java::Runtime::BiFunction::apply(env, action.object(env), _resp, _request);
                    QHttpServerResponse* _result = qtjambi_cast<QHttpServerResponse*>(env, result);
                    InvalidateAfterUse::invalidate(env, _resp);
                    InvalidateAfterUse::invalidate(env, _request);
                    if(_result)
                        return std::move(*_result);
                }
                return std::move(resp);
            });
        }else{
            __qt_this->afterRequest([action = JObjectWrapper(__jni_env, _action)](QHttpServerResponse &&resp){
                if(JniEnvironment env{300}){
                    jobject _resp = qtjambi_cast<jobject>(env, &resp);
                    jobject result = Java::Runtime::Function::apply(env, action.object(env), _resp);
                    QHttpServerResponse* _result = qtjambi_cast<QHttpServerResponse*>(env, result);
                    InvalidateAfterUse::invalidate(env, _resp);
                    if(_result)
                        return std::move(*_result);
                }
                return std::move(resp);
            });
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
#else

#endif //QT_VERSION < QT_VERSION_CHECK(6, 8, 0)


extern "C" Q_DECL_EXPORT bool JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServer_handleRequest)
    (JNIEnv *__jni_env,
     jobject __this,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID __request,
     QtJambiNativeID __argX)
{
    Q_UNUSED(__this)
    bool result = false;
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QHttpServer::handleRequest(const QHttpServerRequest& request0, QHttpServerResponder &responder)", __qt_this)
        const QHttpServerRequest *request = QtJambiAPI::objectFromNativeId<QHttpServerRequest>(__request);
        QtJambiAPI::checkNullPointer(__jni_env, request);
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
        QTcpSocket *argX = QtJambiAPI::objectFromNativeId<QTcpSocket>(__argX);
#else
        QHttpServerResponder& argX = QtJambiAPI::objectReferenceFromNativeId<QHttpServerResponder>(__jni_env, __argX);
#endif
        result = reinterpret_cast<HttpServer*>(__qt_this)->superhandleRequest(*request, argX);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServer_missingHandler)
    (JNIEnv *__jni_env,
     jobject __this,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID __request,
     QtJambiNativeID __argX)
{
    Q_UNUSED(__this)
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QHttpServer::missingHandler(const QHttpServerRequest &request, QHttpServerResponder &&responder)", __qt_this)
        const QHttpServerRequest *request = QtJambiAPI::objectFromNativeId<QHttpServerRequest>(__request);
        QtJambiAPI::checkNullPointer(__jni_env, request);
#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
        QTcpSocket *argX = QtJambiAPI::objectFromNativeId<QTcpSocket>(__argX);
#else
        QHttpServerResponder& argX = QtJambiAPI::objectReferenceFromNativeId<QHttpServerResponder>(__jni_env, __argX);
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
        reinterpret_cast<HttpServer*>(__qt_this)->superMissingHandler(*request, std::move(argX));
#else
        reinterpret_cast<HttpServer*>(__qt_this)->superMissingHandler(*request, argX);
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
