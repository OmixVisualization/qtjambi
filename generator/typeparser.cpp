/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "typeparser.h"

#include <qdebug.h>

class Scanner {
    public:
        enum Token {
            StarToken,
            AmpersandToken,
            LessThanToken,
            ColonToken,
            CommaToken,
            OpenParenToken,
            CloseParenToken,
            SquareBegin,
            SquareEnd,
            GreaterThanToken,

            ConstToken,
            Identifier,
            NoToken
        };

        Scanner(const QString &s)
                : m_pos(0), m_length(s.length()), m_chars(s.constData()) {
        }

        Token nextToken();
        QString identifier() const;

    private:
        int m_pos;
        int m_length;
        int m_token_start;
        const QChar *m_chars;
};

QString Scanner::identifier() const {
    return QString(m_chars + m_token_start, m_pos - m_token_start);
}

Scanner::Token Scanner::nextToken() {
    Token tok = NoToken;

    // remove whitespace
    while (m_pos < m_length && m_chars[m_pos] == ' ') {
        ++m_pos;
    }

    m_token_start = m_pos;

    while (m_pos < m_length) {

        const QChar &c = m_chars[m_pos];

        if (tok == NoToken) {
            switch (c.toLatin1()) {
                case '*': tok = StarToken; break;
                case '&': tok = AmpersandToken; break;
                case '<': tok = LessThanToken; break;
                case '>': tok = GreaterThanToken; break;
                case ',': tok = CommaToken; break;
                case '(': tok = OpenParenToken; break;
                case ')': tok = CloseParenToken; break;
                case '[': tok = SquareBegin; break;
                case ']' : tok = SquareEnd; break;
                case ':':
                    tok = ColonToken;
                    Q_ASSERT(m_pos + 1 < m_length);
                    ++m_pos;
                    break;
                default:
                    if (c.isLetterOrNumber() || c == '_')
                        tok = Identifier;
                    else
                        qFatal("Unrecognized character in lexer: %c", c.toLatin1());
                    break;
            }
        }

        if (tok <= GreaterThanToken) {
            ++m_pos;
            break;
        }

        if (tok == Identifier) {
            if (c.isLetterOrNumber() || c == '_')
                ++m_pos;
            else
                break;
        }
    }

    if (tok == Identifier && m_pos - m_token_start == 5) {
        if (m_chars[m_token_start] == 'c'
                && m_chars[m_token_start + 1] == 'o'
                && m_chars[m_token_start + 2] == 'n'
                && m_chars[m_token_start + 3] == 's'
                && m_chars[m_token_start + 4] == 't')
            tok = ConstToken;
    }

    return tok;

}

TypeParser::Info TypeParser::parse(const QString &str) {
    Scanner scanner(str);

    Info info;
    QStack<Info *> stack;
    stack.push(&info);

    bool colon_prefix = false;
    bool in_array = false;
    QString array;

    // search for 'T', 'T *', 'const T *', 'T const *', 'T * const' and so on
    bool lastWasStarToken = false;
    Scanner::Token tok = scanner.nextToken();
    while (tok != Scanner::NoToken) {

        switch (tok) {

            case Scanner::StarToken:
                //++stack.top()->indirections;
                stack.top()->indirections << false;
                break;

            case Scanner::AmpersandToken:
                stack.top()->is_reference = true;
                break;

            case Scanner::LessThanToken:
                stack.top()->template_instantiations << Info();
                stack.push(&stack.top()->template_instantiations.last());
                break;

            case Scanner::CommaToken:
                stack.pop();
                stack.top()->template_instantiations << Info();
                stack.push(&stack.top()->template_instantiations.last());
                break;

            case Scanner::GreaterThanToken:
                stack.pop();
                break;

            case Scanner::ColonToken:
                colon_prefix = true;
                break;

            case Scanner::ConstToken:
                if(lastWasStarToken){
                    stack.top()->indirections[stack.top()->indirections.size()-1] = true;
                }else{
                    stack.top()->is_constant = true;
                }
                break;

            case Scanner::OpenParenToken: // function pointers not supported
            case Scanner::CloseParenToken: {
                Info i;
                i.is_busted = true;
                return i;
            }

            case Scanner::Identifier:
                parseIdentifier(scanner, stack, array, in_array, colon_prefix);
                break;

            case Scanner::SquareBegin:
                in_array = true;
                break;

            case Scanner::SquareEnd:
                in_array = false;
                stack.top()->arrays += array;
                break;

            default:
                break;
        }

        lastWasStarToken = tok == Scanner::StarToken;
        tok = scanner.nextToken();
    }

    return info;
}

void TypeParser::parseIdentifier(Scanner &scanner, QStack<Info *> &stack, QString &array, bool in_array, bool &colon_prefix) {
    if (in_array) {
        array = scanner.identifier();
    } else if (colon_prefix || stack.top()->qualified_name.isEmpty()) {
        stack.top()->qualified_name << scanner.identifier();
        colon_prefix = false;
    } else {
        //stack.top()->qualified_name.last().append(" " + scanner.identifier());
        QString identifier = scanner.identifier();
        QString &name = stack.top()->qualified_name.last();

        bool set = false;
        if (identifier != "int") set = true;

        // TODO: this handles only one case
        if (name != "short" && !name.endsWith(" short")
                && name != "long" && !name.endsWith(" long")) {
            set = true;
        }

        if (set) name.append(" " + identifier);
    }
}

QString TypeParser::Info::instantiationName() const {
    QString s(qualified_name.join("::"));
    if (!template_instantiations.isEmpty()) {
        s += '<';
        for (int i = 0; i < template_instantiations.size(); ++i) {
            if (i != 0)
                s += ",";
            s += template_instantiations.at(i).toString();
        }
        s += '>';
    }

    return s;
}

QString TypeParser::Info::toString() const {
    QString s;

    if (is_constant) s += "const ";
    s += instantiationName();
    for (int i = 0; i < arrays.size(); ++i)
        s += "[" + arrays.at(i) + "]";
    for(int i=0; i<indirections.size(); i++){
        if(indirections[i]){
            s += "*const ";
        }else{
            s += "*";
        }
    }
    if (is_reference)  s += '&';

    return s;
}
