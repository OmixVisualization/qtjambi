/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

typedef QList<AbstractMetaField *> AbstractMetaFieldList;
typedef QList<AbstractMetaArgument *> AbstractMetaArgumentList;
typedef QList<AbstractMetaFunction *> AbstractMetaFunctionList;
typedef QList<AbstractMetaTemplateParameter *> AbstractMetaTemplateParameterList;

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
        AbstractMetaAttributes() : m_attributes(0) { }

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

            Final                       = DeclaredFinalInCpp | FinalInTargetLang | FinalInCpp
        };

        uint attributes() const { return m_attributes; }
        void setAttributes(uint attributes) { m_attributes = attributes; }

        uint originalAttributes() const { return m_originalAttributes; }
        void setOriginalAttributes(uint attributes) { m_originalAttributes = attributes; }

        uint visibility() const { return m_attributes & Visibility; }
        void setVisibility(uint visi) { m_attributes = (m_attributes & ~Visibility) | visi; }

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

        bool wasPrivate() const { return m_originalAttributes & Private; }
        bool wasProtected() const { return m_originalAttributes & Protected; }
        bool wasPublic() const { return m_originalAttributes & Public; }
        bool wasFriendly() const { return m_originalAttributes & Friendly; }

    private:
        uint m_attributes;
        uint m_originalAttributes;
};


class AbstractMetaType {
    public:
        enum TypeUsagePattern {
            InvalidPattern,
            PrimitivePattern,
            FlagsPattern,
            EnumPattern,
            ValuePattern,
            StringPattern,
            StringRefPattern,
            CharPattern,
            ObjectPattern,
            QObjectPattern,
            NativePointerPattern,
            ContainerPattern,
            PointerContainerPattern,
            VariantPattern,
            JObjectWrapperPattern,
            ArrayPattern,
            ThreadPattern,
            TemplateArgumentPattern
        };

        AbstractMetaType() :
                m_type_entry(0),
                m_array_element_count(0),
                m_array_element_type(0),
                m_original_template_type(0),
                m_pattern(InvalidPattern),
                m_constant(false),
                m_reference(false),
                m_cpp_instantiation(true),
                m_reserved(0) {
        }

        QString package() const { return m_type_entry->javaPackage(); }
        QString name() const { return m_type_entry->targetLangName(); }
        QString fullName() const { return m_type_entry->qualifiedTargetLangName(); }

        void setTypeUsagePattern(TypeUsagePattern pattern) { m_pattern = pattern; }
        TypeUsagePattern typeUsagePattern() const { return m_pattern; }

        // true when use pattern is container
        bool hasInstantiations() const { return !m_instantiations.isEmpty(); }
        void addInstantiation(AbstractMetaType *inst) { m_instantiations << inst; }
        void setInstantiations(const QList<AbstractMetaType *> &insts) { m_instantiations = insts; }
        QList<AbstractMetaType *> instantiations() const { return m_instantiations; }
        void setInstantiationInCpp(bool incpp) { m_cpp_instantiation = incpp; }
        bool hasInstantiationInCpp() const { return hasInstantiations() && m_cpp_instantiation; }

        QString minimalSignature() const;

        // true when the type is a QtJambiObject subclass
        bool hasNativeId() const;

        // returns true if the type was a template argument type
        bool isTemplateArgument() const { return m_pattern == TemplateArgumentPattern; }

        // returns true if the typs is used as a non complex primitive, no & or *'s
        bool isPrimitive() const { return m_pattern == PrimitivePattern; }

        // returns true if the type is used as an enum
        bool isEnum() const { return m_pattern == EnumPattern; }

        // returns true if the type is used as a QObject *
        bool isQObject() const { return m_pattern == QObjectPattern; }

        // returns true if the type is used as an object, e.g. Xxx *
        bool isObject() const { return m_pattern == ObjectPattern; }

        // returns true if the type is used as an array, e.g. Xxx[42]
        bool isArray() const { return m_pattern == ArrayPattern; }

