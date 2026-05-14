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

#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtWidgets/QtWidgets>
#include <QtJambi/Global>
#include <QtJambi/JavaAPI>
#include <QtJambi/Cast>

namespace Java{
namespace QtWidgets{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGraphicsItem$BlockedByModalPanelInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFileDialog$Result,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QFormLayout$ItemInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QGridLayout$ItemInfo,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSplitter$Range,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QStyle,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_OBJECT_METHOD(findSubControl))
}
}

const QObject* getPointerOwner(const QGraphicsLayoutItem* __qt_this);
const QObject* getPointerOwner(const QGraphicsItem* __qt_this);
const QObject* getPointerOwner(const QTreeWidgetItemIterator* __qt_this);
const QObject* getPointerOwner(const QTreeWidgetItem* __qt_this);
const QObject* getPointerOwner(const QListWidgetItem* __qt_this);
const QObject* getPointerOwner(const QTableWidgetItem* __qt_this);

#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
namespace QtJambiPrivate{
template<>
struct supports_qHash<QList<QPair<qreal,qreal>>> : std::false_type{};
}
#endif

extern template jobject qtjambi_cast<jobject,QPaintDevice::PaintDeviceMetric&>(JNIEnv *, QPaintDevice::PaintDeviceMetric&);

extern template jobject qtjambi_cast<jobject,QInputMethodEvent*&>(JNIEnv *, QInputMethodEvent*&);
extern template jobject qtjambi_cast<jobject,QActionEvent*&>(JNIEnv *, QActionEvent*&);
extern template jobject qtjambi_cast<jobject,QCloseEvent*&>(JNIEnv *, QCloseEvent*&);
extern template jobject qtjambi_cast<jobject,QContextMenuEvent*&>(JNIEnv *, QContextMenuEvent*&);
extern template jobject qtjambi_cast<jobject,QDragEnterEvent*&>(JNIEnv *, QDragEnterEvent*&);
extern template jobject qtjambi_cast<jobject,QDragLeaveEvent*&>(JNIEnv *, QDragLeaveEvent*&);
extern template jobject qtjambi_cast<jobject,QDragMoveEvent*&>(JNIEnv *, QDragMoveEvent*&);
extern template jobject qtjambi_cast<jobject,QDropEvent*&>(JNIEnv *, QDropEvent*&);
extern template jobject qtjambi_cast<jobject,QEnterEvent*&>(JNIEnv *, QEnterEvent*&);
extern template jobject qtjambi_cast<jobject,QHideEvent*&>(JNIEnv *, QHideEvent*&);
extern template jobject qtjambi_cast<jobject,QKeyEvent*&>(JNIEnv *, QKeyEvent*&);
extern template jobject qtjambi_cast<jobject,QMouseEvent*&>(JNIEnv *, QMouseEvent*&);
extern template jobject qtjambi_cast<jobject,QMoveEvent*&>(JNIEnv *, QMoveEvent*&);
extern template jobject qtjambi_cast<jobject,QPaintEvent*&>(JNIEnv *, QPaintEvent*&);
extern template jobject qtjambi_cast<jobject,QResizeEvent*&>(JNIEnv *, QResizeEvent*&);
extern template jobject qtjambi_cast<jobject,QShowEvent*&>(JNIEnv *, QShowEvent*&);
extern template jobject qtjambi_cast<jobject,QTabletEvent*&>(JNIEnv *, QTabletEvent*&);
extern template jobject qtjambi_cast<jobject,QPainter*&>(JNIEnv *, QPainter*&);
extern template jobject qtjambi_cast<jobject,QPaintEngine*&>(JNIEnv *, QPaintEngine*&);

