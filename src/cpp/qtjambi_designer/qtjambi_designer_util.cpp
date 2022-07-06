#include <qtjambi/qtjambi_core.h>

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_designer_util_QtJambi_1LibraryUtilities_internalAccess)(JNIEnv *env, jclass cls){
    jobject result{0};
    QTJAMBI_TRY{
        result = qtjambi_get_internal_access(env, cls);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

