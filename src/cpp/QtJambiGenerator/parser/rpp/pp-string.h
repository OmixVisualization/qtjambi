/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright 2005 Roberto Raggi <roberto@kdevelop.org>
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

#ifndef PP_STRING_H
#define PP_STRING_H

#include <cstring>
#include <algorithm>

namespace rpp {

    template <typename _CharT>
    class pp_string {
            typedef std::char_traits<_CharT> traits_type;
            typedef std::size_t size_type;

            _CharT const *_M_begin;
            std::size_t _M_size;

        public:
            inline pp_string() :
                    _M_begin(0), _M_size(0) {}

            explicit pp_string(std::string const &__s) :
                    _M_begin(__s.c_str()), _M_size(__s.size()) {}

            inline pp_string(_CharT const *__begin, std::size_t __size) :
                    _M_begin(__begin), _M_size(__size) {}

            inline _CharT const *begin() const { return _M_begin; }
            inline _CharT const *end() const { return _M_begin + _M_size; }

            inline _CharT at(std::size_t index) const { return _M_begin [index]; }

            inline std::size_t size() const { return _M_size; }

            inline int compare(pp_string const &__other) const {
                size_type const __size = this->size();
                size_type const __osize = __other.size();
                size_type const __len = std::min(__size,  __osize);

                int __r = traits_type::compare(_M_begin, __other._M_begin, __len);
                if (!__r)
                    __r = int(__size - __osize);

                return __r;
            }

            inline bool operator == (pp_string const &__other) const
            { return compare(__other) == 0; }

            inline bool operator != (pp_string const &__other) const
            { return compare(__other) != 0; }

            inline bool operator < (pp_string const &__other) const
            { return compare(__other) < 0; }

            inline bool operator == (char const *s) const {
                std::size_t n = std::strlen(s);

                if (n != _M_size)
                    return false;

                return ! strncmp(_M_begin, s, n);
            }

            inline bool operator != (char const *s) const
            { return ! operator == (s); }
    };

} // namespace rpp

#endif // PP_STRING_H

