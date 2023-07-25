/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
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

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */


// c++ support
#include "parser.h"
#include "tokens.h"
#include "lexer.h"
#include "control.h"
#include "reporthandler.h"

#include <cstdlib>
#include <iostream>
#include <limits>

#define ADVANCE(tk, descr) \
{ \
  if (token_stream.lookAhead() != tk) { \
      tokenRequiredError(tk); \
      return false; \
  } \
  token_stream.nextToken(); \
}

#define ADVANCE_NR(tk, descr) \
  do { \
    if (token_stream.lookAhead() != tk) { \
      tokenRequiredError(tk); \
    } \
    else \
        token_stream.nextToken(); \
  } while (0)

#define CHECK(tk) \
  do { \
    if (token_stream.lookAhead() != tk) { \
        return false; \
    } \
    token_stream.nextToken(); \
  } while (0)

#define UPDATE_POS(_node, start, end) \
  do { \
      (_node)->start_token = start; \
      (_node)->end_token = end; \
  } while (0)

Parser::Parser(Control *c, QMap<QString,QStringList>& requiredFeatures)
        : _M_location(token_stream, location_table, line_table, requiredFeatures),
        control(c),
        lexer(_M_location, control),
        m_isInTemplate(0),
        m_isShift(false),
        m_isTemplateDecl(false),
        m_isExpression(false)
{
    _M_block_errors = false;
}

Parser::~Parser() {
}

void Parser::advance() {
    token_stream.nextToken();
}

TranslationUnitAST *Parser::parse(const char *contents,
                                  std::size_t size, pool *p) {
    _M_block_errors = false;
    _M_pool = p;
    ReportHandler::setContext("Lexer");
    lexer.tokenize(contents, size);
    ReportHandler::setContext("Parser");
    token_stream.nextToken(); // skip the first token

    Lexer *oldLexer = control->changeLexer(&lexer);
    Parser *oldParser = control->changeParser(this);

    TranslationUnitAST *ast = nullptr;
    parseTranslationUnit(ast);

    control->changeLexer(oldLexer);
    control->changeParser(oldParser);

    return ast;
}

bool Parser::parseDeclFinal(QDeclFinalAST *&node){
    bool result = false;
    while(token_stream.lookAhead() == Token_identifier) {
        const NameSymbol *symbol = token_stream.symbol(token_stream.cursor());
        QString annotation = symbol->as_string();
        if(annotation=="override" || annotation=="Q_DECL_NOTHROW" || annotation=="noexcept"){
            token_stream.nextToken();
            // declare override does not matter for generator
            result = true;
        }else if(annotation=="final" || annotation=="Q_DECL_FINAL"){
            node = CreateNode<QDeclFinalAST>(_M_pool);
            std::size_t start = token_stream.cursor();
            UPDATE_POS(node, start, token_stream.cursor());
            token_stream.nextToken();
            result = true;
        }else{
            break;
        }
    }
    return result;
}

bool Parser::parseWinDeclSpec(WinDeclSpecAST *&node) {
    if (token_stream.lookAhead() != Token_identifier)
        return false;

    std::size_t start = token_stream.cursor();

    const NameSymbol *name_symbol = token_stream.symbol(token_stream.cursor());
    QString name = name_symbol->as_string();
    if (name != QLatin1String("__declspec"))
        return false;
    std::size_t specifier = token_stream.cursor();

    token_stream.nextToken();
    if (token_stream.lookAhead() != '(')
        return false;

    token_stream.nextToken();
    if (token_stream.lookAhead() != Token_identifier)
        return false;
    std::size_t modifier = token_stream.cursor();

    token_stream.nextToken();
    if (token_stream.lookAhead() != ')')
        return false;

    token_stream.nextToken();

    node = CreateNode<WinDeclSpecAST>(_M_pool);
    node->specifier = specifier;
    node->modifier = modifier;

    UPDATE_POS(node, start, token_stream.cursor());

    return true;
}

void Parser::tokenRequiredError(int token) {
    QString err;

    err += "expected token ";
    err += "\"";
    err += token_name(token);
    err += "\" found \"";
    err += token_name(token_stream.lookAhead());
    err += "\"";
    if(token_stream.lookAhead()==Token_identifier){
        const NameSymbol *name_symbol = token_stream.symbol(token_stream.cursor());
        if(name_symbol){
            err += " (\"";
            err += name_symbol->as_string();
            err += "\")";
        }
    }

    reportError(err);
}

void Parser::syntaxError() {
    QString err;

    err += "unexpected token ";
    err += "\"";
    err += token_name(token_stream.lookAhead());
    err += "\"";
    if(token_stream.lookAhead()==Token_identifier){
        const NameSymbol *name_symbol = token_stream.symbol(token_stream.cursor());
        if(name_symbol){
            err += " (\"";
            err += name_symbol->as_string();
            err += "\")";
        }
    }else{
        size_t prevTokens = 4;
        if(token_stream.cursor()>prevTokens){
            err += " (context: \"";
            const Token &tk = token_stream.token(token_stream.cursor()-prevTokens);
            const Token &end_tk = token_stream.token(token_stream.cursor()+prevTokens);
            err += QString::fromLatin1(tk.text + tk.position, int(end_tk.position - tk.position)).replace("\r\n", " ").replace("\n", " ");
            err += "\")";
        }
    }

    reportError(err);
}

void Parser::reportError(const QString& msg) {
    if (!_M_block_errors) {
        int line, column;
        QString fileName;

        std::size_t tok = token_stream.cursor();
        location().positionAt(token_stream.position(tok),
                              &line, &column, &fileName);

        Control::ErrorMessage errmsg;
        errmsg.setLine(line + 1);
        errmsg.setColumn(column);
        errmsg.setFileName(fileName);
        errmsg.setMessage(msg);
        control->reportError(errmsg);
    }
}

bool Parser::skipUntil(int token) {
    while (token_stream.lookAhead()) {
        if (token_stream.lookAhead() == token)
            return true;

        token_stream.nextToken();
    }

    return false;
}

bool Parser::skipUntilDeclaration() {
    while (token_stream.lookAhead()) {

        switch (token_stream.lookAhead()) {
            case ';':
            case '~':
            case Token_scope:
            case Token_identifier:
            case Token_operator:
            case Token_char:
            case Token_wchar_t:
            case Token_bool:
            case Token_short:
            case Token_int:
            case Token_long:
            case Token_signed:
            case Token_unsigned:
            case Token_float:
            case Token_double:
            case Token_void:
            case Token_extern:
            case Token_namespace:
            case Token_using:
            case Token_typedef:
            case Token_asm:
            case Token_template:
            case Token_export:
            case Token_auto:
            case Token_constexpr:

            case Token_const:       // cv
            case Token_volatile:    // cv

            case Token_public:
            case Token_protected:
            case Token_private:
            case Token_signals:      // Qt
            case Token_slots:        // Qt
                return true;

            default:
                token_stream.nextToken();
        }
    }

    return false;
}

bool Parser::skipUntilStatement() {
    while (token_stream.lookAhead()) {
        switch (token_stream.lookAhead()) {
            case ';':
            case '{':
            case '}':
            case Token_const:
            case Token_constexpr:
            case Token_volatile:
            case Token_identifier:
            case Token_case:
            case Token_default:
            case Token_if:
            case Token_switch:
            case Token_while:
            case Token_do:
            case Token_for:
            case Token_break:
            case Token_continue:
            case Token_return:
            case Token_goto:
            case Token_try:
            case Token_catch:
            case Token_throw:
            case Token_noexcept:
            case Token_char:
            case Token_wchar_t:
            case Token_bool:
            case Token_short:
            case Token_int:
            case Token_long:
            case Token_auto:
            case Token_signed:
            case Token_unsigned:
            case Token_float:
            case Token_double:
            case Token_void:
            case Token_class:
            case Token_struct:
            case Token_union:
            case Token_enum:
            case Token_scope:
            case Token_template:
            case Token_using:
                return true;

            default:
                token_stream.nextToken();
        }
    }

    return false;
}

bool Parser::skip(int l, int r) {
    int count = 0;
    while (token_stream.lookAhead()) {
        int tk = token_stream.lookAhead();

        if (tk == l)
            ++count;
        else if (tk == r)
            --count;
        else if (l != '{' && (tk == '{' || tk == '}' || tk == ';'))
            return false;

        if (count == 0)
            return true;

        token_stream.nextToken();
    }

    return false;
}

bool Parser::parseName(NameAST *&node, bool acceptTemplateId) {
    std::size_t start = token_stream.cursor();

    WinDeclSpecAST *winDeclSpec = nullptr;
    parseWinDeclSpec(winDeclSpec);

    NameAST *ast = CreateNode<NameAST>(_M_pool);

    if (token_stream.lookAhead() == Token_scope) {
        ast->global = true;
        token_stream.nextToken();
    }

    std::size_t idx = token_stream.cursor();

    while (true) {
        UnqualifiedNameAST *n = nullptr;
        if (!parseUnqualifiedName(n))
            return false;

        if (token_stream.lookAhead() == Token_scope) {
            token_stream.nextToken();

            ast->qualified_names
            = snoc(ast->qualified_names, n, _M_pool);

            if (token_stream.lookAhead() == Token_template) {
                /// skip optional template     #### @todo CHECK
                token_stream.nextToken();
            }
        } else {
            Q_UNUSED(acceptTemplateId)
            /* // c++-17: templates are now also accepted as fields
            Q_ASSERT(n);
            if (!acceptTemplateId) {
                token_stream.rewind(n->start_token);
                parseUnqualifiedName(n, false);
            }
            */

            ast->unqualified_name = n;
            break;
        }
    }

    if (idx == token_stream.cursor())
        return false;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseTranslationUnit(TranslationUnitAST *&node) {
    std::size_t start = token_stream.cursor();
    TranslationUnitAST *ast = CreateNode<TranslationUnitAST>(_M_pool);

    while (token_stream.lookAhead()) {
        std::size_t startDecl = token_stream.cursor();

        DeclarationAST *declaration = nullptr;
        if (parseDeclaration(declaration)) {
            ast->declarations =
                snoc(ast->declarations, declaration, _M_pool);
        } else {
            // error recovery
            if (startDecl == token_stream.cursor()) {
                // skip at least one token
                token_stream.nextToken();
            }

            skipUntilDeclaration();
        }
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseDeclaration(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    if(token_stream.lookAhead()==Token_inline
            && token_stream.lookAhead(1)==Token_namespace)
        return parseNamespace(node);

    switch (token_stream.lookAhead()) {
        case ';':
            token_stream.nextToken();
            return true;
        case Token_QT_REQUIRE_CONFIG:{
            token_stream.nextToken();
            ADVANCE('(' , "(")
            NameAST *name = nullptr;
            bool isExpression = m_isExpression;
            m_isExpression = false;
            parseName(name, true);
            m_isExpression = isExpression;
            QString feature = name->toString(&token_stream).trimmed();
            ADVANCE(')', ")")
            _M_location.addRequiredFeature(token_stream.position(name->start_token), feature);
        }
            return true;
        case Token_extern:
            return parseLinkageSpecification(node);

        case Token_namespace:
            return parseNamespace(node);

        case Token_using:
            return parseUsing(node);

        case Token_typedef:
            return parseTypedef(node);

        case Token_asm:
            return parseAsmDefinition(node);

        case Token_template:
        case Token_export:
            return parseTemplateDeclaration(node);

        case Token_QTJAMBI_DEPRECATED:
        case Token_QTJAMBI_DEPRECATED_X:
            switch (token_stream.lookAhead()) {
            case Token_QTJAMBI_DEPRECATED:
                switch (token_stream.lookAhead(1)){
                case Token_typedef:
                    return parseTypedef(node);
                }
                break;
            case Token_QTJAMBI_DEPRECATED_X:{
                    std::size_t start = token_stream.cursor();
                    token_stream.nextToken();
                    ADVANCE('(', "(")
                    StringLiteralAST *comment = nullptr;
                    if(!parseStringLiteral(comment)){
                        token_stream.rewind(start);
                        return false;
                    }
                    ADVANCE(')', ")")
                    switch (token_stream.lookAhead()){
                    case Token_typedef:
                        token_stream.rewind(start);
                        return parseTypedef(node);
                    }
                    token_stream.rewind(start);
                }
                break;
            default: break;
        }
        Q_FALLTHROUGH();

        default: {
        ExpressionAST *annotationExpression(nullptr);
            if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
                token_stream.nextToken();
                token_stream.nextToken();
                if(!parseExpression(annotationExpression)){
                    token_stream.rewind(start);
                }else{
                    if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
                        token_stream.nextToken();
                        token_stream.nextToken();
                    }else{
                        token_stream.rewind(start);
                    }
                }
            }
            if (token_stream.lookAhead() == Token_typedef){
                token_stream.rewind(start);
                return parseTypedef(node);
            }
            if (token_stream.lookAhead() == Token_inline)
                token_stream.nextToken();

            const ListNode<std::size_t> *cv = nullptr;
            parseCvQualify(cv);

            const ListNode<std::size_t> *storageSpec = nullptr;
            StringLiteralAST *deprecationComment = nullptr;
            bool hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);
            if (!annotationExpression && token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
                token_stream.nextToken();
                token_stream.nextToken();
                if(!parseExpression(annotationExpression)){
                    token_stream.rewind(start);
                }else{
                    if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
                        token_stream.nextToken();
                        token_stream.nextToken();
                    }else{
                        token_stream.rewind(start);
                    }
                }
            }

            parseStorageClassSpecifier(storageSpec);

            if(!hasDeprecated)
                hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);
            if (!annotationExpression && token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
                token_stream.nextToken();
                token_stream.nextToken();
                if(!parseExpression(annotationExpression)){
                    token_stream.rewind(start);
                }else{
                    if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
                        token_stream.nextToken();
                        token_stream.nextToken();
                    }else{
                        token_stream.rewind(start);
                    }
                }
            }

            parseCvQualify(cv);

            if(!hasDeprecated)
                hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);
            if (!annotationExpression && token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
                token_stream.nextToken();
                token_stream.nextToken();
                if(!parseExpression(annotationExpression)){
                    token_stream.rewind(start);
                }else{
                    if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
                        token_stream.nextToken();
                        token_stream.nextToken();
                    }else{
                        token_stream.rewind(start);
                    }
                }
            }

            TypeSpecifierAST *spec = nullptr;
            if (parseEnumSpecifier(spec)
                    || parseEnumClassSpecifier(spec)
                    || parseClassSpecifier(spec)
                    || parseForwardDeclarationSpecifier(spec)) {
                parseCvQualify(cv);

                spec->cv = cv;

                const ListNode<InitDeclaratorAST*> *declarators = nullptr;
                parseInitDeclaratorList(declarators, false);

                if(token_stream.lookAhead()==Token_arrow){
                    token_stream.nextToken();
                    TypeSpecifierAST * typeSpecifier = nullptr;
                    if(token_stream.lookAhead()==Token_typename){
                        token_stream.nextToken();
                    }
                    const ListNode<std::size_t> *storage = nullptr;
                    parseStorageClassSpecifier(storage);
                    if(!parseTypeSpecifierOrClassSpec(typeSpecifier)){
                        token_stream.rewind(start);
                        return false;
                    }
                    PtrOperatorAST *ptrOp = nullptr;
                    while (parsePtrOperator(ptrOp)) {
                        //ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
                    }
//                    ExpressionAST *typeExpression = nullptr;
//                    if(!parseExpression(typeExpression)
//                            && !parseTypeSpecifier(typeSpecifier)){
//                        syntaxError();
//                        return false;
//                    }
//                    spec->auto_type_expression = typeExpression;
//                    spec->auto_type_specifier = typeSpecifier;

                    if(token_stream.lookAhead()!=';'){
                        // not an auto-typed function declaration
                        break; // break switch
                    }
                }

                ADVANCE(';', ";")

                SimpleDeclarationAST *ast =
                    CreateNode<SimpleDeclarationAST>(_M_pool);

                ast->storage_specifiers = storageSpec;
                ast->deprecationComment = deprecationComment;
                ast->type_specifier = spec;
                ast->init_declarators = declarators;
                ast->annotationExpression = annotationExpression;
                UPDATE_POS(ast, start, token_stream.cursor());
                node = ast;

                return true;
            }
        }
    } // end switch

    token_stream.rewind(start);
    return parseDeclarationInternal(node);
}

