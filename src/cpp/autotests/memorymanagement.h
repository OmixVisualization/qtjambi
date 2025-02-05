/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_H

#include <QtCore/QSharedPointer>
#include <QtCore/QObject>

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/QtJambiAPI>
#endif

class PolymorphicObjectType
{
public:
    PolymorphicObjectType();
    virtual ~PolymorphicObjectType();

    static PolymorphicObjectType *newInstance();
    static QSharedPointer<PolymorphicObjectType> newSharedInstance();
    static QSharedPointer<PolymorphicObjectType> asSharedPointer(PolymorphicObjectType*);
    static void deleteLastInstance();

private:
    static PolymorphicObjectType *m_lastInstance;
    Q_DISABLE_COPY_MOVE(PolymorphicObjectType);
};

class PolymorphicOwnedObjectType
{
public:
    PolymorphicOwnedObjectType();
    virtual ~PolymorphicOwnedObjectType();

    QObject* owner() const;

    static PolymorphicOwnedObjectType *newInstance();
    static QSharedPointer<PolymorphicOwnedObjectType> newSharedInstance();
    static QSharedPointer<PolymorphicOwnedObjectType> asSharedPointer(PolymorphicOwnedObjectType*);
    static void deleteLastInstance();

private:
    static PolymorphicOwnedObjectType *m_lastInstance;
    Q_DISABLE_COPY_MOVE(PolymorphicOwnedObjectType);
};

class QObjectType : public QObject
{
    Q_OBJECT
public:
    QObjectType();
    ~QObjectType();

    static QObjectType *newInstance();
    static QSharedPointer<QObjectType> newSharedInstance();
    static QSharedPointer<QObjectType> asSharedPointer(QObjectType*);
    static void deleteLastInstance();

private:
    static QObjectType *m_lastInstance;
    Q_DISABLE_COPY_MOVE(QObjectType);
};

class NonPolymorphicObjectType
{
public:
    NonPolymorphicObjectType();
    ~NonPolymorphicObjectType();

    static NonPolymorphicObjectType *newInstance();
    static QSharedPointer<NonPolymorphicObjectType> newSharedInstance();
    static QSharedPointer<NonPolymorphicObjectType> asSharedPointer(NonPolymorphicObjectType*);
    static void deleteLastInstance();

private:
    static NonPolymorphicObjectType *m_lastInstance;
    Q_DISABLE_COPY_MOVE(NonPolymorphicObjectType);
};

class NonPolymorphicOwnedObjectType
{
public:
    NonPolymorphicOwnedObjectType();
    ~NonPolymorphicOwnedObjectType();

    QObject* owner() const;

    static NonPolymorphicOwnedObjectType *newInstance();
    static QSharedPointer<NonPolymorphicOwnedObjectType> newSharedInstance();
    static QSharedPointer<NonPolymorphicOwnedObjectType> asSharedPointer(NonPolymorphicOwnedObjectType*);
    static void deleteLastInstance();

private:
    static NonPolymorphicOwnedObjectType *m_lastInstance;
    Q_DISABLE_COPY_MOVE(NonPolymorphicOwnedObjectType);
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
    virtual ~Invalidator##T() {} \
    void invalidateObject(T *meh) { \
        overrideMe(meh); \
    } \
    virtual void overrideMe(T *) = 0; \
} \

INVALIDATOR(PolymorphicObjectType);
INVALIDATOR(NonPolymorphicObjectType);
INVALIDATOR(PolymorphicOwnedObjectType);
INVALIDATOR(NonPolymorphicOwnedObjectType);
INVALIDATOR(ValueType);
INVALIDATOR(QObjectType);

#endif
