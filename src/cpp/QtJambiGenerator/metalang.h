/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#ifndef METALANG_H
#define METALANG_H

#include "codemodel.h"

#include "typesystem/utils.h"

#include <QtCore/QtCore>
#include "typesystem/typeentry.h"

using namespace TS;

class MetaClass;
class MetaField;
class MetaFunction;
class MetaType;
class MetaVariable;
class MetaArgument;
class MetaEnumValue;
class MetaEnum;
class QPropertySpec;
class MetaTemplateParameter;
class MetaBuilder;

typedef QList<MetaField *> MetaFieldList;
typedef QList<MetaArgument *> MetaArgumentList;
typedef QList<MetaFunction *> MetaFunctionList;
typedef QList<MetaTemplateParameter *> MetaTemplateParameterList;

enum ArgumentRemove{
    ArgumentRemove_No,
    ArgumentRemove_Remove,
    ArgumentRemove_UseAsUtilArg
};

class MetaClassList : public  QList<MetaClass *> {
    public:
        enum NameFlag{
            QualifiedCppName = 0x0001,
            FullName = 0x0002,
            SimpleName = 0x0004,
            All = QualifiedCppName | FullName | SimpleName,
        };

        MetaClass *findClass(const QString &name, NameFlag nameFlag = All) const;
        MetaEnumValue *findEnumValue(const QString &string) const;
        MetaEnum *findEnum(const EnumTypeEntry *entry) const;

};

class MetaAttributes {
    public:
        MetaAttributes() : m_attributes(0), m_originalAttributes(0), m_href(), m_brief() { }

        enum Attribute {
            None                        = 0x00000000,

            Private                     = 0x00000001,
            Protected                   = 0x00000002,
            Public                      = 0x00000004,
            Friendly                    = 0x00000008,
            Visibility                  = 0x0000000f,

            Native                      = 0x00000010,
            Abstract                    = 0x00000020,
            Static                      = 0x00000040,

            FinalInTargetLang           = 0x00000080,
            FinalInCpp                  = 0x00000100,
            ForceShellImplementation    = 0x00000200,

            GetterFunction              = 0x00000400,
            SetterFunction              = 0x00000800,

            FinalOverload               = 0x00001000,
            InterfaceFunction           = 0x00002000,

            PropertyReader              = 0x00004000,
            PropertyWriter              = 0x00008000,
            PropertyResetter            = 0x00010000,
            PropertyNotify              = 0x00400000,
            PropertyBindable            = 0x02000000,

            Fake                        = 0x00020000,

            Invokable                   = 0x00040000,

            PullProtectedMethodsDown    = 0x00080000,

            DeclaredFinalInCpp          = 0x00100000,

            Annonymous                  = 0x00200000,

            Deprecated                  = 0x01000000,

            ConstExpr                   = 0x04000000,

            Comment                     = 0x00800000,

            Override                    = 0x08000000,

            BracketCall                 = 0x10000000,

            IsUnion                     = 0x20000000,

            Final                       = FinalInTargetLang | FinalInCpp
        };

        uint attributes() const { return m_attributes; }
        void setAttributes(uint attributes) { m_attributes = attributes; }

        uint originalAttributes() const { return m_originalAttributes; }
        void setOriginalAttributes(uint attributes) { m_originalAttributes = attributes; }

        uint visibility() const { return m_attributes & Visibility; }
        void setVisibility(uint visi) { m_attributes = (m_attributes & uint(~Visibility)) | visi; }

        void operator+=(Attribute attribute) { m_attributes |= attribute; }
        void operator-=(Attribute attribute) { m_attributes &= ~attribute; }

        bool isNative() const { return m_attributes & Native; }
        bool isFinal() const { return (m_attributes & Final) == Final; }
        bool isFinalInTargetLang() const { return m_attributes & FinalInTargetLang; }
        bool isPullProtectedMethodsDown() const { return m_attributes & PullProtectedMethodsDown; }
        bool isFinalInCpp() const { return m_attributes & FinalInCpp; }
        bool isDeclaredFinalInCpp() const { return m_attributes & DeclaredFinalInCpp; }
        bool isAbstract() const { return m_attributes & Abstract; }
        bool isStatic() const { return m_attributes & Static; }
        bool isForcedShellImplementation() const { return m_attributes & ForceShellImplementation; }
        bool isInterfaceFunction() const { return m_attributes & InterfaceFunction; }
        bool isFinalOverload() const { return m_attributes & FinalOverload; }
        bool isInvokable() const { return m_attributes & Invokable; }

        bool isPropertyReader() const { return m_attributes & PropertyReader; }
        bool isPropertyBindable() const { return m_attributes & PropertyBindable; }
        bool isPropertyWriter() const { return m_attributes & PropertyWriter; }
        bool isPropertyResetter() const { return m_attributes & PropertyResetter; }
        bool isPropertyNotify() const { return m_attributes & PropertyNotify; }

        bool isPrivate() const { return m_attributes & Private; }
        bool isProtected() const { return m_attributes & Protected; }
        bool isPublic() const { return m_attributes & Public; }
        bool isFriendly() const { return m_attributes & Friendly; }
        bool isAnnonymous() const { return m_attributes & Annonymous; }
        bool isDeclDeprecated() const { return m_attributes & Deprecated; }
        bool isFake() const { return m_attributes & Fake; }

        bool wasPrivate() const { return m_originalAttributes & Private; }
        bool wasProtected() const { return m_originalAttributes & Protected; }
        bool wasPublic() const { return m_originalAttributes & Public; }
        bool wasFriendly() const { return m_originalAttributes & Friendly; }
        void setHref(const QString& href) {m_href = href;}
        void setBrief(const QString& brief) {m_brief = brief;}
        void setSince(const QString& since) {m_since = since;}
        const QString& href() const {return m_href;}
        const QString& brief() const {return m_brief;}
        const QString& since() const {return m_since;}

    private:
        uint m_attributes;
        uint m_originalAttributes;
        QString m_href;
        QString m_brief;
        QString m_since;
};


class MetaType {
    public:
        enum ReferenceType{
            NoReference = TypeInfo::NoReference,
            Reference = TypeInfo::Reference,
            RReference = TypeInfo::RReference
        };
        enum TypeUsagePattern {
            InvalidPattern,
            PrimitivePattern,
            FlagsPattern,
            EnumPattern,
            CharStringPattern,
            QStringPattern,
            QLatin1StringPattern,
            QLatin1StringViewPattern,
            QStringViewPattern,
            QAnyStringViewPattern,
            QUtf8StringViewPattern,
            QStringRefPattern,
            QCharPattern,
            NullptrPattern,
            AutoPattern,
            ValuePattern,
            ObjectPattern,
            NativePointerPattern,
            FunctionalPattern,
            ContainerPattern,
            IteratorPattern,
            SmartPointerPattern,
            InitializerListPattern,
            QSpanPattern,
            QVariantPattern,
            JObjectWrapperPattern,
            ArrayPattern,
            TemplateArgumentPattern
        };

        MetaType() :
                m_type_entry(nullptr),
                m_array_element_type(nullptr),
                m_original_template_type(nullptr),
                m_pattern(InvalidPattern),
                m_constant(false),
                m_volatile(false),
                m_reference(NoReference),
                m_cpp_instantiation(true),
                m_force_boxed_primitives(false),
                m_reserved(0) {
        }

