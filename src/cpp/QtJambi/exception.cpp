/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#include <thread>
#include <sstream>
#include "qtjambiapi.h"
#include "jobjectwrapper.h"
#include "exception.h"
#include "java_p.h"
#include "threadutils_p.h"
#include "utils_p.h"
#include "qtjambi_cast.h"

class JavaExceptionPrivate : public QSharedData{
public:
    JavaExceptionPrivate() = default;
    JavaExceptionPrivate(const JavaExceptionPrivate& copy) = delete;
    JavaExceptionPrivate(JavaExceptionPrivate&& copy) = delete;
    JavaExceptionPrivate(JNIEnv *env, jthrowable obj)
        : QSharedData(), m_throwable(jthrowable(env->NewGlobalRef(obj))), m_what(){}
    ~JavaExceptionPrivate(){
        if(m_throwable){
            if(DefaultJniEnvironment env{0}){
                jthrowable throwable = nullptr;
                if(env->ExceptionCheck()){
                    throwable = env->ExceptionOccurred();
                    env->ExceptionClear();
                }
                env->DeleteGlobalRef(m_throwable);
                if(throwable){
                    env->Throw(throwable);
                    env->DeleteLocalRef(throwable);
                }
            }
        }
    }
    static void clearException(JNIEnv *env, JavaException& exn){
        if(exn.p->m_throwable){
            jthrowable throwable = nullptr;
            if(env->ExceptionCheck()){
                throwable = env->ExceptionOccurred();
                env->ExceptionClear();
            }
            env->DeleteGlobalRef(exn.p->m_throwable);
            exn.p->m_throwable = nullptr;
            if(throwable){
                env->Throw(throwable);
                env->DeleteLocalRef(throwable);
            }
        }
        exn.p->m_what.clear();
        exn.p = nullptr;
    }
private:
    jthrowable m_throwable = nullptr;
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
            if(jthrowable t = throwable(env)){
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
                    p->m_what.fill(0, length+1);
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
    if(p && p->m_throwable){
        env->Throw(p->m_throwable);
    }
}

JavaException* JavaException::clone() const{
    return new JavaException(*this);
}

void JavaException::addSuppressed(JNIEnv* env, const JavaException& exn) const{
    if(exn){
        try{
            if(!*this){
                JavaException& _this = *const_cast<JavaException*>(this);
                _this = exn;
            }else{
                Java::Runtime::Throwable::addSuppressed(env, throwable(env), exn.throwable(env));
            }
        }catch(const JavaException& _exn){ exn.report(env); _exn.report(env); }catch(...){}
    }
}

void JavaException::report(JNIEnv* env) const{
    if(*this){
        JniLocalFrame __jniLocalFrame(env, 200);
        try{
            Java::QtJambi::ExceptionUtility::reportException(env, nullptr, throwable(env));
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
    return p && p->m_throwable;
}

bool JavaException::operator !() const Q_DECL_NOEXCEPT {
    return !(p && p->m_throwable);
}

jthrowable JavaException::throwable(JNIEnv* env) const{
    return p ? jthrowable(env->NewLocalRef(p->m_throwable)) : nullptr;
}

bool JavaException::isInstanceOf(JNIEnv* env, jclass exceptionType) const{
    return p && p->m_throwable && env->IsInstanceOf(p->m_throwable, exceptionType);
}

void JavaException::raise() const{
    if(*this){
#if defined(Q_OS_ANDROID)
        if(JniEnvironment env{200}){
            throw JavaException(env, throwable(env));
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
    if(*this){
        jthrowable t = throwable(env);
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

void JavaException::raiseIndexOutOfBoundsException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::IndexOutOfBoundsException::newInstance(env,jmessage);
    raiseThrowable( QTJAMBI_STACKTRACEINFO_DECL_USE(env, t) );
}

void JavaException::raiseIndexOutOfBoundsException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::IndexOutOfBoundsException::newInstance(env,jmessage);
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

void JavaException::raiseIndexOutOfBoundsException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL ){
    jstring jmessage = qtjambi_cast<jstring>(env, message);
    check(env);
    jthrowable t = Java::Runtime::IndexOutOfBoundsException::newInstance(env,jmessage);
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

JNIEnv *currentJNIEnvironment(bool& requiresDetach, bool initializeJavaThread = true);

struct ExceptionContainer{
    std::thread::id thread_id;
    JavaException exn;
    const char* methodName;
    ExceptionContainer() : thread_id(std::this_thread::get_id()), exn(), methodName(nullptr) {}
    ~ExceptionContainer(){
        if(exn){
            bool requiresDetach = false;
            if(JNIEnv *env = currentJNIEnvironment(requiresDetach, false)){
                JniLocalFrame frame(env, 200);
                try{
                    jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
                    Java::QtJambi::ExceptionUtility::reportException(env, msg, exn.throwable(env));
                }catch(const JavaException& _exn){
                    if(methodName){
                        printf("An exception occured in %s: %s\n", methodName, exn.what());
                    }else{
                        printf("An exception occured: %s\n", exn.what());
                    }
                    printf("An exception occured in ExceptionUtility.reportException(...): %s\n", _exn.what());
                }
                JavaExceptionPrivate::clearException(env, exn);
            }else if(methodName){
                std::ostringstream oss;
                oss << thread_id;
                std::string thread_id_str = oss.str();
                qCWarning(DebugAPI::internalCategory, "An exception occured in thread %s in %s: %s", thread_id_str.c_str(), methodName, exn.what());
            }else{
                std::ostringstream oss;
                oss << thread_id;
                std::string thread_id_str = oss.str();
                qCWarning(DebugAPI::internalCategory, "An exception occured in thread %s: %s", thread_id_str.c_str(), exn.what());
            }
            if(requiresDetach)
                EventDispatcherCheck::detach();
        }
    }
    // QThreadStorage is needed because thread_local is deleted after JNI's thread detach and thus cannot treat java exceptions
    static QThreadStorage<ExceptionContainer> exception;
};
QThreadStorage<ExceptionContainer> ExceptionContainer::exception;

struct ExceptionHandler{
    enum State : quint8{
        None = 0x0,
        Blocking = 0x1,
        Reraise = 0x2,
        Keep = 0x4
    };
    ExceptionHandler() : env(nullptr), blocking(false), reraise(false), hasException(false) {
    }
    JNIEnv *env;
    uint blocking : 1;
    uint reraise : 1;
    uint keep : 1;
    uint hasException : 1;
    static thread_local ExceptionHandler instance;
};

thread_local ExceptionHandler ExceptionHandler::instance;

QtJambiExceptionHandler::QtJambiExceptionHandler()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    if(exceptionHandler.blocking){
        data |= ExceptionHandler::Blocking;
    }
    if(exceptionHandler.reraise){
        data |= ExceptionHandler::Reraise;
    }
    if(exceptionHandler.keep){
        data |= ExceptionHandler::Keep;
    }
    exceptionHandler.blocking = false;
}

QtJambiExceptionHandler::~QtJambiExceptionHandler(){
    ExceptionHandler::instance.blocking = data;
}

void QtJambiExceptionHandler::handle(JNIEnv *env, const JavaException& exn, const char* methodName){
    if(data){
        bool requiresDetach = false;
        ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
        exceptionHandler.hasException = true;
        ExceptionContainer& exceptionContainer = ExceptionContainer::exception.localData();
        if(exceptionContainer.exn){
            if(!env){
                env = currentJNIEnvironment(requiresDetach);
                if(!env){
                    if(methodName){
                        qCWarning(DebugAPI::internalCategory, "An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qCWarning(DebugAPI::internalCategory, "An exception occured: %s", exn.what());
                    }
                    return;
                }else{
                    JniLocalFrame __jniLocalFrame(env, 200);
                    exceptionContainer.exn.addSuppressed(env, exn);
                }
            }else{
                exceptionContainer.exn.addSuppressed(env, exn);
            }
        }else{
            exceptionContainer.exn = exn;
            exceptionContainer.methodName = methodName;
        }
        if(requiresDetach)
            EventDispatcherCheck::detach();
    }else if(exn){
        throw JavaException(exn);//JavaException(env, exn.object());
    }
}

QtJambiExceptionInhibitor::QtJambiExceptionInhibitor()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    if(exceptionHandler.blocking){
        data |= ExceptionHandler::Blocking;
    }
    if(exceptionHandler.reraise){
        data |= ExceptionHandler::Reraise;
    }
    if(exceptionHandler.keep){
        data |= ExceptionHandler::Keep;
    }
    exceptionHandler.blocking = false;
}

QtJambiExceptionInhibitor::~QtJambiExceptionInhibitor(){
    ExceptionHandler::instance.blocking = (data & ExceptionHandler::Blocking);
}

void QtJambiExceptionInhibitor::handle(JNIEnv *env, const JavaException& exn, const char* methodName){
    bool requiresDetach = false;
    if(!(data & ExceptionHandler::Blocking) || (data & ExceptionHandler::Reraise)){
        ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
        exceptionHandler.hasException = true;
        ExceptionContainer& exceptionContainer = ExceptionContainer::exception.localData();
        if(exceptionContainer.exn){
            if(!env){
                env = currentJNIEnvironment(requiresDetach);
                if(!env){
                    if(methodName){
                        qCWarning(DebugAPI::internalCategory, "An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qCWarning(DebugAPI::internalCategory, "An exception occured: %s", exn.what());
                    }
                    return;
                }else{
                    JniLocalFrame __jniLocalFrame(env, 200);
                    exceptionContainer.exn.addSuppressed(env, exn);
                }
            }else{
                exceptionContainer.exn.addSuppressed(env, exn);
            }
        }else{
            exceptionContainer.exn = exn;
            exceptionContainer.methodName = methodName;
        }
    }else{
        if(!env){
            env = currentJNIEnvironment(requiresDetach);
            if(!env){
                if(methodName){
                    qCWarning(DebugAPI::internalCategory, "An exception occured in %s: %s", methodName, exn.what());
                }else{
                    qCWarning(DebugAPI::internalCategory, "An exception occured: %s", exn.what());
                }
                return;
            }
        }
        JniLocalFrame __jniLocalFrame(env, 200);
        try{
            jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
            Java::QtJambi::ExceptionUtility::reportException(env, msg, exn.throwable(env));
        }catch(const JavaException& _exn){
            if(methodName){
                printf("An exception occured in %s: %s\n", methodName, exn.what());
            }else{
                printf("An exception occured: %s\n", exn.what());
            }
            printf("An exception occured in ExceptionUtility.reportException(...): %s\n", _exn.what());
        }
    }
    if(requiresDetach)
        EventDispatcherCheck::detach();
}

QtJambiExceptionBlocker::QtJambiExceptionBlocker()
    : data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    if(exceptionHandler.blocking){
        data |= ExceptionHandler::Blocking;
    }
    //if(exceptionHandler.reraise){
    //    data |= ExceptionHandler::Reraise;
    //}
    //exceptionHandler.reraise = false;
    exceptionHandler.blocking = true;
}

QtJambiExceptionBlocker::~QtJambiExceptionBlocker(){
    ExceptionHandler::instance.blocking = (data & ExceptionHandler::Blocking);
    ExceptionHandler::instance.reraise = (data & ExceptionHandler::Reraise);
    ExceptionHandler::instance.keep = (data & ExceptionHandler::Keep);
}

void QtJambiExceptionBlocker::release(JNIEnv *env){
    bool requiresDetach = false;
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    if(exceptionHandler.hasException && !exceptionHandler.keep){
        ExceptionContainer& exceptionContainer = ExceptionContainer::exception.localData();
        JavaException exn = exceptionContainer.exn;
        exceptionHandler.hasException = false;
        exceptionContainer.exn = JavaException();
        if(exceptionHandler.reraise/*(data & ExceptionHandler::Reraise)*/){
            exceptionContainer.methodName = nullptr;
            if(exn)
                throw exn;
        }else{
            const char* methodName = exceptionContainer.methodName;
            exceptionContainer.methodName = nullptr;
            if(!env){
                env = currentJNIEnvironment(requiresDetach);
                if(!env){
                    if(methodName){
                        qCWarning(DebugAPI::internalCategory, "An exception occured in %s: %s", methodName, exn.what());
                    }else{
                        qCWarning(DebugAPI::internalCategory, "An exception occured: %s", exn.what());
                    }
                    return;
                }
            }
            JniLocalFrame __jniLocalFrame(env, 200);
            try{
                jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
                Java::QtJambi::ExceptionUtility::reportException(env, msg, exn.throwable(env));
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
    if(requiresDetach)
        EventDispatcherCheck::detach();
}

QtJambiExceptionRaiser::QtJambiExceptionRaiser()
    : data(0) {
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    data = exceptionHandler.reraise;
    exceptionHandler.reraise = true;
}

QtJambiExceptionRaiser::~QtJambiExceptionRaiser(){
    ExceptionHandler::instance.reraise = data;
}

void QtJambiExceptionRaiser::raise(JNIEnv *){
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    if(exceptionHandler.hasException){
        ExceptionContainer& exceptionContainer = ExceptionContainer::exception.localData();
        JavaException exn = exceptionContainer.exn;
        exceptionHandler.hasException = false;
        exceptionContainer.exn = JavaException();
        if(exn)
            throw exn;
    }
}

QtJambiExceptionUnraiser::QtJambiExceptionUnraiser()
    : data(0) {
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    data = exceptionHandler.reraise;
    exceptionHandler.reraise = false;
    exceptionHandler.keep = data;
}

QtJambiExceptionUnraiser::~QtJambiExceptionUnraiser(){
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    exceptionHandler.reraise = data;
    exceptionHandler.keep = false;
}

JNIEnv* threadLocalEnvironment(bool& requiresDetach, ExceptionHandler& exceptionHandler){
    static ResettableBoolFlag useFastEnv("io.qt.experimental.fast-jni-for-overrides");
    if(useFastEnv){
        if(!exceptionHandler.env)
            exceptionHandler.env = currentJNIEnvironment(requiresDetach);
        return exceptionHandler.env;
    }else{
        return currentJNIEnvironment(requiresDetach);
    }
}

JniEnvironmentExceptionHandler::JniEnvironmentExceptionHandler(int capacity)
    : JniEnvironment(false),
    data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    if(exceptionHandler.blocking){
        data |= ExceptionHandler::Blocking;
    }
    if(exceptionHandler.reraise){
        data |= ExceptionHandler::Reraise;
    }
    if(exceptionHandler.keep){
        data |= ExceptionHandler::Keep;
    }
    exceptionHandler.blocking = false;
    bool requiresDetach = false;
    JNIEnv* env = threadLocalEnvironment(requiresDetach, exceptionHandler);
    initialize(env, requiresDetach, capacity);
}

JniEnvironmentExceptionHandler::~JniEnvironmentExceptionHandler(){
    ExceptionHandler::instance.blocking = data;
}

void JniEnvironmentExceptionHandler::handleException(const JavaException& exn, const char* methodName){
    if(data){
        ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
        exceptionHandler.hasException = true;
        ExceptionContainer& exceptionContainer = ExceptionContainer::exception.localData();
        if(exceptionContainer.exn){
            exceptionContainer.exn.addSuppressed(m_env, exn);
        }else{
            exceptionContainer.exn = exn;
            exceptionContainer.methodName = methodName;
        }
    }else if(exn){
        throw JavaException(exn);//JavaException(env, exn.object());
    }
}

JniEnvironmentExceptionInhibitor::JniEnvironmentExceptionInhibitor(int capacity)
    : JniEnvironment(false),
    data(0)
{
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    if(exceptionHandler.blocking){
        data |= ExceptionHandler::Blocking;
    }
    if(exceptionHandler.reraise){
        data |= ExceptionHandler::Reraise;
    }
    if(exceptionHandler.keep){
        data |= ExceptionHandler::Keep;
    }
    exceptionHandler.blocking = false;
    bool requiresDetach = false;
    JNIEnv* env = threadLocalEnvironment(requiresDetach, exceptionHandler);
    initialize(env, requiresDetach, capacity);
}

JniEnvironmentExceptionInhibitor::~JniEnvironmentExceptionInhibitor(){
    ExceptionHandler::instance.blocking = (data & ExceptionHandler::Blocking);
}

void JniEnvironmentExceptionInhibitor::handleException(const JavaException& exn, const char* methodName){
    if(!(data & ExceptionHandler::Blocking) || (data & ExceptionHandler::Reraise)){
        ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
        exceptionHandler.hasException = true;
        ExceptionContainer& exceptionContainer = ExceptionContainer::exception.localData();
        if(exceptionContainer.exn){
            exceptionContainer.exn.addSuppressed(m_env, exn);
        }else{
            exceptionContainer.exn = exn;
            exceptionContainer.methodName = methodName;
        }
    }else{
        JniLocalFrame __jniLocalFrame(m_env, 200);
        try{
            jstring msg = methodName ? m_env->NewStringUTF(methodName) : nullptr;
            Java::QtJambi::ExceptionUtility::reportException(m_env, msg, exn.throwable(m_env));
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

void releaseExceptionImpl(JNIEnv *env){
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    if(exceptionHandler.hasException && !exceptionHandler.keep){
        ExceptionContainer& exceptionContainer = ExceptionContainer::exception.localData();
        JavaException exn = exceptionContainer.exn;
        exceptionHandler.hasException = false;
        exceptionContainer.exn = JavaException();
        if(exceptionHandler.reraise/*(data & ExceptionHandler::Reraise)*/){
            exceptionContainer.methodName = nullptr;
            if(exn)
                throw exn;
        }else{
            const char* methodName = exceptionContainer.methodName;
            exceptionContainer.methodName = nullptr;
            JniLocalFrame __jniLocalFrame(env, 200);
            try{
                jstring msg = methodName ? env->NewStringUTF(methodName) : nullptr;
                Java::QtJambi::ExceptionUtility::reportException(env, msg, exn.throwable(env));
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

void JniEnvironmentExceptionHandlerAndBlocker::releaseException(){
    releaseExceptionImpl(environment());
}

JniEnvironmentExceptionHandlerAndBlocker::~JniEnvironmentExceptionHandlerAndBlocker(){
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    exceptionHandler.blocking = (data & ExceptionHandler::Blocking);
    exceptionHandler.reraise = (data & ExceptionHandler::Reraise);
    exceptionHandler.keep = (data & ExceptionHandler::Keep);
}

void JniEnvironmentExceptionInhibitorAndBlocker::releaseException(){
    releaseExceptionImpl(environment());
}

JniEnvironmentExceptionInhibitorAndBlocker::~JniEnvironmentExceptionInhibitorAndBlocker(){
    ExceptionHandler& exceptionHandler = ExceptionHandler::instance;
    exceptionHandler.blocking = (data & ExceptionHandler::Blocking);
    exceptionHandler.reraise = (data & ExceptionHandler::Reraise);
    exceptionHandler.keep = (data & ExceptionHandler::Keep);
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
    if(newInstance)
        throw JavaException(env, newInstance);
}

#ifdef QTJAMBI_STACKTRACE
void QtJambiPrivate::raiseJavaException(JNIEnv* env, jthrowable newInstance, const char *methodName, const char *fileName, int lineNumber)
{
    if(newInstance){
        jstring jmethodName = methodName ? env->NewStringUTF(methodName) : nullptr;
        jstring jfileName = fileName ? env->NewStringUTF(fileName) : nullptr;
        try{
            Java::QtJambi::ExceptionUtility::extendStackTrace(env, newInstance, jmethodName, jfileName, lineNumber);
        }catch(const JavaException& exn){ exn.report(env); }
        throw JavaException(env, newInstance);
    }
}
#endif

void QtJambiPrivate::javaExceptionCheck(JNIEnv* env){
    JavaException::check(env);
}

void QtJambiPrivate::javaInstanceCheck(JNIEnv* env, jobject object, jclass class_ref, bool isMemberFunction, const char* name){
    JavaException::check(env);
    if(env->IsSameObject(object, nullptr)){
        if(isMemberFunction)
            JavaException::raiseNullPointerException(env, QLatin1String("Cannot invoke member function %1.%2(...) on null.").arg(QtJambiAPI::getClassName(env, class_ref), name) QTJAMBI_STACKTRACEINFO );
        else
            JavaException::raiseNullPointerException(env, QLatin1String("Cannot access member field %1.%2 on null.").arg(QtJambiAPI::getClassName(env, class_ref), name) QTJAMBI_STACKTRACEINFO );
    }
    if(class_ref && !env->IsInstanceOf(object, class_ref)){
        if(isMemberFunction)
            JavaException::raiseIllegalArgumentException(env, QLatin1String("Cannot invoke member function %1.%2(...) on object of type %3.").arg(QtJambiAPI::getClassName(env, class_ref), name, QtJambiAPI::getObjectClassName(env, object)) QTJAMBI_STACKTRACEINFO );
        else
            JavaException::raiseIllegalArgumentException(env, QLatin1String("Cannot access member field %1.%2 on object of type %3.").arg(QtJambiAPI::getClassName(env, class_ref), name, QtJambiAPI::getObjectClassName(env, object)) QTJAMBI_STACKTRACEINFO );
    }
}
