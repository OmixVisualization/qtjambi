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

#if !defined(QTJAMBI_EXCEPTION_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_EXCEPTION_H

#include <QtCore/QException>
#include <QtCore/QSharedDataPointer>
#include <QtCore/QThread>
#include "global.h"
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore/QAnyStringView>
#endif

#ifdef QT_DEBUG
#ifndef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACE
#endif
#endif

#ifndef QTJAMBI_STACKTRACEINFO
#ifdef QTJAMBI_STACKTRACE
#define QTJAMBI_STACKTRACEINFO , __FUNCTION__, __FILE__, __LINE__
#define QTJAMBI_STACKTRACEINFO_ENV(env) env QTJAMBI_STACKTRACEINFO
#define QTJAMBI_STACKTRACEINFO_DECL , const char *methodName, const char *fileName, int lineNumber
#else
#define QTJAMBI_STACKTRACEINFO
#define QTJAMBI_STACKTRACEINFO_ENV(env)
#define QTJAMBI_STACKTRACEINFO_DECL
#endif
#endif

class JavaExceptionPrivate;

class QTJAMBI_EXPORT JavaException : public QException
{
public:
    JavaException() Q_DECL_NOEXCEPT;
    JavaException(JNIEnv *env, jthrowable obj);
    JavaException(const JavaException& copy) Q_DECL_NOEXCEPT;
    JavaException(JavaException&& copy) Q_DECL_NOEXCEPT;
    ~JavaException() Q_DECL_NOEXCEPT override;
    void raiseInJava(JNIEnv* env) const;
    void raise() const override;
    void report(JNIEnv* env) const;
    JavaException* clone() const override;
#ifdef QTJAMBI_STACKTRACE
    void raise(JNIEnv* env, const char *methodName, const char *fileName, int lineNumber) const;
#endif
    void addSuppressed(JNIEnv* env, const JavaException& exn) const;
    jthrowable object() const;
    JavaException& operator =(const JavaException& other) Q_DECL_NOEXCEPT;
    JavaException& operator =(JavaException&& other) Q_DECL_NOEXCEPT;
    operator bool() const Q_DECL_NOEXCEPT;
    char const* what() const Q_DECL_NOEXCEPT override;

#ifdef QTJAMBI_STACKTRACE
    static void check(JNIEnv* env);
#endif
    static void check(JNIEnv* env QTJAMBI_STACKTRACEINFO_DECL );
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    static void raiseIllegalArgumentException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIllegalArgumentException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseNullPointerException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseNullPointerException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQNoImplementationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQNoImplementationException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseError(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseError(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseRuntimeException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseRuntimeException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseUnsupportedOperationException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseUnsupportedOperationException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIndexOutOfBoundsException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIndexOutOfBoundsException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQThreadAffinityException(JNIEnv* env, const char *message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3);
    static void raiseQThreadAffinityException(JNIEnv* env, QString&& message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3);
#else
    static void raiseIllegalArgumentException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseNullPointerException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQNoImplementationException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseError(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseRuntimeException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseUnsupportedOperationException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseIndexOutOfBoundsException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL );
    static void raiseQThreadAffinityException(JNIEnv* env, QAnyStringView message QTJAMBI_STACKTRACEINFO_DECL , jobject t1, QThread* t2, QThread* t3);
#endif
private:
    void update(JNIEnv *env);
    QSharedDataPointer<JavaExceptionPrivate> p;
};

#if defined(QTJAMBI_CENTRAL_TRY_CATCH)

namespace TryCatchWrapperAPI{

class TypedTrial{
public:
    template<typename Functor>
    TypedTrial(Functor&& fun)
        : m_functor(&fun),
          m_caller(&caller<Functor>){}
    void operator()();
private:
    template<typename Functor>
    static void caller(void* ptr){(*reinterpret_cast<Functor*>(ptr))();}
    void* m_functor;
    void(*m_caller)(void*);
};

class TypedCatcher{
public:
    template<typename Functor>
    TypedCatcher(Functor&& fun)
        : m_functor(&fun),
          m_caller(&caller<Functor>){}
    void operator()(const JavaException& exn);
private:
    template<typename Functor>
    static void caller(void* ptr, const JavaException& exn){(*reinterpret_cast<Functor*>(ptr))(exn);}
    void* m_functor;
    void(*m_caller)(void*,const JavaException&);
};

QTJAMBI_EXPORT void tryCatch(TypedTrial&& fct, TypedCatcher&& handler);
QTJAMBI_EXPORT void tryCatchAny(TypedTrial&& fct, TypedTrial&& anyHandler);

}

#define QTJAMBI_TRY TryCatchWrapperAPI::tryCatch([&]()->void
#define QTJAMBI_TRY_ANY TryCatchWrapperAPI::tryCatchAny([&]()->void
#define QTJAMBI_TRY_RETURN(variable,value) variable = value;\
                                           return;
#define QTJAMBI_CATCH(EXN) , [&](EXN)->void
#define QTJAMBI_CATCH_ANY , [&]()->void
#define QTJAMBI_TRY_END );
#else
#define QTJAMBI_TRY try
#define QTJAMBI_TRY_ANY try
#define QTJAMBI_TRY_RETURN(variable,value) return value;
#define QTJAMBI_CATCH catch
#define QTJAMBI_CATCH_ANY catch(...)
#define QTJAMBI_TRY_END
#endif

class QTJAMBI_EXPORT QtJambiExceptionHandler{
public:
    QtJambiExceptionHandler();
    ~QtJambiExceptionHandler();
    void handle(JNIEnv *env, const JavaException& exn, const char* methodName);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionHandler)
};

class QTJAMBI_EXPORT QtJambiExceptionInhibitor{
public:
    QtJambiExceptionInhibitor();
    ~QtJambiExceptionInhibitor();
    void handle(JNIEnv *env, const JavaException& exn, const char* methodName);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionInhibitor)
};

class QTJAMBI_EXPORT QtJambiExceptionBlocker{
public:
    QtJambiExceptionBlocker();
    ~QtJambiExceptionBlocker();
    void release(JNIEnv *env);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionBlocker)
};

class QTJAMBI_EXPORT QtJambiExceptionRaiser{
public:
    QtJambiExceptionRaiser();
    ~QtJambiExceptionRaiser();
    void raise(JNIEnv *env);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(QtJambiExceptionRaiser)
};

#endif // QTJAMBI_EXCEPTION_H
