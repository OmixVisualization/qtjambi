/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "pch_p.h"

#include <QtCore/QPointF>
#include <AppKit/AppKit.h>
#import <objc/runtime.h>

jint nse_type(void* message)
{
    NSEvent* nsEvent = static_cast<NSEvent*>(message);
    return (jint)[nsEvent type];
}

#define NS_EVENT_METHOD(TYPE,METHOD)\
TYPE nse_##METHOD(JNIEnv * __jni_env, void* message)\
{\
    NSEvent* nsEvent = static_cast<NSEvent*>(message);\
    QtJambiAPI::checkNullPointer(__jni_env, nsEvent);\
    return (TYPE)[nsEvent METHOD];\
}
#define NS_EVENT_STATIC_METHOD(TYPE,METHOD)\
TYPE nse_##METHOD(JNIEnv *)\
{\
        return (TYPE)[NSEvent METHOD];\
}
#define NS_EVENT_STRING_METHOD(METHOD)\
jstring nse_##METHOD(JNIEnv * __jni_env, void* message)\
{\
        NSEvent* nsEvent = static_cast<NSEvent*>(message);\
        QtJambiAPI::checkNullPointer(__jni_env, nsEvent);\
        const NSString *string = [nsEvent METHOD];\
        return string ? __jni_env->NewStringUTF([string UTF8String]) : nullptr;\
}
#define NS_EVENT_POINT_METHOD(METHOD)\
jobject nse_##METHOD(JNIEnv * __jni_env, void* message)\
{\
        NSEvent* nsEvent = static_cast<NSEvent*>(message);\
        QtJambiAPI::checkNullPointer(__jni_env, nsEvent);\
        NSPoint point = [nsEvent METHOD];\
        return qtjambi_cast<jobject>(__jni_env, QPointF{point.x, point.y});\
}
#define NS_EVENT_STATIC_POINT_METHOD(METHOD)\
jobject nse_##METHOD(JNIEnv * __jni_env)\
{\
        NSPoint point = [NSEvent METHOD];\
        return qtjambi_cast<jobject>(__jni_env, QPointF{point.x, point.y});\
}

NS_EVENT_STATIC_METHOD(jint,doubleClickInterval)
NS_EVENT_METHOD(jint,buttonNumber)
NS_EVENT_METHOD(jint,clickCount)
NS_EVENT_METHOD(jlong,associatedEventsMask)
NS_EVENT_METHOD(jint,modifierFlags)
NS_EVENT_METHOD(jint,type)
NS_EVENT_METHOD(jshort,subtype)
NS_EVENT_METHOD(double,timestamp)
NS_EVENT_METHOD(jlong,window)
NS_EVENT_METHOD(jshort,keyCode)
NS_EVENT_STATIC_METHOD(double,keyRepeatInterval)
NS_EVENT_STATIC_METHOD(double,keyRepeatDelay)
NS_EVENT_METHOD(double,deltaX)
NS_EVENT_METHOD(double,deltaY)
NS_EVENT_METHOD(double,deltaZ)
NS_EVENT_METHOD(jboolean,hasPreciseScrollingDeltas)
NS_EVENT_METHOD(double,scrollingDeltaX)
NS_EVENT_METHOD(double,scrollingDeltaY)
NS_EVENT_METHOD(jint,momentumPhase)
NS_EVENT_METHOD(jboolean,isDirectionInvertedFromDevice)
NS_EVENT_METHOD(jboolean,isARepeat)

NS_EVENT_STRING_METHOD(characters)
NS_EVENT_STRING_METHOD(charactersIgnoringModifiers)

NS_EVENT_STATIC_POINT_METHOD(mouseLocation)
NS_EVENT_POINT_METHOD(locationInWindow)

jobject nse_asBuffer(JNIEnv * __jni_env, void* message)
{
    NSEvent* nsEvent = static_cast<NSEvent*>(message);
    QtJambiAPI::checkNullPointer(__jni_env, nsEvent);
    return __jni_env->NewDirectByteBuffer(message, class_getInstanceSize([nsEvent class]));
}
