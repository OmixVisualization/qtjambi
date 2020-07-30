/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef ABSTRACTMETALANG_H
#define ABSTRACTMETALANG_H

#include "codemodel.h"

#include "typesystem/typesystem.h"

#include <QSet>
#include <QStringList>
#include <QTextStream>
#include "typesystem/typeentry.h"

class EnumTypeEntry;
class AbstractMeta;
class AbstractMetaClass;
class AbstractMetaField;
class AbstractMetaFunction;
class AbstractMetaType;
class AbstractMetaVariable;
class AbstractMetaArgument;
class AbstractMetaEnumValue;
class AbstractMetaEnum;
class QPropertySpec;
class AbstractMetaTemplateParameter;
class AbstractMetaBuilder;

typedef QList<AbstractMetaField *> AbstractMetaFieldList;
typedef QList<AbstractMetaArgument *> AbstractMetaArgumentList;
typedef QList<AbstractMetaFunction *> AbstractMetaFunctionList;
typedef QList<AbstractMetaTemplateParameter *> AbstractMetaTemplateParameterList;

enum ArgumentRemove{
    ArgumentRemove_No,
    ArgumentRemove_Remove,
    ArgumentRemove_UseAsLength
};

class AbstractMetaClassList : public  QList<AbstractMetaClass *> {
    public:
        enum NameFlag{
            QualifiedCppName = 0x0001,
            FullName = 0x0002,
            SimpleName = 0x0004,
            All = QualifiedCppName | FullName | SimpleName,
        };

        AbstractMetaClass *findClass(const QString &name, NameFlag nameFlag = All) const;
        AbstractMetaEnumValue *findEnumValue(const QString &string) const;
        AbstractMetaEnum *findEnum(const EnumTypeEntry *entry) const;

};

class AbstractMetaAttributes {
    public:
        AbstractMetaAttributes() : m_attributes(0), m_originalAttributes(0), m_href(), m_brief() { }

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

            Fake                        = 0x00020000,

            Invokable                   = 0x00040000,

            PullProtectedMethodsDown    = 0x00080000,

            DeclaredFinalInCpp          = 0x00100000,

            Annonymous                  = 0x00200000,

            Deprecated                  = 0x01000000,

            Comment                     = 0x00800000,

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
        const QString& href() const {return m_href;}
        const QString& brief() const {return m_brief;}

    private:
        uint m_attributes;
        uint m_originalAttributes;
        QString m_href;
        QString m_brief;
};


class AbstractMetaType {
    public:
        enum ReferenceType{
            NoReference,
            Reference,
            RReference
        };
        enum TypeUsagePattern {
            InvalidPattern,
            PrimitivePattern,
            FlagsPattern,
            EnumPattern,
            ValuePattern,
            StringPattern,
            Latin1StringPattern,
            StringViewPattern,
            StringRefPattern,
            CharPattern,
            ObjectPattern,
            QObjectPattern,
            NativePointerPattern,
            FunctionalPattern,
            ContainerPattern,
            IteratorPattern,
            PointerContainerPattern,
            InitializerListPattern,
            VariantPattern,
            JObjectWrapperPattern,
            JCollectionWrapperPattern,
            JMapWrapperPattern,
            JIteratorWrapperPattern,
            JEnumWrapperPattern,
            JQFlagsWrapperPattern,
            ArrayPattern,
            ThreadPattern,
            TemplateArgumentPattern,
            RValuePattern
        };

        AbstractMetaType() :
                m_type_entry(nullptr),
                m_array_element_count(0),
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
        void addInstantiation(const AbstractMetaType *inst) {
            m_instantiations << inst;
            if(typeEntry() && typeEntry()->isContainer()){
                const ContainerTypeEntry *ctype =
                    static_cast<const ContainerTypeEntry *>(typeEntry());
                if(ctype->type()==ContainerTypeEntry::ByteArrayListContainer){
                    setInstantiationInCpp(false);
                    Q_ASSERT(m_instantiations.size()==1 && m_instantiations[0] && m_instantiations[0]->typeEntry()->qualifiedCppName()=="QByteArray");
                }
                if(ctype->type()==ContainerTypeEntry::StringListContainer){
                    setInstantiationInCpp(false);
                    Q_ASSERT(m_instantiations.size()==1 && m_instantiations[0] && m_instantiations[0]->typeEntry()->qualifiedCppName()=="QString");
                }
            }
        }
        void setInstantiations(const QList<const AbstractMetaType *> &insts) {
            m_instantiations = insts;
            if(m_instantiations.size()==1 && typeEntry() && typeEntry()->isContainer()){
                const ContainerTypeEntry *ctype =
                    static_cast<const ContainerTypeEntry *>(typeEntry());
                if(ctype->type()==ContainerTypeEntry::ByteArrayListContainer){
                    setInstantiationInCpp(false);
                    Q_ASSERT(m_instantiations[0] && m_instantiations[0]->typeEntry()->qualifiedCppName()=="QByteArray");
                }
                if(ctype->type()==ContainerTypeEntry::StringListContainer){
                    setInstantiationInCpp(false);
                    Q_ASSERT(m_instantiations[0] && m_instantiations[0]->typeEntry()->qualifiedCppName()=="QString");
                }
            }
        }
        const QList<const AbstractMetaType *>& instantiations() const { return m_instantiations; }
        void addIteratorInstantiation(const AbstractMetaType *inst) { m_iteratorInstantiations << inst; }
        void setIteratorInstantiations(const QList<const AbstractMetaType *> &insts) { m_iteratorInstantiations = insts; }
        const QList<const AbstractMetaType *>& iteratorInstantiations() const { return m_iteratorInstantiations; }
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

        // returns true if the type is used as an enum
        bool isEnum() const { return m_pattern == EnumPattern; }

        // returns true if the type is used as a functional
        bool isFunctional() const { return m_pattern == FunctionalPattern; }

        // returns true if the type is used as a QObject *
        bool isQObject() const { return m_pattern == QObjectPattern; }

        // returns true if the type is used as an object, e.g. Xxx *
        bool isObject() const { return m_pattern == ObjectPattern; }

