/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

#include "destruction.h"

DestroyCounter::DestroyCounter() : m_destroyed(0) { }
DestroyCounter::~DestroyCounter() { }
void DestroyCounter::increaseDestroyedCount() { m_destroyed++; }
int DestroyCounter::destroyedCount() const { return m_destroyed; }

OrdinarySuperclass::OrdinarySuperclass() { }
OrdinarySuperclass::~OrdinarySuperclass() { }

OrdinaryDestroyed::OrdinaryDestroyed(DestroyCounter* destroyCounter): m_destroyCounter(destroyCounter)
{
    // nanana
}

OrdinaryDestroyed::~OrdinaryDestroyed()
{
    if(m_destroyCounter)
        m_destroyCounter->increaseDestroyedCount();
}

OrdinaryDestroyed *OrdinaryDestroyed::virtualGetObjectJavaOwnership(DestroyCounter* destroyCounter) { return new OrdinaryDestroyed(destroyCounter); }
OrdinaryDestroyed *OrdinaryDestroyed::virtualGetObjectCppOwnership(DestroyCounter* destroyCounter) { return new OrdinaryDestroyed(destroyCounter); }
void OrdinaryDestroyed::virtualSetDefaultOwnership(OrdinaryDestroyed *) { }

void OrdinaryDestroyed::deleteFromCpp(OrdinaryDestroyed *destroyed)
{
    delete destroyed;
}

void OrdinaryDestroyed::deleteFromCppOther(OrdinarySuperclass *destroyed)
{
    delete destroyed;
}

OrdinaryDestroyed *OrdinaryDestroyed::callGetObjectJavaOwnership(DestroyCounter* destroyCounter, OrdinaryDestroyed *_this)
{
    return _this->virtualGetObjectJavaOwnership(destroyCounter);
}

OrdinaryDestroyed *OrdinaryDestroyed::callGetObjectCppOwnership(DestroyCounter* destroyCounter, OrdinaryDestroyed *_this)
{
    return _this->virtualGetObjectCppOwnership(destroyCounter);
}

void OrdinaryDestroyed::callSetDefaultOwnership(OrdinaryDestroyed *_this, OrdinaryDestroyed *obj)
{
    return _this->virtualSetDefaultOwnership(obj);
}

// Set in type system
OrdinaryDestroyed *OrdinaryDestroyed::getObjectJavaOwnership(DestroyCounter* destroyCounter) { return new OrdinaryDestroyed(destroyCounter); }

// Default ownership
OrdinaryDestroyed *OrdinaryDestroyed::getObjectSplitOwnership(DestroyCounter* destroyCounter) { return new OrdinaryDestroyed(destroyCounter); }

// Set in type system
OrdinaryDestroyed *OrdinaryDestroyed::getObjectCppOwnership(DestroyCounter* destroyCounter) { return new OrdinaryDestroyed(destroyCounter); }
void OrdinaryDestroyed::setDefaultOwnership(OrdinaryDestroyed *) { }

QObject *OrdinaryDestroyed::getGlobalQObjectSplitOwnership() {
    static QPointer<QObject> object = QPointer<QObject>([] () -> QObject* {
                                                            QObject* object = new QObject();
                                                            if(QCoreApplication::instance()){
                                                                QObject::connect(QCoreApplication::instance(), &QObject::destroyed, object, [object](){ delete object; });
                                                            }
                                                            return object;
                                                        }());
    return object;
}

QObjectDestroyed::QObjectDestroyed(DestroyCounter* destroyCounter, QObject *parent)  : QObject(parent), m_destroyCounter(destroyCounter) { }

QObjectDestroyed::~QObjectDestroyed() {
    if(m_destroyCounter)
        m_destroyCounter->increaseDestroyedCount();
}

void QObjectDestroyed::deleteFromCpp(QObjectDestroyed *destroyed)
{
    delete destroyed;
}

void QObjectDestroyed::deleteFromCppOther(QObject *destroyed)
{
    delete destroyed;
}
