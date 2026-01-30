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

#if !defined(QTJAMBI_JAVAENVIRONMENT_H) && !defined(QTJAMBI_GENERATOR_RUNNING)
#define QTJAMBI_JAVAENVIRONMENT_H

#include "global.h"
#include "scope.h"
#include "exception.h"

enum class jValueType {
    z = 1,
    b,
    c,
    s,
    i,
    j,
    f,
    d,
    l,
};

class JniEnvironment{
public:
    QTJAMBI_EXPORT JniEnvironment(int capacity = 0);
    QTJAMBI_EXPORT JniEnvironment(JniEnvironment&& other);
    QTJAMBI_EXPORT ~JniEnvironment();
    QTJAMBI_EXPORT operator bool() const;
    QTJAMBI_EXPORT operator JNIEnv *() const;
    QTJAMBI_EXPORT JNIEnv * environment() const;
    QTJAMBI_EXPORT JNIEnv *operator->() const;
#ifdef QTJAMBI_STACKTRACE
    QTJAMBI_EXPORT void checkException();
#endif
    QTJAMBI_EXPORT void checkException(QTJAMBI_STACKTRACEINFO_DECL_NOENV);
protected:
    QTJAMBI_EXPORT JniEnvironment(bool, int capacity);
    JniEnvironment(int, int capacity);
    void initialize(JNIEnv *env, bool requiresDetach, int capacity);
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    JNIEnv *m_env = nullptr;
    quint8 m_flags = 0;
};

class JavaException;
class QtJambiShellInterface;

class QTJAMBI_EXPORT JniEnvironmentExceptionHandler : public JniEnvironment{
public:
    JniEnvironmentExceptionHandler(int capacity = 0);
    ~JniEnvironmentExceptionHandler();
    void handleException(const JavaException& exn, const char* methodName);
    void handleException(const JavaException& exn, const QObject* object, const char* methodName);
//    void handleException(const JavaException& exn, const QtJambiShellInterface* object, const char* methodName);
    void handleException(const JavaException& exn, const void* object, const char* methodName);
private:
    quint8 data;
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    Q_DISABLE_COPY_MOVE(JniEnvironmentExceptionHandler)
    friend class JniEnvironmentExceptionHandlerAndBlocker;
};

class QTJAMBI_EXPORT JniEnvironmentExceptionInhibitor : public JniEnvironment{
public:
    JniEnvironmentExceptionInhibitor(int capacity = 0);
    ~JniEnvironmentExceptionInhibitor();
    void handleException(const JavaException& exn, const char* methodName);
    void handleException(const JavaException& exn, const QObject* object, const char* methodName);
//    void handleException(const JavaException& exn, const QtJambiShellInterface* object, const char* methodName);
    void handleException(const JavaException& exn, const void* object, const char* methodName);
private:
    quint8 data;
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    Q_DISABLE_COPY_MOVE(JniEnvironmentExceptionInhibitor)
    friend class JniEnvironmentExceptionInhibitorAndBlocker;
};

class QTJAMBI_EXPORT JniEnvironmentExceptionHandlerAndBlocker : public JniEnvironmentExceptionHandler{
public:
    using JniEnvironmentExceptionHandler::JniEnvironmentExceptionHandler;
    ~JniEnvironmentExceptionHandlerAndBlocker();
    void releaseException();
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

class QTJAMBI_EXPORT JniEnvironmentExceptionInhibitorAndBlocker : public JniEnvironmentExceptionInhibitor{
public:
    using JniEnvironmentExceptionInhibitor::JniEnvironmentExceptionInhibitor;
    ~JniEnvironmentExceptionInhibitorAndBlocker();
    void releaseException();
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
};

class QtJambiShell;

class QTJAMBI_EXPORT JniEnvironmentScope : public JniEnvironment{
public:
    JniEnvironmentScope(QtJambiShell* shell, int capacity = 0);
    ~JniEnvironmentScope();
    QtJambiScope& scope();
    jobject getJavaObjectLocalRef();
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    QtJambiScope m_scope;
    Q_DISABLE_COPY_MOVE(JniEnvironmentScope)
};

class QTJAMBI_EXPORT JniEnvironmentScopeExceptionHandler : public JniEnvironmentExceptionHandler{
public:
    JniEnvironmentScopeExceptionHandler(QtJambiShell* shell, int capacity = 0);
    ~JniEnvironmentScopeExceptionHandler();
    QtJambiScope& scope();
    jobject getJavaObjectLocalRef();
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    QtJambiScope m_scope;
    Q_DISABLE_COPY_MOVE(JniEnvironmentScopeExceptionHandler)
};

class QTJAMBI_EXPORT JniEnvironmentScopeExceptionInhibitor : public JniEnvironmentExceptionInhibitor{
public:
    JniEnvironmentScopeExceptionInhibitor(QtJambiShell* shell, int capacity = 0);
    ~JniEnvironmentScopeExceptionInhibitor();
    QtJambiScope& scope();
    jobject getJavaObjectLocalRef();
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    QtJambiScope m_scope;
    Q_DISABLE_COPY_MOVE(JniEnvironmentScopeExceptionInhibitor)
};

class QTJAMBI_EXPORT JniEnvironmentScopeExceptionHandlerAndBlocker : public JniEnvironmentExceptionHandlerAndBlocker{
public:
    JniEnvironmentScopeExceptionHandlerAndBlocker(QtJambiShell* shell, int capacity = 0);
    ~JniEnvironmentScopeExceptionHandlerAndBlocker();
    QtJambiScope& scope();
    jobject getJavaObjectLocalRef();
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    QtJambiScope m_scope;
    Q_DISABLE_COPY_MOVE(JniEnvironmentScopeExceptionHandlerAndBlocker)
};

class QTJAMBI_EXPORT JniEnvironmentScopeExceptionInhibitorAndBlocker : public JniEnvironmentExceptionInhibitorAndBlocker{
public:
    JniEnvironmentScopeExceptionInhibitorAndBlocker(QtJambiShell* shell, int capacity = 0);
    ~JniEnvironmentScopeExceptionInhibitorAndBlocker();
    QtJambiScope& scope();
    jobject getJavaObjectLocalRef();
private:
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    QtJambiScope m_scope;
    Q_DISABLE_COPY_MOVE(JniEnvironmentScopeExceptionInhibitorAndBlocker)
};

class QTJAMBI_EXPORT JniLocalFrame{
public:
    JniLocalFrame(JNIEnv *env, int capacity);
    ~JniLocalFrame();
private:
    JNIEnv *m_env;
    void* operator new(size_t) = delete;
    void* operator new(size_t,size_t) = delete;
    void* operator new[](size_t) = delete;
    Q_DISABLE_COPY_MOVE(JniLocalFrame)
};

#define QTJAMBI_JNI_LOCAL_FRAME(env, capacity)\
    JniLocalFrame __jniLocalFrame(env, capacity)

#endif // QTJAMBI_JAVAENVIRONMENT_H
