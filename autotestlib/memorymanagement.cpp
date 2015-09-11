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

#include "memorymanagement.h"

// PolymorphicObjectType
PolymorphicObjectType *PolymorphicObjectType::m_lastInstance = 0;
PolymorphicObjectType::PolymorphicObjectType()
{
    m_lastInstance = this;
}

PolymorphicObjectType::~PolymorphicObjectType()
{
    if (this == m_lastInstance)
        m_lastInstance = 0;
}

void PolymorphicObjectType::deleteLastInstance()
{
    delete m_lastInstance;
}

PolymorphicObjectType *PolymorphicObjectType::newInstance()
{
    return new PolymorphicObjectType;
}




// NonPolymorphicObjectType
NonPolymorphicObjectType *NonPolymorphicObjectType::m_lastInstance = 0;
NonPolymorphicObjectType::NonPolymorphicObjectType()
{
    m_lastInstance = this;
}

NonPolymorphicObjectType::~NonPolymorphicObjectType()
{
    if (this == m_lastInstance)
        m_lastInstance = 0;
}

void NonPolymorphicObjectType::deleteLastInstance()
{
    delete m_lastInstance;
}

NonPolymorphicObjectType *NonPolymorphicObjectType::newInstance()
{
    return new NonPolymorphicObjectType;
}




// ValueType
ValueType *ValueType::m_lastInstance = 0;
ValueType::ValueType()
{
    m_lastInstance = this;
}

ValueType::~ValueType()
{
    if (this == m_lastInstance)
        m_lastInstance = 0;
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
QObjectType *QObjectType::m_lastInstance = 0;
QObjectType::QObjectType()
{
    m_lastInstance = this;
}

QObjectType::~QObjectType()
{
    if (this == m_lastInstance)
        m_lastInstance = 0;
}

void QObjectType::deleteLastInstance()
{
    delete m_lastInstance;
}

QObjectType *QObjectType::newInstance()
{
    return new QObjectType;
}