        // returns true if the type is used as an array, e.g. Xxx[42]
        bool isArray() const { return m_pattern == ArrayPattern; }

        // returns true if the type is used as an initializer list
        bool isInitializerList() const { return m_pattern == InitializerListPattern; }

        // returns true if the type is used as a value type (X or const X &)
        bool isValue() const { return m_pattern == ValuePattern; }

        // returns true if the type is used as an R-value type (X&&)
        bool isRValue() const { return m_pattern == RValuePattern; }

        // returns true for more complex types...
        bool isNativePointer() const { return m_pattern == NativePointerPattern; }

        // returns true if the type was originally a QString or const QString &
        bool isTargetLangString() const { return m_pattern == StringPattern; }

        // returns true if the type was originally a QLatin1String
        bool isTargetLangLatin1String() const { return m_pattern == Latin1StringPattern; }

        // returns true if the type was originally a QString or const QString & or equivalent for QLatin1String
        bool isTargetLangStringView() const { return m_pattern == StringViewPattern; }

        // returns true if the type was originally a QStringRef or const QStringRef &
        bool isTargetLangStringRef() const { return m_pattern == StringRefPattern; }

        // returns true if the type was originally a QChar or const QChar &
        bool isTargetLangChar() const { return m_pattern == CharPattern; }

        // return true if the type was originally a QVariant or const QVariant &
        bool isVariant() const { return m_pattern == VariantPattern; }

        // return true if the type was originally a JObjectWrapper or const JObjectWrapper &
        bool isJObjectWrapper() const { return m_pattern == JObjectWrapperPattern; }

        // return true if the type was originally a JMapWrapper or const JMapWrapper &
        bool isJMapWrapper() const { return m_pattern == JMapWrapperPattern; }

        // return true if the type was originally a JCollectionWrapper or const JCollectionWrapper &
        bool isJCollectionWrapper() const { return m_pattern == JCollectionWrapperPattern; }

        // return true if the type was originally a JIteratorWrapper or const JIteratorWrapper &
        bool isJIteratorWrapper() const { return m_pattern == JIteratorWrapperPattern; }

        // return true if the type was originally a JEnumWrapper or const JEnumWrapper &
        bool isJEnumWrapper() const { return m_pattern == JEnumWrapperPattern; }

        // return true if the type was originally a JQFlagsWrapper or const JQFlagsWrapper &
        bool isJQFlagsWrapper() const { return m_pattern == JQFlagsWrapperPattern; }

        // returns true if the type was used as a container
        bool isContainer() const { return m_pattern == ContainerPattern; }

        // returns true if the type was used as an iterator
        bool isIterator() const { return m_pattern == IteratorPattern; }

        // returns true if the type was used as a qpointer
        bool isPointerContainer() const { return m_pattern == PointerContainerPattern; }

        // returns true if the type was used as a flag
        bool isFlags() const { return m_pattern == FlagsPattern; }

        // returns true if the type was used as a thread
        bool isThread() const { return m_pattern == ThreadPattern; }

        bool isConstant() const { return m_constant; }
        void setConstant(bool constant) { m_constant = constant; }

        bool isVolatile() const { return m_volatile; }
        void setVolatile(bool vol) { m_volatile = vol; }

        ReferenceType getReferenceType() const { return m_reference; }
        void setReferenceType(ReferenceType ref) { m_reference = ref; }

        // Returns true if the type is to be implemented using Java enums, e.g. not plain ints.
        bool isTargetLangEnum() const { return isEnum() && !static_cast<const EnumTypeEntry *>(typeEntry())->forceInteger(); }
        bool isIntegerEnum() const { return isEnum() && !isTargetLangEnum(); }

        // Returns true if the type is to be implemented using Java QFlags, e.g. not plain ints.
        bool isTargetLangFlags() const {
            return isFlags() && !static_cast<const FlagsTypeEntry *>(typeEntry())->forceInteger();
        }
        bool isIntegerFlags() const { return isFlags() && !isTargetLangFlags(); }

        int actualIndirections() const { return m_indirections.size() + (m_reference==Reference ? 1 : 0); }
        const QList<bool>& indirections() const { return m_indirections; }
        void setIndirections(const QList<bool>& indirections) { m_indirections = indirections; }

        void setArrayElementCount(int n) { m_array_element_count = n; }
        int arrayElementCount() const { return m_array_element_count; }

        AbstractMetaType *arrayElementType() const { return m_array_element_type; }
        void setArrayElementType(AbstractMetaType *t) { m_array_element_type = t; }

        QString cppSignature() const;

        AbstractMetaType *copy() const;

        const TypeEntry *typeEntry() const { return m_type_entry; }
        void setTypeEntry(const TypeEntry *type) { m_type_entry = type; }

        void setOriginalTypeDescription(const QString &otd) { m_original_type_description = otd; }
        QString originalTypeDescription() const { return m_original_type_description; }

        void setOriginalTemplateType(const AbstractMetaType *type) { m_original_template_type = type; }
        const AbstractMetaType *originalTemplateType() const { return m_original_template_type; }

    private:
        const TypeEntry *m_type_entry;
        QList <const AbstractMetaType *> m_instantiations;
        QList <const AbstractMetaType *> m_iteratorInstantiations;
        QString m_package;
        QString m_original_type_description;

        int m_array_element_count;
        AbstractMetaType *m_array_element_type;
        const AbstractMetaType *m_original_template_type;

        TypeUsagePattern m_pattern;
    uint m_constant : 1;
    uint m_volatile : 1;
    ReferenceType m_reference;
    uint m_cpp_instantiation : 1;
    uint m_force_boxed_primitives : 1;
    uint m_reserved : 24; // unused
    QList<bool> m_indirections;
};

class AbstractMetaVariable {
    public:
        AbstractMetaVariable() : m_type(nullptr) { }

        AbstractMetaType *type() const { return m_type; }
        void setType(AbstractMetaType *type) { m_type = type; }

        QString name() const { return m_name; }
        void setName(const QString &name) { m_name = name; }

    private:
        QString m_name;
        AbstractMetaType *m_type;
};

