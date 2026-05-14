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

#ifndef UTILS_P_H
#define UTILS_P_H

#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/Cast>

namespace Java{
namespace QtGui{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QClipboard$Text,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
#if QT_VERSION < QT_VERSION_CHECK(6,11,0)
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQuaternion$Axes,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(xAxis)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(yAxis)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_FIELD(zAxis))
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQuaternion$EulerAngles,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR()
                                     QTJAMBI_REPOSITORY_DECLARE_FLOAT_FIELD(pitch)
                                     QTJAMBI_REPOSITORY_DECLARE_FLOAT_FIELD(yaw)
                                     QTJAMBI_REPOSITORY_DECLARE_FLOAT_FIELD(roll)
                                     )
#endif
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQuaternion$AxisAndAngle,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QTextCursor$SelectedTableCells,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
}
namespace QtWidgets{
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QMenu,)

#if QT_VERSION >= QT_VERSION_CHECK(6,2,0)
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(M) QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(M)
#else
#define QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(M)
#endif

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QApplication,
                                     QTJAMBI_REPOSITORY_DECLARE_STATIC_INT_METHOD(exec)
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD_QT6(resolveInterface)
                                     )
}
}

const QObject* getPointerOwner(const QTextCursor* __qt_this);

inline bool qFuzzyIsNull(const QVector2D& f) noexcept{
    return qFuzzyIsNull(f.x()) || qFuzzyIsNull(f.y());
}

inline bool qFuzzyIsNull(const QVector3D& f) noexcept{
    return qFuzzyIsNull(f.x()) || qFuzzyIsNull(f.y()) || qFuzzyIsNull(f.z());
}

inline bool qFuzzyIsNull(const QVector4D& f) noexcept{
    return qFuzzyIsNull(f.x()) || qFuzzyIsNull(f.y()) || qFuzzyIsNull(f.z()) || qFuzzyIsNull(f.w());
}

extern template jobject qtjambi_cast<jobject,QAccessibleInterface*&>(JNIEnv *, QAccessibleInterface*&);
extern template jobject qtjambi_cast<jobject,const QAccessibleInterface*&>(JNIEnv *, const QAccessibleInterface*&);

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

extern template QWindow* qtjambi_cast<QWindow*,jobject&>(JNIEnv *, jobject&);
extern template jobject qtjambi_cast<jobject,QWindow*&>(JNIEnv *, QWindow*&);
extern template jobject qtjambi_cast<jobject,const QWindow*&>(JNIEnv *, const QWindow*&);

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
extern template jobject qtjambi_cast<jobject,QPalette>(JNIEnv *, QPalette&&);
extern template jobject qtjambi_cast<jobject,const QPalette&>(JNIEnv *, const QPalette&);
extern template jobject qtjambi_cast<jobject,QPalette>(JNIEnv *, QPalette&&);
extern template jobject qtjambi_cast<jobject,const QPalette&>(JNIEnv *, const QPalette&);
extern template jobject qtjambi_cast<jobject,QTextCursor>(JNIEnv *, QTextCursor&&);
extern template jobject qtjambi_cast<jobject,const QTextCursor&>(JNIEnv *, const QTextCursor&);
extern template jobject qtjambi_cast<jobject,QBrush>(JNIEnv *, QBrush&&);
extern template jobject qtjambi_cast<jobject,const QBrush&>(JNIEnv *, const QBrush&);

extern template jobject qtjambi_cast<jobject,QImage>(JNIEnv *, QImage&&);
extern template jobject qtjambi_cast<jobject,const QImage&>(JNIEnv *, const QImage&);

extern template jobject qtjambi_cast<jobject,QFont>(JNIEnv *, QFont&&);
extern template jobject qtjambi_cast<jobject,const QFont&>(JNIEnv *, const QFont&);

extern template jobject qtjambi_cast<jobject,QPixmap>(JNIEnv *, QPixmap&&);
extern template jobject qtjambi_cast<jobject,const QPixmap&>(JNIEnv *, const QPixmap&);

#endif // QTJAMBIGUI_UTILS_H
