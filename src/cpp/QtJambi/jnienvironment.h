/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

class QTJAMBI_EXPORT JniEnvironment{
public:
    JniEnvironment(int capacity = 0);
    JniEnvironment(JniEnvironment&& other);
    ~JniEnvironment();
    operator bool() const;
    operator JNIEnv *() const;
    JNIEnv * environment() const;
    JNIEnv *operator->() const;
#ifdef QTJAMBI_STACKTRACE
    void checkException();
#endif
    void checkException(QTJAMBI_STACKTRACEINFO_DECL_NOENV);
protected:
    JniEnvironment(bool,int capacity);
private:
    JniEnvironment(bool);
    void initialize(JNIEnv *env, int capacity);
    JNIEnv *m_env;
    bool m_hasLocalFrames;
    friend class JniEnvironmentExceptionHandler;
    friend class JniEnvironmentExceptionInhibitor;
};

class JavaException;

class QTJAMBI_EXPORT JniEnvironmentExceptionHandler : public JniEnvironment{
public:
    JniEnvironmentExceptionHandler(int capacity = 0);
    ~JniEnvironmentExceptionHandler();
    void handleException(const JavaException& exn, const char* methodName);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(JniEnvironmentExceptionHandler)
    friend class JniEnvironmentExceptionHandlerAndBlocker;
};

class QTJAMBI_EXPORT JniEnvironmentExceptionInhibitor : public JniEnvironment{
public:
    JniEnvironmentExceptionInhibitor(int capacity = 0);
    ~JniEnvironmentExceptionInhibitor();
    void handleException(const JavaException& exn, const char* methodName);
private:
    quint8 data;
    Q_DISABLE_COPY_MOVE(JniEnvironmentExceptionInhibitor)
    friend class JniEnvironmentExceptionInhibitorAndBlocker;
};

class QTJAMBI_EXPORT JniEnvironmentExceptionHandlerAndBlocker : public JniEnvironmentExceptionHandler{
public:
    using JniEnvironmentExceptionHandler::JniEnvironmentExceptionHandler;
    ~JniEnvironmentExceptionHandlerAndBlocker();
    void releaseException();
};

class QTJAMBI_EXPORT JniEnvironmentExceptionInhibitorAndBlocker : public JniEnvironmentExceptionInhibitor{
public:
    using JniEnvironmentExceptionInhibitor::JniEnvironmentExceptionInhibitor;
    ~JniEnvironmentExceptionInhibitorAndBlocker();
    void releaseException();
};

class QtJambiShell;

class QTJAMBI_EXPORT JniEnvironmentScope : public JniEnvironment{
public:
    JniEnvironmentScope(QtJambiShell* shell, int capacity = 0);
    ~JniEnvironmentScope();
    QtJambiScope& scope();
    jobject getJavaObjectLocalRef();
private:
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
    QtJambiScope m_scope;
    Q_DISABLE_COPY_MOVE(JniEnvironmentScopeExceptionInhibitorAndBlocker)
};

class QTJAMBI_EXPORT JniLocalFrame{
public:
    JniLocalFrame(JNIEnv *env, int capacity);
    ~JniLocalFrame();
private:
    JNIEnv *m_env;
    Q_DISABLE_COPY_MOVE(JniLocalFrame)
};

#define QTJAMBI_JNI_LOCAL_FRAME(env, capacity)\
    JniLocalFrame __jniLocalFrame(env, capacity)

#endif // QTJAMBI_JAVAENVIRONMENT_H
