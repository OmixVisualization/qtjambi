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


#ifndef CONTROL_H
#define CONTROL_H

#include "symbol.h"
#include "smallobject.h"

#include <QtCore/QHash>

struct Declarator;
struct Type;
class Lexer;
class Parser;

struct Context {
    Context *parent;

    inline void bind(const NameSymbol *name, Type *type)
    { symbol_table.insert(name, type); }

    inline Type *resolve(const NameSymbol *name) const {
        if (Type *type = symbol_table.value(name))
            return type;
        else if (parent)
            return parent->resolve(name);

        return nullptr;
    }

    typedef QHash<const NameSymbol*, Type*> symbol_table_t;

    symbol_table_t symbol_table;
};

class Control {
    public:
        class ErrorMessage {
            public:
                ErrorMessage():
                        _M_line(0),
                        _M_column(0) {}

                inline int line() const { return _M_line; }
                inline void setLine(int line) { _M_line = line; }

                inline int column() const { return _M_column; }
                inline void setColumn(int column) { _M_column = column; }

                inline QString fileName() const { return _M_fileName; }
                inline void setFileName(const QString &fileName) { _M_fileName = fileName; }

                inline QString message() const { return _M_message; }
                inline void setMessage(const QString &message) { _M_message = message; }

            private:
                int _M_line;
                int _M_column;
                QString _M_fileName;
                QString _M_message;
        };

        Control();
        ~Control();

        inline bool skipFunctionBody() const { return _M_skipFunctionBody; }
        inline void setSkipFunctionBody(bool skip) { _M_skipFunctionBody = skip; }

        Lexer *changeLexer(Lexer *lexer);
        Parser *changeParser(Parser *parser);

        Lexer *currentLexer() const { return _M_lexer; }
        Parser *currentParser() const { return _M_parser; }

        Context *current_context;

        inline Context *currentContext() const
        { return current_context; }

        void pushContext();
        void popContext();

        Type *lookupType(const NameSymbol *name) const;
        void declare(const NameSymbol *name, Type *type);

        inline const NameSymbol *findOrInsertName(const char *data, size_t count)
        { return name_table.findOrInsert(data, count); }

        void declareTypedef(const NameSymbol *name, Declarator *d);
        bool isTypedef(const NameSymbol *name) const;

        void reportError(const ErrorMessage &errmsg);
    private:
        NameTable name_table;
        QHash<const NameSymbol*, Declarator*> stl_typedef_table;
        bool _M_skipFunctionBody;
        Lexer *_M_lexer;
        Parser *_M_parser;
};

#endif // CONTROL_H

// kate: space-indent on; indent-width 2; replace-tabs on;
