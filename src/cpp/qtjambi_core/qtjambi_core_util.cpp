#include <qtjambi/qtjambi_core.h>
#include "qtjambi_core_repository.h"
#include "qtjambiqfuture.h"

static QtMessageHandler qt_message_handler;
static bool qt_message_handler_installed;
void qtjambi_messagehandler_proxy(QtMsgType type, const QMessageLogContext & context, const QString & message)
{
    if(JNIEnv *env = qtjambi_current_environment()) {
        try{
            QTJAMBI_JNI_LOCAL_FRAME(env, 200)
            jboolean success = Java::Private::QtCore::QMessageHandler.process(env, jint(type), qtjambi_from_qstring(env, message));
            if (!success && qt_message_handler)
                qt_message_handler(type, context, message);
        }catch(const JavaException& exn){
            exn.report(env);
        }
    }else if (qt_message_handler)
        qt_message_handler(type, context, message);
}




extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMessageHandler_installMessageHandlerProxy)
(JNIEnv *env, jclass)
{
    try{
        if (!qt_message_handler_installed) {
            // returns old handler, which we save
            qt_message_handler = qInstallMessageHandler(qtjambi_messagehandler_proxy);
            qt_message_handler_installed = true;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QMessageHandler_removeMessageHandlerProxy)
(JNIEnv *env, jclass)
{
    try{
        if (qt_message_handler_installed) {
            // restore original handler
            qInstallMessageHandler(qt_message_handler);
            qt_message_handler = nullptr;
            qt_message_handler_installed = false;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}
