/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#if QT_VERSION >= QT_VERSION_CHECK(6, 6, 0)
#define QBACKINGSTORERHISUPPORT_P_H
#endif

#include <QtWidgets/private/qwidget_p.h>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/Cast>
#include <QtJambi/Template1Cast>
#include <QtJambi/ContainerCast>
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

extern "C" JNIEXPORT jboolean JNICALL Java_io_qt_widgets_QWidgetItem_hasWidgetItemV2(JNIEnv *__jni_env, jobject, jobject wdg){
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
extern "C" JNIEXPORT void JNICALL Java_io_qt_widgets_QInputDialog_open(JNIEnv *__jni_env, jobject _this, jobject obj, jobject metaMethod){
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
extern "C" JNIEXPORT void JNICALL Java_io_qt_widgets_QFileDialog_open(JNIEnv *__jni_env, jobject _this, jobject obj, jobject metaMethod){
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
extern "C" JNIEXPORT void JNICALL Java_io_qt_widgets_QFontDialog_open(JNIEnv *__jni_env, jobject _this, jobject obj, jobject metaMethod){
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
extern "C" JNIEXPORT void JNICALL Java_io_qt_widgets_QColorDialog_open(JNIEnv *__jni_env, jobject _this, jobject obj, jobject metaMethod){
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

const QObject* ownerOrApp(const QObject* obj){
    if(obj && QApplication::instance())
        obj = QApplication::instance();
    return obj;
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
    return ownerOrApp(p->m_model);
}

const QObject* getPointerOwner(const QTreeWidgetItem* item)
{
    return ownerOrApp(item->treeWidget());
}

const QObject* getPointerOwner(const QListWidgetItem* item)
{
    return ownerOrApp(item->listWidget());
}

const QObject* getPointerOwner(const QTableWidgetItem* item)
{
    return ownerOrApp(item->tableWidget());
}

template jobject qtjambi_cast<jobject,QPaintDevice::PaintDeviceMetric&>(JNIEnv *, QPaintDevice::PaintDeviceMetric&);

template jobject qtjambi_cast<jobject,QInputMethodEvent*&>(JNIEnv *, QInputMethodEvent*&);
template jobject qtjambi_cast<jobject,QActionEvent*&>(JNIEnv *, QActionEvent*&);
template jobject qtjambi_cast<jobject,QCloseEvent*&>(JNIEnv *, QCloseEvent*&);
template jobject qtjambi_cast<jobject,QContextMenuEvent*&>(JNIEnv *, QContextMenuEvent*&);
template jobject qtjambi_cast<jobject,QDragEnterEvent*&>(JNIEnv *, QDragEnterEvent*&);
template jobject qtjambi_cast<jobject,QDragLeaveEvent*&>(JNIEnv *, QDragLeaveEvent*&);
template jobject qtjambi_cast<jobject,QDragMoveEvent*&>(JNIEnv *, QDragMoveEvent*&);
template jobject qtjambi_cast<jobject,QDropEvent*&>(JNIEnv *, QDropEvent*&);
template jobject qtjambi_cast<jobject,QEnterEvent*&>(JNIEnv *, QEnterEvent*&);
template jobject qtjambi_cast<jobject,QHideEvent*&>(JNIEnv *, QHideEvent*&);
template jobject qtjambi_cast<jobject,QKeyEvent*&>(JNIEnv *, QKeyEvent*&);
template jobject qtjambi_cast<jobject,QMouseEvent*&>(JNIEnv *, QMouseEvent*&);
template jobject qtjambi_cast<jobject,QMoveEvent*&>(JNIEnv *, QMoveEvent*&);
template jobject qtjambi_cast<jobject,QPaintEvent*&>(JNIEnv *, QPaintEvent*&);
template jobject qtjambi_cast<jobject,QResizeEvent*&>(JNIEnv *, QResizeEvent*&);
template jobject qtjambi_cast<jobject,QShowEvent*&>(JNIEnv *, QShowEvent*&);
template jobject qtjambi_cast<jobject,QTabletEvent*&>(JNIEnv *, QTabletEvent*&);
template jobject qtjambi_cast<jobject,QPainter*&>(JNIEnv *, QPainter*&);
template jobject qtjambi_cast<jobject,QPaintEngine*&>(JNIEnv *, QPaintEngine*&);

template jobject qtjambi_cast<jobject,QIcon>(JNIEnv *, QIcon&&);
template jobject qtjambi_cast<jobject,const QIcon&>(JNIEnv *, const QIcon&);
template jobject qtjambi_cast<jobject,QColor>(JNIEnv *, QColor&&);
template jobject qtjambi_cast<jobject,const QColor&>(JNIEnv *, const QColor&);
template jobject qtjambi_cast<jobject,QTransform>(JNIEnv *, QTransform&&);
template jobject qtjambi_cast<jobject,const QTransform&>(JNIEnv *, const QTransform&);
template jobject qtjambi_cast<jobject,QCursor>(JNIEnv *, QCursor&&);
template jobject qtjambi_cast<jobject,const QCursor&>(JNIEnv *, const QCursor&);
template jobject qtjambi_cast<jobject,QPainterPath>(JNIEnv *, QPainterPath&&);
template jobject qtjambi_cast<jobject,const QPainterPath&>(JNIEnv *, const QPainterPath&);
template jobject qtjambi_cast<jobject,QRegion>(JNIEnv *, QRegion&&);
template jobject qtjambi_cast<jobject,const QRegion&>(JNIEnv *, const QRegion&);
template jobject qtjambi_cast<jobject,QBrush>(JNIEnv *, QBrush&&);
template jobject qtjambi_cast<jobject,const QBrush&>(JNIEnv *, const QBrush&);

template jobject qtjambi_cast<jobject,QGraphicsItem::Extension&>(JNIEnv *, QGraphicsItem::Extension&);
template jobject qtjambi_cast<jobject,QGraphicsItem::GraphicsItemChange&>(JNIEnv *, QGraphicsItem::GraphicsItemChange&);

template jobject qtjambi_cast<jobject,QGraphicsItemGroup*&>(JNIEnv *, QGraphicsItemGroup*&);
template jobject qtjambi_cast<jobject,QGraphicsItem*&>(JNIEnv *, QGraphicsItem*&);
template jobject qtjambi_cast<jobject,const QGraphicsItem*&>(JNIEnv *, const QGraphicsItem*&);
template jobject qtjambi_cast<jobject,QGraphicsEffect*&>(JNIEnv *, QGraphicsEffect*&);
template jobject qtjambi_cast<jobject,QGraphicsSceneWheelEvent*&>(JNIEnv *, QGraphicsSceneWheelEvent*&);
template jobject qtjambi_cast<jobject,const QStyleOptionGraphicsItem*&>(JNIEnv *, const QStyleOptionGraphicsItem*&);
template jobject qtjambi_cast<jobject,QGraphicsSceneMouseEvent*&>(JNIEnv *, QGraphicsSceneMouseEvent*&);
template jobject qtjambi_cast<jobject,QGraphicsSceneHoverEvent*&>(JNIEnv *, QGraphicsSceneHoverEvent*&);
template jobject qtjambi_cast<jobject,QGraphicsSceneDragDropEvent*&>(JNIEnv *, QGraphicsSceneDragDropEvent*&);
template jobject qtjambi_cast<jobject,QGraphicsSceneContextMenuEvent*&>(JNIEnv *, QGraphicsSceneContextMenuEvent*&);

template QList<QAction*> qtjambi_cast<QList<QAction*>,jobject&>(JNIEnv *, jobject&);
template jobject qtjambi_cast<jobject,QList<QAction*>>(JNIEnv *, QList<QAction*>&&);
template jobject qtjambi_cast<jobject,const QList<QAction*>&>(JNIEnv *, const QList<QAction*>&);

template jobject qtjambi_cast<jobject,QImage>(JNIEnv *, QImage&&);
template jobject qtjambi_cast<jobject,const QImage&>(JNIEnv *, const QImage&);

template jobject qtjambi_cast<jobject,QPixmap>(JNIEnv *, QPixmap&&);
template jobject qtjambi_cast<jobject,const QPixmap&>(JNIEnv *, const QPixmap&);

template jobject qtjambi_cast<jobject,QFont>(JNIEnv *, QFont&&);
template jobject qtjambi_cast<jobject,const QFont&>(JNIEnv *, const QFont&);
