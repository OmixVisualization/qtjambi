/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
#if defined(Q_OS_ANDROID)
#include <QtJambi/AndroidAPI>
#endif

namespace Java{
namespace QtPositioning {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/positioning,QNmeaPositionInfoSource$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/positioning/QGeoPositionInfo;Z)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(info,()Lio/qt/positioning/QGeoPositionInfo;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(hasFix,()Z)
)
}
namespace QtJambi {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/internal,SignalUtility$AbstractSignal,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(containingObject,()Lio/qt/QtSignalEmitterInterface;)
    QTJAMBI_REPOSITORY_DEFINE_METHOD(methodIndex,()I)
)
}
namespace QtCore {
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/core,QMetaMethod,
    QTJAMBI_REPOSITORY_DEFINE_METHOD(toSignal,(Lio/qt/core/QObject;)Lio/qt/core/QMetaObject$AbstractSignal;)
)
}
}

#if defined(Q_OS_ANDROID)
void initialize_meta_info_QtPositioning(){
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define ORG_QTPROJECT_QT "org/qtproject/qt/"
#else
#define ORG_QTPROJECT_QT "org/qtproject/qt5/"
#endif
    if(JniEnvironment env{300}){
        jclass cls = env->FindClass(ORG_QTPROJECT_QT "android/positioning/QtPositioning");
        if(env->ExceptionCheck())
            env->ExceptionClear();
        if(cls){
            jmethodID method = env->GetStaticMethodID(cls, "setContext", "(Landroid/content/Context;)V");
            if(env->ExceptionCheck())
                env->ExceptionClear();
            if(method){
                jobject activity = nullptr;
                try{
                    activity = Java::Android::QtNative::activity(env);
                }catch(...){}
                env->CallStaticVoidMethod(cls, method, activity);
                if(env->ExceptionCheck())
                    env->ExceptionClear();
            }
        }
    }
}
#endif
