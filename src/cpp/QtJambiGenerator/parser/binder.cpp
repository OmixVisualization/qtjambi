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

#include <QtCore/QStringList>
#include "binder.h"
#include "lexer.h"
#include "control.h"
#include "symbol.h"
#include "codemodel_finder.h"
#include "class_compiler.h"
#include "compiler_utils.h"
#include "tokens.h"
#include "dumptree.h"

#include <iostream>

#include <qdebug.h>

void binder_default_message_handler(const std::string &str) {
    std::cerr << str;
}

MessageHandler Binder::_M_message_handler = binder_default_message_handler;

Binder::Binder(uint qtVersionMajor, uint qtVersionMinor, uint qtVersionPatch, const TS::TypeDatabase& database,
               CodeModelPtr __model, LocationManager &__location, Control *__control)
        : _M_model(__model),
        _M_location(__location),
        _M_token_stream(&_M_location.token_stream),
        _M_control(__control),
        _M_current_function_type(CodeModel::Normal),
        _M_current_is_template(false),
        m_qtVersionMajor(qtVersionMajor),
        m_qtVersionMinor(qtVersionMinor),
        m_qtVersionPatch(qtVersionPatch),
        m_database(database),
        class_cc(this),
        type_cc(this),
        name_cc(this),
        decl_cc(this) {
    _M_qualified_types["char"] = QString();
    _M_qualified_types["double"] = QString();
    _M_qualified_types["float"] = QString();
    _M_qualified_types["int"] = QString();
    _M_qualified_types["long"] = QString();
    _M_qualified_types["short"] = QString();
    _M_qualified_types["void"] = QString();
}

Binder::~Binder() {
}

void Binder::run(AST *node) {
    _M_current_access = CodeModel::Public;
    updateItemPosition(_M_model->globalNamespace()->toItem(), node);
    visit(node);
}

ScopeModelItem Binder::currentScope() {
    if (_M_current_class)
        return model_static_cast<ScopeModelItem>(_M_current_class);
    else if (_M_current_namespace)
        return model_static_cast<ScopeModelItem>(_M_current_namespace);

    return model_static_cast<ScopeModelItem>(_M_model->globalNamespace());
}


bool Binder::changeIsTemplate(bool currentIsTemplate){
    bool c = _M_current_is_template;
    _M_current_is_template = currentIsTemplate;
    return c;
}

TemplateParameterList Binder::changeTemplateParameters(TemplateParameterList templateParameters) {
    TemplateParameterList old = _M_current_template_parameters;
    _M_current_template_parameters = templateParameters;
    return old;
}

CodeModel::FunctionType Binder::changeCurrentFunctionType(CodeModel::FunctionType functionType) {
    CodeModel::FunctionType old = _M_current_function_type;
    _M_current_function_type = functionType;
    return old;
}

CodeModel::AccessPolicy Binder::changeCurrentAccess(CodeModel::AccessPolicy accessPolicy) {
    CodeModel::AccessPolicy old = _M_current_access;
    _M_current_access = accessPolicy;
    return old;
}

NamespaceModelItem Binder::changeCurrentNamespace(NamespaceModelItem item) {
    NamespaceModelItem old = _M_current_namespace;
    _M_current_namespace = item;
    return old;
}

ClassModelItem Binder::changeCurrentClass(ClassModelItem item) {
    ClassModelItem old = _M_current_class;
    _M_current_class = item;
    return old;
}

FunctionDefinitionModelItem Binder::changeCurrentFunction(FunctionDefinitionModelItem item) {
    FunctionDefinitionModelItem old = _M_current_function;
    _M_current_function = item;
    return old;
}

int Binder::decode_token(std::size_t index) const {
    return _M_token_stream->kind(index);
}

CodeModel::AccessPolicy Binder::decode_access_policy(std::size_t index) const {
    switch (decode_token(index)) {
        case Token_class:
            return CodeModel::Private;

        case Token_struct:
        case Token_union:
            return CodeModel::Public;

        default:
            return CodeModel::Public;
    }
}

CodeModel::ClassType Binder::decode_class_type(std::size_t index) const {
    switch (decode_token(index)) {
        case Token_class:
            return CodeModel::Class;
        case Token_struct:
            return CodeModel::Struct;
        case Token_union:
            return CodeModel::Union;
        default:
            _M_message_handler("unrecognized class type");
    }
    return CodeModel::Class;
}

const NameSymbol *Binder::decode_symbol(std::size_t index) const {
    return _M_token_stream->symbol(index);
}

void Binder::visitAccessSpecifier(AccessSpecifierAST *node) {
    const ListNode<std::size_t> *it =  node->specs;
    if (!it)
        return;

    it = it->toFront();
    const ListNode<std::size_t> *end = it;

    do {
        switch (decode_token(it->element)) {
            default:
                break;

            case Token_public:
                changeCurrentAccess(CodeModel::Public);
                changeCurrentFunctionType(CodeModel::Normal);
                break;
            case Token_protected:
                changeCurrentAccess(CodeModel::Protected);
                changeCurrentFunctionType(CodeModel::Normal);
                break;
            case Token_private:
                changeCurrentAccess(CodeModel::Private);
                changeCurrentFunctionType(CodeModel::Normal);
                break;
            case Token_signals:
                changeCurrentFunctionType(CodeModel::Signal);
                break;
            case Token_slots:
                changeCurrentFunctionType(CodeModel::Slot);
                break;
        }
        it = it->next;
    } while (it != end);
}

void Binder::visitSimpleDeclaration(SimpleDeclarationAST *node) {
    if(!node->arrowDecl){
        visit(node->type_specifier);

        if (const ListNode<InitDeclaratorAST*> *it = node->init_declarators) {
            it = it->toFront();
            const ListNode<InitDeclaratorAST*> *end = it;
            do {
                InitDeclaratorAST *init_declarator = it->element;
                declare_symbol(node, init_declarator);
                it = it->next;
            } while (it != end);
        }
    }
}

