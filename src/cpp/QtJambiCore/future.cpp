/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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
**
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "pch_p.h"
#include <QtCore/QtGlobal>
#include <qconfig.h>
#if QT_CONFIG(future)
#include <qfutureinterface.h>
#include <qfuturewatcher.h>
#endif //QT_CONFIG(future)
#include <QtJambi/QtJambiAPI>
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
#include "utils_p.h"
#include <QtJambi/JObjectWrapper>
#include <QtJambi/RegistryAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/qtjambi_cast.h>
#include <QtJambi/utils_p.h>
#include "future_p.h"

// QFutureWatcher:

class QFutureWatcher_shell : public QFutureWatcher<QVariant>, public QtJambiShellInterface
{
public:
    QFutureWatcher_shell(QObject* parent = nullptr);
    ~QFutureWatcher_shell() override;

    static void operator delete(void * ptr) noexcept;
private:
    QtJambiShell* __shell() const override final;
public:
    const QMetaObject *metaObject() const override{ return &staticMetaObject; }
    bool m_isVoid;
    friend class QFutureWatcher_mshell;
    friend class QFutureWatcher_oshell;
};

class QFutureWatcher_mshell : public QFutureWatcher_shell
{
public:
    using QFutureWatcher_shell::QFutureWatcher_shell;
    const QMetaObject *metaObject() const override final;
    void *qt_metacast(const char *) override final;
    int qt_metacall(QMetaObject::Call, int, void **) override final;
    friend class QFutureWatcher_oshell;
};

class QFutureWatcher_oshell : public QFutureWatcher_mshell
{
public:
    using QFutureWatcher_mshell::QFutureWatcher_mshell;
    void childEvent(QChildEvent* event0) override;
    void connectNotify(const QMetaMethod& signal0) override;
    void customEvent(QEvent* event0) override;
    void disconnectNotify(const QMetaMethod& signal0) override;
    bool event(QEvent* event0) override;
    bool eventFilter(QObject* watched0, QEvent* event1) override;
    void timerEvent(QTimerEvent* event0) override;
    static void operator delete(void * ptr) noexcept;
private:
    jmethodID __shell_javaMethod(int pos) const;
};

QtJambiShell* QFutureWatcher_shell::__shell() const { return *reinterpret_cast<QtJambiShell**>( quintptr(this) + sizeof(QFutureWatcher_shell) ); }

// emitting (writeShellConstructor)
QFutureWatcher_shell::QFutureWatcher_shell(QObject* parent)
    : QFutureWatcher<QVariant>(parent), m_isVoid(false)
{
    QTJAMBI_IN_CONSTRUCTOR_CALL("QFutureWatcher_shell::QFutureWatcher_shell(QFutureInterfaceBase::State initialState0)", this)
    QFutureWatcher_shell::__shell()->constructed(typeid(QFutureWatcher<QVariant>));
}

// emitting (writeShellDestructor)
QFutureWatcher_shell::~QFutureWatcher_shell()
{
    QTJAMBI_IN_DESTRUCTOR_CALL("QFutureWatcher_shell::~QFutureWatcher_shell()", this)
    QFutureWatcher_shell::__shell()->destructed(typeid(QFutureWatcher<QVariant>));
}

void QFutureWatcher_shell::operator delete(void * ptr) noexcept {
    reinterpret_cast<QFutureWatcher_shell*>(ptr)->QFutureWatcher_shell::__shell()->deleteShell();
}

// emitting (writeQObjectFunctions)
const QMetaObject *QFutureWatcher_mshell::metaObject() const
{
    return QFutureWatcher_shell::__shell()->metaObject();
}

void *QFutureWatcher_mshell::qt_metacast(const char *_clname)
{
    QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL("QFutureWatcher_mshell::qt_metacast(const char *_clname)", this)
    bool ok = false;
    void *rv = QFutureWatcher_shell::__shell()->qt_metacast(_clname, &ok);
    if (!ok)
        rv = QFutureWatcher::qt_metacast(_clname);
    return rv;
}

int QFutureWatcher_mshell::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL("QFutureWatcher_mshell::qt_metacall(QMetaObject::Call _c, int _id, void **_a)", this)
    _id = QFutureWatcher::qt_metacall(_c, _id, _a);
    if (_id >= 0)
        _id = QFutureWatcher_shell::__shell()->qt_metacall(_c, _id, _a);
    return _id;
}

