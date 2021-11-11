#include <QtCore/QtGlobal>
#include <qconfig.h>
#if QT_CONFIG(future)
#include <qfutureinterface.h>
#include <qfuturewatcher.h>
#endif //QT_CONFIG(future)
#include <qtjambi/qtjambi_core.h>
#include <QtCore/QList>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtCore/private/qmetaobjectbuilder_p.h>
#if QT_CONFIG(future)
#include <qfuture.h>
#endif //QT_CONFIG(future)
#include <qmutex.h>
#include <qrunnable.h>
#if QT_CONFIG(thread)
#include <qthreadpool.h>
#endif //QT_CONFIG(thread)
#include "qtjambi_core_future.h"
#include "qtjambi_core_repository.h"
#include <qtjambi/qtjambi_jobjectwrapper.h>
#include <qtjambi/qtjambi_registry.h>
#include <qtjambi/qtjambi_repository.h>
#include <qtjambi/qtjambi_cast.h>

void qtjambi_exception_handler(JNIEnv *__jni_env, void* ptr, void(*expression)(void*)){
    Q_ASSERT(expression);
    try{
        expression(ptr);
    }catch(const JavaException&){
        throw;
    }catch(const QUnhandledException& exn){
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
        if(exn.exception()){
            qtjambi_exception_handler(__jni_env, const_cast<QUnhandledException*>(&exn), [](void* ptr){
                QUnhandledException* exn = reinterpret_cast<QUnhandledException*>(ptr);
                std::rethrow_exception(exn->exception());
            });
        }
#else
        Q_UNUSED(exn)
#endif
        Java::QtCore::QUnhandledException::throwNew(__jni_env, "An exception has been thrown in native code." QTJAMBI_STACKTRACEINFO );
    }catch(const QException& exn){
        if(typeid(exn)==typeid(QException)){
            Java::QtCore::QException::throwNew(__jni_env, "An exception has been thrown in native code." QTJAMBI_STACKTRACEINFO );
        }else{
            QString exceptionName(QLatin1String(qtjambi_type_name(typeid(exn))));
            const char* what = exn.what();
            const char* original_what = exn.std::exception::what();
            if(what && QLatin1String(what)!=QLatin1String(original_what) && exceptionName!=QLatin1String(what)){
                Java::QtCore::QException::throwNew(__jni_env, QString("An exception (%1) has been thrown in native code: %2").arg(exceptionName).arg(QLatin1String(what)) QTJAMBI_STACKTRACEINFO );
            }else{
                Java::QtCore::QException::throwNew(__jni_env, QString("An exception (%1) has been thrown in native code.").arg(exceptionName) QTJAMBI_STACKTRACEINFO );
            }
        }
    }catch(const std::exception& exn){
        QString exceptionName(QLatin1String(qtjambi_type_name(typeid(exn))));
        if(exn.what() && exceptionName!=QLatin1String(exn.what())){
            Java::QtCore::QUnhandledException::throwNew(__jni_env, QString("An exception (%1) has been thrown in native code: %2").arg(exceptionName).arg(QLatin1String(exn.what())) QTJAMBI_STACKTRACEINFO );
        }else{
            Java::QtCore::QUnhandledException::throwNew(__jni_env, QString("An exception (%1) has been thrown in native code.").arg(exceptionName) QTJAMBI_STACKTRACEINFO );
        }
    }catch(...){
        Java::QtCore::QUnhandledException::throwNew(__jni_env, "An exception has been thrown in native code." QTJAMBI_STACKTRACEINFO );
    }
}

#if QT_CONFIG(future)

const std::type_info& typeid_QFutureInterfaceBase_shell();

template<class T>
class QFutureInterface_shell : public QFutureInterface<T>, public QtJambiShellInterface
{
public:
    QFutureInterface_shell(QFutureInterfaceBase::State initialState0 = QFutureInterfaceBase::NoState);
    QFutureInterface_shell(const QFutureInterface<T>& other0);
    ~QFutureInterface_shell() override;

    static void operator delete(void * ptr) noexcept;
private:
    QtJambiShell* __shell() const override final;
    jmethodID __shell_javaMethod(int pos) const;
};

