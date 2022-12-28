/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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


#ifndef FASTLIST_H
#define FASTLIST_H

#include "smallobject.h"

template <typename Tp>
struct ListNode {
    Tp element;
    int index;
    mutable const ListNode<Tp> *next;

    static ListNode *create(const Tp &element, pool *p) {
        ListNode<Tp> *node = new(p->allocate(sizeof(ListNode))) ListNode();
        node->element = element;
        node->index = 0;
        node->next = node;

        return node;
    }

    static ListNode *create(const ListNode *n1, const Tp &element, pool *p) {
        ListNode<Tp> *n2 = ListNode::create(element, p);

        n2->index = n1->index + 1;
        n2->next = n1->next;
        n1->next = n2;

        return n2;
    }

    inline ListNode<Tp>() { }

    inline const ListNode<Tp> *at(int index) const {
        const ListNode<Tp> *node = this;
        while (index != node->index)
            node = node->next;

        return node;
    }

    inline bool hasNext() const
    { return index < next->index; }

    inline int count() const
    { return 1 + toBack()->index; }

    inline const ListNode<Tp> *toFront() const
    { return toBack()->next; }

    inline const ListNode<Tp> *toBack() const {
        const ListNode<Tp> *node = this;
        while (node->hasNext())
            node = node->next;

        return node;
    }
};

template <class Tp>
inline const ListNode<Tp> *snoc(const ListNode<Tp> *list,
                                const Tp &element, pool *p) {
    if (!list)
        return ListNode<Tp>::create(element, p);

    return ListNode<Tp>::create(list->toBack(), element, p);
}

#endif // FASTLIST_H

// kate: space-indent on; indent-width 2; replace-tabs on;

