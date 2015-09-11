/****************************************************************************
**
** Copyright (C) 2012 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2012 D L Miles Consulting Ltd.  All rights reserved.
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

#ifndef METAOBJECTQTMETACAST_H
#define METAOBJECTQTMETACAST_H

//#include <stdint.h>
#include <QtCore/QObject>

#include <qtjambi/qtjambi_core.h>

#ifndef SIGNAL
#  define SIGNAL(A) #A
#endif

#ifndef SLOT
#  define SLOT(A) #A
#endif

#ifndef emit
#  define emit
#endif

class MetaObjectQtMetaCast : public QObject
{
    Q_OBJECT

public:
    MetaObjectQtMetaCast()
    {
    }

    QString className(QObject *qobject) const
    {
        const QMetaObject *mo;
        if (qobject == 0)
            goto fail;
        mo = qobject->metaObject();
        if (mo == 0)
            goto fail;
        //return mo->className();  // const char *
        return QString::fromLatin1(mo->className());
    fail:
        return QString();  // return 0;
    }

    QString superClassName(QObject *qobject, int count) const
    {
        const QMetaObject *mo;
        const QMetaObject *smo;
        if (qobject == 0)
            goto fail;
        mo = qobject->metaObject();
        if (mo == 0)
            goto fail;
        smo = mo;
        do {
            smo = smo->superClass();
            count--;
        } while(count >= 0 && smo != 0);
        if (smo == 0)
            goto fail;
        //return smo->className();
        return QString::fromLatin1(smo->className());
    fail:
        return 0; //QString();  // return 0;
    }

    bool inherits(QObject *qobject, const QString &s) const
    {
        if (qobject == 0)
            return false;
        const QByteArray ba = s.toLatin1();  // anchor lifetime to scope
        const char *str = ba.constData();
        return qobject->inherits(str);
    }

    qint64 do_qt_metacast(QObject *qobject, const QString &s) const
    {
        if (qobject == 0)
            return -1;
        const QByteArray ba = s.toLatin1();  // anchor lifetime to scope
        const char *str = ba.constData();
        void *p = qobject->qt_metacast(str);
        return (qint64) (qintptr) p;
    }
};

#endif // METAOBJECTQTMETACAST_H
