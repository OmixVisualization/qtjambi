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

#ifndef NAMESPACE_H
#define NAMESPACE_H

#ifndef QTJAMBI_GENERATOR_RUNNING
#include <QtJambi/global.h>
#endif
#include <stdio.h>
#include <QtCore/QtCore>
#ifndef QTJAMBI_NO_WIDGETS
#include <QtWidgets/QtWidgets>
#endif

QT_WARNING_DISABLE_CLANG("-Winfinite-recursion")

namespace NameSpace
{

    class ObjectA;
    class ObjectB;
    class ValueA;

    class ObjectA
    {
    public:
        ObjectA *aFunc(ObjectA *);
        ObjectB *bFunc(ObjectB *);
        ValueA vFunc(const ValueA &);
        QPoint point();
        NameSpace::ObjectA *aFuncPrefixed(NameSpace::ObjectA *);
        NameSpace::ObjectB *bFuncPrefixed(NameSpace::ObjectB *);
        NameSpace::ValueA vFuncPrefixed(const NameSpace::ValueA &);
    };


    class ObjectB
    {

    };


    class ValueA
    {
    public:
        ValueA() : x(42) {
        }
        ValueA(const ValueA& v) : x(v.x){
        }
        const ValueA & operator=(const ValueA &a) {
            x = a.x;
            return *this;
        }
        bool operator==(const ValueA &a) const { return a.x == x; }
        bool operator<(const ValueA &a) const { return a.x < x; }

        int getX() const { return x; }
    private:
        int x;
    };

#ifndef QTJAMBI_GENERATOR_RUNNING
    inline hash_type qHash(const ValueA & v, hash_type seed = 0)
    {
        hash_type hashCode = seed;
        hashCode = hashCode * 31 + ::qHash(v.getX());
        return hashCode;
    }
#endif

    namespace NameSpace2
    {

        namespace NameSpace3
        {
            class ObjectD;

            class ObjectC: public ObjectA
            {
            public:
                ObjectC(const QString &s) : m_fooBar(s) {}
                virtual ~ObjectC(){}
                ObjectC *fooBar(ObjectD *obj);

                QString str() { return m_fooBar; }

            private:
                QString m_fooBar;
            };

            class InterfaceA
            {
            public:
                InterfaceA() : m_constructorParameter1(), m_constructorParameter2() {}
                InterfaceA(int i) : m_constructorParameter1(i), m_constructorParameter2() {}
                InterfaceA(const QString& s) : m_constructorParameter1(s), m_constructorParameter2() {}
                InterfaceA(QGraphicsItem* g) : m_constructorParameter1(QVariant::fromValue<QGraphicsItem*>(g)), m_constructorParameter2() {}
                InterfaceA(QObject* g) : m_constructorParameter1(QVariant::fromValue<QObject*>(g)), m_constructorParameter2() {}
                InterfaceA(const QList<QObject*>& g, int i = 0) : m_constructorParameter1(QVariant::fromValue<QList<QObject*> >(g)), m_constructorParameter2(i) {}
                virtual ~InterfaceA(){}
                virtual ObjectD *fooBar2(ObjectC *) = 0;
                virtual InterfaceA *fooBar(InterfaceA *) = 0;
                void publicFinal(){}
                const QVariant& constructorParameter1() const { return m_constructorParameter1; }
                const QVariant& constructorParameter2() const { return m_constructorParameter2; }
            protected:
                virtual InterfaceA *protectedFooBar(InterfaceA *) = 0;
                void protectedFinal(){}
            private:
                QVariant m_constructorParameter1;
                QVariant m_constructorParameter2;
            };

            class ObjectD: public ObjectC, public InterfaceA
            {
            public:
                ObjectD(const QString &s) : ObjectC(s) {}
                virtual ~ObjectD() override {}

                ObjectD *fooBar2(ObjectC *obj) override
                {
                    return new ObjectD(obj->str());
                }

                InterfaceA *fooBar(InterfaceA *obj) override
                {
                    return obj->fooBar(fooBar(this));
                }

            protected:
                InterfaceA *protectedFooBar(InterfaceA *obj) override
                {
                    return obj->fooBar(fooBar(this));
                }
            };

            class ValueB
            {
            public:
                ValueB() : m_i(0) {}
                ValueB(int i) : m_i(i) {}
                ValueB(const ValueB &myValue) : m_i(myValue.m_i) {}

            private:
                int m_i;
            };
        };

    };

};

#ifdef QTJAMBI_GENERATOR_RUNNING
hash_type qHash(const NameSpace::ValueA & v);
#endif


inline NameSpace::NameSpace2::NameSpace3::ObjectC *NameSpace::NameSpace2::NameSpace3::ObjectC::fooBar(NameSpace::NameSpace2::NameSpace3::ObjectD *obj)
{
    return new NameSpace::NameSpace2::NameSpace3::ObjectC(obj->str());
}
inline NameSpace::ObjectA *NameSpace::ObjectA::aFunc(NameSpace::ObjectA *a) { return a; }
inline NameSpace::ObjectB *NameSpace::ObjectA::bFunc(NameSpace::ObjectB *b) { return b; }

inline NameSpace::ValueA NameSpace::ObjectA::vFunc(const NameSpace::ValueA &a) {
    return a;
}

inline QPoint NameSpace::ObjectA::point(){
    return QPoint(10,20);
}

inline NameSpace::ObjectA *NameSpace::ObjectA::aFuncPrefixed(NameSpace::ObjectA *a) { return a; }
inline NameSpace::ObjectB *NameSpace::ObjectA::bFuncPrefixed(NameSpace::ObjectB *b) { return b; }
inline NameSpace::ValueA NameSpace::ObjectA::vFuncPrefixed(const NameSpace::ValueA &a) { return a; }

#endif // NAMESPACE_H
