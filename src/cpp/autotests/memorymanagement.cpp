/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#include <QtCore/QCoreApplication>
#include "memorymanagement.h"

// PolymorphicObjectType
PolymorphicObjectType *PolymorphicObjectType::m_lastInstance = nullptr;
PolymorphicObjectType::PolymorphicObjectType()
{
    m_lastInstance = this;
}

PolymorphicObjectType::~PolymorphicObjectType()
{
    if (this == m_lastInstance)
        m_lastInstance = nullptr;
}

void PolymorphicObjectType::deleteLastInstance()
{
    delete m_lastInstance;
}

PolymorphicObjectType *PolymorphicObjectType::newInstance()
{
    return new PolymorphicObjectType;
}

QSharedPointer<PolymorphicObjectType> PolymorphicObjectType::newSharedInstance()
{
    return QSharedPointer<PolymorphicObjectType>(newInstance());
}

QSharedPointer<PolymorphicObjectType> PolymorphicObjectType::asSharedPointer(PolymorphicObjectType* ptr)
{
    return QSharedPointer<PolymorphicObjectType>(ptr);
}

// PolymorphicOwnedObjectType
PolymorphicOwnedObjectType *PolymorphicOwnedObjectType::m_lastInstance = nullptr;
PolymorphicOwnedObjectType::PolymorphicOwnedObjectType()
{
    m_lastInstance = this;
}

PolymorphicOwnedObjectType::~PolymorphicOwnedObjectType()
{
    if (this == m_lastInstance)
        m_lastInstance = nullptr;
}

void PolymorphicOwnedObjectType::deleteLastInstance()
{
    delete m_lastInstance;
}

PolymorphicOwnedObjectType *PolymorphicOwnedObjectType::newInstance()
{
    return new PolymorphicOwnedObjectType;
}

QSharedPointer<PolymorphicOwnedObjectType> PolymorphicOwnedObjectType::newSharedInstance()
{
    return QSharedPointer<PolymorphicOwnedObjectType>(newInstance());
}

QSharedPointer<PolymorphicOwnedObjectType> PolymorphicOwnedObjectType::asSharedPointer(PolymorphicOwnedObjectType* ptr)
{
    return QSharedPointer<PolymorphicOwnedObjectType>(ptr);
}

QObject *PolymorphicOwnedObjectType::owner() const
{
    return QCoreApplication::instance();
}


// NonPolymorphicObjectType
NonPolymorphicObjectType *NonPolymorphicObjectType::m_lastInstance = nullptr;
NonPolymorphicObjectType::NonPolymorphicObjectType()
{
    m_lastInstance = this;
}

NonPolymorphicObjectType::~NonPolymorphicObjectType()
{
    if (this == m_lastInstance)
        m_lastInstance = nullptr;
}

void NonPolymorphicObjectType::deleteLastInstance()
{
    delete m_lastInstance;
}

NonPolymorphicObjectType *NonPolymorphicObjectType::newInstance()
{
    return new NonPolymorphicObjectType;
}

QSharedPointer<NonPolymorphicObjectType> NonPolymorphicObjectType::newSharedInstance()
{
    return QSharedPointer<NonPolymorphicObjectType>(newInstance());
}

QSharedPointer<NonPolymorphicObjectType> NonPolymorphicObjectType::asSharedPointer(NonPolymorphicObjectType* ptr)
{
    return QSharedPointer<NonPolymorphicObjectType>(ptr);
}


// NonPolymorphicOwnedObjectType
NonPolymorphicOwnedObjectType *NonPolymorphicOwnedObjectType::m_lastInstance = nullptr;
NonPolymorphicOwnedObjectType::NonPolymorphicOwnedObjectType()
{
    m_lastInstance = this;
}

NonPolymorphicOwnedObjectType::~NonPolymorphicOwnedObjectType()
{
    if (this == m_lastInstance)
        m_lastInstance = nullptr;
}

void NonPolymorphicOwnedObjectType::deleteLastInstance()
{
    delete m_lastInstance;
}

NonPolymorphicOwnedObjectType *NonPolymorphicOwnedObjectType::newInstance()
{
    return new NonPolymorphicOwnedObjectType;
}

QObject *NonPolymorphicOwnedObjectType::owner() const
{
    return QCoreApplication::instance();
}

QSharedPointer<NonPolymorphicOwnedObjectType> NonPolymorphicOwnedObjectType::newSharedInstance()
{
    return QSharedPointer<NonPolymorphicOwnedObjectType>(newInstance());
}

QSharedPointer<NonPolymorphicOwnedObjectType> NonPolymorphicOwnedObjectType::asSharedPointer(NonPolymorphicOwnedObjectType* ptr)
{
    return QSharedPointer<NonPolymorphicOwnedObjectType>(ptr);
}




// ValueType
ValueType *ValueType::m_lastInstance = nullptr;
ValueType::ValueType()
{
    m_lastInstance = this;
}

ValueType::~ValueType()
{
    if (this == m_lastInstance)
        m_lastInstance = nullptr;
}

void ValueType::deleteLastInstance()
{
    delete m_lastInstance;
}

ValueType *ValueType::newInstance()
{
    return new ValueType;
}


// QObjectType
QObjectType *QObjectType::m_lastInstance = nullptr;
QObjectType::QObjectType()
{
    m_lastInstance = this;
}

QObjectType::~QObjectType()
{
    if (this == m_lastInstance)
        m_lastInstance = nullptr;
}

void QObjectType::deleteLastInstance()
{
    delete m_lastInstance;
}

QObjectType *QObjectType::newInstance()
{
    return new QObjectType;
}

QSharedPointer<QObjectType> QObjectType::newSharedInstance()
{
    return QSharedPointer<QObjectType>(newInstance());
}

QSharedPointer<QObjectType> QObjectType::asSharedPointer(QObjectType* ptr)
{
    return QSharedPointer<QObjectType>(ptr);
}