// emitting Virtual overrides (virtualFunctions)
void QFutureWatcher_oshell::childEvent(QChildEvent* event0)
{
    if(jmethodID method_id = __shell_javaMethod(0)){
        QTJAMBI_JAVA_METHOD_CALL("QFutureWatcher::childEvent(QChildEvent*)", this)
        if (JniEnvironmentExceptionHandler __jni_env{300}) {
            QTJAMBI_TRY {
                if(jobject __java_this = QFutureWatcher_shell::__shell()->getJavaObjectLocalRef(__jni_env)){
                    jobject __java_event0 = qtjambi_cast<jobject>(__jni_env, event0);
                    QTJAMBI_INVALIDATE_AFTER_USE(__jni_env, __java_event0);
                    __jni_env->CallVoidMethod(__java_this, method_id, __java_event0);
                    __jni_env.checkException( QTJAMBI_STACKTRACEINFO_NOENV );
                } else {
                    QFutureWatcher_shell::__shell()->warnForMethod("QFutureWatcher::childEvent(QChildEvent*)", this);
                }
            } QTJAMBI_CATCH(const JavaException& exn){
                __jni_env.handleException(exn, this, "QFutureWatcher::childEvent(QChildEvent*)");
            } QTJAMBI_TRY_END
        }else{
            QObject::childEvent(event0);
        }
    }else{
        QObject::childEvent(event0);
    }
}

jmethodID QFutureWatcher_oshell::__shell_javaMethod(int pos) const { return QFutureWatcher_shell::__shell()->javaMethod(typeid(QFutureWatcherBase), pos); }

void QFutureWatcher_oshell::operator delete(void * ptr) noexcept {
    reinterpret_cast<QFutureWatcher_shell*>(ptr)->QFutureWatcher_shell::__shell()->deleteShell();
}

void QFutureWatcher_oshell::connectNotify(const QMetaMethod& signal0)
{
    if(jmethodID method_id = __shell_javaMethod(1)){
        QTJAMBI_JAVA_METHOD_CALL("QFutureWatcher::connectNotify(QMetaMethod)", this)
        if (JniEnvironmentExceptionHandler __jni_env{300}) {
            QTJAMBI_TRY {
                if(jobject __java_this = QFutureWatcher_shell::__shell()->getJavaObjectLocalRef(__jni_env)){
                    jobject __java_signal0 = qtjambi_cast<jobject>(__jni_env, signal0);
                    __jni_env->CallVoidMethod(__java_this, method_id, __java_signal0);
                    __jni_env.checkException( QTJAMBI_STACKTRACEINFO_NOENV );
                } else {
                    QFutureWatcher_shell::__shell()->warnForMethod("QFutureWatcher::connectNotify(QMetaMethod)", this);
                }
            } QTJAMBI_CATCH(const JavaException& exn){
                __jni_env.handleException(exn, this, "QFutureWatcher::connectNotify(QMetaMethod)");
            } QTJAMBI_TRY_END
        }else{
            QFutureWatcher::connectNotify(signal0);
        }
    }else{
        QFutureWatcher::connectNotify(signal0);
    }
}

void QFutureWatcher_oshell::customEvent(QEvent* event0)
{
    if(jmethodID method_id = __shell_javaMethod(2)){
        QTJAMBI_JAVA_METHOD_CALL("QFutureWatcher::customEvent(QEvent*)", this)
        if (JniEnvironmentExceptionHandler __jni_env{300}) {
            QTJAMBI_TRY {
                if(jobject __java_this = QFutureWatcher_shell::__shell()->getJavaObjectLocalRef(__jni_env)){
                    jobject __java_event0 = qtjambi_cast<jobject>(__jni_env, event0);
                    QTJAMBI_INVALIDATE_AFTER_USE(__jni_env, __java_event0);
                    __jni_env->CallVoidMethod(__java_this, method_id, __java_event0);
                    __jni_env.checkException( QTJAMBI_STACKTRACEINFO_NOENV );
                } else {
                    QFutureWatcher_shell::__shell()->warnForMethod("QFutureWatcher::customEvent(QEvent*)", this);
                }
            } QTJAMBI_CATCH(const JavaException& exn){
                __jni_env.handleException(exn, this, "QFutureWatcher::customEvent(QEvent*)");
            } QTJAMBI_TRY_END
        }else{
            QObject::customEvent(event0);
        }
    }else{
        QObject::customEvent(event0);
    }
}