bool Parser::parseLinkageSpecification(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(Token_extern);

    LinkageSpecificationAST *ast = CreateNode<LinkageSpecificationAST>(_M_pool);

    if (token_stream.lookAhead() == Token_string_literal || token_stream.lookAhead() == Token_rawstring_literal) {
        ast->extern_type = token_stream.cursor();
        token_stream.nextToken();
    }

    if (token_stream.lookAhead() == '{') {
        parseLinkageBody(ast->linkage_body);
    } else if (!parseDeclaration(ast->declaration)) {
        reportError(("Declaration syntax error"));
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseLinkageBody(LinkageBodyAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK('{');

    LinkageBodyAST *ast = CreateNode<LinkageBodyAST>(_M_pool);

    while (token_stream.lookAhead()) {
        int tk = token_stream.lookAhead();

        if (tk == '}')
            break;

        std::size_t startDecl = token_stream.cursor();

        DeclarationAST *declaration = nullptr;
        if (parseDeclaration(declaration)) {
            ast->declarations = snoc(ast->declarations, declaration, _M_pool);
        } else {
            // error recovery
            if (startDecl == token_stream.cursor()) {
                // skip at least one token
                token_stream.nextToken();
            }

            skipUntilDeclaration();
        }
    }

    if (token_stream.lookAhead() != '}')
        reportError(("} expected"));
    else
        token_stream.nextToken();

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseNamespace(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();
    if (token_stream.lookAhead() == Token_inline) {
        token_stream.nextToken();
    }
    CHECK(Token_namespace);

    std::size_t namespace_name = 0;
    std::size_t namespace_name_size = 0;
    if (token_stream.lookAhead() == Token_identifier) {
        namespace_name = token_stream.cursor();
        token_stream.nextToken();
        ++namespace_name_size;
    }
    while(token_stream.lookAhead() == Token_scope
          && token_stream.lookAhead(1) == Token_identifier) {
        token_stream.nextToken();
        token_stream.nextToken();
        ++namespace_name_size;
    }

    if (token_stream.lookAhead() == '=') {
        // namespace alias
        token_stream.nextToken();

        NameAST *name = nullptr;
        bool isExpression = m_isExpression;
        m_isExpression = false;
        bool p = parseName(name);
        m_isExpression = isExpression;
        if (p) {
            ADVANCE(';', ";")

            NamespaceAliasDefinitionAST *ast
            = CreateNode<NamespaceAliasDefinitionAST>(_M_pool);
            ast->namespace_name = namespace_name;
            ast->alias_name = name;
            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
            return true;
        } else {
            reportError(("namespace expected"));
            return false;
        }
    } else if (token_stream.lookAhead() != '{') {
        reportError(("{ expected"));
        return false;
    }

    NamespaceAST *ast = CreateNode<NamespaceAST>(_M_pool);
    ast->namespace_name = namespace_name;
    ast->namespace_name_size = namespace_name_size;
    parseLinkageBody(ast->linkage_body);

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseUsing(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(Token_using);

    if (token_stream.lookAhead() == Token_namespace)
        return parseUsingDirective(node);

    std::size_t type_name_token = 0;
    if (token_stream.lookAhead() == Token_typename) {
        type_name_token = token_stream.cursor();
        token_stream.nextToken();
    }

    NameAST *name = nullptr;
    bool isExpression = m_isExpression;
    m_isExpression = false;
    bool p = parseName(name);
    m_isExpression = isExpression;
    if (!p)
        return false;

    const ListNode<std::size_t> *storageSpec = nullptr;
    StringLiteralAST *deprecationComment = nullptr;
    parseDeprecatedSpecifier(storageSpec, deprecationComment);

    ExpressionAST *annotationExpression(nullptr);
    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }

    if (token_stream.lookAhead() == '=') {
        const ListNode<std::size_t> *cv = nullptr;
        parseCvQualify(cv);
        token_stream.nextToken();
        TypeSpecifierAST *spec = nullptr;
        if (!parseTypeSpecifierOrClassSpec(spec)) {
            reportError(("Need a type specifier to declare"));
            return false;
        }
        if(cv){
            spec->cv = cv;
        }
        parseCvQualify(cv);
        if(!spec->cv){
            spec->cv = cv;
        }else if(cv){
            spec->cv->next = cv;
        }
        const ListNode<PtrOperatorAST*> *ptr_ops = nullptr;
        PtrOperatorAST *ptrOp = nullptr;
        while (parsePtrOperator(ptrOp)) {
            ptr_ops = snoc(ptr_ops, ptrOp, _M_pool);
        }
        if (token_stream.lookAhead() == '('){
            size_t offset = 1;
            while(token_stream.lookAhead(offset) == Token_identifier
                  && token_stream.lookAhead(offset+1) == Token_scope){
                offset += 2;
            }
            if((token_stream.lookAhead(offset) == '*' || token_stream.lookAhead(offset) == '&')
                    && token_stream.lookAhead(offset+1) == ')'){
                offset += 2;
                token_stream.rewind(token_stream.cursor()+offset);
            }
        }

        ParameterDeclarationClauseAST *parameters = nullptr;
        if (token_stream.lookAhead() == '(') {
            token_stream.nextToken();
            if(!parseParameterDeclarationClause(parameters)){
                return false;
            }else{
                if (token_stream.lookAhead() == ')'){
                    token_stream.nextToken();
                    if (token_stream.lookAhead() == Token_noexcept){
                        token_stream.nextToken();
                        if (token_stream.lookAhead() == '('){
                            std::size_t start = token_stream.cursor();
                            CHECK('(');
                            ExpressionAST *expr = nullptr;
                            if (parseExpression(expr)){
                                CHECK(')');
                            }else{
                                token_stream.rewind(start);
                            }
                        }
                    }
                }
            }
        }

        if (token_stream.lookAhead() == '[') {
            while (token_stream.lookAhead() == '[') {
                token_stream.nextToken();

                ExpressionAST *expr = nullptr;
                parseCommaExpression(expr);

                ADVANCE(']', "]")
            }
        }

        DeclaratorAST* declarator = CreateNode<DeclaratorAST>(_M_pool);
        declarator->ptr_ops = ptr_ops;
        declarator->parameter_declaration_clause = parameters;

        UsingAsAST *ast = CreateNode<UsingAsAST>(_M_pool);
        ast->name = name;
        ast->type_specifier = spec;
        ast->declarator = declarator;
        node = ast;
    }else{
        UsingAST *ast = CreateNode<UsingAST>(_M_pool);
        ast->name = name;
        ast->type_name = type_name_token;
        node = ast;
    }

    ADVANCE(';', ";")

    UPDATE_POS(node, start, token_stream.cursor());

    return true;
}

bool Parser::parseUsingDirective(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(Token_namespace);

    NameAST *name = nullptr;
    bool isExpression = m_isExpression;
    m_isExpression = false;
    bool p = parseName(name);
    m_isExpression = isExpression;
    if (!p) {
        reportError(("Namespace name expected"));
        return false;
    }

    ADVANCE(';', ";")

    UsingDirectiveAST *ast = CreateNode<UsingDirectiveAST>(_M_pool);
    ast->name = name;
    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}


bool Parser::parseOperatorFunctionId(OperatorFunctionIdAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(Token_operator);

    OperatorFunctionIdAST *ast = CreateNode<OperatorFunctionIdAST>(_M_pool);

    if (!parseOperator(ast->op)) {
        ast->op = nullptr;

        if (token_stream.lookAhead() == Token_string_literal
                && token_stream.lookAhead(1) == Token_identifier) {
            ast->type_name = token_stream.cursor();
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            if (token_stream.lookAhead() == Token_typename) {
                ast->type_name = token_stream.cursor();
                token_stream.nextToken();
            }

            // parse cast operator
            const ListNode<std::size_t> *cv = nullptr;
            parseCvQualify(cv);

            if (!parseSimpleTypeSpecifier(ast->type_specifier)) {
                syntaxError();
                return false;
            }

            parseCvQualify(cv);
            ast->type_specifier->cv = cv;

            PtrOperatorAST *ptr_op = nullptr;
            while (parsePtrOperator(ptr_op))
                ast->ptr_ops = snoc(ast->ptr_ops, ptr_op, _M_pool);
        }
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;
    return true;
}

bool Parser::parseTemplateArgumentList(const ListNode<TemplateArgumentAST*> *&node,
                                       bool reportError) {
    TemplateArgumentAST *templArg = nullptr;
    if (!parseTemplateArgument(templArg))
        return false;

    node = snoc(node, templArg, _M_pool);

    while (token_stream.lookAhead() == ',') {
        token_stream.nextToken();

        if (!parseTemplateArgument(templArg)) {
            if (reportError) {
                syntaxError();
                break;
            }

            node = nullptr;
            return false;
        }

        node = snoc(node, templArg, _M_pool);
    }

    return true;
}

bool Parser::parseTypedef(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    ExpressionAST *annotationExpression(nullptr);
    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }
    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED) {
        token_stream.nextToken();
        if (token_stream.lookAhead() != Token_typedef) {
            token_stream.rewind(start);
        }
    }
    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED_X) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        StringLiteralAST *comment = nullptr;
        if(!parseStringLiteral(comment)){
            token_stream.rewind(start);
            return false;
        }
        ADVANCE(')', ")")
        if (token_stream.lookAhead() != Token_typedef) {
            token_stream.rewind(start);
        }
    }

    CHECK(Token_typedef);

    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }

    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED) {
        token_stream.nextToken();
    }
    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED_X) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        StringLiteralAST *comment = nullptr;
        if(!parseStringLiteral(comment)){
            token_stream.rewind(start);
            return false;
        }
        ADVANCE(')', ")")
    }

    TypeSpecifierAST *spec = nullptr;
    if (!parseTypeSpecifierOrClassSpec(spec)) {
        reportError(("Need a type specifier to declare"));
        return false;
    }

    const ListNode<InitDeclaratorAST*> *declarators = nullptr;
    if (!parseInitDeclaratorList(declarators, true)) {
        //reportError(("Need an identifier to declare"));
        //return false;
    }

    ADVANCE(';', ";")

    TypedefAST *ast = CreateNode<TypedefAST>(_M_pool);
    ast->type_specifier = spec;
    ast->init_declarators = declarators;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;


    return true;
}

