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

#ifndef ABSTRACTCLASS_H
#define ABSTRACTCLASS_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <QtCore/QObject>

class AbstractClass
{
public:
    virtual ~AbstractClass(){};

    virtual void abstractFunction(const QString &something) = 0;

    virtual AbstractClass *getAbstractClass() = 0;

    QString getS() { return s; }
    void setS(QString str) { s = str; }

    virtual bool setReferenceCountTest(QObject* object) = 0;

    QT_DEPRECATED_X("This is a test description for deprecated declaration.")
    void deprecatedMethod(){}
private:
    QString s;
};

class AbstractSubclass: public AbstractClass
{
public:
    virtual void abstractFunction(const QString &something)
    {
        setS(something);
    }

    virtual AbstractClass *getAbstractClass()
    {
        return 0;
    }
};

class NonAbstractSubclass: public AbstractClass
{
public:
    virtual void abstractFunction(const QString &something)
    {
        setS(something);
    }

    virtual AbstractClass *getAbstractClass()
    {
        return 0;
    }
    bool setReferenceCountTest(QObject* object) {Q_UNUSED(object) return false;}
};

class AnotherNonAbstractSubclass: public AbstractClass
{
public:
    virtual AbstractClass *getAbstractClass()
    {
        return new NonAbstractSubclass;
    }

    void doVirtualCall(AbstractClass *cls, const QString &something)
    {
        cls->abstractFunction(something);
    }
    bool setReferenceCountTest(QObject* object) {Q_UNUSED(object) return false;}

private:
    virtual void abstractFunction(const QString &something)
    {
        setS("Not " + something);
    }
};



#endif