        QString package() const { return m_type_entry->javaPackage(); }
        QString targetTypeSystem() const { return m_type_entry->targetTypeSystem(); }
        QString name() const { return m_type_entry->targetLangName(); }
        QString fullName() const { return m_type_entry->qualifiedTargetLangName(); }

        void setTypeUsagePattern(TypeUsagePattern pattern) { m_pattern = pattern; }
        TypeUsagePattern typeUsagePattern() const { return m_pattern; }

        // true when use pattern is container
        bool hasInstantiations() const { return !m_instantiations.isEmpty(); }
        void addInstantiation(const MetaType *inst);
        void setInstantiations(const QList<const MetaType *> &insts);
        const QList<const MetaType *>& instantiations() const { return m_instantiations; }
        void addIteratorInstantiation(const MetaType *inst) { m_iteratorInstantiations << inst; }
        void setIteratorInstantiations(const QList<const MetaType *> &insts) { m_iteratorInstantiations = insts; }
        const QList<const MetaType *>& iteratorInstantiations() const { return m_iteratorInstantiations; }
        void setInstantiationInCpp(bool incpp) { m_cpp_instantiation = incpp; }
        bool hasInstantiationInCpp() const { return hasInstantiations() && m_cpp_instantiation; }
        void setForceBoxedPrimitives(bool force_boxed_primitives) { m_force_boxed_primitives = force_boxed_primitives; }
        bool forceBoxedPrimitives() const { return m_force_boxed_primitives; }

        QString minimalSignature() const;
        QString normalizedSignature() const;

        // true when the type is a QtObject subclass
        bool hasNativeId() const;

        // returns true if the type was a template argument type
        bool isTemplateArgument() const { return m_pattern == TemplateArgumentPattern; }

        // returns true if the typs is used as a non complex primitive, no & or *'s
        bool isPrimitive() const { return m_pattern == PrimitivePattern; }

        bool isPrimitiveChar() const { return isQChar() && m_type_entry && m_type_entry->isQChar(); }

        bool isNullPtr() const { return m_pattern == NullptrPattern; }

        // returns true if the type is used as an enum
        bool isEnum() const { return m_pattern == EnumPattern; }

        // returns true if the type is used as a functional
        bool isFunctional() const { return m_pattern == FunctionalPattern; }

        // returns true if the type is used as a QObject *
        bool isQObject() const;

        // returns true if the type is used as a QEvent *
        bool isQEvent() const;

        // returns true if the type is used as an object, e.g. Xxx *
        bool isObject() const { return m_pattern == ObjectPattern; }

        // returns true if the type is used as an array, e.g. Xxx[42]
        bool isArray() const { return m_pattern == ArrayPattern; }

        // returns true if the type is used as an initializer list
        bool isInitializerList() const { return m_pattern == InitializerListPattern; }

        bool isQSpan() const { return m_pattern == QSpanPattern; }

        // returns true if the type is used as a value type (X or const X &)
        bool isValue() const { return m_pattern == ValuePattern; }

        // returns true for more complex types...
        bool isNativePointer() const { return m_pattern == NativePointerPattern; }

        bool isJavaString() const {
            return m_pattern == QStringPattern
                    || m_pattern == QLatin1StringPattern
                    || m_pattern == QLatin1StringViewPattern
                    || m_pattern == QStringViewPattern
                    || m_pattern == QAnyStringViewPattern
                    || m_pattern == QUtf8StringViewPattern
                    || m_pattern == QStringRefPattern;
        }

        // returns true if the type was originally a QString or const QString &
        bool isQString() const { return m_pattern == QStringPattern; }

        bool isCharString() const { return m_pattern == CharStringPattern; }

        // returns true if the type was originally a QLatin1String
        bool isQLatin1String() const { return m_pattern == QLatin1StringPattern; }

        // returns true if the type was originally a QLatin1StringView
        bool isQLatin1StringView() const { return m_pattern == QLatin1StringViewPattern; }

        // returns true if the type was originally a QString or const QString & or equivalent for QLatin1String
        bool isQStringView() const { return m_pattern == QStringViewPattern; }

        bool isQAnyStringView() const { return m_pattern == QAnyStringViewPattern; }

        bool isQUtf8StringView() const { return m_pattern == QUtf8StringViewPattern; }

        // returns true if the type was originally a QStringRef or const QStringRef &
        bool isQStringRef() const { return m_pattern == QStringRefPattern; }

        // returns true if the type was originally a QChar or const QChar &
        bool isQChar() const { return m_pattern == QCharPattern; }

        // return true if the type was originally a QVariant or const QVariant &
        bool isQVariant() const { return m_pattern == QVariantPattern; }

        // return true if the type was originally a JObjectWrapper or const JObjectWrapper &
        bool isJObjectWrapper() const { return m_pattern == JObjectWrapperPattern; }

        // returns true if the type was used as a container
        bool isContainer() const { return m_pattern == ContainerPattern; }

        // returns true if the type was used as an iterator
        bool isIterator() const { return m_pattern == IteratorPattern; }

        // returns true if the type was used as a qpointer
        bool isSmartPointer() const { return m_pattern == SmartPointerPattern; }

        // returns true if the type was used as a flag
        bool isFlags() const { return m_pattern == FlagsPattern; }

        bool isConstant() const { return m_constant; }
        void setConstant(bool constant) { m_constant = constant; }

        bool isVolatile() const { return m_volatile; }
        void setVolatile(bool vol) { m_volatile = vol; }

        ReferenceType getReferenceType() const { return m_reference; }
        void setReferenceType(ReferenceType ref) { m_reference = ref; }

        // Returns true if the type is to be implemented using Java enums, e.g. not plain ints.
        bool isTargetLangEnum() const { return isEnum() && !static_cast<const EnumTypeEntry *>(typeEntry())->forceInteger(); }
        bool isIntegerEnum() const { return isEnum() && static_cast<const EnumTypeEntry *>(typeEntry())->forceInteger(); }

        // Returns true if the type is to be implemented using Java QFlags, e.g. not plain ints.
        bool isTargetLangFlags() const {
            return isFlags() && !static_cast<const FlagsTypeEntry *>(typeEntry())->forceInteger();
        }
        bool isIntegerFlags() const { return isFlags() && static_cast<const FlagsTypeEntry *>(typeEntry())->forceInteger(); }

        int actualIndirections() const { return m_indirections.size() + (m_reference==Reference ? 1 : 0); }
        const QList<bool>& indirections() const { return m_indirections; }
        void setIndirections(const QList<bool>& indirections) { m_indirections = indirections; }

        void setArrayElementCounts(const QList<QPair<int,QString>>& list) { m_array_element_count = list; }
        const QList<QPair<int,QString>>& arrayElementCounts() const { return m_array_element_count; }

        MetaType *arrayElementType() const { return m_array_element_type; }
        void setArrayElementType(MetaType *t) { m_array_element_type = t; }

        QString cppSignature() const;

        MetaType *copy() const;

        const TypeEntry *typeEntry() const { return m_type_entry; }
        void setTypeEntry(const TypeEntry *type) { m_type_entry = type; }

        void setOriginalTypeDescription(const QString &otd) { m_original_type_description = otd; }
        QString originalTypeDescription() const { return m_original_type_description; }

