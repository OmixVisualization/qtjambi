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


#include "name_compiler.h"
#include "type_compiler.h"
#include "declarator_compiler.h"
#include "lexer.h"
#include "symbol.h"
#include "binder.h"
#include "tokens.h"

#include <QtCore/qdebug.h>

NameCompiler::NameCompiler(Binder *binder)
        : _M_binder(binder), _M_token_stream(binder->tokenStream()), _M_operatorType(OperatorType::None) {
}

QString NameCompiler::decode_operator(std::size_t index, unsigned opLength) const {
    const Token &tk = _M_token_stream->token(index);
    return QString::fromUtf8(&tk.text[tk.position], int(tk.size + opLength));
}

void NameCompiler::internal_run(AST *node) {
    _M_name.clear();
    _M_functionalReturnType = TypeInfo();
    _M_templateArgs.clear();
    _M_templateArgumentTypes.clear();
    _M_functionalArgumentTypes.clear();
    _M_functionalArgumentNames.clear();
    visit(node);
}

void NameCompiler::visitUnqualifiedName(UnqualifiedNameAST *node) {
    QString tmp_name;

    if (node->tilde)
        tmp_name += QLatin1String("~");

    if (node->id)
        tmp_name += _M_token_stream->symbol(node->id)->as_string();

    typedef decltype(std::declval<QString>().size()) Size;

    if (OperatorFunctionIdAST *op_id = node->operator_id) {
        if (op_id->op && op_id->op->op) {
            switch (_M_token_stream->kind(op_id->op->op)) {
            case Token_new:
                if(op_id->op->opLength==0)
                    _M_operatorType = OperatorType::New;
                else
                    _M_operatorType = OperatorType::NewArray;
                break;
            case Token_delete:
                if(op_id->op->opLength==0)
                    _M_operatorType = OperatorType::Delete;
                else
                    _M_operatorType = OperatorType::DeleteArray;
                break;
            case '+':
                _M_operatorType = OperatorType::Plus;
                break;
            case Token_plus_assign:
                _M_operatorType = OperatorType::PlusAssign;
                break;
            case '-':
                _M_operatorType = OperatorType::Minus;
                break;
            case Token_minus_assign:
                _M_operatorType = OperatorType::MinusAssign;
                break;
            case '*':
                _M_operatorType = OperatorType::Times;
                break;
            case Token_times_assign:
                _M_operatorType = OperatorType::TimesAssign;
                break;
            case '/':
                _M_operatorType = OperatorType::Div;
                break;
            case Token_div_assign:
                _M_operatorType = OperatorType::DivAssign;
                break;
            case '%':
                _M_operatorType = OperatorType::Rem;
                break;
            case Token_rem_assign:
                _M_operatorType = OperatorType::RemAssign;
                break;
            case '^':
                _M_operatorType = OperatorType::Xor;
                break;
            case Token_xor_eq:
                _M_operatorType = OperatorType::XorAssign;
                break;
            case '&':
                _M_operatorType = OperatorType::And;
                break;
            case Token_and_eq:
                _M_operatorType = OperatorType::AndAssign;
                break;
            case '|':
                _M_operatorType = OperatorType::Or;
                break;
            case Token_or_eq:
                _M_operatorType = OperatorType::OrAssign;
                break;
            case '~':
                _M_operatorType = OperatorType::Not;
                break;
            case '!':
                _M_operatorType = OperatorType::Negation;
                break;
            case Token_assign:
            case '=':
                _M_operatorType = OperatorType::Assign;
                break;
            case '<':
                _M_operatorType = OperatorType::Less;
                break;
            case Token_leq:
                _M_operatorType = OperatorType::LessOrEquals;
                break;
            case Token_geq:
                _M_operatorType = OperatorType::GreaterOrEquals;
                break;
            case ',':
                _M_operatorType = OperatorType::Comma;
                break;
            case Token_left_shift:
                _M_operatorType = OperatorType::ShiftLeft;
                break;
            case Token_left_shift_assign:
                _M_operatorType = OperatorType::ShiftLeftAssign;
                break;
            case Token_right_shift_assign:
                _M_operatorType = OperatorType::ShiftRightAssign;
                break;
            case Token_eq:
                _M_operatorType = OperatorType::Equals;
                break;
            case Token_not_eq:
                _M_operatorType = OperatorType::NotEquals;
                break;
            case Token_compare:
                _M_operatorType = OperatorType::Compare;
                break;
            case Token_and:
                _M_operatorType = OperatorType::ExclusiveAnd;
                break;
            case Token_or:
                _M_operatorType = OperatorType::ExclusiveOr;
                break;
            case Token_incr:
                _M_operatorType = OperatorType::Inc;
                break;
            case Token_decr:
                _M_operatorType = OperatorType::Dec;
                break;
            case Token_ptrmem:
                _M_operatorType = OperatorType::PointerToMemberOfPointer;
                break;
            case Token_arrow:
                _M_operatorType = OperatorType::MemberOfPointer;
                break;
            case '>':
                switch(op_id->op->opLength){
                case 0:
                    _M_operatorType = OperatorType::Greater;
                    break;
                case 1:
                    switch (_M_token_stream->kind(op_id->op->op+1)) {
                    case '>':
                        _M_operatorType = OperatorType::ShiftRight;
                        break;
                    }
                    break;
                }
                break;
            break;
            case '(':
                switch(op_id->op->opLength){
                case 1:
                    switch (_M_token_stream->kind(op_id->op->op+1)) {
                    case ')':
                        _M_operatorType = OperatorType::FunctionCall;
                        break;
                    }
                    break;
                }
            break;
            case '[':
                switch(op_id->op->opLength){
                case 1:
                    switch (_M_token_stream->kind(op_id->op->op+1)) {
                    case ']':
                        _M_operatorType = OperatorType::Subscript;
                        break;
                    }
                    break;
                }
            break;
            }

            tmp_name += QLatin1String("operator");
            tmp_name += decode_operator(op_id->op->op, op_id->op->opLength);
        } else if (op_id->type_specifier) {
            TypeCompiler type_cc(_M_binder);
            type_cc.run(op_id->type_specifier);
            _M_operatorCastType.setQualifiedName(type_cc.qualifiedName());
            _M_operatorCastType.setConstant(type_cc.isConstant());
            _M_operatorCastType.setVolatile(type_cc.isVolatile());
            _M_operatorCastType.setReferenceType(TypeInfo::NoReference);
            _M_operatorCastType.setIndirections({});
            _M_operatorCastType.setArrayElements({});
            _M_operatorCastType.setFunctionalArgumentTypes(type_cc.functionalArgumentTypes());
            _M_operatorCastType.setFunctionalArgumentNames(type_cc.functionalArgumentNames());
            if(!type_cc.functionalReturnType().qualifiedName().isEmpty())
                _M_operatorCastType.setFunctionalReturnType(type_cc.functionalReturnType());
            _M_operatorType = OperatorType::TypeCast;

            Token const &tk = _M_token_stream->token(op_id->start_token);
            Token const &end_tk = _M_token_stream->token(op_id->end_token);
            tmp_name += QString::fromLatin1(&tk.text[tk.position],
                                            Size(end_tk.position - tk.position)).trimmed();
        } else if (_M_token_stream->token(op_id->type_name).kind==Token_string_literal) {
            _M_operatorType = OperatorType::String;
            Token const &tk = _M_token_stream->token(op_id->type_name+1);
            tmp_name += QLatin1String("operator\"\"");
            tmp_name += QString::fromLatin1(&tk.text[tk.position], Size(tk.size)).trimmed();
        }
    }

    _M_name += tmp_name;
    if (node->template_arguments) {
        // ### cleanup
        visitNodes(this, node->template_arguments);
    }

}

