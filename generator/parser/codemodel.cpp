/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2002-2005 Roberto Raggi <roberto@kdevelop.org>
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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


#include "codemodel.h"

// ---------------------------------------------------------------------------

void reduceStringLiteral(QString& strg) {
    if(strg.startsWith("\"") && strg.endsWith("\"")){
        strg = strg.mid(1);
        strg.chop(1);
        strg.replace("\\\\", "\\");
        strg.replace("\\\"", "\"");
        strg.replace("\\t", "\t");
        strg.replace("\\n", "\n");
        strg.replace("\\r", "\r");
        strg.replace("\\b", "\b");
        strg.replace("\\f", "\f");
    }else if(strg.startsWith("u8\"") && strg.endsWith("\"")){
        strg = strg.mid(3);
        strg.chop(1);
        strg.replace("\\\\", "\\");
        strg.replace("\\\"", "\"");
        strg.replace("\\t", "\t");
        strg.replace("\\n", "\n");
        strg.replace("\\r", "\r");
        strg.replace("\\b", "\b");
        strg.replace("\\f", "\f");
    }else if(strg.startsWith("R\"(") && strg.endsWith(")\"")){
        strg = strg.mid(3);
        strg.chop(2);
    }else if((strg.startsWith("u\"")
        || strg.startsWith("U\"")
        || strg.startsWith("L\"")) && strg.endsWith("\"")){
        strg = strg.mid(2);
        strg.chop(1);
        strg.replace("\\\\", "\\");
        strg.replace("\\\"", "\"");
        strg.replace("\\t", "\t");
        strg.replace("\\n", "\n");
        strg.replace("\\r", "\r");
        strg.replace("\\b", "\b");
        strg.replace("\\f", "\f");
    }
}


CodeModel::CodeModel()
        : _M_creation_id(0) {
    _M_globalNamespace = create<NamespaceModelItem>();
}

CodeModel::~CodeModel() {
}

void CodeModel::wipeout() {
    _M_globalNamespace = create<NamespaceModelItem>();
    _M_files.clear();
}

FileList CodeModel::files() const {
    return _M_files.values();
}

NamespaceModelItem CodeModel::globalNamespace() const {
    return _M_globalNamespace;
}

void CodeModel::addFile(FileModelItem item) {
    _M_creation_id = 0; // reset the creation id
    _M_files.insert(item->name(), item);
}

void CodeModel::removeFile(FileModelItem item) {
    QHash<QString, FileModelItem>::Iterator it = _M_files.find(item->name());

    if (it != _M_files.end() && it.value() == item)
        _M_files.erase(it);
}

FileModelItem CodeModel::findFile(const QString &name) const {
    return _M_files.value(name);
}

const QHash<QString, FileModelItem>& CodeModel::fileMap() const {
    return _M_files;
}

CodeModelItem CodeModel::findItem(const QStringList &qualifiedName, CodeModelItem scope) const {
    for (int i = 0; i < qualifiedName.size(); ++i) {
        // ### Extend to look for members etc too.
        const QString &name = qualifiedName.at(i);

        if (NamespaceModelItem ns = model_dynamic_cast<NamespaceModelItem>(scope)) {
            if (NamespaceModelItem tmp_ns = ns->findNamespace(name)) {
                scope = tmp_ns;
                continue;
            }
        }

        if (ScopeModelItem ss = model_dynamic_cast<ScopeModelItem>(scope)) {
            if (ClassModelItem cs = ss->findClass(name)) {
                scope = cs;
            } else if (EnumModelItem es = ss->findEnum(name)) {
                if (i == qualifiedName.size() - 1)
                    return es->toItem();
            } else if (TypeAliasModelItem tp = ss->findTypeAlias(name)) {
                if (i == qualifiedName.size() - 1)
                    return tp->toItem();
            } else {
                // If we don't find the name in the scope chain we
                // need to return an empty item to indicate failure...
                return CodeModelItem();
            }
        }
    }

    return scope;
}


