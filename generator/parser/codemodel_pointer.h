/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2006 Roberto Raggi <roberto@kdevelop.org>
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

#ifndef CODEMODEL_POINTER_H
#define CODEMODEL_POINTER_H

#include <QtCore/QSharedData>

// Since the atomic API changed in 4.4 we need to hack a little here
// to make it work with both 4.3 and 4.4 until that is not required

#if QT_VERSION >= 0x040400

#   include <QtCore/qatomic.h>
template <class T> class CodeModelPointer: public QAtomicPointer<T>

#else

template <class T> class CodeModelPointer

#endif // QT_VERSION >= 0x040400

{
    public:
        typedef T Type;

#if QT_VERSION < 0x040400
        inline T &operator*() { return *d; }
        inline const T &operator*() const { return *d; }
        inline T *operator->() { return d; }
        inline const T *operator->() const { return d; }
        inline operator T *() { return d; }
        inline operator const T *() const { return d; }
        inline T *data() { return d; }
        inline const T *data() const { return d; }
        inline const T *constData() const { return d; }

        inline bool operator==(const CodeModelPointer<T> &other) const { return d == other.d; }
        inline bool operator!=(const CodeModelPointer<T> &other) const { return d != other.d; }
        inline bool operator==(const T *ptr) const { return d == ptr; }
        inline bool operator!=(const T *ptr) const { return d != ptr; }

        inline CodeModelPointer() { d = 0; }
        inline ~CodeModelPointer() { if (d && !d->ref.deref()) delete d; }

        explicit CodeModelPointer(T *data);
        inline CodeModelPointer(const CodeModelPointer<T> &o) : d(o.d) { if (d) d->ref.ref(); }
        inline CodeModelPointer<T> & operator=(const CodeModelPointer<T> &o) {
            if (o.d != d) {
                T *x = o.d;
                if (x) x->ref.ref();
                x = qAtomicSetPtr(&d, x);
                if (x && !x->ref.deref())
                    delete x;
            }
            return *this;
        }
        inline CodeModelPointer &operator=(T *o) {
            if (o != d) {
                T *x = o;
                if (x) x->ref.ref();
                x = qAtomicSetPtr(&d, x);
                if (x && !x->ref.deref())
                    delete x;
            }
            return *this;
        }

        inline bool operator!() const { return !d; }

    private:
        T *d;
#else // QT_VERSION < 0x040400
#if QT_VERSION > 0x050000
        inline bool operator==(T *value) const
        {
            return QAtomicPointer<T>::_q_value == value;
        }

        inline bool operator!=(T *value) const
        {
            return !operator==(value);
        }

        inline bool operator!() const
        {
            return operator==(0);
        }

        inline operator T *() const
        {
            return QAtomicPointer<T>::_q_value;
        }

        inline T *operator->() const
        {
            return QAtomicPointer<T>::_q_value;
        }

        inline QAtomicPointer<T> &operator=(T *value)
        {
    #ifdef QT_ARCH_PARISC
            this->_q_lock[0] = this->_q_lock[1] = this->_q_lock[2] = this->_q_lock[3] = -1;
    #endif
            QAtomicPointer<T>::_q_value = value;
            return *this;
        }
#else
    inline CodeModelPointer &operator=(T *o) {
#if QT_VERSION < 0x050000
        QAtomicPointer<T>::operator=(o);
#else
		QAtomicPointer<T>::store(o);
#endif
        return *this;
    }
#endif
        inline CodeModelPointer(T *value = 0) : QAtomicPointer<T>(value) {}

        inline T *data() { return (T *) *this; }
        inline const T *data() const { return (const T *) *this; }
        inline const T *constData() const { return (const T *) *this; }
#endif
};

#if QT_VERSION < 0x040400
template <class T>
Q_INLINE_TEMPLATE CodeModelPointer<T>::CodeModelPointer(T *adata) : d(adata)
{ if (d) d->ref.ref(); }
#endif

#endif // CODEMODEL_POINTER_H
