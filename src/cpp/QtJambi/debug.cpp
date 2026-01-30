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

#include "pch_p.h"

QT_WARNING_DISABLE_GCC("-Wformat-security")
QT_WARNING_DISABLE_CLANG("-Wformat-security")

#ifndef QTJAMBI_NO_METHOD_TRACE

const char * adaptFile(const char *file){
#if defined(Q_OS_WIN)
#define PATHSEP '\\'
#else
#define PATHSEP '/'
#endif
    if(file){
        QByteArrayView _file(file);
        auto idx = _file.lastIndexOf(PATHSEP);
        if(idx>0)
            idx = _file.lastIndexOf(PATHSEP, idx-1);
        if(idx>0)
            file += idx+1;
    }
    return file;
}

QDebug& operator<<(QDebug& debug, const DebugAPI::Printer& printer){
    printer(debug);
    return debug;
}

bool enabledMethodTracePrints(JNIEnv *env = nullptr){
    static ResettableBoolFlag b(env, "io.qt.enable-method-logs");
    static bool b2 = qEnvironmentVariableIsSet("QTJAMBI_DEBUG_TRACE");
    return b || b2;
}

bool enabledEventPrints(JNIEnv *env = nullptr){
    static ResettableBoolFlag b(env, "io.qt.enable-event-logs");
    return b;
}

