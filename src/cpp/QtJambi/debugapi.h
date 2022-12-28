/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

QTJAMBI_EXPORT void qtjambi_assert(const char *assertion, const char *file, int line);

class QtJambiShell;

#ifdef QT_NO_DEBUG
#  define QTJAMBI_DEBUG_TRACE(location)
#  define QTJAMBI_DEBUG_METHOD_PRINT(context, methodname)
#  define QTJAMBI_DEBUG_METHOD_PRINT_WHERE(context, methodname, shell)
#if defined(Q_ASSERT) && defined(QT_FORCE_ASSERTS)
#  undef Q_ASSERT
#  define Q_ASSERT(cond) ((cond) ? static_cast<void>(0) : qtjambi_assert(#cond, __FILE__, __LINE__))
#endif
#else
namespace DebugAPI{

QTJAMBI_EXPORT void print(const char *location, const char *file, int line);
class QTJAMBI_EXPORT MethodPrint {
public:
    MethodPrint(const char* enterMessage, const char* leaveMethod, const char* file, int line);
    ~MethodPrint();
private:
    const char* m_enterMessage;
    const char* m_leaveMethod;
    const char* m_file;
    int m_line;
};

}
#  define QTJAMBI_DEBUG_TRACE(location) DebugAPI::print(location, __FILE__, __LINE__);

#if defined(Q_ASSERT)
#  undef Q_ASSERT
#  if defined(QT_NO_DEBUG) && !defined(QT_FORCE_ASSERTS)
#    define Q_ASSERT(cond) static_cast<void>(false && (cond))
#  else
#    define Q_ASSERT(cond) ((cond) ? static_cast<void>(0) : qtjambi_assert(#cond, __FILE__, __LINE__))
#  endif
#endif

#define QTJAMBI_DEBUG_METHOD_PRINT(context, methodname)\
DebugAPI::MethodPrint __debug_method_print("(" context ") entering: " methodname, "(" context ") leaving:  " methodname, __FILE__, __LINE__);\
Q_UNUSED(__debug_method_print)

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)

namespace DebugAPI{
class QTJAMBI_EXPORT MethodPrintWithType {
public:
    MethodPrintWithType(const char* enterMessage, const char* leaveMethod, const QtJambiShell* shell, const char* file, int line);
    ~MethodPrintWithType();
private:
    const char* m_enterMessage;
    const char* m_leaveMethod;
    const char* m_type;
    const char* m_file;
    int m_line;
};
}

#define QTJAMBI_DEBUG_METHOD_PRINT_WHERE(context, methodname, shell)\
DebugAPI::MethodPrintWithType __debug_method_print("(" context ") entering: " methodname, "(" context ") leaving: " methodname, shell, __FILE__, __LINE__);\
Q_UNUSED(__debug_method_print)

#else
#define QTJAMBI_DEBUG_METHOD_PRINT_WHERE(context, methodname, shell) QTJAMBI_DEBUG_METHOD_PRINT(context, methodname)
#endif

#endif

#endif // QTJAMBI_DEBUGAPI_H