bool Parser::parseAsmDefinition(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    ADVANCE(Token_asm, "asm")

    const ListNode<std::size_t> *cv = nullptr;
    parseCvQualify(cv);

    skip('(', ')');
    token_stream.nextToken();
    ADVANCE(';', ";")

    AsmDefinitionAST *ast = CreateNode<AsmDefinitionAST>(_M_pool);
    ast->cv = cv;
    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseTemplateDeclaration(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    std::size_t exported = 0;
    if (token_stream.lookAhead() == Token_export) {
        exported = token_stream.cursor();
        token_stream.nextToken();
    }

    CHECK(Token_template);

    const ListNode<TemplateParameterAST*> *params = nullptr;
    if (token_stream.lookAhead() == '<') {
        token_stream.nextToken();
        parseTemplateParameterList(params);

        ADVANCE('>', ">")
    }

    DeclarationAST *declaration = nullptr;
    m_isTemplateDecl = true;
    if (!parseDeclaration(declaration)) {
        m_isTemplateDecl = false;
        reportError(("expected a declaration"));
    }
    m_isTemplateDecl = false;

    TemplateDeclarationAST *ast = CreateNode<TemplateDeclarationAST>(_M_pool);
    ast->exported = exported;
    ast->template_parameters = params;
    ast->declaration = declaration;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseOperator(OperatorAST *&node) {
    std::size_t start = token_stream.cursor();

    OperatorAST *ast = CreateNode<OperatorAST>(_M_pool);

    switch (token_stream.lookAhead()) {
        case Token_new:
        case Token_delete: {
            ast->op = token_stream.cursor();
            token_stream.nextToken();
            if (token_stream.lookAhead() == '['
                    && token_stream.lookAhead(1) == ']') {
                ast->opLength = 2;
                token_stream.nextToken();
                token_stream.nextToken();
            }
        }
        break;

        case '+':
        case Token_plus_assign:
        case '-':
        case Token_minus_assign:
        case '*':
        case Token_times_assign:
        case '/':
        case Token_div_assign:
        case '%':
        case Token_rem_assign:
        case '^':
        case Token_xor_eq:
        case '&':
        case Token_and_eq:
        case '|':
        case Token_or_eq:
        case '~':
        case '!':
        case Token_not_eq:
        case '=':
        case '<':
        case ',':
        case Token_assign:
        case Token_left_shift:
        case Token_left_shift_assign:
        case Token_right_shift_assign:
        case Token_eq:
        case Token_leq:
        case Token_geq:
        case Token_compare:
        case Token_and:
        case Token_or:
        case Token_incr:
        case Token_decr:
        case Token_ptrmem:
        case Token_arrow:
            ast->op = token_stream.cursor();
            token_stream.nextToken();
            break;
        case '>':
            ast->op = token_stream.cursor();
            token_stream.nextToken();
            if(token_stream.lookAhead() == '>'){
                ast->opLength = 1;
                token_stream.nextToken();
            }
        break;
        case '(':
            ast->op = token_stream.cursor();
            token_stream.nextToken();
            if(token_stream.lookAhead() == ')'){
                ast->opLength = 1;
                token_stream.nextToken();
            }
        break;
        case '[':
            ast->op = token_stream.cursor();
            token_stream.nextToken();
            if(token_stream.lookAhead() == ']'){
                ast->opLength = 1;
                token_stream.nextToken();
            }
        break;
        default:
            return false;
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseCvQualify(const ListNode<std::size_t> *&node) {
    std::size_t start = token_stream.cursor();

    int tk;
    while (0 != (tk = token_stream.lookAhead())
            && (tk == Token_const || tk == Token_constexpr || tk == Token_volatile || (tk == '[' && token_stream.lookAhead(1) == '['))) {
        if (tk == '[') {
            token_stream.nextToken();
            token_stream.nextToken();
            ExpressionAST *annotationExpression(nullptr);
            if(!parseExpression(annotationExpression)){
                token_stream.rewind(start);
            }else{
                if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
                    token_stream.nextToken();
                    token_stream.nextToken();
                }else{
                    token_stream.rewind(start);
                    break;
                }
            }
        }else{
            node = snoc(node, token_stream.cursor(), _M_pool);
            token_stream.nextToken();
        }
    }
    return start != token_stream.cursor();
}

bool Parser::parseSimpleTypeSpecifier(TypeSpecifierAST *&node,
                                      bool onlyIntegral) {
    std::size_t start = token_stream.cursor();
    bool isIntegral = false;
    bool done = false;

    const ListNode<std::size_t> *integrals = nullptr;

    while (!done) {
        switch (token_stream.lookAhead()) {
            case Token_char:
            case Token_wchar_t:
            case Token_bool:
            case Token_short:
            case Token_int:
            case Token_long:
            case Token_signed:
            case Token_unsigned:
            case Token_float:
            case Token_double:
            case Token_void:
            case Token_auto:
                integrals = snoc(integrals, token_stream.cursor(), _M_pool);
                isIntegral = true;
                token_stream.nextToken();
                break;

            default:
                done = true;
        }
    }

    SimpleTypeSpecifierAST *ast = CreateNode<SimpleTypeSpecifierAST>(_M_pool);
    if (isIntegral) {
        ast->integrals = integrals;
    } else if (token_stream.lookAhead() == Token___typeof) {
        ast->type_of = token_stream.cursor();
        token_stream.nextToken();

        if (token_stream.lookAhead() == '(') {
            token_stream.nextToken();

            std::size_t saved = token_stream.cursor();
            parseTypeId(ast->type_id);
            if (token_stream.lookAhead() != ')') {
                ast->type_id = nullptr;
                token_stream.rewind(saved);
                parseUnaryExpression(ast->expression);
            }
            ADVANCE(')', ")")
        } else {
            parseUnaryExpression(ast->expression);
        }
    } else if (onlyIntegral) {
        token_stream.rewind(start);
        return false;
    } else {
        if (!parseName(ast->name, true)) {
            ast->name = nullptr;
            token_stream.rewind(start);
            return false;
        }
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parsePtrOperator(PtrOperatorAST *&node) {
    int tk = token_stream.lookAhead();

    if (tk != Token_and && tk != '&' && tk != '*'
            && tk != Token_scope && tk != Token_identifier) {
        return false;
    }

    std::size_t start = token_stream.cursor();

    PtrOperatorAST *ast = CreateNode<PtrOperatorAST>(_M_pool);

    switch (token_stream.lookAhead()) {
        case Token_and:
        case '&':
        case '*':
            ast->op = token_stream.cursor();
            token_stream.nextToken();
            break;

        case Token_scope:
        case Token_identifier: {
            if (!parsePtrToMember(ast->mem_ptr)) {
                token_stream.rewind(start);
                return false;
            }
        }
        break;

        default:
            Q_ASSERT(0);
            break;
    }

    parseCvQualify(ast->cv);

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseTemplateArgument(TemplateArgumentAST *&node) {
    std::size_t start = token_stream.cursor();

    TypeIdAST *typeId = nullptr;
    ExpressionAST *expr = nullptr;

    if (!parseTypeId(typeId) || (token_stream.lookAhead() != ','
                                 && token_stream.lookAhead() != '>')) {
        token_stream.rewind(start);

        if (!parseLogicalOrExpression(expr, true))
            return false;
    }

    TemplateArgumentAST *ast = CreateNode<TemplateArgumentAST>(_M_pool);
    ast->type_id = typeId;
    ast->expression = expr;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;
    return true;
}

bool Parser::parseTypeSpecifier(TypeSpecifierAST *&node) {
    std::size_t start = token_stream.cursor();

    const ListNode<std::size_t> *cv = nullptr;
    parseCvQualify(cv);

    TypeSpecifierAST *ast = nullptr;
    if(token_stream.lookAhead()==Token_decltype){
        token_stream.nextToken();
        CHECK('(');
        ExpressionAST *expr = nullptr;
        if (!parseExpression(expr)){
            token_stream.rewind(start);
            return false;
        }
        CHECK(')');
        DeclTypeSpecifierAST *dast;
        ast = dast = CreateNode<DeclTypeSpecifierAST>(_M_pool);
        dast->expression = expr;
        UPDATE_POS(ast, start, token_stream.cursor());
    }else if (!parseElaboratedTypeSpecifier(ast) && !parseSimpleTypeSpecifier(ast)) {
        token_stream.rewind(start);
        return false;
    }

    parseCvQualify(cv);
    ast->cv = cv;

    if(token_stream.lookAhead()==Token_ellipsis){
        ast->ellipsis = token_stream.cursor();
        token_stream.nextToken();
    }else{
        ast->ellipsis = std::numeric_limits<std::size_t>::max();
    }

    node = ast;

    return true;
}

bool Parser::parseDeclarator(DeclaratorAST *&node, bool iteratorFor) {
    std::size_t start = token_stream.cursor();
    DeclaratorAST *ast = CreateNode<DeclaratorAST>(_M_pool);
    ast->decl_final = nullptr;
    ast->ellipsis = 0;
    ast->fun_reference = 0;

    DeclaratorAST *decl = nullptr;
    NameAST *declId = nullptr;

    PtrOperatorAST *ptrOp = nullptr;
    while (parsePtrOperator(ptrOp)) {
        ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
    }

    if (token_stream.lookAhead() == '(') {
        token_stream.nextToken();

        if (!parseDeclarator(decl))
            return false;

        ast->sub_declarator = decl;

        CHECK(')');
        if(!ast->decl_final){
            QDeclFinalAST* decl_final = nullptr;
            parseDeclFinal(decl_final);
            ast->decl_final = decl_final;
        }
    } else {
        if (token_stream.lookAhead() == Token_ellipsis) {
            ast->ellipsis = token_stream.cursor();
            token_stream.nextToken();
        }
        if (!iteratorFor && token_stream.lookAhead() == ':') {
            // unnamed bitfield
        } else if (parseName(declId, true)) {
            ast->id = declId;
        } else if (token_stream.lookAhead() == '['){
            token_stream.nextToken();
            do{
                if (parseName(declId, true)) {
                    ast->id = declId;
                    if(token_stream.lookAhead() == ',')
                        token_stream.nextToken();
                    else
                        break;
                }else{
                    token_stream.rewind(start);
                    return false;
                }
            }while(true);
            CHECK(']');
        } else {
            token_stream.rewind(start);
            return false;
        }

        if (!iteratorFor && token_stream.lookAhead() == ':') {
            token_stream.nextToken();

            if (!parseConstantExpression(ast->bit_expression)) {
                reportError(("Constant expression expected"));
            }
            goto update_pos;
        }
    }

    {
        if (token_stream.lookAhead() == '{') {
            std::size_t index = token_stream.cursor();
            token_stream.nextToken();
            if (token_stream.lookAhead() == '}') {
                token_stream.nextToken();
                //ast->parameter_declaration_clause = CreateNode<ParameterDeclarationClauseAST>(_M_pool);
            }else{
                ExpressionAST *node = nullptr;
                if(parseCommaExpression(node)){
                    //ast->parameter_declaration_clause = CreateNode<ParameterDeclarationClauseAST>(_M_pool);
                    if (token_stream.lookAhead() != '}') {
                        token_stream.rewind(index);
                    }else{
                        token_stream.nextToken();  // skip '}'
                        goto update_pos;
                    }
                }else{
                    token_stream.rewind(index);
                }
            }
        }
    }

    {
        bool isVector = true;

        while (token_stream.lookAhead() == '[') {
            token_stream.nextToken();

            ExpressionAST *expr = nullptr;
            parseCommaExpression(expr);

            ADVANCE(']', "]")

            ast->array_dimensions = snoc(ast->array_dimensions, expr, _M_pool);
            isVector = true;
        }


        bool skipParen = false;
        if (token_stream.lookAhead() == Token_identifier
                && token_stream.lookAhead(1) == '('
                && token_stream.lookAhead(2) == '(') {
            token_stream.nextToken();
            token_stream.nextToken();
            skipParen = true;
        }

        int tok = token_stream.lookAhead();
        if (ast->sub_declarator
                && !(isVector || tok == '(' || tok == ','
                     || tok == ';' || tok == '=')) {
            token_stream.rewind(start);
            return false;
        }

        std::size_t index = token_stream.cursor();
        if (token_stream.lookAhead() == '(') {
            token_stream.nextToken();

            ParameterDeclarationClauseAST *params = nullptr;
            if (!parseParameterDeclarationClause(params)) {
                token_stream.rewind(index);
                goto update_pos;
            }

            ast->parameter_declaration_clause = params;

            if (token_stream.lookAhead() != ')') {
                token_stream.rewind(index);
                goto update_pos;
            }

            token_stream.nextToken();  // skip ')'

            parseCvQualify(ast->fun_cv);
            if(token_stream.lookAhead() == Token_and) {
                ast->fun_reference = token_stream.cursor();
                token_stream.nextToken();
            }
            if(token_stream.lookAhead() == '&') {
                ast->fun_reference = token_stream.cursor();
                token_stream.nextToken();
            }
            parseExceptionSpecification(ast->exception_spec);

            if(!ast->decl_final){
                QDeclFinalAST* decl_final = nullptr;
                parseDeclFinal(decl_final);
                ast->decl_final = decl_final;
            }

            if (token_stream.lookAhead() == Token___attribute__) {
                parse_Attribute__();
            }
        }

        if (skipParen) {
            if (token_stream.lookAhead() != ')') {
                reportError(("')' expected"));
            } else
                token_stream.nextToken();
        }
    }

update_pos:
    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseAbstractDeclarator(DeclaratorAST *&node) {
    std::size_t start = token_stream.cursor();

    DeclaratorAST *ast = CreateNode<DeclaratorAST>(_M_pool);
    DeclaratorAST *decl = nullptr;

    PtrOperatorAST *ptrOp = nullptr;
    while (parsePtrOperator(ptrOp)) {
        ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
    }

    size_t index = token_stream.cursor();
    if (token_stream.lookAhead() == '(') {
        token_stream.nextToken();

        if (!parseAbstractDeclarator(decl)) {
            token_stream.rewind(index);
            goto label1;
        }

        ast->sub_declarator = decl;

        if (token_stream.lookAhead() != ')') {
            token_stream.rewind(start);
            return false;
        }
        token_stream.nextToken();
    } else if (token_stream.lookAhead() == ':') {
        token_stream.nextToken();
        if (!parseConstantExpression(ast->bit_expression)) {
            ast->bit_expression = nullptr;
            reportError(("Constant expression expected"));
        }
        goto update_pos;
    }

label1: {
        bool isVector = true;

        while (token_stream.lookAhead() == '[') {
            token_stream.nextToken();

            ExpressionAST *expr = nullptr;
            parseCommaExpression(expr);

            ADVANCE(']', "]")

            ast->array_dimensions = snoc(ast->array_dimensions, expr, _M_pool);
            isVector = true;
        }

        int tok = token_stream.lookAhead();
        if (ast->sub_declarator
                && !(isVector || tok == '(' || tok == ','
                     || tok == ';' || tok == '=')) {
            token_stream.rewind(start);
            return false;
        }

        size_t index = token_stream.cursor();
        if (token_stream.lookAhead() == '(') {
            token_stream.nextToken();

            ParameterDeclarationClauseAST *params = nullptr;
            if (!parseParameterDeclarationClause(params)) {
                token_stream.rewind(index);
                goto update_pos;
            }

            ast->parameter_declaration_clause = params;

            if (token_stream.lookAhead() != ')') {
                token_stream.rewind(index);
                goto update_pos;
            }

            token_stream.nextToken();  // skip ')'

            parseCvQualify(ast->fun_cv);
            if(token_stream.lookAhead() == Token_and) {
                ast->fun_reference = token_stream.cursor();
                token_stream.nextToken();
            }
            if(token_stream.lookAhead() == '&') {
                ast->fun_reference = token_stream.cursor();
                token_stream.nextToken();
            }
            parseExceptionSpecification(ast->exception_spec);
        }
    }

update_pos:
    if (token_stream.cursor() == start)
        return false;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseEnumSpecifier(TypeSpecifierAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(Token_enum);

    ExpressionAST *annotationExpression(nullptr);
    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
                token_stream.rewind(start);
                return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
                token_stream.nextToken();
                token_stream.nextToken();
        }else{
                token_stream.rewind(start);
                return false;
        }
    }
    bool isDeprecated = false;
    StringLiteralAST *deprecationComment = nullptr;
    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED) {
        token_stream.nextToken();
        isDeprecated = true;
    }
    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED_X) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        isDeprecated = true;
        if(!parseStringLiteral(deprecationComment)){
            token_stream.rewind(start);
            return false;
        }
        ADVANCE(')', ")")
    }

    NameAST *name = nullptr;
    parseName(name);

    TypeSpecifierAST *base_type = nullptr;

    if (token_stream.lookAhead() == ':') {
        token_stream.nextToken();
        if (!parseTypeSpecifier(base_type)){
            reportError(("Enum super type name expected"));
            skipUntil('{');
        }
    }

    if (token_stream.lookAhead() != '{') {
        token_stream.rewind(start);
        return false;
    }
    token_stream.nextToken();

    EnumSpecifierAST *ast = CreateNode<EnumSpecifierAST>(_M_pool);
    ast->name = name;
    ast->isScoped = false;
    ast->base_type = base_type;
    ast->isDeprecated = isDeprecated;
    ast->deprecationComment = deprecationComment;
    ast->annotationExpression = annotationExpression;

    EnumeratorAST *enumerator = nullptr;
    if (parseEnumerator(enumerator)) {
        ast->enumerators = snoc(ast->enumerators, enumerator, _M_pool);

        while (token_stream.lookAhead() == ',') {
            token_stream.nextToken();

            if (!parseEnumerator(enumerator)) {
                //reportError(("Enumerator expected"));
                break;
            }

            ast->enumerators = snoc(ast->enumerators, enumerator, _M_pool);
        }
    }

    ADVANCE_NR('}', "}");

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseEnumClassSpecifier(TypeSpecifierAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(Token_enum);
    if (token_stream.lookAhead() == Token_struct) {
        token_stream.nextToken();
    }else{
        CHECK(Token_class);
    }

    ExpressionAST *annotationExpression(nullptr);
    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }
    bool isDeprecated = false;
    StringLiteralAST *deprecationComment = nullptr;
    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED) {
        token_stream.nextToken();
        isDeprecated = true;
    }
    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED_X) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        isDeprecated = true;
        if(!parseStringLiteral(deprecationComment)){
            token_stream.rewind(start);
            return false;
        }
        ADVANCE(')', ")")
    }

    NameAST *name = nullptr;
    parseName(name);

    TypeSpecifierAST *base_type = nullptr;

    if (token_stream.lookAhead() == ':') {
        token_stream.nextToken();
        if (!parseTypeSpecifier(base_type)){
            reportError(("Enum super type name expected"));
            skipUntil('{');
        }
    }

    if (token_stream.lookAhead() != '{') {
        token_stream.rewind(start);
        return false;
    }
    token_stream.nextToken();

    EnumSpecifierAST *ast = CreateNode<EnumSpecifierAST>(_M_pool);
    ast->name = name;
    ast->isScoped = true;
    ast->base_type = base_type;
    ast->isDeprecated = isDeprecated;
    ast->deprecationComment = deprecationComment;
    ast->annotationExpression = annotationExpression;

    EnumeratorAST *enumerator = nullptr;
    if (parseEnumerator(enumerator)) {
        ast->enumerators = snoc(ast->enumerators, enumerator, _M_pool);

        while (token_stream.lookAhead() == ',') {
            token_stream.nextToken();

            if (!parseEnumerator(enumerator)) {
                //reportError(("Enumerator expected"));
                break;
            }

            ast->enumerators = snoc(ast->enumerators, enumerator, _M_pool);
        }
    }

    ADVANCE_NR('}', "}");

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseTemplateParameterList(const ListNode<TemplateParameterAST*> *&node) {
    TemplateParameterAST *param = nullptr;
    if (!parseTemplateParameter(param))
        return false;

    node = snoc(node, param, _M_pool);

    while (token_stream.lookAhead() == ',') {
        token_stream.nextToken();

        if (!parseTemplateParameter(param)) {
            syntaxError();
            break;
        } else {
            node = snoc(node, param, _M_pool);
        }
    }

    return true;
}

bool Parser::parseTemplateParameter(TemplateParameterAST *&node) {
    std::size_t start = token_stream.cursor();
    TemplateParameterAST *ast = CreateNode<TemplateParameterAST>(_M_pool);

    int tk = token_stream.lookAhead();

    if ((tk == Token_class || tk == Token_typename || tk == Token_template)
            && parseTypeParameter(ast->type_parameter)) {
        // nothing to do
    } else if (!parseParameterDeclaration(ast->parameter_declaration))
        return false;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseTypeParameter(TypeParameterAST *&node) {
    std::size_t start = token_stream.cursor();

    TypeParameterAST *ast = CreateNode<TypeParameterAST>(_M_pool);
    ast->type = start;

    switch (token_stream.lookAhead()) {
        case Token_class:
        case Token_typename: {
            token_stream.nextToken(); // skip class
            if (token_stream.lookAhead() == Token_ellipsis) {
                ast->ellipsis = token_stream.cursor();
                token_stream.nextToken();
            }

            // parse optional name
            if (token_stream.lookAhead() != '='
                    && token_stream.lookAhead() != ','
                    && token_stream.lookAhead() != '>')
                parseName(ast->name, true);

            if (token_stream.lookAhead() == '=') {
                token_stream.nextToken();

                if (!parseTypeId(ast->type_id)) {
                    if(!parseConditionalExpression(ast->type_expression, true)){
                        //syntaxError();
                        token_stream.rewind(start);
                        return false;
                    }
                }else if(m_isShift
                         && token_stream.lookAhead() == '>'){
                    m_isShift = false;
                    //token_stream.rewind(token_stream.cursor()-1);
                }
            } else if (token_stream.lookAhead() != ','
                       && token_stream.lookAhead() != '>') {
                token_stream.rewind(start);
                return false;
            }
        }
        break;

        case Token_template: {
            token_stream.nextToken(); // skip template
            ADVANCE('<', "<")

            if (!parseTemplateParameterList(ast->template_parameters))
                return false;

            ADVANCE('>', ">")

            if (token_stream.lookAhead() == Token_class)
                token_stream.nextToken();

            // parse optional name
            if (parseName(ast->name, true)) {
                if (token_stream.lookAhead() == '=') {
                    token_stream.nextToken();

                    if (!parseTypeId(ast->type_id)) {
                        syntaxError();
                        return false;
                    }
                }
            }

            if (token_stream.lookAhead() == '=') {
                token_stream.nextToken();

                parseName(ast->template_name, true);
            }
        }
        break;

        default:
            return false;

    } // end switch


    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;
    return true;
}

bool Parser::parseStorageClassSpecifier(const ListNode<std::size_t> *&node, bool allowAuto) {
    std::size_t start = token_stream.cursor();

    int tk;
    while (0 != (tk = token_stream.lookAhead())
            && (tk == Token_friend || (allowAuto && tk == Token_auto)
                || tk == Token_register || tk == Token_static
                || tk == Token_extern || tk == Token_mutable)) {
        node = snoc(node, token_stream.cursor(), _M_pool);
        token_stream.nextToken();
    }

    return start != token_stream.cursor();
}

bool Parser::parseDeprecatedSpecifier(const ListNode<std::size_t> *&node, StringLiteralAST *&deprecationComment){
    std::size_t start = token_stream.cursor();

    int tk;
    while (0 != (tk = token_stream.lookAhead())
            && (tk == Token_QTJAMBI_DEPRECATED || tk == Token_QTJAMBI_DEPRECATED_X)) {
        node = snoc(node, token_stream.cursor(), _M_pool);
        token_stream.nextToken();
        if(tk == Token_QTJAMBI_DEPRECATED_X){
            ADVANCE('(', "(")
            if(!parseStringLiteral(deprecationComment)){
                token_stream.rewind(start);
                return false;
            }
            ADVANCE(')', ")")
        }
    }

    return start != token_stream.cursor();
}

bool Parser::parseFunctionSpecifier(const ListNode<std::size_t> *&node) {
    std::size_t start = token_stream.cursor();

    int tk;
    while (0 != (tk = token_stream.lookAhead())
            && (tk == Token_constexpr || tk == Token_inline || tk == Token_virtual
                || tk == Token_explicit || tk == Token_Q_INVOKABLE || tk == Token_Q_SCRIPTABLE)) {
        node = snoc(node, token_stream.cursor(), _M_pool);
        token_stream.nextToken();
    }

    return start != token_stream.cursor();
}

bool Parser::parseTypeId(TypeIdAST *&node) {
    /// @todo implement the AST for typeId
    std::size_t start = token_stream.cursor();

    TypeSpecifierAST *spec = nullptr;
    if (!parseTypeSpecifier(spec)) {
        token_stream.rewind(start);
        return false;
    }

    DeclaratorAST *decl = nullptr;
    parseAbstractDeclarator(decl);

    TypeIdAST *ast = CreateNode<TypeIdAST>(_M_pool);
    ast->type_specifier = spec;
    ast->declarator = decl;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseInitDeclaratorList(const ListNode<InitDeclaratorAST*> *&node, bool noBrackets) {
    InitDeclaratorAST *decl = nullptr;
    if (!parseInitDeclarator(decl, noBrackets))
        return false;

    node = snoc(node, decl, _M_pool);

    while (token_stream.lookAhead() == ',') {
        token_stream.nextToken();

        if (!parseInitDeclarator(decl, noBrackets)) {
            syntaxError();
            break;
        }
        node = snoc(node, decl, _M_pool);
    }

    return true;
}

bool Parser::parseParameterDeclarationClause(ParameterDeclarationClauseAST *&node) {
    std::size_t start = token_stream.cursor();

    ParameterDeclarationClauseAST *ast
    = CreateNode<ParameterDeclarationClauseAST>(_M_pool);

    if (!parseParameterDeclarationList(ast->parameter_declarations)) {
        if (token_stream.lookAhead() == ')')
            goto good;

        if (token_stream.lookAhead() == Token_ellipsis
                && token_stream.lookAhead(1) == ')') {
            ast->ellipsis = token_stream.cursor();
            goto good;
        }

        return false;
    }

good:

    if (token_stream.lookAhead() == Token_ellipsis) {
        ast->ellipsis = token_stream.cursor();
        token_stream.nextToken();
    }

    /// @todo add ellipsis
    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseParameterDeclarationList(const ListNode<ParameterDeclarationAST*> *&node) {
    std::size_t start = token_stream.cursor();

    ParameterDeclarationAST *param = nullptr;
    if (!parseParameterDeclaration(param)) {
        token_stream.rewind(start);
        return false;
    }

    node = snoc(node, param, _M_pool);

    while (token_stream.lookAhead() == ',') {
        token_stream.nextToken();

        if (token_stream.lookAhead() == Token_ellipsis)
            break;

        if (!parseParameterDeclaration(param)) {
            token_stream.rewind(start);
            return false;
        }
        node = snoc(node, param, _M_pool);
    }

    return true;
}

bool Parser::parseParameterDeclaration(ParameterDeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        ExpressionAST *annotationExpression(nullptr);
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
        }else{
            if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
                token_stream.nextToken();
                token_stream.nextToken();
            }else{
                token_stream.rewind(start);
            }
        }
    }

    const ListNode<std::size_t> *storage = nullptr;
    parseStorageClassSpecifier(storage);

    // parse decl spec
    TypeSpecifierAST *spec = nullptr;
    if (!parseTypeSpecifier(spec)) {
        token_stream.rewind(start);
        return false;
    }

    size_t index = token_stream.cursor();

    DeclaratorAST *decl = nullptr;
    if (!parseDeclarator(decl)) {
        token_stream.rewind(index);

        // try with abstract declarator
        parseAbstractDeclarator(decl);
    }

    ExpressionAST *expr = nullptr;
    if (token_stream.lookAhead() == '=') {
        token_stream.nextToken();
        if (!parseConditionalExpression(expr, true)) {
            //reportError(("Expression expected"));
        }
    }

    ParameterDeclarationAST *ast = CreateNode<ParameterDeclarationAST>(_M_pool);
    ast->type_specifier = spec;
    ast->declarator = decl;
    ast->expression = expr;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parse_Attribute__() {
    token_stream.nextToken();

    ADVANCE('(', "(")

    ExpressionAST *expr = nullptr;
    parseExpression(expr);

    if (token_stream.lookAhead() != ')') {
        reportError(("')' expected"));
        return false;
    } else {
        token_stream.nextToken();
    }
    return true;
}

QString Parser::tokenText(AST *ast) const {
    if (!ast) return QString();

    std::size_t start_token = ast->start_token;
    std::size_t end_token = ast->end_token;

    Token const &tk = token_stream.token(start_token);
    Token const &end_tk = token_stream.token(end_token);

    return QString::fromLatin1(&tk.text[tk.position], int(end_tk.position - tk.position)).trimmed();
}

bool Parser::parseForwardDeclarationSpecifier(TypeSpecifierAST *&node) {
    std::size_t start = token_stream.cursor();

    int kind = token_stream.lookAhead();
    if (kind != Token_enum && kind != Token_class && kind != Token_struct && kind != Token_union)
        return false;
    if (kind == Token_enum){
        token_stream.nextToken();
        if(token_stream.lookAhead() != Token_class && token_stream.lookAhead() != Token_struct){
            token_stream.rewind(start);
            return false;
        }
    }

    std::size_t class_key = token_stream.cursor();
    token_stream.nextToken();

    NameAST *name = nullptr;
    if (!parseName(name, false)) {
        token_stream.rewind(start);
        return false;
    }

    BaseClauseAST *bases = nullptr;
    if (token_stream.lookAhead() == ':') {
        if (!parseBaseClause(bases)) {
            token_stream.rewind(start);
            return false;
        }
    }

    if (token_stream.lookAhead() != ';') {
        token_stream.rewind(start);
        return false;
    }

    ForwardDeclarationSpecifierAST *ast = CreateNode<ForwardDeclarationSpecifierAST>(_M_pool);
    ast->isEnum = kind == Token_enum;
    ast->class_key = class_key;
    ast->name = name;
    ast->base_clause = bases;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseClassSpecifier(TypeSpecifierAST *&node) {
    std::size_t start = token_stream.cursor();

    int kind = token_stream.lookAhead();
    if (kind != Token_class && kind != Token_struct && kind != Token_union)
        return false;

    std::size_t class_key = token_stream.cursor();
    token_stream.nextToken();

    ExpressionAST *annotationExpression(nullptr);
    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }

    bool isDeprecated = false;
    StringLiteralAST *deprecationComment = nullptr;

    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED) {
        token_stream.nextToken();
        isDeprecated = true;
    }

    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED_X) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        isDeprecated = true;
        if(!parseStringLiteral(deprecationComment)){
            token_stream.rewind(start);
            return false;
        }
        ADVANCE(')', ")")
    }

    WinDeclSpecAST *winDeclSpec = nullptr;
    parseWinDeclSpec(winDeclSpec);

    if (token_stream.lookAhead() == Token_alignas) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        ExpressionAST *alignmentExpression(nullptr);
        if(!parseExpression(alignmentExpression)){
            TypeSpecifierAST *node(nullptr);
            if(!parseTypeSpecifierOrClassSpec(node)){
                token_stream.rewind(start);
                return false;
            }
            PtrOperatorAST *ptrOp = nullptr;
            while (parsePtrOperator(ptrOp)) {
                //ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
            }
        }
        ADVANCE(')', ")")
    }

    if (token_stream.lookAhead() == Token___attribute__) {
        parse_Attribute__();
    }

    QVector<NameAST *> names;
    while (token_stream.lookAhead() == Token_identifier
            && token_stream.lookAhead(1) == Token_identifier) {
        std::size_t start = token_stream.cursor();
        NameAST *name = nullptr;
        parseName(name, true);
        names << name;
        token_stream.rewind(start);
        token_stream.nextToken();
    }

    NameAST *name = nullptr;
    parseName(name, true);
    bool isFinal = false;
    if(name){
        QString declFinal = name->toString(&token_stream).trimmed();
        if(declFinal=="final" || declFinal=="Q_DECL_FINAL"){
            if(names.isEmpty())
                return false;
            name = names.takeLast();
            isFinal = true;
        }
    }

    BaseClauseAST *bases = nullptr;

    if (token_stream.lookAhead() == ':') {
        if (!parseBaseClause(bases)) {
            skipUntil('{');
        }
    }

    if (token_stream.lookAhead() != '{') {

        token_stream.rewind(start);
        return false;
    }

    ADVANCE('{', "{")

    ClassSpecifierAST *ast = CreateNode<ClassSpecifierAST>(_M_pool);
    ast->win_decl_specifiers = winDeclSpec;
    ast->class_key = class_key;
    ast->name = name;
    ast->base_clause = bases;
    ast->is_final = isFinal;
    ast->is_deprecated = isDeprecated;
    ast->deprecationComment = deprecationComment;
    ast->annotationExpression = annotationExpression;

    while (token_stream.lookAhead()) {
        if (token_stream.lookAhead() == '}')
            break;

        std::size_t startDecl = token_stream.cursor();

        DeclarationAST *memSpec = nullptr;
        if (!parseMemberSpecification(memSpec)) {
            if (startDecl == token_stream.cursor())
                token_stream.nextToken(); // skip at least one token
            skipUntilDeclaration();
        } else
            ast->member_specs = snoc(ast->member_specs, memSpec, _M_pool);
    }

    ADVANCE_NR('}', "}");

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseAccessSpecifier(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    const ListNode<std::size_t> *specs = nullptr;

    bool done = false;
    while (!done) {
        switch (token_stream.lookAhead()) {
            case Token_signals:
            case Token_slots:
            case Token_k_dcop:
            case Token_k_dcop_signals:
            case Token_public:
            case Token_protected:
            case Token_private:
                specs = snoc(specs, token_stream.cursor(), _M_pool);
                token_stream.nextToken();
                break;

            default:
                done = true;
                break;
        }
    }

    if (!specs)
        return false;

    ADVANCE(':', ":")

    AccessSpecifierAST *ast = CreateNode<AccessSpecifierAST>(_M_pool);
    ast->specs = specs;
    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseMemberSpecification(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    if (token_stream.lookAhead() == ';' || token_stream.lookAhead() == Token_K_DCOP) {
        token_stream.nextToken();
        return true;
    } else if (token_stream.lookAhead() == Token_Q_OBJECT) {
        std::size_t start = token_stream.cursor();
        token_stream.nextToken();
        QObjectAST *ast = CreateNode<QObjectAST>(_M_pool);
        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
        return true;
    } else if (token_stream.lookAhead() == Token_Q_GADGET) {
        std::size_t start = token_stream.cursor();
        token_stream.nextToken();
        QGadgetAST *ast = CreateNode<QGadgetAST>(_M_pool);
        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
        return true;
    } else if (parseTypedef(node)) {
        return true;
    } else if (parseUsing(node)) {
        return true;
    } else if (parseTemplateDeclaration(node)) {
        return true;
    } else if (parseAccessSpecifier(node)) {
        return true;
    } else if (parseQ_PROPERTY(node)) {
        return true;
    }

    token_stream.rewind(start);

    const ListNode<std::size_t> *cv = nullptr;
    parseCvQualify(cv);

    const ListNode<std::size_t> *storageSpec = nullptr;
    StringLiteralAST *deprecationComment = nullptr;
    if (token_stream.lookAhead() == Token_alignas) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        ExpressionAST *alignmentExpression(nullptr);
        if(!parseExpression(alignmentExpression)){
            TypeSpecifierAST *node(nullptr);
            if(!parseTypeSpecifierOrClassSpec(node)){
                token_stream.rewind(start);
                return false;
            }
            PtrOperatorAST *ptrOp = nullptr;
            while (parsePtrOperator(ptrOp)) {
                //ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
            }
        }
        ADVANCE(')', ")")
    }
    bool hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);
    parseStorageClassSpecifier(storageSpec);
    if (token_stream.lookAhead() == Token_inline)
        token_stream.nextToken();

    if(!hasDeprecated)
        hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);

    parseCvQualify(cv);

    if(!hasDeprecated)
        hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);

    TypeSpecifierAST *spec = nullptr;
    if (parseEnumSpecifier(spec) || parseEnumClassSpecifier(spec) || parseClassSpecifier(spec)) {
        parseCvQualify(cv);
        spec->cv = cv;

        const ListNode<InitDeclaratorAST*> *declarators = nullptr;
        parseInitDeclaratorList(declarators, false);
        ADVANCE(';', ";")

        SimpleDeclarationAST *ast = CreateNode<SimpleDeclarationAST>(_M_pool);
        ast->type_specifier = spec;
        ast->init_declarators = declarators;
        ast->deprecationComment = deprecationComment;
        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;

        return true;
    }

    token_stream.rewind(start);
    return parseDeclarationInternal(node);
}

