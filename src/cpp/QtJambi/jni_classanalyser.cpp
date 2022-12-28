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

#include "global.h"
#include "exception.h"
#include "registryutil_p.h"
#include "qtjambi_cast.h"

extern "C" Q_DECL_EXPORT jboolean JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_ClassAnalyzerUtility_isGeneratedClass)
(JNIEnv *env,
 jclass,
 jstring className)
{
    try{
        return getTypeByJavaName(qtjambi_cast<QString>(env, className).replace(".", "/"))!=nullptr;
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
    return false;
}

extern "C" Q_DECL_EXPORT jclass JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_io_qt_internal_ClassAnalyzerUtility_getClass)
(JNIEnv * env,
 jclass, jobject e)
{
    return env->GetObjectClass(e);
}
