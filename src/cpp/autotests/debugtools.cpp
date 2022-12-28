/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#include <QtCore/QByteArray>
#include <QtCore/QHash>
#include <QtCore/QReadWriteLock>

#include <QtJambi/QtJambiAPI>
#include <QtJambi/testapi.h>

typedef QMap<TestAPI::DebugCounters, QMap<QByteArray, int>> CountsForName;
Q_GLOBAL_STATIC(CountsForName, gCountsForName)
typedef QReadWriteLock CountsForNameLock;
Q_GLOBAL_STATIC(CountsForNameLock, gCountsForNameLock)
Q_GLOBAL_STATIC_WITH_ARGS(bool, gEnabled, (false))

QByteArray qtjambi_to_qbytearray(JNIEnv *env, jstring java_string)
{
    QByteArray result;
    if (java_string){
        int length = env->GetStringLength(java_string);
        result.resize(length);
        env->GetStringUTFRegion(java_string, 0, length, result.data());
        JavaException::check(env QTJAMBI_STACKTRACEINFO );
    }else{
        result.clear();
    }
    return result;
}

QByteArray qtjambi_to_qbytearray(JNIEnv *env, jstring java_string);

// ---- TOOLS

void reset(TestAPI::DebugCounters counter, const QByteArray&className)
{
    QWriteLocker locker(gCountsForNameLock());
    if (className.isEmpty())
        (*gCountsForName)[counter].clear();
    else
        (*gCountsForName)[counter][className] = 0;
}

int count(TestAPI::DebugCounters counter, const QByteArray&className)
{
    QReadLocker locker(gCountsForNameLock());
    Q_UNUSED(locker)
    if (className.isEmpty()){
        int sum = 0;
        for(int value : (*gCountsForName)[counter])
            sum += value;
        return sum;
    }else
        return (*gCountsForName)[counter][className];
}

void autotestDebugCounter(TestAPI::DebugCounters counter, const char*className){
    QWriteLocker locker(gCountsForNameLock());
    ++((*gCountsForName)[counter][QByteArray(className)]);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_autotests_DebugTools_enableCounters) (JNIEnv *, jclass)
{
    QWriteLocker locker(gCountsForNameLock());
    *gEnabled = true;
    TestAPI::registerDebugCounter(autotestDebugCounter);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_autotests_DebugTools_disableCounters) (JNIEnv *, jclass)
{
    QWriteLocker locker(gCountsForNameLock());
    *gEnabled = false;
    TestAPI::registerDebugCounter(nullptr);
}

#define COUNTER_IMPLEMENTATION(NAME) \
extern "C" Q_DECL_EXPORT void JNICALL \
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_autotests_DebugTools_reset_1##NAME ) (JNIEnv *env, jclass, jstring className)\
{ \
    reset(TestAPI::DebugCounters::NAME, qtjambi_to_qbytearray(env, className)); \
} \
\
extern "C" Q_DECL_EXPORT jint JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_autotests_DebugTools_##NAME) (JNIEnv *env, jclass, jstring className) \
{ \
    return count(TestAPI::DebugCounters::NAME, qtjambi_to_qbytearray(env, className)); \
}

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

void qtjambi_increase_destructorFunctionCalledCount(const char*className){
    bool enabled;
    {
        enabled = gEnabled();
        QWriteLocker locker(gCountsForNameLock());
    }
    if(enabled){
        autotestDebugCounter(TestAPI::DebugCounters::destructorFunctionCalledCount, className);
    }
}
