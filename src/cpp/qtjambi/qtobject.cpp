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

#include "qtjambi_core.h"

#include <QtCore/QMetaType>
#include <QtCore/QRect>
#include <QtCore/QTime>
#include <QtCore/QSize>
#include <QtCore/QBasicTimer>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>

#include <QtGui/QMouseEvent>
#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QCursor>
#include <QtGui/QIcon>
#include <QtGui/QPainter>
#include <QtGui/QPolygon>

#include <QDebug>

#if defined(QTJAMBI_DEBUG_TOOLS)
#  include "qtjambidebugtools_p.h"
#endif

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiObject_dispose)
    (JNIEnv *env, jobject java)
{
    QtJambiLink *link = QtJambiLink::findLink(env, java);
    if (link) {
        link->javaObjectDisposed(env);
    }
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiObject_nativePointer)
    (JNIEnv *env, jobject javaRef)
{
    QtJambiLink *link = QtJambiLink::findLink(env, javaRef);

    if (link != 0)
        return qtjambi_from_cpointer(env, link->object(), 8, 1);
    else
        return 0;
}

extern "C" Q_DECL_EXPORT jobject JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiObject__1_1qt_1reassignLink)
    (JNIEnv *env, jclass, jlong old_native_id, jclass clazz, jobject constructor)
{
    QtJambiLink *link = reinterpret_cast<QtJambiLink *>(old_native_id);
    Q_ASSERT(link);

    jmethodID methodId = env->FromReflectedMethod(constructor);
    Q_ASSERT(methodId);

    jobject new_object = env->NewObject(clazz, methodId, 0);
    Q_ASSERT(new_object);

    QtJambiLink *new_link = 0;
    if (link->isQObject()) {
        QObject *qobject = link->qobject();
        link->resetObject(env);
        new_link = QtJambiLink::createLinkForQObject(env, new_object, qobject, Q_NULLPTR, Q_NULLPTR);
        switch (link->ownership()) {
        case QtJambiLink::JavaOwnership:
            new_link->setJavaOwnership(env, new_object);
            break;
        case QtJambiLink::SplitOwnership:
            new_link->setSplitOwnership(env, new_object);
            break;
        default: // default is cpp ownership for qobjects
            break;
        }
    } else {
        void *ptr = link->pointer();
        bool wasCached = link->isCached();
        QString java_name = qtjambi_class_name(env, clazz);
        link->resetObject(env);

        // Create new link.
        new_link = QtJambiLink::createLinkForObject(env, new_object, ptr, java_name, wasCached, Q_NULLPTR, Q_NULLPTR);
        new_link->setMetaType(link->metaType());
    }

    delete link;
    return new_object;
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiObject_finalize)
    (JNIEnv *env, jobject java)
{
    Q_ASSERT(env != 0);

#if defined(QTJAMBI_DEBUG_TOOLS)
    QString className = qtjambi_object_class_name(env, java).split(".").last();

    // Count objects severed from their link as well
    qtjambi_increase_finalizedCount(className);
#endif

    if (QtJambiLink *link = QtJambiLink::findLink(env, java)) {
        link->javaObjectFinalized(env);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiObject_disableGarbageCollection)
    (JNIEnv *env, jobject object)
{
    if (QtJambiLink *link = QtJambiLink::findLink(env, object)) {
        link->disableGarbageCollection(env, object);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiObject_reenableGarbageCollection)
    (JNIEnv *env, jobject object)
{
    if (QtJambiLink *link = QtJambiLink::findLink(env, object)) {
        link->setDefaultOwnership(env, object);
    }
}

extern "C" Q_DECL_EXPORT void JNICALL
QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_internal_QtJambiObject_setJavaOwnership)
    (JNIEnv *env, jobject object)
{
    if (QtJambiLink *link = QtJambiLink::findLink(env, object)) {
        link->setJavaOwnership(env, object);
    }
}
