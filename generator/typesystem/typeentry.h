/****************************************************************************
**
** Copyright (C) 2013-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** (in parts)
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

#ifndef TYPEENTRY_H_
#define TYPEENTRY_H_

#include <QHash>
#include <QStringList>
#include "codesnip.h"
#include "modification.h"

extern QString strings_Object;
extern QString strings_String;
extern QString strings_Thread;
extern QString strings_char;
extern QString strings_java_lang;
extern QString strings_jchar;
extern QString strings_jobject;

class InterfaceTypeEntry;
class ObjectTypeEntry;
class AbstractMetaType;

class EnumTypeEntry;
class FlagsTypeEntry;

struct Include;

typedef QList<Include> IncludeList;

struct ExpensePolicy {
    ExpensePolicy() : limit(-1) { }
    int limit;
    QString cost;
    bool isValid() const {
        return limit >= 0;
    }
};

struct EnumValueRedirection {
    EnumValueRedirection(const QString &rej, const QString &us)
            : rejected(rej),
            used(us) {
    }
    QString rejected;
    QString used;
};


class TypeEntry {
    public:
        enum Type {
            PrimitiveType,
            VoidType,
            FlagsType,
            EnumType,
            TemplateArgumentType,
            ThreadType,
            BasicValueType,
            StringType,
            StringRefType,
            ContainerType,
            InterfaceType,
            ObjectType,
            NamespaceType,
            VariantType,
            PointerContainerType,
            JObjectWrapperType,
            CharType,
            ArrayType,
            TypeSystemType,
            CustomType
        };

        enum CodeGeneration {
            GenerateTargetLang      = 0x0001,
            GenerateCpp             = 0x0002,
            GenerateForSubclass     = 0x0004,

            GenerateNothing         = 0,
            GenerateAll             = 0xffff,
            GenerateCode            = GenerateTargetLang | GenerateCpp
        };

        TypeEntry(const QString &name, Type t)
                : m_name(name),
                m_type(t),
                m_code_generation(GenerateAll),
                m_type_flags(),
                m_preferred_conversion(true) {
        };

        virtual ~TypeEntry() { }

        Type type() const {
            return m_type;
        }
        bool isPrimitive() const {
            return m_type == PrimitiveType;
        }
        bool isEnum() const {
            return m_type == EnumType;
        }
        bool isFlags() const {
            return m_type == FlagsType;
        }
        bool isInterface() const {
            return m_type == InterfaceType;
        }
        bool isObject() const {
            return m_type == ObjectType;
        }
        bool isString() const {
            return m_type == StringType;
        }
        bool isStringRef() const {
            return m_type == StringRefType;
        }
        bool isChar() const {
            return m_type == CharType;
        }
        bool isNamespace() const {
            return m_type == NamespaceType;
        }
        bool isContainer() const {
            return m_type == ContainerType;
        }
        bool isVariant() const {
            return m_type == VariantType;
        }
        bool isJObjectWrapper() const {
            return m_type == JObjectWrapperType;
        }
        bool isArray() const {
            return m_type == ArrayType;
        }
        bool isTemplateArgument() const {
            return m_type == TemplateArgumentType;
        }
        bool isVoid() const {
            return m_type == VoidType;
        }
        bool isThread() const {
            return m_type == ThreadType;
        }
        bool isCustom() const {
            return m_type == CustomType;
        }
        bool isPointerContainer() const {
            return m_type == PointerContainerType;
        }
        bool isBasicValue() const {
            return m_type == BasicValueType;
        }
        bool isTypeSystem() const {
            return m_type == TypeSystemType;
        }

        virtual bool preferredConversion() const {
            return m_preferred_conversion;
        }
        virtual void setPreferredConversion(bool b) {
            m_preferred_conversion = b;
        }

        // The type's name in C++, fully qualified
        QString name() const {
            return m_name;
        }

        uint codeGeneration() const {
            return m_code_generation;
        }
        void setCodeGeneration(uint cg) {
            m_code_generation = cg;
        }

        virtual QString qualifiedCppName() const {
            return m_name;
        }

        // Its type's name in JNI
        virtual QString jniName() const {
            return m_name;
        }

        // The type's name in TargetLang
        virtual QString targetLangName() const {
            return m_name;
        }

        // The type to lookup when converting to TargetLang
        virtual QString lookupName() const {
            return targetLangName();
        }

        // The package
        virtual QString javaPackage() const {
            return QString();
        }

        virtual QString qualifiedTargetLangName() const {
            QString pkg = javaPackage();
            if (pkg.isEmpty()) return targetLangName();
            return pkg + '.' + targetLangName();
        }

        virtual InterfaceTypeEntry *designatedInterface() const {
            return 0;
        }

        void setCustomCreator(const CustomFunction &func) {
            m_customCreator = func;
        }
        CustomFunction customCreator() const {
            return m_customCreator;
        }

        void setCustomDeleter(const CustomFunction &func) {
            m_customDeleter = func;
        }
        CustomFunction customDeleter() const {
            return m_customDeleter;
        }

        void setCustomConstructor(const CustomFunction &func) {
            m_customConstructor = func;
        }
        CustomFunction customConstructor() const {
            return m_customConstructor;
        }

        void setCustomDestructor(const CustomFunction &func) {
            m_customDestructor = func;
        }
        CustomFunction customDestructor() const {
            return m_customDestructor;
        }

        // currently not in use
        void setTypeFlags(const QString &flags) {
            m_type_flags = flags;
        }
        QString typeFlags() const {
            return m_type_flags;
        }

        virtual bool isValue() const {
            return false;
        }
        virtual bool isComplex() const {
            return false;
        }

        virtual bool isNativeIdBased() const {
            return false;
        }

    private:
        QString m_name;
        Type m_type;
        uint m_code_generation;
        CustomFunction m_customCreator;
        CustomFunction m_customDeleter;
        CustomFunction m_customConstructor;
        CustomFunction m_customDestructor;
        // currently not yet in use:
        QString m_type_flags;
        bool m_preferred_conversion;
};
typedef QHash<QString, QList<TypeEntry *> > TypeEntryHash;
typedef QHash<QString, TypeEntry *> SingleTypeEntryHash;


class TypeSystemTypeEntry : public TypeEntry {
    public:
        TypeSystemTypeEntry(const QString &name)
                : TypeEntry(name, TypeSystemType) {
        };

        QList<CodeSnip> snips;
};


class ThreadTypeEntry : public TypeEntry {
    public:
        ThreadTypeEntry() : TypeEntry("QThread", ThreadType) {
            setCodeGeneration(GenerateNothing);
        }

        QString jniName() const {
            return strings_jobject;
        }
        QString targetLangName() const {
            return strings_Thread;
        }
        QString javaPackage() const {
            return strings_java_lang;
        }
};

class VoidTypeEntry : public TypeEntry {
    public:
        VoidTypeEntry() : TypeEntry("void", VoidType) { }
};

class TemplateArgumentEntry : public TypeEntry {
    public:
        TemplateArgumentEntry(const QString &name)
                : TypeEntry(name, TemplateArgumentType), m_ordinal(0) {
        }

        int ordinal() const {
            return m_ordinal;
        }
        void setOrdinal(int o) {
            m_ordinal = o;
        }

    private:
        int m_ordinal;
};

class ArrayTypeEntry : public TypeEntry {
    public:
        ArrayTypeEntry(const TypeEntry *nested_type) : TypeEntry("Array", ArrayType), m_nested_type(nested_type) {
            Q_ASSERT(m_nested_type);
        }

        void setNestedTypeEntry(TypeEntry *nested) {
            m_nested_type = nested;
        }
        const TypeEntry *nestedTypeEntry() const {
            return m_nested_type;
        }

        QString targetLangName() const {
            return m_nested_type->targetLangName() + "[]";
        }
        QString jniName() const {
            if (m_nested_type->isPrimitive())
                return m_nested_type->jniName() + "Array";
            else
                return "jobjectArray";
        }

    private:
        const TypeEntry *m_nested_type;
};


class PrimitiveTypeEntry : public TypeEntry {
    public:
        PrimitiveTypeEntry(const QString &name)
                : TypeEntry(name, PrimitiveType),
                m_preferred_java_type(true) {
            setPreferredConversion(true);
        }

        QString targetLangName() const {
            return m_java_name;
        }
        void setTargetLangName(const QString &targetLangName) {
            m_java_name  = targetLangName;
        }

        QString jniName() const {
            return m_jni_name;
        }
        void setJniName(const QString &jniName) {
            m_jni_name = jniName;
        }

        QString javaObjectFullName() const {
            return javaObjectPackage() + "." + javaObjectName();
        }
        QString javaObjectName() const;
        QString javaObjectPackage() const {
            return strings_java_lang;
        }

        virtual bool preferredTargetLangType() const {
            return m_preferred_java_type;
        }
        virtual void setPreferredTargetLangType(bool b) {
            m_preferred_java_type = b;
        }

    private:
        QString m_java_name;
        QString m_jni_name;
        bool m_preferred_java_type;
};

class EnumTypeEntry : public TypeEntry {
    public:
        EnumTypeEntry(const QString &nspace, const QString &enumName)
                : TypeEntry(nspace.isEmpty() ? enumName : nspace + QLatin1String("::") + enumName,
                            EnumType),
                m_flags(0),
                m_extensible(false) {
            m_qualifier = nspace;
            m_java_name = enumName;
        }

        QString javaPackage() const {
            return m_package_name;
        }
        void setTargetLangPackage(const QString &package) {
            m_package_name = package;
        }

        QString targetLangName() const {
            return m_java_name;
        }
        QString javaQualifier() const;
        QString qualifiedTargetLangName() const {
            QString pkg = javaPackage();
            if (pkg.isEmpty()) return javaQualifier() + '.' + targetLangName();
            return pkg + '.' + javaQualifier() + '.' + targetLangName();
        }

        QString jniName() const;

        QString qualifier() const {
            return m_qualifier;
        }
        void setQualifier(const QString &q) {
            m_qualifier = q;
        }

        virtual bool preferredConversion() const {
            return false;
        }

        bool isBoundsChecked() const {
            return m_lower_bound.isEmpty() && m_upper_bound.isEmpty();
        }

        QString upperBound() const {
            return m_upper_bound;
        }
        void setUpperBound(const QString &bound) {
            m_upper_bound = bound;
        }

        QString lowerBound() const {
            return m_lower_bound;
        }
        void setLowerBound(const QString &bound) {
            m_lower_bound = bound;
        }

        void setFlags(FlagsTypeEntry *flags) {
            m_flags = flags;
        }
        FlagsTypeEntry *flags() const {
            return m_flags;
        }

        bool isExtensible() const {
            return m_extensible;
        }
        void setExtensible(bool is) {
            m_extensible = is;
        }

        bool isEnumValueRejected(const QString &name) {
            return m_rejected_enums.contains(name);
        }
        void addEnumValueRejection(const QString &name) {
            m_rejected_enums << name;
        }
        QStringList enumValueRejections() const {
            return m_rejected_enums;
        }

        void addEnumValueRedirection(const QString &rejected, const QString &usedValue);
        QString enumValueRedirection(const QString &value) const;

        bool forceInteger() const {
            return m_force_integer;
        }
        void setForceInteger(bool force) {
            m_force_integer = force;
        }

        virtual QString implements() const {
            return m_implements;
        }

        virtual void setImplements(QString & implements){
            m_implements = implements;
        }

    private:
        QString m_implements;
        QString m_package_name;
        QString m_qualifier;
        QString m_java_name;

        QString m_lower_bound;
        QString m_upper_bound;

        QStringList m_rejected_enums;
        QList<EnumValueRedirection> m_enum_redirections;

        FlagsTypeEntry *m_flags;

        bool m_extensible;
        bool m_force_integer;
};

class FlagsTypeEntry : public TypeEntry {
    public:
        FlagsTypeEntry(const QString &name) : TypeEntry(name, FlagsType), m_enum(0) {
        }

        QString qualifiedTargetLangName() const;
        QString targetLangName() const {
            return m_java_name;
        }
        QString jniName() const;
        virtual bool preferredConversion() const {
            return false;
        }

        QString originalName() const {
            return m_original_name;
        }
        void setOriginalName(const QString &s) {
            m_original_name = s;
        }

        QString flagsName() const {
            return m_java_name;
        }
        void setFlagsName(const QString &name) {
            m_java_name = name;
        }

        bool forceInteger() const {
            return m_enum->forceInteger();
        }

        EnumTypeEntry *originator() const {
            return m_enum;
        }
        void setOriginator(EnumTypeEntry *e) {
            m_enum = e;
        }

        QString javaPackage() const {
            return m_enum->javaPackage();
        }

    private:
        QString m_original_name;
        QString m_java_name;
        EnumTypeEntry *m_enum;
};

class ComplexTypeEntry : public TypeEntry {
    public:
        enum TypeFlag {
            ForceAbstract      = 0x1,
            DeleteInMainThread = 0x2,
            Deprecated         = 0x4
        };
        typedef QFlags<TypeFlag> TypeFlags;

        ComplexTypeEntry(const QString &name, Type t)
            : TypeEntry(QString(name).replace("::", "$"), t), // the A$B notation is the java binary name of an embedded class
                m_qualified_cpp_name(name),
                m_qobject(false),
                m_polymorphic_base(false),
                m_generic_class(false),
                m_type_flags(0) {
            Include inc;
            inc.name = "QtCore/QVariant";
            inc.type = Include::IncludePath;

            addExtraInclude(inc);
        }

        bool isComplex() const {
            return true;
        }

        IncludeList extraIncludes() const {
            return m_extra_includes;
        }
        void setExtraIncludes(const IncludeList &includes) {
            m_extra_includes = includes;
        }
        void addExtraInclude(const Include &include) {
            if (!m_includes_used.value(include.name, false)) {
                m_extra_includes << include;
                m_includes_used[include.name] = true;
            }
        }

        ComplexTypeEntry *copy() const {
            ComplexTypeEntry *centry = new ComplexTypeEntry(name(), type());
            centry->setInclude(include());
            centry->setExtraIncludes(extraIncludes());
            centry->setFunctionModifications(functionModifications());
            centry->setFieldModifications(fieldModifications());
            centry->setQObject(isQObject());
            centry->setDefaultSuperclass(defaultSuperclass());
            centry->setCodeSnips(codeSnips());
            centry->setTargetLangPackage(javaPackage());

            return centry;
        }

        void setLookupName(const QString &name) {
            m_lookup_name = name;
        }

        virtual QString lookupName() const {
            return m_lookup_name.isEmpty() ? targetLangName() : m_lookup_name;
        }

        QString jniName() const {
            return strings_jobject;
        }


        Include include() const {
            return m_include;
        }
        void setInclude(const Include &inc) {
            m_include = inc;
        }

        void setTypeFlags(TypeFlags flags) {
            m_type_flags = flags;
        }

        TypeFlags typeFlags() const {
            return m_type_flags;
        }

        CodeSnipList codeSnips() const {
            return m_code_snips;
        }
        void setCodeSnips(const CodeSnipList &codeSnips) {
            m_code_snips = codeSnips;
        }
        void addCodeSnip(const CodeSnip &codeSnip) {
            m_code_snips << codeSnip;
        }

        FunctionModificationList functionModifications() const {
            return m_function_mods;
        }
        void setFunctionModifications(const FunctionModificationList &functionModifications) {
            m_function_mods = functionModifications;
        }
        void addFunctionModification(const FunctionModification &functionModification) {
            m_function_mods << functionModification;
        }
        FunctionModificationList functionModifications(const QString &signature) const;

        FieldModification fieldModification(const QString &name) const;
        void setFieldModifications(const FieldModificationList &mods) {
            m_field_mods = mods;
        }
        FieldModificationList fieldModifications() const {
            return m_field_mods;
        }

        QString javaPackage() const {
            return m_package;
        }
        void setTargetLangPackage(const QString &package) {
            m_package = package;
        }

        bool isQObject() const {
            return m_qobject;
        }
        void setQObject(bool qobject) {
            m_qobject = qobject;
        }

        QString defaultSuperclass() const {
            return m_default_superclass;
        }
        void setDefaultSuperclass(const QString &sc) {
            m_default_superclass = sc;
        }

        virtual QString qualifiedCppName() const {
            return m_qualified_cpp_name;
        }

        /**
         * the class-type attribute 'implements' can be used to extend the list of interfaces used by a class
         * @brief setImplements
         * @param implements
         */
        void setImplements(const QString &implements) {
            m_implements = implements;
        }
        QString implements() const {
            return m_implements;
        }

        void setIsPolymorphicBase(bool on) {
            m_polymorphic_base = on;
        }
        bool isPolymorphicBase() const {
            return m_polymorphic_base;
        }

        void setPolymorphicIdValue(const QString &value) {
            m_polymorphic_id_value = value;
        }
        QString polymorphicIdValue() const {
            return m_polymorphic_id_value;
        }

        void setExpensePolicy(const ExpensePolicy &policy) {
            m_expense_policy = policy;
        }
        const ExpensePolicy &expensePolicy() const {
            return m_expense_policy;
        }

        QString targetType() const {
            return m_target_type;
        }
        void setTargetType(const QString &code) {
            m_target_type = code;
        }

        QString targetLangName() const {
            if(m_java_name.isEmpty()){
                return TypeEntry::targetLangName();
            }else{
                return m_java_name;
            }
        }
        void setTargetLangName(const QString &name) {
            m_java_name = name;
        }

        bool isGenericClass() const {
            return m_generic_class;
        }
        void setGenericClass(bool isGeneric) {
            m_generic_class = isGeneric;
        }

    private:
        IncludeList m_extra_includes;
        Include m_include;
        QHash<QString, bool> m_includes_used;
        FunctionModificationList m_function_mods;
        FieldModificationList m_field_mods;
        CodeSnipList m_code_snips;
        QString m_package;
        QString m_default_superclass;
        QString m_qualified_cpp_name;
        QString m_java_name;
        QString m_implements;

    uint m_qobject :
        1;
    uint m_polymorphic_base :
        1;
    uint m_generic_class :
        1;

        QString m_polymorphic_id_value;
        QString m_lookup_name;
        QString m_target_type;
        ExpensePolicy m_expense_policy;
        TypeFlags m_type_flags;
};

