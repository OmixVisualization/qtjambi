/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef DESTRUCTION_H
#define DESTRUCTION_H

#include <QtCore/QObject>

class OrdinarySuperclass
{
public:
    OrdinarySuperclass() { }
    virtual ~OrdinarySuperclass() { }
};

class OrdinaryDestroyed: public OrdinarySuperclass
{
public:
    OrdinaryDestroyed()
    {
        // nanana
    }

    virtual ~OrdinaryDestroyed()
    {
        increaseDestroyedCount();
    }

    virtual OrdinaryDestroyed *virtualGetObjectJavaOwnership() { return new OrdinaryDestroyed(); }
    virtual OrdinaryDestroyed *virtualGetObjectCppOwnership() { return new OrdinaryDestroyed(); }
    virtual void virtualSetDefaultOwnership(OrdinaryDestroyed *) { }

    static void deleteFromCpp(OrdinaryDestroyed *destroyed)
    {
        delete destroyed;
    }

    static void deleteFromCppOther(OrdinarySuperclass *destroyed)
    {
        delete destroyed;
    }

    static void setDestroyedCount(int count) { m_destroyed = count; }
    static void increaseDestroyedCount() { m_destroyed++; }
    static int destroyedCount() { return m_destroyed; }

    static OrdinaryDestroyed *callGetObjectJavaOwnership(OrdinaryDestroyed *_this)
    {
        return _this->virtualGetObjectJavaOwnership();
    }

    static OrdinaryDestroyed *callGetObjectCppOwnership(OrdinaryDestroyed *_this)
    {
        return _this->virtualGetObjectCppOwnership();
    }

    static void callSetDefaultOwnership(OrdinaryDestroyed *_this, OrdinaryDestroyed *obj)
    {
        return _this->virtualSetDefaultOwnership(obj);
    }

    // Set in type system
    static OrdinaryDestroyed *getObjectJavaOwnership() { return new OrdinaryDestroyed(); }

    // Default ownership
    static OrdinaryDestroyed *getObjectSplitOwnership() { return new OrdinaryDestroyed(); }

    // Set in type system
    static OrdinaryDestroyed *getObjectCppOwnership() { return new OrdinaryDestroyed(); }
    static void setDefaultOwnership(OrdinaryDestroyed *) { }


private:
    static int m_destroyed;
};

class QObjectDestroyed: public QObject
{
public:
    QObjectDestroyed(QObject *parent = 0)  : QObject(parent) { }

    static void deleteFromCpp(QObjectDestroyed *destroyed)
    {
        delete destroyed;
    }

    static void deleteFromCppOther(QObject *destroyed)
    {
        delete destroyed;
    }
};

#endif
