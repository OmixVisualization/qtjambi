/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright 2005 Roberto Raggi <roberto@kdevelop.org>
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of QtJambi.
**
** $BEGIN_LICENSE$
**
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
#include <functional>
#include <QString>
#include <QFileInfo>
#include <QSet>

#include "pp-macro.h"

typedef void (*MsgHandler)(const std::string &s);

namespace rpp {

    class MessageUtil{
    public:
        static void installMessageHandler(MsgHandler handler);
        static void message(const std::string &message);
    private:
        static MsgHandler _m_messageHandler;
    };

    class pp_environment {
        public:
            typedef std::vector<pp_macro*>::const_iterator const_iterator;

        public:
            pp_environment(const std::function<void(std::string,std::string,const QFileInfo&,bool)>& featureRegistry):
                    current_line(0),
                    _M_hash_size(4093),
                    _M_featureRegistry(featureRegistry){
                _M_base = reinterpret_cast<pp_macro **>(memset(new pp_macro* [_M_hash_size], 0, _M_hash_size * sizeof(pp_macro*)));
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

            bool unbind(pp_fast_string const *__name);

            bool unbind(char const *__s, std::size_t __size);

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

            void log(const std::string &message) const;

            bool pragmaOnce();

            QFileInfo current_file;
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
            QSet<QString> _M_pragmaOnceFiles;
            std::function<void(std::string,std::string,const QFileInfo&,bool)> _M_featureRegistry;
    };

} // namespace rpp

#endif // PP_ENVIRONMENT_H