namespace DebugAPI{

Q_LOGGING_CATEGORY(internalCategory, "io.qtjambi.internal")
Q_LOGGING_CATEGORY(debugAPICategory, "io.qtjambi.debugapi")
Q_LOGGING_CATEGORY(debugAPIInternalMethodsCategory, "io.qtjambi.debugapi.internal")
Q_LOGGING_CATEGORY(debugAPIJavaOverloadsCategory, "io.qtjambi.debugapi.java-overloads")
Q_LOGGING_CATEGORY(debugAPINativeCallsCategory, "io.qtjambi.debugapi.native-calls")
Q_LOGGING_CATEGORY(debugAPIConstructorCallsCategory, "io.qtjambi.debugapi.constructor-calls")
Q_LOGGING_CATEGORY(debugAPIDestructorCallsCategory, "io.qtjambi.debugapi.desctructor-calls")
Q_LOGGING_CATEGORY(debugAPIEventsCategory, "io.qtjambi.debugapi.events")
Q_LOGGING_CATEGORY(debugAPICleanupCallsCategory, "io.qtjambi.debugapi.cleanup-calls")

void initialize(){
    internalCategory();
    debugAPICategory();
    debugAPIInternalMethodsCategory();
    debugAPIJavaOverloadsCategory();
    debugAPINativeCallsCategory();
    debugAPIConstructorCallsCategory();
    debugAPIDestructorCallsCategory();
    debugAPIEventsCategory();
    debugAPICleanupCallsCategory();
}

/*
#if QT_VERSION < QT_VERSION_CHECK(6, 4, 0)
#define QTJAMBI_DEBUG_MESSAGE_LOGGER(category) \
for (bool qt_category_enabled = category().isDebugEnabled(); qt_category_enabled; qt_category_enabled = false) \
        QMessageLogger(adaptFile(file), line, function, category().categoryName()).debug().nospace().noquote()
#else
#define QTJAMBI_DEBUG_MESSAGE_LOGGER(category) \
for (QLoggingCategoryMacroHolder<QtDebugMsg> qt_category(category()); qt_category; qt_category.control = false) \
        QMessageLogger(adaptFile(file), line, function, qt_category.name()).debug().nospace().noquote()
#endif
*/
#define QTJAMBI_DEBUG_MESSAGE_LOGGER(category) QMessageLogger(adaptFile(file), line, function, category.categoryName()).debug().nospace().noquote()

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
#define OPTIONAL_LINK_NAME(ARG) ARG
#define OPTIONAL_LINK_NAME_K(ARG) ARG,
#else
#define OPTIONAL_LINK_NAME(ARG)
#define OPTIONAL_LINK_NAME_K(ARG)
#endif

class MethodPrintPrivate{
    enum Mode{
        Printable,
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
        MethodAndType,
#endif
        Message
    };
    const QLoggingCategory& m_category;
    MethodPrint::Type m_callType;
    const void* m_pointer;
    const char* m_method;
    OPTIONAL_LINK_NAME(const char* m_type;)
    const char* m_file;
    int m_line;
    const char* m_function;
    Printer m_printer;
    Mode m_mode;
    MethodPrintPrivate(const QLoggingCategory& category, MethodPrint::Type callType, const void* pointer, const char* message,
                       const char* file, int line, const char *function)
        : m_category(category),
        m_callType(callType),
        m_pointer(pointer),
        m_method(message),
        OPTIONAL_LINK_NAME_K(m_type(nullptr))
        m_file(file),
        m_line(line),
        m_function(function),
        m_mode(Message)
    {
        methodPrint(true, m_category, m_callType, m_pointer, m_method, m_file, m_line, m_function);
    }
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
    MethodPrintPrivate(const QLoggingCategory& category, MethodPrint::Type callType, const void* pointer, const char* method,
                            const char* type, const char* file, int line, const char *function)
        : m_category(category),
        m_callType(callType),
        m_pointer(pointer),
        m_method(method),
        m_type(type),
        m_file(file),
        m_line(line),
        m_function(function),
        m_mode(type ? MethodAndType : Message)
    {
        if(type)
            printWithType(true, m_category, m_callType, m_pointer, m_method, m_type, m_file, m_line, m_function);
        else
            methodPrint(true, m_category, m_callType, m_pointer, m_method, m_file, m_line, m_function);
    }
#endif
    MethodPrintPrivate(const QLoggingCategory& category,
                       const char* file, int line, const char *function, Printer&& printer)
        : m_category(category),
        m_callType(MethodPrint::Internal),
        m_pointer(nullptr),
        m_method(nullptr),
        OPTIONAL_LINK_NAME_K(m_type(nullptr))
        m_file(file),
        m_line(line),
        m_function(function),
        m_printer(std::move(printer)),
        m_mode(Printable)
    {
        methodPrint(true, m_category, m_file, m_line, m_function, m_printer);
    }
public:
    ~MethodPrintPrivate(){
        switch(m_mode){
        case Printable:
            methodPrint(false, m_category, m_file, m_line, m_function, m_printer);
            break;
#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
        case MethodAndType:
            printWithType(false, m_category, m_callType, m_pointer, m_method, m_type, m_file, m_line, m_function);
            break;
#endif
        case Message:
            methodPrint(false, m_category, m_callType, m_pointer, m_method, m_file, m_line, m_function);
            break;
        }
    }
    static const QLoggingCategory& category(MethodPrint::Type callType){
        switch(callType){
        case MethodPrint::JavaToNativeCall:
            return debugAPINativeCallsCategory();
        case MethodPrint::NativeToJavaCall:
            return debugAPIJavaOverloadsCategory();
        case MethodPrint::LibraryInitialization:
            return debugAPIInternalMethodsCategory();
        case MethodPrint::Internal:
            return debugAPIInternalMethodsCategory();
        case MethodPrint::ConstructorCall:
            return debugAPIConstructorCallsCategory();
        case MethodPrint::DestructorCall:
            return debugAPIDestructorCallsCategory();
        default:
            return debugAPIInternalMethodsCategory();
        }
    }
    static MethodPrintPrivate* create(MethodPrint::Type callType, const void* pointer, const char* message, const char* file, int line, const char *function){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            switch(callType){
            case MethodPrint::Disabled:
                return nullptr;
            default: break;
            }
            const QLoggingCategory& category = MethodPrintPrivate::category(callType);
            if(category.isDebugEnabled())
                return new MethodPrintPrivate(category, callType, pointer, message, file, line, function);
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(MethodPrint::Type callType, const QtJambiShell* shell, const char* method, const char* file, int line, const char *function){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            switch(callType){
            case MethodPrint::Disabled:
                return nullptr;
            default: break;
            }
            const QLoggingCategory& category = MethodPrintPrivate::category(callType);
            if(category.isDebugEnabled()){
                const void* pointer = nullptr;
                OPTIONAL_LINK_NAME(const char* type = nullptr;)
                if(QSharedPointer<QtJambiLink> link = reinterpret_cast<const QtJambiShellImpl*>(shell)->link()){
                    if(link->isDebugMessagingDisabled())
                        return nullptr;
                    pointer = link->pointer();
                    OPTIONAL_LINK_NAME(type = link->qtTypeName();)
                }
                return new MethodPrintPrivate(category, callType, pointer, method,
                                                   OPTIONAL_LINK_NAME_K(type)
                                                   file, line, function);
            }
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(MethodPrint::Type callType, QtJambiNativeID nativeID, const char* method, const char* file, int line, const char *function){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            switch(callType){
            case MethodPrint::Disabled:
                return nullptr;
            default: break;
            }
            const QLoggingCategory& category = MethodPrintPrivate::category(callType);
            if(category.isDebugEnabled()){
                const void* pointer = nullptr;
                OPTIONAL_LINK_NAME(const char* type = nullptr;)
                if(QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeID)){
                    if(link->isDebugMessagingDisabled())
                        return nullptr;
                    pointer = link->pointer();
                    OPTIONAL_LINK_NAME(type = link->qtTypeName();)
                }
                return new MethodPrintPrivate(category, callType, pointer, method,
                                                   OPTIONAL_LINK_NAME_K(type)
                                                   file, line, function);
            }
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(MethodPrint::Type callType, const QtJambiShellInterface* shellInterface, const char* method, const char* file, int line, const char *function){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            switch(callType){
            case MethodPrint::Disabled:
                return nullptr;
            default: break;
            }
            const QLoggingCategory& category = MethodPrintPrivate::category(callType);
            if(category.isDebugEnabled()){
                const void* pointer = nullptr;
                OPTIONAL_LINK_NAME(const char* type = nullptr;)
                if(shellInterface){
                    if(QSharedPointer<QtJambiLink> link = QtJambiShellImpl::get(shellInterface)->link()){
                        if(link->isDebugMessagingDisabled())
                            return nullptr;
                        pointer = link->pointer();
                        OPTIONAL_LINK_NAME(type = link->qtTypeName();)
                    }
                    return new MethodPrintPrivate(category, callType, pointer, method,
                                                       OPTIONAL_LINK_NAME_K(type)
                                                        file, line, function);
                }
            }
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(const QSharedPointer<QtJambiLink>& link, const char* method, const char* file, int line, const char *function){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            const QLoggingCategory& category = debugAPICleanupCallsCategory();
            if(category.isDebugEnabled() && link && !link->isDebugMessagingDisabled()){
                const void* pointer = link->pointer();
                if(!pointer)
                    pointer = link.data();
                OPTIONAL_LINK_NAME(const char* type = link->qtTypeName();)
                return new MethodPrintPrivate(category, MethodPrint::Internal, pointer, method,
                                                   OPTIONAL_LINK_NAME_K(type)
                                                    file, line, function);
            }
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(const QWeakPointer<QtJambiLink>& wlink, const char* method, const char* file, int line, const char *function){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            const QLoggingCategory& category = debugAPICleanupCallsCategory();
            if(category.isDebugEnabled()){
                if(QSharedPointer<QtJambiLink> link = wlink){
                    if(!link->isDebugMessagingDisabled()){
                        const void* pointer = link->pointer();
                        if(!pointer)
                            pointer = link.data();
                        OPTIONAL_LINK_NAME(const char* type = link->qtTypeName();)
                        return new MethodPrintPrivate(category, MethodPrint::Internal, pointer, method,
                                                      OPTIONAL_LINK_NAME_K(type)
                                                      file, line, function);
                    }
                }
            }
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(const QtJambiLink* link, const char* method, const char* file, int line, const char *function){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            const QLoggingCategory& category = debugAPICleanupCallsCategory();
            if(category.isDebugEnabled() && link && !link->isDebugMessagingDisabled()){
                const void* pointer = link->pointer();
                if(!pointer)
                    pointer = link;
                OPTIONAL_LINK_NAME(const char* type = link->qtTypeName();)
                return new MethodPrintPrivate(category, MethodPrint::Internal, pointer, method,
                                                   OPTIONAL_LINK_NAME_K(type)
                                                    file, line, function);
            }
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(MethodPrint::Type callType, const char* file, int line, const char *function, const char* message,...){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            switch(callType){
            case MethodPrint::Disabled:
                return nullptr;
            default: break;
            }
            const QLoggingCategory& category = MethodPrintPrivate::category(callType);
            if(category.isDebugEnabled())
                return new MethodPrintPrivate(category, file, line, function, [data = QString::asprintf(message)](QDebug& dbg) -> QDebug& {
                    return dbg << data;
                });
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(const QLoggingCategory& category, const char* file, int line, const char *function, const char* message,...){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            if(category.isDebugEnabled())
                return new MethodPrintPrivate(category, file, line, function, [data = QString::asprintf(message)](QDebug& dbg) -> QDebug& {
                    return dbg << data;
                });
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(MethodPrint::Type callType, const char* file, int line, const char *function, Printer&& printer){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            switch(callType){
            case MethodPrint::Disabled:
                return nullptr;
            default: break;
            }
            const QLoggingCategory& category = MethodPrintPrivate::category(callType);
            if(category.isDebugEnabled())
                return new MethodPrintPrivate(category, file, line, function, std::move(printer));
        }
        return nullptr;
    }
    static MethodPrintPrivate* create(const QLoggingCategory& category, const char* file, int line, const char *function, Printer&& printer){
        if(Q_UNLIKELY(enabledMethodTracePrints())){
            if(category.isDebugEnabled())
                return new MethodPrintPrivate(category, file, line, function, std::move(printer));
        }
        return nullptr;
    }
    static MethodPrintPrivate* createEventPrint(const char* file, int line, const char *function, QObject *receiver, QEvent *event){
        if(enabledEventPrints()){
            const QLoggingCategory& category = debugAPIEventsCategory();
            if(category.isDebugEnabled()){
                QString receiverDbg;
                QDebug(&receiverDbg).nospace().noquote() << receiver;
                const char* enumValue = QEvent::staticMetaObject.enumerator(0).valueToKey(event->type());
                if(enumValue){
                    return new MethodPrintPrivate(category, file, line, function, [enumValue, receiverDbg, receiver, event](QDebug& dbg){
                        dbg << "QCoreApplication::notify(QObject *receiver, QEvent *event) with receiver: " << reinterpret_cast<void*>(receiver) << "=" << receiverDbg << ", event: " << reinterpret_cast<void*>(event) << "=QEvent::" << enumValue;
                    });
                }
                const std::type_info* eventType = QtJambiPrivate::CheckPointer<QEvent>::trySupplyType(event);
                if(eventType){
                    return new MethodPrintPrivate(category, file, line, function, [receiverDbg, receiver, event, type = event->type(), eventTypeName = QtJambiAPI::typeName(*eventType)](QDebug& dbg){
                        dbg << "QCoreApplication::notify(QObject *receiver, QEvent *event) with receiver: " << reinterpret_cast<void*>(receiver) << "=" << receiverDbg << ", event: " << reinterpret_cast<void*>(event) << "=QEvent::Type(type: " << int(type) << ", class: " << eventTypeName.constData() << ")";
                    });
                }else{
                    return new MethodPrintPrivate(category, file, line, function, [receiverDbg, receiver, event, type = event->type()](QDebug& dbg){
                        dbg << "QCoreApplication::notify(QObject *receiver, QEvent *event) with receiver: " << reinterpret_cast<void*>(receiver) << "=" << receiverDbg << ", event: " << reinterpret_cast<void*>(event) << "=QEvent::Type(" << int(type) << ")";
                    });
                }
            }
        }
        return nullptr;
    }
    static void methodPrint(bool isEnter, const QLoggingCategory& category, const char *file, int line, const char *function, const Printer& printer)
    {
        QMessageLogger logger(adaptFile(file), line, function, category.categoryName());
        QDebug dbg = logger.debug().nospace().noquote();
        if(isEnter){
            dbg << "ENTER: ";
        }else{
            dbg << "LEAVE: ";
        }
        dbg << printer << " in thread tid=" << quintptr(QThread::currentThreadId());
    }
    static void methodPrint(bool isEnter, const QLoggingCategory& category, MethodPrint::Type callType, const void* pointer, const char *method, const char *file, int line, const char *function)
    {
        QMessageLogger logger(adaptFile(file), line, function, category.categoryName());
        QDebug dbg = logger.debug().nospace().noquote();
        if(isEnter){
            dbg << "ENTER: ";
        }else{
            dbg << "LEAVE: ";
        }
        if(pointer){
            switch(callType){
            case MethodPrint::JavaToNativeCall:
                dbg << "Java calling native method " << method << " on object " << pointer;
                break;
            case MethodPrint::NativeToJavaCall:
                dbg << "Shell (object: " << pointer << ") calling java method " << method;
                break;
            case MethodPrint::LibraryInitialization:
                dbg << "Initializing " << method << " on object " << pointer;
                break;
            case MethodPrint::Internal:
                dbg << "" << method << " on object " << pointer;
                break;
            case MethodPrint::ConstructorCall:
                dbg << "Constructor call " << method << " on object " << pointer;
                break;
            case MethodPrint::DestructorCall:
                dbg << "Destuctor call " << method << " on object " << pointer;
                break;
            default:break;
            }
        }else{
            switch(callType){
            case MethodPrint::JavaToNativeCall:
                dbg << "Java calling native method " << method;
                break;
            case MethodPrint::NativeToJavaCall:
                dbg << "Shell calling java method " << method;
                break;
            case MethodPrint::LibraryInitialization:
                dbg << "Initializing " << method;
                break;
            case MethodPrint::Internal:
                dbg << "" << method;
                break;
            case MethodPrint::ConstructorCall:
                dbg << "Constructor call " << method;
                break;
            case MethodPrint::DestructorCall:
                dbg << "Destuctor call " << method;
                break;
            default:break;
            }
        }
        dbg << " in thread tid=" << quintptr(QThread::currentThreadId());
    }
    static void printImpl(const char *message, const char *file, int line, const char *function, const QLoggingCategory& category)
    {
        QMessageLogger(adaptFile(file), line, function).debug(category).nospace().noquote() << message << " in thread tid=" << quintptr(QThread::currentThreadId());
    }
    static void printImpl(const QString& message, const char *file, int line, const char *function, const QLoggingCategory& category)
    {
        QMessageLogger(adaptFile(file), line, function).debug(category).nospace().noquote() << message << " in thread tid=" << quintptr(QThread::currentThreadId());
    }
    static void printImpl(Printer&& printer, const char *file, int line, const char *function, const QLoggingCategory& category)
    {
        QMessageLogger(adaptFile(file), line, function).debug(category).nospace().noquote() << printer << " in thread tid=" << quintptr(QThread::currentThreadId());
    }
    static void print(const char *message, const char *file, int line, const char *function)
    {
        if (enabledMethodTracePrints()){
            const QLoggingCategory& category = debugAPICategory();
            if (category.isDebugEnabled()) {
                printImpl(message, file, line, function, category);
            }
        }
    }
    static void print(const char *file, int line, const char *function, Printer&& printer)
    {
        if (enabledMethodTracePrints()){
            const QLoggingCategory& category = internalCategory();
            if (category.isDebugEnabled()) {
                printImpl(std::move(printer), file, line, function, category);
            }
        }
    }
    static void printCleanupArgs(const char *file, int line, const char *function, const char *format,...)
    {
        if (enabledMethodTracePrints()){
            const QLoggingCategory& category = debugAPICleanupCallsCategory();
            if (category.isDebugEnabled()) {
                va_list args;
                va_start(args, format);
                QString string = QString::vasprintf(format, args);
                va_end(args);
                printImpl(string, file, line, function, category);
            }
        }
    }
    static void printArgs(const char *file, int line, const char *function, const char *format,...)
    {
        if (enabledMethodTracePrints()){
            const QLoggingCategory& category = internalCategory();
            if (category.isDebugEnabled()) {
                va_list args;
                va_start(args, format);
                QString string = QString::vasprintf(format, args);
                va_end(args);
                printImpl(string, file, line, function, category);
            }
        }
    }

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
    static void printWithType(bool isEnter, const QLoggingCategory& category, MethodPrint::Type callType, const void* pointer, const char *method, const char * typeName, const char *file, int line, const char *function)
    {
        Q_ASSERT(typeName);
        QMessageLogger logger(adaptFile(file), line, function, category.categoryName());
        QDebug dbg = logger.debug().nospace().noquote();
        if(isEnter){
            dbg << "ENTER: ";
        }else{
            dbg << "LEAVE: ";
        }
        switch(callType){
        case MethodPrint::NativeToJavaCall:
            dbg << "Shell (object: " << pointer << " , type: " << typeName << ") calling java method " << method;
            break;
        case MethodPrint::Internal:
            dbg << "" << method << " on object " << pointer << " of type " << typeName;
            break;
        case MethodPrint::ConstructorCall:
            dbg << "Constructor call " << method << " on object " << pointer << " of type " << typeName;
            break;
        case MethodPrint::DestructorCall:
            dbg << "Destuctor call " << method << " on object " << pointer << " of type " << typeName;
            break;
        default:
            break;
        }
        dbg << " in thread tid=" << quintptr(QThread::currentThreadId());
    }
#endif
};

void print(const char *message, const char *file, int line, const char *function)
{
    MethodPrintPrivate::print(message, file, line, function);
}

void printCleanupArgs(const char *file, int line, const char *function, const char *format,...){
    MethodPrintPrivate::printCleanupArgs(file, line, function, format);
}

void printArgs(const char *file, int line, const char *function, const char *format,...){
    MethodPrintPrivate::printArgs(file, line, function, format);
}

void print(const char *file, int line, const char *function, Printer&& printer){
    MethodPrintPrivate::print(file, line, function, std::move(printer));
}

MethodPrint::MethodPrint(MethodPrint::Type callType, const void* pointer, const char* method, const char* file, int line, const char *function)
    : d(MethodPrintPrivate::create(callType, pointer, method, file, line, function)) {
}

MethodPrint::MethodPrint(MethodPrint::Type callType, QtJambiNativeID nativeID, const char* method, const char* file, int line, const char *function)
    : d(MethodPrintPrivate::create(callType, nativeID, method, file, line, function)) {
}

MethodPrint::MethodPrint(MethodPrint::Type callType, const QtJambiShell* shell, const char* method, const char* file, int line, const char *function)
    :
    d(MethodPrintPrivate::create(callType, shell, method, file, line, function)) {
}
MethodPrint::MethodPrint(MethodPrint::Type callType, const QtJambiShellInterface* shellInterface, const char* method, const char* file, int line, const char *function)
    :
    d(MethodPrintPrivate::create(callType, shellInterface, method, file, line, function)) {
}
MethodPrint::MethodPrint(MethodPrint::Type callType, const char* method, const char* file, int line, const char *function)
    :
    d(MethodPrintPrivate::create(callType, static_cast<const void*>(nullptr), method, file, line, function)) {
}

MethodPrint::MethodPrint(MethodPrintPrivate* _d)
    : d(_d)
{
}

MethodPrint::~MethodPrint(){
    delete d;
}

MethodPrintFromLink::MethodPrintFromLink(const QSharedPointer<QtJambiLink>& link, const char* method, const char* file, int line, const char *function)
    : MethodPrint(MethodPrintPrivate::create(link, method, file, line, function))
{
}

MethodPrintFromLink::MethodPrintFromLink(const QWeakPointer<QtJambiLink>& link, const char* method, const char* file, int line, const char *function)
    : MethodPrint(MethodPrintPrivate::create(link, method, file, line, function))
{
}

MethodPrintFromLink::MethodPrintFromLink(const QtJambiLink* link, const char* method, const char* file, int line, const char *function)
    : MethodPrint(MethodPrintPrivate::create(link, method, file, line, function))
{
}

MethodPrintFromArgs::MethodPrintFromArgs(MethodPrint::Type callType, const char* file, int line, const char *function, const char* message, ...)
    : MethodPrint(MethodPrintPrivate::create(callType, file, line, function, message))
{
}

MethodPrintFromArgs::MethodPrintFromArgs(const QLoggingCategory& category, const char* file, int line, const char *function, const char* message, ...)
    : MethodPrint(MethodPrintPrivate::create(category, file, line, function, message))
{
}

MethodPrintFromSupplier::MethodPrintFromSupplier(MethodPrint::Type callType, const char* file, int line, const char *function, Printer&& supplier)
    : MethodPrint(MethodPrintPrivate::create(callType, file, line, function, std::forward<Printer>(supplier)))
{
}

MethodPrintFromSupplier::MethodPrintFromSupplier(const QLoggingCategory& category, const char* file, int line, const char *function, Printer&& supplier)
    : MethodPrint(MethodPrintPrivate::create(category, file, line, function, std::forward<Printer>(supplier)))
{
}

EventPrint::EventPrint(const char* file, int line, const char *function, QObject *receiver, QEvent *event)
    : MethodPrint(MethodPrintPrivate::createEventPrint(file, line, function, receiver, event))
{
}

struct PrinterPrivate : QSharedData{
    void* data = nullptr;
    Printer::Invoker invoker;
    Printer::Deleter deleter;
    PrinterPrivate(void* _data, Printer::Invoker _invoker, Printer::Deleter _deleter = nullptr)
        : QSharedData(), data(_data), invoker(_invoker), deleter(_deleter){
        Q_ASSERT(_data);
        Q_ASSERT(_invoker);
    }
    ~PrinterPrivate(){ if(deleter) deleter(data); }
};

Printer::Printer(FunctionPointer functor) noexcept
    : d(!functor ? nullptr : new PrinterPrivate(
            reinterpret_cast<void*>(functor),
            [](void* data, QDebug& d){
                FunctionPointer task = reinterpret_cast<FunctionPointer>(data);
                (*task)(d);
            })){}

Printer::Printer() noexcept : d(){}
Printer::~Printer() noexcept {}
Printer::Printer(const Printer& other) noexcept : d(other.d) {}
Printer::Printer(Printer&& other) noexcept : d(std::move(other.d)) {}
Printer::Printer(void* data, Invoker invoker, Deleter deleter) noexcept
    : d(!data ? nullptr : new PrinterPrivate(data, invoker, deleter)) {}
Printer& Printer::operator=(const Printer& other) noexcept { d = other.d; return *this; }
Printer& Printer::operator=(Printer&& other) noexcept { d = std::move(other.d); return *this; }
bool Printer::operator==(const Printer& other) const noexcept { return d == other.d; }

Printer::operator bool() const noexcept{
    return d;
}

bool Printer::operator !() const noexcept{
    return !d;
}

void Printer::operator()(QDebug& dbg) const{
    if(d)
        d->invoker(d->data, dbg);
}

}

#endif // QTJAMBI_NO_METHOD_TRACE

void qtjambi_assert(const char *assertion, const char *file, int line, const char *function){
#ifndef QTJAMBI_NO_METHOD_TRACE
    DebugAPI::print(assertion, file, line, function);
#endif
    qt_assert(assertion, file, line);
}
