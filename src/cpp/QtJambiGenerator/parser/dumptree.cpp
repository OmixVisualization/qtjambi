/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#include "dumptree.h"

#include <QtCore/QString>
#include <QtCore/qdebug.h>

static char const * const names[] = {
    nullptr,
    "AccessSpecifier",
    "AsmDefinition",
    "BaseClause",
    "BaseSpecifier",
    "BinaryExpression",
    "CastExpression",
    "ClassMemberAccess",
    "ClassSpecifier",
    "CompoundStatement",
    "Condition",
    "ConditionalExpression",
    "CppCastExpression",
    "CtorInitializer",
    "DeclarationStatement",
    "Declarator",
    "DeleteExpression",
    "DoStatement",
    "ElaboratedTypeSpecifier",
    "EnumSpecifier",
    "Enumerator",
    "ExceptionSpecification",
    "ExpressionOrDeclarationStatement",
    "ExpressionStatement",
    "ForStatement",
    "FunctionCall",
    "FunctionDefinition",
    "IfStatement",
    "IncrDecrExpression",
    "InitDeclarator",
    "Initializer",
    "InitializerClause",
    "LabeledStatement",
    "LinkageBody",
    "LinkageSpecification",
    "MemInitializer",
    "Name",
    "Namespace",
    "NamespaceAliasDefinition",
    "NewDeclarator",
    "NewExpression",
    "NewInitializer",
    "NewTypeId",
    "Operator",
    "OperatorFunctionId",
    "ParameterDeclaration",
    "ParameterDeclarationClause",
    "PostfixExpression",
    "PrimaryExpression",
    "PtrOperator",
    "PtrToMember",
    "ReturnStatement",
    "SimpleDeclaration",
    "SimpleTypeSpecifier",
    "SizeofExpression",
    "AlignofExpression",
    "StringLiteral",
    "SubscriptExpression",
    "SwitchStatement",
    "TemplateArgument",
    "TemplateDeclaration",
    "TemplateParameter",
    "ThrowExpression",
    "TranslationUnit",
    "TryBlockStatement",
    "TypeId",
    "TypeIdentification",
    "TypeParameter",
    "Typedef",
    "UnaryExpression",
    "UnqualifiedName",
    "Using",
    "UsingDirective",
    "WhileStatement",
    "WinDeclSpec"
};

DumpTree::DumpTree() {
}

void DumpTree::visit(AST *node) {
    static int indent = 0;

    if (node)
        qDebug() << QString(indent * 2, ' ').toLatin1().constData() << names[node->kind]
        << '[' << node->start_token << ", " << node->end_token << ']';

    ++indent;
    DefaultVisitor::visit(node);
    --indent;
}

// kate: space-indent on; indent-width 2; replace-tabs on;
