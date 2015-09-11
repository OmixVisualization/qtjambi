/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
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


#include "name_compiler.h"
#include "type_compiler.h"
#include "declarator_compiler.h"
#include "lexer.h"
#include "symbol.h"
#include "binder.h"

#include <QtCore/qdebug.h>

NameCompiler::NameCompiler(Binder *binder)
        : _M_binder(binder), _M_token_stream(binder->tokenStream()) {
}

QString NameCompiler::decode_operator(std::size_t index) const {
    const Token &tk = _M_token_stream->token((int) index);
    return QString::fromUtf8(&tk.text[tk.position], (int) tk.size);
}

QString NameCompiler::internal_run(AST *node) {
    _M_name.clear();
    visit(node);
    return name();
}

void NameCompiler::visitUnqualifiedName(UnqualifiedNameAST *node) {
    QString tmp_name;

    if (node->tilde)
        tmp_name += QLatin1String("~");

    if (node->id)
        tmp_name += _M_token_stream->symbol(node->id)->as_string();

    if (OperatorFunctionIdAST *op_id = node->operator_id) {
#if defined(__GNUC__)
#warning "NameCompiler::visitUnqualifiedName() -- implement me"
#endif

        if (op_id->op && op_id->op->op) {
            tmp_name += QLatin1String("operator");
            tmp_name += decode_operator(op_id->op->op);
            if (op_id->op->close)
                tmp_name += decode_operator(op_id->op->close);
        } else if (op_id->type_specifier) {
#if defined(__GNUC__)
#warning "don't use an hardcoded string as cast' name"
#endif
            Token const &tk = _M_token_stream->token((int) op_id->start_token);
            Token const &end_tk = _M_token_stream->token((int) op_id->end_token);
            tmp_name += QString::fromLatin1(&tk.text[tk.position],
                                            (int)(end_tk.position - tk.position)).trimmed();
        }
    }

    _M_name += tmp_name;
    if (node->template_arguments) {
        // ### cleanup
        _M_name.last() += QLatin1String("<");
        visitNodes(this, node->template_arguments);
        _M_name.last().truncate(_M_name.last().count() - 1); // remove the last ','
        _M_name.last() += QLatin1String(">");
    }

}

void NameCompiler::visitTemplateArgument(TemplateArgumentAST *node) {
    if (node->type_id && node->type_id->type_specifier) {
        TypeCompiler type_cc(_M_binder);
        type_cc.run(node->type_id->type_specifier);

        DeclaratorCompiler decl_cc(_M_binder);
        decl_cc.run(node->type_id->declarator);

        if (type_cc.isConstant())
            _M_name.last() += "const ";

        QStringList q = type_cc.qualifiedName();

        if (q.count() == 1) {
#if defined (RXX_RESOLVE_TYPEDEF) // ### it'll break :(
            TypeInfo tp;
            tp.setQualifiedName(q);
            tp = TypeInfo::resolveType(tp, _M_binder->currentScope()->toItem());
            q = tp.qualifiedName();
#endif

            if (CodeModelItem item = _M_binder->model()->findItem(q, _M_binder->currentScope()->toItem())) {
                if (item->name() == q.last())
                    q = item->qualifiedName();
            }
        }

        _M_name.last() += q.join("::");

        if (decl_cc.isReference())
            _M_name.last() += "&";
        for (int i=0; i<decl_cc.indirection().size(); i++){
            if(decl_cc.indirection()[i]){
                _M_name.last() += "*const ";
            }else{
                _M_name.last() += "*";
            }
        }
        _M_name.last() += QLatin1String(",");
    }else if(node->expression){
//        DefaultVisitor defaultVisitor;
//        defaultVisitor.visit(node->expression);
        _M_name.last() += QLatin1String("EXPRESSION,");
    }
}

// kate: space-indent on; indent-width 2; replace-tabs on;
