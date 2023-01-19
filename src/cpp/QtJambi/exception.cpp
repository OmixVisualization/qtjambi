/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QThreadStorage>
#include <QtCore/QSharedData>
#include <QtCore/QByteArray>
#include "qtjambiapi.h"
#include "jobjectwrapper.h"
#include "exception.h"
#include "java_p.h"
#include "qtjambi_cast.h"

class JavaExceptionPrivate : public QSharedData{
public:
    JavaExceptionPrivate() = default;
    JavaExceptionPrivate(const JavaExceptionPrivate& copy)
        : QSharedData(copy), m_throwable(copy.m_throwable), m_what(copy.m_what){}
    JavaExceptionPrivate(JNIEnv *env, jthrowable obj)
        : QSharedData(), m_throwable(env, obj), m_what(){}
    JObjectWrapper m_throwable;
    QByteArray m_what;
    friend JavaException;
};

JavaException::JavaException() Q_DECL_NOEXCEPT
    : QException(), p(){}

JavaException::JavaException(JNIEnv *env, jthrowable obj)
    : QException(), p(env->IsSameObject(obj, nullptr) ? nullptr : new JavaExceptionPrivate(env, obj)){
    update(env);
#if defined(Q_OS_ANDROID)
    static bool preprint = qgetenv("QTJAMBI_PREPRINT_EXCEPTIONS")=="true";
    if(preprint){
        fprintf(stderr, "JavaException about to throw: %s", what());
        env->Throw(obj);
        env->ExceptionDescribe();
        env->ExceptionClear();
    }
#endif
}

void JavaException::update(JNIEnv *env)
{
    try{
        if(env){
            if(jthrowable t = object()){
                jstring msg = Java::Runtime::Throwable::tryGetMessage(env, t);
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                int length = msg ? env->GetStringUTFLength(msg) : 0;
                if(env->ExceptionCheck()){
                    env->ExceptionDescribe();
                    env->ExceptionClear();
                }
                if(length<=0){
                    msg = Java::Runtime::Class::tryGetName(env, env->GetObjectClass(t));
                    if(env->ExceptionCheck()){
                        env->ExceptionDescribe();
                        env->ExceptionClear();
                    }
                    length = msg ? env->GetStringUTFLength(msg) : 0;
                    if(env->ExceptionCheck()){
                        env->ExceptionDescribe();
                        env->ExceptionClear();
                    }
                }
                if(length>0){
                    p->m_what.resize(length);
                    env->GetStringUTFRegion(msg, 0, length, p->m_what.data());
                    if(env->ExceptionCheck()){
                        env->ExceptionDescribe();
                        env->ExceptionClear();
                    }
                }else{
                    p->m_what.clear();
                }
                //report(env);
            }else if(p){
                p = nullptr;
            }
        }
    }catch(...){}
}

JavaException::JavaException(const JavaException& copy) Q_DECL_NOEXCEPT
    : QException(copy), p(copy.p) {}

JavaException::JavaException(JavaException&& copy) Q_DECL_NOEXCEPT
    : QException(std::move(copy)), p(std::move(copy.p)) {}

JavaException::~JavaException() Q_DECL_NOEXCEPT {}

char const* JavaException::what() const Q_DECL_NOEXCEPT
{
    return !p || p->m_what.isEmpty() ? "Unknown exception" : p->m_what.data();
}

void JavaException::raiseInJava(JNIEnv* env) const {
    jthrowable t = object();
    if(t){
        env->Throw(t);
    }
}

JavaException* JavaException::clone() const{
    return new JavaException(*this);
}

jthrowable JavaException::object() const{
    return reinterpret_cast<jthrowable>(p ? p->m_throwable.object() : nullptr);
}

void JavaException::addSuppressed(JNIEnv* env, const JavaException& exn) const{
    if(exn){
        try{
            if(!object()){
                JavaException& _this = *const_cast<JavaException*>(this);
                _this = exn;
            }else{
                Java::Runtime::Throwable::addSuppressed(env, object(), exn.object());
            }
        }catch(const JavaException& _exn){ exn.report(env); _exn.report(env); }catch(...){}
    }
}

void JavaException::report(JNIEnv* env) const{
    jthrowable t = object();
    if(t){
        JniLocalFrame __jniLocalFrame(env, 200);
        try{
            Java::QtJambi::ExceptionUtility::reportException(env, nullptr, t);
        }catch(const JavaException& exn){
            printf("QtJambi: exception pending at ExceptionUtility.reportException(...): %s\n", exn.what());
            printf("QtJambi: exception pending in native code: %s\n", what());
        }catch(...){
            printf("QtJambi: exception pending in native code: %s\n", what());
        }
    }
}

