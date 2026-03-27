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

#include "util_p.h"
#include <QtCore/QtGlobal>
#include <QtTaskTree/QTaskTree>
#include <QtTaskTree/QBarrier>
#include <QtTaskTree/QThreadFunction>
#include <QtTaskTree/qprocesstask.h>
#include <QtCore/QList>
#include <QtCore/QVariant>

#include <QtJambi/QtJambiAPI>
#include <QtJambi/JObjectWrapper>

#include <QtJambi/RegistryAPI>
#include <QtJambi/Cast>

jobject convertTaskInterface(JNIEnv* env, QtTaskTree::QTaskInterface *iface){
    return qtjambi_cast<jobject>(env, iface);
}

void __qt_destruct_QtTaskTree_QCustomTask_JObjectWrapper_(void* ptr)
{
    QTJAMBI_NATIVE_METHOD_CALL("destruct QtTaskTree::QCustomTask<JObjectWrapper>")
    typedef QtTaskTree::QCustomTask<JObjectWrapper> DESTRUCTOR;
    reinterpret_cast<QtTaskTree::QCustomTask<JObjectWrapper>*>(ptr)->~DESTRUCTOR();
}

void deleter_QtTaskTree_QCustomTask_JObjectWrapper_(void *ptr, bool isShell)
{
    QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QtTaskTree::QCustomTask<JObjectWrapper>")
    QtTaskTree::QCustomTask<JObjectWrapper> *_ptr = reinterpret_cast<QtTaskTree::QCustomTask<JObjectWrapper> *>(ptr);
    if(!isShell){
        QtJambiAPI::registerNonShellDeletion(ptr);
    }
    delete _ptr;
}

// emitting (writeConstructors)

