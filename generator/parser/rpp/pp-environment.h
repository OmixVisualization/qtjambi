/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright 2005 Roberto Raggi <roberto@kdevelop.org>
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

#ifndef PP_ENVIRONMENT_H
#define PP_ENVIRONMENT_H

#include <vector>
#include <string>
#include <cstring>

#include "pp-macro.h"

namespace rpp {

    class pp_environment {
        public:
            typedef std::vector<pp_macro*>::const_iterator const_iterator;

        public:
            pp_environment():
                    current_line(0),
                    _M_hash_size(4093) {
                _M_base = (pp_macro **) memset(new pp_macro* [_M_hash_size], 0, _M_hash_size * sizeof(pp_macro*));
            }

            ~pp_environment() {
                for (std::size_t i = 0; i < _M_macros.size(); ++i)
                    delete _M_macros [i];

                delete [] _M_base;
            }

            const_iterator first_macro() const {
                return _M_macros.begin();
            }
            const_iterator last_macro() const {
                return _M_macros.end();
            }

            void bind(pp_fast_string const *__name, pp_macro const &__macro);

            void unbind(pp_fast_string const *__name);

            void unbind(char const *__s, std::size_t __size);

            /**
             * Find corresponding pp_macro from hash table.
             */
            pp_macro *resolve(const rpp::pp_fast_string* p_name) const;

            /**
             * Wrapper for resolve which takes char array and size_t and constructs
             * pp_fast_string for the resolve(pp_fast_string).
             *
             * Find corresponding pp_macro from hash table.
             */
            pp_macro *resolve(char const *__data, std::size_t __size) const;

            std::string current_file;
            int current_line;

        private:
            /**
             * Returns hash value for string s.
             */
            std::size_t hash_code(pp_fast_string const &s) const;

            void rehash();

        private:
            std::vector<pp_macro*> _M_macros;
            pp_macro **_M_base;
            std::size_t _M_hash_size;
    };

} // namespace rpp

#endif // PP_ENVIRONMENT_H
