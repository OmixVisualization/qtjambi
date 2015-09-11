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

#ifndef MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_H

#include <qtjambi/qtjambi_core.h>

class PolymorphicObjectType
{
public:
    PolymorphicObjectType();
    virtual ~PolymorphicObjectType();

    static PolymorphicObjectType *newInstance();
    static void deleteLastInstance();

private:
    static PolymorphicObjectType *m_lastInstance;
};

class QObjectType: public QObject
{
    Q_OBJECT
public:
    QObjectType();
    ~QObjectType();

    static QObjectType *newInstance();
    static void deleteLastInstance();

private:
    static QObjectType *m_lastInstance;
};

class NonPolymorphicObjectType
{
public:
    NonPolymorphicObjectType();
    ~NonPolymorphicObjectType();

    static NonPolymorphicObjectType *newInstance();
    static void deleteLastInstance();

private:
    static NonPolymorphicObjectType *m_lastInstance;
};

class ValueType
{
public:
    ValueType();
    ~ValueType();

    static ValueType *newInstance();
    static void deleteLastInstance();

private:
    static ValueType *m_lastInstance;

};

#define INVALIDATOR(T) class Invalidator##T \
{ \
public: \
    Invalidator##T() {} \
    void invalidateObject(T *meh) { \
        overrideMe(meh); \
    } \
    virtual void overrideMe(T *) = 0; \
} \

INVALIDATOR(PolymorphicObjectType);
INVALIDATOR(NonPolymorphicObjectType);
INVALIDATOR(ValueType);
INVALIDATOR(QObjectType);

#endif
