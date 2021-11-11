#include <qtjambi/qtjambi_core.h>

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_designer_util_CustomWidgetInterface_lambdaReturnType)(JNIEnv *env, jclass, jobject object){
    try{
        return qtjambi_lambda_return_type(env, object);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