void QFutureWatcher_oshell::disconnectNotify(const QMetaMethod& signal0)
{
    if(jmethodID method_id = __shell_javaMethod(3)){
        QTJAMBI_JAVA_METHOD_CALL("QFutureWatcher::disconnectNotify(QMetaMethod)", this)
        if (JniEnvironmentExceptionHandler __jni_env{300}) {
            QTJAMBI_TRY {
                if(jobject __java_this = QFutureWatcher_shell::__shell()->getJavaObjectLocalRef(__jni_env)){
                    jobject __java_signal0 = qtjambi_cast<jobject>(__jni_env, signal0);
                    __jni_env->CallVoidMethod(__java_this, method_id, __java_signal0);
                    __jni_env.checkException( QTJAMBI_STACKTRACEINFO_NOENV );
                } else {
                    QFutureWatcher_shell::__shell()->warnForMethod("QFutureWatcher::disconnectNotify(QMetaMethod)", this);
                }
            } QTJAMBI_CATCH(const JavaException& exn){
                __jni_env.handleException(exn, this, "QFutureWatcher::disconnectNotify(QMetaMethod)");
            } QTJAMBI_TRY_END
        }else{
            QFutureWatcher::disconnectNotify(signal0);
        }
    }else{
        QFutureWatcher::disconnectNotify(signal0);
    }
}

bool QFutureWatcher_oshell::event(QEvent* event0)
{
    if(jmethodID method_id = __shell_javaMethod(4)){
        QTJAMBI_JAVA_METHOD_CALL("QFutureWatcher::event(QEvent*)", this)
        if (JniEnvironmentExceptionHandler __jni_env{300}) {
            bool  __qt_return_value = false;
            QTJAMBI_TRY {
                if(jobject __java_this = QFutureWatcher_shell::__shell()->getJavaObjectLocalRef(__jni_env)){
                    jobject __java_event0 = qtjambi_cast<jobject>(__jni_env, event0);
                    QTJAMBI_INVALIDATE_AFTER_USE(__jni_env, __java_event0);
                    jboolean __java_return_value = __jni_env->CallBooleanMethod(__java_this, method_id, __java_event0);
                    __jni_env.checkException( QTJAMBI_STACKTRACEINFO_NOENV );
                    __qt_return_value = static_cast<bool>(__java_return_value);
                } else {
                    if(event0->type()==QEvent::DeferredDelete || event0->type()==QEvent::ChildRemoved){
                        __qt_return_value = QFutureWatcher::event(event0);
                    } else {
                        QFutureWatcher_shell::__shell()->warnForMethod("QFutureWatcher::event(QEvent*)", this);
                    }
                }
            } QTJAMBI_CATCH(const JavaException& exn){
                __jni_env.handleException(exn, this, "QFutureWatcher::event(QEvent*)");
            } QTJAMBI_TRY_END
                return __qt_return_value;
        }else{
            return QFutureWatcher::event(event0);
        }
    }else{
        return QFutureWatcher::event(event0);
    }
}

bool QFutureWatcher_oshell::eventFilter(QObject* watched0, QEvent* event1)
{
    if(jmethodID method_id = __shell_javaMethod(5)){
        QTJAMBI_JAVA_METHOD_CALL("QFutureWatcher::eventFilter(QObject*,QEvent*)", this)
        if (JniEnvironmentExceptionInhibitorAndBlocker __jni_env{400}) {
            bool  __qt_return_value = false;
            QTJAMBI_TRY {
                if(jobject __java_this = QFutureWatcher_shell::__shell()->getJavaObjectLocalRef(__jni_env)){
                    jobject __java_watched0;
                    try{
                        __java_watched0 = qtjambi_cast<jobject>(__jni_env, watched0);
                        if(!__java_watched0 && watched0)
                            return QObject::eventFilter(watched0, event1);
                    }catch(const JavaException&){
                        return QObject::eventFilter(watched0, event1);
                    }
                    jobject __java_event1;
                    try{
                        __java_event1 = qtjambi_cast<jobject>(__jni_env, event1);
                    }catch(const JavaException&){
                        return QObject::eventFilter(watched0, event1);
                    }
                    QTJAMBI_INVALIDATE_AFTER_USE(__jni_env, __java_event1);
                    jboolean __java_return_value = __jni_env->CallBooleanMethod(__java_this, method_id, __java_watched0, __java_event1);
                    __jni_env.checkException( QTJAMBI_STACKTRACEINFO_NOENV );
                    __qt_return_value = static_cast<bool>(__java_return_value);
                } else {
                    if(event1->type()==QEvent::DeferredDelete || event1->type()==QEvent::ChildRemoved){
                        __qt_return_value = QObject::eventFilter(watched0, event1);
                    } else {
                        QFutureWatcher_shell::__shell()->warnForMethod("QFutureWatcher::eventFilter(QObject*,QEvent*)", this);
                    }
                }
            } QTJAMBI_CATCH(const JavaException& exn){
                __jni_env.handleException(exn, this, "QFutureWatcher::eventFilter(QObject*,QEvent*)");
            } QTJAMBI_TRY_END
                __jni_env.releaseException();
            return __qt_return_value;
        }else{
            QtJambiExceptionBlocker __qj_exceptionBlocker;
            bool  __qt_return_value = QObject::eventFilter(watched0, event1);
            __qj_exceptionBlocker.release(nullptr);
            return __qt_return_value;
        }
    }else{
        QtJambiExceptionBlocker __qj_exceptionBlocker;
        bool  __qt_return_value = QObject::eventFilter(watched0, event1);
        __qj_exceptionBlocker.release(nullptr);
        return __qt_return_value;
    }
}