class NamespaceTypeEntry : public ComplexTypeEntry {
    public:
        NamespaceTypeEntry(const QString &name) : ComplexTypeEntry(name, NamespaceType) { }
};


class InterfaceTypeEntry : public ComplexTypeEntry {
    public:
        InterfaceTypeEntry(const QString &name)
                : ComplexTypeEntry(name, InterfaceType) {
        }

        static QString interfaceName(const QString &name) {
            return name + "Interface";
        }

        ObjectTypeEntry *origin() const {
            return m_origin;
        }
        void setOrigin(ObjectTypeEntry *origin) {
            m_origin = origin;
        }

        virtual bool isNativeIdBased() const {
            return true;
        }
        virtual QString qualifiedCppName() const {
            return ComplexTypeEntry::qualifiedCppName().left(ComplexTypeEntry::qualifiedCppName().length() - interfaceName("").length());
        }

    private:
        ObjectTypeEntry *m_origin;
};


class ObjectTypeEntry : public ComplexTypeEntry {
    public:
        ObjectTypeEntry(const QString &name)
                : ComplexTypeEntry(name, ObjectType), m_interface(0) {
        }

        InterfaceTypeEntry *designatedInterface() const {
            return m_interface;
        }
        void setDesignatedInterface(InterfaceTypeEntry *entry) {
            m_interface = entry;
        }

