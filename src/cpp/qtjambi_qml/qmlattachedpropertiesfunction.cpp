/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QtGlobal>
#include <qtjambi/qtjambi_global.h>

#include "qmlattachedpropertiesfunction.h"
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_functionpointer.h>
#include <qtjambi/qtjambi_jobjectwrapper.h>

QObject* attachedProperties(const JObjectWrapper& clazzWrapper, jmethodID qmlAttachedProperties, QObject* parent){
    if(JNIEnv * env = qtjambi_current_environment()){
        QTJAMBI_JNI_LOCAL_FRAME(env, 200)
        jobject result = env->CallStaticObjectMethod(jclass(clazzWrapper.object()), qmlAttachedProperties, qtjambi_from_QObject(env, parent));
        qtjambi_throw_java_exception(env);
        qtjambi_rethrowing(env,
            return qtjambi_to_qobject(env, result);
        )
    }
    return nullptr;
}

QQmlAttachedPropertiesFunc attachedPropertiesFunc(JNIEnv * env, jclass clazz, jobject method){
    if(!method)
        return nullptr;
    uint hash = 1;
    hash = 31 * hash + uint(qtjambi_java_object_hashcode(env, method));
    hash = 31 * hash + uint(qtjambi_java_object_hashcode(env, clazz));
    jmethodID qmlAttachedProperties = env->FromReflectedMethod(method);
    JObjectWrapper clazzWrapper(env, clazz);
    return qtjambi_function_pointer<64,QObject*(QObject*)>([clazzWrapper, qmlAttachedProperties](QObject* parent) -> QObject* {
        return attachedProperties(clazzWrapper, qmlAttachedProperties, parent);
    }, hash);
}


