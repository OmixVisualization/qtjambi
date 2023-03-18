/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/QMutex>
#include "utils_p.h"

#include <QtSensors/QSensorBackend>
#ifndef QTJAMBI_NO_SENSORS_PRIVATE
#include <QtSensors/private/qsensor_p.h>
#include <QtCore/private/qobject_p.h>
#endif
#include <QtJambi/Cast>


namespace Java{
namespace QtCore {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaMethod,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(methodSignature,()Ljava/lang/String;)
)
}
}

jobject qtjambi_setReading(JNIEnv *env,
                           QSensorBackend *__qt_this,
                           jclass readingClass,
                           jobject readingObject){
#ifndef QTJAMBI_NO_SENSORS_PRIVATE
    jmethodID constructor = JavaAPI::resolveMethod(env, "<init>", "(Lio/qt/core/QObject;)V", readingClass, false);
    jobject backend = qtjambi_cast<jobject>(env, __qt_this);
    if(!readingObject)
        readingObject = env->NewObject(readingClass, constructor, backend);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    jobject filter_reading = env->NewObject(readingClass, constructor, backend);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    jobject cache_reading = env->NewObject(readingClass, constructor, backend);
    JavaException::check(env QTJAMBI_STACKTRACEINFO );
    QSensorPrivate* d = reinterpret_cast<QSensorPrivate*>(QObjectPrivate::get(__qt_this->sensor()));
    d->device_reading = qtjambi_cast<QSensorReading*>(env, readingObject);
    d->filter_reading = qtjambi_cast<QSensorReading*>(env, filter_reading);
    d->cache_reading = qtjambi_cast<QSensorReading*>(env, cache_reading);
    return readingObject;
#else
    Q_UNUSED(__qt_this)
    Q_UNUSED(readingClass)
    Q_UNUSED(readingObject)
    JavaException::raiseQNoImplementationException(env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
    return nullptr;
#endif
}

jobject qtjambi_setReading(JNIEnv *env,
                           QSensorBackend *__qt_this,
                           jobject readingClass){
    return qtjambi_setReading(env, __qt_this, env->GetObjectClass(readingClass), readingClass);
}

jobject qtjambi_setReading(JNIEnv *env,
                           QSensorBackend *__qt_this,
                           jclass readingClass){
    return qtjambi_setReading(env, __qt_this, readingClass, nullptr);
}
