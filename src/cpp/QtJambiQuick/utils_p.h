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

#include <QtGui/QWindow>
#include <QtGui/QPalette>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QCursor>
#include <QtGui/QIcon>
#include <QtGui/QInputMethodEvent>
#include <QtJambi/QtJambiAPI>
#include <QtJambi/JavaAPI>
#include <QtJambi/Cast>

namespace Java{
namespace QtQuick {
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQuickItem,)

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSGGeometry,)

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSGGeometry$AttributeSet,
                                     QTJAMBI_REPOSITORY_DECLARE_OBJECT_METHOD(clone)
                                     )

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSGGeometry$Point2DVertexData,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSGGeometry$TexturedPoint2DVertexData,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSGGeometry$ColoredPoint2DVertexData,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())
    QTJAMBI_REPOSITORY_DECLARE_CLASS(QSGGeometry$VertexData,
                                     QTJAMBI_REPOSITORY_DECLARE_CONSTRUCTOR())

    QTJAMBI_REPOSITORY_DECLARE_CLASS(QQuickWindow,)
}
}

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

extern template QWindow* qtjambi_cast<QWindow*,jobject&>(JNIEnv *, jobject&);
extern template jobject qtjambi_cast<jobject,QWindow*&>(JNIEnv *, QWindow*&);
extern template jobject qtjambi_cast<jobject,const QWindow*&>(JNIEnv *, const QWindow*&);

extern template jobject qtjambi_cast<jobject,QColor>(JNIEnv *, QColor&&);
extern template jobject qtjambi_cast<jobject,const QColor&>(JNIEnv *, const QColor&);

extern template jobject qtjambi_cast<jobject,QImage>(JNIEnv *, QImage&&);
extern template jobject qtjambi_cast<jobject,const QImage&>(JNIEnv *, const QImage&);

extern template jobject qtjambi_cast<jobject,QFont>(JNIEnv *, QFont&&);
extern template jobject qtjambi_cast<jobject,const QFont&>(JNIEnv *, const QFont&);

#endif // QTJAMBI_QML_REPOSITORY_H
