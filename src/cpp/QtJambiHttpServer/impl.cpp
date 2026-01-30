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

#include <QtCore/QtGlobal>
#include <QtHttpServer/QHttpServer>
#include <QtCore/qloggingcategory.h>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/CoreAPI>
#include <QtJambi/JObjectWrapper>
#include <QtJambi/JavaAPI>
#include <QtJambi/qtjambi_cast.h>

#if QT_VERSION < QT_VERSION_CHECK(6, 5, 0)
typedef void (QHttpServer::*SendResponse)(QHttpServerResponse &&, const QHttpServerRequest &, QTcpSocket *);
typedef bool (QHttpServer::*HandleRequest)(const QHttpServerRequest &, QTcpSocket*);
typedef void (QHttpServer::*MissingHandler)(const QHttpServerRequest &, QTcpSocket*);
#elif QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
typedef void (QHttpServer::*SendResponse)(QHttpServerResponse &&, const QHttpServerRequest &, QHttpServerResponder &&);
typedef bool (QHttpServer::*HandleRequest)(const QHttpServerRequest &, QHttpServerResponder &);
typedef void (QHttpServer::*MissingHandler)(const QHttpServerRequest &, QHttpServerResponder &&);
#else
typedef void (QHttpServer::*SendResponse)(QHttpServerResponse &&, const QHttpServerRequest &, QHttpServerResponder &&);
typedef bool (QHttpServer::*HandleRequest)(const QHttpServerRequest &, QHttpServerResponder &);
typedef void (QHttpServer::*MissingHandler)(const QHttpServerRequest &, QHttpServerResponder &);
#endif

template<>
constexpr void *QHttpServer::route<void,SendResponse>(const QString &,SendResponse && out)
{
    out = &QHttpServer::sendResponse;
    return nullptr;
}

template<>
constexpr void *QHttpServer::route<void,HandleRequest>(const QString &,HandleRequest && out)
{
    out = &QHttpServer::handleRequest;
    return nullptr;
}

template<>
constexpr void *QHttpServer::route<void,MissingHandler>(const QString &,MissingHandler && out)
{
    out = &QHttpServer::missingHandler;
    return nullptr;
}

struct AddRuleArguments{
    QHttpServerRouterRule *rule = nullptr;
    QList<QMetaType>* metaTypes = nullptr;
};

template<>
QHttpServerRouterRule *QHttpServerRouter::addRule<AddRuleArguments,void>(std::unique_ptr<QHttpServerRouterRule> ptr){
    AddRuleArguments* args = reinterpret_cast<AddRuleArguments*>(ptr.release());
    return addRuleImpl(std::unique_ptr<QHttpServerRouterRule>(args->rule), QtJambiAPI::createIterable<std::initializer_list<QMetaType>>(args->metaTypes->data(), args->metaTypes->size()));
}