template<class T>
QtJambiShell* QFutureInterface_shell<T>::__shell() const { return *reinterpret_cast<QtJambiShell**>( quintptr(this) + sizeof(QFutureInterface_shell) ); }

template<class T>
jmethodID QFutureInterface_shell<T>::__shell_javaMethod(int pos) const { return QFutureInterface_shell::__shell()->javaMethod(typeid(QFutureInterface<T>), pos); }

// emitting (writeShellConstructor)
template<class T>
QFutureInterface_shell<T>::QFutureInterface_shell(QFutureInterfaceBase::State initialState0)
    : QFutureInterface<T>(initialState0)
{
    QTJAMBI_DEBUG_METHOD_PRINT_WHERE("shell", "QFutureInterface_shell::QFutureInterface_shell(QFutureInterfaceBase::State initialState0)", QFutureInterface_shell::__shell())
    QFutureInterface_shell::__shell()->constructed(typeid(QFutureInterface<T>));
}

template<class T>
QFutureInterface_shell<T>::QFutureInterface_shell(const QFutureInterface<T>& other0)
    : QFutureInterface<T>(other0)
{
    QTJAMBI_DEBUG_METHOD_PRINT_WHERE("shell", "QFutureInterface_shell::QFutureInterface_shell(const QFutureInterface<QVariant>& other0)", QFutureInterface_shell::__shell())
    QFutureInterface_shell::__shell()->constructed(typeid(QFutureInterface<T>));
}

// emitting (writeShellDestructor)
template<class T>
QFutureInterface_shell<T>::~QFutureInterface_shell()
{
    QTJAMBI_DEBUG_METHOD_PRINT_WHERE("shell", "QFutureInterface_shell::~QFutureInterface_shell()", QFutureInterface_shell::__shell())
    QFutureInterface_shell::__shell()->destructed(typeid(QFutureInterface<T>));
}

template<class T>
void QFutureInterface_shell<T>::operator delete(void * ptr) noexcept {
    reinterpret_cast<QFutureInterface_shell<T>*>(ptr)->QFutureInterface_shell<T>::__shell()->deleteShell();
}

// emitting Virtual overrides (virtualFunctions)
// emitting Functions in shell class (nonVirtualShellFunctions)
// emitting Public Override Functions (publicOverrideFunctions)
// emitting Virtual Override Functions (virtualOverrideFunctions)

#endif // QT_CONFIG(future)

// emitting (writeExtraFunctions)
// emitting (writeToStringFunction)
// emitting (writeCloneFunction)

extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface_clone)
(JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    try{

#if QT_CONFIG(future)

        const QFutureInterfaceBase *__qt_this = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(const QFutureInterface<QVariant>* _this = dynamic_cast<const QFutureInterface<QVariant>*>(__qt_this)){
            return qtjambi_cast<jobject>(__jni_env, *_this);
        }else if(const QFutureInterface<void>* _this = dynamic_cast<const QFutureInterface<void>*>(__qt_this)){
            return qtjambi_cast<jobject>(__jni_env, *_this);
        }else{
            return qtjambi_cast<jobject>(__jni_env, *__qt_this);
        }

#else
        Q_UNUSED(__this_nativeId)
        return nullptr;
#endif // QT_CONFIG(future)

    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

// emitting (writeSignalFunction)
// emitting (writeConstructors)
// new QFutureInterface(QFutureInterfaceBase::State initialState)
void __qt_construct_QFutureInterface_QVariant__0(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue*)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QFutureInterface(QFutureInterfaceBase::State initialState)")
#if QT_CONFIG(future)
    QFutureInterfaceBase *__qt_this = new(__qtjambi_ptr) QFutureInterface_shell<QVariant>();
    Q_UNUSED(__jni_env)
    Q_UNUSED(__qt_this)
#else
    Q_UNUSED(__qtjambi_ptr)
    JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
}

// new QFutureInterface(QFutureInterfaceBase::State initialState)
void __qt_construct_QFutureInterface_void__0(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue*)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QFutureInterface(QFutureInterfaceBase::State initialState)")
#if QT_CONFIG(future)
    QFutureInterfaceBase *__qt_this = new(__qtjambi_ptr) QFutureInterface_shell<void>();
    Q_UNUSED(__jni_env)
    Q_UNUSED(__qt_this)
#else
    Q_UNUSED(__qtjambi_ptr)
    JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
}