        void setOriginalTemplateType(const MetaType *type) { m_original_template_type = type; }
        const MetaType *originalTemplateType() const { return m_original_template_type; }
    private:
        const TypeEntry *m_type_entry;
        QList <const MetaType *> m_instantiations;
        QList <const MetaType *> m_iteratorInstantiations;
        QString m_package;
        QString m_original_type_description;

        QList<QPair<int,QString>> m_array_element_count;
        MetaType *m_array_element_type;
        const MetaType *m_original_template_type;

        TypeUsagePattern m_pattern;
    uint m_constant : 1;
    uint m_volatile : 1;
    ReferenceType m_reference;
    uint m_cpp_instantiation : 1;
    uint m_force_boxed_primitives : 1;
    uint m_reserved : 24; // unused
    QList<bool> m_indirections;
};

class MetaVariable {
    public:
        MetaVariable() : m_type(nullptr) { }

        MetaType *type() const { return m_type; }
        void setType(MetaType *type) { m_type = type; }

        const QString & name() const { return m_name; }
        void setName(const QString &name) { m_name = name; }

        const QString & defaultType() const { return m_defaultType; }
        void setDefaultType(const QString &type) { m_defaultType = type; }

    private:
        QString m_name;
        MetaType *m_type;
        QString m_defaultType;
};

class MetaTemplateParameter : public MetaVariable {
public:
    MetaTemplateParameter():m_instantiationType(nullptr),m_implicit(false){}

    const QString& instantiation() const { return m_instantiation; }
    const MetaType * instantiationType() const { return m_instantiationType; }
    void setInstantiation(bool implicit, const QString &expr, const MetaType *instantiationType) { m_implicit = implicit; m_instantiation = expr; m_instantiationType = instantiationType; }
    bool isImplicit() const{return m_implicit;}
    QString parameterType() const { return m_parameterType; }
    void setParameterType(const QString &expr) { m_parameterType = expr; }
    MetaTemplateParameter *copy() const;
private:
    QString m_instantiation;
    const MetaType *m_instantiationType;
    QString m_parameterType;
    bool m_implicit;
};

class MetaArgument : public MetaVariable {
    public:
        MetaArgument() : m_argument_index(0) { }
        virtual ~MetaArgument(){}

        inline const QString& defaultValueExpression() const { return m_expression; }
        inline void setDefaultValueExpression(const QString &expr) { m_expression = expr; }

        inline const QString& originalDefaultValueExpression() const { return m_original_expression; }
        inline void setOriginalDefaultValueExpression(const QString &expr) { m_original_expression = expr; }

        inline QString toString() const {
            return type()->name() + " " + MetaVariable::name() +
                   (m_expression.isEmpty() ? "" :  " = " + m_expression);
        }

        inline int argumentIndex() const { return m_argument_index; }
        inline void setArgumentIndex(int argIndex) { m_argument_index = argIndex; }

        virtual QString argumentName() const; // is overridden by java meta type to exclude java key words
        QString modifiedArgumentName() const;
        QString indexedName() const;
        inline void setModifiedName(const QString &javaName) { m_modifiedName = javaName; }
        inline QString comment() const { return m_comment; }
        inline void setComment(const QString &comment) { m_comment = comment; }

        MetaArgument *copy() const;

        bool isNullPointerDisabled(const MetaFunction* function) const;

    private:
        // Just to force people to call argumentName() And indexedName();
        QString name() const;

        QString m_modifiedName;
        QString m_comment;
        QString m_expression;
        QString m_original_expression;
        int m_argument_index;
};


class MetaField : public MetaVariable, public MetaAttributes {
    public:
        MetaField();
        ~MetaField();

        const MetaClass *enclosingClass() const { return m_class; }
        void setEnclosingClass(const MetaClass *cls) { m_class = cls; }

        const MetaFunction *getter() const;
        const MetaFunction *setter() const;

        FieldModificationList modifications() const;

        MetaField *copy() const;

    private:
        mutable MetaFunction *m_getter;
        mutable MetaFunction *m_setter;
        const MetaClass *m_class;
};


class MetaFunction : public MetaAttributes {
    public:
    enum FunctionType {
        ConstructorFunction,
        DestructorFunction,
        NormalFunction,
        SignalFunction,
        PrivateSignalFunction,
        EmptyFunction,
        SlotFunction,
        GlobalScopeFunction,
        BaseClassDelegateFunction
    };

    enum CompareResult {
        EqualName                   = 0x00000001,
        EqualArguments              = 0x00000002,
        EqualAttributes             = 0x00000004,
        EqualImplementor            = 0x00000008,
        EqualReturnType             = 0x00000010,
        EqualDefaultValueOverload   = 0x00000020,
        EqualModifiedName           = 0x00000040,

        NameLessThan                = 0x00001000,

        PrettySimilar               = EqualName | EqualArguments,
        Equal                       = 0x0000001f,
        NotEqual                    = 0x00001000
    };

    MetaFunction();
    ~MetaFunction();

    QString name() const;
    void setName(const QString &name);

    QString originalName() const;
    void setOriginalName(const QString &name);

    QString modifiedName() const;

    QString minimalSignature() const;
    QString minimalSignatureNoTemplate() const;
    void setOriginalSignature(const QString &signature);
    const QString& originalSignature() const;

    QString marshalledName() const;
    QString marshalledArguments(int count = -1) const;

    static QString marshalledArguments(const QList<MetaArgument *>& arguments, bool isConst, int count = -1);
    static QString marshalledArguments(const QList<const MetaType *>& arguments);

    // true if one or more of the arguments are of QtObject subclasses
    bool argumentsHaveNativeId() const;

    bool isModifiedRemoved(int types = TS::All) const;

    bool isProxyCall() const;

    MetaType *type() const;
    void setType(MetaType *type);

    // The class that has this function as a member.
    const MetaClass *ownerClass() const;
    void setOwnerClass(const MetaClass *cls);

    // The first class in a hierarchy that declares the function
    const MetaClass *declaringClass() const;
    void setDeclaringClass(const MetaClass *cls);

    // The class that actually implements this function
    const MetaClass *implementingClass() const;
    void setImplementingClass(const MetaClass *cls);

    bool needsCallThrough() const;

    bool needsReturnScope() const;

    const MetaArgumentList& arguments() const;
    void setArguments(const MetaArgumentList &arguments);
    void addArgument(MetaArgument *argument);
    int actualMinimumArgumentCount() const;

    void setInvalid(bool on);
    bool isInvalid() const;
    void setTemplate(bool on);
    bool isTemplate() const;
    bool isDeprecated() const;
    bool isDestructor() const;
    bool isConstructor() const;

    /**
     *detects, if the constructor is of form T(const T&)
     * @brief isCopyConstructor
     * @return
     */
    bool isCopyConstructor() const;
    bool hasRReferences() const;
    bool isNormal() const;
    bool isInGlobalScope() const;
    bool isSignal() const;
    bool isPrivateSignal() const;
    bool isSlot() const;
    bool isEmptyFunction() const;
    bool isBaseClassDelegateFunction() const;
    bool hasUnresolvedTemplateTypes() const;
    bool hasTemplateTypes() const;
    FunctionType functionType() const;
    void setFunctionType(FunctionType type);