extern template jobject qtjambi_cast<jobject,QIcon>(JNIEnv *, QIcon&&);
extern template jobject qtjambi_cast<jobject,const QIcon&>(JNIEnv *, const QIcon&);
extern template jobject qtjambi_cast<jobject,QColor>(JNIEnv *, QColor&&);
extern template jobject qtjambi_cast<jobject,const QColor&>(JNIEnv *, const QColor&);
extern template jobject qtjambi_cast<jobject,QTransform>(JNIEnv *, QTransform&&);
extern template jobject qtjambi_cast<jobject,const QTransform&>(JNIEnv *, const QTransform&);
extern template jobject qtjambi_cast<jobject,QCursor>(JNIEnv *, QCursor&&);
extern template jobject qtjambi_cast<jobject,const QCursor&>(JNIEnv *, const QCursor&);
extern template jobject qtjambi_cast<jobject,QPainterPath>(JNIEnv *, QPainterPath&&);
extern template jobject qtjambi_cast<jobject,const QPainterPath&>(JNIEnv *, const QPainterPath&);
extern template jobject qtjambi_cast<jobject,QRegion>(JNIEnv *, QRegion&&);
extern template jobject qtjambi_cast<jobject,const QRegion&>(JNIEnv *, const QRegion&);
extern template jobject qtjambi_cast<jobject,QBrush>(JNIEnv *, QBrush&&);
extern template jobject qtjambi_cast<jobject,const QBrush&>(JNIEnv *, const QBrush&);

extern template jobject qtjambi_cast<jobject,QGraphicsItem::Extension&>(JNIEnv *, QGraphicsItem::Extension&);
extern template jobject qtjambi_cast<jobject,QGraphicsItem::GraphicsItemChange&>(JNIEnv *, QGraphicsItem::GraphicsItemChange&);

extern template jobject qtjambi_cast<jobject,QGraphicsItemGroup*&>(JNIEnv *, QGraphicsItemGroup*&);
extern template jobject qtjambi_cast<jobject,QGraphicsItem*&>(JNIEnv *, QGraphicsItem*&);
extern template jobject qtjambi_cast<jobject,const QGraphicsItem*&>(JNIEnv *, const QGraphicsItem*&);
extern template jobject qtjambi_cast<jobject,QGraphicsEffect*&>(JNIEnv *, QGraphicsEffect*&);
extern template jobject qtjambi_cast<jobject,QGraphicsSceneWheelEvent*&>(JNIEnv *, QGraphicsSceneWheelEvent*&);
extern template jobject qtjambi_cast<jobject,const QStyleOptionGraphicsItem*&>(JNIEnv *, const QStyleOptionGraphicsItem*&);
extern template jobject qtjambi_cast<jobject,QGraphicsSceneMouseEvent*&>(JNIEnv *, QGraphicsSceneMouseEvent*&);
extern template jobject qtjambi_cast<jobject,QGraphicsSceneHoverEvent*&>(JNIEnv *, QGraphicsSceneHoverEvent*&);
extern template jobject qtjambi_cast<jobject,QGraphicsSceneDragDropEvent*&>(JNIEnv *, QGraphicsSceneDragDropEvent*&);
extern template jobject qtjambi_cast<jobject,QGraphicsSceneContextMenuEvent*&>(JNIEnv *, QGraphicsSceneContextMenuEvent*&);

extern template QList<QAction*> qtjambi_cast<QList<QAction*>,jobject&>(JNIEnv *, jobject&);
extern template jobject qtjambi_cast<jobject,QList<QAction*>>(JNIEnv *, QList<QAction*>&&);
extern template jobject qtjambi_cast<jobject,const QList<QAction*>&>(JNIEnv *, const QList<QAction*>&);

extern template jobject qtjambi_cast<jobject,QImage>(JNIEnv *, QImage&&);
extern template jobject qtjambi_cast<jobject,const QImage&>(JNIEnv *, const QImage&);

extern template jobject qtjambi_cast<jobject,QPixmap>(JNIEnv *, QPixmap&&);
extern template jobject qtjambi_cast<jobject,const QPixmap&>(JNIEnv *, const QPixmap&);

extern template jobject qtjambi_cast<jobject,QFont>(JNIEnv *, QFont&&);
extern template jobject qtjambi_cast<jobject,const QFont&>(JNIEnv *, const QFont&);

#endif // UTILS_P_H
