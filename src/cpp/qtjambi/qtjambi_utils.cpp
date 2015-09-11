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

#include "qtjambi_global.h"
#include "qtjambi_utils.h"
#include "qtjambi_core.h"

bool qtjambi_resolve_classes(JNIEnv *env, ClassData *data)
{
    bool rv = true;
    // Resolve Data...
    for (int i=0; data[i].cl; ++i) {
        if (*data[i].cl)  // function is restartable
            continue;

        jclass cl = qtjambi_find_class(env, data[i].name);
        if (cl == 0) {
            // Handle possible exception
            qtjambi_exception_check(env);
            rv = false;
            continue;  // best-effort try next one
        }

        jclass gcls = (jclass) env->NewGlobalRef(cl);
        if (gcls == 0) {
            qtjambi_exception_check(env);
            rv = false;
            goto next;  // best-effort try next one
        }

        *data[i].cl = gcls;

next:
        env->DeleteLocalRef(cl);
    }

    return rv;
}

void qtjambi_unresolve_classes(JNIEnv *env, ClassData *data)
{
    for (int i=0; data[i].cl; ++i) {
        if (*data[i].cl) {
            env->DeleteGlobalRef(*data[i].cl);
            *data[i].cl = 0;
        }
    }
}

bool qtjambi_resolve_fields(JNIEnv *env, bool bf_static, FieldData *data)
{
    bool rv = true;
    // Resolve fields
    for (int i=0; data[i].cl; ++i) {
        if (*data[i].id)  // function is restartable
            continue;
        if (*data[i].cl == 0) {
            rv = false;   // no class to lookup fieldID against
            continue;
        }

        jfieldID fid;
        if (bf_static)
            fid = env->GetStaticFieldID(*data[i].cl, data[i].name, data[i].signature);
        else
            fid = env->GetFieldID(*data[i].cl, data[i].name, data[i].signature);
        Q_ASSERT_X(fid, data[i].name, data[i].signature);

        if (fid == 0) {
            // Handle possible exception
            qtjambi_exception_check(env);
            rv = false;
            continue;  // best-effort try next one
        }

        *data[i].id = fid;
    }

    return rv;
}

void qtjambi_unresolve_fields(JNIEnv *env, FieldData *data)
{
    Q_UNUSED(env);
    for (int i=0; data[i].cl; ++i) {
        if (*data[i].id)  // it is a jfieldID so no memory allocation exists
            *data[i].id = 0;
    }
}

bool qtjambi_resolve_methods(JNIEnv *env, bool bf_static, MethodData *data)
{
    bool rv = true;
    // Resolve static functions
    for (int i=0; data[i].cl; ++i) {
        if (*data[i].id)  // function is restartable
            continue;
        if (*data[i].cl == 0) {
            rv = false;   // no class to lookup methodID against
            continue;
        }

        jmethodID mid;
        if(bf_static)
            mid = env->GetStaticMethodID(*data[i].cl, data[i].name, data[i].signature);
        else
            mid = env->GetMethodID(*data[i].cl, data[i].name, data[i].signature);
        Q_ASSERT_X(mid, data[i].name, data[i].signature);

        if (mid == 0) {
            // Handle possible exception
            qtjambi_exception_check(env);
            rv = false;
            continue;  // best-effort try next one
        }

        *data[i].id = mid;
    }

    return rv;
}

void qtjambi_unresolve_methods(JNIEnv *env, MethodData *data)
{
    Q_UNUSED(env);
    for (int i=0; data[i].cl; ++i) {
        if (*data[i].id)  // it is a jmethodID so no memory allocation exists
            *data[i].id = 0;
    }
}