    QStringList introspectionCompatibleSignatures(const QStringList &resolvedArguments = QStringList()) const;
    QString signature(bool skipName = false) const;
    QString targetLangSignature(bool minimal = false) const;
    bool shouldReturnThisObject() const;

    bool isConstant() const;
    void setConstant(bool constant);

    const QString &toString() const;

    uint compareTo(const MetaFunction *other) const;

    bool operator <(const MetaFunction &a) const;

    MetaFunction *copy() const;

    QString replacedDefaultExpression(const MetaClass *cls, int idx) const;
    bool removedDefaultExpression(const MetaClass *cls, int idx) const;
    QString conversionRule(TS::Language language, int idx) const;
    bool hasConversionRule(TS::Language language, int idx) const;
    QList<ReferenceCount> referenceCounts(const MetaClass *cls, int idx = -2) const;
    QList<TemplateInstantiation> templateInstantiations(const MetaClass *cls) const;

    bool nullPointersDisabled(const MetaClass *cls = nullptr, int argument_idx = 0) const;
    QString nullPointerDefaultValue(const MetaClass *cls = nullptr, int argument_idx = 0) const;

    bool resetObjectAfterUse(int argument_idx) const;

    bool valueAsPointer(int argument_idx) const;

    // Returns the ownership rules for the given argument in the given context
    OwnershipRule ownership(const MetaClass *cls, TS::Language language, int idx) const;

    bool isVirtualSlot() const;
    bool isAllowedAsSlot() const;
    bool isPaintMethod() const;
    bool isThreadAffine() const;
    bool isUIThreadAffine() const;
    bool isPixmapThreadAffine() const;

    int utilArgumentIndex(int argument_index) const;
    QString arrayOrBufferLengthExpression(int key) const;
    int arrayOrBufferLengthMinValue(int argument_index) const;
    int arrayOrBufferLengthMaxValue(int argument_index) const;
    bool useArgumentAsDerefPointer(int key) const;
    bool useArgumentAsSlotContext(int key) const;
    bool useArgumentAsSlot(int key) const;
    bool useArgumentAsArray(int key) const;
    bool useArgumentAsBuffer(int key) const;
    bool useArgumentAsVarArgs(int key) const;
    bool insertUtilArgument(int key) const;
    bool implementPlainDelegate(int key) const;
    bool isNoExcept() const;
    bool isBlockExceptions() const;
    bool isRethrowExceptions() const;
    QString typeReplaced(int argument_index, QString* jniType = nullptr) const;
    bool isRemovedFromAllLanguages(const MetaClass *) const;
    bool isRemovedFrom(const MetaClass *, TS::Language language) const;

    ArgumentRemove argumentRemoved(int) const;
    QStringList implicitCalls(int) const;
    QStringList inhibitedImplicitCalls(int) const;
    bool isNoImplicitCalls(int) const;
    bool isForcedExplicit() const;
    bool isForcedImplicit() const;
    bool isNoImplicitArguments() const;
    ThreadAffinity argumentThreadAffinity(int) const;

    QList<Delegate> delegates() const;

    QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments() const;

    QString proxyCall() const;
    QString argumentReplaced(int key) const;
    bool isSelfReturningFunction() const;
    bool needsSuppressUncheckedWarning() const;
    bool needsSuppressRawTypeWarning() const;

    bool hasModifications(const MetaClass *implementor) const;
    bool hasModifications(const ComplexTypeEntry *entry) const;
    FunctionModificationList modifications(const MetaClass *implementor) const;
    FunctionModificationList modifications(const ComplexTypeEntry *entry) const;
    FunctionModificationList modifications(const TypeSystemTypeEntry *entry) const;
    bool hasCodeInjections(const MetaClass *implementor, TS::Language language, const QSet<CodeSnip::Position>& positions) const;
    QString throws(const MetaClass *implementor = nullptr) const;

    // If this function stems from an interface, this returns the
    // interface that declares it.
    const MetaClass *interfaceClass() const;
    void setInterfaceClass(const MetaClass *cl);

    void setPropertySpec(QPropertySpec *spec);
    QPropertySpec *propertySpec() const;

    const MetaTemplateParameterList & templateParameters() const {
        return m_templateParameters;
    }

    void setTemplateParameters(const MetaTemplateParameterList & templateParameters) {
        m_templateParameters = templateParameters;
    }

    void addTemplateParameter(MetaTemplateParameter* templateParameter) {
        m_templateParameters << templateParameter;
    }

    const MetaField *accessedField() const { return m_accessedField; }

    void setAccessedField(const MetaField *accessedField) { m_accessedField = accessedField; }
    void setDeprecatedComment(const QString &deprecatedComment) { m_deprecatedComment = deprecatedComment; }
    const QString& deprecatedComment() const { return m_deprecatedComment; }

    bool isVariadics() const { return m_variadics; }
    void setVariadics(bool isVariadics) { m_variadics = isVariadics; }
    bool isDeclExplicit() const { return m_declExplicit; }
    void setDeclExplicit(bool isExplicit) { m_declExplicit = isExplicit; }

    MetaType::ReferenceType functionReferenceType() const{
        return m_functionReferenceType;
    }

    void setFunctionReferenceType(MetaType::ReferenceType functionReferenceType) {
        m_functionReferenceType = functionReferenceType;
    }

    const QPair<MetaFunction*,FunctionModification>& functionTemplate() const{
        return m_functionTemplate;
    }

    void setFunctionTemplate(MetaFunction* function, const FunctionModification& modification = {}) {
        m_functionTemplate = {function, modification};
    }

    QList<Parameter> addedParameterTypes() const;

    OperatorType operatorType() const{return m_operatorType;}

    void setOperatorType(OperatorType operatorType){ m_operatorType = operatorType; }

    int returnScopeIndex() const;
    void setReturnScopeIndex(int newReturnScopeIndex);

    QString returnValueComment() const;
    void setReturnValueComment(const QString& returnValueComment);

private:
    QString m_name;
    QString m_original_name;
    QString m_original_signature;
    mutable QString m_cached_full_signature;
    mutable QString m_cached_full_signature_no_name;
    mutable QString m_cached_minimal_signature;
    mutable QString m_cached_minimal_signature_no_template;
    mutable QString m_cached_modified_name;

    FunctionType m_function_type;
    MetaType *m_type;
    const MetaClass *m_class;
    const MetaClass *m_implementing_class;
    const MetaClass *m_declaring_class;
    const MetaClass *m_interface_class;
    QPropertySpec *m_property_spec;
    MetaArgumentList m_arguments;
    MetaTemplateParameterList m_templateParameters;
    OperatorType m_operatorType;
    uint m_constant          : 1;
    uint m_variadics         : 1;
    uint m_declExplicit      : 1;
    uint m_template          : 1;
    uint m_invalid           : 1;
    mutable int m_actualMinimumArgumentCount;
    const MetaField *m_accessedField;
    QString m_deprecatedComment;
    MetaType::ReferenceType m_functionReferenceType;
    QPair<MetaFunction*,FunctionModification> m_functionTemplate;
    int m_returnScopeIndex = -1;
    QString m_returnValueComment;
};

class MetaEnum;

class MetaEnumValue {
public:
    MetaEnumValue()
            : m_value(), _M_deprecated(false) {
    }

    const QVariant& value() const { return m_value; }
    void setValue(const QVariant& value) { m_value = value; }