void Binder::declare_symbol(SimpleDeclarationAST *node, InitDeclaratorAST *init_declarator) {
    DeclaratorAST *declarator = init_declarator->declarator;

    while (declarator && declarator->sub_declarator)
        declarator = declarator->sub_declarator;

    NameAST *id = declarator->id;
    if (! declarator->id) {
        _M_message_handler("expected a declarator id");
        return;
    }

    CodeModelFinder finder(model(), this);
    ScopeModelItem symbolScope = finder.resolveScope(id, currentScope());
    if (!symbolScope) {
        name_cc.run(declarator->id);
        QStringList qualifiedName = name_cc.qualifiedName();
        QString name = qualifiedName.isEmpty() ? QString() : qualifiedName.takeLast();
        bool isTemplate = false;
        for(QString& n : qualifiedName){
            auto idx = n.indexOf('<');
            if(idx>0){
                n = n.mid(0, idx);
                isTemplate = true;
            }
        }
        if(currentScope()){
            if(isTemplate){
                symbolScope = model_dynamic_cast<ScopeModelItem> (model()->findItem(qualifiedName, currentScope()));
            }
            if(!symbolScope){
                if (TypeAliasModelItem __alias = currentScope()->findTypeAlias(qualifiedName.join("::"))){
                    const TypeInfo& typeInfo = __alias->type();
                    if(!typeInfo.isConstant()
                            && !typeInfo.isFunctionPointer()
                            && !typeInfo.isVariadic()
                            && !typeInfo.isVolatile()
                            && typeInfo.getReferenceType()==TypeInfo::NoReference
                            && typeInfo.indirections().isEmpty()
                            && typeInfo.arguments().isEmpty()){
                        QString typeName = typeInfo.toString();
                        symbolScope = model_dynamic_cast<ScopeModelItem> (model()->findItem(typeInfo.qualifiedName(), currentScope()));
                    }
                }
            }
        }
        if(!symbolScope && isTemplate){
            symbolScope = model_dynamic_cast<ScopeModelItem> (model()->findItem(qualifiedName, model()->globalNamespace()));
        }
        if(!symbolScope){
            if (TypeAliasModelItem __alias = model()->globalNamespace()->findTypeAlias(qualifiedName.join("::"))){
                const TypeInfo& typeInfo = __alias->type();
                if(!typeInfo.isConstant()
                        && !typeInfo.isFunctionPointer()
                        && !typeInfo.isVariadic()
                        && !typeInfo.isVolatile()
                        && typeInfo.getReferenceType()==TypeInfo::NoReference
                        && typeInfo.indirections().isEmpty()
                        && typeInfo.arguments().isEmpty()){
                    QString typeName = typeInfo.toString();
                    symbolScope = model_dynamic_cast<ScopeModelItem> (model()->findItem(typeInfo.qualifiedName(), model()->globalNamespace()));
                }
            }
        }
        if (!symbolScope) {
            if(_M_message_handler && !isTemplate)
                _M_message_handler(std::string("scope not found for symbol: ") + qPrintable(name_cc.name()));
            return;
        }
    }

    decl_cc.run(declarator);

    if (decl_cc.isFunction()) {
        FunctionModelItem fun = model()->create<FunctionModelItem>();
        updateItemPosition(fun->toItem(), node);
        fun->setAccessPolicy(_M_current_access);
        fun->setFunctionType(_M_current_function_type);
        fun->setName(decl_cc.id().split("::").last());
        fun->setAbstract(false);
        if(init_declarator->initializer){
            if(init_declarator->initializer->initializer_clause
                    && init_declarator->initializer->initializer_clause->expression){
                if(init_declarator->initializer->initializer_clause->expression->kind==AST::Kind_DeclDeleteAST){
                    fun->setAccessPolicy(CodeModel::Private);
                    fun->setDeleted(true);
                }else if(init_declarator->initializer->initializer_clause->expression->kind==AST::Kind_DeclDefaultAST){
                    fun->setDeclDefault(true);
                }else{
                    fun->setAbstract(true);
                }
            }else{
                fun->setAbstract(true);
            }
        }
        if(declarator->fun_cv){
            for(int i=0; i<declarator->fun_cv->count(); ++i){
                int kind = decode_token(declarator->fun_cv->at(i)->element);
                if(kind==Token_const){
                    fun->setConstant(true);
                }else if(kind==Token_constexpr){
                    fun->setConstExpr(true);
                }else if(kind==Token_volatile){
                    fun->setVolatile(true);
                }
            }
        }
        if(declarator->fun_reference!=0){
            int kind = decode_token(declarator->fun_reference);
            if(kind==Token_and){
                fun->setReferenceType(TypeInfo::RReference);
            }else if(kind=='&'){
                fun->setReferenceType(TypeInfo::Reference);
            }
        }
        fun->setDeclFinal(declarator->decl_final != nullptr);
        fun->setTemplate(_M_current_is_template);
        fun->setTemplateParameters(_M_current_template_parameters);
        if(_M_current_is_template){
            fun->setTemplateInstantiations(name_cc.templateArgumentTypes());
        }
        applyStorageSpecifiers(node->storage_specifiers, model_static_cast<MemberModelItem>(fun));
        applyFunctionSpecifiers(node->function_specifiers, fun);
        if(!fun->isDeprecated() && node->annotationExpression)
            fun->setDeprecated(isDeprecated(node->annotationExpression, node->deprecationComment));
        if(fun->isDeprecated() && node->deprecationComment){
            if(node->deprecationComment->literals){
                QString comment;
                const ListNode<std::size_t>* front = node->deprecationComment->literals->toFront();
                const ListNode<std::size_t>* iter = front;
                do{
                    const Token &tk = tokenStream()->token(iter->element);
                    const Token &end_tk = tokenStream()->token(iter->element+1);
                    QString cmm = QString::fromLatin1(tk.text + int(tk.position), int(end_tk.position - tk.position));
                    if(cmm.trimmed().startsWith("\"") && cmm.trimmed().endsWith("\"")){
                        cmm = cmm.trimmed().chopped(1).mid(1);
                    }
                    comment += cmm;
                    iter = iter->next;
                }while(iter && iter!=front);
                fun->setDeprecatedComment(comment);
            }else{
                fun->setDeprecatedComment(node->deprecationComment->toString(tokenStream()));
            }
        }

        // build the type
        type_cc.run(node->type_specifier);
        TypeInfo typeInfo;
        typeInfo.setQualifiedName(type_cc.qualifiedName());
        typeInfo.setVariadic(type_cc.isVariadic());
        typeInfo.setConstant(type_cc.isConstant());
        typeInfo.setVolatile(type_cc.isVolatile());
        typeInfo.setReferenceType(TypeInfo::ReferenceType(decl_cc.getReferenceType()));
        typeInfo.setIndirections(decl_cc.indirection());
        typeInfo.setArrayElements(decl_cc.arrayElements());
        typeInfo.setArguments(type_cc.templateArgumentTypes());
        typeInfo.setFunctionalArgumentTypes(type_cc.functionalArgumentTypes());
        typeInfo.setFunctionalArgumentNames(type_cc.functionalArgumentNames());
        if(!type_cc.functionalReturnType().qualifiedName().isEmpty())
            typeInfo.setFunctionalReturnType(type_cc.functionalReturnType());

        fun->setType(qualifyType(typeInfo, symbolScope->qualifiedName()));
        if(decl_cc.operatorType()==OperatorType::TypeCast){
            if(!decl_cc.operatorCastType().qualifiedName().isEmpty()){
                fun->setType(decl_cc.operatorCastType());
            }
        }
        fun->setOperatorType(decl_cc.operatorType());
        fun->setVariadics(decl_cc.isVariadics());

        // ... and the signature
        for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()) {
            ArgumentModelItem arg = model()->create<ArgumentModelItem>();
            TypeInfo tti = qualifyType(p.type, _M_context);
            arg->setType(tti);
            arg->setName(p.name);
            arg->setDefaultValue(p.defaultValue);
            if (p.defaultValue)
                arg->setDefaultValueExpression(p.defaultValueExpression);
            if(!tti.isConstant()
                    && !tti.isFunctionPointer()
                    && !tti.isVolatile()
                    && tti.qualifiedName().join("::")=="void"
                    && tti.indirections().isEmpty()
                    && p.name.isEmpty()
                    && !p.defaultValue
                    && decl_cc.parameters().size()==1){
                // parameter is like: function(void);
            }else{
                fun->addArgument(arg);
            }
        }

        fun->setScope(symbolScope->qualifiedName());
        symbolScope->addFunction(fun);
    } else {
        VariableModelItem var = model()->create<VariableModelItem>();
        updateItemPosition(var->toItem(), node);
        var->setTemplate(_M_current_is_template);
        var->setTemplateParameters(_M_current_template_parameters);
        if(_M_current_is_template){
            var->setTemplateInstantiations(name_cc.templateArgumentTypes());
        }
        var->setAccessPolicy(_M_current_access);
        name_cc.run(id->unqualified_name);
        var->setName(name_cc.name());

        type_cc.run(node->type_specifier);
        TypeInfo typeInfo;
        typeInfo.setQualifiedName(type_cc.qualifiedName());
        typeInfo.setVariadic(type_cc.isVariadic());
        typeInfo.setConstant(type_cc.isConstant());
        typeInfo.setVolatile(type_cc.isVolatile());
        typeInfo.setReferenceType(TypeInfo::ReferenceType(decl_cc.getReferenceType()));
        typeInfo.setIndirections(decl_cc.indirection());
        typeInfo.setArrayElements(decl_cc.arrayElements());
        typeInfo.setArguments(type_cc.templateArgumentTypes());
        typeInfo.setFunctionalArgumentTypes(type_cc.functionalArgumentTypes());
        typeInfo.setFunctionalArgumentNames(type_cc.functionalArgumentNames());
        if(!type_cc.functionalReturnType().qualifiedName().isEmpty())
            typeInfo.setFunctionalReturnType(type_cc.functionalReturnType());

        if (node->type_specifier && node->type_specifier->cv) {
            const ListNode<std::size_t> *it = node->type_specifier->cv->toFront();
            const ListNode<std::size_t> *end = it;
            do {
                int kind = _M_token_stream->kind(it->element);
                if (kind == Token_constexpr)
                    var->setConstExpr(true);
                it = it->next;
            } while (it != end);
        }
        if (node->function_specifiers) {
            const ListNode<std::size_t> *it = node->function_specifiers->toFront();
            const ListNode<std::size_t> *end = it;
            do {
                int kind = _M_token_stream->kind(it->element);
                if (kind == Token_constexpr)
                    var->setConstExpr(true);
                it = it->next;
            } while (it != end);
        }
        if (declarator != init_declarator->declarator
                && init_declarator->declarator->parameter_declaration_clause != nullptr) {
            TypeInfo fpTypeInfo;
            fpTypeInfo.setFunctionPointer(true);
            fpTypeInfo.setFunctionalReturnType(typeInfo);
            decl_cc.run(init_declarator->declarator);
            for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()){
                fpTypeInfo.addFunctionalArgumentType(p.type);
                fpTypeInfo.addFunctionalArgumentName(p.name);
            }
            typeInfo = fpTypeInfo;
        }

        var->setType(qualifyType(typeInfo, _M_context));
        applyStorageSpecifiers(node->storage_specifiers, model_static_cast<MemberModelItem>(var));

        var->setScope(symbolScope->qualifiedName());
        symbolScope->addVariable(var);
    }
}