class AbstractMetaTemplateParameter : public AbstractMetaVariable {
public:
    AbstractMetaTemplateParameter(){}

    QString instantiation() const { return m_instantiation; }
    void setInstantiation(const QString &expr) { m_instantiation = expr; }
    AbstractMetaTemplateParameter *copy() const;
private:
    QString m_instantiation;
};

class AbstractMetaArgument : public AbstractMetaVariable {
    public:
        AbstractMetaArgument() : m_argument_index(0) { }
        virtual ~AbstractMetaArgument(){}

        const QString& defaultValueExpression() const { return m_expression; }
        void setDefaultValueExpression(const QString &expr) { m_expression = expr; }

        const QString& originalDefaultValueExpression() const { return m_original_expression; }
        void setOriginalDefaultValueExpression(const QString &expr) { m_original_expression = expr; }

        QString toString() const {
            return type()->name() + " " + AbstractMetaVariable::name() +
                   (m_expression.isEmpty() ? "" :  " = " + m_expression);
        }

        int argumentIndex() const { return m_argument_index; }
        void setArgumentIndex(int argIndex) { m_argument_index = argIndex; }

        virtual QString argumentName() const; // is overridden by java meta type to exclude java key words
        QString modifiedArgumentName() const;
        QString indexedName() const;
        void setModifiedName(const QString &javaName) { m_modifiedName = javaName; }

        AbstractMetaArgument *copy() const;

    private:
        // Just to force people to call argumentName() And indexedName();
        QString name() const;

        QString m_modifiedName;
        QString m_expression;
        QString m_original_expression;
        int m_argument_index;
};


class AbstractMetaField : public AbstractMetaVariable, public AbstractMetaAttributes {
    public:
        AbstractMetaField();
        ~AbstractMetaField();

        const AbstractMetaClass *enclosingClass() const { return m_class; }
        void setEnclosingClass(const AbstractMetaClass *cls) { m_class = cls; }

        const AbstractMetaFunction *getter() const;
        const AbstractMetaFunction *setter() const;

        FieldModificationList modifications() const;

        AbstractMetaField *copy() const;

    private:
        mutable AbstractMetaFunction *m_getter;
        mutable AbstractMetaFunction *m_setter;
        const AbstractMetaClass *m_class;
};


class AbstractMetaFunction : public AbstractMetaAttributes {
    public:
        enum FunctionType {
            ConstructorFunction,
            DestructorFunction,
            NormalFunction,
            SignalFunction,
            PrivateSignalFunction,
            EmptyFunction,
            SlotFunction,
            GlobalScopeFunction
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

        AbstractMetaFunction()
                :AbstractMetaAttributes(),
                m_original_name(),
                m_original_signature(),
                m_function_type(NormalFunction),
                m_type(nullptr),
                m_class(nullptr),
                m_implementing_class(nullptr),
                m_declaring_class(nullptr),
                m_declaring_template(),
                m_interface_class(nullptr),
                m_property_spec(nullptr),
                m_constant(false),
                m_variadics(false),
                m_invalid(false),
                m_actualMinimumArgumentCount(-1),
                m_accessedField(nullptr),
                m_jumptable_id(-1) {
        }

        ~AbstractMetaFunction();

        QString name() const { return m_name; }
        void setName(const QString &name) { m_name = name; }

        QString originalName() const { return m_original_name.isEmpty() ? name() : m_original_name; }
        void setOriginalName(const QString &name) { m_original_name = name; }

        QString modifiedName() const;

        QString minimalSignature() const;
        void setOriginalSignature(const QString &signature) { m_original_signature = signature; }
        const QString& originalSignature() const { return m_original_signature; }
        QStringList possibleIntrospectionCompatibleSignatures() const;

        QString marshalledName() const;

        // true if one or more of the arguments are of QtObject subclasses
        bool argumentsHaveNativeId() const {
            for(const AbstractMetaArgument *arg : m_arguments) {
                if (arg->type()->hasNativeId())
                    return true;
            }

            return false;
        }

        bool isModifiedRemoved(int types = TypeSystem::All) const;

        AbstractMetaType *type() const { return m_type; }
        void setType(AbstractMetaType *type) { m_type = type; }

        // The class that has this function as a member.
        const AbstractMetaClass *ownerClass() const { return m_class; }
        void setOwnerClass(const AbstractMetaClass *cls) { m_class = cls; }

        // The first class in a hierarchy that declares the function
        const AbstractMetaClass *declaringClass() const { return m_declaring_class; }
        void setDeclaringClass(const AbstractMetaClass *cls) { m_declaring_class = cls; }

        const QString &declaringTemplate() const { return m_declaring_template; }
        void setDeclaringTemplate(const QString &declaring_template) { m_declaring_template = declaring_template; }

        // The class that actually implements this function
        const AbstractMetaClass *implementingClass() const { return m_implementing_class; }
        void setImplementingClass(const AbstractMetaClass *cls) { m_implementing_class = cls; }

        bool needsCallThrough() const;

        const AbstractMetaArgumentList& arguments() const { return m_arguments; }
        void setArguments(const AbstractMetaArgumentList &arguments) { m_arguments = arguments; m_actualMinimumArgumentCount = -1; }
        void addArgument(AbstractMetaArgument *argument) { m_arguments << argument; m_actualMinimumArgumentCount = -1; }
        int actualMinimumArgumentCount() const;

        void setInvalid(bool on) { m_invalid = on; }
        bool isInvalid() const { return m_invalid; }
        bool isDeprecated() const;
        bool isDestructor() const { return functionType() == DestructorFunction; }
        bool isConstructor() const { return functionType() == ConstructorFunction; }

