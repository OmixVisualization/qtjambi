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


#include "declarator_compiler.h"
#include "name_compiler.h"
#include "type_compiler.h"
#include "compiler_utils.h"
#include "lexer.h"
#include "binder.h"
#include "tokens.h"

#include <qdebug.h>

DeclaratorCompiler::DeclaratorCompiler(Binder *binder)
        : _M_binder(binder), _M_token_stream(binder->tokenStream()), _M_operatorType(OperatorType::None) {
}

void DeclaratorCompiler::run(DeclaratorAST *node) {
    _M_id.clear();
    _M_parameters.clear();
    _M_array.clear();
    _M_function = false;
    _M_reference_type = DeclaratorCompiler::NoReference;
    _M_variadics = false;
    _M_indirection.clear();

    if (node) {
        NameCompiler name_cc(_M_binder);

        DeclaratorAST *decl = node;
        while (decl && decl->sub_declarator)
            decl = decl->sub_declarator;

        Q_ASSERT(decl);

        name_cc.run(decl->id);
        _M_id = name_cc.name();
        _M_operatorType = name_cc.operatorType();
        _M_operatorCastType = name_cc.operatorCastType();
        _M_function = (node->parameter_declaration_clause != nullptr);
        if (node->parameter_declaration_clause && node->parameter_declaration_clause->ellipsis)
            _M_variadics = true;

        visitNodes(this, node->ptr_ops);
        visit(node->parameter_declaration_clause);

        if (const ListNode<ExpressionAST*> *it = node->array_dimensions) {
            it->toFront();
            const ListNode<ExpressionAST*> *end = it;

            do {
                QString elt;
                if (ExpressionAST *expr = it->element) {
                    const Token &start_token = _M_token_stream->token(expr->start_token);
                    const Token &end_token = _M_token_stream->token(expr->end_token);

                    elt += QString::fromUtf8(&start_token.text[start_token.position],
                                             int(end_token.position - start_token.position)).trimmed();
                }

                _M_array.append(elt);

                it = it->next;
            } while (it != end);
        }
    }
}

void DeclaratorCompiler::visitPtrOperator(PtrOperatorAST *node) {
    int op =  _M_token_stream->kind(node->op);

    switch (op) {
        case Token_and:
            _M_reference_type = DeclaratorCompiler::RReference;
            break;
        case '&':
            _M_reference_type = DeclaratorCompiler::Reference;
            break;
        case '*':
            _M_indirection << (node->cv!=nullptr);
            break;
        default:
            break;
    }

    if (node->mem_ptr) {
//ptr to mem -- not implemented
    }
}

void DeclaratorCompiler::visitParameterDeclaration(ParameterDeclarationAST *node) {
    Parameter p;

    //TypeCompiler type_cc(_M_binder);
    DeclaratorCompiler decl_cc(_M_binder);

    decl_cc.run(node->declarator);

    p.name = decl_cc.id();
    p.type = CompilerUtils::typeDescription(node->type_specifier, node->declarator, _M_binder);
    if (node->expression != nullptr) {
        const Token &start = _M_token_stream->token(node->expression->start_token);
        const Token &end = _M_token_stream->token(node->expression->end_token);
        int length = int(end.position - start.position);

        p.defaultValueExpression = QString();
        QString source = QString::fromUtf8(&start.text[start.position], length).trimmed();
        QStringList list = source.split("\n");


        for (int i = 0; i < list.size(); ++i) {
            if (!list.at(i).startsWith("#"))
                p.defaultValueExpression += list.at(i).trimmed();
        }

        p.defaultValue = p.defaultValueExpression.size() > 0;

    }

    _M_parameters.append(p);
}

// kate: space-indent on; indent-width 2; replace-tabs on;
