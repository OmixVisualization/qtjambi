/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

Binder::Binder(CodeModel *__model, LocationManager &__location, Control *__control)
        : _M_model(__model),
        _M_location(__location),
        _M_token_stream(&_M_location.token_stream),
        _M_control(__control),
        _M_current_function_type(CodeModel::Normal),
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

FileModelItem Binder::run(AST *node) {
    FileModelItem old = _M_current_file;
    _M_current_access = CodeModel::Public;

    _M_current_file = model()->create<FileModelItem>();
    updateItemPosition(_M_current_file->toItem(), node);
    visit(node);
    FileModelItem result = _M_current_file;

    _M_current_file = old; // restore

    return result;
}

ScopeModelItem Binder::currentScope() {
    if (_M_current_class)
        return model_static_cast<ScopeModelItem>(_M_current_class);
    else if (_M_current_namespace)
        return model_static_cast<ScopeModelItem>(_M_current_namespace);

    return model_static_cast<ScopeModelItem>(_M_current_file);
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
    if (! symbolScope) {
        name_cc.run(id);
        _M_message_handler(std::string("scope not found for symbol: ") + qPrintable(name_cc.name()));
        return;
    }

    decl_cc.run(declarator);

    if (decl_cc.isFunction()) {
        name_cc.run(id->unqualified_name);

        FunctionModelItem fun = model()->create<FunctionModelItem>();
        updateItemPosition(fun->toItem(), node);
        fun->setAccessPolicy(_M_current_access);
        fun->setFunctionType(_M_current_function_type);
        fun->setName(name_cc.name());
        fun->setAbstract(false);
        if(init_declarator->initializer){
            if(init_declarator->initializer->initializer_clause
                    && init_declarator->initializer->initializer_clause->expression){
                if(init_declarator->initializer->initializer_clause->expression->kind==AST::Kind_DeclDeleteAST){
                    fun->setAccessPolicy(CodeModel::Private);
                }else if(init_declarator->initializer->initializer_clause->expression->kind==AST::Kind_DeclDefaultAST){
                    // do nothing
                }else{
                    fun->setAbstract(true);
                }
            }else{
                fun->setAbstract(true);
            }
        }
        fun->setConstant(declarator->fun_cv != nullptr);
        fun->setDeclFinal(declarator->decl_final != nullptr);
        fun->setTemplateParameters(_M_current_template_parameters);
        applyStorageSpecifiers(node->storage_specifiers, model_static_cast<MemberModelItem>(fun));
        applyFunctionSpecifiers(node->function_specifiers, fun);
        if(fun->isDeprecated() && node->deprecationComment){
            fun->setDeprecatedComment(node->deprecationComment->toString(tokenStream()));
        }

        // build the type
        TypeInfo typeInfo = CompilerUtils::typeDescription(node->type_specifier,
                            declarator,
                            this);

        fun->setType(qualifyType(typeInfo, symbolScope->qualifiedName()));


        fun->setVariadics(decl_cc.isVariadics());

        // ... and the signature
        for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()) {
            ArgumentModelItem arg = model()->create<ArgumentModelItem>();
            TypeInfo tti = qualifyType(p.type, _M_context);

            /*
            This is trying to work out a parameter that is an enum, that uses
            QFlags<> and has isConstant() and isReference() set so
            we can revoke the isReference().  This was needed to get the
            method QScriptValue::setProperty() to emit code, this is the only
            part of the Qt API to date to need this.

            QScriptValue::PropertyFlag   QScriptValue::ResolveFlag
            typedef QFlag<QScriptValue::PropertyFlag> QScriptValue::PropertyFlags

            Revoking the isReference() does not seem to matter since the
            C++ code emitter already creates a new instance in order to pass-by-value.
            In order to implement "const&" in Java it would need to provide an read-only
            encapsulated version of such objects, which has no setters methods.
            */
            CodeModelItem scope = model()->findItem(_M_context, _M_current_file->toItem());
            if(ClassModelItem class_scope = model_dynamic_cast<ClassModelItem> (scope)) {

                // found the class that is the: typedef QFlag<QScriptValue::PropertyFlag> QScriptValue::PropertyFlags
                QStringList enumQualifiedName(tti.qualifiedName());
                QString last = enumQualifiedName.takeLast();
                int lastCharOffset = last.length() - 1;

                if(lastCharOffset >= 0) {
                    const QChar lastChar = last.at(lastCharOffset);
                    if(lastChar == QChar('s')) {
                        // We're editing the last part of the qualifiedName to remove
                        // any trailing 's' character, this the convention used in QFlags typedefs.
                        last = last.left(lastCharOffset);
                        enumQualifiedName.append(last);

                        CodeModelItem enum_scope = model()->findItem(enumQualifiedName, _M_current_file->toItem());
                        if(EnumModelItem klass = model_dynamic_cast<EnumModelItem> (enum_scope)) {
                            if(tti.isConstant() && tti.getReferenceType()==TypeInfo::Reference) {
                                tti.setReferenceType(TypeInfo::NoReference);
                                tti.setConstant(false);
                            } else if(tti.getReferenceType()==TypeInfo::Reference) {
                                qDebug() << "CHECKME: " << tti.toString() << " " << tti.qualifiedName() <<
                                    " " << enumQualifiedName << "; isReference=true";
                            } else if(tti.getReferenceType()==TypeInfo::RReference) {
                                qDebug() << "CHECKME: " << tti.toString() << " " << tti.qualifiedName() <<
                                    " " << enumQualifiedName << "; isRValueReference=true";
                            } else if(tti.isConstant()) {
                                qDebug() << "CHECKME: " << tti.toString() << " " << tti.qualifiedName() <<
                                    " " << enumQualifiedName << "; isConstant=true";
                            }
                        }
                    }
                }
            }

            // End of code for QScriptValue::setProperty(... const QScriptValue::PropertyFlag & ...) support
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
        var->setTemplateParameters(_M_current_template_parameters);
        var->setAccessPolicy(_M_current_access);
        name_cc.run(id->unqualified_name);
        var->setName(name_cc.name());
        TypeInfo typeInfo = CompilerUtils::typeDescription(node->type_specifier,
                            declarator,
                            this);
        if (declarator != init_declarator->declarator
                && init_declarator->declarator->parameter_declaration_clause != nullptr) {
            typeInfo.setFunctionPointer(true);
            decl_cc.run(init_declarator->declarator);
            for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()){
                typeInfo.addArgument(p.type);
                typeInfo.addArgumentName(p.name);
            }
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
    if (_M_message_handler && ! functionScope) {
        name_cc.run(declarator->id);
        _M_message_handler(std::string("scope not found for function definition: '")
                           + qPrintable(name_cc.name())
                           + std::string("' - definition *ignored*"));
        return;
    }

    decl_cc.run(declarator);

    Q_ASSERT(! decl_cc.id().isEmpty());

    FunctionDefinitionModelItem
    old = changeCurrentFunction(_M_model->create<FunctionDefinitionModelItem>());
    _M_current_function->setScope(functionScope->qualifiedName());
    updateItemPosition(_M_current_function->toItem(), node);

    Q_ASSERT(declarator->id->unqualified_name);
    name_cc.run(declarator->id->unqualified_name);
    QString unqualified_name = name_cc.name();

    _M_current_function->setName(unqualified_name);
    TypeInfo tmp_type = CompilerUtils::typeDescription(node->type_specifier,
                        declarator, this);

    _M_current_function->setType(qualifyType(tmp_type, _M_context));
    _M_current_function->setAccessPolicy(_M_current_access);
    _M_current_function->setFunctionType(_M_current_function_type);
    _M_current_function->setConstant(declarator->fun_cv != nullptr);
    _M_current_function->setTemplateParameters(_M_current_template_parameters);
    _M_current_function->setDeclFinal(declarator->decl_final != nullptr);

    applyStorageSpecifiers(node->storage_specifiers,
                           model_static_cast<MemberModelItem>(_M_current_function));
    applyFunctionSpecifiers(node->function_specifiers,
                            model_static_cast<FunctionModelItem>(_M_current_function));
    if(_M_current_function->isDeprecated() && node->deprecationComment){
        _M_current_function->setDeprecatedComment(node->deprecationComment->toString(tokenStream()));
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
    if (!it)
        return;

    TemplateParameterList savedTemplateParameters = changeTemplateParameters(TemplateParameterList());

    it = it->toFront();
    const ListNode<TemplateParameterAST*> *end = it;

    TemplateParameterList templateParameters;
    do {
        TemplateParameterAST *parameter = it->element;
        TypeParameterAST *type_parameter = parameter->type_parameter;

        NameAST *name;
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

            name = parameter->parameter_declaration->declarator->id;
        } else {
            int tk = decode_token(type_parameter->type);
            if (tk != Token_typename && tk != Token_class) {
                changeTemplateParameters(savedTemplateParameters);
                return;
            }
            assert(tk == Token_typename || tk == Token_class);

            name = type_parameter->name;
        }

        TemplateParameterModelItem p = model()->create<TemplateParameterModelItem>();
        name_cc.run(name);
        p->setName(name_cc.name());

        _M_current_template_parameters.append(p);
        it = it->next;
    } while (it != end);

    visit(node->declaration);

    changeTemplateParameters(savedTemplateParameters);
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

        if (decl != init_declarator->declarator
                && init_declarator->declarator->parameter_declaration_clause != nullptr) {
            typeInfo.setFunctionPointer(true);
            decl_cc.run(init_declarator->declarator);
            for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()){
                typeInfo.addArgument(p.type);
                typeInfo.addArgumentName(p.name);
            }
        }

        ScopeModelItem scope = currentScope();
        DeclaratorAST *declarator = init_declarator->declarator;
        CodeModelFinder finder(model(), this);
        ScopeModelItem typedefScope = finder.resolveScope(declarator->id, scope);

        TypeAliasModelItem typeAlias = model()->create<TypeAliasModelItem> ();
        updateItemPosition(typeAlias->toItem(), node);
        typeAlias->setName(alias_name);
        typeAlias->setType(qualifyType(typeInfo, currentScope()->qualifiedName()));
        typeAlias->setScope(typedefScope->qualifiedName());
        _M_qualified_types[typeAlias->qualifiedName().join(".")] = QString();
        currentScope()->addTypeAlias(typeAlias);
    } while (it != end);
}

