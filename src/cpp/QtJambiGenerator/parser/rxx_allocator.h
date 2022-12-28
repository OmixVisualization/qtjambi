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


#ifndef RXX_ALLOCATOR_H
#define RXX_ALLOCATOR_H

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <memory>

template <class _Tp> class rxx_allocator {
    public:
        typedef _Tp value_type;
        typedef _Tp* pointer;
        typedef const _Tp* const_pointer;
        typedef _Tp& reference;
        typedef const _Tp& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        static const size_type max_block_count = size_type(-1);
        static const size_type _S_block_size = 1 << 16; // 64K

        rxx_allocator() {
            _M_block_index = max_block_count;
            _M_current_index = 0;
            _M_storage = 0;
            _M_current_block = 0;
        }

        ~rxx_allocator() {
            for (size_type index = 0; index < _M_block_index + 1; ++index)
                delete[] _M_storage[index];

            ::free(_M_storage);
        }

        pointer address(reference __val) { return &__val; }
        const_pointer address(const_reference __val) const { return &__val; }

        pointer allocate(size_type __n, const void* = nullptr) {
            const size_type bytes = __n * sizeof(_Tp);

            if (_M_current_block == 0
                    || _S_block_size < _M_current_index + bytes) {
                ++_M_block_index;

                _M_storage = reinterpret_cast<char**>
                             (::realloc(_M_storage, sizeof(char*) * (1 + _M_block_index)));

                _M_current_block = _M_storage[_M_block_index] = reinterpret_cast<char*>
                                   (new char[_S_block_size]);

#if defined(RXX_ALLOCATOR_INIT_0) // ### make it a policy
                ::memset(_M_current_block, 0, _S_block_size);
#endif
                _M_current_index = 0;
            }

            pointer p = reinterpret_cast<pointer>
                        (_M_current_block + _M_current_index);

            _M_current_index += bytes;

            return p;
        }

        void deallocate(pointer, size_type) {}

        size_type max_size() const { return size_type(-1) / sizeof(_Tp); }

        void contruct(pointer __p, const_reference __val) { new(__p) _Tp(__val); }
        void destruct(pointer __p) { __p->~_Tp(); }

    private:
        template <class _Tp1> struct rebind {
            typedef rxx_allocator<_Tp1> other;
        };

        template <class _Tp1> rxx_allocator(const rxx_allocator<_Tp1> &) {}

    private:
        size_type _M_block_index;
        size_type _M_current_index;
        char *_M_current_block;
        char **_M_storage;
};

#endif // RXX_ALLOCATOR_H

// kate: space-indent on; indent-width 2; replace-tabs on;