// ---------------------------------------------------------------------------
TypeInfo TypeInfo::combine(const TypeInfo &__lhs, const TypeInfo &__rhs) {
    TypeInfo __result = __lhs;

    __result.setConstant(__result.isConstant() || __rhs.isConstant());
    __result.setVolatile(__result.isVolatile() || __rhs.isVolatile());
    if(__result.getReferenceType()!=TypeInfo::NoReference || __rhs.getReferenceType()!=TypeInfo::NoReference){
        if(__result.getReferenceType()==TypeInfo::RReference || __rhs.getReferenceType()==TypeInfo::RReference){
            __result.setReferenceType(TypeInfo::RReference);
            __result.setConstant(false);
        }else{
            __result.setReferenceType(TypeInfo::Reference);
        }
    }
    __result.setIndirections(__result.indirections() + __rhs.indirections());
    __result.setArrayElements(__result.arrayElements() + __rhs.arrayElements());

    return __result;
}

TypeInfo TypeInfo::resolveType(TypeInfo const &__type, CodeModelItem __scope) {
    CodeModel *__model = __scope->model();
    Q_ASSERT(__model);

    CodeModelItem __item = __model->findItem(__type.qualifiedName(), __scope);

    // Copy the type and replace with the proper qualified name. This
    // only makes sence to do if we're actually getting a resolved
    // type with a namespace. We only get this if the returned type
    // has more than 2 entries in the qualified name... This test
    // could be improved by returning if the type was found or not.
    TypeInfo otherType(__type);
    if (__item && __item->qualifiedName().size() > 1) {
        otherType.setQualifiedName(__item->qualifiedName());
    }

    if (TypeAliasModelItem __alias = model_dynamic_cast<TypeAliasModelItem> (__item)){
        otherType = resolveType(TypeInfo::combine(__alias->type(), otherType), __scope);
    }

    return otherType;
}

QString TypeInfo::toString() const {
    QString tmp;

    if (isConstant())
        tmp += QLatin1String("const ");

    if (isVolatile())
        tmp += QLatin1String("volatile ");

    tmp += m_qualifiedName.join("::");

    for (int i=0; i<indirections().size(); i++){
        if(indirections()[i]){
            tmp += QLatin1String("*const");
        }else{
            tmp += QLatin1Char('*');
        }
    }
    if (getReferenceType()==TypeInfo::Reference)
        tmp += QLatin1Char('&');
    if (getReferenceType()==TypeInfo::RReference)
        tmp += QLatin1String("&&");

    if (isFunctionPointer()) {
        tmp += QLatin1String(" (*)(");
        for (int i = 0; i < m_arguments.count(); ++i) {
            if (i != 0)
                tmp += QLatin1String(", ");

            tmp += m_arguments.at(i).toString();
        }
        tmp += QLatin1String(")");
    }

    for(const QString& elt : arrayElements()) {
        tmp += QLatin1String("[");
        tmp += elt;
        tmp += QLatin1String("]");
    }

    return tmp;
}

void TypeInfo::setQualifiedName(const QStringList &qualified_name) {
    m_qualifiedName = qualified_name;
}

bool TypeInfo::operator==(const TypeInfo &other) const {
    if (arrayElements().count() != other.arguments().count())
        return false;

#if defined (RXX_CHECK_ARRAY_ELEMENTS) // ### it'll break
    for (int i = 0; i < arrayElements().count(); ++i) {
        QString elt1 = arrayElements().at(i).trimmed();
        QString elt2 = other.arrayElements().at(i).trimmed();

        if (elt1 != elt2)
            return false;
    }
#endif

    return m_flags == other.m_flags
           && m_qualifiedName == other.m_qualifiedName
           && (!isFunctionPointer() || m_arguments == other.m_arguments);
}

// ---------------------------------------------------------------------------
_CodeModelItem::_CodeModelItem(CodeModel *model, int kind)
        : _M_model(model),
        _M_kind(kind),
        _M_startLine(0),
        _M_startColumn(0),
        _M_endLine(0),
        _M_endColumn(0),
        _M_creation_id(0) {
}

_CodeModelItem::~_CodeModelItem() {
}

CodeModelItem _CodeModelItem::toItem() const {
    return CodeModelItem(const_cast<_CodeModelItem*>(this));
}

int _CodeModelItem::kind() const {
    return _M_kind;
}

void _CodeModelItem::setKind(int kind) {
    _M_kind = kind;
}

QStringList _CodeModelItem::qualifiedName() const {
    QStringList q = scope();

    if (!name().isEmpty())
        q += name();

    return q;
}

const QString& _CodeModelItem::name() const {
    return _M_name;
}