void Binder::visitNamespace(NamespaceAST *node) {
    bool anonymous = (node->namespace_name == 0);

    ScopeModelItem scope = currentScope();

    NamespaceModelItem old;
    if (! anonymous) {
        QString name = decode_symbol(node->namespace_name)->as_string();

        QStringList qualified_name = scope->qualifiedName();
        qualified_name += name;
        NamespaceModelItem ns =
            model_safe_cast<NamespaceModelItem>(_M_model->findItem(qualified_name,
                                                _M_current_file->toItem()));
        if (!ns) {
            ns = _M_model->create<NamespaceModelItem>();
            updateItemPosition(ns->toItem(), node);
            ns->setName(name);
            ns->setScope(scope->qualifiedName());
        }
        old = changeCurrentNamespace(ns);

        _M_context.append(name);
    }

    DefaultVisitor::visitNamespace(node);

    if (! anonymous) {
        Q_ASSERT(scope->kind() == _CodeModelItem::Kind_Namespace
                 || scope->kind() == _CodeModelItem::Kind_File);

        _M_context.removeLast();

        if (NamespaceModelItem ns = model_static_cast<NamespaceModelItem>(scope)) {
            ns->addNamespace(_M_current_namespace);
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

void Binder::visitClassSpecifier(ClassSpecifierAST *node) {
    ClassCompiler class_cc(this);
    class_cc.run(node);

    if (class_cc.name().isEmpty()) {
        // anonymous not supported
        return;
    }

    Q_ASSERT(node->name != nullptr && node->name->unqualified_name != nullptr);

    ScopeModelItem scope = currentScope();

    ClassModelItem old = changeCurrentClass(_M_model->create<ClassModelItem>());
    updateItemPosition(_M_current_class->toItem(), node);
    _M_current_class->setName(class_cc.name());
    _M_current_class->setDeclFinal(node->is_final);
    _M_current_class->setDeclDeprecated(node->is_deprecated);
    if(node->deprecationComment)
        _M_current_class->setDeclDeprecatedComment(node->deprecationComment->toString(tokenStream()));

    QStringList baseClasses = class_cc.baseClasses(); TypeInfo info;
    for (int i = 0; i < baseClasses.size(); ++i) {
        info.setQualifiedName(baseClasses.at(i).split("::"));
        baseClasses[i] = qualifyType(info, scope->qualifiedName()).qualifiedName().join("::");
    }

    _M_current_class->setBaseClasses(baseClasses);
    _M_current_class->setClassType(decode_class_type(node->class_key));
    _M_current_class->setTemplateParameters(_M_current_template_parameters);
    //_M_current_class->setIsGadget(node->);

    if (! _M_current_template_parameters.isEmpty()) {
        QString name = _M_current_class->name();
        name += "<";
        for (int i = 0; i < _M_current_template_parameters.size(); ++i) {
            if (i != 0)
                name += ",";

            name += _M_current_template_parameters.at(i)->name();
        }

        name += ">";
        _M_current_class->setName(name);
    }

    CodeModel::AccessPolicy oldAccessPolicy = changeCurrentAccess(decode_access_policy(node->class_key));
    CodeModel::FunctionType oldFunctionType = changeCurrentFunctionType(CodeModel::Normal);

    _M_current_class->setScope(scope->qualifiedName());
    _M_qualified_types[_M_current_class->qualifiedName().join(".")] = QString();

    scope->addClass(_M_current_class);

    name_cc.run(node->name->unqualified_name);
    _M_context.append(name_cc.name());
    visitNodes(this, node->member_specs);
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

        if (decl != node->declarator
                && node->declarator->parameter_declaration_clause != nullptr) {
            typeInfo.setFunctionPointer(true);
            decl_cc.run(node->declarator);
            for(const DeclaratorCompiler::Parameter& p : decl_cc.parameters()){
                typeInfo.addArgument(p.type);
                typeInfo.addArgumentName(p.name);
            }
        }

        ScopeModelItem scope = currentScope();
        DeclaratorAST *declarator = node->declarator;
        CodeModelFinder finder(model(), this);
        ScopeModelItem typedefScope = finder.resolveScope(declarator->id, scope);

        TypeAliasModelItem typeAlias = model()->create<TypeAliasModelItem> ();
        updateItemPosition(typeAlias->toItem(), node);
        typeAlias->setName(alias_name);
        typeAlias->setType(qualifyType(typeInfo, currentScope()->qualifiedName()));
        typeAlias->setScope(typedefScope->qualifiedName());
        _M_qualified_types[typeAlias->qualifiedName().join(".")] = QString();
        currentScope()->addTypeAlias(typeAlias);
        //visitNodes(this, node->template_arguments);
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
        TypeAliasModelItem alias = scope->typeAliasMap()[baseType.qualifiedName().join("::")];
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
    e->setDeprecated(node->isDeprecated);
    if(node->deprecationComment)
        e->setDeprecatedComment(node->deprecationComment->toString(tokenStream()));

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

void Binder::visitQEnums(QEnumsAST *node) {
    const Token &start = _M_token_stream->token(node->start_token);
    const Token &end = _M_token_stream->token(node->end_token);
    QStringList enum_list = QString::fromLatin1(start.text + start.position,
                            int(end.position - start.position)).split(' ');

    ScopeModelItem scope = currentScope();
    for (int i = 0; i < enum_list.size(); ++i)
        scope->addEnumsDeclaration(enum_list.at(i));
}

void Binder::visitQEnum(QEnumAST *node) {
    const Token &start = _M_token_stream->token(node->start_token);
    const Token &end = _M_token_stream->token(node->end_token);
    QStringList enum_list = QString::fromLatin1(start.text + start.position,
                            int(end.position - start.position)).split(' ');

    ScopeModelItem scope = currentScope();
    for (int i = 0; i < enum_list.size(); ++i)
        scope->addEnumsDeclaration(enum_list.at(i));
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

TypeInfo Binder::qualifyType(const TypeInfo &type, const QStringList &context) const {
    // ### Potentially improve to use string list in the name table to
    if (context.size() == 0) {
        // ### We can assume that this means global namespace for now...
        return type;
    } else if (_M_qualified_types.contains(type.qualifiedName().join("."))) {
        return type;
    } else {
        QStringList expanded = context;
        expanded << type.qualifiedName();
        if (_M_qualified_types.contains(expanded.join("."))) {
            TypeInfo modified_type = type;
            modified_type.setQualifiedName(expanded);
            return modified_type;
        } else {
            CodeModelItem scope = model()->findItem(context, _M_current_file->toItem());

            if (ClassModelItem klass = model_dynamic_cast<ClassModelItem> (scope)) {
                for(const QString& base : klass->baseClasses()) {
                    QStringList ctx = context;
                    if(ctx.takeLast()!=base){
                        ctx.append(base);

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