void QFutureWatcher_oshell::timerEvent(QTimerEvent* event0)
{
    if(jmethodID method_id = __shell_javaMethod(8)){
        QTJAMBI_JAVA_METHOD_CALL("QFutureWatcher::timerEvent(QTimerEvent*)", this)
        if (JniEnvironmentExceptionHandler __jni_env{300}) {
            QTJAMBI_TRY {
                if(jobject __java_this = QFutureWatcher_shell::__shell()->getJavaObjectLocalRef(__jni_env)){
                    jobject __java_event0 = qtjambi_cast<jobject>(__jni_env, event0);
                    QTJAMBI_INVALIDATE_AFTER_USE(__jni_env, __java_event0);
                    __jni_env->CallVoidMethod(__java_this, method_id, __java_event0);
                    __jni_env.checkException( QTJAMBI_STACKTRACEINFO_NOENV );
                } else {
                    QFutureWatcher_shell::__shell()->warnForMethod("QFutureWatcher::timerEvent(QTimerEvent*)", this);
                }
            } QTJAMBI_CATCH(const JavaException& exn){
                __jni_env.handleException(exn, this, "QFutureWatcher::timerEvent(QTimerEvent*)");
            } QTJAMBI_TRY_END
        }else{
            QObject::timerEvent(event0);
        }
    }else{
        QObject::timerEvent(event0);
    }
}

// new QFutureWatcher()
void __qt_construct_QFutureWatcher(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue*, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QFutureWatcher(QObject*)")
#if QT_CONFIG(future)
    QFutureWatcher<QVariant> *__qt_this;
    if(__qtjambi_constructor_options & QtJambiAPI::HasOverrides)
        __qt_this = new(__qtjambi_ptr) QFutureWatcher_oshell();
    else if(__qtjambi_constructor_options & QtJambiAPI::HasDerivedMetaObject)
        __qt_this = new(__qtjambi_ptr) QFutureWatcher_mshell();
    else
        __qt_this = new(__qtjambi_ptr) QFutureWatcher_shell();
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_env)
    Q_UNUSED(__jni_object)
#else
    Q_UNUSED(__jni_env)
    Q_UNUSED(__jni_object)
    Q_UNUSED(__java_arguments)
    Q_UNUSED(__qtjambi_ptr)
    JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
}