void _CodeModelItem::setName(const QString &name) {
    _M_name = name;
}

bool _ClassModelItem::isDeclFinal() const {
    return _M_declFinal;
}

void _ClassModelItem::setDeclFinal(bool declFinal) {
    _M_declFinal = declFinal;
}

const QString& _ClassModelItem::declDeprecatedComment() const {
    return _M_declDeprecatedComment;
}

void _ClassModelItem::setDeclDeprecatedComment(const QString& declDeprecatedComment) {
    _M_declDeprecatedComment = declDeprecatedComment;
    reduceStringLiteral(_M_declDeprecatedComment);
}

bool _ClassModelItem::isDeclDeprecated() const {
    return _M_declDeprecated;
}

void _ClassModelItem::setDeclDeprecated(bool declDeprecated) {
    _M_declDeprecated = declDeprecated;
}

CodeModel::AccessPolicy _ClassModelItem::accessPolicy() const {
    return _M_accessPolicy;
}

void _ClassModelItem::setAccessPolicy(CodeModel::AccessPolicy accessPolicy) {
    _M_accessPolicy = accessPolicy;
}

const QStringList& _CodeModelItem::scope() const {
    return _M_scope;
}

void _CodeModelItem::setScope(const QStringList &scope) {
    _M_scope = scope;
}

const QString& _CodeModelItem::fileName() const {
    return _M_fileName;
}

void _CodeModelItem::setFileName(const QString &fileName) {
    _M_fileName = fileName;
}

const QStringList& _CodeModelItem::requiredFeatures() const {
    return _M_requiredFeatures;
}

void _CodeModelItem::setRequiredFeatures(const QStringList &features) {
    for(const QString& feature : features){
        if(!feature.isEmpty())
            _M_requiredFeatures << feature;
    }
}

FileModelItem _CodeModelItem::file() const {
    return model()->findFile(fileName());
}

void _CodeModelItem::getStartPosition(int *line, int *column) {
    *line = _M_startLine;
    *column = _M_startColumn;
}

void _CodeModelItem::setStartPosition(int line, int column) {
    _M_startLine = line;
    _M_startColumn = column;
}

void _CodeModelItem::getEndPosition(int *line, int *column) {
    *line = _M_endLine;
    *column = _M_endColumn;
}

void _CodeModelItem::setEndPosition(int line, int column) {
    _M_endLine = line;
    _M_endColumn = column;
}

// ---------------------------------------------------------------------------
const QList<QPair<QString,bool>>& _ClassModelItem::baseClasses() const {
    return _M_baseClasses;
}

void _ClassModelItem::setBaseClasses(const QList<QPair<QString,bool>> &baseClasses) {
    _M_baseClasses = baseClasses;
}

const TemplateParameterList& _ClassModelItem::templateParameters() const {
    return _M_templateParameters;
}

void _ClassModelItem::setTemplateParameters(const TemplateParameterList &templateParameters) {
    _M_templateParameters = templateParameters;
    for (int i = 0; i < _M_templateParameters.size(); ++i) {
        _M_templateParameters.at(i)->setOwnerClass(this);
    }
}

bool _ClassModelItem::extendsClass(const QString &name) const {
    return _M_baseClasses.contains({name,true}) || _M_baseClasses.contains({name,false});
}

void _ClassModelItem::setClassType(CodeModel::ClassType type) {
    _M_classType = type;
}

CodeModel::ClassType _ClassModelItem::classType() const {
    return _M_classType;
}

void _ClassModelItem::addPropertyDeclaration(const QString &propertyDeclaration) {
    _M_propertyDeclarations << propertyDeclaration;
}


// ---------------------------------------------------------------------------
FunctionModelItem _ScopeModelItem::declaredFunction(FunctionModelItem item) {
    FunctionList function_list = findFunctions(item->name());

    for(const FunctionModelItem& fun : function_list) {
        if (fun->isSimilar(item))
            return fun;
    }

    return FunctionModelItem();
}

ScopeModelItem _ScopeModelItem::toScope() const {
    return ScopeModelItem(const_cast<_ScopeModelItem*>(this));
}

TypeAliasList _ScopeModelItem::typeAliases() const {
    return _M_typeAliases.values();
}

VariableList _ScopeModelItem::variables() const {
    return _M_variables.values();
}

