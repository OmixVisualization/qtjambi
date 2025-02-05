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


#include "compiler_utils.h"
#include "type_compiler.h"
#include "name_compiler.h"
#include "declarator_compiler.h"
#include "ast.h"
#include "binder.h"

TypeInfo CompilerUtils::typeDescription(TypeSpecifierAST *type_specifier, DeclaratorAST *declarator, Binder *binder) {
    TypeCompiler type_cc(binder);
    DeclaratorCompiler decl_cc(binder);

    type_cc.run(type_specifier);
    decl_cc.run(declarator);

    TypeInfo typeInfo;
    typeInfo.setQualifiedName(type_cc.qualifiedName());
    typeInfo.setVariadic(type_cc.isVariadic());
    typeInfo.setConstant(type_cc.isConstant());
    typeInfo.setVolatile(type_cc.isVolatile());
    typeInfo.setReferenceType(TypeInfo::ReferenceType(decl_cc.getReferenceType()));
    typeInfo.setIndirections(decl_cc.indirection());
    typeInfo.setArguments(type_cc.templateArgumentTypes());
    typeInfo.setFunctionalArgumentTypes(type_cc.functionalArgumentTypes());
    typeInfo.setFunctionalArgumentNames(type_cc.functionalArgumentNames());
    if(!type_cc.functionalReturnType().qualifiedName().isEmpty())
        typeInfo.setFunctionalReturnType(type_cc.functionalReturnType());
    if(decl_cc.isFunction()){
        TypeInfo fpTypeInfo;
        fpTypeInfo.setFunctionPointer(true);
        fpTypeInfo.setFunctionalReturnType(typeInfo);
        for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()){
            fpTypeInfo.addFunctionalArgumentType(p.type);
            fpTypeInfo.addFunctionalArgumentName(p.name);
        }
        typeInfo = fpTypeInfo;
        typeInfo.setReferenceType(TypeInfo::ReferenceType(decl_cc.getSubReferenceType()));
        typeInfo.setIndirections(decl_cc.subIndirection());
    }else if(!decl_cc.arrayElements().isEmpty()){
        TypeInfo aTypeInfo;
        aTypeInfo.setArray(true);
        aTypeInfo.setArrayType(typeInfo);
        typeInfo = aTypeInfo;
        typeInfo.setArrayElements(decl_cc.arrayElements());
        typeInfo.setReferenceType(TypeInfo::ReferenceType(decl_cc.getSubReferenceType()));
        typeInfo.setIndirections(decl_cc.subIndirection());
    }

    return typeInfo;
}

// kate: space-indent on; indent-width 2; replace-tabs on;
