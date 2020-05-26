/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include "qmlcreateparentfunction.h"
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>

QObject* createParent(const JObjectWrapper& clazzWrapper, jmethodID constructor, QObject* parent){
    if(JNIEnv * env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        jobject result = env->NewObject(jclass(clazzWrapper.object()), constructor, qtjambi_from_QObject(env, parent));
        qtjambi_throw_java_exception(env)
        qtjambi_rethrowing(env,
            return qtjambi_to_qobject(env, result);
        )
    }
    return nullptr;
}

CreateParentFunction createParentFunction(JNIEnv * env, jclass clazz, jmethodID constructor){
    uint hash = 1;
    hash = 31 * hash + qHash(qint64(constructor));
    hash = 31 * hash + uint(qtjambi_java_object_hashcode(env, clazz));
    JObjectWrapper clazzWrapper(env, clazz);
    return qtjambi_function_pointer<8 /*=512 options*/,QObject*(QObject*)>([clazzWrapper, constructor](QObject* parent) -> QObject* {
        return createParent(clazzWrapper, constructor, parent);
    }, hash);
}