NamespaceModelItem _ScopeModelItem::findNamespace(const QString &) const
{
    return NamespaceModelItem();
}

FunctionList _ScopeModelItem::functions() const {
    return _M_functions.values();
}

void _ScopeModelItem::addEnumsDeclaration(const QString &enumsDeclaration) {
    _M_enumsDeclarations << enumsDeclaration;
}

FunctionDefinitionList _ScopeModelItem::functionDefinitions() const {
    return _M_functionDefinitions.values();
}

EnumList _ScopeModelItem::enums() const {
    return _M_enums.values();
}

void _ScopeModelItem::addClass(ClassModelItem item) {
    QString name = item->name();
    auto idx = name.indexOf("<");
    if (idx > 0)
        _M_classes.insert(name.left(idx), item);
    _M_classes.insert(name, item);
    _M_classList << item;
}

void _ScopeModelItem::addFunction(FunctionModelItem item) {
    _M_functions.insert(item->name(), item);
}

void _ScopeModelItem::addFunctionDefinition(FunctionDefinitionModelItem item) {
    _M_functionDefinitions.insert(item->name(), item);
}

void _ScopeModelItem::addVariable(VariableModelItem item) {
    _M_variables.insert(item->name(), item);
}

void _ScopeModelItem::addTypeAlias(TypeAliasModelItem item) {
    _M_typeAliases.insert(item->name(), item);
}

void _ScopeModelItem::addEnum(EnumModelItem item) {
    _M_enums.insert(item->name(), item);
}

void _ScopeModelItem::removeClass(ClassModelItem item) {
    QHash<QString, ClassModelItem>::Iterator it = _M_classes.find(item->name());

    if (it != _M_classes.end() && it.value() == item)
        _M_classes.erase(it);
    _M_classList.removeAll(item);
}

void _ScopeModelItem::removeFunction(FunctionModelItem item) {
    QMultiHash<QString, FunctionModelItem>::Iterator it = _M_functions.find(item->name());

    while (it != _M_functions.end() && it.key() == item->name()
            && it.value() != item) {
        ++it;
    }

    if (it != _M_functions.end() && it.value() == item) {
        _M_functions.erase(it);
    }
}

void _ScopeModelItem::removeFunctionDefinition(FunctionDefinitionModelItem item) {
    QMultiHash<QString, FunctionDefinitionModelItem>::Iterator it = _M_functionDefinitions.find(item->name());

    while (it != _M_functionDefinitions.end() && it.key() == item->name()
            && it.value() != item) {
        ++it;
    }

    if (it != _M_functionDefinitions.end() && it.value() == item) {
        _M_functionDefinitions.erase(it);
    }
}

void _ScopeModelItem::removeVariable(VariableModelItem item) {
    QHash<QString, VariableModelItem>::Iterator it = _M_variables.find(item->name());

    if (it != _M_variables.end() && it.value() == item)
        _M_variables.erase(it);
}

void _ScopeModelItem::removeTypeAlias(TypeAliasModelItem item) {
    QHash<QString, TypeAliasModelItem>::Iterator it = _M_typeAliases.find(item->name());

    if (it != _M_typeAliases.end() && it.value() == item)
        _M_typeAliases.erase(it);
}

void _ScopeModelItem::removeEnum(EnumModelItem item) {
    QHash<QString, EnumModelItem>::Iterator it = _M_enums.find(item->name());

    if (it != _M_enums.end() && it.value() == item)
        _M_enums.erase(it);
}

ClassModelItem _ScopeModelItem::findClass(const QString &name) const {
    return _M_classes.value(name);
}

VariableModelItem _ScopeModelItem::findVariable(const QString &name) const {
    return _M_variables.value(name);
}

TypeAliasModelItem _ScopeModelItem::findTypeAlias(const QString &name) const {
    return _M_typeAliases.value(name);
}

EnumModelItem _ScopeModelItem::findEnum(const QString &name) const {
    return _M_enums.value(name);
}

FunctionList _ScopeModelItem::findFunctions(const QString &name) const {
    return _M_functions.values(name);
}

FunctionDefinitionList _ScopeModelItem::findFunctionDefinitions(const QString &name) const {
    return _M_functionDefinitions.values(name);
}