    const QString& stringValue() const { return m_string_value; }
    void setStringValue(const QString &v) { m_string_value = v; }

    const QString& name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }
    QString cppName() const;

    const MetaEnum * getEnum(){ return m_enum;}
    void setEnum(const MetaEnum * enm) {m_enum = enm;}
    bool deprecated() const { return _M_deprecated; }
    void setDeprecated(bool value) { _M_deprecated = value; }
    void setDeprecatedComment(const QString &deprecatedComment) { m_deprecatedComment = deprecatedComment; }
    const QString& deprecatedComment() const { return m_deprecatedComment; }

private:
    const MetaEnum * m_enum;
    QString m_name;
    QString m_string_value;

    QVariant m_value;
    bool _M_deprecated;
    QString m_deprecatedComment;
};


class MetaEnumValueList : public QList<MetaEnumValue *> {
    public:
        MetaEnumValue *find(const QString &name) const;
};

class MetaEnum : public MetaAttributes {
    public:
        MetaEnum() : MetaAttributes(), m_type_entry(nullptr), m_class(nullptr), m_isMetaEnum(false) {}

        const MetaEnumValueList& values() const { return m_enum_values; }
        void addEnumValue(MetaEnumValue *enumValue) { m_enum_values << enumValue; }

        QString name() const { return m_type_entry->targetLangName(); }
        QString qualifier() const { return m_type_entry->javaQualifier(); }
        QString package() const { return m_type_entry->javaPackage(); }
        QString targetTypeSystem() const { return m_type_entry->targetTypeSystem(); }
        QString fullName() const { return qualifier().isEmpty() ? package() + "." + name() : package() + "." + qualifier()  + "." + name(); }

        // Has the enum been declared inside a Q_ENUMS() macro in its enclosing class?
        void setMetaEnum(bool on) { m_isMetaEnum = on; }
        bool isMetaEnum() const { return m_isMetaEnum; }

        EnumTypeEntry *typeEntry() const { return m_type_entry; }
        void setTypeEntry(EnumTypeEntry *entry) { m_type_entry = entry; }

        MetaClass *enclosingClass() const { return m_class; }
        void setEnclosingClass(MetaClass *c) { m_class = c; }
        const QString& baseTypeName() const { return m_base_type_name; }
        void setBaseTypeName(const QString &names) { m_base_type_name = names; }
        void setDeprecatedComment(const QString &deprecatedComment) { m_deprecatedComment = deprecatedComment; }
        const QString& deprecatedComment() const { return m_deprecatedComment; }
    private:
        QString m_base_type_name;
        MetaEnumValueList m_enum_values;
        EnumTypeEntry *m_type_entry;
        MetaClass *m_class;
        QString m_deprecatedComment;

    uint m_isMetaEnum : 1;
};

class MetaFunctional : public MetaAttributes {
    public:
        MetaFunctional() : MetaAttributes(), m_base_type_name(), m_type_entry(nullptr), m_class(nullptr), m_type(nullptr), m_arguments() {}

        QString name() const { return m_type_entry->targetLangName(); }
        QString qualifier() const { return m_type_entry->javaQualifier(); }
        QString package() const { return m_type_entry->javaPackage(); }
        QString targetTypeSystem() const { return m_type_entry->targetTypeSystem(); }
        QString fullName() const { return qualifier().isEmpty() ? package() + "." + name() : package() + "." + qualifier()  + "." + name(); }

        FunctionalTypeEntry *typeEntry() const { return m_type_entry; }
        void setTypeEntry(FunctionalTypeEntry *entry) { m_type_entry = entry; }

        OwnershipRule ownership(TS::Language language, int idx) const;

        MetaClass *enclosingClass() const { return m_class; }
        void setEnclosingClass(MetaClass *c) { m_class = c; }
        const QString& baseTypeName() const { return m_base_type_name; }
        void setBaseTypeName(const QString &names) { m_base_type_name = names; }
        const QString& javaFunctionalInterface() const { return m_javaFunctionalInterface; }
        void setJavaFunctionalInterface(const QString &names) { m_javaFunctionalInterface = names; }
        const QList<uint>& javaFunctionalInterfaceParameterTypes() const { return m_javaFunctionalInterfaceParameterTypes; }
        void setJavaFunctionalInterfaceParameterTypes(const QList<uint> &javaFunctionalInterfaceParameterTypes) { m_javaFunctionalInterfaceParameterTypes = javaFunctionalInterfaceParameterTypes; }
        const QString& typeSignature() const { return m_typeSignature; }
        void setTypeSignature(const QString& type) { m_typeSignature = type; }
        MetaType * type() const { return m_type; }
        void setType(MetaType * type) { m_type = type; }
        const QList<MetaArgument *>& arguments() const { return m_arguments; }
        void addArgument(MetaArgument * type) { m_arguments << type; }
        ArgumentRemove argumentRemoved(int) const;
        int utilArgumentIndex(int key) const;
        QString arrayOrBufferLengthExpression(int key) const;
        int arrayOrBufferLengthMinValue(int argument_index) const;
        int arrayOrBufferLengthMaxValue(int argument_index) const;
        bool useArgumentAsDerefPointer(int key) const;
        bool useArgumentAsSlotContext(int key) const;
        bool useArgumentAsSlot(int key) const;
        bool useArgumentAsArray(int key) const;
        bool useArgumentAsBuffer(int key) const;
        bool useArgumentAsVarArgs(int key) const;
        bool insertUtilArgument(int key) const;
        bool implementPlainDelegate(int key) const;
        QString typeReplaced(int argument_index, QString* jniType = nullptr) const;
        QString conversionRule(TS::Language language, int idx) const;
        bool hasConversionRule(TS::Language language, int idx) const;
        bool resetObjectAfterUse(int argument_idx) const;
        bool isNoExcept() const;
        bool isBlockExceptions() const;
        bool isRethrowExceptions() const;
        bool nullPointersDisabled(int argument_idx = 0) const;
        bool needsCallThrough() const;
        bool needsReturnScope() const;

    private:
        QString m_base_type_name;
        FunctionalTypeEntry *m_type_entry;
        MetaClass *m_class;
        MetaType *m_type;
        QList<MetaArgument *> m_arguments;
        QString m_typeSignature;
        QString m_javaFunctionalInterface;
        QList<uint> m_javaFunctionalInterfaceParameterTypes;
};

typedef QList<MetaEnum *> MetaEnumList;

typedef QList<MetaFunctional *> MetaFunctionalList;

