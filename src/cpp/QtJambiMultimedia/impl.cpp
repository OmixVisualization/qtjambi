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
#include "hashes.h"
#include "utils_p.h"
#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>

namespace Java{
namespace QtMultimedia{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/multimedia,QMediaService$Result,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Z)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(result,Ljava/lang/Object;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(continuous,Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/multimedia,QMediaService$ListResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Lio/qt/core/QList;Z)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/multimedia,QAbstractVideoBuffer$MapResult,
    QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/nio/ByteBuffer;IZ)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(data,Ljava/nio/ByteBuffer;)
    QTJAMBI_REPOSITORY_DEFINE_FIELD(bytesPerLine,I)
)
}
}


void initialize_meta_info_QtMultimedia(){
#if defined(Q_OS_ANDROID)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define ORG_QTPROJECT_QT "org/qtproject/qt/"
#else
#define ORG_QTPROJECT_QT "org/qtproject/qt5/"
#endif
    if(JniEnvironment env{300}){
        jobject activity = nullptr;
        try{
            activity = Java::Android::QtNative::activity(env);
        }catch(...){}
        jclass cls = nullptr;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        cls = env->FindClass(ORG_QTPROJECT_QT "android/multimedia/QtAudioDeviceManager");
        if(env->ExceptionCheck())
            env->ExceptionClear();
        if(cls){
            jmethodID method = env->GetStaticMethodID(cls, "setContext", "(Landroid/content/Context;)V");
            if(env->ExceptionCheck())
                env->ExceptionClear();
            if(method){
                env->CallStaticVoidMethod(cls, method, activity);
                if(env->ExceptionCheck())
                    env->ExceptionClear();
            }
        }
#endif

        cls = env->FindClass(ORG_QTPROJECT_QT "android/multimedia/QtMultimediaUtils");
        if(env->ExceptionCheck())
            env->ExceptionClear();
        if(cls){
            jmethodID method = env->GetStaticMethodID(cls, "setContext", "(Landroid/content/Context;)V");
            if(env->ExceptionCheck())
                env->ExceptionClear();
            if(method){
                env->CallStaticVoidMethod(cls, method, activity);
                if(env->ExceptionCheck())
                    env->ExceptionClear();
            }

            method = env->GetStaticMethodID(cls, "setActivity", "(Landroid/app/Activity;Ljava/lang/Object;)V");
            if(env->ExceptionCheck())
                env->ExceptionClear();
            if(method){
                jobject activityDelegate = nullptr;
                try{
                    activityDelegate = Java::Android::QtNative::activity(env);
                }catch(...){}
                env->CallStaticVoidMethod(cls, method, activity, activityDelegate);
                if(env->ExceptionCheck())
                    env->ExceptionClear();
            }
        }
    }
#endif
}