void Binder::visitFunctionDefinition(FunctionDefinitionAST *node) {
    Q_ASSERT(node->init_declarator);

    ScopeModelItem scope = currentScope();

    InitDeclaratorAST *init_declarator = node->init_declarator;
    DeclaratorAST *declarator = init_declarator->declarator;

    // in the case of "void (func)()" or "void ((func))()" we need to
    // skip to the inner most.  This is in line with how the declarator
    // node is generated in 'parser.cpp'
    while (declarator && declarator->sub_declarator)
        declarator = declarator->sub_declarator;
    if(!declarator->id){
        // not sure why this happens. TODO: find out!
        return;
    }
    Q_ASSERT(declarator->id);

    CodeModelFinder finder(model(), this);

    ScopeModelItem functionScope = finder.resolveScope(declarator->id, scope);
    if (!functionScope) {
        name_cc.run(declarator->id);
        QStringList qualifiedName = name_cc.qualifiedName();
        QString name = qualifiedName.isEmpty() ? QString() : qualifiedName.takeLast();
        bool isTemplate = false;
        for(QString& n : qualifiedName){
            auto idx = n.indexOf('<');
            if(idx>0){
                n = n.mid(0, idx);
                isTemplate = true;
            }
        }
        if(scope){
            if(isTemplate){
                functionScope = model_dynamic_cast<ScopeModelItem> (model()->findItem(qualifiedName, scope));
            }
            if(!functionScope){
                if (TypeAliasModelItem __alias = scope->findTypeAlias(qualifiedName.join("::"))){
                    TypeInfo typeInfo = __alias->type();
                    if(!typeInfo.isConstant()
                            && !typeInfo.isFunctionPointer()
                            && !typeInfo.isVariadic()
                            && !typeInfo.isVolatile()
                            && typeInfo.getReferenceType()==TypeInfo::NoReference
                            && typeInfo.indirections().isEmpty()
                            && typeInfo.arguments().isEmpty()){
                        QString typeName = typeInfo.toString();
                        functionScope = model_dynamic_cast<ScopeModelItem> (model()->findItem(typeInfo.qualifiedName(), scope));
                    }
                }
            }
        }
        if(!functionScope && isTemplate){
            functionScope = model_dynamic_cast<ScopeModelItem> (model()->findItem(qualifiedName, model()->globalNamespace()));
        }
        if(!functionScope){
            if (TypeAliasModelItem __alias = model()->globalNamespace()->findTypeAlias(qualifiedName.join("::"))){
                TypeInfo typeInfo = __alias->type();
                if(!typeInfo.isConstant()
                        && !typeInfo.isFunctionPointer()
                        && !typeInfo.isVariadic()
                        && !typeInfo.isVolatile()
                        && typeInfo.getReferenceType()==TypeInfo::NoReference
                        && typeInfo.indirections().isEmpty()
                        && typeInfo.arguments().isEmpty()){
                    QString typeName = typeInfo.toString();
                    functionScope = model_dynamic_cast<ScopeModelItem> (model()->findItem(typeInfo.qualifiedName(), model()->globalNamespace()));
                }
            }
        }
        if(!functionScope){
            if(_M_message_handler && !isTemplate)
                _M_message_handler(std::string("scope not found for function definition: '")
                               + qPrintable(name_cc.name())
                               + std::string("' - definition *ignored*"));
            return;
        }
    }

    decl_cc.run(declarator);

    Q_ASSERT(! decl_cc.id().isEmpty());

    FunctionDefinitionModelItem old = changeCurrentFunction(_M_model->create<FunctionDefinitionModelItem>());
    _M_current_function->setScope(functionScope->qualifiedName());
    updateItemPosition(_M_current_function->toItem(), node);

    Q_ASSERT(declarator->id->unqualified_name);
    name_cc.run(declarator->id->unqualified_name);
    QString unqualified_name = name_cc.name();

    _M_current_function->setName(unqualified_name);

    type_cc.run(node->type_specifier);
    TypeInfo typeInfo;
    typeInfo.setQualifiedName(type_cc.qualifiedName());
    typeInfo.setVariadic(type_cc.isVariadic());
    typeInfo.setConstant(type_cc.isConstant());
    typeInfo.setVolatile(type_cc.isVolatile());
    typeInfo.setReferenceType(TypeInfo::ReferenceType(decl_cc.getReferenceType()));
    typeInfo.setIndirections(decl_cc.indirection());
    typeInfo.setArrayElements(decl_cc.arrayElements());
    typeInfo.setArguments(type_cc.templateArgumentTypes());
    typeInfo.setFunctionalArgumentTypes(type_cc.functionalArgumentTypes());
    typeInfo.setFunctionalArgumentNames(type_cc.functionalArgumentNames());
    if(!type_cc.functionalReturnType().qualifiedName().isEmpty())
        typeInfo.setFunctionalReturnType(type_cc.functionalReturnType());

    _M_current_function->setType(qualifyType(typeInfo, _M_context));
    _M_current_function->setAccessPolicy(_M_current_access);
    _M_current_function->setFunctionType(_M_current_function_type);
    _M_current_function->setOperatorType(decl_cc.operatorType());
    if(_M_current_function->operatorType()==OperatorType::TypeCast){
        if(!decl_cc.operatorCastType().qualifiedName().isEmpty())
            _M_current_function->setType(decl_cc.operatorCastType());
    }
    if(declarator->fun_cv){
        for(int i=0; i<declarator->fun_cv->count(); ++i){
            int kind = decode_token(declarator->fun_cv->at(i)->element);
            if(kind==Token_const){
                _M_current_function->setConstant(true);
            }else if(kind==Token_constexpr){
                _M_current_function->setConstExpr(true);
            }else if(kind==Token_volatile){
                _M_current_function->setVolatile(true);
            }
        }
    }
    if(declarator->fun_reference!=0){
        int kind = decode_token(declarator->fun_reference);
        if(kind==Token_and){
            _M_current_function->setReferenceType(TypeInfo::RReference);
        }else if(kind=='&'){
            _M_current_function->setReferenceType(TypeInfo::Reference);
        }
    }
    _M_current_function->setTemplate(_M_current_is_template);
    _M_current_function->setTemplateParameters(_M_current_template_parameters);
    if(_M_current_is_template){
        _M_current_function->setTemplateInstantiations(name_cc.templateArgumentTypes());
    }
    _M_current_function->setDeclFinal(declarator->decl_final != nullptr);
    _M_current_function->setHasBody(node->function_body);

    applyStorageSpecifiers(node->storage_specifiers,
                           model_static_cast<MemberModelItem>(_M_current_function));
    if(_M_current_function->isFriend()){
        _M_current_function->setAccessPolicy(CodeModel::Public);
    }
    applyFunctionSpecifiers(node->function_specifiers,
                            model_static_cast<FunctionModelItem>(_M_current_function));
    if(!_M_current_function->isDeprecated() && node->annotationExpression)
        _M_current_function->setDeprecated(isDeprecated(node->annotationExpression, node->deprecationComment));
    if(_M_current_function->isDeprecated() && node->deprecationComment){
        if(node->deprecationComment->literals){
            QString comment;
            const ListNode<std::size_t>* front = node->deprecationComment->literals->toFront();
            const ListNode<std::size_t>* iter = front;
            do{
                const Token &tk = tokenStream()->token(iter->element);
                const Token &end_tk = tokenStream()->token(iter->element+1);
                QString cmm = QString::fromLatin1(tk.text + int(tk.position), int(end_tk.position - tk.position));
                if(cmm.trimmed().startsWith("\"") && cmm.trimmed().endsWith("\"")){
                    cmm = cmm.trimmed().chopped(1).mid(1);
                }
                comment += cmm;
                iter = iter->next;
            }while(iter && iter!=front);
            _M_current_function->setDeprecatedComment(comment);
        }else{
            _M_current_function->setDeprecatedComment(node->deprecationComment->toString(tokenStream()));
        }
    }

    _M_current_function->setVariadics(decl_cc.isVariadics());

    for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()) {
        ArgumentModelItem arg = model()->create<ArgumentModelItem>();
        TypeInfo tti = qualifyType(p.type, functionScope->qualifiedName());
        arg->setType(tti);
        arg->setName(p.name);
        arg->setDefaultValue(p.defaultValue);
        if (p.defaultValue)
            arg->setDefaultValueExpression(p.defaultValueExpression);
        if(!tti.isConstant()
                && !tti.isFunctionPointer()
                && !tti.isVolatile()
                && tti.qualifiedName().join("::")=="void"
                && tti.indirections().isEmpty()
                && p.name.isEmpty()
                && !p.defaultValue
                && decl_cc.parameters().size()==1){
            // parameter is like: function(void);
        }else{
            _M_current_function->addArgument(arg);
        }
    }

    functionScope->addFunctionDefinition(_M_current_function);

    FunctionModelItem prototype = model_static_cast<FunctionModelItem>(_M_current_function);
    FunctionModelItem declared = functionScope->declaredFunction(prototype);

    // try to find a function declaration for this definition..
    if (! declared) {
        functionScope->addFunction(prototype);
    } else {
        applyFunctionSpecifiers(node->function_specifiers, declared);

        // fix the function type and the access policy
        _M_current_function->setAccessPolicy(declared->accessPolicy());
        _M_current_function->setFunctionType(declared->functionType());
    }

    changeCurrentFunction(old);
}

