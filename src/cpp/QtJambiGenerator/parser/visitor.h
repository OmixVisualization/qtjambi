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


#ifndef VISITOR_H
#define VISITOR_H

#include "ast.h"

class Visitor {
    public:
        Visitor();
        virtual ~Visitor();

        virtual void visit(AST *node);

    protected:
        virtual void visitAccessSpecifier(AccessSpecifierAST *) {}
        virtual void visitAsmDefinition(AsmDefinitionAST *) {}
        virtual void visitBaseClause(BaseClauseAST *) {}
        virtual void visitBaseSpecifier(BaseSpecifierAST *) {}
        virtual void visitBinaryExpression(BinaryExpressionAST *) {}
        virtual void visitCastExpression(CastExpressionAST *) {}
        virtual void visitClassMemberAccess(ClassMemberAccessAST *) {}
        virtual void visitClassSpecifier(ClassSpecifierAST *) {}
        virtual void visitCompoundStatement(CompoundStatementAST *) {}
        virtual void visitCondition(ConditionAST *) {}
        virtual void visitConditionalExpression(ConditionalExpressionAST *) {}
        virtual void visitCppCastExpression(CppCastExpressionAST *) {}
        virtual void visitCtorInitializer(CtorInitializerAST *) {}
        virtual void visitDeclarationStatement(DeclarationStatementAST *) {}
        virtual void visitDeclarator(DeclaratorAST *) {}
        virtual void visitDeleteExpression(DeleteExpressionAST *) {}
        virtual void visitDoStatement(DoStatementAST *) {}
        virtual void visitElaboratedTypeSpecifier(ElaboratedTypeSpecifierAST *) {}
        virtual void visitEnumSpecifier(EnumSpecifierAST *) {}
        virtual void visitEnumerator(EnumeratorAST *) {}
        virtual void visitExceptionSpecification(ExceptionSpecificationAST *) {}
        virtual void visitExpressionOrDeclarationStatement(ExpressionOrDeclarationStatementAST *) {}
        virtual void visitExpressionStatement(ExpressionStatementAST *) {}
        virtual void visitClassicForStatement(ClassicForStatementAST *) {}
        virtual void visitIteratorForStatement(IteratorForStatementAST *) {}
        virtual void visitFunctionCall(FunctionCallAST *) {}
        virtual void visitFunctionDefinition(FunctionDefinitionAST *) {}
        virtual void visitIfStatement(IfStatementAST *) {}
        virtual void visitIncrDecrExpression(IncrDecrExpressionAST *) {}
        virtual void visitInitDeclarator(InitDeclaratorAST *) {}
        virtual void visitInitializer(InitializerAST *) {}
        virtual void visitInitializerClause(InitializerClauseAST *) {}
        virtual void visitLabeledStatement(LabeledStatementAST *) {}
        virtual void visitLinkageBody(LinkageBodyAST *) {}
        virtual void visitLinkageSpecification(LinkageSpecificationAST *) {}
        virtual void visitMemInitializer(MemInitializerAST *) {}
        virtual void visitName(NameAST *) {}
        virtual void visitNamespace(NamespaceAST *) {}
        virtual void visitNamespaceAliasDefinition(NamespaceAliasDefinitionAST *) {}
        virtual void visitNewDeclarator(NewDeclaratorAST *) {}
        virtual void visitNewExpression(NewExpressionAST *) {}
        virtual void visitNewInitializer(NewInitializerAST *) {}
        virtual void visitNewTypeId(NewTypeIdAST *) {}
        virtual void visitOperator(OperatorAST *) {}
        virtual void visitOperatorFunctionId(OperatorFunctionIdAST *) {}
        virtual void visitParameterDeclaration(ParameterDeclarationAST *) {}
        virtual void visitParameterDeclarationClause(ParameterDeclarationClauseAST *) {}
        virtual void visitPostfixExpression(PostfixExpressionAST *) {}
        virtual void visitPrimaryExpression(PrimaryExpressionAST *) {}
        virtual void visitPtrOperator(PtrOperatorAST *) {}
        virtual void visitPtrToMember(PtrToMemberAST *) {}
        virtual void visitReturnStatement(ReturnStatementAST *) {}
        virtual void visitSimpleDeclaration(SimpleDeclarationAST *) {}
        virtual void visitSimpleTypeSpecifier(SimpleTypeSpecifierAST *) {}
        virtual void visitAutoTypeSpecifier(AutoTypeSpecifierAST *) {}
        virtual void visitSizeofExpression(SizeofExpressionAST *) {}
        virtual void visitAlignofExpression(AlignofExpressionAST *) {}
        virtual void visitTypeidExpression(TypeidExpressionAST *) {}
        virtual void visitStringLiteral(StringLiteralAST *) {}
        virtual void visitSubscriptExpression(SubscriptExpressionAST *) {}
        virtual void visitSwitchStatement(SwitchStatementAST *) {}
        virtual void visitTemplateArgument(TemplateArgumentAST *) {}
        virtual void visitTemplateDeclaration(TemplateDeclarationAST *) {}
        virtual void visitTemplateParameter(TemplateParameterAST *) {}
        virtual void visitThrowExpression(ThrowExpressionAST *) {}
        virtual void visitTranslationUnit(TranslationUnitAST *) {}
        virtual void visitTryBlockStatement(TryBlockStatementAST *) {}
        virtual void visitTypeId(TypeIdAST *) {}
        virtual void visitTypeIdentification(TypeIdentificationAST *) {}
        virtual void visitTypeParameter(TypeParameterAST *) {}
        virtual void visitTypedef(TypedefAST *) {}
        virtual void visitUnaryExpression(UnaryExpressionAST *) {}
        virtual void visitUnqualifiedName(UnqualifiedNameAST *) {}
        virtual void visitUsing(UsingAST *) {}
        virtual void visitUsingAs(UsingAsAST *) {}
        virtual void visitUsingDirective(UsingDirectiveAST *) {}
        virtual void visitWhileStatement(WhileStatementAST *) {}
        virtual void visitWinDeclSpec(WinDeclSpecAST *) {}
        virtual void visitQProperty(QPropertyAST *) {}
        virtual void visitForwardDeclarationSpecifier(ForwardDeclarationSpecifierAST *) {}
        virtual void visitQGadget(QGadgetAST *) {}
        virtual void visitQObject(QObjectAST *) {}
        virtual void visitDeclDefault(DeclDefaultAST *) {}
        virtual void visitDeclDelete(DeclDeleteAST *) {}
        virtual void visitQDeclFinal(QDeclFinalAST *) {}
        virtual void visitNamedTypeSpecifier(NamedTypeSpecifierAST *) {}
        virtual void visitDecltypeExpression(DecltypeExpressionAST *) {}
        virtual void visitDeclTypeSpecifier(DeclTypeSpecifierAST *) {}

    private:
        typedef void (Visitor::*visitor_fun_ptr)(AST *);
        static visitor_fun_ptr _S_table[];
};

template <class _Tp>
void visitNodes(Visitor *v, const ListNode<_Tp> *nodes) {
    if (!nodes)
        return;

    const ListNode<_Tp>
    *it = nodes->toFront(),
          *end = it;

    do {
        v->visit(it->element);
        it = it->next;
    } while (it != end);
}

#endif // VISITOR_H

// kate: space-indent on; indent-width 2; replace-tabs on;
