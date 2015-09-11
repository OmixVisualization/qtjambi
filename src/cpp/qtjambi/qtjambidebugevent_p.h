/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QTJAMBIDEBUGEVENT_H
#define QTJAMBIDEBUGEVENT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt Jambi API.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//

#include "qtjambilink.h"

#include <sys/types.h>
#include <QtCore/QString>
#include <QtCore/QEvent>
#include <QtGui/QAccessibleEvent>
#include <QtGui/QActionEvent>
#include <QtCore/QChildEvent> 
#include <QtGui/QCloseEvent> 
#include <QtGui/QDragLeaveEvent>
#include <QtGui/QDropEvent>
#include <QtCore/QDynamicPropertyChangeEvent>
#include <QtGui/QFileOpenEvent>
#include <QtGui/QFocusEvent>
#include <QtWidgets/QGestureEvent>
#include <QtWidgets/QGraphicsSceneEvent>
#include <QtGui/QHelpEvent>
#include <QtGui/QHideEvent>
#include <QtGui/QHoverEvent>
#include <QtGui/QIconDragEvent>
#include <QtGui/QInputEvent>
#include <QtGui/QInputMethodEvent>
#include <QtGui/QMoveEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QResizeEvent>
#include <QtGui/QShortcutEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QStatusTipEvent>
#include <QtCore/QTimerEvent>
#include <QtGui/QWhatsThisClickedEvent>
#include <QtGui/QWindowStateChangeEvent>

class QtJambiDebugEvent
{
public:
    static QString qtjambi_event_to_string(QAccessibleEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QActionEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QChildEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QCloseEvent *e, const char *type_name);
//    static QString qtjambi_event_to_string(QCustomEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QDragLeaveEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QDropEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QDynamicPropertyChangeEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QFileOpenEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QFocusEvent *e, const char *type_name);
#if QT_VERSION >= 0x040600
    static QString qtjambi_event_to_string(QGestureEvent *e, const char *type_name);
#endif
    static QString qtjambi_event_to_string(QGraphicsSceneEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QHelpEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QHideEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QHoverEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QIconDragEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QInputEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QInputMethodEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QMoveEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QPaintEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QResizeEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QShortcutEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QShowEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QStatusTipEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QTimerEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QWhatsThisClickedEvent *e, const char *type_name);
    static QString qtjambi_event_to_string(QWindowStateChangeEvent *e, const char *type_name);

    static const char *qtjambi_event_type_to_string(int type);
    static const char *qtjambi_event_type_to_string(char *buf, size_t buflen, int type, bool with_id);
    static QString qtjambi_event_to_string(QEvent *event);

    static const char *qtjambi_event_desc_to_string(char *buf, size_t buflen, QEvent *event);
};

#endif