// new QFutureInterface(QFutureInterfaceBase::State initialState)
void __qt_construct_QFutureInterface_QVariant__1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QFutureInterface(QFutureInterfaceBase::State initialState)")
#if QT_CONFIG(future)
    jobject initialState0 = __java_arguments[0].l;
    QFutureInterfaceBase::State __qt_initialState0 = qtjambi_cast<QFutureInterfaceBase::State>(__jni_env, initialState0);
    QFutureInterfaceBase *__qt_this = new(__qtjambi_ptr) QFutureInterface_shell<QVariant>(QFutureInterfaceBase::State(__qt_initialState0));
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qt_this)
#else
    Q_UNUSED(__jni_object)
    Q_UNUSED(__java_arguments)
    Q_UNUSED(__qtjambi_ptr)
    JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
}

// new QFutureInterface(const QFutureInterface<QVariant> & other)
void __qt_construct_QFutureInterface_QVariant__2(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QFutureInterface(const QFutureInterface<QVariant> & other)")
#if QT_CONFIG(future)
    jobject other0 = __java_arguments[0].l;
    QFutureInterfaceBase* __qt_other0 = qtjambi_cast<QFutureInterfaceBase*>(__jni_env, other0);
    if(QFutureInterface<QVariant>* other = dynamic_cast<QFutureInterface<QVariant>*>(__qt_other0)){
        new(__qtjambi_ptr) QFutureInterface_shell<QVariant>(*other);
    }else if(dynamic_cast<QFutureInterface<void>*>(__qt_other0)){
        JavaException::raiseIllegalArgumentException(__jni_env, "QFutureInterface<void> not a valid argument for QFutureInterface<T>(QFutureInterface<T>)." QTJAMBI_STACKTRACEINFO );
    }else{
        JavaException::raiseIllegalArgumentException(__jni_env, "QFutureInterfaceBase not a valid argument for QFutureInterface<T>(QFutureInterface<T>)." QTJAMBI_STACKTRACEINFO );
    }
    Q_UNUSED(__jni_object)
#else
    Q_UNUSED(__jni_object)
    Q_UNUSED(__java_arguments)
    Q_UNUSED(__qtjambi_ptr)
    JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
}

// new QFutureInterface(QFutureInterfaceBase::State initialState)
void __qt_construct_QFutureInterface_void__1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QFutureInterface(QFutureInterfaceBase::State initialState)")
#if QT_CONFIG(future)
    jobject initialState0 = __java_arguments[0].l;
    QFutureInterfaceBase::State __qt_initialState0 = qtjambi_cast<QFutureInterfaceBase::State>(__jni_env, initialState0);
    QFutureInterfaceBase *__qt_this = new(__qtjambi_ptr) QFutureInterface_shell<void>(QFutureInterfaceBase::State(__qt_initialState0));
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qt_this)
#else
    Q_UNUSED(__jni_object)
    Q_UNUSED(__java_arguments)
    Q_UNUSED(__qtjambi_ptr)
    JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
}

// new QFutureInterface(const QFutureInterface<void> & other)
void __qt_construct_QFutureInterface_void__2(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QFutureInterface(const QFutureInterface<QVariant> & other)")
#if QT_CONFIG(future)
    jobject other0 = __java_arguments[0].l;
    QFutureInterfaceBase* __qt_other0 = qtjambi_cast<QFutureInterfaceBase*>(__jni_env, other0);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if(QFutureInterface<void>* other = dynamic_cast<QFutureInterface<void>*>(__qt_other0)){
        new(__qtjambi_ptr) QFutureInterface_shell<void>(*other);
    }else if(dynamic_cast<QFutureInterface<QVariant>*>(__qt_other0)){
        JavaException::raiseIllegalArgumentException(__jni_env, "QFutureInterface<T> not a valid argument for QFutureInterface<void>(QFutureInterface<void>)." QTJAMBI_STACKTRACEINFO );
    }else{
        JavaException::raiseIllegalArgumentException(__jni_env, "QFutureInterfaceBase not a valid argument for QFutureInterface<T>(QFutureInterface<void>)." QTJAMBI_STACKTRACEINFO );
    }
#else
    Q_ASSERT(__qt_other0);
    new(__qtjambi_ptr) QFutureInterface_shell<void>(*__qt_other0);
#endif
    Q_UNUSED(__jni_object)
#else
    Q_UNUSED(__jni_object)
    Q_UNUSED(__java_arguments)
    Q_UNUSED(__qtjambi_ptr)
    JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
}