        // returns true if the type is used as a value type (X or const X &)
        bool isValue() const { return m_pattern == ValuePattern; }

        // returns true for more complex types...
        bool isNativePointer() const { return m_pattern == NativePointerPattern; }

        // returns true if the type was originally a QString or const QString & or equivalent for QLatin1String
        bool isTargetLangString() const { return m_pattern == StringPattern; }

        // returns true if the type was originally a QStringRef or const QStringRef &
        bool isTargetLangStringRef() const { return m_pattern == StringRefPattern; }

        // returns true if the type was originally a QChar or const QChar &
        bool isTargetLangChar() const { return m_pattern == CharPattern; }

        // return true if the type was originally a QVariant or const QVariant &
        bool isVariant() const { return m_pattern == VariantPattern; }

        // return true if the type was originally a JObjectWrapper or const JObjectWrapper &
        bool isJObjectWrapper() const { return m_pattern == JObjectWrapperPattern; }

        // returns true if the type was used as a container
        bool isContainer() const { return m_pattern == ContainerPattern; }

        // returns true if the type was used as a qpointer
        bool isPointerContainer() const { return m_pattern == PointerContainerPattern; }

        // returns true if the type was used as a flag
        bool isFlags() const { return m_pattern == FlagsPattern; }

        // returns true if the type was used as a thread
        bool isThread() const { return m_pattern == ThreadPattern; }

        bool isConstant() const { return m_constant; }
        void setConstant(bool constant) { m_constant = constant; }

        bool isReference() const { return m_reference; }
        void setReference(bool ref) { m_reference = ref; }

        // Returns true if the type is to be implemented using Java enums, e.g. not plain ints.
        bool isTargetLangEnum() const { return isEnum() && !((EnumTypeEntry *) typeEntry())->forceInteger(); }
        bool isIntegerEnum() const { return isEnum() && !isTargetLangEnum(); }

        // Returns true if the type is to be implemented using Java QFlags, e.g. not plain ints.
        bool isTargetLangFlags() const {
            return isFlags() && !((FlagsTypeEntry *) typeEntry())->forceInteger();
        }
        bool isIntegerFlags() const { return isFlags() && !isTargetLangFlags(); }

        int actualIndirections() const { return m_indirections.size() + (isReference() ? 1 : 0); }
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
        QList <AbstractMetaType *> m_instantiations;
        QString m_package;
        QString m_original_type_description;

        int m_array_element_count;
        AbstractMetaType *m_array_element_type;
        const AbstractMetaType *m_original_template_type;

        TypeUsagePattern m_pattern;
    uint m_constant : 1;
    uint m_reference : 1;
    uint m_cpp_instantiation : 1;
    uint m_reserved : 25; // unused
    QList<bool> m_indirections;
};

class AbstractMetaVariable {
    public:
        AbstractMetaVariable() : m_type(0) { }

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

        QString defaultValueExpression() const { return m_expression; }
        void setDefaultValueExpression(const QString &expr) { m_expression = expr; }

        QString originalDefaultValueExpression() const { return m_original_expression; }
        void setOriginalDefaultValueExpression(const QString &expr) { m_original_expression = expr; }

        QString toString() const {
            return type()->name() + " " + AbstractMetaVariable::name() +
                   (m_expression.isEmpty() ? "" :  " = " + m_expression);
        }

        int argumentIndex() const { return m_argument_index; }
        void setArgumentIndex(int argIndex) { m_argument_index = argIndex; }

        virtual QString argumentName() const; // is overridden by java meta type to exclude java key words
        QString indexedName() const;

        AbstractMetaArgument *copy() const;

    private:
        // Just to force people to call argumentName() And indexedName();
        QString name() const;

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
                : m_function_type(NormalFunction),
                m_type(0),
                m_class(0),
                m_implementing_class(0),
                m_declaring_class(0),
                m_interface_class(0),
                m_property_spec(0),
                m_constant(false),
                m_invalid(false),
                m_jumptable_id(-1) {
        }

        ~AbstractMetaFunction();

        QString name() const { return m_name; }
        void setName(const QString &name) { m_name = name; }

