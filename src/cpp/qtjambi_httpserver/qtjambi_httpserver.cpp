#include <QtCore/QtGlobal>
#include <QtHttpServer/QHttpServer>
#include <QtHttpServer/private/qhttpserver_p.h>
#include <QtHttpServer/private/qhttpserverrouter_p.h>
#include <QtCore/private/qobject_p.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_repository.h>
#include <qtjambi/qtjambi_cast.h>

struct HttpServer : QAbstractHttpServer{
    inline void afterRequestImpl(QHttpServer::AfterRequestHandler afterRequestHandler)
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
};

struct HttpServerRouterRule : QHttpServerRouterRule{
    bool hasValidMethods() const{ return QHttpServerRouterRule::hasValidMethods(); }

    bool createPathRegexp(std::initializer_list<QMetaType> metaTypes,
                          const QHash<QMetaType, QLatin1StringView> &converters){
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
        QHttpServer *__qt_this = qtjambi_object_from_nativeId<QHttpServer>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QHttpServerRouterRule *rule = qtjambi_object_from_nativeId<QHttpServerRouterRule>(__rule);
        qtjambi_check_resource(__jni_env, rule);
        qtjambi_set_cpp_ownership(__jni_env, __rule);
        QList<QMetaType>* metaTypes = qtjambi_object_from_nativeId<QList<QMetaType>>(__metaTypes);
        qtjambi_check_resource(__jni_env, metaTypes);
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
        QHttpServer *__qt_this = qtjambi_object_from_nativeId<QHttpServer>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QHttpServerResponse *response = qtjambi_object_from_nativeId<QHttpServerResponse>(__response);
        qtjambi_check_resource(__jni_env, response);
        const QHttpServerRequest *request = qtjambi_object_from_nativeId<QHttpServerRequest>(__request);
        qtjambi_check_resource(__jni_env, request);
        QTcpSocket *socket = qtjambi_object_from_nativeId<QTcpSocket>(__socket);
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
        QHttpServer *__qt_this = qtjambi_object_from_nativeId<QHttpServer>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        const QHttpServerRequest *request = qtjambi_object_from_nativeId<QHttpServerRequest>(__request);
        qtjambi_check_resource(__jni_env, request);
        QTcpSocket *socket = qtjambi_object_from_nativeId<QTcpSocket>(__socket);
        result = reinterpret_cast<HttpServer*>(__qt_this)->superhandleRequest(*request, socket);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
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
        QHttpServer *__qt_this = qtjambi_object_from_nativeId<QHttpServer>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(isBi){
            __qt_this->afterRequest([action = JObjectWrapper(__jni_env, _action)](QHttpServerResponse &&resp, const QHttpServerRequest &request){
                if(JNIEnv *env = qtjambi_current_environment()){
                    jobject _resp = qtjambi_cast<jobject>(env, &resp);
                    jobject _request = qtjambi_cast<jobject>(env, &request);
                    jobject result = Java::Runtime::BiFunction::apply(env, action.object(), _resp, _request);
                    QHttpServerResponse* _result = qtjambi_cast<QHttpServerResponse*>(env, result);
                    qtjambi_invalidate_object(env, _resp);
                    qtjambi_invalidate_object(env, _request);
                    if(_result)
                        return std::move(*_result);
                }
                return std::move(resp);
            });
        }else{
            __qt_this->afterRequest([action = JObjectWrapper(__jni_env, _action)](QHttpServerResponse &&resp){
                if(JNIEnv *env = qtjambi_current_environment()){
                    jobject _resp = qtjambi_cast<jobject>(env, &resp);
                    jobject result = Java::Runtime::Function::apply(env, action.object(), _resp);
                    QHttpServerResponse* _result = qtjambi_cast<QHttpServerResponse*>(env, result);
                    qtjambi_invalidate_object(env, _resp);
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
