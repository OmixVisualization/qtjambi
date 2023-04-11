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


#ifndef LEXER_H
#define LEXER_H

#include "symbol.h"

#include <QtCore/QString>
#include <QtCore/QMap>
#include <cstdlib>
#include <cassert>

struct NameSymbol;
class Lexer;
class Control;

typedef void (Lexer::*scan_fun_ptr)();
char const *token_name(int token);

class Token {
    public:
        int kind;
        std::size_t position;
        std::size_t size;
        char const *text;

        union {
            const NameSymbol *symbol;
            std::size_t right_brace;
        } extra;
};

class LocationTable {
    private:
        LocationTable(const LocationTable &source);
        void operator = (const LocationTable &source);

    public:
        inline LocationTable(std::size_t size = 1024)
                : lines(nullptr),
                line_count(0),
                current_line(0) {
            resize(size);
        }

        inline ~LocationTable() {
            free(lines);
        }

        inline std::size_t size() const
        { return line_count; }

        void resize(std::size_t size) {
            Q_ASSERT(size > 0);
            lines = reinterpret_cast<std::size_t*>(::realloc(lines, sizeof(std::size_t) * size));
            line_count = size;
        }

        void positionAt(std::size_t offset, int *line, int *column) const
        { positionAt(offset, int(current_line), line, column); }

        void positionAt(std::size_t offset, int max_line, int *line, int *column) const;

        inline std::size_t &operator[](std::size_t index)
        { return lines[index]; }

    private:
        std::size_t *lines;
        std::size_t line_count;
        std::size_t current_line;

        friend class Lexer;
};

class TokenStream {
    public:
        TokenStream(std::size_t size = 1024);
        ~TokenStream();
        std::size_t size() const;
        std::size_t cursor() const;
        void rewind(std::size_t i);
        void resize(std::size_t size);
        std::size_t nextToken();
        int lookAhead(std::size_t i = 0) const;
        int kind(std::size_t i) const;
        std::size_t position(std::size_t i) const;
        const NameSymbol *symbol(std::size_t i) const;
        std::size_t matchingBrace(std::size_t i) const;
        Token &operator[](std::size_t index);
        const Token &token(std::size_t index) const;
    private:
        QVector<Token> tokens;
        std::size_t index;
        std::size_t token_count;
        Token* currentToken;
        const char* currentText;

    private:
        friend class Lexer;
        Q_DISABLE_COPY_MOVE(TokenStream)
};

class LocationManager {
    public:
        LocationManager(TokenStream &__token_stream,
                        LocationTable &__location_table,
                        LocationTable &__line_table,
                        QMap<QString,QStringList>& requiredFeatures);

        void positionAt(std::size_t offset, int *line, int *column,
                        QString *filename) const;

        void addRequiredFeature(std::size_t offset, QString feature);

        void extract_line(int offset, int *line, QString *filename) const;

        const QMap<QString,QStringList>& requiredFeatures() const;

        TokenStream &token_stream;
        LocationTable &location_table;
        LocationTable &line_table;
    private:
        QMap<QString,QStringList>& _M_requiredFeatures;
        Q_DISABLE_COPY_MOVE(LocationManager)
};

class Lexer {
    public:
        Lexer(LocationManager &__location, Control *__control);
        void tokenize(const char *contents, std::size_t size);

        LocationManager &_M_location;
        TokenStream &token_stream;
        LocationTable &location_table;
        LocationTable &line_table;

    private:
        void reportError(const QString& msg);

        void initialize_scan_table();
        void scan_newline();
        void scan_white_spaces();
        void scan_identifier_or_keyword();
        void scan_identifier_or_literal();
        void scan_int_constant();
        void scan_char_constant();
        void scan_string_constant();
        void scan_invalid_input();
        void scan_preprocessor();

        // keywords
        void scanKeyword0();
        void scanKeyword2();
        void scanKeyword3();
        void scanKeyword4();
        void scanKeyword5();
        void scanKeyword6();
        void scanKeyword7();
        void scanKeyword8();
        void scanKeyword9();
        void scanKeyword10();
        void scanKeyword11();
        void scanKeyword12();
        void scanKeyword13();
        void scanKeyword14();
        void scanKeyword16();
        void scanKeyword17();
        void scanKeyword18();
        void scanKeyword19();
        void scanKeyword20();

        // operators
        void scan_not();
        void scan_remainder();
        void scan_and();
        void scan_left_paren();
        void scan_right_paren();
        void scan_star();
        void scan_plus();
        void scan_comma();
        void scan_minus();
        void scan_dot();
        void scan_divide();
        void scan_backslash();
        void scan_colon();
        void scan_semicolon();
        void scan_less();
        void scan_equal();
        void scan_greater();
        void scan_question();
        void scan_left_bracket();
        void scan_right_bracket();
        void scan_xor();
        void scan_left_brace();
        void scan_or();
        void scan_right_brace();
        void scan_tilde();
        void scan_EOF();

    private:
        Control *control;
        const unsigned char *cursor;
        const unsigned char *begin_buffer;
        const unsigned char *end_buffer;
        std::size_t index;

        static scan_fun_ptr s_scan_table[];
        static scan_fun_ptr s_scan_keyword_table[];
        static bool s_initialized;
        Q_DISABLE_COPY_MOVE(Lexer)
};

#endif // LEXER_H

// kate: space-indent on; indent-width 2; replace-tabs on;