        /**
         *detects, if the constructor is of form T(const T&)
         * @brief isCopyConstructor
         * @return
         */
        bool isCopyConstructor() const;
        bool hasRReferences() const;
        bool isNormal() const { return functionType() == NormalFunction || isSlot() || isInGlobalScope(); }
        bool isInGlobalScope() const { return functionType() == GlobalScopeFunction; }
        bool isSignal() const { return functionType() == SignalFunction || functionType() == PrivateSignalFunction; }
        bool isPrivateSignal() const { return functionType() == PrivateSignalFunction; }
        bool isSlot() const { return functionType() == SlotFunction; }
        bool isEmptyFunction() const { return functionType() == EmptyFunction; }
        bool hasTemplateArgumentTypes() const{
            for(const AbstractMetaArgument *arg : m_arguments) {
                if (arg->type()->isTemplateArgument())
                    return true;
            }
            return m_type && m_type->isTemplateArgument();
        }
        FunctionType functionType() const { return m_function_type; }
        void setFunctionType(FunctionType type) { m_function_type = type; }

        QStringList introspectionCompatibleSignatures(const QStringList &resolvedArguments = QStringList()) const;
        QString signature(bool skipName = false) const;
        QString targetLangSignature(bool minimal = false) const;
        bool shouldReturnThisObject() const { return QLatin1String("this") == argumentReplaced(0); }

        bool isConstant() const { return m_constant; }
        void setConstant(bool constant) { m_constant = constant; }

        QString toString() const { return m_name; }

        uint compareTo(const AbstractMetaFunction *other) const;

        bool operator <(const AbstractMetaFunction &a) const;

        AbstractMetaFunction *copy() const;

        QString replacedDefaultExpression(const AbstractMetaClass *cls, int idx) const;
        bool removedDefaultExpression(const AbstractMetaClass *cls, int idx) const;
        QString conversionRule(TypeSystem::Language language, int idx) const;
        bool hasConversionRule(TypeSystem::Language language, int idx) const;
        QList<ReferenceCount> referenceCounts(const AbstractMetaClass *cls, int idx = -2) const;
        QList<TemplateInstantiation> templateInstantiations(const AbstractMetaClass *cls) const;

        bool nullPointersDisabled(const AbstractMetaClass *cls = nullptr, int argument_idx = 0) const;
        QString nullPointerDefaultValue(const AbstractMetaClass *cls = nullptr, int argument_idx = 0) const;

        bool resetObjectAfterUse(int argument_idx) const;

        // Returns the ownership rules for the given argument in the given context
        OwnershipRule ownership(const AbstractMetaClass *cls, TypeSystem::Language language, int idx) const;

        bool isVirtualSlot() const;
        bool isAllowedAsSlot() const;
        bool isThreadAffine() const;
        bool isUIThreadAffine() const;

        int argumentTypeArrayLengthIndex(int argument_index) const;
        int argumentTypeArrayLengthMinValue(int argument_index) const;
        int argumentTypeArrayLengthMaxValue(int argument_index) const;
        bool argumentTypeArrayDeref(int key) const;
        bool argumentTypeArray(int key) const;
        bool argumentTypeBuffer(int key) const;
        bool argumentTypeArrayVarArgs(int key) const;
        bool isNoExcept() const;
        QString typeReplaced(int argument_index, QString* jniType = nullptr) const;
        bool isRemovedFromAllLanguages(const AbstractMetaClass *) const;
        bool isRemovedFrom(const AbstractMetaClass *, TypeSystem::Language language) const;

        ArgumentRemove argumentRemoved(int) const;

        QList<const ArgumentModification*> addedArguments() const;

        QString argumentReplaced(int key) const;
        bool needsSuppressUncheckedWarning() const;

        bool hasModifications(const AbstractMetaClass *implementor) const;
        FunctionModificationList modifications(const AbstractMetaClass *implementor) const;
        bool hasCodeInjections(const AbstractMetaClass *implementor, TypeSystem::Language language, const QSet<CodeSnip::Position>& positions) const;
        QString throws(const AbstractMetaClass *implementor = nullptr) const;

        // If this function stems from an interface, this returns the
        // interface that declares it.
        const AbstractMetaClass *interfaceClass() const { return m_interface_class; }
        void setInterfaceClass(const AbstractMetaClass *cl) { m_interface_class = cl; }

        void setPropertySpec(QPropertySpec *spec) { m_property_spec = spec; }
        QPropertySpec *propertySpec() const { return m_property_spec; }

        int jumpTableId() const { return m_jumptable_id; }
        void setJumpTableId(int id) { m_jumptable_id = id; }

        const AbstractMetaTemplateParameterList & templateParameters() const {
            return m_templateParameters;
        }

        void setTemplateParameters(const AbstractMetaTemplateParameterList & templateParameters) {
            m_templateParameters = templateParameters;
        }

        void addTemplateParameter(AbstractMetaTemplateParameter* templateParameter) {
            m_templateParameters << templateParameter;
        }

        const AbstractMetaField *accessedField() const { return m_accessedField; }

        void setAccessedField(const AbstractMetaField *accessedField) { m_accessedField = accessedField; }
        void setDeprecatedComment(const QString &deprecatedComment) { m_deprecatedComment = deprecatedComment; }
        const QString& deprecatedComment() const { return m_deprecatedComment; }

        bool isVariadics() const { return m_variadics; }
        void setVariadics(bool isVariadics) { m_variadics = isVariadics; }

    private:
        QString m_name;
        QString m_original_name;
        QString m_original_signature;
        mutable QString m_cached_full_signature;
        mutable QString m_cached_full_signature_no_name;
        mutable QString m_cached_minimal_signature;
        mutable QString m_cached_modified_name;

        FunctionType m_function_type;
        AbstractMetaType *m_type;
        const AbstractMetaClass *m_class;
        const AbstractMetaClass *m_implementing_class;
        const AbstractMetaClass *m_declaring_class;
        QString m_declaring_template;
        const AbstractMetaClass *m_interface_class;
        QPropertySpec *m_property_spec;
        AbstractMetaArgumentList m_arguments;
        AbstractMetaTemplateParameterList m_templateParameters;
        uint m_constant          : 1;
        uint m_variadics         : 1;
        uint m_invalid           : 1;
        mutable int m_actualMinimumArgumentCount;
        const AbstractMetaField *m_accessedField;
        int m_jumptable_id;
        QString m_deprecatedComment;
};

class AbstractMetaEnum;

class AbstractMetaEnumValue {
    public:
        AbstractMetaEnumValue()
                : m_value(), _M_deprecated(false) {
        }

