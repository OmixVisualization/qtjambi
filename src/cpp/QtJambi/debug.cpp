/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qtjambilink_p.h"
#include "debugapi.h"
#include "java_p.h"

#include <QtCore/QLoggingCategory>

#ifndef QTJAMBI_NO_METHOD_TRACE

const char * adaptFile(const char *file){
#if defined(Q_OS_WIN)
#define PATHSEP '\\'
#else
#define PATHSEP '/'
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QByteArrayView _file(file);
    auto idx = _file.lastIndexOf(PATHSEP);
    if(idx>0)
        idx = _file.lastIndexOf(PATHSEP, idx-1);
    if(idx>0)
        file += idx+1;
#endif
    return file;
}

namespace DebugAPI{

Q_LOGGING_CATEGORY(debugAPICategory, "io.qtjambi.debugapi")
Q_LOGGING_CATEGORY(debugAPIInternalMethodsCategory, "io.qtjambi.debugapi.internal")
Q_LOGGING_CATEGORY(debugAPIJavaOverloadsCategory, "io.qtjambi.debugapi.java-overloads")
Q_LOGGING_CATEGORY(debugAPINativeCallsCategory, "io.qtjambi.debugapi.native-calls")

bool enabledMethodTracePrints(){
    static bool b = []()->bool{
        if(JniEnvironment env{200})
            return Java::Runtime::Boolean::getBoolean(env, env->NewStringUTF("io.qt.enable-method-logs")) || qEnvironmentVariableIsSet("QTJAMBI_DEBUG_TRACE");
        else return qEnvironmentVariableIsSet("QTJAMBI_DEBUG_TRACE");
    }();
    return b;
}

class MethodPrintPrivate{
    MethodPrint::Type m_callType;
    const void* m_pointer;
    const char* m_method;
    const char* m_file;
    int m_line;
    const char* m_function;
    MethodPrintPrivate(MethodPrint::Type callType, const void* pointer, const char* method, const char* file, int line, const char *function) :
        m_callType(callType),
        m_pointer(pointer),
        m_method(method),
        m_file(file),
        m_line(line),
        m_function(function){}
public:
    ~MethodPrintPrivate(){
        methodPrint(false, m_callType, m_pointer, m_method, m_file, m_line, m_function);
    }
    static MethodPrintPrivate* create(MethodPrint::Type callType, const void* pointer, const char* method, const char* file, int line, const char *function){
        if(enabledMethodTracePrints() && callType!=MethodPrint::Disabled){
            methodPrint(true, callType, pointer, method, file, line, function);
            return new MethodPrintPrivate(callType, pointer, method, file, line, function);
        }else{
            return nullptr;
        }
    }
    static void methodPrint(bool isEnter, MethodPrint::Type callType, const void* pointer, const char *method, const char *file, int line, const char *function)
    {
        if(pointer){
            switch(callType){
            case MethodPrint::JavaToNativeCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "ENTER: Java calling native method " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "LEAVE: Java calling native method " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::NativeToJavaCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "ENTER: Shell (object: " << pointer << ") calling java method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "LEAVE: Shell (object: " << pointer << ") calling java method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::LibraryInitialization:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Initializing " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Initializing " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::Internal:{
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            }
            case MethodPrint::ConstructorCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Constructor call " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Constructor call " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::DestructorCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Destuctor call " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Destuctor call " << method << " on object " << pointer << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            default:break;
            }
        }else{
            switch(callType){
            case MethodPrint::JavaToNativeCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "ENTER: Java calling native method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "LEAVE: Java calling native method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::NativeToJavaCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "ENTER: Shell calling java method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "LEAVE: Shell calling java method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::LibraryInitialization:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Initializing " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Initializing " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::Internal:{
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            }
            case MethodPrint::ConstructorCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Constructor call " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Constructor call " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::DestructorCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Destuctor call " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Destuctor call " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            default:break;
            }
        }
    }
    static void printImpl(const char *message, const char *file, int line, const char *function)
    {
        QMessageLogger(adaptFile(file), line, function).debug(debugAPICategory).nospace() << message << " in thread tid=" << quintptr(QThread::currentThreadId());
    }
    static void printImpl(const QString& message, const char *file, int line, const char *function)
    {
        QMessageLogger(adaptFile(file), line, function).debug(debugAPICategory).nospace().noquote() << message << " in thread tid=" << quintptr(QThread::currentThreadId());
    }
    static void print(const char *message, const char *file, int line, const char *function)
    {
        if (enabledMethodTracePrints()) {
            printImpl(message, file, line, function);
        }
    }
    static void printArgs(const char *file, int line, const char *function, const char *format,...)
    {
        if (enabledMethodTracePrints()) {
            printImpl(QString::asprintf(format), file, line, function);
        }
    }
};

void print(const char *message, const char *file, int line, const char *function)
{
    MethodPrintPrivate::print(message, file, line, function);
}

void printArgs(const char *file, int line, const char *function, const char *format,...){
    MethodPrintPrivate::printArgs(file, line, function, format);
}

MethodPrint::MethodPrint(MethodPrint::Type callType, const void* pointer, const char* method, const char* file, int line, const char *function) :
    d(MethodPrintPrivate::create(callType, pointer, method, file, line, function)) {
}

MethodPrint::~MethodPrint(){
    delete d;
}

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
void printWithType(bool isEnter, MethodPrint::Type callType, const void* pointer, const char *method, const char * typeName, const char *file, int line, const char *function)
{
    if(typeName){
        if(pointer){
            switch(callType){
            case MethodPrint::NativeToJavaCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "ENTER: Shell (object: " << pointer << " , type: " << typeName << ") calling java method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "LEAVE: Shell (object: " << pointer << " , type: " << typeName << ") calling java method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::Internal:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: " << method << " on object " << pointer << " of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: " << method << " on object " << pointer << " of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::ConstructorCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Constructor call " << method << " on object " << pointer << " of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Constructor call " << method << " on object " << pointer << " of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::DestructorCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Destuctor call " << method << " on object " << pointer << " of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Destuctor call " << method << " on object " << pointer << " of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            default:
                break;
            }
        }else{
            switch(callType){
            case MethodPrint::NativeToJavaCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "ENTER: Shell (type: " << typeName << ") calling java method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIJavaOverloadsCategory).nospace().noquote() << "LEAVE: Shell (type: " << typeName << ") calling java method " << method << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::Internal:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: " << method << " for object of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: " << method << " for object of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::ConstructorCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Constructor call " << method << " on object of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Constructor call " << method << " on object of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            case MethodPrint::DestructorCall:
                if(isEnter){
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "ENTER: Destuctor call " << method << " on object of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }else{
                    QMessageLogger(adaptFile(file), line, function).debug(debugAPIInternalMethodsCategory).nospace().noquote() << "LEAVE: Destuctor call " << method << " on object of type " << typeName << " in thread tid=" << quintptr(QThread::currentThreadId());
                }
                break;
            default:
                break;
            }
        }
    }else{
        MethodPrintPrivate::methodPrint(isEnter, callType, pointer, method, file, line, function);
    }
}

struct MethodPrintWithTypePrivate{
    MethodPrint::Type m_callType;
    const void* m_pointer;
    const char* m_method;
    const char* m_type;
    const char* m_file;
    int m_line;
    const char* m_function;
    MethodPrintWithTypePrivate(MethodPrint::Type callType, const void* pointer, const char* method, const char* type, const char* file, int line, const char *function) :
        m_callType(callType),
        m_pointer(pointer),
        m_method(method),
        m_type(type),
        m_file(file),
        m_line(line),
        m_function(function) {
    }
public:
    ~MethodPrintWithTypePrivate(){
        printWithType(false, m_callType, m_pointer, m_method, type_name, m_file, m_line, m_function);
    }
    static MethodPrintWithTypePrivate* create(MethodPrint::Type callType, const char* method, const QtJambiShell* shell, const char* file, int line, const char *function){
        if(enabledMethodTracePrints() && callType!=MethodPrint::Disabled){
            const char* type_name = nullptr;
            const void* pointer = nullptr;
            if(QSharedPointer<QtJambiLink> link = static_cast<const QtJambiShellImpl*>(shell)->link()){
                type_name = link->qtTypeName();
                pointer = link->pointer();
            }
            printWithType(true, callType, pointer, method, type_name, m_file, m_line, m_function);
            return new MethodPrintWithTypePrivate(callType, pointer, method, type_name, file, line, function);
        }else{
            return nullptr;
        }
    }
    static MethodPrintWithTypePrivate* create(MethodPrint::Type callType, const void* pointer, const char* method, const char* type_name, const char* file, int line, const char *function){
        if(enabledMethodTracePrints() && callType!=MethodPrint::Disabled){
            printWithType(true, callType, pointer, method, type_name, m_file, m_line, m_function);
            return new MethodPrintWithTypePrivate(callType, pointer, method, type_name, file, line, function);
        }else{
            return nullptr;
        }
    }
};

MethodPrintWithType::MethodPrintWithType(MethodPrint::Type callType, const char* method, const QtJambiShell* shell, const char* file, int line, const char *function) :
    d(MethodPrintWithTypePrivate::create(callType, method, shell, file, line, function)){}
}

MethodPrintWithType::MethodPrintWithType(MethodPrint::Type callType, const void* pointer, const char* method, const char* type_name, const char* file, int line, const char *function) :
    d(MethodPrintWithTypePrivate::create(callType, pointer, method, type_name, file, line, function)){}
}

MethodPrintWithType::~MethodPrintWithType(){
    delete d;
}

#endif // defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)

}

#endif // QTJAMBI_NO_METHOD_TRACE

void qtjambi_assert(const char *assertion, const char *file, int line, const char *function){
#ifndef QTJAMBI_NO_METHOD_TRACE
    DebugAPI::print(assertion, file, line, function);
#endif
    qt_assert(assertion, file, line);
}
