/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
#include <QException>

struct ScannerException : QException{
    inline ScannerException(const QByteArray& what) : QException(), m_what(what) {}
    inline char const* what() const noexcept{
        return m_what;
    }
    QByteArray m_what;
};

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
            VolatileToken,
            Identifier,
            EllipsisToken,
            NoToken
        };

        Scanner(const QString &s)
                : m_pos(0), m_length(s.length()), m_chars(s.constData()) {
        }

        Token nextToken();
        QString identifier() const;

    private:
        int m_pos;
        decltype(QString().size()) m_length;
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
                    if (c == '.' && m_pos+2 < m_length && m_chars[m_pos+1] == '.' && m_chars[m_pos+2] == '.'){
                        ++m_pos;
                        ++m_pos;
                        tok = EllipsisToken;
                    }else if (c.isLetterOrNumber() || c == '_')
                        tok = Identifier;
                    else
                        throw ScannerException(QString("Unrecognized character in lexer: %c found in %s").arg(c.toLatin1()).arg(QString(this->m_chars)).toUtf8());
                    break;
            }
        }

        if (tok <= GreaterThanToken || tok==EllipsisToken) {
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

    if (tok == Identifier && m_pos - m_token_start == 8) {
        if (m_chars[m_token_start] == 'v'
                && m_chars[m_token_start + 1] == 'o'
                && m_chars[m_token_start + 2] == 'l'
                && m_chars[m_token_start + 3] == 'a'
                && m_chars[m_token_start + 4] == 't'
                && m_chars[m_token_start + 5] == 'i'
                && m_chars[m_token_start + 6] == 'l'
                && m_chars[m_token_start + 7] == 'e')
            tok = VolatileToken;
    }

    return tok;

}

TypeParser::Info TypeParser::parse(const QString &str) {
    Info info;
    info.originalType = str;
    try{
        Scanner scanner(str);

        QStack<Info *> stack;
        stack.push(&info);

        bool colon_prefix = false;
        bool in_array = false;
        bool isFunctionPointerExpected = false;

        enum State{
            None,
            InParentheses,
            InTemplateDecl
        };

        State currentState = None;
        QStack<State> lastState;
        QString array;

        // search for 'T', 'T *', 'const T *', 'T const *', 'T * const' and so on
        bool lastWasStarToken = false;
        Scanner::Token tok = scanner.nextToken();
        while (tok != Scanner::NoToken) {

            Q_ASSERT(!stack.isEmpty());
            switch (tok) {
            case Scanner::EllipsisToken:
                stack.top()->is_variadics = true;
                break;
            case Scanner::StarToken:
                if(isFunctionPointerExpected){
                    stack.top()->is_functionPointer = true;
                }else{
                    //++stack.top()->indirections;
                    stack.top()->indirections << false;
                }
                break;

            case Scanner::AmpersandToken:
                if(stack.top()->reference_type==TypeParser::Info::Reference){
                    stack.top()->reference_type = TypeParser::Info::RReference;
                }else{
                    stack.top()->reference_type = TypeParser::Info::Reference;
                }
                break;

            case Scanner::LessThanToken:
                lastState.push(currentState);
                currentState = InTemplateDecl;
                stack.top()->arguments << Info();
                stack.push(&stack.top()->arguments.last());
                break;

            case Scanner::CommaToken:
                switch(currentState){
                case InTemplateDecl:
                    stack.pop();
                    Q_ASSERT(!stack.isEmpty());
                    stack.top()->arguments << Info();
                    stack.push(&stack.top()->arguments.last());
                    break;
                case InParentheses:
                    stack.pop();
                    Q_ASSERT(!stack.isEmpty());
                    stack.top()->functionalArgumentTypes << Info();
                    stack.push(&stack.top()->functionalArgumentTypes.last());
                    break;
                default:
                    Info i;
                    i.is_busted = true;
                    i.originalType = str;
                    return i;
                }
                break;

            case Scanner::GreaterThanToken:
                switch(currentState){
                case InTemplateDecl:
                    if(stack.top()->arguments.size()>0
                            && !stack.top()->arguments.last()){
                        stack.top()->arguments.takeLast();
                    }
                    break;
                default:
                    break;
                }
                currentState = lastState.pop();
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

            case Scanner::VolatileToken:
                stack.top()->is_volatile = true;
                break;

            case Scanner::OpenParenToken:
                switch(currentState){
                case InTemplateDecl:
                    lastState.push(currentState);
                    currentState = InParentheses;
                    stack.pop();
                    Q_ASSERT(!stack.isEmpty());
                    stack.top()->functionalReturnType << stack.top()->arguments;
                    stack.top()->arguments.clear();
                    stack.top()->functionalArgumentTypes << Info();
                    stack.push(&stack.top()->functionalArgumentTypes.last());
                    break;
                default:
                    lastState.push(currentState);
                    currentState = InParentheses;
                    if(stack.top()->is_functionPointer){
                        Info* info = stack.top();
                        Info returnType = *info;
                        returnType.originalType.clear();
                        returnType.is_functionPointer = false;
                        *info = Info();
                        info->originalType = str;
                        info->is_functionPointer = true;
                        info->functionalReturnType << returnType;
                        info->functionalArgumentTypes << Info();
                        stack.push(&info->functionalArgumentTypes.last());
                    }else{
                        isFunctionPointerExpected = true;
                    }
                }
                break;
            case Scanner::CloseParenToken:
                switch(currentState){
                case InParentheses:
                    currentState = lastState.pop();
                    if(stack.top()->functionalArgumentTypes.size()>0
                            && !stack.top()->functionalArgumentTypes.last()){
                        stack.top()->functionalArgumentTypes.takeLast();
                    }
                    isFunctionPointerExpected = false;
                    break;
                default:
                    Info i;
                    i.is_busted = true;
                    i.originalType = str;
                    return i;
                }
                break;
            case Scanner::Identifier:
                if(!isFunctionPointerExpected){
                    parseIdentifier(scanner, stack, array, in_array, colon_prefix);
                }
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
    }catch(const ScannerException& e){
        info.is_busted = true;
        qWarning("%s", e.what());
    }catch(const std::exception& e){
        info.is_busted = true;
        qWarning("%s", e.what());
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
    if (!arguments.isEmpty()) {
        s += '<';
        for (int i = 0; i < arguments.size(); ++i) {
            if (i != 0)
                s += ",";
            s += arguments.at(i).toString();
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
    if(reference_type==RReference){
        s += '&';
        s += '&';
    }else if(reference_type==Reference){
        s += '&';
    }

    return s;
}