        const QVariant& value() const { return m_value; }
        void setValue(const QVariant& value) { m_value = value; }

        const QString& stringValue() const { return m_string_value; }
        void setStringValue(const QString &v) { m_string_value = v; }

        const QString& name() const { return m_name; }
        void setName(const QString &name) { m_name = name; }
        QString cppName() const;

        const AbstractMetaEnum * getEnum(){ return m_enum;}
        void setEnum(const AbstractMetaEnum * enm) {m_enum = enm;}
        bool deprecated() const { return _M_deprecated; }
        void setDeprecated(bool value) { _M_deprecated = value; }
        void setDeprecatedComment(const QString &deprecatedComment) { m_deprecatedComment = deprecatedComment; }
        const QString& deprecatedComment() const { return m_deprecatedComment; }

    private:
        const AbstractMetaEnum * m_enum;
        QString m_name;
        QString m_string_value;

        QVariant m_value;
        bool _M_deprecated;
        QString m_deprecatedComment;
};


class AbstractMetaEnumValueList : public QList<AbstractMetaEnumValue *> {
    public:
        AbstractMetaEnumValue *find(const QString &name) const;
};

class AbstractMetaEnum : public AbstractMetaAttributes {
    public:
        AbstractMetaEnum() : AbstractMetaAttributes(), m_type_entry(nullptr), m_class(nullptr), m_has_qenums_declaration(false) {}

        const AbstractMetaEnumValueList& values() const { return m_enum_values; }
        void addEnumValue(AbstractMetaEnumValue *enumValue) { m_enum_values << enumValue; }

        QString name() const { return m_type_entry->targetLangName(); }
        QString qualifier() const { return m_type_entry->javaQualifier(); }
        QString package() const { return m_type_entry->javaPackage(); }
        QString targetTypeSystem() const { return m_type_entry->targetTypeSystem(); }
        QString fullName() const { return qualifier().isEmpty() ? package() + "." + name() : package() + "." + qualifier()  + "." + name(); }

        // Has the enum been declared inside a Q_ENUMS() macro in its enclosing class?
        void setHasQEnumsDeclaration(bool on) { m_has_qenums_declaration = on; }
        bool hasQEnumsDeclaration() const { return m_has_qenums_declaration; }

        EnumTypeEntry *typeEntry() const { return m_type_entry; }
        void setTypeEntry(EnumTypeEntry *entry) { m_type_entry = entry; }

        AbstractMetaClass *enclosingClass() const { return m_class; }
        void setEnclosingClass(AbstractMetaClass *c) { m_class = c; }
        const QString& baseTypeName() const { return m_base_type_name; }
        void setBaseTypeName(const QString &names) { m_base_type_name = names; }
        void setDeprecatedComment(const QString &deprecatedComment) { m_deprecatedComment = deprecatedComment; }
        const QString& deprecatedComment() const { return m_deprecatedComment; }
    private:
        QString m_base_type_name;
        AbstractMetaEnumValueList m_enum_values;
        EnumTypeEntry *m_type_entry;
        AbstractMetaClass *m_class;
        QString m_deprecatedComment;

    uint m_has_qenums_declaration : 1;
};

class AbstractMetaFunctional : public AbstractMetaAttributes {
    public:
        AbstractMetaFunctional() : AbstractMetaAttributes(), m_base_type_name(), m_type_entry(nullptr), m_class(nullptr), m_type(nullptr), m_arguments(), m_isFunctionPointer(false) {}

        QString name() const { return m_type_entry->targetLangName(); }
        QString qualifier() const { return m_type_entry->javaQualifier(); }
        QString package() const { return m_type_entry->javaPackage(); }
        QString targetTypeSystem() const { return m_type_entry->targetTypeSystem(); }
        QString fullName() const { return qualifier().isEmpty() ? package() + "." + name() : package() + "." + qualifier()  + "." + name(); }

        FunctionalTypeEntry *typeEntry() const { return m_type_entry; }
        void setTypeEntry(FunctionalTypeEntry *entry) { m_type_entry = entry; }

        AbstractMetaClass *enclosingClass() const { return m_class; }
        void setEnclosingClass(AbstractMetaClass *c) { m_class = c; }
        const QString& baseTypeName() const { return m_base_type_name; }
        void setBaseTypeName(const QString &names) { m_base_type_name = names; }
        const QString& typeSignature() const { return m_typeSignature; }
        void setTypeSignature(const QString& type) { m_typeSignature = type; }
        AbstractMetaType * type() const { return m_type; }
        void setType(AbstractMetaType * type) { m_type = type; }
        const QList<AbstractMetaArgument *>& arguments() const { return m_arguments; }
        void addArgument(AbstractMetaArgument * type) { m_arguments << type; }
        ArgumentRemove argumentRemoved(int) const;
        int argumentTypeArrayLengthIndex(int key) const;
        int argumentTypeArrayLengthMinValue(int argument_index) const;
        int argumentTypeArrayLengthMaxValue(int argument_index) const;
        bool argumentTypeArrayDeref(int key) const;
        bool argumentTypeArray(int key) const;
        bool argumentTypeBuffer(int key) const;
        bool argumentTypeArrayVarArgs(int key) const;
        QString typeReplaced(int argument_index, QString* jniType = nullptr) const;
        QString conversionRule(TypeSystem::Language language, int idx) const;
        bool hasConversionRule(TypeSystem::Language language, int idx) const;
        bool resetObjectAfterUse(int argument_idx) const;
        bool isFunctionPointer() const { return m_isFunctionPointer; }
        void setFunctionPointer(bool isFunctionPointer) { m_isFunctionPointer = isFunctionPointer; }

    private:
        QString m_base_type_name;
        FunctionalTypeEntry *m_type_entry;
        AbstractMetaClass *m_class;
        AbstractMetaType *m_type;
        QList<AbstractMetaArgument *> m_arguments;
        QString m_typeSignature;
        uint m_isFunctionPointer : 1;
};

typedef QList<AbstractMetaEnum *> AbstractMetaEnumList;

typedef QList<AbstractMetaFunctional *> AbstractMetaFunctionalList;

class AbstractMetaClass : public AbstractMetaAttributes {
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

