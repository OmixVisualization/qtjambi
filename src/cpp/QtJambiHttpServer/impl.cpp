/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    inline void afterRequestImpl(std::function<QHttpServerResponse(QHttpServerResponse &&response,
                                                                           const QHttpServerRequest &request)> afterRequestHandler)
    {
        QHttpServerPrivate* d = reinterpret_cast<QHttpServerPrivate*>(QObjectPrivate::get(this));
        d->afterRequestHandlers.push_back(std::move(afterRequestHandler));
    }

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

extern "C" Q_DECL_EXPORT bool JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServerRouter_addRuleImpl)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID __rule,
 QtJambiNativeID __metaTypes)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHttpServer::handleRequest()")
    Q_UNUSED(__this)
    bool result = false;
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, __qt_this);
        QHttpServerRouterRule *rule = QtJambiAPI::objectFromNativeId<QHttpServerRouterRule>(__rule);
        QtJambiAPI::checkPointer(__jni_env, rule);
        QtJambiAPI::setCppOwnership(__jni_env, __rule);
        QList<QMetaType>* metaTypes = QtJambiAPI::objectFromNativeId<QList<QMetaType>>(__metaTypes);
        QtJambiAPI::checkPointer(__jni_env, metaTypes);
#ifdef Q_CC_MSVC
        result = reinterpret_cast<HttpServerRouter*>(__qt_this)->addRuleImpl(std::unique_ptr<QHttpServerRouterRule>(rule), std::initializer_list<QMetaType>(metaTypes->data(), metaTypes->data()+metaTypes->size()));
#else
        struct{
            QMetaType* _M_array;
            size_t _M_len;
        } __initializer_tmp;
        __initializer_tmp._M_array = metaTypes->data();
        __initializer_tmp._M_len = size_t(metaTypes->size());
        result = reinterpret_cast<HttpServerRouter*>(__qt_this)->addRuleImpl(std::unique_ptr<QHttpServerRouterRule>(rule), *reinterpret_cast<std::initializer_list<QMetaType>*>(&__initializer_tmp));
#endif
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
 QtJambiNativeID __socket)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHttpServer::sendResponse()")
    Q_UNUSED(__this)
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, __qt_this);
        QHttpServerResponse *response = QtJambiAPI::objectFromNativeId<QHttpServerResponse>(__response);
        QtJambiAPI::checkPointer(__jni_env, response);
        const QHttpServerRequest *request = QtJambiAPI::objectFromNativeId<QHttpServerRequest>(__request);
        QtJambiAPI::checkPointer(__jni_env, request);
        QTcpSocket *socket = QtJambiAPI::objectFromNativeId<QTcpSocket>(__socket);
        reinterpret_cast<HttpServer*>(__qt_this)->sendResponse(std::move(*response), *request, socket);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT bool JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServer_handleRequest)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID __request,
 QtJambiNativeID __socket)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHttpServer::handleRequest()")
    Q_UNUSED(__this)
    bool result = false;
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, __qt_this);
        const QHttpServerRequest *request = QtJambiAPI::objectFromNativeId<QHttpServerRequest>(__request);
        QtJambiAPI::checkPointer(__jni_env, request);
        QTcpSocket *socket = QtJambiAPI::objectFromNativeId<QTcpSocket>(__socket);
        result = reinterpret_cast<HttpServer*>(__qt_this)->superhandleRequest(*request, socket);
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
 QtJambiNativeID __socket)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHttpServer::missingHandler()")
    Q_UNUSED(__this)
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, __qt_this);
        const QHttpServerRequest *request = QtJambiAPI::objectFromNativeId<QHttpServerRequest>(__request);
        QtJambiAPI::checkPointer(__jni_env, request);
        QTcpSocket *socket = QtJambiAPI::objectFromNativeId<QTcpSocket>(__socket);
        reinterpret_cast<HttpServer*>(__qt_this)->superMissingHandler(*request, socket);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_httpserver_QHttpServer_afterRequest)
(JNIEnv *__jni_env,
 jobject __this,
 QtJambiNativeID __this_nativeId,
 jboolean isBi,
 jobject _action)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QHttpServer::afterRequest()")
    Q_UNUSED(__this)
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkPointer(__jni_env, __qt_this);
        if(isBi){
            __qt_this->afterRequest([action = JObjectWrapper(__jni_env, _action)](QHttpServerResponse &&resp, const QHttpServerRequest &request){
                if(JniEnvironment env{300}){
                    jobject _resp = qtjambi_cast<jobject>(env, &resp);
                    jobject _request = qtjambi_cast<jobject>(env, &request);
                    jobject result = Java::Runtime::BiFunction::apply(env, action.object(), _resp, _request);
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
                    jobject result = Java::Runtime::Function::apply(env, action.object(), _resp);
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