void NameCompiler::visitTemplateArgument(TemplateArgumentAST *node) {
    QString templateArg;
    if (node->type_id && node->type_id->type_specifier) {
        TypeCompiler type_cc(_M_binder);
        type_cc.run(node->type_id->type_specifier);

        DeclaratorCompiler decl_cc(_M_binder);
        decl_cc.run(node->type_id->declarator);

        if (type_cc.isConstant())
            templateArg += "const ";

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

        templateArg += q.join("::");

        if (decl_cc.getReferenceType()==DeclaratorCompiler::Reference)
            templateArg += "&";
        if (decl_cc.getReferenceType()==DeclaratorCompiler::RReference)
            templateArg += "&&";
        for (int i=0; i<decl_cc.indirection().size(); i++){
            if(decl_cc.indirection()[i]){
                templateArg += "*const ";
            }else{
                templateArg += "*";
            }
        }

        if(decl_cc.isFunction()){
            _M_functionalReturnType.setQualifiedName(type_cc.qualifiedName());
            _M_functionalReturnType.setConstant(type_cc.isConstant());
            _M_functionalReturnType.setVolatile(type_cc.isVolatile());
            _M_functionalReturnType.setReferenceType(TypeInfo::ReferenceType(decl_cc.getReferenceType()));
            _M_functionalReturnType.setIndirections(decl_cc.indirection());
            _M_functionalReturnType.setArrayElements(decl_cc.arrayElements());
            _M_functionalReturnType.setFunctionalArgumentTypes(type_cc.functionalArgumentTypes());
            _M_functionalReturnType.setFunctionalArgumentNames(type_cc.functionalArgumentNames());
            if(!type_cc.functionalReturnType().qualifiedName().isEmpty())
                _M_functionalReturnType.setFunctionalReturnType(type_cc.functionalReturnType());
            templateArg += "(";
            bool isBegin = true;
            for(const DeclaratorCompiler::Parameter& parameter : decl_cc.parameters()){
                if(!isBegin)
                    templateArg += ",";
                if(parameter.type.qualifiedName().isEmpty()){
                    templateArg += parameter.name;
                }else{
                    templateArg += parameter.type.toString();
                }
                _M_functionalArgumentTypes << parameter.type;
                _M_functionalArgumentNames << parameter.name;
                isBegin = false;
            }
            templateArg += ")";
        }else{
            TypeInfo type;
            type.setQualifiedName(type_cc.qualifiedName());
            type.setConstant(type_cc.isConstant());
            type.setVolatile(type_cc.isVolatile());
            type.setReferenceType(TypeInfo::ReferenceType(decl_cc.getReferenceType()));
            type.setIndirections(decl_cc.indirection());
            type.setArrayElements(decl_cc.arrayElements());
            type.setArguments(type_cc.templateArgumentTypes());
            type.setFunctionalArgumentTypes(type_cc.functionalArgumentTypes());
            if(!type_cc.functionalReturnType().qualifiedName().isEmpty())
                type.setFunctionalArgumentNames(type_cc.functionalArgumentNames());
            type.setFunctionalReturnType(type_cc.functionalReturnType());
            _M_templateArgumentTypes << type;
        }
    }else if(node->expression){
//        DefaultVisitor defaultVisitor;
//        defaultVisitor.visit(node->expression);
        templateArg = QLatin1String("EXPRESSION");
    }
    _M_templateArgs << templateArg;
}

// kate: space-indent on; indent-width 2; replace-tabs on;