        virtual bool isNativeIdBased() const {
            return true;
        }

    private:
        InterfaceTypeEntry *m_interface;
};

class CustomTypeEntry : public ComplexTypeEntry {
    public:
        CustomTypeEntry(const QString &name) : ComplexTypeEntry(name, CustomType) { }

        virtual void generateCppJavaToQt(QTextStream &s,
                                         const AbstractMetaType *java_type,
                                         const QString &env_name,
                                         const QString &qt_name,
                                         const QString &java_name) const = 0;

        virtual void generateCppQtToJava(QTextStream &s,
                                         const AbstractMetaType *java_type,
                                         const QString &env_name,
                                         const QString &qt_name,
                                         const QString &java_name) const = 0;
};

class ValueTypeEntry : public ComplexTypeEntry {
    public:
        ValueTypeEntry(const QString &name) : ComplexTypeEntry(name, BasicValueType) { }

        bool isValue() const {
            return true;
        }

        virtual bool isNativeIdBased() const {
            return true;
        }

    protected:
        ValueTypeEntry(const QString &name, Type t) : ComplexTypeEntry(name, t) { }
};

class StringTypeEntry : public ValueTypeEntry {
    public:
        StringTypeEntry(const QString &name)
                : ValueTypeEntry(name, StringType) {
            setCodeGeneration(GenerateNothing);
        }