// destruct QFutureInterface_shell
void __qt_destruct_QFutureInterface_QVariant_(void* ptr)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "destruct QFutureInterface<QVariant>")

#if QT_CONFIG(future)

    reinterpret_cast<QFutureInterface_shell<QVariant>*>(ptr)->~QFutureInterface_shell<QVariant>();

#else
    Q_UNUSED(ptr)

#endif // QT_CONFIG(future)

}

void __qt_destruct_QFutureInterface_void_(void* ptr)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "destruct QFutureInterface<void>")

#if QT_CONFIG(future)

    reinterpret_cast<QFutureInterface_shell<void>*>(ptr)->~QFutureInterface_shell<void>();

#else
    Q_UNUSED(ptr)

#endif // QT_CONFIG(future)

}

// emitting  (functionsInTargetLang writeFinalFunction)
// QFutureInterface<T>::QFutureInterface(QFutureInterfaceBase::State initialState)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface_initialize_1native__Lio_qt_core_QFutureInterface_2Lio_qt_core_QFutureInterfaceBase_00024State_2Z)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject initialState0,
 jboolean isVoid)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterface<QVariant>::QFutureInterface(QFutureInterfaceBase::State initialState)")
    try{
#if QT_CONFIG(future)
        jvalue arguments;
        arguments.l = initialState0;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object,
                                        isVoid ? &__qt_construct_QFutureInterface_void__1
                                               : &__qt_construct_QFutureInterface_QVariant__1,
                                        isVoid ? sizeof(QFutureInterface_shell<void>)
                                               : sizeof(QFutureInterface_shell<QVariant>),
                                        isVoid ? typeid(QFutureInterface<void>)
                                               : typeid(QFutureInterface<QVariant>), true, &arguments);