void Binder::visitTemplateDeclaration(TemplateDeclarationAST *node) {
    const ListNode<TemplateParameterAST*> *it = node->template_parameters;

    TemplateParameterList savedTemplateParameters = changeTemplateParameters(TemplateParameterList());
    bool oldIsTemplate = changeIsTemplate(true);
    auto sc = qScopeGuard([&](){
        changeTemplateParameters(savedTemplateParameters);
        changeIsTemplate(oldIsTemplate);
    });

    if (it){
        it = it->toFront();
        const ListNode<TemplateParameterAST*> *end = it;

        do{
            TemplateParameterAST *parameter = it->element;
            TypeParameterAST *type_parameter = parameter->type_parameter;

            QString defaultType;
            NameAST *name;
            bool isVariadic = false;
            if (!type_parameter) {
                // A hacky hack to work around missing support for parameter declarations in
                // templates. We just need the to get the name of the variable, since we
                // aren't actually compiling these anyway. We are still not supporting much
                // more, but we are refusing to fail for a few more declarations
                if (parameter->parameter_declaration == nullptr ||
                        parameter->parameter_declaration->declarator == nullptr ||
                        parameter->parameter_declaration->declarator->id == nullptr) {
                    changeTemplateParameters(savedTemplateParameters);
                    return;

                }

                isVariadic = parameter->parameter_declaration->declarator->ellipsis!=0 && parameter->parameter_declaration->declarator->ellipsis!=std::numeric_limits<std::size_t>::max();
                name = parameter->parameter_declaration->declarator->id;
                if(parameter->parameter_declaration->expression){
                    defaultType = parameter->parameter_declaration->expression->toString(this->tokenStream());
                }
            } else {
                int tk = decode_token(type_parameter->type);
                if (tk != Token_typename && tk != Token_class) {
                    return;
                }
                assert(tk == Token_typename || tk == Token_class);

                isVariadic = type_parameter->ellipsis!=0 && type_parameter->ellipsis!=std::numeric_limits<std::size_t>::max();
                name = type_parameter->name;
                if(type_parameter->type_id){
                    defaultType = type_parameter->type_id->toString(this->tokenStream());
                }else if(type_parameter->type_expression){
                    defaultType = type_parameter->type_expression->toString(this->tokenStream());
                }
            }

            TemplateParameterModelItem p = model()->create<TemplateParameterModelItem>();
            name_cc.run(name);
            p->setName(name_cc.name());
            p->setIsVaradic(isVariadic);
            p->setDefaultValue(defaultType);

            _M_current_template_parameters.append(p);
            it = it->next;
        }while (it != end);
    }

    visit(node->declaration);
}