        QString originalName() const { return m_original_name.isEmpty() ? name() : m_original_name; }
        void setOriginalName(const QString &name) { m_original_name = name; }

        QString modifiedName() const;

        QString minimalSignature() const;
        QStringList possibleIntrospectionCompatibleSignatures() const;

        QString marshalledName() const;

        // true if one or more of the arguments are of QtJambiObject subclasses
        bool argumentsHaveNativeId() const {
            foreach(const AbstractMetaArgument *arg, m_arguments) {
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

        // The class that actually implements this function
        const AbstractMetaClass *implementingClass() const { return m_implementing_class; }
        void setImplementingClass(const AbstractMetaClass *cls) { m_implementing_class = cls; }

        bool needsCallThrough() const;

        AbstractMetaArgumentList arguments() const { return m_arguments; }
        void setArguments(const AbstractMetaArgumentList &arguments) { m_arguments = arguments; }
        void addArgument(AbstractMetaArgument *argument) { m_arguments << argument; }
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
        bool isNormal() const { return functionType() == NormalFunction || isSlot() || isInGlobalScope(); }
        bool isInGlobalScope() const { return functionType() == GlobalScopeFunction; }
        bool isSignal() const { return functionType() == SignalFunction; }
        bool isSlot() const { return functionType() == SlotFunction; }
        bool isEmptyFunction() const { return functionType() == EmptyFunction; }
        bool hasTemplateArgumentTypes() const{
            foreach(const AbstractMetaArgument *arg, m_arguments) {
                if (arg->type()->isTemplateArgument())
                    return true;
            }
            return m_type && m_type->isTemplateArgument();
        }
        FunctionType functionType() const { return m_function_type; }
        void setFunctionType(FunctionType type) { m_function_type = type; }

        QStringList introspectionCompatibleSignatures(const QStringList &resolvedArguments = QStringList()) const;
        QString signature() const;
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
        QList<ReferenceCount> referenceCounts(const AbstractMetaClass *cls, int idx = -2) const;
        QList<TemplateInstantiation> templateInstantiations(const AbstractMetaClass *cls) const;

        bool nullPointersDisabled(const AbstractMetaClass *cls = 0, int argument_idx = 0) const;
        QString nullPointerDefaultValue(const AbstractMetaClass *cls = 0, int argument_idx = 0) const;

        bool resetObjectAfterUse(int argument_idx) const;

        // Returns whether garbage collection is disabled for the argument in any context
        bool disabledGarbageCollection(const AbstractMetaClass *cls, int key) const;

        // Returns the ownership rules for the given argument in the given context
        TypeSystem::Ownership ownership(const AbstractMetaClass *cls, TypeSystem::Language language, int idx) const;

        bool isVirtualSlot() const;
        bool isAllowedAsSlot() const;

        QString typeReplaced(int argument_index) const;
        bool isRemovedFromAllLanguages(const AbstractMetaClass *) const;
        bool isRemovedFrom(const AbstractMetaClass *, TypeSystem::Language language) const;
        bool argumentRemoved(int) const;

        QString argumentReplaced(int key) const;
        bool needsSuppressUncheckedWarning() const;

        bool hasModifications(const AbstractMetaClass *implementor) const;
        FunctionModificationList modifications(const AbstractMetaClass *implementor) const;

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

    private:
        QString m_name;
        QString m_original_name;
        mutable QString m_cached_minimal_signature;
        mutable QString m_cached_modified_name;

        FunctionType m_function_type;
        AbstractMetaType *m_type;
        const AbstractMetaClass *m_class;
        const AbstractMetaClass *m_implementing_class;
        const AbstractMetaClass *m_declaring_class;
        const AbstractMetaClass *m_interface_class;
        QPropertySpec *m_property_spec;
        AbstractMetaArgumentList m_arguments;
        AbstractMetaTemplateParameterList m_templateParameters;
    uint m_constant                 : 1;
    uint m_invalid                  : 1;
        int m_jumptable_id;
};


class AbstractMetaEnumValue {
    public:
        AbstractMetaEnumValue()
                : m_value_set(false), m_value(0) {
        }

        int value() const { return m_value; }
        void setValue(int value) { m_value_set = true; m_value = value; }

        QString stringValue() const { return m_string_value; }
        void setStringValue(const QString &v) { m_string_value = v; }

        QString name() const { return m_name; }
        void setName(const QString &name) { m_name = name; }

        bool isValueSet() const { return m_value_set; }

    private:
        QString m_name;
        QString m_string_value;

        bool m_value_set;
        int m_value;
};


class AbstractMetaEnumValueList : public QList<AbstractMetaEnumValue *> {
    public:
        AbstractMetaEnumValue *find(const QString &name) const;
};

class AbstractMetaEnum : public AbstractMetaAttributes {
    public:
        AbstractMetaEnum() : m_type_entry(0), m_class(0), m_has_qenums_declaration(false) {}

        AbstractMetaEnumValueList values() const { return m_enum_values; }
        void addEnumValue(AbstractMetaEnumValue *enumValue) { m_enum_values << enumValue; }

        QString name() const { return m_type_entry->targetLangName(); }
        QString qualifier() const { return m_type_entry->javaQualifier(); }
        QString package() const { return m_type_entry->javaPackage(); }
        QString fullName() const { return package() + "." + qualifier()  + "." + name(); }

        // Has the enum been declared inside a Q_ENUMS() macro in its enclosing class?
        void setHasQEnumsDeclaration(bool on) { m_has_qenums_declaration = on; }
        bool hasQEnumsDeclaration() const { return m_has_qenums_declaration; }

        EnumTypeEntry *typeEntry() const { return m_type_entry; }
        void setTypeEntry(EnumTypeEntry *entry) { m_type_entry = entry; }

        AbstractMetaClass *enclosingClass() const { return m_class; }
        void setEnclosingClass(AbstractMetaClass *c) { m_class = c; }

    private:
        AbstractMetaEnumValueList m_enum_values;
        EnumTypeEntry *m_type_entry;
        AbstractMetaClass *m_class;

    uint m_has_qenums_declaration : 1;
    uint m_reserved : 31;
};

typedef QList<AbstractMetaEnum *> AbstractMetaEnumList;

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
                : m_namespace(false),
                m_qobject(false),
                m_has_virtuals(false),
                m_has_nonpublic(false),
                m_has_virtual_slots(false),
                m_has_justprivateconstructors(false),
                m_has_purevirtualfunctions(false),
                m_privatePureVirtualFunctions(),
                m_functions_fixed(false),
                m_has_public_destructor(false),
                m_has_private_destructor(false),
                m_has_hash_function(false),
                m_has_equals_operator(false),
                m_has_clone_operator(false),
                m_is_type_alias(false),
                m_enclosing_class(0),
                m_base_class(0),
                m_template_base_class(0),
                m_extracted_interface(0),
                m_primary_interface_implementor(0),
                m_type_entry(0),
                m_qDebug_stream_function(0) {
        }

        virtual ~AbstractMetaClass();

        AbstractMetaClass *extractInterface();
        void fixFunctions();
        void fixPrivatePureVirtualFunctions();

        AbstractMetaFunctionList functions() const { return m_functions; }
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
        bool hasPrivatePureVirtualFunction() const {return m_has_purevirtualfunctions;}
        void setPrivatePureVirtualFunctions(const QSet<QString>& privatePureVirtualFunctions) { m_privatePureVirtualFunctions = privatePureVirtualFunctions; }
        bool hasPrivateDestructor() const { return m_has_private_destructor; }
        void setHasPublicDestructor(bool on) { m_has_public_destructor = on; }
        bool hasPublicDestructor() const { return m_has_public_destructor; }

        AbstractMetaFunctionList queryFunctionsByName(const QString &name) const;
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
        AbstractMetaFunctionList nonVirtualShellFunctions() const;

        AbstractMetaFieldList fields() const { return m_fields; }
        void setFields(const AbstractMetaFieldList &fields) { m_fields = fields; }
        void addField(AbstractMetaField *field) { m_fields << field; }

        AbstractMetaEnumList enums() const { return m_enums; }
        void setEnums(const AbstractMetaEnumList &enums) { m_enums = enums; }
        void addEnum(AbstractMetaEnum *e) { m_enums << e; }

        AbstractMetaEnum *findEnum(const QString &enumName);
        AbstractMetaEnum *findEnumForValue(const QString &enumName);
        AbstractMetaEnumValue *findEnumValue(const QString &enumName, AbstractMetaEnum *meta_enum);

        AbstractMetaClassList interfaces() const { return m_interfaces; }
        void addInterface(AbstractMetaClass *interface);
        void setInterfaces(const AbstractMetaClassList &interface);

        AbstractMetaClassList enclosedClasses() const { return m_enclosed_classes; }
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
        void setBaseClass(AbstractMetaClass *base_class) { m_base_class = base_class; }

        const AbstractMetaClass *enclosingClass() const { return m_enclosing_class; }
        void setEnclosingClass(AbstractMetaClass *cl) { m_enclosing_class = cl; }

        QString package() const { return m_type_entry->javaPackage(); }
        bool isInterface() const { return m_type_entry->isInterface(); }
        bool isNamespace() const { return m_type_entry->isNamespace(); }
        bool isQObject() const { return m_type_entry->isQObject(); }
        bool isQtNamespace() const { return isNamespace() && name() == "Qt"; }
        QString qualifiedCppName() const { return m_type_entry->qualifiedCppName(); }

        bool hasInconsistentFunctions() const;
        bool hasSignals() const;
        bool inheritsFrom(const AbstractMetaClass *other) const;

        void setHasPrivateDestructor(bool on) { m_has_private_destructor = on; }
        void setHasVirtualDestructor(bool on) { m_has_virtuals |= on; }
        bool generateShellClass() const;

        bool hasVirtualSlots() const { return m_has_virtual_slots; }
        bool hasVirtualFunctions() const { return !isFinal() && m_has_virtuals; }
        bool hasProtectedFunctions() const;

        QList<TypeEntry *> templateArguments() const { return m_template_args; }
        void setTemplateArguments(const QList<TypeEntry *> &args) { m_template_args = args; }

        bool hasFieldAccessors() const;

        // only valid during metajavabuilder's run
        QStringList baseClassNames() const { return m_base_class_names; }
        void setBaseClassNames(const QStringList &names) { m_base_class_names = names; }

        AbstractMetaClass *primaryInterfaceImplementor() const { return m_primary_interface_implementor; }
        void setPrimaryInterfaceImplementor(AbstractMetaClass *cl) { m_primary_interface_implementor = cl; }

        const ComplexTypeEntry *typeEntry() const { return m_type_entry; }
        ComplexTypeEntry *typeEntry() { return m_type_entry; }
        void setTypeEntry(ComplexTypeEntry *type) { m_type_entry = type; }

        void setHasHashFunction(bool on) { m_has_hash_function = on; }
        bool hasHashFunction() const { return m_has_hash_function; }

        void setToStringCapability(FunctionModelItem fun) { m_qDebug_stream_function = fun; }
        FunctionModelItem hasToStringCapability() const { return m_qDebug_stream_function; }

        virtual bool hasDefaultToStringFunction() const;

        void setHasEqualsOperator(bool on) { m_has_equals_operator = on; }
        bool hasEqualsOperator() const { return m_has_equals_operator; }

        void setHasCloneOperator(bool on) { m_has_clone_operator = on; }
        bool hasCloneOperator() const { return m_has_clone_operator; }

        void addPropertySpec(QPropertySpec *spec) { m_property_specs << spec; }
        QList<QPropertySpec *> propertySpecs() const { return m_property_specs; }

        QPropertySpec *propertySpecForRead(const QString &name) const;
        QPropertySpec *propertySpecForWrite(const QString &name) const;
        QPropertySpec *propertySpecForReset(const QString &name) const;

        QList<ReferenceCount> referenceCounts() const;
        QList<TemplateInstantiation> templateInstantiations() const;

        void setEqualsFunctions(const AbstractMetaFunctionList &lst) { m_equals_functions = lst; }
        AbstractMetaFunctionList equalsFunctions() const { return m_equals_functions; }

        void setNotEqualsFunctions(const AbstractMetaFunctionList &lst) { m_nequals_functions = lst; }
        AbstractMetaFunctionList notEqualsFunctions() const { return m_nequals_functions; }

        void setLessThanFunctions(const AbstractMetaFunctionList &lst) { m_less_than_functions = lst; }
        AbstractMetaFunctionList lessThanFunctions() const { return m_less_than_functions; }

        void setGreaterThanFunctions(const AbstractMetaFunctionList &lst) { m_greater_than_functions = lst; }
        AbstractMetaFunctionList greaterThanFunctions() const { return m_greater_than_functions; }

        void setLessThanEqFunctions(const AbstractMetaFunctionList &lst) { m_less_than_eq_functions = lst; }
        AbstractMetaFunctionList lessThanEqFunctions() const { return m_less_than_eq_functions; }

        void setGreaterThanEqFunctions(const AbstractMetaFunctionList &lst) { m_greater_than_eq_functions = lst; }
        AbstractMetaFunctionList greaterThanEqFunctions() const { return m_greater_than_eq_functions; }

        void sortFunctions();

        const AbstractMetaClass *templateBaseClass() const { return m_template_base_class; }
        void setTemplateBaseClass(const AbstractMetaClass *cls) { m_template_base_class = cls; }

        void setTypeAlias(bool typeAlias) { m_is_type_alias = typeAlias; }
        bool isTypeAlias() const { return m_is_type_alias; }

private:
        QSet<QString> getAllPrivatePureVirtualFunctions() const;

    private:
    uint m_namespace : 1;
    uint m_qobject : 1;
    uint m_has_virtuals : 1;
    uint m_has_nonpublic : 1;
    uint m_has_virtual_slots : 1;
    uint m_has_justprivateconstructors : 1;
    uint m_has_purevirtualfunctions : 1;
    QSet<QString> m_privatePureVirtualFunctions;
    uint m_functions_fixed : 1;
    uint m_has_public_destructor : 1;
    uint m_has_private_destructor : 1;
    uint m_has_hash_function : 1;
    uint m_has_equals_operator : 1;
    uint m_has_clone_operator : 1;
    uint m_is_type_alias : 1;
    uint m_reserved : 19;

        const AbstractMetaClass *m_enclosing_class;
        AbstractMetaClass *m_base_class;
        const AbstractMetaClass *m_template_base_class;
        AbstractMetaFunctionList m_functions;
        AbstractMetaFunctionList m_invalidfunctions;
        AbstractMetaFieldList m_fields;
        AbstractMetaEnumList m_enums;
        AbstractMetaClassList m_interfaces;
        AbstractMetaClassList m_enclosed_classes;
        AbstractMetaClass *m_extracted_interface;
        AbstractMetaClass *m_primary_interface_implementor;
        QList<QPropertySpec *> m_property_specs;
        AbstractMetaFunctionList m_equals_functions;
        AbstractMetaFunctionList m_nequals_functions;

        AbstractMetaFunctionList m_less_than_functions;
        AbstractMetaFunctionList m_greater_than_functions;
        AbstractMetaFunctionList m_less_than_eq_functions;
        AbstractMetaFunctionList m_greater_than_eq_functions;

        QStringList m_base_class_names;
        QList<TypeEntry *> m_template_args;
        ComplexTypeEntry *m_type_entry;
        FunctionModelItem m_qDebug_stream_function;
};

class QPropertySpec {
    public:
        QPropertySpec(const TypeEntry *type)
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
                m_type(type),
                m_index(-1) {
        }

        const TypeEntry *type() const { return m_type; }

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
        bool m_constant;
        bool m_final;
        const TypeEntry *m_type;
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