        QString jniName() const {
            return strings_jobject;
        }
        QString targetLangName() const {
            return strings_String;
        }
        QString javaPackage() const {
            return strings_java_lang;
        }

        virtual bool isNativeIdBased() const {
            return false;
        }
};

class StringRefTypeEntry : public ValueTypeEntry {
    public:
        StringRefTypeEntry(const QString &name)
                : ValueTypeEntry(name, StringRefType) {
            setCodeGeneration(GenerateNothing);
        }

        QString jniName() const {
            return strings_jobject;
        }
        QString targetLangName() const {
            return strings_String;
        }
        QString javaPackage() const {
            return strings_java_lang;
        }

        virtual bool isNativeIdBased() const {
            return false;
        }
};

class CharTypeEntry : public ValueTypeEntry {
    public:
        CharTypeEntry(const QString &name) : ValueTypeEntry(name, CharType) {
            setCodeGeneration(GenerateNothing);
        }

        QString jniName() const {
            return strings_jchar;
        }
        QString targetLangName() const {
            return strings_char;
        }
        QString javaPackage() const {
            return QString();
        }

        virtual bool isNativeIdBased() const {
            return false;
        }
};

class JObjectWrapperTypeEntry: public ValueTypeEntry {
    public:
        JObjectWrapperTypeEntry(const QString &name) : ValueTypeEntry(name, JObjectWrapperType) { }

