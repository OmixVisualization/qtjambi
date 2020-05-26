/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

#ifndef QTJAMBI_DEBUG_TOOLS_H
#define QTJAMBI_DEBUG_TOOLS_H

#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)

#include "qtjambi_global.h"

#define QTJAMBI_COUNTER_DECLARATIONS(NAME) \
    public: static void increase_##NAME(const char*className);\
    private: static Increaser increase_##NAME##_fct;

class QtJambiMemoryDebugTool{
private:
    typedef void(*Increaser)(const char*className);
    QtJambiMemoryDebugTool() = delete;
    ~QtJambiMemoryDebugTool() = delete;

    QTJAMBI_COUNTER_DECLARATIONS(cleanCallerCount)
    QTJAMBI_COUNTER_DECLARATIONS(userDataDestroyedCount)
    QTJAMBI_COUNTER_DECLARATIONS(destructorFunctionCalledCount)
    QTJAMBI_COUNTER_DECLARATIONS(pointerContainerDestroyedCount)
    QTJAMBI_COUNTER_DECLARATIONS(shellDestructorCalledCount)
    QTJAMBI_COUNTER_DECLARATIONS(objectInvalidatedCount)
    QTJAMBI_COUNTER_DECLARATIONS(disposeCalledCount)
    QTJAMBI_COUNTER_DECLARATIONS(linkDestroyedCount)
    QTJAMBI_COUNTER_DECLARATIONS(linkConstructedCount)
    QTJAMBI_COUNTER_DECLARATIONS(shellDestroyedCount)

    public: static void enable();
    public: static void disable();
};

#endif

QTJAMBI_EXPORT void qtjambi_increase_destructorFunctionCalledCount(const char*className);

#endif
