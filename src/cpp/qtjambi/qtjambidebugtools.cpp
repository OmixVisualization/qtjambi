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

#include <QtCore/QtCore>

#include "qtjambidebugtools_p.h"

#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)

#include "qtjambi_core.h"

    QByteArray qtjambi_to_qbytearray(JNIEnv *env, jstring java_string);

    // ---- TOOLS
    typedef QHash<QByteArray, int> CountsForName;
    typedef QReadWriteLock CountsForNameLock;
    Q_GLOBAL_STATIC(CountsForNameLock, gCountsForNameLock)

    static int sum_of(const CountsForName *countsForName)
    {
        int sum = 0;
        if(countsForName){
            for(int value : countsForName->values())
                sum += value;
        }
        return sum;
    }

    static void increase(CountsForName *countsForName, const QByteArray&className)
    {
        if(countsForName && !className.isEmpty()){
            (*countsForName)[className]++;
        }
    }

    static void reset(CountsForName *countsForName, const QByteArray&className)
    {
        if(countsForName){
            if (className.isEmpty())
                countsForName->clear();
            else
                (*countsForName)[className] = 0;
        }
    }

    static int count(const CountsForName *countsForName, const QByteArray&className)
    {
        if (className.isEmpty())
            return sum_of(countsForName);
        else
            return countsForName->value(className, 0);
    }

    // ---- MACROS
#define COUNTER_IMPLEMENTATION(NAME) \
    \
    Q_GLOBAL_STATIC(CountsForName, g_##NAME) \
    \
    static void qtjambi_reset_##NAME(const char*className) \
    { \
        QWriteLocker locker(gCountsForNameLock());\
        Q_UNUSED(locker)\
        reset(g_##NAME(), className); \
    } \
    \
    static int qtjambi_##NAME(const char*className) \
    { \
        QReadLocker locker(gCountsForNameLock());\
        Q_UNUSED(locker)\
        return count(g_##NAME(), className); \
    } \
    \
    void qtjambi_increase_##NAME##_enabled(const char*className) \
    { \
        QWriteLocker locker(gCountsForNameLock());\
        Q_UNUSED(locker)\
        increase(g_##NAME(), className);\
    } \
    \
    void qtjambi_increase_##NAME##_disabled(const char*){} \
    \
    QtJambiMemoryDebugTool::Increaser QtJambiMemoryDebugTool::increase_##NAME##_fct = qtjambi_increase_##NAME##_disabled;\
    \
    void QtJambiMemoryDebugTool::increase_##NAME(const char*className) \
    { \
        increase_##NAME##_fct(className);\
    } \
    \
    extern "C" Q_DECL_EXPORT void JNICALL \
    QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_reset_1##NAME ) \
    (JNIEnv *env, \
    jclass, \
    jstring className)\
    { \
        qtjambi_reset_##NAME(qtjambi_to_qbytearray(env, className)); \
    } \
    \
    extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_##NAME) \
    (JNIEnv *env, jclass, \
    jstring className) \
    { \
        return qtjambi_##NAME(qtjambi_to_qbytearray(env, className)); \
    }

#define ENABLE_COUNTER(NAME) \
    increase_##NAME##_fct = qtjambi_increase_##NAME##_enabled;
#define DISABLE_COUNTER(NAME) \
    increase_##NAME##_fct = qtjambi_increase_##NAME##_disabled;

    // ---- IMPLEMENTATIONS

    COUNTER_IMPLEMENTATION(cleanCallerCount)
    COUNTER_IMPLEMENTATION(userDataDestroyedCount)
    COUNTER_IMPLEMENTATION(destructorFunctionCalledCount)
    COUNTER_IMPLEMENTATION(shellDestructorCalledCount)
    COUNTER_IMPLEMENTATION(shellDestroyedCount)
    COUNTER_IMPLEMENTATION(objectInvalidatedCount)
    COUNTER_IMPLEMENTATION(disposeCalledCount)
    COUNTER_IMPLEMENTATION(linkDestroyedCount)
    COUNTER_IMPLEMENTATION(linkConstructedCount)
    COUNTER_IMPLEMENTATION(pointerContainerDestroyedCount)

    void QtJambiMemoryDebugTool::enable()
    {
        ENABLE_COUNTER(cleanCallerCount)
        ENABLE_COUNTER(userDataDestroyedCount)
        ENABLE_COUNTER(destructorFunctionCalledCount)
        ENABLE_COUNTER(shellDestructorCalledCount)
        ENABLE_COUNTER(shellDestroyedCount)
        ENABLE_COUNTER(objectInvalidatedCount)
        ENABLE_COUNTER(disposeCalledCount)
        ENABLE_COUNTER(linkDestroyedCount)
        ENABLE_COUNTER(linkConstructedCount)
        ENABLE_COUNTER(pointerContainerDestroyedCount)
    }

    void QtJambiMemoryDebugTool::disable()
    {
        DISABLE_COUNTER(cleanCallerCount)
        DISABLE_COUNTER(userDataDestroyedCount)
        DISABLE_COUNTER(destructorFunctionCalledCount)
        DISABLE_COUNTER(shellDestructorCalledCount)
        DISABLE_COUNTER(shellDestroyedCount)
        DISABLE_COUNTER(objectInvalidatedCount)
        DISABLE_COUNTER(disposeCalledCount)
        DISABLE_COUNTER(linkDestroyedCount)
        DISABLE_COUNTER(linkConstructedCount)
        DISABLE_COUNTER(pointerContainerDestroyedCount)
    }

    extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_enableCounters) (JNIEnv *, jclass)
    {
        QtJambiMemoryDebugTool::enable();
    }

    extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_QtJambiDebugTools_disableCounters) (JNIEnv *, jclass)
    {
        QtJambiMemoryDebugTool::disable();
    }

#endif


void qtjambi_increase_destructorFunctionCalledCount(const char*className){
#if defined(QTJAMBI_DEBUG_TOOLS) || !defined(QT_NO_DEBUG)
    QtJambiMemoryDebugTool::increase_destructorFunctionCalledCount(className);
#else
    Q_UNUSED(className)
#endif
}