void Binder::visitTypedef(TypedefAST *node) {
    const ListNode<InitDeclaratorAST*> *it = node->init_declarators;
    if (it == nullptr)
        return;

    it = it->toFront();
    const ListNode<InitDeclaratorAST*> *end = it;

    do {
        InitDeclaratorAST *init_declarator = it->element;
        it = it->next;

        Q_ASSERT(init_declarator->declarator != nullptr);

        // the name
        decl_cc.run(init_declarator->declarator);
        QString alias_name = decl_cc.id();

        if (alias_name.isEmpty()) {
            _M_message_handler("anonymous typedef not supported! ``");
            Token const &tk = _M_token_stream->token(node->start_token);
            Token const &end_tk = _M_token_stream->token(node->end_token);

            _M_message_handler(std::string(&tk.text[tk.position], end_tk.position - tk.position));
            continue;
        }

        // build the type
        TypeInfo typeInfo = CompilerUtils::typeDescription(node->type_specifier,
                            init_declarator->declarator,
                            this);

        DeclaratorAST *decl = init_declarator->declarator;
        while (decl && decl->sub_declarator)
            decl = decl->sub_declarator;

        /*if (decl != init_declarator->declarator
                && init_declarator->declarator->parameter_declaration_clause != nullptr) {
            TypeInfo fpTypeInfo;
            fpTypeInfo.setFunctionPointer(true);
            fpTypeInfo.setFunctionalReturnType(typeInfo);
            decl_cc.run(init_declarator->declarator);
            for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()){
                fpTypeInfo.addFunctionalArgumentType(p.type);
                fpTypeInfo.addFunctionalArgumentName(p.name);
            }
            typeInfo = fpTypeInfo;
        }*/

        ScopeModelItem scope = currentScope();
        DeclaratorAST *declarator = init_declarator->declarator;
        CodeModelFinder finder(model(), this);
        ScopeModelItem typedefScope = finder.resolveScope(declarator->id, scope);

        TypeAliasModelItem typeAlias = model()->create<TypeAliasModelItem> ();
        updateItemPosition(typeAlias->toItem(), node);
        typeAlias->setName(alias_name);
        typeAlias->setType(qualifyType(typeInfo, currentScope()->qualifiedName()));
        typeAlias->setScope(typedefScope->qualifiedName());
        typeAlias->setAccessPolicy(_M_current_access);
        _M_qualified_types[typeAlias->qualifiedName().join(".")] = QString();
        if(m_database.findPrimitiveType(alias_name))
            continue;
        /*if(typeAlias->fileName().endsWith("qtypes.h"))
            continue;
        if(!typeAlias->fileName().endsWith("qtypes.h")
                && !typeAlias->fileName().endsWith("qglobal.h"))
        */
        currentScope()->addTypeAlias(typeAlias);
    } while (it != end);
}

