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

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */


#include "type_compiler.h"
#include "name_compiler.h"
#include "lexer.h"
#include "symbol.h"
#include "tokens.h"
#include "binder.h"

#include <QtCore/QString>

TypeCompiler::TypeCompiler(Binder *binder)
        : _M_binder(binder), _M_token_stream(binder->tokenStream()), _m_isVariadic(false) {
}

void TypeCompiler::run(TypeSpecifierAST *node) {
    _M_type.clear();
    _M_cv.clear();

    visit(node);

    if (node && node->cv) {
        const ListNode<std::size_t> *it = node->cv->toFront();
        const ListNode<std::size_t> *end = it;
        _m_isVariadic = node->ellipsis!=0 && node->ellipsis!=std::numeric_limits<std::size_t>::max();
        do {
            int kind = _M_token_stream->kind(it->element);
            if (! _M_cv.contains(kind))
                _M_cv.append(kind);

            it = it->next;
        } while (it != end);
    }
}

void TypeCompiler::visitClassSpecifier(ClassSpecifierAST *node) {
    _m_isVariadic = node->ellipsis!=0 && node->ellipsis!=std::numeric_limits<std::size_t>::max();
    visit(node->name);
}

void TypeCompiler::visitEnumSpecifier(EnumSpecifierAST *node) {
    visit(node->name);
    visit(node->base_type);
}

void TypeCompiler::visitElaboratedTypeSpecifier(ElaboratedTypeSpecifierAST *node) {
    _m_isVariadic = node->ellipsis!=0 && node->ellipsis!=std::numeric_limits<std::size_t>::max();
    visit(node->name);
}

void TypeCompiler::visitSimpleTypeSpecifier(SimpleTypeSpecifierAST *node) {
    _m_isVariadic = node->ellipsis!=0 && node->ellipsis!=std::numeric_limits<std::size_t>::max();
    if (const ListNode<std::size_t> *it = node->integrals) {
        it = it->toFront();
        const ListNode<std::size_t> *end = it;
        QString current_item;
        do {
            std::size_t token = it->element;
            current_item += token_name(_M_token_stream->kind(token));
            current_item += " ";
            it = it->next;
        } while (it != end);
        _M_type += current_item.trimmed();
    } else if (node->type_of) {
        // ### implement me
        _M_type += QLatin1String("typeof<...>");
    }

    visit(node->name);
}

void TypeCompiler::visitAutoTypeSpecifier(AutoTypeSpecifierAST *) {
    _M_type += QLatin1String("auto");
}

void TypeCompiler::visitName(NameAST *node) {
    NameCompiler name_cc(_M_binder);
    name_cc.run(node);
    _M_type = QStringList();
    if(node->global){
        _M_type << "";
    }
    _M_type << name_cc.qualifiedName();
    _M_functionalReturnType = name_cc.functionalReturnType();
    _M_templateArgumentTypes = name_cc.templateArgumentTypes();
    _M_functionalArgumentTypes = name_cc.functionalArgumentTypes();
    _M_functionalArgumentNames = name_cc.functionalArgumentNames();
}

QStringList TypeCompiler::cvString() const {
    QStringList lst;

    for(int q : cv()) {
        if (q == Token_const)
            lst.append(QLatin1String("const"));
        else if (q == Token_volatile)
            lst.append(QLatin1String("volatile"));
    }

    return lst;
}

bool TypeCompiler::isConstant() const {
    return _M_cv.contains(Token_const);
}

bool TypeCompiler::isVolatile() const {
    return _M_cv.contains(Token_volatile);
}

bool TypeCompiler::isVariadic() const {
    return _m_isVariadic;
}

// kate: space-indent on; indent-width 2; replace-tabs on;
