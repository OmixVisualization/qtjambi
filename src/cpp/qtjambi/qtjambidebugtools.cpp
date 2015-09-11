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

#include "qtjambidebugtools_p.h"

#if defined(QTJAMBI_DEBUG_TOOLS)

#include "qtjambi_global.h"
#include "qtjambi_core.h"

#include <QMutex>
#include <QHash>

    // ---- TOOLS
    typedef QHash<QString, int> CountsForName;

    // Looks like (at least from JDK5 build 1.5.0_22-b03) the JVM is allowed
    // to run finalizers from two threads at once.  Maybe this is only when
    // we call Runtime#runFinalization(); or maybe the matter is unspecified.
    typedef QMutex CountsForNameLock;

    static int sum_of(const CountsForName &countsForName)
    {
        int sum = 0;
        foreach (int value, countsForName.values())
            sum += value;
        return sum;
    }

    static void reset(CountsForName &countsForName, const QString &className)
    {
        if (className.isEmpty())
            countsForName.clear();
        else
            countsForName[className] = 0;
    }

    static int count(const CountsForName &countsForName, const QString &className)
    {
        if (className.isEmpty())
            return sum_of(countsForName);
        else
            return countsForName.value(className, 0);
    }

    // ---- MACROS
#define COUNTER_IMPLEMENTATION(NAME) \
    \
    Q_GLOBAL_STATIC(CountsForName, g_##NAME) \
    \
    Q_GLOBAL_STATIC(CountsForNameLock, l_##NAME) \
    \
    static void qtjambi_reset_##NAME(const QString &className) \
    { \
        (*l_##NAME()).lock(); \
        reset(*g_##NAME(), className); \
        (*l_##NAME()).unlock(); \
    } \
    \
    static int qtjambi_##NAME(const QString &className) \
    { \
        int counter; \
        (*l_##NAME()).lock(); \
        counter = count(*g_##NAME(), className); \
        (*l_##NAME()).unlock(); \
        return counter; \
    } \
    \
    void qtjambi_increase_##NAME(const QString &className) \
    { \
        (*l_##NAME()).lock(); \
        (*g_##NAME())[className] ++; \
        (*l_##NAME()).unlock(); \
    } \
    \
    extern "C" Q_DECL_EXPORT void JNICALL \
    QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiDebugTools_reset_1##NAME ) \
    (JNIEnv *env, \
    jclass, \
    jstring className)\
    { \
        qtjambi_reset_##NAME(qtjambi_to_qstring(env, className)); \
    } \
    \
    extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiDebugTools_##NAME) \
    (JNIEnv *env, jclass, \
    jstring className) \
    { \
        return qtjambi_##NAME(qtjambi_to_qstring(env, className)); \
    }


    // ---- IMPLEMENTATIONS

    COUNTER_IMPLEMENTATION(finalizedCount)
    COUNTER_IMPLEMENTATION(userDataDestroyedCount)
    COUNTER_IMPLEMENTATION(destructorFunctionCalledCount)
    COUNTER_IMPLEMENTATION(shellDestructorCalledCount)
    COUNTER_IMPLEMENTATION(objectInvalidatedCount)
    COUNTER_IMPLEMENTATION(disposeCalledCount)
    COUNTER_IMPLEMENTATION(linkDestroyedCount)
    COUNTER_IMPLEMENTATION(linkConstructedCount)

#else // QTJAMBI_DEBUG_TOOLS
#  error "Don't include this file without QTJAMBI_DEBUG_TOOLS defined"
#endif