void Binder::visitNamespace(NamespaceAST *node) {
    bool anonymous = (node->namespace_name == 0);

    bool isNew = false;
    ScopeModelItem scope = currentScope();

    NamespaceModelItem old;
    if (! anonymous) {
        QStringList name{decode_symbol(node->namespace_name)->as_string()};

        QStringList qualified_name = scope->qualifiedName();
        for(decltype (node->namespace_name_size) i=1; i<node->namespace_name_size; ++i){
            name << decode_symbol(node->namespace_name+(i*2))->as_string();
        }
        qualified_name << name.join("::");
        NamespaceModelItem ns =
            model_safe_cast<NamespaceModelItem>(_M_model->findItem(qualified_name,
                                                scope));
//        if (!ns) {
//            if (NamespaceModelItem sc = model_static_cast<NamespaceModelItem>(scope)) {
//                ns = sc->findNamespace(name.join("::"));
//            }
//        }
        if (!ns) {
            isNew = true;
            ns = _M_model->create<NamespaceModelItem>();
            updateItemPosition(ns->toItem(), node);
            ns->setName(name.join("::"));
            ns->setScope(scope->qualifiedName());
        }
        old = changeCurrentNamespace(ns);

        _M_context << name.join("::");
    }

    DefaultVisitor::visitNamespace(node);

    if (! anonymous) {
        Q_ASSERT(scope->kind() == _CodeModelItem::Kind_Namespace
                 || scope->kind() == _CodeModelItem::Kind_File);

        _M_context.removeLast();

        if(isNew){
            if (NamespaceModelItem ns = model_static_cast<NamespaceModelItem>(scope)) {
                ns->addNamespace(_M_current_namespace);
            }
        }

        changeCurrentNamespace(old);
    }
}

void Binder::visitForwardDeclarationSpecifier(ForwardDeclarationSpecifierAST *node) {
    name_cc.run(node->name);
    if (name_cc.name().isEmpty())
        return;

    ScopeModelItem scope = currentScope();
    _M_qualified_types[(scope->qualifiedName() + name_cc.qualifiedName()).join(".")] = QString();
}

bool Binder::isDeprecated(ExpressionAST *annotationExpression, StringLiteralAST *&deprecationComment){
    if(annotationExpression){
        class DeprecationBinder: protected DefaultVisitor {
            DeprecationBinder(Binder* binder, StringLiteralAST *&deprecationComment)
                : DefaultVisitor(),
                m_isCurrentDeprecated(false),
                m_isDeprecated(false),
                m_binder(binder),
                m_deprecationComment(deprecationComment){}

            void visitPostfixExpression(PostfixExpressionAST *node) {
                if(!m_isCurrentDeprecated)
                    DefaultVisitor::visitPostfixExpression(node);
                m_isCurrentDeprecated = false;
            }
            void visitSimpleTypeSpecifier(SimpleTypeSpecifierAST *node) {
                m_binder->name_cc.run(node->name);
                if(m_binder->name_cc.name()=="deprecated"){
                    m_isDeprecated = true;
                    m_isCurrentDeprecated = true;
                }
            }
            void visitStringLiteral(StringLiteralAST *ast){
                if(m_isCurrentDeprecated)
                    m_deprecationComment = ast;
            }
            void visitName(NameAST *node) {
                m_binder->name_cc.run(node);
                if(m_binder->name_cc.name()=="deprecated"){
                    m_isDeprecated = true;
                }
            }
            bool m_isCurrentDeprecated;
            bool m_isDeprecated;
            Binder* m_binder;
            StringLiteralAST *&m_deprecationComment;
            friend Binder;
        } b(this, deprecationComment);
        b.visit(annotationExpression);
        return b.m_isDeprecated;
    }
    return false;
}

void Binder::visitClassSpecifier(ClassSpecifierAST *node) {
    class_cc.run(node);
    if (class_cc.name().isEmpty()) {
        // anonymous not supported
        return;
    }

    Q_ASSERT(node->name != nullptr && node->name->unqualified_name != nullptr);

    ScopeModelItem scope = currentScope();
    ClassModelItem thisClass/* = scope->findClass((scope->qualifiedName() << class_cc.name()).join("::"));
    bool isNewClass = !thisClass;
    if(isNewClass)
        thisClass*/ = _M_model->create<ClassModelItem>();

    ClassModelItem old = changeCurrentClass(thisClass);
    updateItemPosition(thisClass->toItem(), node);
    thisClass->setName(class_cc.name());
    thisClass->setDeclFinal(node->is_final);
    if(!node->is_deprecated && node->annotationExpression)
        node->is_deprecated = isDeprecated(node->annotationExpression, node->deprecationComment);
    thisClass->setDeclDeprecated(node->is_deprecated);
    thisClass->setAccessPolicy(_M_current_access);
    if(node->deprecationComment)
        thisClass->setDeclDeprecatedComment(node->deprecationComment->toString(tokenStream()));

    QList<QPair<TypeInfo,bool>> baseClasses = class_cc.baseClasses();
    for (int i = 0; i < baseClasses.size(); ++i) {
        baseClasses[i].first.setQualifiedName(qualifyType(baseClasses.at(i).first, scope->qualifiedName()).qualifiedName());
    }

    thisClass->setBaseClasses(baseClasses);
    thisClass->setClassType(decode_class_type(node->class_key));
    thisClass->setIsTemplate(_M_current_is_template);
    thisClass->setTemplateParameters(_M_current_template_parameters);
    if(_M_current_is_template){
        thisClass->setTemplateInstantiations(class_cc.templateArgumentTypes());
    }
    //thisClass->setIsGadget(node->);

//    if (_M_current_is_template) {
//        QString name = thisClass->name();
//        name += "<";
//        for (int i = 0; i < _M_current_template_parameters.size(); ++i) {
//            if (i != 0)
//                name += ",";

//            name += _M_current_template_parameters.at(i)->name();
//        }

//        name += ">";
//        thisClass->setName(name);
//    }

    CodeModel::AccessPolicy oldAccessPolicy = changeCurrentAccess(decode_access_policy(node->class_key));
    CodeModel::FunctionType oldFunctionType = changeCurrentFunctionType(CodeModel::Normal);

    /*if(isNewClass)*/{
        thisClass->setScope(scope->qualifiedName());
        _M_qualified_types[thisClass->qualifiedName().join(".")] = QString();

        scope->addClass(thisClass);
    }

    //name_cc.run(node->name->unqualified_name);
    //_M_context.append(name_cc.name());
    _M_context.append(class_cc.name());
    bool oldIsTemplate = changeIsTemplate(false);
    visitNodes(this, node->member_specs);
    changeIsTemplate(oldIsTemplate);
    _M_context.removeLast();

    changeCurrentClass(old);
    changeCurrentAccess(oldAccessPolicy);
    changeCurrentFunctionType(oldFunctionType);
}