        AbstractMetaClass()
                : AbstractMetaAttributes(),
                  m_namespace(false),
                m_qobject(false),
                m_has_virtuals(false),
                m_has_nonpublic(false),
                m_has_virtual_slots(false),
                m_has_justprivateconstructors(false),
                m_has_standardconstructor(0),
                m_has_unimplmentablePureVirtualFunctions(false),
                m_unimplmentablePureVirtualFunctions(),
                m_functions_fixed(false),
                m_has_public_destructor(true),
                m_has_private_destructor(false),
                m_has_private_metaObject(false),
                m_has_private_metacall(false),
                m_has_virtual_destructor(false),
                m_has_hash_function(false),
                m_has_equals_operator(false),
                m_has_clone_operator(false),
                m_is_type_alias(false),
                m_has_Q_GADGET(false),
                m_has_Q_OBJECT(false),
                m_has_subClasses(false),
                m_enclosing_class(nullptr),
                m_base_class(nullptr),
                m_typeAliasType(nullptr),
                m_template_base_class(nullptr),
                m_template_base_class_instantiations(),
                m_extracted_interface(nullptr),
                m_extracted_interface_impl(nullptr),
                m_primary_interface_implementor(nullptr),
                m_type_entry(nullptr),
                m_qDebug_stream_function(nullptr){
        }

        virtual ~AbstractMetaClass();

        AbstractMetaClass *extractInterface();
        AbstractMetaClass *extractInterfaceImpl();
        AbstractMetaClass *extractInterface()const;
        AbstractMetaClass *extractInterfaceImpl()const;
        void fixFunctions(std::function<AbstractMetaArgument*(TypeEntry *)> argumentCreator);
        void fixUnimplmentablePureVirtualFunctions();

        const AbstractMetaFunctionList& functions() const { return m_functions; }
        const AbstractMetaFunctionList& invalidFunctions() const { return m_invalidfunctions; }
        void setFunctions(const AbstractMetaFunctionList &functions);
        void addFunction(AbstractMetaFunction *function);
        bool hasFunction(const AbstractMetaFunction *f) const;
        bool hasFunction(const QString &str) const;
        bool hasSignal(const AbstractMetaFunction *f) const;
        void addInvalidFunction(AbstractMetaFunction *function);

        bool hasConstructors() const;

        void addDefaultConstructor();

        bool hasJustPrivateConstructors() const { return m_has_justprivateconstructors; }
        void setHasJustPrivateConstructors(bool on) { m_has_justprivateconstructors = on; }
        bool hasPublicStandardConstructor() const;
        bool hasStandardConstructor() const;
        bool hasUnimplmentablePureVirtualFunction() const {return m_has_unimplmentablePureVirtualFunctions;}
        const QSet<QString>& unimplmentablePureVirtualFunctions() const {return m_unimplmentablePureVirtualFunctions;}
        void setUnimplmentablePureVirtualFunctions(const QSet<QString>& privatePureVirtualFunctions) { m_unimplmentablePureVirtualFunctions = privatePureVirtualFunctions; }
        bool hasPrivateMetaObject() const { return m_has_private_metaObject; }
        bool hasPrivateMetaCall() const { return m_has_private_metacall; }
        bool hasPrivateDestructor() const { return m_has_private_destructor; }
        bool hasVirtualDestructor() const {
            if(baseClass() && baseClass()->hasVirtualDestructor())
                return true;
            for(AbstractMetaClass* iface : interfaces()){
                if(iface->hasVirtualDestructor())
                    return true;
            }
            return m_has_virtual_destructor;
        }
        void setHasPublicDestructor(bool on) { m_has_public_destructor = on; }
        bool hasPublicDestructor() const { return m_has_public_destructor; }

        AbstractMetaFunctionList queryFunctionsByName(const QString &name) const;
        AbstractMetaFunctionList queryFunctionsByOriginalName(const QString &name) const;
        AbstractMetaFunctionList queryOtherFunctions(const AbstractMetaFunction* fun) const;
        AbstractMetaFunctionList queryFunctions(uint query) const;
        inline AbstractMetaFunctionList allVirtualFunctions() const;
        inline AbstractMetaFunctionList allFinalFunctions() const;
        AbstractMetaFunctionList functionsInTargetLang() const;
        AbstractMetaFunctionList functionsInShellClass() const;
        inline AbstractMetaFunctionList cppInconsistentFunctions() const;
        inline AbstractMetaFunctionList cppSignalFunctions() const;
        AbstractMetaFunctionList publicOverrideFunctions() const;
        AbstractMetaFunctionList virtualOverrideFunctions() const;
        AbstractMetaFunctionList virtualFunctions() const;
        AbstractMetaFunctionList implementableFunctions() const;
        AbstractMetaFunctionList nonVirtualShellFunctions() const;

        const AbstractMetaFieldList& fields() const { return m_fields; }
        void setFields(const AbstractMetaFieldList &fields) { m_fields = fields; }
        void addField(AbstractMetaField *field) { m_fields << field; }

        const AbstractMetaEnumList& enums() const { return m_enums; }
        void setEnums(const AbstractMetaEnumList &enums) { m_enums = enums; }
        void addEnum(AbstractMetaEnum *e) { m_enums << e; }

        const AbstractMetaFunctionalList& functionals() const { return m_functionals; }
        void setFunctionals(const AbstractMetaFunctionalList &functionals) { m_functionals = functionals; }
        void addFunctional(AbstractMetaFunctional *e) {
            m_functionals << e;
            e->setEnclosingClass(this);
        }

        AbstractMetaEnum *findEnum(const QString &enumName);
        AbstractMetaFunctional *findFunctional(const QString &functionalName);
        AbstractMetaEnum *findEnumForValue(const QString &enumName);
        AbstractMetaEnumValue *findEnumValue(const QString &enumName, AbstractMetaEnum *meta_enum);
        AbstractMetaClass *findIterator(const IteratorTypeEntry* iteratorType) const;

