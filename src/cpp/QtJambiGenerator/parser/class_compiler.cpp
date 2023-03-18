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


#include "class_compiler.h"
#include "lexer.h"
#include "binder.h"
#include "tokens.h"

ClassCompiler::ClassCompiler(Binder *binder): m_isClass(false), _M_binder(binder),
        _M_token_stream(binder->tokenStream()),
        name_cc(_M_binder),
        type_cc(_M_binder) {
}

ClassCompiler::~ClassCompiler() {}

const QString& ClassCompiler::name() const { return _M_name; }
const QList<TypeInfo>& ClassCompiler::templateArgumentTypes() const { return _M_templateArgumentTypes; }
const QList<QPair<TypeInfo,bool>>& ClassCompiler::baseClasses() const { return _M_base_classes; }

void ClassCompiler::run(ClassSpecifierAST *node){
    m_isClass = _M_token_stream->kind(node->class_key)==Token_class;
    name_cc.run(node->name);
    _M_name = name_cc.name();
    _M_templateArgumentTypes = name_cc.templateArgumentTypes();
    _M_base_classes.clear();

    visit(node->base_clause);
}

void ClassCompiler::visitBaseSpecifier(BaseSpecifierAST *node) {
    name_cc.run(node->name);
    QString name = name_cc.name();
    if (! name.isEmpty()){
        TypeInfo info;
        info.setQualifiedName(name_cc.qualifiedName());
        info.setArguments(name_cc.templateArgumentTypes());
        if(node->access_specifier>0){
            switch(_M_token_stream->kind(node->access_specifier)){
            case Token_protected:
                _M_base_classes.append({info, false});
                break;
            case Token_public:
                _M_base_classes.append({info, true});
                break;
            default:break;
            }
        }else if(!m_isClass){
            _M_base_classes.append({info, true});
        }
    }
}

EnumCompiler::EnumCompiler(Binder *binder): _M_binder(binder),
        _M_token_stream(binder->tokenStream()),
        name_cc(_M_binder),
        type_cc(_M_binder) {
}

EnumCompiler::~EnumCompiler() {}

const QString& EnumCompiler::name() const { return _M_name; }
const TypeInfo& EnumCompiler::baseType() const { return _M_base_type; }


void EnumCompiler::run(EnumSpecifierAST *node){
    name_cc.run(node->name);
    _M_name = name_cc.name();
    _M_base_type = TypeInfo();

    if(node->base_type){
        type_cc.run(node->base_type);
        _M_base_type.setQualifiedName(type_cc.qualifiedName());
        _M_base_type.setConstant(type_cc.isConstant());
        _M_base_type.setVolatile(type_cc.isVolatile());
    }
}

// kate: space-indent on; indent-width 2; replace-tabs on;
