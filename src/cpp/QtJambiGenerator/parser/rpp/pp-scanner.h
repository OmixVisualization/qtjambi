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

#ifndef PP_SCANNER_H
#define PP_SCANNER_H

#include <cassert>
#include "pp-cctype.h"

namespace rpp {

    /**
     * Skips over different space types
     */
    struct pp_skip_blanks {
        int lines;

        /**
         * Iterates over _InputIterator first until first == last
         * and increases first until there is no space characters.
         */
        template <typename _InputIterator>
        _InputIterator operator()(_InputIterator first, _InputIterator last) {
            lines = 0;

            for (; first != last; ++first) {
                if (*first == '\\') {
                    _InputIterator begin = first;
                    ++begin;
                    int counter = 1;
                    while(begin != last && *begin==' ') {
                        ++begin;
                        ++counter;
                    }
                    if (begin != last && *begin == '\n') {
                        for(int i=0; i<counter; ++i)
                            ++first;
                    } else {
                        break;
                    }
                } else if (*first == '\n' || !pp_isspace(*first)) {
                    break;
                }
                lines += (*first != '\n' ? 0 : 1);
            }

            return first;
        }
    };

    struct pp_skip_whitespaces {
        int lines;

        template <typename _InputIterator>
        _InputIterator operator()(_InputIterator __first, _InputIterator __last) {
            lines = 0;

            for (; __first != __last; lines += (*__first != '\n' ? 0 : 1), ++__first) {
                if (! pp_isspace(*__first))
                    break;
            }

            return __first;
        }
    };

    /**
     * Skip comments of //foo or /_* foo *_/
     */
    struct pp_skip_comment_or_divop {
        int lines;

        template <typename _InputIterator>
        _InputIterator operator()(_InputIterator p_first, _InputIterator p_last) {
            enum {
                MAYBE_BEGIN,
                BEGIN,
                MAYBE_END,
                END,
                IN_COMMENT,
                IN_CXX_COMMENT
            } state(MAYBE_BEGIN);

            lines = 0;

            for (; p_first != p_last; lines += (*p_first != '\n' ? 0 : 1), ++p_first) {
                switch (state) {
                    default:
                        assert(0);
                        break;

                    case MAYBE_BEGIN:
                        if (*p_first != '/')
                            return p_first;

                        state = BEGIN;
                        break;

                    case BEGIN:
                        if (*p_first == '*')
                            state = IN_COMMENT;
                        else if (*p_first == '/')
                            state = IN_CXX_COMMENT;
                        else{
                            //++lines;
                            return p_first-1;
                        }
                        break;

                    case IN_COMMENT:
                        if (*p_first == '*')
                            state = MAYBE_END;
                        break;

                    case IN_CXX_COMMENT:
                        if (*p_first == '\n')
                            return p_first;
                        break;

                    case MAYBE_END:
                        if (*p_first == '/')
                            state = END;
                        else if (*p_first != '*')
                            state = IN_COMMENT;
                        break;

                    case END:
                        return p_first;
                }
            }

            return p_first;
        }
    };

    /**
     * Skips through alphanumeric characters and _.
     */
    struct pp_skip_identifier {
        int lines;

        /**
         * Loops through frist until first == last or first is not _ or alphanumeric.
         */
        template <typename _InputIterator>
        _InputIterator operator()(_InputIterator first, _InputIterator last) {
            lines = 0;

            for (; first != last; ++first) {
                if (! pp_isalnum(*first) && *first != '_') {
                    break;
                }
                lines += (*first != '\n' ? 0 : 1);
            }

            return first;
        }
    };

    struct pp_skip_number {
        int lines;

        template <typename _InputIterator>
        _InputIterator operator()(_InputIterator __first, _InputIterator __last) {
            lines = 0;

            for (; __first != __last; lines += (*__first != '\n' ? 0 : 1), ++__first) {
                if (! pp_isalnum(*__first) && *__first != '.')
                    break;
            }

            return __first;
        }
    };