void Binder::visitQGadget(QGadgetAST *) {
    _M_current_class->setHas_Q_GADGET(true);
}

void Binder::visitQObject(QObjectAST *) {
    _M_current_class->setHas_Q_OBJECT(true);
}

void Binder::visitLinkageSpecification(LinkageSpecificationAST *node) {
    DefaultVisitor::visitLinkageSpecification(node);
}

void Binder::visitUsing(UsingAST *node) {
    if(_M_current_class){
        name_cc.run(node->name);
        QStringList qualifiedName = name_cc.qualifiedName();
        if(!_M_current_class->baseClasses().isEmpty()){
            QString baseClassName = _M_current_class->baseClasses().first().first.qualifiedName().last();
            QString methodName = qualifiedName.takeLast();
            QString className = qualifiedName.join("::");
            if((className==baseClassName || className.startsWith(baseClassName+"<"))
                    && methodName==baseClassName.split("::").last()){
                _M_current_class->setUsingBaseConstructors(_M_current_access);
            }
        }
    }
    DefaultVisitor::visitUsing(node);
}

void Binder::visitUsingAs(UsingAsAST *node) {
    if (node->type_specifier == nullptr)
        return;

    TypeInfo typeInfo = CompilerUtils::typeDescription(node->type_specifier,
                        node->declarator,
                        this);
    QString alias_name = node->name->toString(_M_token_stream).trimmed();

    if (alias_name.isEmpty()) {
        _M_message_handler("anonymous using not supported! ``");
        Token const &tk = _M_token_stream->token(node->start_token);
        Token const &end_tk = _M_token_stream->token(node->end_token);

        _M_message_handler(std::string(&tk.text[tk.position], end_tk.position - tk.position));
    }else{

        // build the type
        DeclaratorAST *decl = node->declarator;
        while (decl && decl->sub_declarator)
            decl = decl->sub_declarator;

        /*if (decl != node->declarator
                && node->declarator->parameter_declaration_clause != nullptr) {
            TypeInfo fpTypeInfo;
            fpTypeInfo.setFunctionPointer(true);
            fpTypeInfo.setFunctionalReturnType(typeInfo);
            decl_cc.run(node->declarator);
            for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()){
                fpTypeInfo.addFunctionalArgumentType(p.type);
                fpTypeInfo.addFunctionalArgumentName(p.name);
            }
            typeInfo = fpTypeInfo;
        }*/

        ScopeModelItem scope = currentScope();
        DeclaratorAST *declarator = node->declarator;
        CodeModelFinder finder(model(), this);
        ScopeModelItem typedefScope = finder.resolveScope(declarator->id, scope);

        TypeAliasModelItem typeAlias = model()->create<TypeAliasModelItem> ();
        updateItemPosition(typeAlias->toItem(), node);
        typeAlias->setName(alias_name);
        typeAlias->setType(qualifyType(typeInfo, currentScope()->qualifiedName()));
        typeAlias->setScope(typedefScope->qualifiedName());
        typeAlias->setAccessPolicy(_M_current_access);
        _M_qualified_types[typeAlias->qualifiedName().join(".")] = QString();
        if(m_database.findPrimitiveType(alias_name))
            return;
        currentScope()->addTypeAlias(typeAlias);
    }
}

void Binder::visitEnumSpecifier(EnumSpecifierAST *node) {
    CodeModelFinder finder(model(), this);
    ScopeModelItem scope = currentScope();
    ScopeModelItem enumScope = finder.resolveScope(node->name, scope);

    name_cc.run(node->name);
    QString name = name_cc.name();

    bool isAnonymous = false;
    if (name.isEmpty()) {
        // anonymous enum
        isAnonymous = true;
        QString key = _M_context.join("::");
        int current = ++_M_anonymous_enums[key];
        name += QLatin1String("enum_");
        name += QString::number(current);
    }

    _M_current_enum = model()->create<EnumModelItem>();
    _M_current_enum->setAccessPolicy(_M_current_access);
    _M_current_enum->setAnonymous(isAnonymous);
    _M_current_enum->setScopedEnum(node->isScoped);
    if(!node->isDeprecated && node->annotationExpression)
        node->isDeprecated = isDeprecated(node->annotationExpression, node->deprecationComment);
    _M_current_enum->setDeclDeprecated(node->isDeprecated);
    if(node->deprecationComment){
        _M_current_enum->setDeclDeprecatedComment(node->deprecationComment->toString(this->tokenStream()));
    }
    updateItemPosition(_M_current_enum->toItem(), node);
    _M_current_enum->setName(name);
    _M_current_enum->setScope(enumScope->qualifiedName());

    _M_qualified_types[_M_current_enum->qualifiedName().join(".")] = QString();

    enumScope->addEnum(_M_current_enum);

    DefaultVisitor::visitEnumSpecifier(node);

    EnumCompiler enum_cc(this);
    enum_cc.run(node);
    TypeInfo baseType = enum_cc.baseType();
    if(!baseType.qualifiedName().isEmpty()) {
        TypeAliasModelItem alias = scope->findTypeAlias(baseType.qualifiedName().join("::"));
        if(alias){
            baseType.setQualifiedName(alias->name().split("::"));
        }
        TypeInfo info = qualifyType(baseType, scope->qualifiedName());
        if(!info.qualifiedName().isEmpty()){
            baseType.setQualifiedName(info.qualifiedName());
        }
    }

    _M_current_enum->setBaseType(baseType);

    _M_current_enum = nullptr;
}

static QString strip_preprocessor_lines(const QString &name) {
    QStringList lst = name.split("\n");
    QString s;
    for (int i = 0; i < lst.size(); ++i) {
        if (!lst.at(i).startsWith('#'))
            s += lst.at(i);
    }
    return s.trimmed();
}

