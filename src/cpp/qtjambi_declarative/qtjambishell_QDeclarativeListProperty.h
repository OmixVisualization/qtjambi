/****************************************************************************
**
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


#ifndef QTJAMBISHELL_QDECLARATIVELISTPROPERTY_H
#define QTJAMBISHELL_QDECLARATIVELISTPROPERTY_H

#include <qtjambi/qtjambi_core.h>
#include <QtCore/QHash>
#include <qdeclarativelistproperty.h>

#include <QtCore/QVariant>
#include <qlist.h>
#include <qobject.h>

class QtJambiFunctionTable;
class QtJambiLink;
class QtJambiShell_QDeclarativeListProperty : public QtjambiDeclarativeListProperty
{
public:
    QtJambiShell_QDeclarativeListProperty();
    QtJambiShell_QDeclarativeListProperty(QObject*  o0, QList<QObject*>&  list1);
    ~QtJambiShell_QDeclarativeListProperty();

    void append(QObject*  object0);
    QObject*  at(int  index0);
    bool  canAppend();
    bool  canAt();
    bool  canClear();
    bool  canCount();
    void clear();
    int  count();
    QObject*  object();
    void __override_append(QObject*  object0, bool static_call);
    QObject*  __override_at(int  index0, bool static_call);
    bool  __override_canAppend(bool static_call);
    bool  __override_canAt(bool static_call);
    bool  __override_canClear(bool static_call);
    bool  __override_canCount(bool static_call);
    void __override_clear(bool static_call);
    int  __override_count(bool static_call);
    QObject*  __override_object(bool static_call);

    QtJambiFunctionTable *m_vtable;
    QtJambiLink *m_link;
};

#endif // QTJAMBISHELL_QDECLARATIVELISTPROPERTY_H