extern "C" JNIEXPORT bool JNICALL Java_io_qt_httpserver_QHttpServerRouter_addRuleImpl
    (JNIEnv *__jni_env,
     jobject __this,
     QtJambiNativeID __this_nativeId,
     QtJambiNativeID __rule,
     QtJambiNativeID __metaTypes)
{
    Q_UNUSED(__this)
    bool result = false;
    QTJAMBI_TRY {
        QHttpServerRouter *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServerRouter>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QHttpServer::addRuleImpl(std::unique_ptr<QHttpServerRouterRule>, std::initializer_list<QMetaType>)", __qt_this)
        AddRuleArguments arguments;
        arguments.rule = QtJambiAPI::objectFromNativeId<QHttpServerRouterRule>(__rule);
        arguments.metaTypes = QtJambiAPI::objectFromNativeId<QList<QMetaType>>(__metaTypes);
        QtJambiAPI::checkNullPointer(__jni_env, arguments.rule);
        QtJambiAPI::checkNullPointer(__jni_env, arguments.metaTypes);
        QtJambiAPI::setCppOwnership(__jni_env, __rule);
        QHttpServerRouterRule *returnedRule = __qt_this->addRule<AddRuleArguments,void>(std::unique_ptr<QHttpServerRouterRule>{reinterpret_cast<QHttpServerRouterRule*>(&arguments)});
        if(returnedRule==arguments.rule){
            result = true;
            CoreAPI::registerDependentObject(__rule, __this_nativeId);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_httpserver_QHttpServer_sendResponse
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
        SendResponse function{nullptr};
        __qt_this->route<void,SendResponse>({}, std::move(function));
        (__qt_this->*function)(std::move(*response), *request, std::move(argX));
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
extern "C" JNIEXPORT void JNICALL Java_io_qt_httpserver_QHttpServer_sendResponseFuture
    (JNIEnv *__jni_env,
     jobject __this,
     QtJambiNativeID __this_nativeId,
     jobject __response,
     QtJambiNativeID __request,
     QtJambiNativeID __argX)
{
    Q_UNUSED(__this)
    QTJAMBI_TRY {
        QHttpServer *__qt_this = QtJambiAPI::objectFromNativeId<QHttpServer>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QHttpServer::sendResponse(QFuture<QHttpServerResponse> &&response, const QHttpServerRequest &request, QHttpServerResponder &&responder)", __qt_this)
        QFuture<QVariant>& response = QtJambiAPI::convertJavaObjectToNativeReference<QFuture<QVariant>>(__jni_env, __response);
        const QHttpServerRequest *request = QtJambiAPI::objectFromNativeId<QHttpServerRequest>(__request);
        QtJambiAPI::checkNullPointer(__jni_env, request);
        QHttpServerResponder& argX = QtJambiAPI::objectReferenceFromNativeId<QHttpServerResponder>(__jni_env, __argX);
        response.then(__qt_this,
                      [__qt_this, &request,
                       responder = std::move(argX)](QVariant &&vresponse) mutable {
                          if(vresponse.metaType()==QMetaType::fromType<QHttpServerResponse*>()){
                              QHttpServerResponse* response = vresponse.value<QHttpServerResponse*>();
                              SendResponse function{nullptr};
                              __qt_this->route<void,SendResponse>({}, std::move(function));
                              (__qt_this->*function)(std::move(*response), *request, std::move(responder));
                          }else if(vresponse.metaType()==QMetaType::fromType<JObjectWrapper>()){
                              if(JniEnvironment env{128}){
                                  JObjectWrapper object = vresponse.value<JObjectWrapper>();
                                  SendResponse function{nullptr};
                                  __qt_this->route<void,SendResponse>({}, std::move(function));
                                  (__qt_this->*function)(std::move(*qtjambi_cast<QHttpServerResponse*>(env, object.object(env))), *request, std::move(responder));
                              }
                          }
                      });
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 8, 0)

extern "C" JNIEXPORT void JNICALL Java_io_qt_httpserver_QHttpServer_afterRequest
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
                    QTJAMBI_TRY{
                        jobject _resp = qtjambi_cast<jobject>(env, &resp);
                        jobject _request = qtjambi_cast<jobject>(env, &request);
                        QTJAMBI_INVALIDATE_AFTER_USE(env, _resp);
                        QTJAMBI_INVALIDATE_AFTER_USE(env, _request);
                        jobject result = Java::Runtime::BiFunction::apply(env, action.object(env), _resp, _request);
                        QHttpServerResponse* _result = qtjambi_cast<QHttpServerResponse*>(env, result);
                        if(_result)
                            return std::move(*_result);
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
                return std::move(resp);
            });
        }else{
            __qt_this->afterRequest([action = JObjectWrapper(__jni_env, _action)](QHttpServerResponse &&resp){
                if(JniEnvironment env{300}){
                    QTJAMBI_TRY{
                        jobject _resp = qtjambi_cast<jobject>(env, &resp);
                        QTJAMBI_INVALIDATE_AFTER_USE(env, _resp);
                        jobject result = Java::Runtime::Function::apply(env, action.object(env), _resp);
                        QHttpServerResponse* _result = qtjambi_cast<QHttpServerResponse*>(env, result);
                        if(_result)
                            return std::move(*_result);
                    }QTJAMBI_CATCH(const JavaException& exn){
                        exn.report(env);
                    }QTJAMBI_TRY_END
                }
                return std::move(resp);
            });
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
#endif //QT_VERSION < QT_VERSION_CHECK(6, 8, 0)


extern "C" JNIEXPORT bool JNICALL Java_io_qt_httpserver_QHttpServer_handleRequest
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
        HandleRequest function{nullptr};
        __qt_this->route<void,HandleRequest>({}, std::move(function));
        result = (__qt_this->*function)(*request, argX);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_httpserver_QHttpServer_missingHandler
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
        MissingHandler function{nullptr};
        __qt_this->route<void,MissingHandler>({}, std::move(function));
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0) && QT_VERSION < QT_VERSION_CHECK(6, 8, 0)
        (__qt_this->*function)(*request, std::move(argX));
#else
        (__qt_this->*function)(*request, argX);
#endif
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}
