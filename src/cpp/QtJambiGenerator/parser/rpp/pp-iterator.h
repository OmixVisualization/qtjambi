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

#ifndef PP_ITERATOR_H
#define PP_ITERATOR_H

#include <iterator>

namespace rpp {

    class pp_null_output_iterator
                : public std::iterator<std::output_iterator_tag, void, void, void, void> {
        public:
            pp_null_output_iterator() {}

            template <typename _Tp>
            pp_null_output_iterator &operator=(_Tp const &) {
                return *this;
            }

            inline pp_null_output_iterator &operator * () {
                return *this;
            }
            inline pp_null_output_iterator &operator ++ () {
                return *this;
            }
            inline pp_null_output_iterator operator ++ (int) {
                return *this;
            }
    };

    template <typename _Container>
    class pp_output_iterator
                : public std::iterator<std::output_iterator_tag, void, void, void, void> {
            std::string &_M_result;

        public:
            explicit pp_output_iterator(std::string &__result):
                    _M_result(__result) {}

#if _MSC_VER > 1700
            // This fixes the MSVC2010 compiler compile issues with "*result++ = *first++;"
            //  however I am concerned for (this._M_result == __v._M_result) behavior, so
            //  this is implemented via operator type conversion, which forces the source
            //  to become a copy.  I do it this way because I consider is strange for an
            //  output_iterator to be the source for another output_iterator, however it
            //  it natural to think of a std::string to be a valid source to emit.
            inline pp_output_iterator &operator=(const pp_output_iterator &__v) {
                const size_t need = _M_result.size() + __v._M_result.size();
                if (_M_result.capacity() < need)
                    _M_result.reserve(need);

                _M_result = __v._M_result;

                return *this;
            }
#else
            inline pp_output_iterator &operator=(const std::string &__v) {
                const size_t need = _M_result.size() + __v.size();
                if (_M_result.capacity() < need)
                    _M_result.reserve(need);

                _M_result = __v;

                return *this;
            }
            inline operator const std::string() {
                const std::string c = std::string(_M_result);    // forces a copy
                return c;
            }
#endif
            inline pp_output_iterator &operator=(typename _Container::const_reference __v) {
                if (_M_result.capacity() == _M_result.size())
                    _M_result.reserve(_M_result.capacity() << 2);

                _M_result.push_back(__v);
                return *this;
            }

            inline pp_output_iterator &operator * () {
                return *this;
            }
            inline pp_output_iterator &operator ++ () {
                return *this;
            }
            inline pp_output_iterator operator ++ (int) {
                return *this;
            }
    };

} // namespace rpp

#endif // PP_ITERATOR_H