        const AbstractMetaClassList& interfaces() const { return m_interfaces; }
        void addInterface(AbstractMetaClass *interface);
        void setInterfaces(const AbstractMetaClassList &interface);

        const AbstractMetaClassList& enclosedClasses() const { return m_enclosed_classes; }
        void addEnclosedClass(AbstractMetaClass *enclosed_class);
        void setEnclosedClasses(const AbstractMetaClassList &enclosed_classes);

        virtual QString fullName() const;
        virtual QString name() const;

        /**
         * returns the simple name of an embedded class. of the complete name in case of top level classes.
         * @brief simpleName
         * @return
         */
        QString simpleName() const;

        QString baseClassName() const { return m_base_class ? m_base_class->name() : QString(); }

        AbstractMetaClass *baseClass() const { return m_base_class; }
        AbstractMetaClass *baseClass() { return m_base_class; }
        void setBaseClass(AbstractMetaClass *base_class) {
            m_base_class = base_class;
            if(base_class)
                base_class->m_has_subClasses = true;
        }

        const AbstractMetaClass *enclosingClass() const { return m_enclosing_class; }
        void setEnclosingClass(AbstractMetaClass *cl) { m_enclosing_class = cl; }

        QString package() const { return m_type_entry->javaPackage(); }
        QString targetTypeSystem() const { return m_type_entry->targetTypeSystem(); }
        bool isInterface() const { return m_type_entry->isInterface(); }
        bool isNamespace() const { return m_type_entry->isNamespace(); }
        bool isQObject() const { return m_type_entry->isQObject(); }
        bool isQtNamespace() const { return isNamespace() && name() == "Qt"; }
        QString qualifiedCppName() const { return m_type_entry->qualifiedCppName(); }

        bool hasSignals() const;
        bool inheritsFrom(const AbstractMetaClass *other) const;
        bool inheritsFromInterface(const AbstractMetaClass *other) const;

        void setHasPrivateDestructor(bool on) { m_has_private_destructor = on; }
        void setHasVirtualDestructor(bool on) { m_has_virtuals |= on; m_has_virtual_destructor = on; }
        void setHasVirtuals(bool on) { m_has_virtuals = on; }
        void setHasVirtualSlots(bool on) { m_has_virtual_slots = on; }
        bool generateShellClass() const;
        bool instantiateShellClass() const;

        bool hasVirtualSlots() const { return m_has_virtual_slots; }
        bool hasVirtualFunctions() const { return !isFinal() && m_has_virtuals; }
        bool hasVirtuals() const { return m_has_virtuals; }
        bool hasProtectedFunctions() const;
        bool hasProtectedConstructors() const;

        const QList<TypeEntry *>& templateArguments() const { return m_template_args; }
        void setTemplateArguments(const QList<TypeEntry *> &args) { m_template_args = args; }

        bool hasProtectedFieldAccessors() const;

        // only valid during metajavabuilder's run
        const QStringList& baseClassNames() const { return m_base_class_names; }
        void setBaseClassNames(const QStringList &names) { m_base_class_names = names; }

        AbstractMetaClass *primaryInterfaceImplementor() const { return m_primary_interface_implementor; }
        void setPrimaryInterfaceImplementor(AbstractMetaClass *cl) { m_primary_interface_implementor = cl; }

        const ComplexTypeEntry *typeEntry() const { return m_type_entry; }
        ComplexTypeEntry *typeEntry() { return m_type_entry; }
        void setTypeEntry(ComplexTypeEntry *type) { m_type_entry = type; }

        void setHasHashFunction(bool on) { m_has_hash_function = on; }
        bool hasHashFunction() const { return m_has_hash_function; }

        void setToStringCapability(FunctionModelItem fun) { m_qDebug_stream_function = fun; }
        const FunctionModelItem& toStringCapability() const { return m_qDebug_stream_function; }

        virtual bool hasDefaultToStringFunction() const;

        void setHasEqualsOperator(bool on) { m_has_equals_operator = on; }
        bool hasEqualsOperator() const { return m_has_equals_operator; }

        void setHasCloneOperator(bool on) { m_has_clone_operator = on; }
        bool hasCloneOperator() const { return m_has_clone_operator; }

        void addPropertySpec(QPropertySpec *spec) { m_property_specs << spec; }
        const QList<QPropertySpec *>& propertySpecs() const { return m_property_specs; }

        QPropertySpec *propertySpecForRead(const QString &name) const;
        QPropertySpec *propertySpecForWrite(const QString &name) const;
        QPropertySpec *propertySpecForReset(const QString &name) const;
        QPropertySpec *propertySpecForNotify(const QString &name) const;

        QList<ReferenceCount> referenceCounts() const;
        QList<TemplateInstantiation> templateInstantiations() const;

        void setEqualsFunctions(const AbstractMetaFunctionList &lst) { m_equals_functions = lst; }
        const AbstractMetaFunctionList& equalsFunctions() const { return m_equals_functions; }

        void setNotEqualsFunctions(const AbstractMetaFunctionList &lst) { m_nequals_functions = lst; }
        const AbstractMetaFunctionList& notEqualsFunctions() const { return m_nequals_functions; }

        void setLessThanFunctions(const AbstractMetaFunctionList &lst) { m_less_than_functions = lst; }
        const AbstractMetaFunctionList& lessThanFunctions() const { return m_less_than_functions; }

        void setGreaterThanFunctions(const AbstractMetaFunctionList &lst) { m_greater_than_functions = lst; }
        const AbstractMetaFunctionList& greaterThanFunctions() const { return m_greater_than_functions; }

        void setLessThanEqFunctions(const AbstractMetaFunctionList &lst) { m_less_than_eq_functions = lst; }
        const AbstractMetaFunctionList& lessThanEqFunctions() const { return m_less_than_eq_functions; }

        void setGreaterThanEqFunctions(const AbstractMetaFunctionList &lst) { m_greater_than_eq_functions = lst; }
        const AbstractMetaFunctionList& greaterThanEqFunctions() const { return m_greater_than_eq_functions; }

