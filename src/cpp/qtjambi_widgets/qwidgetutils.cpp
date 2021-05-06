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

#include <qtjambi/qtjambi_global.h>
#include <QtCore/QObject>
#include <QtCore/private/qobject_p.h>
#include <QtWidgets/QtWidgets>
#include <QtWidgets/private/qwidget_p.h>
#include <qtjambi/qtjambi_core.h>
#include <qtjambi/qtjambi_cast.h>
#include "qwidgetutils.h"

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QWidgetItem_hasWidgetItemV2)
(JNIEnv *__jni_env,
 jobject,
 jobject wdg)
{
    try{
        const QWidget *widget = qtjambi_cast<const QWidget*>(__jni_env, wdg);
        if(widget){
            const QWidgetPrivate *wd = static_cast<const QWidgetPrivate *>(QObjectPrivate::get(widget));
            Q_ASSERT(wd);
            return wd->widgetItem!=nullptr;
        }
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
    return false;
}

// QInputDialog::open(QObject * receiver, const char * member)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QInputDialog_open)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID objectId, jobject metaMethod)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QInputDialog::open(QObject * receiver, const char * member)")
    try{
#if QT_CONFIG(filedialog)
        QInputDialog *__qt_this = qtjambi_object_from_nativeId<QInputDialog>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *object = qtjambi_object_from_nativeId<QObject>(objectId);
        qtjambi_check_resource(__jni_env, object);
        QMetaMethod method = qtjambi_cast<QMetaMethod>(__jni_env, metaMethod);
        QByteArray signature;
        if(method.methodType()==QMetaMethod::Signal){
            signature += "2";
        }else{
            signature += "1";
        }
        signature += method.methodSignature();
        __qt_this->open(object, signature);
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(options0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(colordialog)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}


// QFileDialog::open(QObject * receiver, const char * member)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QFileDialog_open)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID objectId, jobject metaMethod)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFileDialog::open(QObject * receiver, const char * member)")
    try{
#if QT_CONFIG(filedialog)
        QFileDialog *__qt_this = qtjambi_object_from_nativeId<QFileDialog>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *object = qtjambi_object_from_nativeId<QObject>(objectId);
        qtjambi_check_resource(__jni_env, object);
        QMetaMethod method = qtjambi_cast<QMetaMethod>(__jni_env, metaMethod);
        QByteArray signature;
        if(method.methodType()==QMetaMethod::Signal){
            signature += "2";
        }else{
            signature += "1";
        }
        signature += method.methodSignature();
        __qt_this->open(object, signature);
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(options0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(colordialog)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

// QFontDialog::open(QObject * receiver, const char * member)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QFontDialog_open)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID objectId, jobject metaMethod)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QFontDialog::open(QObject * receiver, const char * member)")
    try{
#if QT_CONFIG(fontdialog)
        QFontDialog *__qt_this = qtjambi_object_from_nativeId<QFontDialog>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *object = qtjambi_object_from_nativeId<QObject>(objectId);
        qtjambi_check_resource(__jni_env, object);
        QMetaMethod method = qtjambi_cast<QMetaMethod>(__jni_env, metaMethod);
        QByteArray signature;
        if(method.methodType()==QMetaMethod::Signal){
            signature += "2";
        }else{
            signature += "1";
        }
        signature += method.methodSignature();
        __qt_this->open(object, signature);
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(options0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(colordialog)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}


// QColorDialog::open(QObject * receiver, const char * member)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QColorDialog_open)
(JNIEnv *__jni_env,
 jclass,
 QtJambiNativeID __this_nativeId,
 QtJambiNativeID objectId, jobject metaMethod)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QColorDialog::open(QObject * receiver, const char * member)")
    try{
#if QT_CONFIG(colordialog)
        QColorDialog *__qt_this = qtjambi_object_from_nativeId<QColorDialog>(__this_nativeId);
        qtjambi_check_resource(__jni_env, __qt_this);
        QObject *object = qtjambi_object_from_nativeId<QObject>(objectId);
        qtjambi_check_resource(__jni_env, object);
        QMetaMethod method = qtjambi_cast<QMetaMethod>(__jni_env, metaMethod);
        QByteArray signature;
        if(method.methodType()==QMetaMethod::Signal){
            signature += "2";
        }else{
            signature += "1";
        }
        signature += method.methodSignature();
        __qt_this->open(object, signature);
#else
        Q_UNUSED(__this_nativeId)
        Q_UNUSED(options0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(colordialog)
    }catch(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }
}

const QObject* qtjambi_get_owner(const QGraphicsLayoutItem* __qt_this){
    if(__qt_this->graphicsItem()){
        return qtjambi_get_owner(__qt_this->graphicsItem());
    }else if(__qt_this->parentLayoutItem()){
        return qtjambi_get_owner(__qt_this->parentLayoutItem());
    }else{
        return nullptr;
    }
}

const QObject* qtjambi_get_owner(const QGraphicsItem* __qt_this)
{
    if(__qt_this->toGraphicsObject()){
        return __qt_this->toGraphicsObject();
    }else if(__qt_this->scene()){
        return __qt_this->scene();
    }else if(__qt_this->parentObject()){
        return __qt_this->parentObject();
    }else if(__qt_this->parentItem()){
        return qtjambi_get_owner(__qt_this->parentItem());
    }else return nullptr;
}

const QObject* qtjambi_get_owner(const QTreeWidgetItemIterator* iter)
{
    struct TreeWidgetItemIteratorPrivate{
        int m_currentIndex;
        QObject *m_model;
    };

    struct TreeWidgetItemIterator{
        QScopedPointer<TreeWidgetItemIteratorPrivate> d_ptr;
    };
    const TreeWidgetItemIteratorPrivate* p = reinterpret_cast<const TreeWidgetItemIterator*>(iter)->d_ptr.get();
    return p->m_model;
    /*
    if(!iter->operator*()){
        iter->operator--();
    }
    if(iter->operator*() && iter->operator*()->treeWidget()){
        return iter->operator*()->treeWidget()->model();
    }
    return nullptr;*/
}