JavaException& JavaException::operator =(const JavaException& other) Q_DECL_NOEXCEPT {
    QException::operator=(static_cast<const QException&>(other));
    p = other.p;
    return *this;
}

JavaException& JavaException::operator =(JavaException&& other) Q_DECL_NOEXCEPT {
    QException::operator=(std::move(static_cast<QException&&>(other)));
    p = std::move(other.p);
    return *this;
}

JavaException::operator bool() const Q_DECL_NOEXCEPT {
    return object();
}

void JavaException::raise() const{
    if(object()){
#if defined(Q_OS_ANDROID)
        if(JniEnvironment env{200}){
            throw JavaException(env, object());
        }
#endif
        JavaException e = *this;
        throw e;
    }
}

#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) env, t, methodName, fileName, lineNumber
#else
#define QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) env, t
#endif

#ifdef QTJAMBI_STACKTRACE
void JavaException::raise(JNIEnv* env, const char *methodName, const char *fileName, int lineNumber) const {
    jthrowable t = object();
    if(t){
        jstring jmethodName = methodName ? env->NewStringUTF(methodName) : nullptr;
        jstring jfileName = fileName ? env->NewStringUTF(fileName) : nullptr;
        try{
            Java::QtJambi::ExceptionUtility::extendStackTrace(env, t, jmethodName, jfileName, lineNumber);
        }catch(const JavaException& exn){ exn.report(env); }
        throw JavaException(env, t);
    }
}
#define raiseThrowable QtJambiPrivate::raiseJavaException
#else
#define raiseThrowable throw JavaException
#endif

#ifdef QTJAMBI_STACKTRACE
void JavaException::check(JNIEnv* env){
    if(env->ExceptionCheck()){
        jthrowable t = env->ExceptionOccurred();
        env->ExceptionClear();
        if(t){
            throw JavaException(env, t);
        }
    }
}
#endif

