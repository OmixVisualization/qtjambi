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


#ifndef CODEMODEL_H
#define CODEMODEL_H

#include "codemodel_fwd.h"

#include <QtCore/QtCore>

#define DECLARE_MODEL_NODE(k) \
    enum { __node_kind = Kind_##k }; \
    typedef QExplicitlySharedDataPointer<k##ModelItem> Pointer;

template <class _Target, class _Source>
_Target model_static_cast(_Source item) {
    typedef typename _Target::Type * _Target_pointer;

    _Target ptr(static_cast<_Target_pointer>(item.data()));
    return ptr;
}

class CodeModel {
    public:
        enum AccessPolicy {
            Public,
            Protected,
            Private
        };

        enum FunctionType {
            Normal,
            Signal,
            PrivateSignal,
            Slot
        };

        enum ClassType {
            Class,
            Struct,
            Union
        };

    public:
        CodeModel();
        virtual ~CodeModel();

        template <class _Target> _Target create() {
            typedef typename _Target::Type _Target_type;

            _Target result = _Target_type::create(this);
            result->setCreationId(_M_creation_id++);
            return result;
        }

        FileList files() const;
        NamespaceModelItem globalNamespace() const;

        void addFile(FileModelItem item);
        void removeFile(FileModelItem item);
        FileModelItem findFile(const QString &name) const;
        const QMap<QString, FileModelItem>& fileMap() const;

        CodeModelItem findItem(const QStringList &qualifiedName, CodeModelItem scope) const;

        void wipeout();

    private:
        QMap<QString, FileModelItem> _M_files;
        NamespaceModelItem _M_globalNamespace;
        std::size_t _M_creation_id;

    private:
        CodeModel(const CodeModel &other);
        void operator = (const CodeModel &other);
};

class TypeInfo {
    public:
        enum ReferenceType{
            NoReference,
            Reference,
            RReference
        };

        TypeInfo(const TypeInfo &other) = default;
        TypeInfo &operator=(const TypeInfo &other) = default;
        TypeInfo(TypeInfo &&other) = default;
        TypeInfo &operator=(TypeInfo &&other) = default;

        TypeInfo():
                m_flags(), m_reference_type(NoReference) {}

        const QStringList& qualifiedName() const { return m_qualifiedName; }
        void setQualifiedName(const QStringList &qualified_name);

        bool isConstant() const { return m_flags.testFlag(IsConst); }
        void setConstant(bool is) { m_flags.setFlag(IsConst, is); }

        bool isVolatile() const { return m_flags.testFlag(IsVolatile); }
        void setVolatile(bool is) { m_flags.setFlag(IsVolatile, is); }

        ReferenceType getReferenceType() const { return m_reference_type; }
        void setReferenceType(ReferenceType reference_type) { m_reference_type = reference_type; }

        inline const QList<bool>& indirections() const { return m_indirections; }
        void setIndirections(const QList<bool> & indirections) { m_indirections = indirections; }

        bool isFunctionPointer() const { return m_flags.testFlag(IsFunctionPointer); }
        void setFunctionPointer(bool is) { m_flags.setFlag(IsFunctionPointer, is); }

        bool isVariadic() const { return m_flags.testFlag(IsVariadic); }
        void setVariadic(bool is) { m_flags.setFlag(IsVariadic, is); }

        const QStringList& arrayElements() const { return m_arrayElements; }
        void setArrayElements(const QStringList &arrayElements) { m_arrayElements = arrayElements; }

        const QList<TypeInfo>& arguments() const { return m_arguments; }
        void setArguments(const QList<TypeInfo> &arguments) { m_arguments = arguments; }
        void addArgument(const TypeInfo &arg) { m_arguments.append(arg); }

        const QList<QString>& argumentNames() const { return m_argumentNames; }
        void setArgumentNames(const QList<QString> &argumentNames) { m_argumentNames = argumentNames; }
        void addArgumentName(const QString &arg) { m_argumentNames.append(arg); }

        bool operator==(const TypeInfo &other) const;
        bool operator!=(const TypeInfo &other) const { return !(*this == other); }

        // ### arrays and templates??

        QString toString() const;

        static TypeInfo combine(const TypeInfo &__lhs, const TypeInfo &__rhs);
        static TypeInfo resolveType(TypeInfo const &__type, CodeModelItem __scope);
        TypeInfo functionalReturnType() const { return m_functionalReturnType.isEmpty() ? TypeInfo() : m_functionalReturnType.first(); }
        const QList<TypeInfo>& functionalArgumentTypes() const { return m_functionalArgumentTypes; }
        const QList<QString>& functionalArgumentNames() const { return m_functionalArgumentNames; }
        void setFunctionalReturnType(const TypeInfo& s) { m_functionalReturnType.clear(); if(!s.qualifiedName().isEmpty()) m_functionalReturnType << s; }
        void setFunctionalArgumentTypes(const QList<TypeInfo>& l) { m_functionalArgumentTypes = l; }
        void setFunctionalArgumentNames(const QList<QString>& l) { m_functionalArgumentNames = l; }

    private:
        enum Flag : quint8{
            None = 0x00,
            IsConst = 0x01,
            IsVolatile = 0x02,
            IsFunctionPointer = 0x04,
            IsVariadic = 0x08,
        };
        QFlags<Flag> m_flags;

        ReferenceType m_reference_type;
        /**
         * the size of this array represents the number of indirections of the type.
         * the individual entry 'false' it represents a *.
         * the individual entry 'true' it represents a * const.
         */
        QList<bool> m_indirections;
        QStringList m_qualifiedName;
        QStringList m_arrayElements;
        QList<TypeInfo> m_arguments;
        QList<QString> m_argumentNames;
        QList<TypeInfo> m_functionalReturnType;
        QList<TypeInfo> m_functionalArgumentTypes;
        QList<QString> m_functionalArgumentNames;
};

class _CodeModelItem: public QSharedData {
    public:
        enum Kind {
            /* These are bit-flags resembling inheritance */
            Kind_Scope = 0x1,
            Kind_Namespace = 0x2 | Kind_Scope,
            Kind_Member = 0x4,
            Kind_Function = 0x8 | Kind_Member,
            KindMask = 0xf,

            /* These are for classes that are not inherited from */
            FirstKind = 0x8,
            Kind_Argument = 1 << FirstKind,
            Kind_Class = 2 << FirstKind | Kind_Scope,
            Kind_Enum = 3 << FirstKind,
            Kind_Enumerator = 4 << FirstKind,
            Kind_File = 5 << FirstKind | Kind_Namespace,
            Kind_FunctionDefinition = 6 << FirstKind | Kind_Function,
            Kind_TemplateParameter = 7 << FirstKind,
            Kind_TypeAlias = 8 << FirstKind,
            Kind_Variable = 9 << FirstKind | Kind_Member
        };

    public:
        virtual ~_CodeModelItem();

        int kind() const;

        QStringList qualifiedName() const;

        const QString& name() const;
        void setName(const QString &name);

        const QStringList& scope() const;
        void setScope(const QStringList &scope);

        const QString& fileName() const;
        void setFileName(const QString &fileName);

        const QStringList& requiredFeatures() const;
        void setRequiredFeatures(const QStringList &features);

        FileModelItem file() const;

        void getStartPosition(int *line, int *column);
        void setStartPosition(int line, int column);

        void getEndPosition(int *line, int *column);
        void setEndPosition(int line, int column);

        inline std::size_t creationId() const { return _M_creation_id; }
        inline void setCreationId(std::size_t creation_id) { _M_creation_id = creation_id; }

        inline CodeModel *model() const { return _M_model; }

        CodeModelItem toItem() const;

    protected:
        _CodeModelItem(CodeModel *model, int kind);
        void setKind(int kind);

    private:
        CodeModel *_M_model;
        int _M_kind;
        int _M_startLine;
        int _M_startColumn;
        int _M_endLine;
        int _M_endColumn;
        std::size_t _M_creation_id;
        QString _M_name;
        QString _M_fileName;
        QStringList _M_scope;
        QStringList _M_requiredFeatures;
    private:
        _CodeModelItem(const _CodeModelItem &other);
        void operator = (const _CodeModelItem &other);
};

class _ScopeModelItem: public _CodeModelItem {
    public:
        DECLARE_MODEL_NODE(Scope)

        static ScopeModelItem create(CodeModel *model);

    public:
        ScopeModelItem toScope() const;
        EnumList enums() const;
        FunctionDefinitionList functionDefinitions() const;
        FunctionList functions() const;
        TypeAliasList typeAliases() const;
        VariableList variables() const;
        virtual NamespaceModelItem findNamespace(const QString &name) const;

        void addClass(ClassModelItem item);
        void addEnum(EnumModelItem item);
        void addFunction(FunctionModelItem item);
        void addFunctionDefinition(FunctionDefinitionModelItem item);
        void addTypeAlias(TypeAliasModelItem item);
        void addVariable(VariableModelItem item);

        void removeClass(ClassModelItem item);
        void removeEnum(EnumModelItem item);
        void removeFunction(FunctionModelItem item);
        void removeFunctionDefinition(FunctionDefinitionModelItem item);
        void removeTypeAlias(TypeAliasModelItem item);
        void removeVariable(VariableModelItem item);

        ClassModelItem findClass(const QString &name) const;
        EnumModelItem findEnum(const QString &name) const;
        FunctionDefinitionList findFunctionDefinitions(const QString &name) const;
        FunctionList findFunctions(const QString &name) const;
        TypeAliasModelItem findTypeAlias(const QString &name) const;
        VariableModelItem findVariable(const QString &name) const;

        inline const QList<ClassModelItem>& classes() const { return _M_classList; }
        inline const QMap<QString, EnumModelItem>& enumMap() const { return _M_enums; }
        inline const QMap<QString, TypeAliasModelItem>& typeAliasMap() const { return _M_typeAliases; }
        inline const QMap<QString, VariableModelItem>& variableMap() const { return _M_variables; }
        inline const QMultiHash<QString, FunctionDefinitionModelItem>& functionDefinitionMap() const { return _M_functionDefinitions; }
        inline const QMultiHash<QString, FunctionModelItem>& functionMap() const { return _M_functions; }

        FunctionModelItem declaredFunction(FunctionModelItem item);

    protected:
        _ScopeModelItem(CodeModel *model, int kind = __node_kind)
                : _CodeModelItem(model, kind) {}

    private:
        QList<ClassModelItem> _M_classList;
        QMap<QString, ClassModelItem> _M_classes;
        QMap<QString, EnumModelItem> _M_enums;
        QMap<QString, TypeAliasModelItem> _M_typeAliases;
        QMap<QString, VariableModelItem> _M_variables;
        QMultiHash<QString, FunctionDefinitionModelItem> _M_functionDefinitions;
        QMultiHash<QString, FunctionModelItem> _M_functions;

    private:
        _ScopeModelItem(const _ScopeModelItem &other);
        void operator = (const _ScopeModelItem &other);
};

class _ClassModelItem: public _ScopeModelItem {
    public:
        DECLARE_MODEL_NODE(Class)

        static ClassModelItem create(CodeModel *model);

    public:
        const QList<QPair<QString,bool>>& baseClasses() const;

        void setBaseClasses(const QList<QPair<QString,bool>> &baseClasses);

        const TemplateParameterList& templateParameters() const;
        void setTemplateParameters(const TemplateParameterList &templateParameters);

        bool extendsClass(const QString &name) const;

        void setClassType(CodeModel::ClassType type);
        CodeModel::ClassType classType() const;

        void addPropertyDeclaration(const QString &propertyDeclaration);
        const QStringList& propertyDeclarations() const { return _M_propertyDeclarations; }

        void setHas_Q_GADGET(bool has_Q_GADGET){m_has_Q_GADGET = has_Q_GADGET;}
        bool has_Q_GADGET() const {return m_has_Q_GADGET;}

        void setHas_Q_OBJECT(bool has_Q_OBJECT){m_has_Q_OBJECT = has_Q_OBJECT;}
        bool has_Q_OBJECT() const {return m_has_Q_OBJECT;}

        void setUsingBaseConstructors(CodeModel::AccessPolicy usingBaseConstructors){_M_usingBaseConstructors = usingBaseConstructors;}
        CodeModel::AccessPolicy usingBaseConstructors() const {return _M_usingBaseConstructors;}
        bool isDeclFinal() const;
        void setDeclFinal(bool declFinal);
        bool isDeclDeprecated() const;
        void setDeclDeprecated(bool declDeprecated);
        const QString& declDeprecatedComment() const;
        void setDeclDeprecatedComment(const QString& comment);
        CodeModel::AccessPolicy accessPolicy() const;
        void setAccessPolicy(CodeModel::AccessPolicy accessPolicy);
    protected:
        _ClassModelItem(CodeModel *model, int kind = __node_kind)
                : _ScopeModelItem(model, kind), m_has_Q_GADGET(false), m_has_Q_OBJECT(false), _M_classType(CodeModel::Class),
                  _M_declFinal(false), _M_declDeprecated(false),
                  _M_accessPolicy(CodeModel::Public),
                  _M_usingBaseConstructors(CodeModel::AccessPolicy::Private){}

    private:
        bool m_has_Q_GADGET;
        bool m_has_Q_OBJECT;
        QList<QPair<QString,bool>> _M_baseClasses;
        TemplateParameterList _M_templateParameters;
        CodeModel::ClassType _M_classType;
        QStringList _M_propertyDeclarations;
        bool _M_declFinal;
        bool _M_declDeprecated;
        QString _M_declDeprecatedComment;
        CodeModel::AccessPolicy _M_accessPolicy;
        CodeModel::AccessPolicy _M_usingBaseConstructors;

    private:
        _ClassModelItem(const _ClassModelItem &other);
        void operator = (const _ClassModelItem &other);
};

class _NamespaceModelItem: public _ScopeModelItem {
    public:
        DECLARE_MODEL_NODE(Namespace)

        static NamespaceModelItem create(CodeModel *model);

    public:
        NamespaceList namespaces() const;

        void addNamespace(NamespaceModelItem item);
        void removeNamespace(NamespaceModelItem item);

        NamespaceModelItem findNamespace(const QString &name) const;

        inline const QMap<QString, NamespaceModelItem>& namespaceMap() const { return _M_namespaces; }

    protected:
        _NamespaceModelItem(CodeModel *model, int kind = __node_kind)
                : _ScopeModelItem(model, kind) {}

    private:
        QMap<QString, NamespaceModelItem> _M_namespaces;

    private:
        _NamespaceModelItem(const _NamespaceModelItem &other);
        void operator = (const _NamespaceModelItem &other);
};

class _FileModelItem: public _NamespaceModelItem {
    public:
        DECLARE_MODEL_NODE(File)

        static FileModelItem create(CodeModel *model);

    protected:
        _FileModelItem(CodeModel *model, int kind = __node_kind)
                : _NamespaceModelItem(model, kind) {}

    private:
        _FileModelItem(const _FileModelItem &other);
        void operator = (const _FileModelItem &other);
};

class _ArgumentModelItem: public _CodeModelItem {
    public:
        DECLARE_MODEL_NODE(Argument)

        static ArgumentModelItem create(CodeModel *model);

    public:
        const TypeInfo& type() const;
        void setType(const TypeInfo &type);

        bool defaultValue() const;
        void setDefaultValue(bool defaultValue);

        const QString& defaultValueExpression() const { return _M_defaultValueExpression; }
        void setDefaultValueExpression(const QString &expr) { _M_defaultValueExpression = expr; }

    protected:
        _ArgumentModelItem(CodeModel *model, int kind = __node_kind)
                : _CodeModelItem(model, kind), _M_defaultValue(false) {}

    private:
        TypeInfo _M_type;
        QString _M_defaultValueExpression;
        bool _M_defaultValue;

    private:
        _ArgumentModelItem(const _ArgumentModelItem &other);
        void operator = (const _ArgumentModelItem &other);
};

class _MemberModelItem: public _CodeModelItem {
    public:
        DECLARE_MODEL_NODE(Member)

        bool isConstant() const;
        void setConstant(bool isConstant);

        bool isConstExpr() const;
        void setConstExpr(bool isConstExpr);

        bool isVolatile() const;
        void setVolatile(bool isVolatile);

        bool isStatic() const;
        void setStatic(bool isStatic);

        bool isFriend() const;
        void setFriend(bool isFriend);

        bool isRegister() const;
        void setRegister(bool isRegister);

        bool isExtern() const;
        void setExtern(bool isExtern);

        bool isMutable() const;
        void setMutable(bool isMutable);

        const QString& getDeprecatedComment() const;
        void setDeprecatedComment(const QString& comment);

        bool isDeprecated() const;
        void setDeprecated(bool isDeprecated);

        CodeModel::AccessPolicy accessPolicy() const;
        void setAccessPolicy(CodeModel::AccessPolicy accessPolicy);

        const TemplateParameterList& templateParameters() const
        { return _M_templateParameters; }

        void setTemplateParameters(const TemplateParameterList &templateParameters)
        { _M_templateParameters = templateParameters; }

        const TypeInfo& type() const;
        void setType(const TypeInfo &type);

    protected:
        _MemberModelItem(CodeModel *model, int kind)
                : _CodeModelItem(model, kind),
                _M_accessPolicy(CodeModel::Public),
                _M_flags(None) {}

    private:
        enum Flag : quint16{
            None = 0x00,
            IsConstant = 0x001,
            IsVolatile = 0x002,
            IsStatic = 0x004,
            IsFriend = 0x008,
            IsRegister = 0x010,
            IsExtern = 0x020,
            IsMutable = 0x040,
            IsDeprecated = 0x080,
            IsConstExpr = 0x100,
        };

        TemplateParameterList _M_templateParameters;
        TypeInfo _M_type;
        CodeModel::AccessPolicy _M_accessPolicy;
        QFlags<Flag> _M_flags;
        QString _M_deprecatedComment;
};

class _FunctionModelItem: public _MemberModelItem {
    public:
        DECLARE_MODEL_NODE(Function)

        static FunctionModelItem create(CodeModel *model);

    public:
        const ArgumentList& arguments() const;

        void addArgument(ArgumentModelItem item);
        void removeArgument(ArgumentModelItem item);

        CodeModel::FunctionType functionType() const;
        void setFunctionType(CodeModel::FunctionType functionType);

        bool isVirtual() const;
        void setVirtual(bool isVirtual);

        bool isInline() const;
        void setInline(bool isInline);

        bool isExplicit() const;
        void setExplicit(bool isExplicit);

        bool isInvokable() const; // Qt
        void setInvokable(bool isInvokable); // Qt

        bool isAbstract() const;
        void setAbstract(bool isAbstract);

        bool isDeclFinal() const;
        void setDeclFinal(bool isDeclFinal);

        bool isVariadics() const;
        void setVariadics(bool isVariadics);
        bool hasBody() const;
        void setHasBody(bool hasBody);
        bool isDeleted() const;
        void setDeleted(bool deleted);

        TypeInfo::ReferenceType referenceType() const;
        void setReferenceType(TypeInfo::ReferenceType referenceType);

        bool isSimilar(FunctionModelItem other) const;

    protected:
        _FunctionModelItem(CodeModel *model, int kind = __node_kind)
                : _MemberModelItem(model, kind),
                _M_functionType(CodeModel::Normal),
                _M_flags(None), _M_referenceType(TypeInfo::NoReference) {}

    private:
        enum Flag : quint16{
            None = 0x000,
            IsVirtual = 0x001,
            IsInline = 0x002,
            IsAbstract = 0x004,
            IsExplicit = 0x008,
            IsVariadics = 0x010,
            IsInvokable = 0x020,
            IsDeclFinal = 0x040,
            HasBody = 0x080,
            IsDeleted = 0x100,
        };
        ArgumentList _M_arguments;
        CodeModel::FunctionType _M_functionType;
        QFlags<Flag> _M_flags;
        TypeInfo::ReferenceType _M_referenceType;

    private:
        _FunctionModelItem(const _FunctionModelItem &other);
        void operator = (const _FunctionModelItem &other);
};

class _FunctionDefinitionModelItem: public _FunctionModelItem {
    public:
        DECLARE_MODEL_NODE(FunctionDefinition)

        static FunctionDefinitionModelItem create(CodeModel *model);

    protected:
        _FunctionDefinitionModelItem(CodeModel *model, int kind = __node_kind)
                : _FunctionModelItem(model, kind) {}

    private:
        _FunctionDefinitionModelItem(const _FunctionDefinitionModelItem &other);
        void operator = (const _FunctionDefinitionModelItem &other);
};

class _VariableModelItem: public _MemberModelItem {
    public:
        DECLARE_MODEL_NODE(Variable)

        static VariableModelItem create(CodeModel *model);

    protected:
        _VariableModelItem(CodeModel *model, int kind = __node_kind)
                : _MemberModelItem(model, kind) {}

    private:
        _VariableModelItem(const _VariableModelItem &other);
        void operator = (const _VariableModelItem &other);
};

class _TypeAliasModelItem: public _CodeModelItem {
    public:
        DECLARE_MODEL_NODE(TypeAlias)

        static TypeAliasModelItem create(CodeModel *model);

    public:
        const TypeInfo& type() const;
        void setType(const TypeInfo &type);
        CodeModel::AccessPolicy accessPolicy() const;
        void setAccessPolicy(CodeModel::AccessPolicy accessPolicy);

    protected:
        _TypeAliasModelItem(CodeModel *model, int kind = __node_kind)
                : _CodeModelItem(model, kind),
                  _M_accessPolicy(CodeModel::Public) {}

    private:
        TypeInfo _M_type;
        CodeModel::AccessPolicy _M_accessPolicy;

    private:
        _TypeAliasModelItem(const _TypeAliasModelItem &other);
        void operator = (const _TypeAliasModelItem &other);
};

class _EnumModelItem: public _CodeModelItem {
    public:
        DECLARE_MODEL_NODE(Enum)

        static EnumModelItem create(CodeModel *model);

    public:
        const TypeInfo& baseType() const;
        void setBaseType(const TypeInfo &baseType);
        bool isAnonymous() { return _M_isAnonymous; }
        void setAnonymous(bool b) { _M_isAnonymous = b; }
        bool isScopedEnum() { return _M_isScopedEnum; }
        void setScopedEnum(bool b) { _M_isScopedEnum = b; }
        bool isDeclDeprecated() { return _M_isDeclDeprecated; }
        void setDeclDeprecated(bool b) { _M_isDeclDeprecated = b; }
        const QString& getDeclDeprecatedComment() { return _M_declDeprecatedComment; }
        void setDeclDeprecatedComment(const QString& s) { _M_declDeprecatedComment = s; }

        CodeModel::AccessPolicy accessPolicy() const;
        void setAccessPolicy(CodeModel::AccessPolicy accessPolicy);

        const EnumeratorList& enumerators() const;
        void addEnumerator(EnumeratorModelItem item);
        void removeEnumerator(EnumeratorModelItem item);

    protected:
        _EnumModelItem(CodeModel *model, int kind = __node_kind)
                : _CodeModelItem(model, kind),
                _M_accessPolicy(CodeModel::Public), _M_isAnonymous(false), _M_isScopedEnum(false), _M_baseType() {}

    private:
        CodeModel::AccessPolicy _M_accessPolicy;
        EnumeratorList _M_enumerators;
        bool _M_isAnonymous;
        bool _M_isScopedEnum;
        bool _M_isDeclDeprecated;
        QString _M_declDeprecatedComment;
        TypeInfo _M_baseType;

    private:
        _EnumModelItem(const _EnumModelItem &other);
        void operator = (const _EnumModelItem &other);
};

class _EnumeratorModelItem: public _CodeModelItem {
    public:
        DECLARE_MODEL_NODE(Enumerator)

        static EnumeratorModelItem create(CodeModel *model);

    public:
        const QString& value() const;
        void setValue(const QString &value);
        bool deprecated() const;
        void setDeprecated(bool value);
        const QString& deprecatedComment() const;
        void setDeprecatedComment(const QString& value);

    protected:
        _EnumeratorModelItem(CodeModel *model, int kind = __node_kind)
                : _CodeModelItem(model, kind), _M_deprecated(false), _M_deprecatedComment() {}

    private:
        QString _M_value;
        bool _M_deprecated;
        QString _M_deprecatedComment;

    private:
        _EnumeratorModelItem(const _EnumeratorModelItem &other);
        void operator = (const _EnumeratorModelItem &other);
};

class _TemplateParameterModelItem: public _CodeModelItem {
    public:
        DECLARE_MODEL_NODE(TemplateParameter)

        static TemplateParameterModelItem create(CodeModel *model);

    public:
        const QString& defaultValue() const;
        void setDefaultValue(const QString& defaultValue);
        void setIsVaradic(bool isVaradic);
        bool isVaradic() const;
        void setOwnerClass(const ClassModelItem& ownerClass);
        ClassModelItem ownerClass() const;

    protected:
        _TemplateParameterModelItem(CodeModel *model, int kind = __node_kind)
                : _CodeModelItem(model, kind), _M_defaultValue(), _M_isVaradic(false), _M_ownerClass(nullptr) {}

    private:
        QString _M_defaultValue;
        bool _M_isVaradic;
        ClassModelItem _M_ownerClass;

    private:
        _TemplateParameterModelItem(const _TemplateParameterModelItem &other);
        void operator = (const _TemplateParameterModelItem &other);
};

template <class _Target, class _Source>
_Target model_safe_cast(_Source item) {
    typedef typename _Target::Type * _Target_pointer;
    typedef typename _Source::Type * _Source_pointer;

    _Source_pointer source = item.data();
    if (source && source->kind() == _Target_pointer(0)->__node_kind) {
        _Target ptr(static_cast<_Target_pointer>(source));
        return ptr;
    }

    return _Target();
}

template <typename _Target, typename _Source>
_Target model_dynamic_cast(_Source item) {
    typedef typename _Target::Type * _Target_pointer;
    typedef typename _Source::Type * _Source_pointer;

    _Source_pointer source = item.data();
    if (source && (source->kind() == _Target_pointer(0)->__node_kind
                   || (int(_Target_pointer(0)->__node_kind) <= int(_CodeModelItem::KindMask)
                       && ((source->kind() & _Target_pointer(0)->__node_kind)
                           == _Target_pointer(0)->__node_kind)))) {
        _Target ptr(static_cast<_Target_pointer>(source));
        return ptr;
    }

    return _Target();
}
#endif // CODEMODEL_H

// kate: space-indent on; indent-width 2; replace-tabs on;
