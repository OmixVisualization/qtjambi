/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef QTJAMBIDESTRUCTOREVENT_H
#define QTJAMBIDESTRUCTOREVENT_H

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
//

#include "qtjambilink.h"

#include <QtCore/QEvent>
#include <qtjambi/qtjambi_core.h>

class QtJambiDestructorEvent : public QEvent
{

public:
    QtJambiDestructorEvent(QtJambiLink *link, void *pointer, int meta_type, int ownership, PtrDeleterFunction destructor_function)
    : QEvent( QEvent::Type(513) ),
      m_link(link),
      m_pointer(pointer),
      m_meta_type(meta_type),
      m_ownership(ownership),
      m_destructor_function(destructor_function)
    { }

    inline void callDestructor()
    {
        QTJAMBI_DEBUG_TRACE("(native) entering: QtJambiDestructorEvent::callDestructor()");
        if (m_pointer != 0 && 
#if QT_VERSION < 0x050000
			m_meta_type != QMetaType::Void) {
#else
			m_meta_type != QMetaType::UnknownType) {
#endif
            QMetaType::destroy(m_meta_type, m_pointer);
        } else if (m_ownership == QtJambiLink::JavaOwnership && m_destructor_function) {
            m_destructor_function(m_pointer);
        }
        m_pointer = 0;

        // This cannot be deleted before now, since the type may have a virtual destructor and may be a shell class object,
        // which means it will try to access its link. But everything is ready for
        // deletion, as this was done when the java object was finalized.
        delete m_link;
        QTJAMBI_DEBUG_TRACE("(native) leaving: QtJambiDestructorEvent::callDestructor()");
    }

private:
    QtJambiLink *m_link;
    void * m_pointer;
    int m_meta_type;
    int m_ownership;
    PtrDeleterFunction m_destructor_function;
};

#endif