// new QFutureWatcher(QObject*)
void __qt_construct_QFutureWatcher_QObject_ptr(void* __qtjambi_ptr, JNIEnv* __jni_env, jobject __jni_object, jvalue* __java_arguments, QtJambiAPI::ConstructorOptions __qtjambi_constructor_options)
{
    QTJAMBI_NATIVE_METHOD_CALL("construct QFutureWatcher(QObject*)")
#if QT_CONFIG(future)
    jobject parent0 = __java_arguments[0].l;
    QObject* __qt_parent0 = qtjambi_cast<QObject*>(__jni_env, parent0);
    QFutureWatcher<QVariant> *__qt_this;
    if(__qtjambi_constructor_options & QtJambiAPI::HasOverrides)
        __qt_this = new(__qtjambi_ptr) QFutureWatcher_oshell(__qt_parent0);
    else if(__qtjambi_constructor_options & QtJambiAPI::HasDerivedMetaObject)
        __qt_this = new(__qtjambi_ptr) QFutureWatcher_mshell(__qt_parent0);
    else
        __qt_this = new(__qtjambi_ptr) QFutureWatcher_shell(__qt_parent0);
    Q_UNUSED(__qt_this)
    Q_UNUSED(__jni_object)
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
    QTJAMBI_NATIVE_METHOD_CALL("destruct QFutureWatcher")
#if QT_CONFIG(future)
    reinterpret_cast<QFutureWatcher_shell*>(ptr)->~QFutureWatcher_shell();
#else
    Q_UNUSED(ptr)
#endif // QT_CONFIG(future)
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QFutureWatcher_initialize_1native__Lio_qt_core_QFutureWatcher_2Lio_qt_core_QObject_2
(JNIEnv *__jni_env,
 jclass __jni_class,
 jobject __jni_object,
 jobject parent0)
{
    QTJAMBI_NATIVE_METHOD_CALL("QFutureWatcher::QFutureWatcher(QObject * parent)")
    QTJAMBI_TRY{
#if QT_CONFIG(future)
        jvalue arguments;
        arguments.l = parent0;
        QtJambiShell::initialize(__jni_env, __jni_class, __jni_object, &__qt_construct_QFutureWatcher_QObject_ptr, sizeof(QFutureWatcher_shell), alignof(QFutureWatcher_shell), typeid(QFutureWatcher<QVariant>), 0, QFutureWatcher_shell::staticMetaObject, true, false, &arguments);
#else
        Q_UNUSED(__jni_object)
        Q_UNUSED(__jni_class)
        Q_UNUSED(parent0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(future)
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QFutureWatcher_future
(JNIEnv *__jni_env, jobject _this)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        QFutureWatcherBase *__qt_this = QtJambiAPI::convertJavaObjectToNative<QFutureWatcherBase>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        if(QFutureWatcher_shell* shell = dynamic_cast<QFutureWatcher_shell*>(__qt_this)){
            if(shell->m_isVoid){
                result = qtjambi_cast<jobject>(__jni_env, reinterpret_cast<QFutureWatcher<void>*>(__qt_this)->future());
            }else{
                result = qtjambi_cast<jobject>(__jni_env, shell->future());
            }
        }else{
            QtJambiAPI::FutureGetter futureGetter = QtJambiAPI::FutureGetter(Java::QtCore::QFutureWatcher::futureGetter(__jni_env, _this));
            if(!futureGetter)
                JavaException::raiseQNoImplementationException(__jni_env, "future() not available." QTJAMBI_STACKTRACEINFO );
            result = futureGetter(__jni_env, __qt_this);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QFutureWatcher_resultAt
(JNIEnv *__jni_env, jobject _this, jint index)
{
    jobject result = nullptr;
    QTJAMBI_TRY{
        QFutureWatcherBase *__qt_this = QtJambiAPI::convertJavaObjectToNative<QFutureWatcherBase>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        if(QFutureWatcher_shell* shell = dynamic_cast<QFutureWatcher_shell*>(__qt_this)){
            result = QtJambiAPI::convertQVariantToJavaObject(__jni_env, shell->resultAt(index));
        }else{
            QtJambiAPI::FutureResult futureResult = QtJambiAPI::FutureResult(Java::QtCore::QFutureWatcher::futureResult(__jni_env, _this));
            if(!futureResult)
                JavaException::raiseQNoImplementationException(__jni_env, "resultAt(int) not available for QFutureWatcher<void>." QTJAMBI_STACKTRACEINFO );
            result = futureResult(__jni_env, __qt_this, index);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_core_QFutureWatcher_setFuture
(JNIEnv *__jni_env, jobject _this, jobject future)
{
    QTJAMBI_TRY{
        QFutureWatcherBase *__qt_this = QtJambiAPI::convertJavaObjectToNative<QFutureWatcherBase>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        if(QFutureWatcher_shell* shell = dynamic_cast<QFutureWatcher_shell*>(__qt_this)){
            QFuture<QVariant>* _future = QtJambiAPI::convertJavaObjectToNative<QFuture<QVariant>>(__jni_env, future);
            QFutureInterfaceBase* base = QtPrivate::Continuation<QVariant,bool,bool>::sourceFuture(_future);
            shell->m_isVoid = !QtJambiAPI::isVariantFutureInterface(base);
            QFutureInterface<QVariant> _fi(*base);
            shell->setFuture(QFuture<QVariant>(&_fi));
        }else{
            QtJambiAPI::FutureSetter futureSetter = QtJambiAPI::FutureSetter(Java::QtCore::QFutureWatcher::futureSetter(__jni_env, _this));
            if(!futureSetter)
                JavaException::raiseQNoImplementationException(__jni_env, "setFuture() not available." QTJAMBI_STACKTRACEINFO );
            futureSetter(__jni_env, __qt_this, future);
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QFuture_metaType
    (JNIEnv *__jni_env, jobject __this)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QFuture<QVariant>* _future = QtJambiAPI::convertJavaObjectToNative<QFuture<QVariant>>(__jni_env, __this);
        const QFutureInterfaceBase* base = QtPrivate::Continuation<QVariant,bool,bool>::sourceFuture(_future);
        QtJambiAPI::checkNullPointer(__jni_env, base);
        if(QtJambiAPI::isVariantFutureInterface(base)){
            result = qtjambi_cast<jobject>(__jni_env, QMetaType::fromType<QFuture<QVariant>>());
        }else{
            result = qtjambi_cast<jobject>(__jni_env, QMetaType::fromType<QFuture<void>>());
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

extern "C" JNIEXPORT jobject JNICALL Java_io_qt_core_QFutureInterface_metaType
    (JNIEnv *__jni_env, jobject __this)
{
    jobject result{nullptr};
    QTJAMBI_TRY{
        QFutureInterfaceBase* base = QtJambiAPI::convertJavaObjectToNative<QFutureInterfaceBase>(__jni_env, __this);
        QtJambiAPI::checkNullPointer(__jni_env, base);
        if(QtJambiAPI::isVariantFutureInterface(base)){
            result = qtjambi_cast<jobject>(__jni_env, QMetaType::fromType<QFutureInterface<QVariant>>());
        }else{
            result = qtjambi_cast<jobject>(__jni_env, QMetaType::fromType<QFutureInterface<void>>());
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
        return result;
}

// emitting Field accessors (writeFieldAccessors)
// emitting (writeCustomStructors)
// emitting (writeJavaLangObjectOverrideFunctions)
void initialize_meta_info_QFutureInterface(){
    using namespace RegistryAPI;
#if QT_CONFIG(future)
    {
        struct Deleter{
            void operator()(QMetaObject* m) const{
                free(m);
            }
        };
        static std::unique_ptr<QMetaObject,Deleter> FutureWatcherMetaObject{[]()->QMetaObject*{
            QMetaObjectBuilder builder;
            builder.setClassName("QFutureWatcher<QVariant>");
            builder.setSuperClass(&QFutureWatcherBase::staticMetaObject);
            //builder.setFlags();
            return builder.toMetaObject();
        }()};
        const std::type_info& typeId = registerQObjectTypeInfo<QFutureWatcher<QVariant>>("QFutureWatcher<QVariant>", "io/qt/core/QFutureWatcher");
        registerMetaObject(typeId, *FutureWatcherMetaObject, false, nullptr);
        registerSizeOfShell(typeId, sizeof(QFutureWatcher_shell), alignof(QFutureWatcher_shell));
        registerFunctionInfos(typeId, {
                                       FunctionInfo{/* 0 */ "childEvent", "(Lio/qt/core/QChildEvent;)V"},
                                       FunctionInfo{/* 1 */ "connectNotify", "(Lio/qt/core/QMetaMethod;)V"},
                                       FunctionInfo{/* 2 */ "customEvent", "(Lio/qt/core/QEvent;)V"},
                                       FunctionInfo{/* 3 */ "disconnectNotify", "(Lio/qt/core/QMetaMethod;)V"},
                                       FunctionInfo{/* 4 */ "event", "(Lio/qt/core/QEvent;)Z"},
                                       FunctionInfo{/* 5 */ "eventFilter", "(Lio/qt/core/QObject;Lio/qt/core/QEvent;)Z"},
                                       FunctionInfo{/* 6 */ "timerEvent", "(Lio/qt/core/QTimerEvent;)V"}});
        registerConstructorInfos(typeId, 0, &__qt_destruct_QFutureWatcher, {
                                                                                   ConstructorInfo{&__qt_construct_QFutureWatcher, nullptr}
                                                                                   ,ConstructorInfo{&__qt_construct_QFutureWatcher_QObject_ptr, "Lio/qt/core/QObject;"}
                                                                               });
    }
#endif //QT_CONFIG(future)

}