class MetaClass : public MetaAttributes {
    public:
        enum FunctionQueryOption {
            Constructors            = 0x000001,   // Only constructors
            //Destructors             = 0x000002,   // Only destructors. Not included in class.
            VirtualFunctions        = 0x000004,   // Only virtual functions (virtual in both TargetLang and C++)
            FinalInTargetLangFunctions    = 0x000008,   // Only functions that are non-virtual in TargetLang
            FinalInCppFunctions     = 0x000010,   // Only functions that are non-virtual in C++
            ClassImplements         = 0x000020,   // Only functions implemented by the current class
            Inconsistent            = 0x000040,   // Only inconsistent functions (inconsistent virtualness in TargetLang/C++)
            StaticFunctions         = 0x000080,   // Only static functions
            Signals                 = 0x000100,   // Only signals
            NormalFunctions         = 0x000200,   // Only functions that aren't signals
            Visible                 = 0x000400,   // Only public and protected functions
            ForcedShellFunctions    = 0x000800,   // Only functions that are overridden to be implemented in the shell class
            WasPublic               = 0x001000,   // Only functions that were originally public
            WasProtected            = 0x002000,   // Only functions that were originally protected
            NonStaticFunctions      = 0x004000,   // No static functions
            Empty                   = 0x008000,   // Empty overrides of abstract functions
            Invisible               = 0x010000,   // Only private functions
            VirtualInCppFunctions   = 0x020000,   // Only functions that are virtual in C++
            NonEmptyFunctions       = 0x040000,   // Only functions with JNI implementations
            VirtualInTargetLangFunctions  = 0x080000,   // Only functions which are virtual in TargetLang
            AbstractFunctions       = 0x100000,   // Only abstract functions
            WasVisible              = 0x200000,   // Only functions that were public or protected in the original code
            NotRemovedFromTargetLang      = 0x400000,   // Only functions that have not been removed from TargetLang
            NotRemovedFromShell     = 0x800000,    // Only functions that have not been removed from the shell class
            VirtualSlots           = 0x1000000     // Only functions that are set as virtual slots in the type system
        };

        MetaClass();
        virtual ~MetaClass();

        MetaClass *extractInterface();
        MetaClass *extractInterfaceImpl();
        MetaClass *extractInterface()const;
        MetaClass *extractInterfaceImpl()const;
        void fixUnimplmentablePureVirtualFunctions();

        const MetaFunctionList& functions() const;
        const MetaFunctionList& invalidFunctions() const;
        const MetaFunctionList& deletedFunctions() const;
        void setFunctions(const MetaFunctionList &functions);
        void addFunction(MetaFunction *function);
        bool hasFunction(const MetaFunction *f) const;
        bool hasFunction(const QString &str) const;
        bool hasSignal(const MetaFunction *f) const;
        void addInvalidFunction(MetaFunction *function);
        void addDeletedFunction(MetaFunction *function);

        bool hasConstructors() const;

        void addDefaultConstructor();

        bool hasJustPrivateConstructors() const { return m_has_justprivateconstructors; }
        void setHasJustPrivateConstructors(bool on) { m_has_justprivateconstructors = on; }
        bool hasPublicStandardConstructor() const;
        bool hasExplicitStandardConstructor() const;
        bool hasExplicitCopyConstructor() const;
        bool hasPublicAssignment() const;
        bool hasStandardConstructor() const;
        bool hasUnimplmentablePureVirtualFunction() const {return m_has_unimplmentablePureVirtualFunctions;}
        const QSet<QString>& unimplmentablePureVirtualFunctions() const {return m_unimplmentablePureVirtualFunctions;}
        void setUnimplmentablePureVirtualFunctions(const QSet<QString>& privatePureVirtualFunctions) { m_unimplmentablePureVirtualFunctions = privatePureVirtualFunctions; }
        bool hasPrivateMetaObjectFunction() const { return m_has_private_metaObject; }
        bool hasPrivateMetaCallFunction() const { return m_has_private_metacall; }
        bool hasPrivateMetaCastFunction() const { return m_has_private_metacast; }
        bool hasMetaObjectFunction() const { return m_has_metaObject; }
        bool hasMetaCallFunction() const { return m_has_metacall; }
        bool hasMetaCastFunction() const { return m_has_metacast; }
        bool hasVirtualDestructor() const {
            if(baseClass() && baseClass()->hasVirtualDestructor())
                return true;
            for(MetaClass* iface : interfaces()){
                if(iface->hasVirtualDestructor())
                    return true;
            }
            return m_type_entry->isDestructorVirtual();
        }

        MetaFunctionList queryFunctionsByName(const QString &name) const;
        MetaFunctionList queryFunctionsByOriginalName(const QString &name) const;
        MetaFunctionList queryOtherFunctions(const MetaFunction* fun) const;
        MetaFunctionList queryFunctions(uint query) const;
        inline MetaFunctionList allVirtualFunctions() const;
        inline MetaFunctionList allFinalFunctions() const;
        MetaFunctionList functionsInTargetLang() const;
        MetaFunctionList functionsInShellClass() const;
        inline MetaFunctionList cppInconsistentFunctions() const;
        inline MetaFunctionList cppSignalFunctions() const;
        MetaFunctionList publicOverrideFunctions() const;
        MetaFunctionList virtualOverrideFunctions() const;
        MetaFunctionList virtualFunctions() const;
        MetaFunctionList implementableFunctions() const;
        MetaFunctionList nonVirtualShellFunctions() const;

        const MetaFieldList& fields() const { return m_fields; }
        void setFields(const MetaFieldList &fields) { m_fields = fields; }
        void addField(MetaField *field) { m_fields << field; }

        const MetaEnumList& enums() const { return m_enums; }
        void setEnums(const MetaEnumList &enums) { m_enums = enums; }
        void addEnum(MetaEnum *e) { m_enums << e; }

        const MetaFunctionalList& functionals() const { return m_functionals; }
        void setFunctionals(const MetaFunctionalList &functionals) { m_functionals = functionals; }
        void addFunctional(MetaFunctional *e) {
            m_functionals << e;
            if(!isFake())
                e->setEnclosingClass(this);
        }

        MetaField *findField(const QString &name);
        MetaFunction *findFunction(const QString &name);
        MetaEnum *findEnum(const QString &enumName);
        MetaFunctional *findFunctional(const QString &functionalName);
        MetaEnum *findEnumForValue(const QString &enumName);
        MetaEnumValue *findEnumValue(const QString &enumName, MetaEnum *meta_enum);
        MetaClass *findIterator(const IteratorTypeEntry* iteratorType) const;

        const MetaClassList& interfaces() const { return m_interfaces; }
        void addInterface(MetaClass *interface);
        void setInterfaces(const MetaClassList &interface);

        const MetaClassList& enclosedClasses() const { return m_enclosed_classes; }
        void addEnclosedClass(MetaClass *enclosed_class);
        void setEnclosedClasses(const MetaClassList &enclosed_classes);

        virtual QString fullName() const;
        virtual QString name() const;

        /**
         * returns the simple name of an embedded class. of the complete name in case of top level classes.
         * @brief simpleName
         * @return
         */
        QString simpleName() const;

        QString baseClassName() const { return m_base_class ? m_base_class->name() : QString(); }

        MetaClass *baseClass() const { return m_base_class; }
        MetaClass *baseClass() { return m_base_class; }
        void setBaseClass(MetaClass *base_class);

        const MetaClass *enclosingClass() const { return m_enclosing_class; }
        void setEnclosingClass(MetaClass *cl) { m_enclosing_class = cl; }

        QString package() const { return m_type_entry->javaPackage(); }
        QString targetTypeSystem() const { return m_type_entry->targetTypeSystem(); }
        bool isInterface() const { return m_type_entry->isInterface(); }
        bool isNamespace() const { return m_type_entry->isNamespace(); }
        bool isQObject() const { return m_type_entry->isQObject(); }
        bool isQEvent() const { return m_type_entry->isQEvent(); }
        bool isQWidget() const { return m_type_entry->isQWidget(); }
        bool isQWindow() const { return m_type_entry->isQWindow(); }
        bool isQAction() const { return m_type_entry->isQAction(); }
        bool isQCoreApplication() const { return m_type_entry->isQCoreApplication(); }
        bool isQtNamespace() const { return isNamespace() && name() == "Qt"; }
        QString qualifiedCppName() const { return m_type_entry->qualifiedCppName(); }
        QString fullQualifiedCppName() const;

