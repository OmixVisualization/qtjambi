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

#include <QtJambi/Global>
#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/private/qobject_p.h>
#include <QtWidgets/QtWidgets>
#include <QtWidgets/private/qwidget_p.h>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/qtjambi_cast.h>
#include "utils_p.h"

namespace Java{
namespace QtWidgets{
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QGraphicsItem$BlockedByModalPanelInfo,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ZLio/qt/widgets/QGraphicsItem;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QFileDialog$Result,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(Ljava/lang/Object;Ljava/lang/String;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QFormLayout$ItemInfo,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(ILio/qt/widgets/QFormLayout$ItemInfo;)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QGridLayout$ItemInfo,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(IIII)
)

QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QSplitter$Range,
                                QTJAMBI_REPOSITORY_DEFINE_CONSTRUCTOR(II)
)
QTJAMBI_REPOSITORY_DEFINE_CLASS(io/qt/widgets,QStyle,
                                QTJAMBI_REPOSITORY_DEFINE_STATIC_METHOD(findSubControl,(II)Lio/qt/widgets/QStyle$SubControl;)
)
}
}

extern "C" JNIEXPORT jlong JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QItemEditorCreator_constructorHandle)
(JNIEnv *__jni_env,
 jclass,
 jobject constructor)
{
    return jlong(__jni_env->FromReflectedMethod(constructor));
}

extern "C" JNIEXPORT jobject JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QItemEditorCreator_construct)
(JNIEnv *__jni_env,
 jclass,
 jclass cls,
 jlong constructorHandle,
 jobject parent)
{
    return __jni_env->NewObject(cls, jmethodID(constructorHandle), parent);
}

extern "C" Q_DECL_EXPORT jboolean JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QWidgetItem_hasWidgetItemV2)
(JNIEnv *__jni_env,
 jobject,
 jobject wdg)
{
    jboolean result = false;
    QTJAMBI_TRY{
        const QWidget *widget = qtjambi_cast<const QWidget*>(__jni_env, wdg);
        if(widget){
            const QWidgetPrivate *wd = static_cast<const QWidgetPrivate *>(QObjectPrivate::get(widget));
            Q_ASSERT(wd);
            result = wd->widgetItem!=nullptr;
        }
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
    return result;
}

// QInputDialog::open(QObject * receiver, const char * member)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QInputDialog_open)
(JNIEnv *__jni_env,
 jobject _this,
 jobject obj, jobject metaMethod)
{
    QTJAMBI_TRY{
#if QT_CONFIG(filedialog)
        QInputDialog *__qt_this = QtJambiAPI::convertJavaObjectToNative<QInputDialog>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QInputDialog::open(QObject * receiver, const char * member)", __qt_this)
        QObject *object = qtjambi_cast<QObject*>(__jni_env, obj);
        QtJambiAPI::checkNullPointer(__jni_env, object);
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
        Q_UNUSED(_this)
        Q_UNUSED(options0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(colordialog)
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}


// QFileDialog::open(QObject * receiver, const char * member)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QFileDialog_open)
(JNIEnv *__jni_env,
 jobject _this,
 jobject obj, jobject metaMethod)
{
    QTJAMBI_TRY{
#if QT_CONFIG(filedialog)
        QFileDialog *__qt_this = QtJambiAPI::convertJavaObjectToNative<QFileDialog>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QFileDialog::open(QObject * receiver, const char * member)", __qt_this)
        QObject *object = qtjambi_cast<QObject*>(__jni_env, obj);
        QtJambiAPI::checkNullPointer(__jni_env, object);
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
        Q_UNUSED(_this)
        Q_UNUSED(options0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(colordialog)
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

// QFontDialog::open(QObject * receiver, const char * member)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QFontDialog_open)
(JNIEnv *__jni_env,
 jobject _this,
 jobject obj, jobject metaMethod)
{
    QTJAMBI_TRY{
#if QT_CONFIG(fontdialog)
        QFontDialog *__qt_this = QtJambiAPI::convertJavaObjectToNative<QFontDialog>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QFontDialog::open(QObject * receiver, const char * member)", __qt_this)
        QObject *object = qtjambi_cast<QObject*>(__jni_env, obj);
        QtJambiAPI::checkNullPointer(__jni_env, object);
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
        Q_UNUSED(_this)
        Q_UNUSED(options0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(colordialog)
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}


// QColorDialog::open(QObject * receiver, const char * member)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_widgets_QColorDialog_open)
(JNIEnv *__jni_env,
 jobject _this,
 jobject obj, jobject metaMethod)
{
    QTJAMBI_TRY{
#if QT_CONFIG(colordialog)
        QColorDialog *__qt_this = QtJambiAPI::convertJavaObjectToNative<QColorDialog>(__jni_env, _this);
        QtJambiAPI::checkNullPointer(__jni_env, __qt_this);
        QTJAMBI_NATIVE_INSTANCE_METHOD_CALL("QColorDialog::open(QObject * receiver, const char * member)", __qt_this)
        QObject *object = qtjambi_cast<QObject*>(__jni_env, obj);
        QtJambiAPI::checkNullPointer(__jni_env, object);
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
        Q_UNUSED(_this)
        Q_UNUSED(options0)
        JavaException::raiseQNoImplementationException(__jni_env, "The method has no implementation on this platform." QTJAMBI_STACKTRACEINFO );
#endif // QT_CONFIG(colordialog)
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(__jni_env);
    }QTJAMBI_TRY_END
}

const QObject* getPointerOwner(const QGraphicsLayoutItem* __qt_this){
    if(__qt_this->graphicsItem()){
        return getPointerOwner(__qt_this->graphicsItem());
    }else if(__qt_this->parentLayoutItem()){
        return getPointerOwner(__qt_this->parentLayoutItem());
    }
    try{
        return dynamic_cast<const QObject*>(__qt_this);
    }catch(...){}
    return nullptr;
}

const QObject* getPointerOwner(const QGraphicsItem* __qt_this)
{
    if(__qt_this->toGraphicsObject()){
        return __qt_this->toGraphicsObject();
    }else if(__qt_this->scene()){
        return __qt_this->scene();
    }else if(__qt_this->parentObject()){
        return __qt_this->parentObject();
    }else if(__qt_this->parentItem()){
        return getPointerOwner(__qt_this->parentItem());
    }
    try{
        return dynamic_cast<const QObject*>(__qt_this);
    }catch(...){}
    return nullptr;
}

const QObject* getPointerOwner(const QTreeWidgetItemIterator* iter)
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