// ---------------------------------------------------------------------------
NamespaceList _NamespaceModelItem::namespaces() const {
    return _M_namespaces.values();
}
void _NamespaceModelItem::addNamespace(NamespaceModelItem item) {
    _M_namespaces.insert(item->name(), item);
}
void _NamespaceModelItem::removeNamespace(NamespaceModelItem item) {
    QHash<QString, NamespaceModelItem>::Iterator it = _M_namespaces.find(item->name());

    if (it != _M_namespaces.end() && it.value() == item)
        _M_namespaces.erase(it);
}

NamespaceModelItem _NamespaceModelItem::findNamespace(const QString &name) const {
    return _M_namespaces.value(name);
}

// ---------------------------------------------------------------------------
const TypeInfo& _ArgumentModelItem::type() const {
    return _M_type;
}

void _ArgumentModelItem::setType(const TypeInfo &type) {
    _M_type = type;
}

bool _ArgumentModelItem::defaultValue() const {
    return _M_defaultValue;
}

void _ArgumentModelItem::setDefaultValue(bool defaultValue) {
    _M_defaultValue = defaultValue;
}

// ---------------------------------------------------------------------------
bool _FunctionModelItem::isSimilar(FunctionModelItem other) const {
    if (name() != other->name())
        return false;

    if (isConstant() != other->isConstant())
        return false;

    if (isVariadics() != other->isVariadics())
        return false;

    if (arguments().count() != other->arguments().count())
        return false;

    // ### check the template parameters

    for (int i = 0; i < arguments().count(); ++i) {
        ArgumentModelItem arg1 = arguments().at(i);
        ArgumentModelItem arg2 = other->arguments().at(i);

        if (arg1->type() != arg2->type())
            return false;
    }

    return true;
}

const ArgumentList& _FunctionModelItem::arguments() const {
    return _M_arguments;
}

void _FunctionModelItem::addArgument(ArgumentModelItem item) {
    /**
     * In Qt5 signals are made private by the fake type QPrivateSignal.
     * Here, in code analysis, do not add the fake parameter but
     * define signal to be private.
     */
    if(this->functionType()==CodeModel::Signal && item.data()->type().toString()=="QPrivateSignal"){
        setFunctionType(CodeModel::PrivateSignal);
    }else{
        _M_arguments.append(item);
    }
}

void _FunctionModelItem::removeArgument(ArgumentModelItem item) {
    _M_arguments.removeAt(_M_arguments.indexOf(item));
}

CodeModel::FunctionType _FunctionModelItem::functionType() const {
    return _M_functionType;
}

void _FunctionModelItem::setFunctionType(CodeModel::FunctionType functionType) {
    _M_functionType = functionType;
}

bool _FunctionModelItem::hasBody() const {
    return _M_flags.testFlag(HasBody);
}

void _FunctionModelItem::setHasBody(bool hasBody) {
    _M_flags.setFlag(HasBody, hasBody);
}

bool _FunctionModelItem::isDeleted() const {
    return _M_flags.testFlag(IsDeleted);
}

void _FunctionModelItem::setDeleted(bool d) {
    _M_flags.setFlag(IsDeleted, d);
}

bool _FunctionModelItem::isVariadics() const {
    return _M_flags.testFlag(IsVariadics);
}

void _FunctionModelItem::setVariadics(bool isVariadics) {
    _M_flags.setFlag(IsVariadics, isVariadics);
}

TypeInfo::ReferenceType _FunctionModelItem::referenceType() const {
    return _M_referenceType;
}

void _FunctionModelItem::setReferenceType(TypeInfo::ReferenceType referenceType) {
    _M_referenceType = referenceType;
}

bool _FunctionModelItem::isVirtual() const {
    return _M_flags.testFlag(IsVirtual);
}

void _FunctionModelItem::setVirtual(bool isVirtual) {
    _M_flags.setFlag(IsVirtual, isVirtual);
}

bool _FunctionModelItem::isInline() const {
    return _M_flags.testFlag(IsInline);
}

void _FunctionModelItem::setInline(bool isInline) {
    _M_flags.setFlag(IsInline, isInline);
}

bool _FunctionModelItem::isExplicit() const {
    return _M_flags.testFlag(IsExplicit);
}

void _FunctionModelItem::setExplicit(bool isExplicit) {
    _M_flags.setFlag(IsExplicit, isExplicit);
}