bool Parser::parseCtorInitializer(CtorInitializerAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(':');

    CtorInitializerAST *ast = CreateNode<CtorInitializerAST>(_M_pool);
    ast->colon = start;

    if (!parseMemInitializerList(ast->member_initializers)) {
        reportError(("Member initializers expected"));
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseElaboratedTypeSpecifier(TypeSpecifierAST *&node) {
    std::size_t start = token_stream.cursor();

    int tk = token_stream.lookAhead();
    if (tk == Token_class  ||
            tk == Token_struct ||
            tk == Token_union  ||
            tk == Token_enum   ||
            tk == Token_typename) {
        std::size_t type = token_stream.cursor();
        token_stream.nextToken();

        NameAST *name = nullptr;
        if (parseName(name, true)) {
            ElaboratedTypeSpecifierAST *ast
            = CreateNode<ElaboratedTypeSpecifierAST>(_M_pool);

            ast->type = type;
            ast->name = name;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;

            return true;
        }
    }

    token_stream.rewind(start);
    return false;
}

bool Parser::parseExceptionSpecification(ExceptionSpecificationAST *&node) {
    std::size_t start = token_stream.cursor();
    if (token_stream.lookAhead() == Token_noexcept) {
        token_stream.nextToken();
        ExceptionSpecificationAST *ast = CreateNode<ExceptionSpecificationAST>(_M_pool);
        if (token_stream.lookAhead() == '(') {
              token_stream.nextToken();
              parseExpression(ast->expression);
              ADVANCE(')', ")")
          }
        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
        return true;
    }else{
        CHECK(Token_throw);
        ADVANCE('(', "(")

        ExceptionSpecificationAST *ast
        = CreateNode<ExceptionSpecificationAST>(_M_pool);

        if (token_stream.lookAhead() == Token_ellipsis) {
            ast->ellipsis = token_stream.cursor();
            token_stream.nextToken();
        } else {
            parseTypeIdList(ast->type_ids);
        }

        ADVANCE(')', ")")

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;

        return true;
    }
}

bool Parser::parseEnumerator(EnumeratorAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(Token_identifier);
    std::size_t id = token_stream.cursor() - 1;

    EnumeratorAST *ast = CreateNode<EnumeratorAST>(_M_pool);
    ast->id = id;

    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(ast->annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }

    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED) {
        token_stream.nextToken();
        ast->isDeprecated = true;
    }
    if (token_stream.lookAhead() == Token_QTJAMBI_DEPRECATED_X) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        ast->isDeprecated = true;
        StringLiteralAST *comment = nullptr;
        if(!parseStringLiteral(comment)){
            token_stream.rewind(start);
            return false;
        }
        ast->deprecationComment = comment;
        ADVANCE(')', ")")
    }
    if (token_stream.lookAhead() == '=') {
        token_stream.nextToken();

        if (!parseConstantExpression(ast->expression)) {
            reportError(("Constant expression expected"));
        }
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseInitDeclarator(InitDeclaratorAST *&node, bool noBrackets) {
    std::size_t start = token_stream.cursor();

    DeclaratorAST *decl = nullptr;
    if (!parseDeclarator(decl)) {
        return false;
    }

    if (token_stream.lookAhead(0) == Token_asm) {
        token_stream.nextToken();
        skip('(', ')');
        token_stream.nextToken();
    }

    if (!noBrackets && token_stream.lookAhead() == '{') {
        token_stream.nextToken();
        InitDeclaratorAST *ast = CreateNode<InitDeclaratorAST>(_M_pool);
        ast->initializer = CreateNode<InitializerAST>(_M_pool);
        if (token_stream.lookAhead() != '}') {
            parseCommaExpression(ast->initializer->expression);
        }
        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
        CHECK('}');
    }else{
        InitializerAST *init = nullptr;
        parseInitializer(init);

        InitDeclaratorAST *ast = CreateNode<InitDeclaratorAST>(_M_pool);
        ast->declarator = decl;
        ast->initializer = init;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }
    return true;
}

bool Parser::parseBaseClause(BaseClauseAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(':');

    BaseSpecifierAST *baseSpec = nullptr;
    if (!parseBaseSpecifier(baseSpec))
        return false;

    BaseClauseAST *ast = CreateNode<BaseClauseAST>(_M_pool);
    ast->base_specifiers = snoc(ast->base_specifiers, baseSpec, _M_pool);

    while (token_stream.lookAhead() == ',') {
        token_stream.nextToken();

        if (!parseBaseSpecifier(baseSpec)) {
            reportError(("Base class specifier expected"));
            break;
        }
        ast->base_specifiers = snoc(ast->base_specifiers, baseSpec, _M_pool);
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseInitializer(InitializerAST *&node) {
    std::size_t start = token_stream.cursor();

    int tk = token_stream.lookAhead();
    if (tk != '=' && tk != '(')
        return false;

    InitializerAST *ast = CreateNode<InitializerAST>(_M_pool);

    if (tk == '=') {
        token_stream.nextToken();

        if (!parseInitializerClause(ast->initializer_clause)) {
            reportError(("Initializer clause expected"));
        }
    } else if (tk == '(') {
        token_stream.nextToken();
        parseCommaExpression(ast->expression);
        CHECK(')');
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseMemInitializerList(const ListNode<MemInitializerAST*> *&node) {
    MemInitializerAST *init = nullptr;

    if (!parseMemInitializer(init))
        return false;

    node = snoc(node, init, _M_pool);

    while (token_stream.lookAhead() == ',') {
        token_stream.nextToken();

        if (!parseMemInitializer(init))
            break;

        node = snoc(node, init, _M_pool);
    }

    return true;
}

bool Parser::parseMemInitializer(MemInitializerAST *&node) {
    std::size_t start = token_stream.cursor();

    NameAST *initId = nullptr;
    bool isExpression = m_isExpression;
    m_isExpression = false;
    bool p = parseName(initId, true);
    m_isExpression = isExpression;
    if (!p) {
        reportError(("Identifier expected"));
        return false;
    }

    ExpressionAST *expr = nullptr;
    if(token_stream.lookAhead()=='{'){
        ADVANCE('{', "{")
        parseCommaExpression(expr);
        ADVANCE('}', "}")
    }else{
        ADVANCE('(', "(")
        parseCommaExpression(expr);
        ADVANCE(')', ")")
    }

    MemInitializerAST *ast = CreateNode<MemInitializerAST>(_M_pool);
    ast->initializer_id = initId;
    ast->expression = expr;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseTypeIdList(const ListNode<TypeIdAST*> *&node) {
    TypeIdAST *typeId = nullptr;
    if (!parseTypeId(typeId))
        return false;

    node = snoc(node, typeId, _M_pool);

    while (token_stream.lookAhead() == ',') {
        token_stream.nextToken();
        if (parseTypeId(typeId)) {
            node = snoc(node, typeId, _M_pool);
        } else {
            reportError(("Type id expected"));
            break;
        }
    }

    return true;
}

bool Parser::parseBaseSpecifier(BaseSpecifierAST *&node) {
    std::size_t start = token_stream.cursor();

    BaseSpecifierAST *ast = CreateNode<BaseSpecifierAST>(_M_pool);

    if (token_stream.lookAhead() == Token_virtual) {
        ast->virt = token_stream.cursor();
        token_stream.nextToken();

        int tk = token_stream.lookAhead();
        if (tk == Token_public || tk == Token_protected
                || tk == Token_private) {
            ast->access_specifier = token_stream.cursor();
            token_stream.nextToken();
        }
    } else {
        int tk = token_stream.lookAhead();
        if (tk == Token_public || tk == Token_protected
                || tk == Token_private) {
            ast->access_specifier = token_stream.cursor();
            token_stream.nextToken();
        }

        if (token_stream.lookAhead() == Token_virtual) {
            ast->virt = token_stream.cursor();
            token_stream.nextToken();
        }
    }

    bool isExpression = m_isExpression;
    m_isExpression = false;
    bool p = parseName(ast->name, true);
    m_isExpression = isExpression;
    if (!p)
        reportError(("Class name expected"));

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseInitializerClause(InitializerClauseAST *&node) {
    std::size_t start = token_stream.cursor();

    InitializerClauseAST *ast = CreateNode<InitializerClauseAST>(_M_pool);

    if (token_stream.lookAhead() == '{') {
        if (skip('{', '}'))
            token_stream.nextToken();
        else
            reportError(("} missing"));
    } else {
        if(token_stream.lookAhead() == Token_delete){
            ast->expression = CreateNode<DeclDeleteAST>(_M_pool);
            token_stream.nextToken();  // skip 'delete' / 'default'
        }else if(token_stream.lookAhead() == Token_default){
            ast->expression = CreateNode<DeclDefaultAST>(_M_pool);
            token_stream.nextToken();  // skip 'delete' / 'default'
        }else if (!parseAssignmentExpression(ast->expression)) {
            if(token_stream.lookAhead() == Token_delete){
                ast->expression = CreateNode<DeclDeleteAST>(_M_pool);
                token_stream.nextToken();  // skip 'delete' / 'default'
            }else if(token_stream.lookAhead() == Token_default){
                ast->expression = CreateNode<DeclDefaultAST>(_M_pool);
                token_stream.nextToken();  // skip 'delete' / 'default'
            }else{
                //reportError(("Expression expected"));
            }
        }else if (ast->expression && token_stream.lookAhead() == Token_ellipsis) {
            ast->expression->ellipsis_token = token_stream.cursor();
            token_stream.nextToken();
        }
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parsePtrToMember(PtrToMemberAST *&node) {
    std::size_t start = token_stream.cursor();

    std::size_t global_scope = 0;
    if (token_stream.lookAhead() == Token_scope) {
        global_scope = token_stream.cursor();
        token_stream.nextToken();
    }
    Q_UNUSED(global_scope)

    UnqualifiedNameAST *name = nullptr;
    while (token_stream.lookAhead() == Token_identifier) {
        if (!parseUnqualifiedName(name))
            break;

        if (token_stream.lookAhead() == Token_scope
                && token_stream.lookAhead(1) == '*') {
            token_stream.nextToken();
            token_stream.nextToken();

            PtrToMemberAST *ast = CreateNode<PtrToMemberAST>(_M_pool);
            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;

            return true;
        }

        if (token_stream.lookAhead() == Token_scope)
            token_stream.nextToken();
    }

    token_stream.rewind(start);
    return false;
}

bool Parser::parseUnqualifiedName(UnqualifiedNameAST *&node,
                                  bool parseTemplateId) {
    std::size_t start = token_stream.cursor();

    std::size_t tilde = 0;
    std::size_t id = 0;
    OperatorFunctionIdAST *operator_id = nullptr;

    if (token_stream.lookAhead() == Token_identifier) {
        id = token_stream.cursor();
        token_stream.nextToken();
    } else if ((token_stream.lookAhead() == '~' || token_stream.lookAhead() == Token_compl)
               && token_stream.lookAhead(1) == Token_identifier) {
        tilde = token_stream.cursor();
        token_stream.nextToken(); // skip ~

        id = token_stream.cursor();
        token_stream.nextToken(); // skip classname
    } else if (token_stream.lookAhead() == Token_operator) {
        if (!parseOperatorFunctionId(operator_id))
            return false;
    } else {
        return false;
    }

    UnqualifiedNameAST *ast = CreateNode<UnqualifiedNameAST>(_M_pool);
    ast->tilde = tilde;
    ast->id = id;
    ast->operator_id = operator_id;

    if (parseTemplateId && !tilde) {
        std::size_t index = token_stream.cursor();

        if (token_stream.lookAhead() == '<') {
            token_stream.nextToken();

            // optional template arguments
            m_isInTemplate++;
            parseTemplateArgumentList(ast->template_arguments);
            m_isInTemplate--;
            UPDATE_POS(ast, start, token_stream.cursor());
//            qWarning("tmp type: %s", qPrintable(ast->toString(&token_stream)));
//            const Token & tkn = token_stream.token(token_stream.cursor());
//            qWarning("token: %s", qPrintable(QLatin1String(&tkn.text[tkn.position], tkn.size)));
            if(m_isShift){
                token_stream.nextToken();
                m_isShift = false;
            }else if (token_stream.lookAhead() == '>') {
                if(!m_isShift){
                    const Token & tkn = token_stream.token(token_stream.cursor());
                    if(QLatin1String(&tkn.text[tkn.position], int(tkn.size))==">>")
                        m_isShift = true;
                    else{
                        token_stream.nextToken();
                        if (m_isExpression && (token_stream.lookAhead() == Token_identifier
                                                || (token_stream.lookAhead()>='0' && token_stream.lookAhead()<='9')
                                                || token_stream.lookAhead()=='\''
                                                || token_stream.lookAhead()=='"')) {
                            ast->template_arguments = nullptr;
                            token_stream.rewind(index);
                        }
                    }
                }else{
                    token_stream.nextToken();
                }
            } else {
                ast->template_arguments = nullptr;
                token_stream.rewind(index);
            }
        }
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;
//    qWarning("type: %s", qPrintable(ast->toString(&token_stream)));

    return true;
}

bool Parser::parseStringLiteral(StringLiteralAST *&node) {
    std::size_t start = token_stream.cursor();

    if (token_stream.lookAhead() != Token_string_literal && token_stream.lookAhead() != Token_rawstring_literal)
        return false;

    StringLiteralAST *ast = CreateNode<StringLiteralAST>(_M_pool);

    while (token_stream.lookAhead() == Token_string_literal || token_stream.lookAhead() == Token_rawstring_literal) {
        ast->literals = snoc(ast->literals, token_stream.cursor(), _M_pool);
        token_stream.nextToken();
    }
    auto const & tk = token_stream.token(token_stream.cursor());
    if(tk.kind == Token_identifier && tk.text[tk.position]=='_'){
        token_stream.nextToken();
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseExpressionStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    ExpressionAST *expr = nullptr;
    parseCommaExpression(expr);

    ADVANCE(';', ";")

    ExpressionStatementAST *ast = CreateNode<ExpressionStatementAST>(_M_pool);
    ast->expression = expr;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    switch (token_stream.lookAhead()) {
        case Token_while:
            return parseWhileStatement(node);

        case Token_do:
            return parseDoStatement(node);

        case Token_for:
            return parseForStatement(node);

        case Token_if:
            return parseIfStatement(node);

        case Token_switch:
            return parseSwitchStatement(node);

        case Token_try:
            return parseTryBlockStatement(node);

        case Token_case:
        case Token_default:
            return parseLabeledStatement(node);

        case Token_break:
        case Token_continue:
            token_stream.nextToken();
            ADVANCE(';', ";")
            return true;

        case Token_goto:
            token_stream.nextToken();
            ADVANCE(Token_identifier, "identifier")
            ADVANCE(';', ";")
            return true;

        case Token_return: {
            token_stream.nextToken();
            ExpressionAST *expr = nullptr;
            if(token_stream.lookAhead()=='{'){
                token_stream.nextToken();
                if(token_stream.lookAhead() != '}' && !parseCommaExpression(expr)){
                    token_stream.rewind(start);
                    return false;
                }
                ADVANCE('}', "}")
            }else{
                if(token_stream.lookAhead() != ';' && !parseCommaExpression(expr)){
                    token_stream.rewind(start);
                    return false;
                }
            }

            ADVANCE(';', ";")

            ReturnStatementAST *ast = CreateNode<ReturnStatementAST>(_M_pool);
            ast->expression = expr;
            ast->isthrow = false;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        case Token_throw: {
            token_stream.nextToken();
            ExpressionAST *expr = nullptr;
            if(token_stream.lookAhead() != ';' && !parseCommaExpression(expr)){
                token_stream.rewind(start);
                return false;
            }

            ADVANCE(';', ";")

            ReturnStatementAST *ast = CreateNode<ReturnStatementAST>(_M_pool);
            ast->expression = expr;
            ast->isthrow = true;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        case '{':
            return parseCompoundStatement(node);

        case Token_identifier:
            if (parseLabeledStatement(node))
                return true;
            break;
    }

    return parseExpressionOrDeclarationStatement(node);
}

bool Parser::parseExpressionOrDeclarationStatement(StatementAST *&node) {
    bool blocked = block_errors(true);

    std::size_t start = token_stream.cursor();

    StatementAST *decl_ast = nullptr;
    bool maybe_amb = parseDeclarationStatement(decl_ast);
    maybe_amb &= token_stream.kind(token_stream.cursor() - 1) == ';';

    std::size_t end = token_stream.cursor();

    token_stream.rewind(start);
    StatementAST *expr_ast = nullptr;
    maybe_amb &= parseExpressionStatement(expr_ast);
    maybe_amb &= token_stream.kind(token_stream.cursor() - 1) == ';';

    if (maybe_amb) {
        Q_ASSERT(decl_ast && expr_ast);
        ExpressionOrDeclarationStatementAST *ast
        = CreateNode<ExpressionOrDeclarationStatementAST>(_M_pool);
        ast->declaration = decl_ast;
        ast->expression = expr_ast;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    } else {
        token_stream.rewind(std::max(end, token_stream.cursor()));

        node = decl_ast;
        if (!node)
            node = expr_ast;
    }

    block_errors(blocked);

    if (!node)
        syntaxError();

    return node;
}

bool Parser::parseCondition(ConditionAST *&node, bool initRequired) {
    std::size_t start = token_stream.cursor();

    ConditionAST *ast = CreateNode<ConditionAST>(_M_pool);
    TypeSpecifierAST *spec = nullptr;

    if (parseTypeSpecifier(spec)) {
        ast->type_specifier = spec;

        std::size_t declarator_start = token_stream.cursor();

        DeclaratorAST *decl = nullptr;
        if (!parseDeclarator(decl)) {
            token_stream.rewind(declarator_start);
            if (!initRequired && !parseAbstractDeclarator(decl))
                decl = nullptr;
        }

        if (decl && (!initRequired || token_stream.lookAhead() == '=')) {
            ast->declarator = decl;

            if (token_stream.lookAhead() == '=') {
                token_stream.nextToken();

                parseExpression(ast->expression);
            }

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;

            return true;
        }
    }

    token_stream.rewind(start);

    if (!parseCommaExpression(ast->expression))
        return false;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}


bool Parser::parseWhileStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    ADVANCE(Token_while, "while")
    ADVANCE('(' , "(")

    ConditionAST *cond = nullptr;
    if (!parseCondition(cond)) {
        reportError(("condition expected"));
        return false;
    }
    ADVANCE(')', ")")

    StatementAST *body = nullptr;
    if (!parseStatement(body)) {
        reportError(("statement expected"));
        return false;
    }

    WhileStatementAST *ast = CreateNode<WhileStatementAST>(_M_pool);
    ast->condition = cond;
    ast->statement = body;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseDoStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    ADVANCE(Token_do, "do")

    StatementAST *body = nullptr;
    if (!parseStatement(body)) {
        reportError(("statement expected"));
        //return false;
    }

    ADVANCE_NR(Token_while, "while");
    ADVANCE_NR('(' , "(");

    ExpressionAST *expr = nullptr;
    if (!parseCommaExpression(expr)) {
        reportError(("expression expected"));
        //return false;
    }

    ADVANCE_NR(')', ")");
    ADVANCE_NR(';', ";");

    DoStatementAST *ast = CreateNode<DoStatementAST>(_M_pool);
    ast->statement = body;
    ast->expression = expr;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseForStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();
    bool blocked = this->block_errors(true);
    if(!parseIteratorForStatement(node)){
        token_stream.rewind(start);
        this->block_errors(blocked);
        return parseClassicForStatement(node);
    }else{
        this->block_errors(blocked);
    }
    return node;
}

bool Parser::parseIteratorForStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    ADVANCE(Token_for, "for")
    ADVANCE('(', "(")

    const ListNode<std::size_t> *storage = nullptr;
    parseStorageClassSpecifier(storage);

    // parse decl spec
    TypeSpecifierAST *spec = nullptr;
    if (!parseTypeSpecifier(spec)) {
        token_stream.rewind(start);
        return false;
    }

    size_t index = token_stream.cursor();

    DeclaratorAST *decl = nullptr;
    if (!parseDeclarator(decl, true)) {
        token_stream.rewind(index);

        // try with abstract declarator
        parseAbstractDeclarator(decl);
    }

    ExpressionAST *expr = nullptr;
    if (token_stream.lookAhead() == ':') {
        token_stream.nextToken();
        if (!parseConditionalExpression(expr, true)) {
            //reportError(("Expression expected"));
        }
    }else{
        token_stream.rewind(index);
        return false;
    }

    ADVANCE(')', ")")

    StatementAST *body = nullptr;
    if (!parseStatement(body))
        return false;

    IteratorForStatementAST *ast = CreateNode<IteratorForStatementAST>(_M_pool);
    ast->declarator = decl;
    ast->expression = expr;
    ast->statement = body;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseClassicForStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    ADVANCE(Token_for, "for")
    ADVANCE('(', "(")

    StatementAST *init = nullptr;
    if (!parseForInitStatement(init)) {
        reportError(("for initialization expected"));
        return false;
    }

    ConditionAST *cond = nullptr;
    parseCondition(cond);
    ADVANCE(';', ";")

    ExpressionAST *expr = nullptr;
    parseCommaExpression(expr);
    ADVANCE(')', ")")

    StatementAST *body = nullptr;
    if (!parseStatement(body))
        return false;

    ClassicForStatementAST *ast = CreateNode<ClassicForStatementAST>(_M_pool);
    ast->init_statement = init;
    ast->condition = cond;
    ast->expression = expr;
    ast->statement = body;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseForInitStatement(StatementAST *&node) {
    if (parseDeclarationStatement(node))
        return true;

    return parseExpressionStatement(node);
}

bool Parser::parseCompoundStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK('{');

    CompoundStatementAST *ast = CreateNode<CompoundStatementAST>(_M_pool);

    while (token_stream.lookAhead()) {
        if (token_stream.lookAhead() == '}')
            break;

        std::size_t startStmt = token_stream.cursor();

        StatementAST *stmt = nullptr;
        if (!parseStatement(stmt)) {
            if (startStmt == token_stream.cursor())
                token_stream.nextToken();

            skipUntilStatement();
        } else {
            ast->statements = snoc(ast->statements, stmt, _M_pool);
        }
    }

    ADVANCE_NR('}', "}");

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseIfStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    ADVANCE(Token_if, "if")

    if (token_stream.lookAhead() == Token_constexpr)
        token_stream.nextToken();

    ADVANCE('(' , "(")

    IfStatementAST *ast = CreateNode<IfStatementAST>(_M_pool);

    ConditionAST *cond = nullptr;
    StatementAST *decl_ast = nullptr;
    //std::size_t start2 = token_stream.cursor();
    bool ok = false;
    while(true){
        if(!(ok = parseDeclarationStatement(decl_ast))){
            ok = parseCondition(cond);
            if(ok && token_stream.lookAhead() == ';'){
                token_stream.nextToken();
            }
        }
        if(!ok)
            break;
    }
    ADVANCE(')', ")")

    StatementAST *stmt = nullptr;
    if (!parseStatement(stmt)) {
        reportError(("statement expected"));
        return false;
    }

    ast->condition = cond;
    ast->statement = stmt;

    if (token_stream.lookAhead() == Token_else) {
        token_stream.nextToken();

        if (!parseStatement(ast->else_statement)) {
            reportError(("statement expected"));
            return false;
        }
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseSwitchStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();
    ADVANCE(Token_switch, "switch")

    ADVANCE('(' , "(")

    ConditionAST *cond = nullptr;
    if (!parseCondition(cond)) {
        reportError(("condition expected"));
        return false;
    }
    ADVANCE(')', ")")

    StatementAST *stmt = nullptr;
    if (!parseCompoundStatement(stmt)) {
        syntaxError();
        return false;
    }

    SwitchStatementAST *ast = CreateNode<SwitchStatementAST>(_M_pool);
    ast->condition = cond;
    ast->statement = stmt;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseLabeledStatement(StatementAST *&node) {
    switch (token_stream.lookAhead()) {
        case Token_identifier:
        case Token_default:
            if (token_stream.lookAhead(1) == ':') {
                token_stream.nextToken();
                token_stream.nextToken();

                StatementAST *stmt = nullptr;
                if (parseStatement(stmt)) {
                    node = stmt;
                    return true;
                }
            }
            break;

        case Token_case: {
            token_stream.nextToken();
            ExpressionAST *expr = nullptr;
            if (!parseConstantExpression(expr)) {
                reportError(("expression expected"));
            } else if (token_stream.lookAhead() == Token_ellipsis) {
                token_stream.nextToken();

                ExpressionAST *expr2 = nullptr;
                if (!parseConstantExpression(expr2)) {
                    reportError(("expression expected"));
                }
            }
            ADVANCE(':', ":")

            StatementAST *stmt = nullptr;
            if (parseStatement(stmt)) {
                node = stmt;
                return true;
            }
        }
        break;

    }

    return false;
}

bool Parser::parseBlockDeclaration(DeclarationAST *&node) {
    switch (token_stream.lookAhead()) {
        case Token_typedef:
            return parseTypedef(node);
        case Token_using:
            return parseUsing(node);
        case Token_asm:
            return parseAsmDefinition(node);
        case Token_namespace:
            return parseNamespaceAliasDefinition(node);
        case Token_QTJAMBI_DEPRECATED:
            switch (token_stream.lookAhead(1)){
            case Token_typedef:
                return parseTypedef(node);
            default: break;
            }
        Q_FALLTHROUGH();
        case Token_QTJAMBI_DEPRECATED_X:{
            std::size_t start = token_stream.cursor();
            token_stream.nextToken();
            ADVANCE('(', "(")
            StringLiteralAST *comment = nullptr;
            if(!parseStringLiteral(comment)){
                token_stream.rewind(start);
                return false;
            }
            ADVANCE(')', ")")
            switch (token_stream.lookAhead()){
            case Token_typedef:
                token_stream.rewind(start);
                return parseTypedef(node);
            }
            token_stream.rewind(start);
            break;
        }
        default: break;
    }

    std::size_t start = token_stream.cursor();

    const ListNode<std::size_t> *cv = nullptr;
    parseCvQualify(cv);

    const ListNode<std::size_t> *storageSpec = nullptr;
    parseStorageClassSpecifier(storageSpec);

    parseCvQualify(cv);

    TypeSpecifierAST *spec = nullptr;
    if (!parseTypeSpecifierOrClassSpec(spec)) { // replace with simpleTypeSpecifier?!?!
        token_stream.rewind(start);
        return false;
    }

    parseCvQualify(cv);
    spec->cv = cv;

    const ListNode<InitDeclaratorAST*> *declarators = nullptr;
    parseInitDeclaratorList(declarators, false);

    if (token_stream.lookAhead() != ';') {
        token_stream.rewind(start);
        return false;
    }
    token_stream.nextToken();

    SimpleDeclarationAST *ast = CreateNode<SimpleDeclarationAST>(_M_pool);
    ast->type_specifier = spec;
    ast->init_declarators = declarators;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseNamespaceAliasDefinition(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    CHECK(Token_namespace);

    NamespaceAliasDefinitionAST *ast
    = CreateNode<NamespaceAliasDefinitionAST>(_M_pool);

    ADVANCE(Token_identifier,  "identifier")
    ast->namespace_name = token_stream.cursor() - 1;

    ADVANCE('=', "=")

    bool isExpression = m_isExpression;
    m_isExpression = false;
    bool p = parseName(ast->alias_name);
    m_isExpression = isExpression;
    if (!p) {
        reportError(("Namespace name expected"));
    }

    ADVANCE(';', ";")

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseDeclarationStatement(StatementAST *&node) {
    std::size_t start = token_stream.cursor();

    DeclarationAST *decl = nullptr;
    if (!parseBlockDeclaration(decl))
        return false;

    DeclarationStatementAST *ast = CreateNode<DeclarationStatementAST>(_M_pool);
    ast->declaration = decl;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseDeclarationInternal(DeclarationAST *&node) {
    std::size_t start = token_stream.cursor();

    if (token_stream.lookAhead() == Token_alignas) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        ExpressionAST *alignmentExpression(nullptr);
        if(!parseExpression(alignmentExpression)){
            TypeSpecifierAST *node(nullptr);
            if(!parseTypeSpecifierOrClassSpec(node)){
                token_stream.rewind(start);
                return false;
            }
            PtrOperatorAST *ptrOp = nullptr;
            while (parsePtrOperator(ptrOp)) {
                //ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
            }
        }
        ADVANCE(')', ")")
    }

    // that is for the case '__declspec(dllexport) int ...' or
    // '__declspec(dllexport) inline int ...', etc.
    WinDeclSpecAST *winDeclSpec = nullptr;
    parseWinDeclSpec(winDeclSpec);

    if (token_stream.lookAhead() == Token_alignas) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        ExpressionAST *alignmentExpression(nullptr);
        if(!parseExpression(alignmentExpression)){
            TypeSpecifierAST *node(nullptr);
            if(!parseTypeSpecifierOrClassSpec(node)){
                token_stream.rewind(start);
                return false;
            }
            PtrOperatorAST *ptrOp = nullptr;
            while (parsePtrOperator(ptrOp)) {
                //ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
            }
        }
        ADVANCE(')', ")")
    }

    ExpressionAST *annotationExpression(nullptr);
    if (token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }

    const ListNode<std::size_t> *funSpec = nullptr;
    const ListNode<std::size_t> *storageSpec = nullptr;
    StringLiteralAST *deprecationComment = nullptr;
    bool hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);
    bool hasFunSpec = parseFunctionSpecifier(funSpec);

    const ListNode<std::size_t> *cv = nullptr;
    parseCvQualify(cv);

    if(!hasDeprecated)
        hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);

    if (!annotationExpression && token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }

    parseStorageClassSpecifier(storageSpec);

    parseCvQualify(cv);

    if(!hasDeprecated)
        hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);

    if (!annotationExpression && token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }

    hasFunSpec |= parseFunctionSpecifier(funSpec);

    if(!hasDeprecated)
        hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);

    if (!annotationExpression && token_stream.lookAhead() == '[' && token_stream.lookAhead(1) == '[') {
        token_stream.nextToken();
        token_stream.nextToken();
        if(!parseExpression(annotationExpression)){
            token_stream.rewind(start);
            return false;
        }
        if (token_stream.lookAhead() == ']' && token_stream.lookAhead(1) == ']') {
            token_stream.nextToken();
            token_stream.nextToken();
        }else{
            token_stream.rewind(start);
            return false;
        }
    }

    // that is for the case 'friend __declspec(dllexport) ....'
    parseWinDeclSpec(winDeclSpec);

    parseCvQualify(cv);

    size_t index = token_stream.cursor();
    NameAST *name = nullptr;
    bool isExpression = m_isExpression;
    m_isExpression = false;
    bool p = parseName(name, true);
    m_isExpression = isExpression;
    if (p && token_stream.lookAhead() == '(') {
        // no type specifier, maybe a constructor or a cast operator??

        token_stream.rewind(index);

        InitDeclaratorAST *declarator = nullptr;
        if (parseInitDeclarator(declarator, true)) {
            UnqualifiedNameAST *arrowDecl = nullptr;
            if(m_isTemplateDecl && declarator && !declarator->initializer && token_stream.lookAhead()==Token_arrow){
                token_stream.nextToken();
                bool isExpression = m_isExpression;
                m_isExpression = false;
                parseUnqualifiedName(arrowDecl, true);
                m_isExpression = isExpression;
            }
            switch (token_stream.lookAhead()) {
                case ';': {
                    token_stream.nextToken();

                    SimpleDeclarationAST *ast
                    = CreateNode<SimpleDeclarationAST>(_M_pool);
                    ast->arrowDecl = arrowDecl;
                    ast->storage_specifiers = storageSpec;
                    ast->deprecationComment = deprecationComment;
                    ast->function_specifiers = funSpec;
                    ast->init_declarators = snoc(ast->init_declarators,
                                                 declarator, _M_pool);

                    UPDATE_POS(ast, start, token_stream.cursor());
                    node = ast;
                }
                return true;

                case ':': {
                    CtorInitializerAST *ctorInit = nullptr;
                    StatementAST *funBody = nullptr;

                    if (parseCtorInitializer(ctorInit)
                            && parseFunctionBody(funBody)) {
                        FunctionDefinitionAST *ast
                        = CreateNode<FunctionDefinitionAST>(_M_pool);

                        ast->storage_specifiers = storageSpec;
                        ast->function_specifiers = funSpec;
                        ast->deprecationComment = deprecationComment;
                        ast->annotationExpression = annotationExpression;
                        ast->init_declarator = declarator;
                        ast->function_body = funBody;

                        UPDATE_POS(ast, start, token_stream.cursor());
                        node = ast;

                        return true;
                    }
                }
                break;

                case '{': {
                    StatementAST *funBody = nullptr;
                    if (parseFunctionBody(funBody)) {
                        FunctionDefinitionAST *ast
                        = CreateNode<FunctionDefinitionAST>(_M_pool);

                        ast->storage_specifiers = storageSpec;
                        ast->deprecationComment = deprecationComment;
                        ast->annotationExpression = annotationExpression;
                        ast->function_specifiers = funSpec;
                        ast->init_declarator = declarator;
                        ast->function_body = funBody;

                        UPDATE_POS(ast, start, token_stream.cursor());
                        node = ast;

                        return true;
                    }
                }
                break;

                case '(':
                case '[':
                    // ops!! it seems a declarator
                    goto start_decl;
            }

        }
    }

start_decl:
    token_stream.rewind(index);

    if (token_stream.lookAhead() == Token_const
            && token_stream.lookAhead(1) == Token_identifier
            && token_stream.lookAhead(2) == '=') {
        // constant definition
        token_stream.nextToken(); // skip const

        const ListNode<InitDeclaratorAST*> *declarators = nullptr;
        if (!parseInitDeclaratorList(declarators, false)) {
            syntaxError();
            return false;
        }

        ADVANCE(';', ";")

        SimpleDeclarationAST *ast = CreateNode<SimpleDeclarationAST>(_M_pool);
        ast->init_declarators = declarators;
        ast->deprecationComment = deprecationComment;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;

        return true;
    }

    TypeSpecifierAST *spec = nullptr;
    if (parseTypeSpecifier(spec)) {
        Q_ASSERT(spec);

        if(!hasDeprecated)
            hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);
        if (!hasFunSpec)
            parseFunctionSpecifier(funSpec);         // e.g. "void inline"
        if (token_stream.lookAhead() == Token_static) // e.g. "enable_if_t static"
            token_stream.nextToken();
        if(!hasDeprecated)
            hasDeprecated = parseDeprecatedSpecifier(storageSpec, deprecationComment);
        if(!spec->cv){
            spec->cv = cv;
        }else if(cv){ // this change was necessary to parse types like 'T * const * const' correctly.
            spec->cv->next = cv;
        }

        const ListNode<InitDeclaratorAST*> *declarators = nullptr;
        InitDeclaratorAST *decl = nullptr;
        size_t startDeclarator = token_stream.cursor();
        bool maybeFunctionDefinition = false;

        if (token_stream.lookAhead() != ';') {
            if (parseInitDeclarator(decl, true) && (token_stream.lookAhead() == '{' || token_stream.lookAhead()==Token_arrow)) {
                // function definition
                maybeFunctionDefinition = true;
            } else {
                token_stream.rewind(startDeclarator);
                if (!parseInitDeclaratorList(declarators, false)) {
                    syntaxError();
                    return false;
                }
            }
        }

        if(token_stream.lookAhead()==Token_arrow){
            token_stream.nextToken();
            TypeSpecifierAST * typeSpecifier = nullptr;
            //ExpressionAST *typeExpression = nullptr;
            if(token_stream.lookAhead()==Token_typename){
                token_stream.nextToken();
            }
            const ListNode<std::size_t> *storage = nullptr;
            parseStorageClassSpecifier(storage);
            if(!parseTypeSpecifierOrClassSpec(typeSpecifier)){
                token_stream.rewind(start);
                return false;
            }
            PtrOperatorAST *ptrOp = nullptr;
            while (parsePtrOperator(ptrOp)) {
                //ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
            }
//            if(!parseExpression(typeExpression)
//                    && !parseTypeSpecifier(typeSpecifier)){
//                syntaxError();
//                return false;
//            }
//            spec->auto_type_expression = typeExpression;
//            spec->auto_type_specifier = typeSpecifier;
            maybeFunctionDefinition = true;
        }

        switch (token_stream.lookAhead()) {
            case ';': {
                token_stream.nextToken();
                SimpleDeclarationAST *ast
                = CreateNode<SimpleDeclarationAST>(_M_pool);

                ast->storage_specifiers = storageSpec;
                ast->deprecationComment = deprecationComment;
                ast->function_specifiers = funSpec;
                ast->type_specifier = spec;
                ast->win_decl_specifiers = winDeclSpec;
                ast->init_declarators = declarators;

                UPDATE_POS(ast, start, token_stream.cursor());
                node = ast;
            }
            return true;

            case '{': {
                if (!maybeFunctionDefinition) {
                    syntaxError();
                    return false;
                }

                StatementAST *funBody = nullptr;
                if (parseFunctionBody(funBody)) {
                    FunctionDefinitionAST *ast
                    = CreateNode<FunctionDefinitionAST>(_M_pool);

                    ast->win_decl_specifiers = winDeclSpec;
                    ast->storage_specifiers = storageSpec;
                    ast->deprecationComment = deprecationComment;
                    ast->annotationExpression = annotationExpression;
                    ast->function_specifiers = funSpec;
                    ast->type_specifier = spec;
                    ast->init_declarator = decl;
                    ast->function_body = funBody;

                    UPDATE_POS(ast, start, token_stream.cursor());
                    node = ast;

                    return true;
                }
            }
            break;
        } // end switch
    }

    syntaxError();
    return false;
}

bool Parser::skipFunctionBody(StatementAST *&) {
    Q_ASSERT_X(0, "Parser::skipFunctionBody()", "implement me");
    return 0;
}

bool Parser::parseFunctionBody(StatementAST *&node) {
    if (control->skipFunctionBody())
        return skipFunctionBody(node);

    return parseCompoundStatement(node);
}

bool Parser::parseTypeSpecifierOrClassSpec(TypeSpecifierAST *&node) {
    if (parseClassSpecifier(node))
        return true;
    else if (parseEnumSpecifier(node))
        return true;
    else if (parseEnumClassSpecifier(node))
        return true;
    else if (parseTypeSpecifier(node))
        return true;

    return false;
}

bool Parser::parseTryBlockStatement(StatementAST *&node) {
    CHECK(Token_try);

    StatementAST *stmt = nullptr;
    if (!parseCompoundStatement(stmt)) {
        syntaxError();
        return false;
    }

    if (token_stream.lookAhead() != Token_catch) {
        reportError(("catch expected"));
        return false;
    }

    while (token_stream.lookAhead() == Token_catch) {
        token_stream.nextToken();
        ADVANCE('(', "(")
        ConditionAST *cond = nullptr;
        if (token_stream.lookAhead() == Token_ellipsis) {
            token_stream.nextToken();
        } else if (!parseCondition(cond, false)) {
            reportError(("condition expected"));
            return false;
        }
        ADVANCE(')', ")")

        StatementAST *body = nullptr;
        if (!parseCompoundStatement(body)) {
            syntaxError();
            return false;
        }
    }

    node = stmt;
    return true;
}

bool Parser::parseInitializerListExpression(ExpressionAST *&) {
    if(token_stream.lookAhead()=='{'){
        std::size_t start = token_stream.cursor();
        token_stream.nextToken();
        if(token_stream.lookAhead()!='}'){
            ExpressionAST *commaNode = nullptr;
            if(!parseCommaExpression(commaNode)){
                token_stream.rewind(start);
                return false;
            }
        }
        if(token_stream.lookAhead()!='}'){
            token_stream.rewind(start);
            return false;
        }
        token_stream.nextToken();
        return true;
    }
    return false;
}

bool Parser::parseLambdaExpression(ExpressionAST *&) {
    if(token_stream.lookAhead()=='['){
        std::size_t start = token_stream.cursor();
        token_stream.nextToken();
        if((token_stream.lookAhead()=='&' || token_stream.lookAhead()=='=') && token_stream.lookAhead(1)==']'){
            token_stream.nextToken();
        }else if(token_stream.lookAhead()!=']'){
            ExpressionAST *expressions = nullptr;
            if(!parseCommaExpression(expressions)){
                token_stream.rewind(start);
                return false;
            }
        }
        if(token_stream.lookAhead()!=']'){
            token_stream.rewind(start);
            return false;
        }
        token_stream.nextToken();
        if(token_stream.lookAhead()=='('){ // parameters are optional
            token_stream.nextToken();
            if(token_stream.lookAhead()!=')'){
                ParameterDeclarationClauseAST *params = nullptr;
                if (!parseParameterDeclarationClause(params)) {
                    token_stream.rewind(start);
                    return false;
                }
                if(token_stream.lookAhead()!=')'){
                    token_stream.rewind(start);
                    return false;
                }
            }
            token_stream.nextToken();
        }
        if(token_stream.lookAhead()==Token_mutable){
            token_stream.nextToken();
        }
        if (token_stream.lookAhead() == Token_noexcept){
            token_stream.nextToken();
            if (token_stream.lookAhead() == '('){
                std::size_t start = token_stream.cursor();
                CHECK('(');
                ExpressionAST *expr = nullptr;
                if (parseExpression(expr)){
                    CHECK(')');
                }else{
                    token_stream.rewind(start);
                }
            }
        }
        if(token_stream.lookAhead()==Token_mutable){
            token_stream.nextToken();
        }
        if(token_stream.lookAhead()==Token_arrow){
            token_stream.nextToken();
            if(token_stream.lookAhead()==Token_typename){
                token_stream.nextToken();
            }
            TypeSpecifierAST *node = nullptr;
            const ListNode<std::size_t> *storage = nullptr;
            parseStorageClassSpecifier(storage);
            if(!parseTypeSpecifierOrClassSpec(node)){
                token_stream.rewind(start);
                return false;
            }
            PtrOperatorAST *ptrOp = nullptr;
            while (parsePtrOperator(ptrOp)) {
                //ast->ptr_ops = snoc(ast->ptr_ops, ptrOp, _M_pool);
            }
        }
        if(token_stream.lookAhead()==Token_mutable){
            token_stream.nextToken();
        }
        if(token_stream.lookAhead()!='{'){
            token_stream.rewind(start);
            return false;
        }
        StatementAST * statement = nullptr;
        if(!parseFunctionBody(statement)){
            token_stream.rewind(start);
            return false;
        }
        return true;
    }
    return false;
}

bool Parser::parsePrimaryExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    PrimaryExpressionAST *ast = CreateNode<PrimaryExpressionAST>(_M_pool);

    switch (token_stream.lookAhead()) {
        case Token_string_literal:
        case Token_rawstring_literal:
            parseStringLiteral(ast->literal);
            break;

//        case Token_nullptr:
        case Token_number_literal:
        case Token_char_literal:
        case Token_true:
        case Token_false:
        case Token_this:
        case Token_ellipsis:
            ast->token = token_stream.cursor();
            token_stream.nextToken();
            break;

        case '(':
            token_stream.nextToken();

            if (token_stream.lookAhead() == '{') {
                std::size_t pos = token_stream.cursor();
                if (!parseInitializerListExpression(ast->sub_expression)){
                    token_stream.rewind(pos);
                    if (!parseCompoundStatement(ast->expression_statement))
                        return false;
                }
            } else {
                if (!parseExpression(ast->sub_expression))
                    return false;
            }

            CHECK(')');
            break;

        default:{
            bool isExpression = m_isExpression;
            m_isExpression = true;
            bool p = parseName(ast->name);
            m_isExpression = isExpression;
            if (!p)
                return false;
            }
            break;
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}


/*
  postfix-expression-internal:
  [ expression ]
  ( expression-list [opt] )
  (.|->) template [opt] id-expression
  (.|->) pseudo-destructor-name
  ++
  --
*/
bool Parser::parsePostfixExpressionInternal(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    switch (token_stream.lookAhead()) {
        case '[': {
            token_stream.nextToken();
            ExpressionAST *expr = nullptr;
            parseExpression(expr);
            CHECK(']');

            SubscriptExpressionAST *ast
            = CreateNode<SubscriptExpressionAST>(_M_pool);

            ast->subscript = expr;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        case '(': {
            token_stream.nextToken();
            ExpressionAST *expr = nullptr;
            parseExpression(expr);
            CHECK(')');

            FunctionCallAST *ast = CreateNode<FunctionCallAST>(_M_pool);
            ast->arguments = expr;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        case '.':
        case Token_arrow: {
            std::size_t op = token_stream.cursor();
            token_stream.nextToken();

            std::size_t templ = 0;
            std::size_t tilde = 0;
            if (token_stream.lookAhead() == Token_template) {
                templ = token_stream.cursor();
                token_stream.nextToken();
            }else if (token_stream.lookAhead() == '~' || token_stream.lookAhead() == Token_compl) {
                tilde = token_stream.cursor();
                token_stream.nextToken();
            }
            Q_UNUSED(tilde)

            size_t saved = token_stream.cursor();
            NameAST *name = nullptr;

            bool isExpression = m_isExpression;
            m_isExpression = true;
            bool p = parseName(name, true);
            m_isExpression = isExpression;
            if (p && name->unqualified_name
                    && name->unqualified_name->template_arguments
                    && token_stream.lookAhead() == '(') {
                // a template method call
                // ### reverse the logic
            } else {
                token_stream.rewind(saved);
                name = nullptr;

                isExpression = m_isExpression;
                m_isExpression = false;
                p = parseName(name, templ != 0);
                m_isExpression = isExpression;
                if (!p)
                    return false;
            }

            ClassMemberAccessAST *ast = CreateNode<ClassMemberAccessAST>(_M_pool);
            ast->op = op;
            ast->name = name;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        case Token_incr:
        case Token_decr: {
            std::size_t op = token_stream.cursor();
            token_stream.nextToken();

            IncrDecrExpressionAST *ast = CreateNode<IncrDecrExpressionAST>(_M_pool);
            ast->op = op;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        default:
            return false;
    }
}

/*
  postfix-expression:
  simple-type-specifier ( expression-list [opt] )
  primary-expression postfix-expression-internal*
*/
bool Parser::parsePostfixExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    switch (token_stream.lookAhead()) {
        case Token_dynamic_cast:
        case Token_static_cast:
        case Token_reinterpret_cast:
        case Token_const_cast: {
            std::size_t castOp = token_stream.cursor();
            token_stream.nextToken();

            CHECK('<');
            TypeIdAST *typeId = nullptr;
            parseTypeId(typeId);
            CHECK('>');

            CHECK('(');
            ExpressionAST *expr = nullptr;
            parseCommaExpression(expr);
            CHECK(')');

            CppCastExpressionAST *ast = CreateNode<CppCastExpressionAST>(_M_pool);
            ast->op = castOp;
            ast->type_id = typeId;
            ast->expression = expr;

            ExpressionAST *e = nullptr;
            while (parsePostfixExpressionInternal(e)) {
                ast->sub_expressions = snoc(ast->sub_expressions, e, _M_pool);
            }

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        case Token_typename: {
            std::size_t token = token_stream.cursor();
            token_stream.nextToken();

            NameAST* name = nullptr;
            bool isExpression = m_isExpression;
            m_isExpression = false;
            bool p = parseName(name, true);
            m_isExpression = isExpression;
            if (!p)
                return false;

            ExpressionAST *expr = nullptr;
            if(token_stream.lookAhead() == '{'){
                CHECK('{');
                parseCommaExpression(expr);
                CHECK('}');
            }else{
                CHECK('(');
                parseCommaExpression(expr);
                CHECK(')');
            }
            TypeIdentificationAST *ast = CreateNode<TypeIdentificationAST>(_M_pool);
            ast->typename_token = token;
            ast->name = name;
            ast->expression = expr;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        /*case Token_typeid: {
            token_stream.nextToken();

            CHECK('(');
            TypeIdAST *typeId = nullptr;
            parseTypeId(typeId);
            CHECK(')');

            TypeIdentificationAST *ast = CreateNode<TypeIdentificationAST>(_M_pool);
            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;*/

        default:
            break;
    }

    std::size_t saved_pos = token_stream.cursor();

    TypeSpecifierAST *typeSpec = nullptr;
    ExpressionAST *expr = nullptr;

    // let's try to parse a type
    NameAST *name = nullptr;
    bool isExpression = m_isExpression;
    m_isExpression = true;
    bool p = parseName(name, true);
    m_isExpression = isExpression;

    if (p) {
        Q_ASSERT(name->unqualified_name);

        bool has_template_args = name->unqualified_name->template_arguments != nullptr;

        if (has_template_args) {
            if(token_stream.lookAhead() == '('){
                ExpressionAST *cast_expr = nullptr;
                if (parseCastExpression(cast_expr)
                        && cast_expr->kind == AST::Kind_CastExpression) {
                    token_stream.rewind(saved_pos);
                    parsePrimaryExpression(expr);
                    goto L_no_rewind;
                }
            }else if(token_stream.lookAhead() == '{'){
                // continue
            }else if(token_stream.lookAhead() == ')'
                     || token_stream.lookAhead() == '}'
                     || token_stream.lookAhead() == ']'
                     || token_stream.lookAhead() == ';'
                     || token_stream.lookAhead() == ','
                     || token_stream.lookAhead() == '('
                     || token_stream.lookAhead() == '{'
                     || token_stream.lookAhead() == '['
                     || token_stream.lookAhead() == '.'
                     || token_stream.lookAhead() == '+'
                     || token_stream.lookAhead() == '-'
                     || token_stream.lookAhead() == '*'
                     || token_stream.lookAhead() == '/'
                     || token_stream.lookAhead() == '%'
                     || token_stream.lookAhead() == '^'
                     || token_stream.lookAhead() == '|'
                     || token_stream.lookAhead() == '&'
                     || token_stream.lookAhead() == '>'
                     || token_stream.lookAhead() == Token_or
                     || token_stream.lookAhead() == Token_and
                     || token_stream.lookAhead() == Token_left_shift
                     || token_stream.lookAhead() == Token_leq
                     || token_stream.lookAhead() == Token_compare
                     || token_stream.lookAhead() == Token_not
                     || token_stream.lookAhead() == Token_eq
                     || token_stream.lookAhead() == Token_geq
                     || token_stream.lookAhead() == Token_xor
                     || token_stream.lookAhead() == Token_incr
                     || token_stream.lookAhead() == Token_decr
                     || token_stream.lookAhead() == Token_bitor
                     || token_stream.lookAhead() == Token_and_eq
                     || token_stream.lookAhead() == Token_xor_eq
                     || token_stream.lookAhead() == Token_or_eq
                     || token_stream.lookAhead() == Token_bitand){
                // expression shall be a template function pointer
                PostfixExpressionAST *ast = CreateNode<PostfixExpressionAST>(_M_pool);
                ast->type_specifier = typeSpec;
                ast->expression = expr;
                UPDATE_POS(ast, start, token_stream.cursor());
                node = ast;
                return true;
            }
        }
    }

    token_stream.rewind(saved_pos);

L_no_rewind:
    if (!expr && parseSimpleTypeSpecifier(typeSpec)
            && (token_stream.lookAhead() == '(' || token_stream.lookAhead() == '{')) {
        bool paren = token_stream.lookAhead() == '(';
        token_stream.nextToken(); // skip '('
        if(token_stream.lookAhead() != (paren ? ')' : '}') && !parseCommaExpression(expr)){
            token_stream.rewind(start);
            return false;
        }
        if(paren){
            CHECK(')');
        }else{
            CHECK('}');
        }
    } else if (expr) {
        typeSpec = nullptr;
    } else {
        typeSpec = nullptr;
        token_stream.rewind(start);

        if (!parsePrimaryExpression(expr)){
            token_stream.rewind(start);
            if (!parseLambdaExpression(expr)){
                token_stream.rewind(start);
                if (!parseInitializerListExpression(expr)){
                    return false;
                }
            }
        }
    }

    const ListNode<ExpressionAST*> *sub_expressions = nullptr;
    ExpressionAST *sub_expression = nullptr;

    while (parsePostfixExpressionInternal(sub_expression))
        sub_expressions = snoc(sub_expressions, sub_expression, _M_pool);

    if (sub_expressions || ! expr || (typeSpec && expr)) {
        PostfixExpressionAST *ast = CreateNode<PostfixExpressionAST>(_M_pool);
        ast->type_specifier = typeSpec;
        ast->expression = expr;
        ast->sub_expressions = sub_expressions;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    } else
        node = expr;

    return true;
}

bool Parser::parseUnaryExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    switch (token_stream.lookAhead()) {
        case Token_incr:
        case Token_decr:
        case Token_compl:
        case '*':
        case '&':
        case '+':
        case '-':
        case '!':
        case '~': {
            std::size_t op = token_stream.cursor();
            token_stream.nextToken();

            ExpressionAST *expr = nullptr;
            if (!parseCastExpression(expr))
                return false;

            UnaryExpressionAST *ast = CreateNode<UnaryExpressionAST>(_M_pool);
            ast->op = op;
            ast->expression = expr;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
        return true;

        case Token_sizeof: {
            std::size_t sizeof_token = token_stream.cursor();
            token_stream.nextToken();

            SizeofExpressionAST *ast = CreateNode<SizeofExpressionAST>(_M_pool);
            ast->ellipsis_token = 0;
            ast->sizeof_token = sizeof_token;
            std::size_t index = token_stream.cursor();
            if (token_stream.lookAhead() == '(') {
                token_stream.nextToken();
                if (parseTypeId(ast->type_id) && token_stream.lookAhead() == ')') {
                    token_stream.nextToken(); // skip )

                    UPDATE_POS(ast, start, token_stream.cursor());
                    node = ast;
                    return true;
                }

                ast->type_id = nullptr;
                token_stream.rewind(index);
            }

            if (!parseUnaryExpression(ast->expression))
                return false;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
            return true;
        }

        case Token_alignof: {
            std::size_t sizeof_token = token_stream.cursor();
            token_stream.nextToken();

            AlignofExpressionAST *ast = CreateNode<AlignofExpressionAST>(_M_pool);
            ast->ellipsis_token = 0;
            ast->alignof_token = sizeof_token;
            std::size_t index = token_stream.cursor();
            if (token_stream.lookAhead() == '(') {
                token_stream.nextToken();
                if (parseTypeId(ast->type_id) && token_stream.lookAhead() == ')') {
                    token_stream.nextToken(); // skip )

                    UPDATE_POS(ast, start, token_stream.cursor());
                    node = ast;
                    return true;
                }

                ast->type_id = nullptr;
                token_stream.rewind(index);
            }

            if (!parseUnaryExpression(ast->expression))
                return false;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
            return true;
        }

        case Token_typeid: {
            std::size_t typeid_token = token_stream.cursor();
            token_stream.nextToken();

            TypeidExpressionAST *ast = CreateNode<TypeidExpressionAST>(_M_pool);
            ast->typeid_token = typeid_token;
            ast->sub_expressions = nullptr;

            std::size_t index = token_stream.cursor();
            if (token_stream.lookAhead() == '(') {
                token_stream.nextToken();
                if (parseTypeId(ast->type_id) && token_stream.lookAhead() == ')') {
                    token_stream.nextToken(); // skip )

                    UPDATE_POS(ast, start, token_stream.cursor());
                    node = ast;

                    ExpressionAST *e = nullptr;
                    while (parsePostfixExpressionInternal(e)) {
                        ast->sub_expressions = snoc(ast->sub_expressions, e, _M_pool);
                    }

                    return true;
                }

                ast->type_id = nullptr;
                token_stream.rewind(index);
            }
            return false;
        }

        case Token_decltype: {
            std::size_t start = token_stream.cursor();
            token_stream.nextToken();

            CHECK('(');
            ExpressionAST *expr = nullptr;
            if (parseExpression(expr)){
                CHECK(')');
                DecltypeExpressionAST *ast = CreateNode<DecltypeExpressionAST>(_M_pool);
                ast->expression = expr;
                UPDATE_POS(ast, start, token_stream.cursor());
                node = ast;
                return true;
            }
            token_stream.rewind(start);
            return false;
        }

        case Token_noexcept: {
            std::size_t start = token_stream.cursor();
            token_stream.nextToken();

            CHECK('(');
            ExpressionAST *expr = nullptr;
            if (parseExpression(expr)){
                CHECK(')');
                NoexceptExpressionAST *ast = CreateNode<NoexceptExpressionAST>(_M_pool);
                ast->expression = expr;
                UPDATE_POS(ast, start, token_stream.cursor());
                node = ast;
                return true;
            }
            token_stream.rewind(start);
            return false;
        }

        default:
            break;
    }

    int token = token_stream.lookAhead();

    if (token == Token_new
            || (token == Token_scope && token_stream.lookAhead(1) == Token_new))
        return parseNewExpression(node);

    if (token == Token_delete
            || (token == Token_scope && token_stream.lookAhead(1) == Token_delete))
        return parseDeleteExpression(node);

    return parsePostfixExpression(node);
}

bool Parser::parseNewExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    NewExpressionAST *ast = CreateNode<NewExpressionAST>(_M_pool);

    if (token_stream.lookAhead() == Token_scope
            && token_stream.lookAhead(1) == Token_new) {
        ast->scope_token = token_stream.cursor();
        token_stream.nextToken();
    }

    CHECK(Token_new);
    ast->new_token = token_stream.cursor() - 1;

    if (token_stream.lookAhead() == '(') {
        token_stream.nextToken();
        parseCommaExpression(ast->expression);
        CHECK(')');
    }

    if (token_stream.lookAhead() == '(') {
        token_stream.nextToken();
        parseTypeId(ast->type_id);
        CHECK(')');
    } else {
        parseNewTypeId(ast->new_type_id);
    }

    parseNewInitializer(ast->new_initializer);

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseNewTypeId(NewTypeIdAST *&node) {
    std::size_t start = token_stream.cursor();

    TypeSpecifierAST *typeSpec = nullptr;
    if (!parseTypeSpecifier(typeSpec))
        return false;

    NewTypeIdAST *ast = CreateNode<NewTypeIdAST>(_M_pool);
    ast->type_specifier = typeSpec;

    parseNewDeclarator(ast->new_declarator);

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseNewDeclarator(NewDeclaratorAST *&node) {
    std::size_t start = token_stream.cursor();

    NewDeclaratorAST *ast = CreateNode<NewDeclaratorAST>(_M_pool);

    PtrOperatorAST *ptrOp = nullptr;
    if (parsePtrOperator(ptrOp)) {
        ast->ptr_op = ptrOp;
        parseNewDeclarator(ast->sub_declarator);
    }

    while (token_stream.lookAhead() == '[') {
        token_stream.nextToken();
        ExpressionAST *expr = nullptr;
        parseExpression(expr);
        ast->expressions = snoc(ast->expressions, expr, _M_pool);
        ADVANCE(']', "]")
    }

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseNewInitializer(NewInitializerAST *&node) {
    std::size_t start = token_stream.cursor();

    NewInitializerAST *ast (nullptr);
    if (token_stream.lookAhead() == '{') {
        CHECK('{');
        ast = CreateNode<NewInitializerAST>(_M_pool);
        parseCommaExpression(ast->expression);
        CHECK('}');
    }else{
        CHECK('(');
        ast = CreateNode<NewInitializerAST>(_M_pool);
        parseCommaExpression(ast->expression);
        CHECK(')');
    }
    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;
    return true;
}

bool Parser::parseDeleteExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    DeleteExpressionAST *ast = CreateNode<DeleteExpressionAST>(_M_pool);

    if (token_stream.lookAhead() == Token_scope
            && token_stream.lookAhead(1) == Token_delete) {
        ast->scope_token = token_stream.cursor();
        token_stream.nextToken();
    }

    CHECK(Token_delete);
    ast->delete_token = token_stream.cursor() - 1;

    if (token_stream.lookAhead() == '[') {
        ast->lbracket_token = token_stream.cursor();
        token_stream.nextToken();
        CHECK(']');
        ast->rbracket_token = token_stream.cursor() - 1;
    }

    if (!parseCastExpression(ast->expression))
        return false;

    UPDATE_POS(ast, start, token_stream.cursor());
    node = ast;

    return true;
}

bool Parser::parseCastExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    if (token_stream.lookAhead() == '(') {
        token_stream.nextToken();

        CastExpressionAST *ast = CreateNode<CastExpressionAST>(_M_pool);

        if (parseTypeId(ast->type_id)) {
            if (token_stream.lookAhead() == ')') {
                token_stream.nextToken();

                if (parseCastExpression(ast->expression)) {
                    UPDATE_POS(ast, start, token_stream.cursor());
                    node = ast;

                    return true;
                }
            }
        }
    }

    token_stream.rewind(start);
    return parseUnaryExpression(node);
}

bool Parser::parsePmExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    if (!parseCastExpression(node) || !node) // ### fixme
        return false;

    while (token_stream.lookAhead() == Token_ptrmem) {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseCastExpression(rightExpr))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseMultiplicativeExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    if (!parsePmExpression(node))
        return false;

    while (token_stream.lookAhead() == '*'
            || token_stream.lookAhead() == '/'
            || token_stream.lookAhead() == '%') {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parsePmExpression(rightExpr))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}


bool Parser::parseAdditiveExpression(ExpressionAST *&node) {
    std::size_t start = token_stream.cursor();

    if (!parseMultiplicativeExpression(node))
        return false;

    while (token_stream.lookAhead() == '+' || token_stream.lookAhead() == '-') {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseMultiplicativeExpression(rightExpr))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseShiftExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseAdditiveExpression(node))
        return false;

    while (token_stream.lookAhead() == Token_left_shift
           || (token_stream.lookAhead() == '>' && token_stream.lookAhead(1) == '>' && token_stream.lookAhead(2) != '=' && !templArgs)) {
        std::size_t op = token_stream.cursor();
        unsigned opLength = 0;

        if(token_stream.lookAhead() == Token_left_shift){
            token_stream.nextToken();
        }else{
            auto tk1 = token_stream.token(op);
            auto tk2 = token_stream.token(op+1);
            if(tk2.position!=tk1.position+1){
                return true;
            }
            token_stream.nextToken();
            token_stream.nextToken();
            opLength = 1;
        }

        ExpressionAST *rightExpr = nullptr;
        if (!parseAdditiveExpression(rightExpr))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->opLength = opLength;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseRelationalExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseShiftExpression(node, templArgs))
        return false;

    while (token_stream.lookAhead() == '<'
            || (token_stream.lookAhead() == '>' && token_stream.lookAhead(1) != '>' && !templArgs)
            || token_stream.lookAhead() == Token_leq
            || token_stream.lookAhead() == Token_geq
            || token_stream.lookAhead() == Token_compare
           ) {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseShiftExpression(rightExpr, templArgs))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->opLength = 0;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseEqualityExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseRelationalExpression(node, templArgs))
        return false;

    while (token_stream.lookAhead() == Token_eq
            || token_stream.lookAhead() == Token_not_eq) {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseRelationalExpression(rightExpr, templArgs))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseAndExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseEqualityExpression(node, templArgs))
        return false;

    while (token_stream.lookAhead() == '&' || token_stream.lookAhead() == Token_bitand) {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseEqualityExpression(rightExpr, templArgs))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseExclusiveOrExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseAndExpression(node, templArgs))
        return false;

    while (token_stream.lookAhead() == '^' || token_stream.lookAhead() == Token_xor) {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseAndExpression(rightExpr, templArgs))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseInclusiveOrExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseExclusiveOrExpression(node, templArgs))
        return false;

    while (token_stream.lookAhead() == '|' || token_stream.lookAhead() == Token_bitor) {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseExclusiveOrExpression(rightExpr, templArgs))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseLogicalAndExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseInclusiveOrExpression(node, templArgs))
        return false;

    while (token_stream.lookAhead() == Token_and) {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseInclusiveOrExpression(rightExpr, templArgs))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseLogicalOrExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseLogicalAndExpression(node, templArgs))
        return false;

    while (token_stream.lookAhead() == Token_or) {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseLogicalAndExpression(rightExpr, templArgs))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseConditionalExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseLogicalOrExpression(node, templArgs))
        return false;

    if (token_stream.lookAhead() == '?') {
        token_stream.nextToken();

        ExpressionAST *leftExpr = nullptr;
        if (!parseExpression(leftExpr, templArgs))
            return false;

        CHECK(':');

        ExpressionAST *rightExpr = nullptr;
        if (!parseAssignmentExpression(rightExpr, templArgs))
            return false;
        if (rightExpr && token_stream.lookAhead() == Token_ellipsis) {
            rightExpr->ellipsis_token = token_stream.cursor();
            token_stream.nextToken();
        }

        ConditionalExpressionAST *ast
        = CreateNode<ConditionalExpressionAST>(_M_pool);

        ast->condition = node;
        ast->left_expression = leftExpr;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseAssignmentExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseConditionalExpression(node, templArgs))
        return false;

    while (token_stream.lookAhead() == Token_assign
           || token_stream.lookAhead() == Token_xor_eq
           || token_stream.lookAhead() == Token_or_eq
           || token_stream.lookAhead() == Token_and_eq
           || token_stream.lookAhead() == Token_plus_assign
           || token_stream.lookAhead() == Token_minus_assign
           || token_stream.lookAhead() == Token_times_assign
           || token_stream.lookAhead() == Token_rem_assign
           || token_stream.lookAhead() == Token_div_assign
           || token_stream.lookAhead() == Token_left_shift_assign
           || token_stream.lookAhead() == Token_right_shift_assign
           || token_stream.lookAhead() == '=') {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        ExpressionAST *rightExpr = nullptr;
        if (!parseConditionalExpression(rightExpr, templArgs))
            return false;

        BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
        ast->op = op;
        ast->opLength = 0;
        ast->left_expression = node;
        ast->right_expression = rightExpr;

        UPDATE_POS(ast, start, token_stream.cursor());
        node = ast;
    }

    return true;
}

bool Parser::parseConstantExpression(ExpressionAST *&node) {
    return parseConditionalExpression(node);
}

bool Parser::parseExpression(ExpressionAST *&node, bool templArgs) {
    return parseCommaExpression(node, templArgs);
}

bool Parser::parseCommaExpression(ExpressionAST *&node, bool templArgs) {
    std::size_t start = token_stream.cursor();

    if (!parseAssignmentExpression(node, templArgs))
        return false;
    if (node && token_stream.lookAhead() == Token_ellipsis) {
        node->ellipsis_token = token_stream.cursor();
        token_stream.nextToken();
    }

    while (token_stream.lookAhead() == ',') {
        std::size_t op = token_stream.cursor();
        token_stream.nextToken();

        if(token_stream.lookAhead() == Token_ellipsis){
            BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
            ast->op = token_stream.cursor();
            ast->left_expression = node;
            ast->right_expression = nullptr;
            token_stream.nextToken();

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }else{
            ExpressionAST *rightExpr = nullptr;
            if (!parseAssignmentExpression(rightExpr, templArgs))
                return false;
            if (rightExpr && token_stream.lookAhead() == Token_ellipsis) {
                rightExpr->ellipsis_token = token_stream.cursor();
                token_stream.nextToken();
            }

            BinaryExpressionAST *ast = CreateNode<BinaryExpressionAST>(_M_pool);
            ast->op = op;
            ast->left_expression = node;
            ast->right_expression = rightExpr;

            UPDATE_POS(ast, start, token_stream.cursor());
            node = ast;
        }
    }

    return true;
}

bool Parser::parseQ_PROPERTY(DeclarationAST *&node) {
    if (token_stream.lookAhead() != Token_Q_PROPERTY)
        return false;

    if (token_stream.lookAhead(1) != '(')
        return false;

    token_stream.nextToken();
    token_stream.nextToken();

    std::size_t firstToken = token_stream.cursor();
    int nestCount = 1;
    while (nestCount > 0) {
        if (token_stream.lookAhead() == '(')
            nestCount++;
        else if (token_stream.lookAhead() == ')')
            nestCount--;

        if (nestCount > 0)	// don't include last ')'
            token_stream.nextToken();
    }
    QPropertyAST *ast = CreateNode<QPropertyAST>(_M_pool);
    UPDATE_POS(ast, firstToken, token_stream.cursor());
    node = ast;

//   const Token &t1 = token_stream[firstToken];
//   const Token &t2 = token_stream[token_stream.cursor()];
//   printf("property: %s\n",
//          qPrintable(QString::fromLatin1(t1.text + t1.position, t2.position - t1.position)));

    token_stream.nextToken();

    return true;
}

bool Parser::block_errors(bool block) {
    bool current = _M_block_errors;
    _M_block_errors = block;
    return current;
}


// kate: space-indent on; indent-width 2; replace-tabs on;
