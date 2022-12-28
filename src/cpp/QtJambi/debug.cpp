/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#ifndef QT_NO_DEBUG

namespace DebugAPI{

void print(const char *location, const char *file, int line)
{
    static bool should = getenv("QTJAMBI_DEBUG_TRACE") != nullptr;
    if (should) {
        fprintf(stderr, "%s in THREAD %p ( %s:%d )\n", location, reinterpret_cast<void*>(QThread::currentThreadId()), file, line);
        fflush(stderr);
    }
}

MethodPrint::MethodPrint(const char* enterMessage, const char* leaveMethod, const char* file, int line) :
    m_enterMessage(enterMessage),
    m_leaveMethod(leaveMethod),
    m_file(file),
    m_line(line) {
    print(m_enterMessage, m_file, m_line);
}
MethodPrint::~MethodPrint(){
    print(m_leaveMethod, m_file, m_line);
}

#if defined(QTJAMBI_DEBUG_TOOLS) || defined(QTJAMBI_LINK_NAME)
void printWithType(const char *location, const char * typeName, const char *file, int line)
{
    static bool should = getenv("QTJAMBI_DEBUG_TRACE") != nullptr;
    if (should) {
        if(typeName){
            fprintf(stderr, "%s on object %s in THREAD %p ( %s:%d )\n", location, typeName, reinterpret_cast<void*>(QThread::currentThreadId()), file, line);
        }else{
            fprintf(stderr, "%s in THREAD %p ( %s:%d )\n", location, reinterpret_cast<void*>(QThread::currentThreadId()), file, line);
        }
        fflush(stderr);
    }
}

MethodPrintWithType::MethodPrintWithType(const char* enterMessage, const char* leaveMethod, const QtJambiShell* shell, const char* file, int line) :
    m_enterMessage(enterMessage),
    m_leaveMethod(leaveMethod),
    m_type(nullptr),
    m_file(file),
    m_line(line) {
    if(QSharedPointer<QtJambiLink> link = static_cast<const QtJambiShellImpl*>(shell)->link()){
        m_type = link->qtTypeName();
    }
    printWithType(m_enterMessage, m_type, m_file, m_line);
}
MethodPrintWithType::~MethodPrintWithType(){
    printWithType(m_leaveMethod, m_type, m_file, m_line);
}
#endif

}

#endif // QT_NO_DEBUG

void qtjambi_assert(const char *assertion, const char *file, int line){
#ifndef QT_NO_DEBUG
    DebugAPI::print(assertion, file, line);
#endif
    qt_assert(assertion, file, line);
}