        void setBeginFunctions(const AbstractMetaFunctionList &lst) { m_beginFunctions = lst; }
        const AbstractMetaFunctionList& beginFunctions() const { return m_beginFunctions; }

        void setEndFunctions(const AbstractMetaFunctionList &lst) { m_endFunctions = lst; }
        const AbstractMetaFunctionList& endFunctions() const { return m_endFunctions; }

        void sortFunctions();

        const AbstractMetaClass *templateBaseClass() const { return m_template_base_class; }
        void setTemplateBaseClass(const AbstractMetaClass *cls) { m_template_base_class = cls; }

        const QList<const AbstractMetaType *>& templateBaseClassInstantiations() const { return m_template_base_class_instantiations; }
        void setTemplateBaseClassInstantiations(const QList<const AbstractMetaType *>& instantiations) { m_template_base_class_instantiations = instantiations; }

        void setTypeAlias(bool typeAlias) { m_is_type_alias = typeAlias; }
        bool isTypeAlias() const { return m_is_type_alias; }

        void setTypeAliasType(AbstractMetaType* typeAlias) { m_typeAliasType = typeAlias; }
        const AbstractMetaType* typeAliasType() const { return m_typeAliasType; }

        void setHas_Q_GADGET(bool isGadget){m_has_Q_GADGET = isGadget;}
        bool has_Q_GADGET() const {return m_has_Q_GADGET;}

        void setHas_Q_OBJECT(bool isOBJECT){m_has_Q_OBJECT = isOBJECT;}
        bool has_Q_OBJECT() const {return m_has_Q_OBJECT;}
        bool isDeprecated() const {return isDeclDeprecated() || (typeEntry()->typeFlags() & ComplexTypeEntry::Deprecated);}

        void setHasSubClasses(bool subClasses){m_has_subClasses = subClasses;}
        bool hasSubClasses() const {return m_has_subClasses;}
        void addFunctionalByUsing(QString _using, AbstractMetaFunctional* f){
            m_functionalByUsing[_using] = f;
        }
        const QMap<QString,AbstractMetaFunctional*>& functionalByUsing(){ return m_functionalByUsing; }

        void setDeprecatedComment(const QString &deprecatedComment) { m_deprecatedComment = deprecatedComment; }
        const QString& deprecatedComment() const { return m_deprecatedComment; }
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
    uint m_has_unimplmentablePureVirtualFunctions : 1;
    QSet<QString> m_unimplmentablePureVirtualFunctions;
    uint m_functions_fixed : 1;
    uint m_has_public_destructor : 1;
    uint m_has_private_destructor : 1;
    uint m_has_private_metaObject : 1;
    uint m_has_private_metacall : 1;
    uint m_has_virtual_destructor : 1;
    uint m_has_hash_function : 1;
    uint m_has_equals_operator : 1;
    uint m_has_clone_operator : 1;
    uint m_is_type_alias : 1;
    uint m_has_Q_GADGET : 1;
    uint m_has_Q_OBJECT : 1;
    uint m_has_subClasses : 1;
    uint m_reserved : 17;

        const AbstractMetaClass *m_enclosing_class;
        AbstractMetaClass *m_base_class;
        AbstractMetaType* m_typeAliasType;
        const AbstractMetaClass *m_template_base_class;
        QList<const AbstractMetaType *> m_template_base_class_instantiations;
        AbstractMetaFunctionList m_functions;
        AbstractMetaFunctionList m_invalidfunctions;
        AbstractMetaFieldList m_fields;
        AbstractMetaEnumList m_enums;
        AbstractMetaFunctionalList m_functionals;
        QMap<QString,AbstractMetaFunctional*> m_functionalByUsing;
        AbstractMetaClassList m_interfaces;
        AbstractMetaClassList m_enclosed_classes;
        AbstractMetaClass *m_extracted_interface;
        AbstractMetaClass *m_extracted_interface_impl;
        AbstractMetaClass *m_primary_interface_implementor;
        QList<QPropertySpec *> m_property_specs;
        AbstractMetaFunctionList m_equals_functions;
        AbstractMetaFunctionList m_nequals_functions;

        AbstractMetaFunctionList m_less_than_functions;
        AbstractMetaFunctionList m_greater_than_functions;
        AbstractMetaFunctionList m_less_than_eq_functions;
        AbstractMetaFunctionList m_greater_than_eq_functions;
        AbstractMetaFunctionList m_beginFunctions;
        AbstractMetaFunctionList m_endFunctions;

        QStringList m_base_class_names;
        QList<TypeEntry *> m_template_args;
        ComplexTypeEntry *m_type_entry;
        FunctionModelItem m_qDebug_stream_function;
        QString m_deprecatedComment;
};

class QPropertySpec {
    public:
        QPropertySpec()
            : m_name(),
                m_read(),
                m_write(),
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

        int index() const { return m_index; }
        void setIndex(int index) { m_index = index; }

    private:
        QString m_name;
        QString m_read;
        QString m_write;
        QString m_designable;
        QString m_scriptable;
        QString m_reset;
        QString m_notify;
        QString m_stored;
        QString m_revision;
        QString m_user;
        bool m_required;
        bool m_constant;
        bool m_final;
        int m_index;
};

inline AbstractMetaFunctionList AbstractMetaClass::allVirtualFunctions() const {
    return queryFunctions(VirtualFunctions
                          | NotRemovedFromTargetLang);
}

inline AbstractMetaFunctionList AbstractMetaClass::allFinalFunctions() const {
    return queryFunctions(FinalInTargetLangFunctions
                          | FinalInCppFunctions
                          | NotRemovedFromTargetLang);
}

inline AbstractMetaFunctionList AbstractMetaClass::cppInconsistentFunctions() const {
    return queryFunctions(Inconsistent
                          | NormalFunctions
                          | Visible
                          | NotRemovedFromTargetLang);
}

inline AbstractMetaFunctionList AbstractMetaClass::cppSignalFunctions() const {
    return queryFunctions(Signals
                          | NotRemovedFromTargetLang);
}

#endif // ABSTRACTMETALANG_H