bool _FunctionModelItem::isAbstract() const {
    return _M_flags.testFlag(IsAbstract);
}

void _FunctionModelItem::setAbstract(bool isAbstract) {
    _M_flags.setFlag(IsAbstract, isAbstract);
}

bool _FunctionModelItem::isDeclFinal() const {
    return _M_flags.testFlag(IsDeclFinal);
}

void _FunctionModelItem::setDeclFinal(bool isDeclFinal) {
    _M_flags.setFlag(IsDeclFinal, isDeclFinal);
}

// Qt
bool _FunctionModelItem::isInvokable() const {
    return _M_flags.testFlag(IsInvokable);
}

void _FunctionModelItem::setInvokable(bool isInvokable) {
    _M_flags.setFlag(IsInvokable, isInvokable);
}

// ---------------------------------------------------------------------------
const TypeInfo& _TypeAliasModelItem::type() const {
    return _M_type;
}

void _TypeAliasModelItem::setType(const TypeInfo &type) {
    _M_type = type;
}

CodeModel::AccessPolicy _TypeAliasModelItem::accessPolicy() const {
    return _M_accessPolicy;
}

void _TypeAliasModelItem::setAccessPolicy(CodeModel::AccessPolicy accessPolicy) {
    _M_accessPolicy = accessPolicy;
}

// ---------------------------------------------------------------------------
CodeModel::AccessPolicy _EnumModelItem::accessPolicy() const {
    return _M_accessPolicy;
}

const TypeInfo& _EnumModelItem::baseType() const {
    return _M_baseType;
}

void _EnumModelItem::setBaseType(const TypeInfo &baseType) {
    _M_baseType = baseType;
}

void _EnumModelItem::setAccessPolicy(CodeModel::AccessPolicy accessPolicy) {
    _M_accessPolicy = accessPolicy;
}

const EnumeratorList& _EnumModelItem::enumerators() const {
    return _M_enumerators;
}

void _EnumModelItem::addEnumerator(EnumeratorModelItem item) {
    _M_enumerators.append(item);
}

void _EnumModelItem::removeEnumerator(EnumeratorModelItem item) {
    _M_enumerators.removeAt(_M_enumerators.indexOf(item));
}

// ---------------------------------------------------------------------------
const QString& _EnumeratorModelItem::value() const {
    return _M_value;
}

void _EnumeratorModelItem::setValue(const QString &value) {
    _M_value = value;
}

bool _EnumeratorModelItem::deprecated() const {
    return _M_deprecated;
}

void _EnumeratorModelItem::setDeprecated(bool value){
    _M_deprecated = value;
}

const QString& _EnumeratorModelItem::deprecatedComment() const {
    return _M_deprecatedComment;
}

void _EnumeratorModelItem::setDeprecatedComment(const QString& value){
    _M_deprecatedComment = value;
    reduceStringLiteral(_M_deprecatedComment);
}

// ---------------------------------------------------------------------------
const TypeInfo& _TemplateParameterModelItem::type() const {
    return _M_type;
}

void _TemplateParameterModelItem::setType(const TypeInfo &type) {
    _M_type = type;
}

ClassModelItem _TemplateParameterModelItem::ownerClass() const {
    return _M_ownerClass;
}

void _TemplateParameterModelItem::setOwnerClass(const ClassModelItem& ownerClass) {
    _M_ownerClass = ownerClass;
}

const QString& _TemplateParameterModelItem::defaultValue() const {
    return _M_defaultValue;
}

void _TemplateParameterModelItem::setDefaultValue(const QString& defaultValue) {
    _M_defaultValue = defaultValue;
}

// ---------------------------------------------------------------------------
ScopeModelItem _ScopeModelItem::create(CodeModel *model) {
    ScopeModelItem item(new _ScopeModelItem(model));
    return item;
}

ClassModelItem _ClassModelItem::create(CodeModel *model) {
    ClassModelItem item(new _ClassModelItem(model));
    return item;
}

NamespaceModelItem _NamespaceModelItem::create(CodeModel *model) {
    NamespaceModelItem item(new _NamespaceModelItem(model));
    return item;
}

FileModelItem _FileModelItem::create(CodeModel *model) {
    FileModelItem item(new _FileModelItem(model));
    return item;
}

