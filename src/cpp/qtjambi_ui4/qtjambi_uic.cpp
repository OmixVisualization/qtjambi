#include <QtDesigner/private/ui4_p.h>
#include <qtjambi/qtjambi_application.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_cast.h>

extern "C" Q_DECL_EXPORT jstring JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_uic_java_WriteClass_getExternalTypeName)
    (JNIEnv *env, jclass, jstring externalName)
{
    jstring result = nullptr;
    QTJAMBI_TRY{
        QString internalName = qtjambi_external_type_name(env, qtjambi_to_qstring(env, externalName));
        result = qtjambi_from_qstring(env, internalName);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
    return result;
}

void initialize_meta_info_UIC(){
    registerContainerAccessFactory(typeid(QHash<DomWidget*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomWidget*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomSpacer*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomSpacer*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomLayout*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomLayout*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomActionGroup*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomActionGroup*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomButtonGroup*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomButtonGroup*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<DomAction*,QString>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<DomAction*,QString>::newInstance));
    registerContainerAccessFactory(typeid(QHash<QString,bool>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<QString,bool>::newInstance));
    registerContainerAccessFactory(typeid(QHash<QString,DomProperty*>), NewContainerAccessFunction(&QtJambiPrivate::QHashAccess<QString,DomProperty*>::newInstance));
}