#else
        Q_UNUSED(__jni_object)
        Q_UNUSED(__jni_class)
        Q_UNUSED(initialState0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

// QFutureInterface<T>::QFutureInterface(const QFutureInterface<T> & other)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface_initialize_1native__Lio_qt_core_QFutureInterface_2Lio_qt_core_QFutureInterfaceBase_2)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject other0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterface<T>::QFutureInterface(const QFutureInterface<T> & other)")
    try{
#if QT_CONFIG(future)
        QFutureInterfaceBase* __qt_other0 = qtjambi_cast<QFutureInterfaceBase*>(__jni_env, other0);
        qtjambi_check_resource(__jni_env, __qt_other0);
        bool isVoid = !dynamic_cast<QFutureInterface<QVariant>*>(__qt_other0)
                   && !dynamic_cast<QFutureInterface_shell<QVariant>*>(__qt_other0);
        jvalue arguments;
        arguments.l = other0;
        qtjambi_initialize_native_value(__jni_env, __jni_class, __jni_object,
                                        isVoid ? &__qt_construct_QFutureInterface_void__2
                                               : &__qt_construct_QFutureInterface_QVariant__2,
                                        isVoid ? sizeof(QFutureInterface_shell<void>)
                                               : sizeof(QFutureInterface_shell<QVariant>),
                                        isVoid ? typeid(QFutureInterface<void>)
                                               : typeid(QFutureInterface<QVariant>), true, &arguments);
#else
        Q_UNUSED(__jni_object)
        Q_UNUSED(__jni_class)
        Q_UNUSED(other0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

// QFutureInterface<QVariant>::reportFinished(const T * result)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface_reportFinished__JLjava_lang_Object_2)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject result0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterface<QVariant>::reportFinished(const T * result)")
    try{
#if QT_CONFIG(future)
        QFutureInterfaceBase *__qt_this = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(QFutureInterface<QVariant>* _this = dynamic_cast<QFutureInterface<QVariant>*>(__qt_this)){
            QVariant __tmp_result0 = qtjambi_cast<QVariant>(__jni_env, result0);
            QVariant* __qt_result0 = result0 ? &__tmp_result0 : nullptr;
            _this->reportFinished(__qt_result0);
        }else if(dynamic_cast<const QFutureInterface<void>*>(__qt_this)){
            JavaException::raiseQNoImplementationException(__jni_env, "reportFinished() not available for QFutureInterface<void>." QTJAMBI_STACKTRACEINFO );
        }else{
            JavaException::raiseQNoImplementationException(__jni_env, "reportFinished() not available for QFutureInterfaceBase." QTJAMBI_STACKTRACEINFO );
        }
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(result0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

// QFutureInterface<QVariant>::reportResult(const QVariant & result, int index)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface_reportResult__JLjava_lang_Object_2I)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jobject result0,
 jint index1)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterface<QVariant>::reportResult(const QVariant & result, int index)")
    try{
#if QT_CONFIG(future)
        QFutureInterfaceBase *__qt_this = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(QFutureInterface<QVariant>* _this = dynamic_cast<QFutureInterface<QVariant>*>(__qt_this)){
            const QVariant& __qt_result0 = qtjambi_cast<QVariant>(__jni_env, result0);
            _this->reportResult(__qt_result0, static_cast<int>(index1));
        }else if(dynamic_cast<const QFutureInterface<void>*>(__qt_this)){
            JavaException::raiseQNoImplementationException(__jni_env, "reportResult(T, int) not available for QFutureInterface<void>." QTJAMBI_STACKTRACEINFO );
        }else{
            JavaException::raiseQNoImplementationException(__jni_env, "reportResult(T, int) not available for QFutureInterfaceBase." QTJAMBI_STACKTRACEINFO );
        }
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(result0)
        Q_UNUSED(index1)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

// QFutureInterface<QVariant>::reportResults(const QVector<QVariant> & results, int beginIndex, int count)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface_reportResults__JLjava_util_Collection_2II)
(JNIEnv *__jni_env,
 jobject,
 QtJambiNativeID __this_nativeId,
 jobject results0,
 jint beginIndex1,
 jint count2)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterface<QVariant>::reportResults(const QVector<QVariant> & results, int beginIndex, int count)")
    QtJambiScope __qtjambi_scope(__this_nativeId);
    Q_UNUSED(__qtjambi_scope)
    try{
#if QT_CONFIG(future)
        QFutureInterfaceBase *__qt_this = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(QFutureInterface<QVariant>* _this = dynamic_cast<QFutureInterface<QVariant>*>(__qt_this)){
            const QVector<QVariant>&  __qt_results0 = qtjambi_cast<const QVector<QVariant>& >(__jni_env, __qtjambi_scope, results0);
            _this->reportResults(__qt_results0, static_cast<int>(beginIndex1), static_cast<int>(count2));
        }else if(dynamic_cast<const QFutureInterface<void>*>(__qt_this)){
            JavaException::raiseQNoImplementationException(__jni_env, "reportResults(List<T>, int, int) not available for QFutureInterface<void>." QTJAMBI_STACKTRACEINFO );
        }else{
            JavaException::raiseQNoImplementationException(__jni_env, "reportResults(List<T>, int, int) not available for QFutureInterfaceBase." QTJAMBI_STACKTRACEINFO );
        }
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(results0)
        Q_UNUSED(beginIndex1)
        Q_UNUSED(count2)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

// QFutureInterface<QVariant>::resultReference(int index) const
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface_result__JI)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 jint index0)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterface<QVariant>::resultReference(int index) const")
    try{
#if QT_CONFIG(future)
        const QFutureInterfaceBase *__qt_this = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(const QFutureInterface<QVariant>* _this = dynamic_cast<const QFutureInterface<QVariant>*>(__qt_this)){
            if(index0<0 || index0>=_this->resultCount())
                JavaException::raiseIndexOutOfBoundsException(__jni_env, qPrintable(QString::number(index0)) QTJAMBI_STACKTRACEINFO );
            const QVariant& __qt_return_value = _this->resultReference(static_cast<int>(index0));
            return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
        }else if(dynamic_cast<const QFutureInterface<void>*>(__qt_this)){
            JavaException::raiseQNoImplementationException(__jni_env, "result(int) not available for QFutureInterface<void>." QTJAMBI_STACKTRACEINFO );
        }else{
            JavaException::raiseQNoImplementationException(__jni_env, "result(int) not available for QFutureInterfaceBase." QTJAMBI_STACKTRACEINFO );
        }
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(index0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QFutureInterface<QVariant>::results()
extern "C" Q_DECL_EXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureInterface_results__J)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureInterface<QVariant>::results()")
    try{
#if QT_CONFIG(future)
        QFutureInterfaceBase *__qt_this = qtjambi_object_from_nativeId<QFutureInterfaceBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        if(QFutureInterface<QVariant>* _this = dynamic_cast<QFutureInterface<QVariant>*>(__qt_this)){
            QList<QVariant> __qt_return_value = _this->results();
            return qtjambi_cast<jobject>(__jni_env, __qt_return_value);
        }else if(dynamic_cast<const QFutureInterface<void>*>(__qt_this)){
            JavaException::raiseQNoImplementationException(__jni_env, "results() not available for QFutureInterface<void>." QTJAMBI_STACKTRACEINFO );
        }else{
            JavaException::raiseQNoImplementationException(__jni_env, "results() not available for QFutureInterfaceBase." QTJAMBI_STACKTRACEINFO );
        }
#else
        Q_UNUSED(__this_nativeId)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return nullptr;

}

// QFutureWatcher:

class QFutureWatcher_shell : public QFutureWatcher<QVariant>, public QtJambiShellInterface
{
public:
    QFutureWatcher_shell(QObject* parent = nullptr);
    ~QFutureWatcher_shell() override;

    static void operator delete(void * ptr) noexcept;
private:
    QtJambiShell* __shell() const override final;
    jmethodID __shell_javaMethod(int pos) const;
public:
    bool m_isVoid;
    static const QMetaObject& staticMetaObject;
};

const QMetaObject& QFutureWatcher_shell::staticMetaObject = []()->const QMetaObject&{
    QMetaObjectBuilder builder;
    builder.setClassName("QFutureWatcher<QVariant>");
    builder.setSuperClass(&QFutureWatcherBase::staticMetaObject);
    //builder.setFlags();
    return *builder.toMetaObject();
}();

QtJambiShell* QFutureWatcher_shell::__shell() const { return *reinterpret_cast<QtJambiShell**>( quintptr(this) + sizeof(QFutureWatcher_shell) ); }

jmethodID QFutureWatcher_shell::__shell_javaMethod(int pos) const { return QFutureWatcher_shell::__shell()->javaMethod(typeid(QFutureWatcher<QVariant>), pos); }

// emitting (writeShellConstructor)
QFutureWatcher_shell::QFutureWatcher_shell(QObject* parent)
    : QFutureWatcher<QVariant>(parent), m_isVoid(false)
{
    QTJAMBI_DEBUG_METHOD_PRINT_WHERE("shell", "QFutureWatcher_shell::QFutureWatcher_shell(QFutureInterfaceBase::State initialState0)", QFutureWatcher_shell::__shell())
    QFutureWatcher_shell::__shell()->constructed(typeid(QFutureWatcher<QVariant>));
}

// emitting (writeShellDestructor)
QFutureWatcher_shell::~QFutureWatcher_shell()
{
    QTJAMBI_DEBUG_METHOD_PRINT_WHERE("shell", "QFutureWatcher_shell::~QFutureWatcher_shell()", QFutureWatcher_shell::__shell())
    QFutureWatcher_shell::__shell()->destructed(typeid(QFutureWatcher<QVariant>));
}

void QFutureWatcher_shell::operator delete(void * ptr) noexcept {
    reinterpret_cast<QFutureWatcher_shell*>(ptr)->QFutureWatcher_shell::__shell()->deleteShell();
}

// new QFutureWatcher(QObject*)
void __qt_construct_QFutureWatcher_1(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject, jvalue* __java_arguments)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QFutureWatcher(QObject*)")
#if QT_CONFIG(future)
    jobject parent0 = __java_arguments[0].l;
    QObject* __qt_parent0 = qtjambi_cast<QObject*>(__jni_env, parent0);
    jlongArray functions = jlongArray(__java_arguments[1].l);
    QFutureWatcher<QVariant> *__qt_this = new(__qtjambi_ptr) QFutureWatcher_shell(__qt_parent0);
    FutureSetter futureSetter = [](JNIEnv * env, QFutureWatcherBase* base, jobject future){
                 QFutureWatcher_shell* watcher = dynamic_cast<QFutureWatcher_shell*>(base);
                 jobject jfi = qtjambi_get_QFutureInterface(env, future);
                 QFutureInterfaceBase* fibase = qtjambi_cast<QFutureInterfaceBase*>(env, jfi);
                 const std::type_info& fibaseType = typeid(*fibase);
                 if(dynamic_cast<QFutureInterface<void>*>(fibase)
                         || fibaseType==typeid(QFutureInterfaceBase)
                         || fibaseType==typeid_QFutureInterfaceBase_shell()){
                     watcher->m_isVoid = true;
                     reinterpret_cast<QFutureWatcher<void>*>(base)->setFuture(QFuture<void>(fibase));
                 }else{
                     watcher->m_isVoid = false;
                     watcher->setFuture(qtjambi_cast<QFuture<QVariant>>(env, future));
                 }
            };
    FutureResult futureResult = [](JNIEnv * env, QFutureWatcherBase* base, int index) -> jobject{
                 QFutureWatcher_shell* watcher = dynamic_cast<QFutureWatcher_shell*>(base);
                 if(watcher->m_isVoid){
                    JavaException::raiseQNoImplementationException(env, "resultAt(int) not available for QFutureWatcher<void>." QTJAMBI_STACKTRACEINFO );
                 }
                 return qtjambi_from_qvariant(env, watcher->resultAt(index));
            };
    FutureGetter futureGetter = [](JNIEnv * env, QFutureWatcherBase* base) -> jobject {
                 QFutureWatcher_shell* watcher = dynamic_cast<QFutureWatcher_shell*>(base);
                 QFuture<QVariant> ft = watcher->future();
                 if(watcher->m_isVoid){
                    return qtjambi_cast<jobject>(env, QFuture<void>(ft));
                 }else{
                    return qtjambi_cast<jobject>(env, ft);
                 }
            };
    jlong* array = __jni_env->GetLongArrayElements(functions, nullptr);
    Q_ASSERT(__jni_env->GetArrayLength(functions)>2);
    array[0] = jlong(futureSetter);
    array[1] = jlong(futureResult);
    array[2] = jlong(futureGetter);
    __jni_env->ReleaseLongArrayElements(functions, array, 0);
    Q_UNUSED(__qt_this)
#else
    Q_UNUSED(__jni_object)
    Q_UNUSED(__java_arguments)
    Q_UNUSED(__qtjambi_ptr)
    JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
}

// destruct QFutureInterface_shell
void __qt_destruct_QFutureWatcher(void* ptr)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "destruct QFutureWatcher")
#if QT_CONFIG(future)
    reinterpret_cast<QFutureWatcher_shell*>(ptr)->~QFutureWatcher_shell();
#else
    Q_UNUSED(ptr)
#endif // QT_CONFIG(future)
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureWatcher_initialize_1native__Lio_qt_core_QFutureWatcher_2Lio_qt_core_QObject_2_3J)
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject parent0,
 jlongArray functions)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFutureWatcher::QFutureWatcher(QObject * parent)")
    try{
#if QT_CONFIG(future)
        jvalue arguments[2];
        arguments[0].l = parent0;
        arguments[1].l = functions;
        qtjambi_initialize_native_qobject(__jni_env, __jni_class, __jni_object, &__qt_construct_QFutureWatcher_1, sizeof(QFutureWatcher_shell), typeid(QFutureWatcher<QVariant>), QFutureWatcher_shell::staticMetaObject, true, false, arguments);
#else
        Q_UNUSED(__jni_object)
        Q_UNUSED(__jni_class)
        Q_UNUSED(parent0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }

}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureWatcher_future)
(JNIEnv *__jni_env, jclass, QtJambiNativeID __this_nativeId, jlong _futureGetter)
{
    try{
        QFutureWatcherBase *__qt_this = qtjambi_object_from_nativeId<QFutureWatcherBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        FutureGetter futureGetter = FutureGetter(_futureGetter);
        return futureGetter(__jni_env, __qt_this);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureWatcher_resultAt)
(JNIEnv *__jni_env, jclass, QtJambiNativeID __this_nativeId, jlong _futureResult, jint index)
{
    try{
        QFutureWatcherBase *__qt_this = qtjambi_object_from_nativeId<QFutureWatcherBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        FutureResult futureResult = FutureResult(_futureResult);
        return futureResult(__jni_env, __qt_this, index);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
        return nullptr;
    }
}

extern "C" JNIEXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_core_QFutureWatcher_setFuture)
(JNIEnv *__jni_env, jclass, QtJambiNativeID __this_nativeId, jlong _futureSetter, jobject future)
{
    try{
        QFutureWatcherBase *__qt_this = qtjambi_object_from_nativeId<QFutureWatcherBase>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        FutureSetter futureSetter = FutureSetter(_futureSetter);
        futureSetter(__jni_env, __qt_this, future);
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// emitting Field accessors (writeFieldAccessors)
// emitting (writeCustomStructors)
// emitting (writeJavaLangObjectOverrideFunctions)
void initialize_meta_info_QFutureInterface(){

#if QT_CONFIG(future)
    {
        const std::type_info& typeId = registerValueTypeInfo<QFutureInterface<QVariant>>("QFutureInterface<QVariant>", "io/qt/core/QFutureInterface");
        registerConstructorInfos(typeId, &__qt_destruct_QFutureInterface_QVariant_, {
            ConstructorInfo(&__qt_construct_QFutureInterface_QVariant__0, nullptr)
           ,ConstructorInfo(&__qt_construct_QFutureInterface_QVariant__1, "Lio/qt/core/QFutureInterfaceBase$State;")
           ,ConstructorInfo(&__qt_construct_QFutureInterface_QVariant__2, "Lio/qt/core/QFutureInterface;")
        });
        registerMetaType<QFutureInterface<QVariant>>("QFutureInterface<QVariant>");
        registerSizeOfShell(typeId, sizeof(QFutureInterface_shell<QVariant>));
    }
    {
        const std::type_info& typeId = registerValueTypeInfo<QFutureInterface<void>>("QFutureInterface<void>", "io/qt/core/QFutureInterface");
        registerConstructorInfos(typeId, &__qt_destruct_QFutureInterface_void_, {
            ConstructorInfo(&__qt_construct_QFutureInterface_void__0, nullptr)
           ,ConstructorInfo(&__qt_construct_QFutureInterface_void__1, "Lio/qt/core/QFutureInterfaceBase$State;")
           ,ConstructorInfo(&__qt_construct_QFutureInterface_void__2, "Lio/qt/core/QFutureInterface;")
        });
        registerMetaType<QFutureInterface<void>>("QFutureInterface<void>");
        registerSizeOfShell(typeId, sizeof(QFutureInterface_shell<void>));
    }
    {
        const std::type_info& typeId = registerQObjectTypeInfo<QFutureWatcher<QVariant>>("QFutureWatcher<QVariant>", "io/qt/core/QFutureWatcher");
        registerFunctionInfos(typeId, {});
        registerMetaObject(typeId, QFutureWatcher_shell::staticMetaObject, false, nullptr);
    }
#endif //QT_CONFIG(future)

}
