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

#ifndef QTJAMBI_DEBUGAPI_H
#define QTJAMBI_DEBUGAPI_H

#include "global.h"

QTJAMBI_EXPORT void qtjambi_assert(const char *assertion, const char *file, int line, const char *function);

class QtJambiShell;

#ifdef QT_NO_DEBUG
#if defined(Q_ASSERT) && defined(QT_FORCE_ASSERTS)
#  undef Q_ASSERT
#  define Q_ASSERT(cond) ((cond) ? static_cast<void>(0) : qtjambi_assert(#cond, __FILE__, __LINE__, __FUNCTION__))
#endif
#endif

#ifdef QTJAMBI_NO_METHOD_TRACE
#  define QTJAMBI_DEBUG_PRINT(...)
#  define QTJAMBI_INTERNAL_METHOD_CALL(...)
#  define QTJAMBI_NATIVE_METHOD_CALL(...)
#  define QTJAMBI_LIBRARY_INITIALIZATION_METHOD_CALL(...)
#else
namespace DebugAPI{

QTJAMBI_EXPORT void print(const char *location, const char *file, int line, const char *function);
class QTJAMBI_EXPORT MethodPrint {
public:
    enum Type{
        Disabled,
        JavaToNativeCall,
        NativeToJavaCall,
        LibraryInitialization,
        Internal,
        ConstructorCall,
        DestructorCall,
    };
    MethodPrint(Type callType, const void* pointer, const char* method, const char* file, int line, const char *function);
    ~MethodPrint();
private:
    class MethodPrintPrivate* d;
};

}
#  define QTJAMBI_DEBUG_PRINT(message) DebugAPI::print(message, __FILE__, __LINE__, __FUNCTION__);

#if defined(Q_ASSERT)
#  undef Q_ASSERT
#  if defined(QT_NO_DEBUG) && !defined(QT_FORCE_ASSERTS)
#    define Q_ASSERT(cond) static_cast<void>(false && (cond))
#  else
#    define Q_ASSERT(cond) ((cond) ? static_cast<void>(0) : qtjambi_assert(#cond, __FILE__, __LINE__, __FUNCTION__))
#  endif
#endif

#define QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL_1(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::Internal, this, methodname, __FILE__, __LINE__, __FUNCTION__);

#define QTJAMBI_INTERNAL_METHOD_CALL(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::Internal, nullptr, methodname, __FILE__, __LINE__, __FUNCTION__);

#define QTJAMBI_NATIVE_METHOD_CALL(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::JavaToNativeCall, nullptr, methodname, __FILE__, __LINE__, __FUNCTION__);

#define QTJAMBI_NATIVE_INSTANCE_METHOD_CALL(methodname, _this)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::JavaToNativeCall, _this, methodname, __FILE__, __LINE__, __FUNCTION__);

#define QTJAMBI_LIBRARY_INITIALIZATION_METHOD_CALL(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::LibraryInitialization, nullptr, methodname, __FILE__, __LINE__, __FUNCTION__);

#define QTJAMBI_JAVA_METHOD_CALL_1(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::NativeToJavaCall, this, methodname, __FILE__, __LINE__, __FUNCTION__);

#define QTJAMBI_IN_DESTRUCTOR_CALL_1(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::DestructorCall, this, methodname, __FILE__, __LINE__, __FUNCTION__);

#define QTJAMBI_IN_CONSTRUCTOR_CALL_1(methodname)\
DebugAPI::MethodPrint __debug_method_print(DebugAPI::MethodPrint::ConstructorCall, this, methodname, __FILE__, __LINE__, __FUNCTION__);

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
namespace DebugAPI{
class QTJAMBI_EXPORT MethodPrintWithType {
public:
    MethodPrintWithType(Type callType, const char* method, const QtJambiShell* shell, const char* file, int line, const char *function);
    MethodPrintWithType(Type callType, const void* pointer, const char* method, const char* type_name, const char* file, int line, const char *function);
    ~MethodPrintWithType();
private:
    class MethodPrintWithTypePrivate* d;
};
}

#define QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL_2(methodname, shell)\
DebugAPI::MethodPrintWithType __debug_method_print(DebugAPI::MethodPrint::Internal, methodname, shell, __FILE__, __LINE__, __FUNCTION__);
#define QTJAMBI_JAVA_METHOD_CALL_2(methodname, shell)\
DebugAPI::MethodPrintWithType __debug_method_print(DebugAPI::MethodPrint::NativeToJavaCall, methodname, shell, __FILE__, __LINE__, __FUNCTION__);
#define QTJAMBI_IN_DESTRUCTOR_CALL_2(methodname, shell)\
DebugAPI::MethodPrintWithType __debug_method_print(DebugAPI::MethodPrint::DestructorCall, methodname, shell, __FILE__, __LINE__, __FUNCTION__);
#define QTJAMBI_IN_CONSTRUCTOR_CALL_2(methodname, shell)\
DebugAPI::MethodPrintWithType __debug_method_print(DebugAPI::MethodPrint::ConstructorCall, methodname, shell, __FILE__, __LINE__, __FUNCTION__);

#define QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL(...) QT_OVERLOADED_MACRO(QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL, __VA_ARGS__)
#define QTJAMBI_JAVA_METHOD_CALL(...) QT_OVERLOADED_MACRO(QTJAMBI_JAVA_METHOD_CALL, __VA_ARGS__)
#define QTJAMBI_IN_DESTRUCTOR_CALL(...) QT_OVERLOADED_MACRO(QTJAMBI_IN_DESTRUCTOR_CALL, __VA_ARGS__)
#define QTJAMBI_IN_CONSTRUCTOR_CALL(...) QT_OVERLOADED_MACRO(QTJAMBI_IN_CONSTRUCTOR_CALL, __VA_ARGS__)

#else
#define QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL(methodname, ...) QTJAMBI_INTERNAL_INSTANCE_METHOD_CALL_1(methodname)
#define QTJAMBI_JAVA_METHOD_CALL(methodname, ...) QTJAMBI_JAVA_METHOD_CALL_1(methodname)
#define QTJAMBI_IN_DESTRUCTOR_CALL(methodname, ...) QTJAMBI_IN_DESTRUCTOR_CALL_1(methodname)
#define QTJAMBI_IN_CONSTRUCTOR_CALL(methodname, ...) QTJAMBI_IN_CONSTRUCTOR_CALL_1(methodname)
#endif

#endif

#endif // QTJAMBI_DEBUGAPI_H