// new QtTaskTree::QCustomTask<JObjectWrapper>(SetupHandler &&,DoneHandler &&,CallDone)
void __qt_construct_QCustomTask(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QCustomTask<JObjectWrapper>(QtTaskTree::QSyncBoolSupplier&&)")
    jobject setup0 = __java_arguments[0].l;
    jobject done1 = __java_arguments[1].l;
    jobject callDone2 = __java_arguments[2].l;
    jobject taskFactory = __java_arguments[3].l;
    jobject adapterFactory = __java_arguments[4].l;
    QtTaskTree::QCustomTask<JObjectWrapper>::TaskSetupHandler __qt_setup0;
    if(setup0){
        __qt_setup0 = [wrapper = JObjectWrapper(__jni_env, setup0)](JObjectWrapper& arg) -> QtTaskTree::SetupResult {
            QtTaskTree::SetupResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject value = Java::Runtime::Function::apply(env, wrapper.object(env), arg.object(env));
                    result = qtjambi_cast<QtTaskTree::SetupResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QCustomTask<JObjectWrapper>::TaskDoneHandler __qt_done1;
    if(done1){
        __qt_done1 = [wrapper = JObjectWrapper(__jni_env, done1)](const JObjectWrapper& arg, QtTaskTree::DoneWith arg2) -> QtTaskTree::DoneResult {
            QtTaskTree::DoneResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject value = Java::Runtime::BiFunction::apply(env, wrapper.object(env), arg.object(env), qtjambi_cast<jobject>(env, arg2));
                    result = qtjambi_cast<QtTaskTree::DoneResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QCustomTask<JObjectWrapper> *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::QCustomTask<JObjectWrapper>(__jni_env, taskFactory, adapterFactory, std::move(__qt_setup0), std::move(__qt_done1), qtjambi_cast<QtTaskTree::CallDone>(__jni_env, callDone2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::QCustomTask<JObjectWrapper>(SetupHandler &&,DoneHandler &&,CallDone)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QCustomTask_initialize_1native_1by_1factories
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject setup0,
     jobject done1,
     jobject callDone2,
     jobject taskFactory,
     jobject adapterFactory)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::QCustomTask<JObjectWrapper>::QCustomTask<JObjectWrapper>(QtTaskTree::QSyncBoolSupplier&&)")
    QTJAMBI_TRY {
        jvalue arguments[5];
        arguments[0].l = setup0;
        arguments[1].l = done1;
        arguments[2].l = callDone2;
        arguments[3].l = taskFactory;
        arguments[4].l = adapterFactory;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QCustomTask, sizeof(QtTaskTree::QCustomTask<JObjectWrapper>), alignof(QtTaskTree::QCustomTask<JObjectWrapper>), typeid(QtTaskTree::QCustomTask<JObjectWrapper>), 0, false, &deleter_QtTaskTree_QCustomTask_JObjectWrapper_, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void deleter_QTimeoutTask(void *ptr, bool isShell)
{
    QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QtTaskTree::QCustomTask<JObjectWrapper>")
    QtTaskTree::QTimeoutTask *_ptr = reinterpret_cast<QtTaskTree::QTimeoutTask *>(ptr);
    if(!isShell){
        QtJambiAPI::registerNonShellDeletion(ptr);
    }
    delete _ptr;
}

// new QtTaskTree::QTimeoutTask(SetupHandler &&,DoneHandler &&,CallDone)
void __qt_construct_QTimeoutTask(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QTimeoutTask(QtTaskTree::QSyncBoolSupplier&&)")
    jobject setup0 = __java_arguments[0].l;
    jobject done1 = __java_arguments[1].l;
    jobject callDone2 = __java_arguments[2].l;
    QtTaskTree::QTimeoutTask::TaskSetupHandler __qt_setup0;
    if(setup0){
        __qt_setup0 = [wrapper = JObjectWrapper(__jni_env, setup0)](std::chrono::milliseconds& arg) -> QtTaskTree::SetupResult {
            QtTaskTree::SetupResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject _arg = Java::QtTaskTree::Timeout::newInstance(env, jlong(&arg), true);
                    auto guard = qScopeGuard([&](){Java::QtTaskTree::Timeout::set___qt_directLink(env, _arg, 0);});
                    jobject value = Java::Runtime::Function::apply(env, wrapper.object(env), _arg);
                    result = qtjambi_cast<QtTaskTree::SetupResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QTimeoutTask::TaskDoneHandler __qt_done1;
    if(done1){
        __qt_done1 = [wrapper = JObjectWrapper(__jni_env, done1)](const std::chrono::milliseconds& arg, QtTaskTree::DoneWith arg2) -> QtTaskTree::DoneResult {
            QtTaskTree::DoneResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject _arg = Java::QtTaskTree::Timeout::newInstance(env, jlong(&arg), false);
                    auto guard = qScopeGuard([&](){Java::QtTaskTree::Timeout::set___qt_directLink(env, _arg, 0);});
                    jobject value = Java::Runtime::BiFunction::apply(env, wrapper.object(env), _arg, qtjambi_cast<jobject>(env, arg2));
                    result = qtjambi_cast<QtTaskTree::DoneResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QTimeoutTask *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::QTimeoutTask(std::move(__qt_setup0), std::move(__qt_done1), qtjambi_cast<QtTaskTree::CallDone>(__jni_env, callDone2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::QCustomTask<JObjectWrapper>(SetupHandler &&,DoneHandler &&,CallDone)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QCustomTask_initialize_1native_1QTimeoutTask
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject setup0,
     jobject done1,
     jobject callDone2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::QCustomTask<JObjectWrapper>::QCustomTask<JObjectWrapper>(QtTaskTree::QSyncBoolSupplier&&)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = setup0;
        arguments[1].l = done1;
        arguments[2].l = callDone2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QTimeoutTask, sizeof(QtTaskTree::QTimeoutTask), alignof(QtTaskTree::QCustomTask<JObjectWrapper>), typeid(QtTaskTree::QCustomTask<JObjectWrapper>), 0, false, &deleter_QTimeoutTask, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void deleter_QBarrierTask(void *ptr, bool isShell)
{
    QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QtTaskTree::QCustomTask<JObjectWrapper>")
    QtTaskTree::QBarrierTask *_ptr = reinterpret_cast<QtTaskTree::QBarrierTask *>(ptr);
    if(!isShell){
        QtJambiAPI::registerNonShellDeletion(ptr);
    }
    delete _ptr;
}

// new QtTaskTree::QBarrierTask(SetupHandler &&,DoneHandler &&,CallDone)
void __qt_construct_QBarrierTask(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QBarrierTask(QtTaskTree::QSyncBoolSupplier&&)")
    jobject setup0 = __java_arguments[0].l;
    jobject done1 = __java_arguments[1].l;
    jobject callDone2 = __java_arguments[2].l;
    QtTaskTree::QBarrierTask::TaskSetupHandler __qt_setup0;
    if(setup0){
        __qt_setup0 = [wrapper = JObjectWrapper(__jni_env, setup0)](QtTaskTree::QBarrier& arg) -> QtTaskTree::SetupResult {
            QtTaskTree::SetupResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject value = Java::Runtime::Function::apply(env, wrapper.object(env), qtjambi_cast<jobject>(env, arg));
                    result = qtjambi_cast<QtTaskTree::SetupResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QBarrierTask::TaskDoneHandler __qt_done1;
    if(done1){
        __qt_done1 = [wrapper = JObjectWrapper(__jni_env, done1)](const QtTaskTree::QBarrier& arg, QtTaskTree::DoneWith arg2) -> QtTaskTree::DoneResult {
            QtTaskTree::DoneResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject value = Java::Runtime::BiFunction::apply(env, wrapper.object(env), qtjambi_cast<jobject>(env, arg), qtjambi_cast<jobject>(env, arg2));
                    result = qtjambi_cast<QtTaskTree::DoneResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QBarrierTask *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::QBarrierTask(std::move(__qt_setup0), std::move(__qt_done1), qtjambi_cast<QtTaskTree::CallDone>(__jni_env, callDone2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::QCustomTask<JObjectWrapper>(SetupHandler &&,DoneHandler &&,CallDone)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QCustomTask_initialize_1native_1QBarrierTask
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject setup0,
     jobject done1,
     jobject callDone2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::QCustomTask<JObjectWrapper>::QCustomTask<JObjectWrapper>(QtTaskTree::QSyncBoolSupplier&&)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = setup0;
        arguments[1].l = done1;
        arguments[2].l = callDone2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QBarrierTask, sizeof(QtTaskTree::QBarrierTask), alignof(QtTaskTree::QCustomTask<JObjectWrapper>), typeid(QtTaskTree::QCustomTask<JObjectWrapper>), 0, false, &deleter_QBarrierTask, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void deleter_QTaskTreeTask(void *ptr, bool isShell)
{
    QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QtTaskTree::QCustomTask<JObjectWrapper>")
    QtTaskTree::QTaskTreeTask *_ptr = reinterpret_cast<QtTaskTree::QTaskTreeTask *>(ptr);
    if(!isShell){
        QtJambiAPI::registerNonShellDeletion(ptr);
    }
    delete _ptr;
}

// new QtTaskTree::QTaskTreeTask(SetupHandler &&,DoneHandler &&,CallDone)
void __qt_construct_QTaskTreeTask(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QTaskTreeTask(QtTaskTree::QSyncBoolSupplier&&)")
    jobject setup0 = __java_arguments[0].l;
    jobject done1 = __java_arguments[1].l;
    jobject callDone2 = __java_arguments[2].l;
    QtTaskTree::QTaskTreeTask::TaskSetupHandler __qt_setup0;
    if(setup0){
        __qt_setup0 = [wrapper = JObjectWrapper(__jni_env, setup0)](QtTaskTree::QTaskTree& arg) -> QtTaskTree::SetupResult {
            QtTaskTree::SetupResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject value = Java::Runtime::Function::apply(env, wrapper.object(env), qtjambi_cast<jobject>(env, arg));
                    result = qtjambi_cast<QtTaskTree::SetupResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QTaskTreeTask::TaskDoneHandler __qt_done1;
    if(done1){
        __qt_done1 = [wrapper = JObjectWrapper(__jni_env, done1)](const QtTaskTree::QTaskTree& arg, QtTaskTree::DoneWith arg2) -> QtTaskTree::DoneResult {
            QtTaskTree::DoneResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject value = Java::Runtime::BiFunction::apply(env, wrapper.object(env), qtjambi_cast<jobject>(env, arg), qtjambi_cast<jobject>(env, arg2));
                    result = qtjambi_cast<QtTaskTree::DoneResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QTaskTreeTask *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::QTaskTreeTask(std::move(__qt_setup0), std::move(__qt_done1), qtjambi_cast<QtTaskTree::CallDone>(__jni_env, callDone2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::QCustomTask<JObjectWrapper>(SetupHandler &&,DoneHandler &&,CallDone)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QCustomTask_initialize_1native_1QTaskTreeTask
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject setup0,
     jobject done1,
     jobject callDone2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::QCustomTask<JObjectWrapper>::QCustomTask<JObjectWrapper>(QtTaskTree::QSyncBoolSupplier&&)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = setup0;
        arguments[1].l = done1;
        arguments[2].l = callDone2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QTaskTreeTask, sizeof(QtTaskTree::QTaskTreeTask), alignof(QtTaskTree::QCustomTask<JObjectWrapper>), typeid(QtTaskTree::QCustomTask<JObjectWrapper>), 0, false, &deleter_QTaskTreeTask, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void deleter_QThreadFunctionTask(void *ptr, bool isShell)
{
    QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QtTaskTree::QCustomTask<JObjectWrapper>")
    QtTaskTree::QThreadFunctionTask<QVariant> *_ptr = reinterpret_cast<QtTaskTree::QThreadFunctionTask<QVariant> *>(ptr);
    if(!isShell){
        QtJambiAPI::registerNonShellDeletion(ptr);
    }
    delete _ptr;
}

// new QtTaskTree::QThreadFunctionTask(SetupHandler &&,DoneHandler &&,CallDone)
void __qt_construct_QThreadFunctionTask(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QThreadFunctionTask(QtTaskTree::QSyncBoolSupplier&&)")
    jobject setup0 = __java_arguments[0].l;
    jobject done1 = __java_arguments[1].l;
    jobject callDone2 = __java_arguments[2].l;
    QtTaskTree::QThreadFunctionTask<QVariant>::TaskSetupHandler __qt_setup0;
    if(setup0){
        __qt_setup0 = [wrapper = JObjectWrapper(__jni_env, setup0)](QtTaskTree::QThreadFunction<QVariant>& arg) -> QtTaskTree::SetupResult {
            QtTaskTree::SetupResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject _arg = qtjambi_cast<jobject>(env, arg);
                    QTJAMBI_INVALIDATE_AFTER_USE(env, _arg);
                    jobject value = Java::Runtime::Function::apply(env, wrapper.object(env), _arg);
                    result = qtjambi_cast<QtTaskTree::SetupResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QThreadFunctionTask<QVariant>::TaskDoneHandler __qt_done1;
    if(done1){
        __qt_done1 = [wrapper = JObjectWrapper(__jni_env, done1)](const QtTaskTree::QThreadFunction<QVariant>& arg, QtTaskTree::DoneWith arg2) -> QtTaskTree::DoneResult {
            QtTaskTree::DoneResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject _arg = qtjambi_cast<jobject>(env, arg);
                    QTJAMBI_INVALIDATE_AFTER_USE(env, _arg);
                    jobject value = Java::Runtime::BiFunction::apply(env, wrapper.object(env), _arg, qtjambi_cast<jobject>(env, arg2));
                    result = qtjambi_cast<QtTaskTree::DoneResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QThreadFunctionTask<QVariant> *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::QThreadFunctionTask<QVariant>(std::move(__qt_setup0), std::move(__qt_done1), qtjambi_cast<QtTaskTree::CallDone>(__jni_env, callDone2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::QCustomTask<JObjectWrapper>(SetupHandler &&,DoneHandler &&,CallDone)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QCustomTask_initialize_1native_1QThreadFunctionTask
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject setup0,
     jobject done1,
     jobject callDone2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::QCustomTask<JObjectWrapper>::QCustomTask<JObjectWrapper>(QtTaskTree::QSyncBoolSupplier&&)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = setup0;
        arguments[1].l = done1;
        arguments[2].l = callDone2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QThreadFunctionTask, sizeof(QtTaskTree::QThreadFunctionTask<QVariant>), alignof(QtTaskTree::QCustomTask<JObjectWrapper>), typeid(QtTaskTree::QCustomTask<JObjectWrapper>), 0, false, &deleter_QThreadFunctionTask, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

void deleter_QProcessTask(void *ptr, bool isShell)
{
    QTJAMBI_NATIVE_METHOD_CALL("qtjambi_deleter for QtTaskTree::QCustomTask<JObjectWrapper>")
    QtTaskTree::QProcessTask *_ptr = reinterpret_cast<QtTaskTree::QProcessTask *>(ptr);
    if(!isShell){
        QtJambiAPI::registerNonShellDeletion(ptr);
    }
    delete _ptr;
}

// new QtTaskTree::QProcessTask(SetupHandler &&,DoneHandler &&,CallDone)
void __qt_construct_QProcessTask(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QProcessTask(QtTaskTree::QSyncBoolSupplier&&)")
    jobject setup0 = __java_arguments[0].l;
    jobject done1 = __java_arguments[1].l;
    jobject callDone2 = __java_arguments[2].l;
    QtTaskTree::QProcessTask::TaskSetupHandler __qt_setup0;
    if(setup0){
        __qt_setup0 = [wrapper = JObjectWrapper(__jni_env, setup0)](QProcess& arg) -> QtTaskTree::SetupResult {
            QtTaskTree::SetupResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject value = Java::Runtime::Function::apply(env, wrapper.object(env), qtjambi_cast<jobject>(env, arg));
                    result = qtjambi_cast<QtTaskTree::SetupResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QProcessTask::TaskDoneHandler __qt_done1;
    if(done1){
        __qt_done1 = [wrapper = JObjectWrapper(__jni_env, done1)](const QProcess& arg, QtTaskTree::DoneWith arg2) -> QtTaskTree::DoneResult {
            QtTaskTree::DoneResult result{};
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject value = Java::Runtime::BiFunction::apply(env, wrapper.object(env), qtjambi_cast<jobject>(env, arg), qtjambi_cast<jobject>(env, arg2));
                    result = qtjambi_cast<QtTaskTree::DoneResult>(env, value);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
            return result;
        };
    }
    QtTaskTree::QProcessTask *__qt_this;
    __qt_this = new(__qtjambi_ptr) QtTaskTree::QProcessTask(std::move(__qt_setup0), std::move(__qt_done1), qtjambi_cast<QtTaskTree::CallDone>(__jni_env, callDone2));
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__qtjambi_constructor_options)
}

// QtTaskTree::QCustomTask<JObjectWrapper>(SetupHandler &&,DoneHandler &&,CallDone)
extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QCustomTask_initialize_1native_1QProcessTask
    (JNIEnv *__jni_env,
     jclass __jni_class,
     jobject __jni_object,
     jobject setup0,
     jobject done1,
     jobject callDone2)
{
    QTJAMBI_NATIVE_METHOD_CALL("QtTaskTree::QCustomTask<JObjectWrapper>::QCustomTask<JObjectWrapper>(QtTaskTree::QSyncBoolSupplier&&)")
    QTJAMBI_TRY {
        jvalue arguments[3];
        arguments[0].l = setup0;
        arguments[1].l = done1;
        arguments[2].l = callDone2;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QProcessTask, sizeof(QtTaskTree::QProcessTask), alignof(QtTaskTree::QCustomTask<JObjectWrapper>), typeid(QtTaskTree::QCustomTask<JObjectWrapper>), 0, false, &deleter_QProcessTask, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// emitting  (functionsInTargetLang writeFinalFunction)
// emitting (writeJavaLangObjectOverrideFunctions)
// emitting (writeCloneFunction)

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_tasktree_QCustomTask_clone_1native
    (JNIEnv *__jni_env, jobject, QtJambiNativeID __this_nativeId)
{
    jobject __java_return_value = nullptr;
    QTJAMBI_TRY {
        const QtTaskTree::QCustomTask<JObjectWrapper> *__qt_this = QtJambiAPI::objectFromNativeId<QtTaskTree::QCustomTask<JObjectWrapper>>(__this_nativeId);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        __java_return_value = qtjambi_cast<jobject>(__jni_env, *__qt_this);
    } QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    } QTJAMBI_TRY_END
    return __java_return_value;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_tasktree_Timeout_getTimeout(JNIEnv *__jni_env, jclass, jlong __qt_directLink){
    jobject __java_return_value = nullptr;
    QTJAMBI_TRY {
        Q_ASSERT(__qt_directLink);
        __java_return_value = QtJambiAPI::convertDuration(__jni_env, *reinterpret_cast<const std::chrono::milliseconds*>(__qt_directLink));
    } QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    } QTJAMBI_TRY_END
        return __java_return_value;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_Timeout_setTimeout(JNIEnv *__jni_env, jclass, jlong __qt_directLink, jobject time){
    QTJAMBI_TRY {
        Q_ASSERT(__qt_directLink);
        *reinterpret_cast<std::chrono::milliseconds*>(__qt_directLink) = QtJambiAPI::convertDuration(__jni_env, time, std::chrono::milliseconds{});
    } QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    } QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QTaskTree_onStorageSetup(JNIEnv *env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID storage, jobject handler){
    QTJAMBI_TRY {
        QtTaskTree::QTaskTree *__qt_this = QtJambiAPI::objectFromNativeId<QtTaskTree::QTaskTree>(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, __qt_this);
        const QtTaskTree::Storage<QVariant> &__qt_storage = QtJambiAPI::objectReferenceFromNativeId<QtTaskTree::Storage<QVariant>>(env, storage);
        __qt_this->onStorageSetup(__qt_storage, [structType = __qt_storage.structType(), handler = JObjectWrapper(env, handler)](QVariant& storageStruct){
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject variant = QtJambiAPI::convertQVariantToJavaVariant(env, storageStruct);
                    InvalidateAfterUse invalidate(env, variant);
                    jobject activeStorage = Java::QtTaskTree::Storage$ActiveStorage::newInstance(env, structType.object(env), variant);
                    Java::Runtime::Consumer::accept(env, activeStorage);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
        });
    } QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    } QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QTaskTree_onStorageDone(JNIEnv *env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID storage, jobject handler){
    QTJAMBI_TRY {
        QtTaskTree::QTaskTree *__qt_this = QtJambiAPI::objectFromNativeId<QtTaskTree::QTaskTree>(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, __qt_this);
        const QtTaskTree::Storage<QVariant> &__qt_storage = QtJambiAPI::objectReferenceFromNativeId<QtTaskTree::Storage<QVariant>>(env, storage);
        __qt_this->onStorageDone(__qt_storage, [handler = JObjectWrapper(env, handler)](const QVariant& storageStruct){
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    jobject activeStorage = QtJambiAPI::convertQVariantToJavaObject(env, storageStruct);
                    Java::Runtime::Consumer::accept(env, activeStorage);
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
        });
    } QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    } QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QTaskTree_onStorageSetupBarrier(JNIEnv *env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID storage, jobject handler){
    QTJAMBI_TRY {
        QtTaskTree::QTaskTree *__qt_this = QtJambiAPI::objectFromNativeId<QtTaskTree::QTaskTree>(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, __qt_this);
        const QtTaskTree::QStoredBarrier &__qt_storage = QtJambiAPI::objectReferenceFromNativeId<QtTaskTree::QStoredBarrier>(env, storage);
        __qt_this->onStorageSetup(__qt_storage, [handler = JObjectWrapper(env, handler)](QtTaskTree::QStartedBarrier& storageStruct){
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    Java::Runtime::Consumer::accept(env, qtjambi_cast<jobject>(env, storageStruct));
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
        });
    } QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    } QTJAMBI_TRY_END
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_tasktree_QTaskTree_onStorageDoneBarrier(JNIEnv *env, jclass, QtJambiNativeID __this_nativeId, QtJambiNativeID storage, jobject handler){
    QTJAMBI_TRY {
        QtTaskTree::QTaskTree *__qt_this = QtJambiAPI::objectFromNativeId<QtTaskTree::QTaskTree>(__this_nativeId);
        QtJambiAPI::checkNullPointer(env, __qt_this);
        const QtTaskTree::QStoredBarrier &__qt_storage = QtJambiAPI::objectReferenceFromNativeId<QtTaskTree::QStoredBarrier>(env, storage);
        __qt_this->onStorageDone(__qt_storage, [handler = JObjectWrapper(env, handler)](const QtTaskTree::QStartedBarrier& storageStruct){
            if(JniEnvironment env{200}){
                QTJAMBI_TRY{
                    Java::Runtime::Consumer::accept(env, qtjambi_cast<jobject>(env, storageStruct));
                }QTJAMBI_CATCH(const JavaException& exn){
                    exn.report(env);
                }QTJAMBI_TRY_END
            }
        });
    } QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    } QTJAMBI_TRY_END
}

QtTaskTree::Storage<QVariant>::Storage(JNIEnv* env, jclass structType, jobject supplier)
    : StorageBase(Storage::ctor(env, supplier), Storage::dtor()),
    m_structType(env, structType)
{}
QVariant *QtTaskTree::Storage<QVariant>::activeStorage() const {
    return static_cast<QVariant *>(activeStorageVoid());
}
const JObjectWrapper& QtTaskTree::Storage<QVariant>::structType() const{
    return m_structType;
}
auto QtTaskTree::Storage<QVariant>::ctor(JNIEnv* env, jobject supplier) -> StorageConstructor {
    return [supplier = JObjectWrapper(env, supplier)] {
        if(JniEnvironment env{200}){
            QTJAMBI_TRY{
                jobject value = Java::Runtime::Supplier::get(env, supplier.object(env));
                QVariant variant = QtJambiAPI::convertJavaObjectToQVariant(env, value);
                if(variant.metaType().flags() & QMetaType::IsPointer){
                    Java::Runtime::RuntimeException::throwNew(env, QStringLiteral("Unable to use %1 as storage.").arg(variant.metaType().name()) QTJAMBI_STACKTRACEINFO );
                }
                return new QVariant(variant);
            }QTJAMBI_CATCH(const JavaException& exn){
                exn.report(env);
            }QTJAMBI_TRY_END
        }
        return new QVariant();
    };
}
auto QtTaskTree::Storage<QVariant>::dtor() -> StorageDestructor {
    return [](void *storage) {
        QVariant * variant = static_cast<QVariant *>(storage);
        if(QObject* object = variant->value<QObject*>()){
            delete object;
        }
        delete variant;
    };
}

namespace Java{
namespace QtTaskTree{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/tasktree,Timeout,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(JZ)
                                QTJAMBI_REPOSITORY_DEFINE_FIELD(__qt_directLink,J)
                                )
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/tasktree,Storage$ActiveStorage,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Class;Lio/qt/core/QVariant;)
                                )

}
}

void initialize_meta_info_QtTaskTree_impl(){
    using namespace RegistryAPI;
    {
        const std::type_info& typeId = registerValueTypeInfo<QtTaskTree::QCustomTask<JObjectWrapper>>("QtTaskTree::QCustomTask<JObjectWrapper>", "io/qt/tasktree/QCustomTask");
        Q_UNUSED(typeId)
        registerConstructorInfos(typeId, 0, &__qt_destruct_QtTaskTree_QCustomTask_JObjectWrapper_, {
                                                                                                   });
        registerDeleter(typeId, &deleter_QtTaskTree_QCustomTask_JObjectWrapper_);
        registerMetaType<QtTaskTree::QCustomTask<JObjectWrapper>>("QtTaskTree::QCustomTask<JObjectWrapper>");
    }
}