        bool hasSignals() const;
        bool inheritsFrom(const MetaClass *other) const;
        bool inheritsFromInterface(const MetaClass *other) const;

        void setHasVirtuals(bool on) { m_has_virtuals = on; }
        void setHasVirtualSlots(bool on) { m_has_virtual_slots = on; }
        bool generateShellClass() const;
        bool instantiateShellClass() const;
        void setNeedsOShellDestructor(bool b);
        bool needsOShellDestructor() const;

        bool hasVirtualSlots() const { return m_has_virtual_slots; }
        bool hasVirtualFunctions() const { return !isFinal() && m_has_virtuals; }
        bool hasVirtuals() const { return m_has_virtuals; }
        bool hasProtectedFunctions() const;
        bool hasProtectedConstructors() const;

        const QList<TypeEntry *>& templateArguments() const { return m_template_args; }
        void setTemplateArguments(const QList<TypeEntry *> &args) { m_template_args = args; }

        bool hasProtectedFieldAccessors() const;

        // only valid during metajavabuilder's run
        const QList<QPair<TypeInfo,int>>& baseClassTypeInfo() const { return m_base_class_typeinfo; }
        void setBaseClassTypeInfo(const QList<QPair<TypeInfo,int>> &names) { m_base_class_typeinfo = names; }

        MetaClass *primaryInterfaceImplementor() const { return m_primary_interface_implementor; }
        void setPrimaryInterfaceImplementor(MetaClass *cl) { m_primary_interface_implementor = cl; }

        const ComplexTypeEntry *typeEntry() const { return m_type_entry; }
        ComplexTypeEntry *typeEntry() { return m_type_entry; }
        void setTypeEntry(ComplexTypeEntry *type) { m_type_entry = type; }

        bool hasHashFunction() const;

        void setNeedsHashWorkaround(bool on) { m_needs_hash_workaround = on; }
        bool needsHashWorkaround() const { return m_needs_hash_workaround; }

        void setToStringCapability(const MetaFunction* fun) { m_qDebug_stream_function = fun; }
        const MetaFunction* toStringCapability() const { return m_qDebug_stream_function; }

        virtual bool hasDefaultToStringFunction() const;

        void setHasEqualsOperator(bool on) { m_has_equals_operator = on; }
        bool hasEqualsOperator() const;

        bool hasCloneOperator() const;

        void addPropertySpec(QPropertySpec *spec) { m_property_specs << spec; }
        const QList<QPropertySpec *>& propertySpecs() const { return m_property_specs; }

        QPropertySpec *propertySpecForRead(const QString &name) const;
        QPropertySpec *propertySpecForMember(const QString &name) const;
        QPropertySpec *propertySpecForWrite(const QString &name) const;
        QPropertySpec *propertySpecForReset(const QString &name) const;
        QPropertySpec *propertySpecForNotify(const QString &name) const;
        QPropertySpec *propertySpecForBindable(const QString &name) const;

        QList<ReferenceCount> referenceCounts() const;
        QList<TemplateInstantiation> templateInstantiations() const;

        void setEqualsFunctions(const MetaFunctionList &lst) { m_equals_functions = lst; }
        const MetaFunctionList& equalsFunctions() const { return m_equals_functions; }

        void setNotEqualsFunctions(const MetaFunctionList &lst) { m_nequals_functions = lst; }
        const MetaFunctionList& notEqualsFunctions() const { return m_nequals_functions; }

        void setLessThanFunctions(const MetaFunctionList &lst) { m_less_than_functions = lst; }
        const MetaFunctionList& lessThanFunctions() const { return m_less_than_functions; }

        void setCompareFunctions(const MetaFunctionList &lst) { m_compare_functions = lst; }
        const MetaFunctionList& compareFunctions() const { return m_compare_functions; }

        void setGreaterThanFunctions(const MetaFunctionList &lst) { m_greater_than_functions = lst; }
        const MetaFunctionList& greaterThanFunctions() const { return m_greater_than_functions; }

        void setLessThanEqFunctions(const MetaFunctionList &lst) { m_less_than_eq_functions = lst; }
        const MetaFunctionList& lessThanEqFunctions() const { return m_less_than_eq_functions; }

        void setGreaterThanEqFunctions(const MetaFunctionList &lst) { m_greater_than_eq_functions = lst; }
        const MetaFunctionList& greaterThanEqFunctions() const { return m_greater_than_eq_functions; }

        void setBeginFunctions(const MetaFunctionList &lst) { m_beginFunctions = lst; }
        const MetaFunctionList& beginFunctions() const { return m_beginFunctions; }

        void setEndFunctions(const MetaFunctionList &lst) { m_endFunctions = lst; }
        const MetaFunctionList& endFunctions() const { return m_endFunctions; }

        void sortFunctions();
        void sortEnums();
        void sortFields();
        void sortFunctionals();

        const ContainerTypeEntry* findContainerSuperClass(QList<const MetaType *>* instantiations = nullptr) const;

        const MetaClass *templateBaseClass() const { return m_template_base_class; }
        void setTemplateBaseClass(const MetaClass *cls) { m_template_base_class = cls; }

        const QList<const MetaType *>& templateBaseClassInstantiations() const { return m_template_base_class_instantiations; }
        void setTemplateBaseClassInstantiations(const QList<const MetaType *>& instantiations) { m_template_base_class_instantiations = instantiations; }

        void setTypeAlias(bool typeAlias) { m_is_type_alias = typeAlias; }
        bool isTypeAlias() const { return m_is_type_alias; }

        void setTypeAliasType(MetaType* typeAlias) { m_typeAliasType = typeAlias; }
        const MetaType* typeAliasType() const { return m_typeAliasType; }

        void setHas_Q_GADGET(bool isGadget){m_has_Q_GADGET = isGadget;}
        bool has_Q_GADGET() const {return m_has_Q_GADGET;}

        void setHas_Q_OBJECT(bool isOBJECT){m_has_Q_OBJECT = isOBJECT;}
        bool has_Q_OBJECT() const {return m_has_Q_OBJECT;}
        bool isDeprecated() const {return isDeclDeprecated() || typeEntry()->isDeprecated();}

        void setHasSubClasses(bool subClasses){m_has_subClasses = subClasses;}
        bool hasSubClasses() const {return m_has_subClasses;}
        void addFunctionalByUsing(QString _using, MetaFunctional* f){
            m_functionalByUsing[_using] = f;
        }
        const QMap<QString,MetaFunctional*>& functionalByUsing() const { return m_functionalByUsing; }

        const QMap<QString,MetaFunction*>& functionBySignature() const { return m_functionsBySignature; }

        void setDeprecatedComment(const QString &deprecatedComment) { m_deprecatedComment = deprecatedComment; }
        const QString& deprecatedComment() const { return m_deprecatedComment; }

        void setQHashScope(const QString &qHashScope) { m_qHashScope = qHashScope; }
        const QString& qHashScope() const { return m_qHashScope; }

