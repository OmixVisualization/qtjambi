#include <qtjambi/qtjambi_application.h>

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_uic_java_WriteClass_getExternalTypeName)
    (JNIEnv *env, jclass, jstring externalName)
{
    try{
        QString internalName = qtjambi_external_type_name(env, qtjambi_to_qstring(env, externalName));
        return qtjambi_from_qstring(env, internalName);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return nullptr;
}