ArgumentModelItem _ArgumentModelItem::create(CodeModel *model) {
    ArgumentModelItem item(new _ArgumentModelItem(model));
    return item;
}

FunctionModelItem _FunctionModelItem::create(CodeModel *model) {
    FunctionModelItem item(new _FunctionModelItem(model));
    return item;
}

FunctionDefinitionModelItem _FunctionDefinitionModelItem::create(CodeModel *model) {
    FunctionDefinitionModelItem item(new _FunctionDefinitionModelItem(model));
    return item;
}

VariableModelItem _VariableModelItem::create(CodeModel *model) {
    VariableModelItem item(new _VariableModelItem(model));
    return item;
}

TypeAliasModelItem _TypeAliasModelItem::create(CodeModel *model) {
    TypeAliasModelItem item(new _TypeAliasModelItem(model));
    return item;
}

EnumModelItem _EnumModelItem::create(CodeModel *model) {
    EnumModelItem item(new _EnumModelItem(model));
    return item;
}

EnumeratorModelItem _EnumeratorModelItem::create(CodeModel *model) {
    EnumeratorModelItem item(new _EnumeratorModelItem(model));
    return item;
}

TemplateParameterModelItem _TemplateParameterModelItem::create(CodeModel *model) {
    TemplateParameterModelItem item(new _TemplateParameterModelItem(model));
    return item;
}

// ---------------------------------------------------------------------------
const TypeInfo& _MemberModelItem::type() const {
    return _M_type;
}

void _MemberModelItem::setType(const TypeInfo &type) {
    _M_type = type;
}

CodeModel::AccessPolicy _MemberModelItem::accessPolicy() const {
    return _M_accessPolicy;
}

void _MemberModelItem::setAccessPolicy(CodeModel::AccessPolicy accessPolicy) {
    _M_accessPolicy = accessPolicy;
}

bool _MemberModelItem::isStatic() const {
    return _M_flags.testFlag(IsStatic);
}

void _MemberModelItem::setStatic(bool isStatic) {
    _M_flags.setFlag(IsStatic, isStatic);
}

bool _MemberModelItem::isConstant() const {
    return _M_flags.testFlag(IsConstant);
}

void _MemberModelItem::setConstant(bool isConstant) {
    _M_flags.setFlag(IsConstant, isConstant);
}

bool _MemberModelItem::isConstExpr() const {
    return _M_flags.testFlag(IsConstExpr);
}

void _MemberModelItem::setConstExpr(bool isConstExpr) {
    _M_flags.setFlag(IsConstExpr, isConstExpr);
}

bool _MemberModelItem::isVolatile() const {
    return _M_flags.testFlag(IsVolatile);
}

void _MemberModelItem::setVolatile(bool isVolatile) {
    _M_flags.setFlag(IsVolatile, isVolatile);
}

bool _MemberModelItem::isFriend() const {
    return _M_flags.testFlag(IsFriend);
}

void _MemberModelItem::setFriend(bool isFriend) {
    _M_flags.setFlag(IsFriend, isFriend);
}

bool _MemberModelItem::isRegister() const {
    return _M_flags.testFlag(IsRegister);
}

void _MemberModelItem::setRegister(bool isRegister) {
    _M_flags.setFlag(IsRegister, isRegister);
}

bool _MemberModelItem::isExtern() const {
    return _M_flags.testFlag(IsExtern);
}

void _MemberModelItem::setExtern(bool isExtern) {
    _M_flags.setFlag(IsExtern, isExtern);
}

bool _MemberModelItem::isMutable() const {
    return _M_flags.testFlag(IsMutable);
}

void _MemberModelItem::setMutable(bool isMutable) {
    _M_flags.setFlag(IsMutable, isMutable);
}

bool _MemberModelItem::isDeprecated() const {
    return _M_flags.testFlag(IsDeprecated);
}

void _MemberModelItem::setDeprecated(bool isDeprecated) {
    _M_flags.setFlag(IsDeprecated, isDeprecated);
}

const QString& _MemberModelItem::getDeprecatedComment() const {
    return _M_deprecatedComment;
}

void _MemberModelItem::setDeprecatedComment(const QString& comment) {
    _M_deprecatedComment = comment;
    reduceStringLiteral(_M_deprecatedComment);
}

// kate: space-indent on; indent-width 2; replace-tabs on;