void Binder::visitEnumerator(EnumeratorAST *node) {
    Q_ASSERT(_M_current_enum != nullptr);
    EnumeratorModelItem e = model()->create<EnumeratorModelItem>();
    updateItemPosition(e->toItem(), node);
    e->setName(decode_symbol(node->id)->as_string());
    if(!node->isDeprecated && node->annotationExpression)
        node->isDeprecated = isDeprecated(node->annotationExpression, node->deprecationComment);
    e->setDeprecated(node->isDeprecated);
    if(node->deprecationComment){
        QString comment;
        if(node->deprecationComment->literals){
            const ListNode<std::size_t>* front = node->deprecationComment->literals->toFront();
            const ListNode<std::size_t>* iter = front;
            do{
                const Token &tk = tokenStream()->token(iter->element);
                const Token &end_tk = tokenStream()->token(iter->element+1);
                QString cmm = QString::fromLatin1(tk.text + int(tk.position), int(end_tk.position - tk.position)).trimmed();
                if(cmm.startsWith("\"") && cmm.endsWith("\"")){
                    cmm = cmm.chopped(1).mid(1);
                }
                comment += cmm;
                iter = iter->next;
            }while(iter && iter!=front);
        }else{
            comment = node->deprecationComment->toString(tokenStream()).trimmed();
            if(comment.startsWith("\"") && comment.endsWith("\"")){
                comment = comment.chopped(1).mid(1);
            }
        }
        e->setDeprecatedComment(comment);
    }

    if (ExpressionAST *expr = node->expression) {
        const Token &start_token = _M_token_stream->token(expr->start_token);
        const Token &end_token = _M_token_stream->token(expr->end_token);

        e->setValue(strip_preprocessor_lines(QString::fromUtf8(&start_token.text[start_token.position],
                                             int(end_token.position - start_token.position)).trimmed()).remove(' '));
    }

    _M_current_enum->addEnumerator(e);
}

void Binder::visitUsingDirective(UsingDirectiveAST *node) {
    DefaultVisitor::visitUsingDirective(node);
}

void Binder::visitQProperty(QPropertyAST *node) {
    const Token &start = _M_token_stream->token(node->start_token);
    const Token &end = _M_token_stream->token(node->end_token);
    QString property = QString::fromLatin1(start.text + start.position,
                                           int(end.position - start.position));
    _M_current_class->addPropertyDeclaration(property);
}

void Binder::applyStorageSpecifiers(const ListNode<std::size_t> *it, MemberModelItem item) {
    if (it == nullptr)
        return;

    it = it->toFront();
    const ListNode<std::size_t> *end = it;

    do {
        switch (decode_token(it->element)) {
            default:
                break;

            case Token_friend:
                item->setFriend(true);
                break;
            case Token_register:
                item->setRegister(true);
                break;
            case Token_static:
                item->setStatic(true);
                break;
            case Token_constexpr:
                item->setConstExpr(true);
                break;
            case Token_QTJAMBI_DEPRECATED:
                item->setDeprecated(true);
                break;
            case Token_QTJAMBI_DEPRECATED_X:
                item->setDeprecated(true);
                break;
            case Token_extern:
                item->setExtern(true);
                break;
            case Token_mutable:
                item->setMutable(true);
                break;
        }
        it = it->next;
    } while (it != end);
}

void Binder::applyFunctionSpecifiers(const ListNode<std::size_t> *it, FunctionModelItem item) {
    if (it == nullptr)
        return;

    it = it->toFront();
    const ListNode<std::size_t> *end = it;

    do {
        switch (decode_token(it->element)) {
            default:
                break;

            case Token_constexpr:
                //item->setConstant(true);
                break;

            case Token_inline:
                item->setInline(true);
                break;

            case Token_virtual:
                item->setVirtual(true);
                break;

            case Token_explicit:
                item->setExplicit(true);
                break;

            case Token_Q_INVOKABLE:
                item->setInvokable(true);
                break;

            case Token_QTJAMBI_DEPRECATED:
                item->setDeprecated(true);
                break;

            case Token_QTJAMBI_DEPRECATED_X:
                item->setDeprecated(true);
                break;

            case Token_Q_SCRIPTABLE:
                item->setInvokable(true);
                break;
        }
        it = it->next;
    } while (it != end);
}

TypeInfo Binder::qualifyType(TypeInfo type, const QStringList &context) const {
    // ### Potentially improve to use string list in the name table to
    if(type.functionalReturnType()){
        type.setFunctionalReturnType(qualifyType(type.functionalReturnType(), context));
    }
    if(!type.functionalArgumentTypes().isEmpty()){
        QList<TypeInfo> functionalArgumentTypes;
        for(const TypeInfo& functionalArgumentType : type.functionalArgumentTypes()){
            functionalArgumentTypes << qualifyType(functionalArgumentType, context);
        }
        type.setFunctionalArgumentTypes(functionalArgumentTypes);
    }
    if(!type.arguments().isEmpty()){
        QList<TypeInfo> argumentTypes;
        for(const TypeInfo& argumentType : type.arguments()){
            argumentTypes << qualifyType(argumentType, context);
        }
        type.setArguments(argumentTypes);
    }
    if (context.isEmpty() || type.qualifiedName().isEmpty()) {
        // ### We can assume that this means global namespace for now...
        return type;
    } else if (_M_qualified_types.contains(type.qualifiedName().join("."))) {
        return type;
    } else {
        QStringList expanded = context;
        expanded << type.qualifiedName();
        if (_M_qualified_types.contains(expanded.join("."))) {
            type.setQualifiedName(expanded);
            return type;
        } else {
            CodeModelItem scope = model()->findItem(context, _M_model->globalNamespace()->toItem());

            if (ClassModelItem klass = model_dynamic_cast<ClassModelItem> (scope)) {
                for(const QPair<TypeInfo,bool>& base : klass->baseClasses()) {
                    QStringList ctx = context;
                    if(ctx.takeLast()!=base.first.qualifiedName().join("::")){
                        ctx.append(base.first.qualifiedName().join("::"));

                        TypeInfo qualified = qualifyType(type, ctx);
                        if (qualified != type)
                            return qualified;
                    }
                }
            }

            QStringList copy = context;
            copy.removeLast();
            return qualifyType(type, copy);
        }
    }
}

void Binder::updateItemPosition(CodeModelItem item, AST *node) {
    QString filename;
    int line, column;

    assert(node != nullptr);
    _M_location.positionAt(_M_token_stream->position(node->start_token), &line, &column, &filename);
    item->setFileName(filename);
    item->setRequiredFeatures(_M_location.requiredFeatures()[filename]);
}

void Binder::installMessageHandler(MessageHandler handler) {
    _M_message_handler = handler;
}