        QString jniName() const {
            return strings_jobject;
        }
        QString targetLangName() const {
            return strings_Object;
        }
        QString javaPackage() const {
            return strings_java_lang;
        }

        bool isNativeIdBased() const {
            return false;
        }
};

class VariantTypeEntry: public ValueTypeEntry {
    public:
        VariantTypeEntry(const QString &name) : ValueTypeEntry(name, VariantType) { }

        QString jniName() const {
            return strings_jobject;
        }
        QString targetLangName() const {
            return strings_Object;
        }
        QString javaPackage() const {
            return strings_java_lang;
        }

        virtual bool isNativeIdBased() const {
            return false;
        }
};

class PointerContainerTypeEntry: public ValueTypeEntry {
    public:
        enum Type {
            QSharedPointer,
            QWeakPointer
        };

        PointerContainerTypeEntry(const QString &name, Type type) : ValueTypeEntry(name, PointerContainerType) {
            m_type = type;
        }

        QString jniName() const {
            return strings_jobject;
        }
        QString targetLangName() const {
            return "";
        }
        QString javaPackage() const {
            return "";
        }

        virtual bool isNativeIdBased() const {
            return false;
        }

        Type type() const { return m_type; }

    private:
        Type m_type;
};


class ContainerTypeEntry : public ComplexTypeEntry {
    public:
        enum Type {
            NoContainer,
            ListContainer,
            StringListContainer,
            ByteArrayListContainer,
            LinkedListContainer,
            VectorContainer,
            StackContainer,
            QueueContainer,
            SetContainer,
            MapContainer,
            MultiMapContainer,
            HashContainer,
            MultiHashContainer,
            PairContainer,
            /*the following entries are required to map corresponding
              collection types from the new Qt3D library to java containers
              */
            QArrayContainer,            // new since Qt3D. mapped to java.util.List<?>
            QVector2DArrayContainer,    // new since Qt3D. mapped to java.util.List<QVector2D>
            QVector3DArrayContainer,    // new since Qt3D. mapped to java.util.List<QVector3D>
            QVector4DArrayContainer,    // new since Qt3D. mapped to java.util.List<QVector4D>
            /*
              For QDeclarative and QQml module.
              This entry is to support the QDeclarativeListProperty / QQmlListProperty
             */
            QQmlListPropertyContainer,
            QDeclarativeListPropertyContainer
        };

        ContainerTypeEntry(const QString &name, Type type)
                : ComplexTypeEntry(name, ContainerType) {
            m_type = type;
            setCodeGeneration(GenerateForSubclass);
        }

        Type type() const { return m_type; }
        QString targetLangName() const;
        QString javaPackage() const;
        QString qualifiedCppName() const;

    private:
        Type m_type;
};

#endif