    struct pp_skip_string_literal {
        int lines;

        /**
         * Reads through string literal.
         * Loops until end of the string, character "
         */
        template <typename _InputIterator>
        _InputIterator operator()(_InputIterator first, _InputIterator last) {
            enum {
                BEGIN,
                IN_STRING,
                QUOTE,
                END
            } state(BEGIN);

            lines = 0;

            for (; first != last; ++first) {
                switch (state) {
                    default:
                        assert(0);
                        break;

                    case BEGIN:
                        if (*first != '\"')
                            return first;
                        state = IN_STRING;
                        break;

                    case IN_STRING:
                        assert(*first != '\n');

                        if (*first == '\"')
                            state = END;
                        else if (*first == '\\')
                            state = QUOTE;
                        break;

                    case QUOTE:
                        state = IN_STRING;
                        break;

                    case END:
                        return first;
                }

                lines += (*first != '\n' ? 0 : 1);
            }

            return first;
        }
    };

    /*
     * Skips through 'c'. See above.
     */
    struct pp_skip_char_literal {
        int lines;

        template <typename _InputIterator>
        _InputIterator operator()(_InputIterator __first, _InputIterator __last) {
            enum {
                BEGIN,
                IN_STRING,
                QUOTE,
                END
            } state(BEGIN);

            lines = 0;

            for (; state != END && __first != __last; lines += (*__first != '\n' ? 0 : 1), ++__first) {
                switch (state) {
                    default:
                        assert(0);
                        break;

                    case BEGIN:
                        if (*__first != '\'')
                            return __first;
                        state = IN_STRING;
                        break;

                    case IN_STRING:
                        assert(*__first != '\n');

                        if (*__first == '\'')
                            state = END;
                        else if (*__first == '\\')
                            state = QUOTE;
                        break;

                    case QUOTE:
                        state = IN_STRING;
                        break;
                }
            }

            return __first;
        }
    };

    /**
     * Reads through argument and skips everything inside it.
     */
    struct pp_skip_argument {
        pp_skip_identifier skip_number;
        pp_skip_identifier skip_identifier;
        pp_skip_string_literal skip_string_literal;
        pp_skip_char_literal skip_char_literal;
        pp_skip_comment_or_divop skip_comment_or_divop;
        int lines;

        /**
         * Reads through first from ( to ) or , or to depth amount of ) if
         * content read has ( chars.
         */
        template <typename _InputIterator>
        _InputIterator operator()(_InputIterator first, _InputIterator last) {
            int depth = 0;
            lines = 0;

            while (first != last) {
                if (!depth && (*first == ')' || *first == ',')) // last
                    break;
                else if (*first == '(') // sub block new
                    ++depth, ++first;
                else if (*first == ')') // sub block end
                    --depth, ++first;
                else if (*first == '\"') { // skips string
                    first = skip_string_literal(first, last);
                    lines += skip_string_literal.lines;
                } else if (*first == '\'') { // skips char
                    first = skip_char_literal(first, last);
                    lines += skip_char_literal.lines;
                } else if (*first == '/') { // skips comments
                    _InputIterator _first = skip_comment_or_divop(first, last);
                    lines += skip_comment_or_divop.lines;
                    if(first==_first){
                        ++first;
                    }else{
                        first = _first;
                    }
                } else if (pp_isalpha(*first) || *first == '_') { // skips identifier
                    first = skip_identifier(first, last);
                    lines += skip_identifier.lines;
                } else if (pp_isdigit(*first)) { // skips numbers
                    first = skip_number(first, last);
                    lines += skip_number.lines;
                } else if (*first == '\n') { // new line
                    ++first;
                    ++lines;
                } else
                    ++first;
            }

            return first;
        }
    };

} // namespace rpp

#endif // PP_SCANNER_H