void JavaException::check(JNIEnv* env QTJAMBI_STACKTRACEINFO_DECL ){
    if(env->ExceptionCheck()){
        jthrowable t = env->ExceptionOccurred();
        env->ExceptionClear();
        if(t)
            raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
    }
}

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
void JavaException::raiseNullPointerException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::NullPointerException::newInstance(env, jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseIllegalArgumentException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::IllegalArgumentException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQNoImplementationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::QtJambi::QNoImplementationException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseError(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::Error::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseRuntimeException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::RuntimeException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseUnsupportedOperationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::Runtime::UnsupportedOperationException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQThreadAffinityException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3){
    jstring jmessage = message ? env->NewStringUTF(message) : nullptr;
    check(env);
    jthrowable t = Java::QtJambi::QThreadAffinityException::newInstance(env,jmessage, t1,
                                                          qtjambi_cast<jobject>(env, t2),
                                                          qtjambi_cast<jobject>(env, t3)
                                        );
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseNullPointerException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::NullPointerException::newInstance(env, jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseIllegalArgumentException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::IllegalArgumentException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQNoImplementationException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::QtJambi::QNoImplementationException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseError(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::Error::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseRuntimeException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::RuntimeException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseUnsupportedOperationException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::UnsupportedOperationException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQThreadAffinityException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    JavaException::check(env);
    jthrowable t = Java::QtJambi::QThreadAffinityException::newInstance(env,jmessage, t1,
                                                          qtjambi_cast<jobject>(env, t2),
                                                          qtjambi_cast<jobject>(env, t3)
                                        );
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}
#else
void JavaException::raiseNullPointerException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::NullPointerException::newInstance(env, jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseIllegalArgumentException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::IllegalArgumentException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQNoImplementationException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::QtJambi::QNoImplementationException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseError(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::Error::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseRuntimeException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::RuntimeException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseUnsupportedOperationException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::UnsupportedOperationException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseQThreadAffinityException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    JavaException::check(env);
    jthrowable t = Java::QtJambi::QThreadAffinityException::newInstance(env,jmessage, t1,
                                                          qtjambi_cast<jobject>(env, t2),
                                                          qtjambi_cast<jobject>(env, t3)
                                        );
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}
#endif

JNIEnv *currentJNIEnvironment(bool initializeJavaThread = true);

struct ExceptionHandler{
    ExceptionHandler() : exn(), methodName(nullptr), blocking(false), reraise(false) {}
    ~ExceptionHandler(){
        if(exn){
            if(DefaultJniEnvironment env{200}){
                try{
                    jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
                    Java::QtJambi::ExceptionUtility::reportException(env, msg, exn.object());
                }catch(const JavaException& _exn){
                    if(methodName){
                        printf("An exception occured in %s: %s\n", methodName, exn.what());
                    }else{
                        printf("An exception occured: %s\n", exn.what());
                    }
                    printf("An exception occured in ExceptionUtility.reportException(...): %s\n", _exn.what());
                }
            }else if(methodName){
                qWarning("An exception occured in %s: %s", methodName, exn.what());
            }else{
                qWarning("An exception occured: %s", exn.what());
            }
        }
    }
    JavaException exn;
    const char* methodName;
    uint blocking : 1;
    uint reraise : 1;
    static QThreadStorage<ExceptionHandler> storage;
};

QThreadStorage<ExceptionHandler> ExceptionHandler::storage;

QtJambiExceptionHandler::QtJambiExceptionHandler()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    data = exceptionHandler.blocking;
    exceptionHandler.blocking = false;
}

QtJambiExceptionHandler::~QtJambiExceptionHandler(){
    ExceptionHandler::storage.localData().blocking = data;
}

void QtJambiExceptionHandler::handle(JNIEnv *env, const JavaException& exn, const char* methodName){
    if(data){
        ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
        if(exceptionHandler.exn){
            if(!env){
                env = currentJNIEnvironment();
                if(!env){
                    if(methodName){
                        qWarning("An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qWarning("An exception occured: %s", exn.what());
                    }
                    return;
                }else{
                    JniLocalFrame __jniLocalFrame(env, 200);
                    exceptionHandler.exn.addSuppressed(env, exn);
                }
            }else{
                exceptionHandler.exn.addSuppressed(env, exn);
            }
        }else{
            exceptionHandler.exn = exn;
            exceptionHandler.methodName = methodName;
        }
    }else if(exn){
        throw JavaException(exn);//JavaException(env, exn.object());
    }
}

QtJambiExceptionInhibitor::QtJambiExceptionInhibitor()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    data = (exceptionHandler.blocking ? 0x01 : 0x0) | (exceptionHandler.reraise ? 0x10 : 0x0);
    exceptionHandler.blocking = false;
}

QtJambiExceptionInhibitor::~QtJambiExceptionInhibitor(){
    ExceptionHandler::storage.localData().blocking = (data & 0x01);
}

void QtJambiExceptionInhibitor::handle(JNIEnv *env, const JavaException& exn, const char* methodName){
    if(!(data & 0x01) || (data & 0x10)){
        ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
        if(exceptionHandler.exn){
            if(!env){
                env = currentJNIEnvironment();
                if(!env){
                    if(methodName){
                        qWarning("An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qWarning("An exception occured: %s", exn.what());
                    }
                    return;
                }else{
                    JniLocalFrame __jniLocalFrame(env, 200);
                    exceptionHandler.exn.addSuppressed(env, exn);
                }
            }else{
                exceptionHandler.exn.addSuppressed(env, exn);
            }
        }else{
            exceptionHandler.exn = exn;
            exceptionHandler.methodName = methodName;
        }
    }else{
        if(!env){
            env = currentJNIEnvironment();
            if(!env){
                if(methodName){
                    qWarning("An exception occured in %s: %s", methodName, exn.what());
                }else{
                    qWarning("An exception occured: %s", exn.what());
                }
                return;
            }
        }
        JniLocalFrame __jniLocalFrame(env, 200);
        try{
            jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
            Java::QtJambi::ExceptionUtility::reportException(env, msg, exn.object());
        }catch(const JavaException& _exn){
            if(methodName){
                printf("An exception occured in %s: %s\n", methodName, exn.what());
            }else{
                printf("An exception occured: %s\n", exn.what());
            }
            printf("An exception occured in ExceptionUtility.reportException(...): %s\n", _exn.what());
        }
    }
}

QtJambiExceptionBlocker::QtJambiExceptionBlocker()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    data = exceptionHandler.blocking;
    exceptionHandler.blocking = true;
}

QtJambiExceptionBlocker::~QtJambiExceptionBlocker(){
    ExceptionHandler::storage.localData().blocking = data;
}

void QtJambiExceptionBlocker::release(JNIEnv *env){
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    if(exceptionHandler.exn){
        JavaException exn = exceptionHandler.exn;
        exceptionHandler.exn = JavaException();
        if(exceptionHandler.reraise){
            exceptionHandler.methodName = nullptr;
            if(exn)
                throw exn;
        }else{
            const char* methodName = exceptionHandler.methodName;
            exceptionHandler.methodName = nullptr;
            if(!env){
                env = currentJNIEnvironment();
                if(!env){
                    if(methodName){
                        qWarning("An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qWarning("An exception occured: %s", exn.what());
                    }
                    return;
                }
            }
            JniLocalFrame __jniLocalFrame(env, 200);
            try{
                jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
                Java::QtJambi::ExceptionUtility::reportException(env, msg, exn.object());
            }catch(const JavaException& _exn){
                if(methodName){
                    printf("An exception occured in %s: %s\n", methodName, exn.what());
                }else{
                    printf("An exception occured: %s\n", exn.what());
                }
                printf("An exception occured in ExceptionUtility.reportException(...): %s\n", _exn.what());
            }
        }
    }
}

QtJambiExceptionRaiser::QtJambiExceptionRaiser()
    : data(0) {
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    data = exceptionHandler.reraise;
    exceptionHandler.reraise = true;
}

QtJambiExceptionRaiser::~QtJambiExceptionRaiser(){
    ExceptionHandler::storage.localData().reraise = data;
}

void QtJambiExceptionRaiser::raise(JNIEnv *){
    ExceptionHandler& exceptionHandler = ExceptionHandler::storage.localData();
    if(exceptionHandler.exn){
        JavaException exn = exceptionHandler.exn;
        exceptionHandler.exn = JavaException();
        if(exn)
            throw exn;
    }
}

#if defined(QTJAMBI_CENTRAL_TRY_CATCH)
namespace TryCatchWrapperAPI{
void TypedCatcher::operator()(const JavaException& exn){m_caller(m_functor, exn);}
void TypedTrial::operator()(){m_caller(m_functor);}

void tryCatchAny(TypedTrial&& fct, TypedTrial&& anyHandler){
    try{
        fct();
    }catch(...){
        anyHandler();
    }
}

void tryCatch(TypedTrial&& fct, TypedCatcher&& handler){
    try{
        fct();
    }catch(const JavaException& exn){
        handler(exn);
    }
}
}
#endif

void QtJambiPrivate::raiseJavaException(JNIEnv* env, jthrowable newInstance)
{
    throw JavaException(env, newInstance);
}

#ifdef QTJAMBI_STACKTRACE
void QtJambiPrivate::raiseJavaException(JNIEnv* env, jthrowable newInstance, const char *methodName, const char *fileName, int lineNumber)
{
    jstring jmethodName = methodName ? env->NewStringUTF(methodName) : nullptr;
    jstring jfileName = fileName ? env->NewStringUTF(fileName) : nullptr;
    try{
        Java::QtJambi::ExceptionUtility::extendStackTrace(env, newInstance, jmethodName, jfileName, lineNumber);
    }catch(const JavaException& exn){ exn.report(env); }
    throw JavaException(env, newInstance);
}
#endif

void QtJambiPrivate::javaExceptionCheck(JNIEnv* env){
    JavaException::check(env);
}

void QtJambiPrivate::javaInstanceCheck(JNIEnv* env,jobject object, jclass class_ref, bool isMemberFunction, const char* name){
    if(env->IsSameObject(object, nullptr)){
        if(isMemberFunction)
            JavaException::raiseNullPointerException(env, QLatin1String("Cannot invoke member function %1.%2(...) on null.").arg(QtJambiAPI::getClassName(env, class_ref), name) QTJAMBI_STACKTRACEINFO );
        else
            JavaException::raiseNullPointerException(env, QLatin1String("Cannot access member field %1.%2 on null.").arg(QtJambiAPI::getClassName(env, class_ref), name) QTJAMBI_STACKTRACEINFO );
    }
    if(!env->IsInstanceOf(object, class_ref)){
        if(isMemberFunction)
            JavaException::raiseIllegalArgumentException(env, QLatin1String("Cannot invoke member function %1.%2(...) on object of type %3.").arg(QtJambiAPI::getClassName(env, class_ref), name, QtJambiAPI::getObjectClassName(env, object)) QTJAMBI_STACKTRACEINFO );
        else
            JavaException::raiseIllegalArgumentException(env, QLatin1String("Cannot access member field %1.%2 on object of type %3.").arg(QtJambiAPI::getClassName(env, class_ref), name, QtJambiAPI::getObjectClassName(env, object)) QTJAMBI_STACKTRACEINFO );
    }
}