        void setUsingProtectedBaseConstructors(bool usingBaseConstructors){m_usingProtectedBaseConstructors = usingBaseConstructors;}
        void setUsingPublicBaseConstructors(bool usingBaseConstructors){m_usingPublicBaseConstructors = usingBaseConstructors;}
        bool usingProtectedBaseConstructors() const {return m_usingProtectedBaseConstructors;}
        bool usingPublicBaseConstructors() const {return m_usingPublicBaseConstructors;}
        bool hasPaintMethod() const;
        bool hasQmlListProperty() const;
        MetaFunction* publicCopyConstructor() const;
        uint returnScopeRequired() const;
        void setProtectedUsingStatements(const QStringList& usingStatements) { m_protectedUsingStatements = usingStatements; }
        const QStringList& protectedUsingStatements() const{ return m_protectedUsingStatements; }
        void setPublicUsingStatements(const QStringList& usingStatements) { m_publicUsingStatements = usingStatements; }
        const QStringList& publicUsingStatements() const{ return m_publicUsingStatements; }

    private:
        QSet<QString> getAllUnimplmentablePureVirtualFunctions() const;

    private:
    uint m_namespace : 1;
    uint m_qobject : 1;
    uint m_has_virtuals : 1;
    uint m_has_nonpublic : 1;
    uint m_has_virtual_slots : 1;
    uint m_has_justprivateconstructors : 1;
    mutable int m_has_standardconstructor : 2;
    mutable int m_has_publicstandardconstructor : 2;
    mutable int m_has_explicitstandardconstructor : 2;
    mutable int m_publiccopyconstructor_requested : 1;
    mutable int m_has_explicitcopyconstructor : 2;
    mutable int m_has_publicassignment : 2;
    mutable MetaFunction* m_publiccopyconstructor = nullptr;
    uint m_has_unimplmentablePureVirtualFunctions : 1;
    QSet<QString> m_unimplmentablePureVirtualFunctions;
    uint m_has_metaObject : 1;
    uint m_has_metacall : 1;
    uint m_has_metacast : 1;
    uint m_has_private_metaObject : 1;
    uint m_has_private_metacall : 1;
    uint m_has_private_metacast : 1;
    uint m_needs_hash_workaround : 1;
    uint m_has_equals_operator : 1;
    uint m_is_type_alias : 1;
    uint m_has_Q_GADGET : 1;
    uint m_has_Q_OBJECT : 1;
    uint m_has_subClasses : 1;
    uint m_usingProtectedBaseConstructors : 1;
    uint m_usingPublicBaseConstructors : 1;
    uint m_needsOShellDestructor : 1;
    uint m_reserved : 14;

        const MetaClass *m_enclosing_class;
        MetaClass *m_base_class;
        MetaType* m_typeAliasType;
        const MetaClass *m_template_base_class;
        QList<const MetaType *> m_template_base_class_instantiations;
        MetaFunctionList m_functions;
        QMap<QString,MetaFunction*> m_functionsBySignature;
        MetaFunctionList m_invalidfunctions;
        MetaFunctionList m_deletedFunctions;
        MetaFieldList m_fields;
        MetaEnumList m_enums;
        MetaFunctionalList m_functionals;
        QMap<QString,MetaFunctional*> m_functionalByUsing;
        MetaClassList m_interfaces;
        MetaClassList m_enclosed_classes;
        MetaClass *m_extracted_interface;
        MetaClass *m_extracted_interface_impl;
        MetaClass *m_primary_interface_implementor;
        QList<QPropertySpec *> m_property_specs;
        MetaFunctionList m_equals_functions;
        MetaFunctionList m_nequals_functions;

        MetaFunctionList m_compare_functions;
        MetaFunctionList m_less_than_functions;
        MetaFunctionList m_greater_than_functions;
        MetaFunctionList m_less_than_eq_functions;
        MetaFunctionList m_greater_than_eq_functions;
        MetaFunctionList m_beginFunctions;
        MetaFunctionList m_endFunctions;

        QList<QPair<TypeInfo,int>> m_base_class_typeinfo;
        QList<TypeEntry *> m_template_args;
        ComplexTypeEntry *m_type_entry;
        const MetaFunction* m_qDebug_stream_function;
        QString m_deprecatedComment;
        QString m_qHashScope;
        uint m_returnScopeRequired = 0;
        QStringList m_protectedUsingStatements;
        QStringList m_publicUsingStatements;
};

class QPropertySpec {
    public:
        QPropertySpec()
            : m_name(),
                m_read(),
                m_write(),
                m_bindable(),
                m_designable(),
                m_scriptable(),
                m_reset(),
                m_notify(),
                m_stored(),
                m_revision(),
                m_user(),
                m_required(false),
                m_constant(false),
                m_final(false),
                m_index(-1) {
        }

        QString name() const { return m_name; }
        void setName(const QString &name) { m_name = name; }

        QString read() const { return m_read; }
        void setRead(const QString &read) { m_read = read; }

        QString write() const { return m_write; }
        void setWrite(const QString &write) { m_write = write; }

        QString bindable() const { return m_bindable; }
        void setBindable(const QString &bindable) { m_bindable = bindable; }

        QString designable() const { return m_designable; }
        void setDesignable(const QString &designable) { m_designable = designable; }

        QString scriptable() const { return m_scriptable; }
        void setScriptable(const QString &scriptable) { m_scriptable = scriptable; }

        QString reset() const { return m_reset; }
        void setReset(const QString &reset) { m_reset = reset; }

        QString notify() const { return m_notify; }
        void setNotify(const QString &notify) { m_notify = notify; }

        QString stored() const { return m_stored; }
        void setStored(const QString &stored) { m_stored = stored; }

        QString revision() const { return m_revision; }
        void setRevision(const QString &revision) { m_revision = revision; }

        QString user() const { return m_user; }
        void setUser(const QString &user) { m_user = user; }

        bool required() const { return m_required; }
        void setRequired(bool required) { m_required = required; }

        bool constant() const { return m_constant; }
        void setConstant(bool constant) { m_constant = constant; }

        bool final() const { return m_final; }
        void setFinal(bool final) { m_final = final; }

        QString member() const { return m_member; }
        void setMember(const QString& member) { m_member = member; }

        int index() const { return m_index; }
        void setIndex(int index) { m_index = index; }

    private:
        QString m_name;
        QString m_read;
        QString m_write;
        QString m_bindable;
        QString m_designable;
        QString m_scriptable;
        QString m_reset;
        QString m_notify;
        QString m_stored;
        QString m_revision;
        QString m_user;
        QString m_member;
        bool m_required;
        bool m_constant;
        bool m_final;
        int m_index;
};

inline MetaFunctionList MetaClass::allVirtualFunctions() const {
    return queryFunctions(VirtualFunctions
                          | NotRemovedFromTargetLang);
}

inline MetaFunctionList MetaClass::allFinalFunctions() const {
    return queryFunctions(FinalInTargetLangFunctions
                          | FinalInCppFunctions
                          | NotRemovedFromTargetLang);
}

inline MetaFunctionList MetaClass::cppInconsistentFunctions() const {
    return queryFunctions(Inconsistent
                          | NormalFunctions
                          | Visible
                          | NotRemovedFromTargetLang);
}

inline MetaFunctionList MetaClass::cppSignalFunctions() const {
    return queryFunctions(Signals
                          | NotRemovedFromTargetLang);
}

#endif // METALANG_H
