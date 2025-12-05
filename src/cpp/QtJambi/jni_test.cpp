/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include <QtCore/qcompilerdetection.h>
QT_WARNING_DISABLE_DEPRECATED

#include "pch_p.h"

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_TestUtility_hasDeleteLaterEvents(JNIEnv *, jclass){
#if defined(QTJAMBI_DEBUG_TOOLS)
    return QtJambiLink::hasDeleteLaterEvents();
#else
    return false;
#endif
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_TestUtility_hasDebugTools(JNIEnv *, jclass){
#if defined(QTJAMBI_DEBUG_TOOLS)
    return true;
#else
    return false;
#endif
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_internal_TestUtility_objectCount(JNIEnv * env, jclass){
#if defined(QTJAMBI_DEBUG_TOOLS)
    return QtJambiLink::dumpLinks(env);
#else
    Q_UNUSED(env)
    return 0;
#endif
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_TestUtility_flushOut(JNIEnv *, jclass){
    fflush(stdout);
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_TestUtility_flushErr(JNIEnv *, jclass){
    fflush(stderr);
}

extern "C" JNIEXPORT jint JNICALL Java_io_qt_internal_TestUtility_objectCacheMode(JNIEnv *, jclass){
    return QtJambiLink::getObjectCacheMode();
}

extern "C" JNIEXPORT void JNICALL Java_io_qt_internal_TestUtility_setObjectCacheMode(JNIEnv *, jclass, jint object_cache_mode){
    QtJambiLink::setObjectCacheMode(object_cache_mode);
}

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_internal_TestUtility_isSharedPointer(JNIEnv *__jni_env, jclass, QtJambiNativeID nativeId){
    try{
        QSharedPointer<QtJambiLink> link = QtJambiLink::fromNativeId(nativeId);
        return link && link->isSmartPointer();
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;

}
