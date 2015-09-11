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

#include <stdio.h>
#include <QtCore>

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
        ValueA() { x = 42; }
        bool operator==(const ValueA &a) const { return a.x == x; }

        int getX() const { return x; }
    private:
        int x;
    };

    namespace NameSpace2
    {

        namespace NameSpace3
        {
            class ObjectD;

            class ObjectC: public ObjectA
            {
            public:
                ObjectC(const QString &s) : m_fooBar(s) {}
                ObjectC *fooBar(ObjectD *obj);

                QString str() { return m_fooBar; }

            private:
                QString m_fooBar;
            };

            class InterfaceA
            {
            public:
                virtual ObjectD *fooBar2(ObjectC *) = 0;
                virtual InterfaceA *fooBar(InterfaceA *) = 0;
            };

            class ObjectD: public ObjectC, InterfaceA
            {
            public:
                ObjectD(const QString &s) : ObjectC(s) {}

                ObjectD *fooBar2(ObjectC *obj)
                {
                    return new ObjectD(obj->str());
                }

                InterfaceA *fooBar(InterfaceA *obj)
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
