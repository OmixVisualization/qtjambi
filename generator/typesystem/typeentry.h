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

#define USE_NATIVE_IDS true

extern QString strings_Object;
extern QString strings_String;
extern QString strings_Thread;
extern QString strings_char;
extern QString strings_java_lang;
extern QString strings_jchar;
extern QString strings_jobject;
extern QString strings_jobjectArray;

class InterfaceTypeEntry;
class ObjectTypeEntry;
class AbstractMetaType;

class EnumTypeEntry;
class FlagsTypeEntry;

struct Include;

typedef QList<Include> IncludeList;

struct ExpensePolicy {
    ExpensePolicy() : limit(-1) { }
    qint64 limit;
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
            AliasType,
            PrimitiveType,
            VoidType,
            FlagsType,
            EnumType,
            FunctionalType,
            TemplateArgumentType,
            ThreadType,
            BasicValueType,
            StringType,
            Latin1StringType,
            StringViewType,
            StringRefType,
            ContainerType,
            IteratorType,
            InterfaceType,
            ObjectType,
            TemplateType,
            NamespaceType,
            VariantType,
            PointerContainerType,
            InitializerListType,
            JObjectWrapperType,
            QAndroidJniObjectType,
            JMapWrapperType,
            JCollectionWrapperType,
            JIteratorWrapperType,
            JEnumWrapperType,
            JQFlagsWrapperType,
            CharType,
            ArrayType,
            TypeSystemType,
            QModelIndexType,
            QMetaObjectType,
            QMetaObjectConnectionType,
            QMetaMethodType,
            QMetaPropertyType,
            QMetaEnumType,
            GlobalType,
        };

        enum CodeGeneration {
            GenerateTargetLang      = 0x0001,
            GenerateCpp             = 0x0002,
            GenerateForSubclass     = 0x0004,
            GenerateNoShell         = 0x0010,

            GenerateNothing         = 0,
            GenerateAll             = 0xff0f,
            GenerateCode            = GenerateTargetLang | GenerateCpp
        };

        enum ConstructorType{
            LegacyCopyConstructor,
            DefaultConstructor,
            CopyConstructor,
            MoveConstructor
        };

        TypeEntry(const QString &name, Type t)
                : m_name(name),
                m_type(t),
                m_code_generation(GenerateAll),
                m_type_flags(),
                m_preferred_conversion(true) {
        }

        virtual ~TypeEntry() { }

        Type type() const {
            return m_type;
        }
        bool isAlias() const {
            return m_type == AliasType;
        }
        bool isPrimitive() const {
            return m_type == PrimitiveType;
        }
        bool isEnum() const {
            return m_type == EnumType;
        }
        bool isFunctional() const {
            return m_type == FunctionalType;
        }
        bool isGlobal() const {
            return m_type == GlobalType;
        }
        bool isQModelIndexType() const {
            return m_type == QModelIndexType;
        }
        bool isQMetaObjectType() const {
            return m_type == QMetaObjectType;
        }
        bool isQMetaObjectConnectionType() const {
            return m_type == QMetaObjectConnectionType;
        }
        bool isQMetaEnumType() const {
            return m_type == QMetaEnumType;
        }
        bool isQMetaMethodType() const {
            return m_type == QMetaMethodType;
        }
        bool isQMetaPropertyType() const {
            return m_type == QMetaPropertyType;
        }
        virtual bool isScopedEnum() const {
             return false;
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
        bool isLatin1String() const {
            return m_type == Latin1StringType;
        }
        bool isStringView() const {
            return m_type == StringViewType;
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
        bool isIterator() const {
            return m_type == IteratorType;
        }
        bool isVariant() const {
            return m_type == VariantType;
        }
        bool isJObjectWrapper() const {
            return m_type == JObjectWrapperType;
        }
        bool isQAndroidJniObject() const {
            return m_type == QAndroidJniObjectType;
        }
        bool isJMapWrapper() const {
            return m_type == JMapWrapperType;
        }
        bool isJCollectionWrapper() const {
            return m_type == JCollectionWrapperType;
        }
        bool isJIteratorWrapper() const {
            return m_type == JIteratorWrapperType;
        }
        bool isJEnumWrapper() const {
            return m_type == JEnumWrapperType;
        }
        bool isJQFlagsWrapper() const {
            return m_type == JQFlagsWrapperType;
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
        bool isPointerContainer() const {
            return m_type == PointerContainerType;
        }
        bool isInitializerList() const {
            return m_type == InitializerListType;
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
        const QString& name() const {
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

        virtual QString targetTypeSystem() const {
            return javaPackage();
        }

        virtual QString qualifiedTargetLangName() const {
            QString pkg = javaPackage();
            if (pkg.isEmpty()) return targetLangName();
            return pkg + '.' + targetLangName();
        }

        virtual QString qualifiedTargetLangJNIName() const {
            QString pkg = javaPackage().replace('.', '/');
            if (pkg.isEmpty()) return targetLangName();
            return pkg + '/' + targetLangName();
        }

        virtual InterfaceTypeEntry *designatedInterface() const {
            return nullptr;
        }

        void setCustomConstructor(const CustomFunction &func, ConstructorType type = CopyConstructor) {
            m_customConstructors[type] = func;
        }
        CustomFunction customConstructor(ConstructorType type = CopyConstructor) const {
            return m_customConstructors[type];
        }

        void setCustomDestructor(const CustomFunction &func) {
            m_customDestructor = func;
        }
        const CustomFunction& customDestructor() const {
            return m_customDestructor;
        }

        // currently not in use
        void setTypeFlags(const QString &flags) {
            m_type_flags = flags;
        }
        const QString typeFlags() const {
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

        virtual const QString& ppCondition() const {
            static QString empty;
            return empty;
        }

    private:
        QString m_name;
        Type m_type;
        uint m_code_generation;
        QHash<ConstructorType,CustomFunction> m_customConstructors;
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
                : TypeEntry(name, TypeSystemType),
                  snips(),
                  m_include(),
                  m_extra_includes(),
                  m_includes_used(),
                  m_qtLibrary(),
                  m_requiredTypeSystems() {
        }

        TypeSystemTypeEntry(const QString &name, const QString &lib)
                : TypeEntry(name, TypeSystemType),
                  snips(),
                  m_include(),
                  m_extra_includes(),
                  m_includes_used(),
                  m_qtLibrary(lib),
                  m_requiredTypeSystems() {
        }

        const Include& include() const {
            return m_include;
        }
        void setInclude(const Include &inc) {
            m_include = inc;
        }

        const IncludeList& extraIncludes() const {
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

        const QString& qtLibrary() const {
            return m_qtLibrary;
        }
        void setQtLibrary(const QString &lib) {
            m_qtLibrary = lib;
        }

        const QList<const TypeSystemTypeEntry*>& requiredTypeSystems() const{
            return m_requiredTypeSystems;
        }

        void addRequiredTypeSystem(const TypeSystemTypeEntry* entry){
            if(entry)
                m_requiredTypeSystems.append(entry);
        }

        const QList<QString>& requiredQtLibraries() const{
            return m_requiredQtLibraries;
        }

        void addRequiredQtLibrary(const QString& entry){
            m_requiredQtLibraries.append(entry);
        }

        void addCodeSnip(const CodeSnip &snip) {
            snips << snip;
        }

        QList<CodeSnip> snips;
        Include m_include;
        IncludeList m_extra_includes;
        QHash<QString, bool> m_includes_used;
        QString m_qtLibrary;
        QList<const TypeSystemTypeEntry*> m_requiredTypeSystems;
        QList<QString> m_requiredQtLibraries;
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
        ArrayTypeEntry(const TypeEntry *nested_type, int indirections) : TypeEntry("Array", ArrayType), m_nested_type(nested_type), m_indirections(indirections) {
            Q_ASSERT(m_nested_type);
        }

        void setNestedTypeEntry(TypeEntry *nested) {
            m_nested_type = nested;
        }
        const TypeEntry *nestedTypeEntry() const {
            return m_nested_type;
        }

        QString targetLangName() const {
            if(m_indirections==0)
                return m_nested_type->targetLangName() + "[]";
            else
                return "io.qt.QNativePointer[]";
        }
        QString jniName() const {
            if (m_nested_type->isPrimitive() && m_nested_type->jniName()!="void" && m_indirections==0)
                return m_nested_type->jniName() + "Array";
            else
                return "jobjectArray";
        }

    private:
        const TypeEntry *m_nested_type;
        int m_indirections;
};

class PrimitiveTypeEntry : public TypeEntry {
    public:
        PrimitiveTypeEntry(const QString &name, QString typeSystemPackage)
                : TypeEntry(name, PrimitiveType),
                m_preferred_java_type(true), m_package(typeSystemPackage) {
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

        QString typeSystemPackage() const{
            return m_package;
        }

    private:
        QString m_java_name;
        QString m_jni_name;
        bool m_preferred_java_type;
        QString m_package;
};

class FunctionalTypeEntry : public TypeEntry {
    public:
        FunctionalTypeEntry(const QString &nspace, const QString &name)
                : TypeEntry(nspace.isEmpty() ? name : nspace + QLatin1String("::") + name,
                            FunctionalType),
                m_qualifier_type(nullptr),
                m_include(),
                m_extra_includes(), m_count(0), m_pp_condition(), m_using(), m_normalizedSignature() {
            m_qualifier = nspace;
            m_java_name = name;
        }

        QString javaPackage() const {
            return m_package_name;
        }
        void setTargetLangPackage(const QString &package) {
            m_package_name = package;
        }

        void setTargetLangName(const QString &name) {
            m_java_name = name;
        }

        QString targetLangName() const {
            return m_java_name;
        }
        QString javaQualifier() const;
        QString qualifiedTargetLangName() const {
            QString pkg = javaPackage();
            QString javaQual = javaQualifier();
            if (pkg.isEmpty()) return javaQual.isEmpty() ? targetLangName() : javaQual + '.' + targetLangName();
            return javaQual.isEmpty() ? pkg + '.' + targetLangName() : pkg + '.' + javaQual + '.' + targetLangName();
        }

        QString qualifiedTargetLangJNIName() const {
            QString pkg = javaPackage().replace('.', '/');
            QString javaQual = javaQualifier();
            if (pkg.isEmpty()) return javaQual.isEmpty() ? targetLangName() : javaQual + '$' + targetLangName();
            return javaQual.isEmpty() ? pkg + '/' + targetLangName() : pkg + '.' + javaQual + '$' + targetLangName();
        }

        QString jniName() const{
            return "jobject";
        }

        const QString& qualifier() const {
            return m_qualifier;
        }
        void setQualifier(const QString &q) {
            m_qualifier = q;
        }

        virtual bool preferredConversion() const {
            return false;
        }

        virtual QString implements() const {
            return m_implements;
        }

        virtual void setImplements(QString & implements){
            m_implements = implements;
        }

        const Include& include() const {
            return m_include;
        }
        void setInclude(const Include &inc) {
            m_include = inc;
        }

        const IncludeList& extraIncludes() const {
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

        void addArgumentModification(const ArgumentModification &argumentModification) {
            argument_mods << argumentModification;
        }

        void addModification(const Modification &modification) {
            m_modifications << modification;
        }

        const QList<ArgumentModification>& argumentModification() const{
            return argument_mods;
        }

        const QList<Modification>& modifications() const{
            return m_modifications;
        }

        void setCount(uint count) { m_count = count; }

        uint count() const { return m_count; }

        virtual bool isNativeIdBased() const {
            return false;
        }

        const QString& ppCondition() const {
            return m_pp_condition;
        }
        void setPPCondition(const QString &pp_condition) {
            m_pp_condition = pp_condition;
        }

        const QString& getUsing() const {
            return m_using;
        }
        void setUsing(const QString &_using) {
            m_using = _using;
            if(m_normalizedSignature.isEmpty())
                m_normalizedSignature = _using;
        }

        const QString& normalizedSignature() const {
            return m_normalizedSignature.isEmpty() ? name() : m_normalizedSignature;
        }
        void setNormalizedSignature(const QString &_normalizedSignature) {
            m_normalizedSignature = _normalizedSignature;
        }

        virtual QString qualifiedCppName() const {
            return m_using.isEmpty() ? name() : m_using;
        }
        QString targetTypeSystem() const {
            return m_target_typesystem;
        }
        void setTargetTypeSystem(const QString &qt_jambi_library) {
            m_target_typesystem = qt_jambi_library;
        }
    private:
        QString m_implements;
        QString m_package_name;
        mutable const TypeEntry *m_qualifier_type;
        QString m_qualifier;
        QString m_java_name;
        Include m_include;
        IncludeList m_extra_includes;
        QList<Modification> m_modifications;
        QHash<QString, bool> m_includes_used;
        QList<ArgumentModification> argument_mods;
        uint m_count;
        QString m_pp_condition;
        QString m_using;
        QString m_normalizedSignature;
        QString m_target_typesystem;
};

class EnumTypeEntry : public TypeEntry {
    public:
        EnumTypeEntry(const QString &nspace, const QString &enumName)
                : TypeEntry(nspace.isEmpty() ? enumName : nspace + QLatin1String("::") + enumName,
                            EnumType),
                m_qualifier_type(nullptr),
                m_flags(nullptr),
                m_extensible(false),
                m_noMetaObject(false),
                m_force_integer(false),
                m_isEnumClass(false),
                m_isPublic(true),
                m_size(32),
                m_include(),
                m_pp_condition(){
            m_qualifier = nspace;
            m_java_name = enumName;
        }

        QString javaPackage() const {
            return m_package_name;
        }
        void setTargetLangPackage(const QString &package) {
            m_package_name = package;
        }
        QString targetTypeSystem() const {
            return m_target_typesystem;
        }
        void setTargetTypeSystem(const QString &qt_jambi_library) {
            m_target_typesystem = qt_jambi_library;
        }

        QString targetLangName() const {
            return m_java_name;
        }
        QString javaQualifier() const;
        QString qualifiedTargetLangName() const {
            QString pkg = javaPackage();
            QString javaQual = javaQualifier().replace('$', '.');
            if (pkg.isEmpty()) return javaQual.isEmpty() ? targetLangName() : javaQual + '.' + targetLangName();
            return javaQual.isEmpty() ? pkg + '.' + targetLangName() : pkg + '.' + javaQual + '.' + targetLangName();
        }

        QString qualifiedTargetLangJNIName() const {
            QString pkg = javaPackage().replace('.', '/');
            QString javaQual = javaQualifier();
            if (pkg.isEmpty()) return javaQual.isEmpty() ? targetLangName() : javaQual + '$' + targetLangName();
            return javaQual.isEmpty() ? pkg + '/' + targetLangName() : pkg + '/' + javaQual + '$' + targetLangName();
        }

        QString jniName() const;

        const QString& qualifier() const {
            return m_qualifier;
        }

        QString qualifiedCppName() const{
            if(m_cppName.isEmpty())
                return TypeEntry::qualifiedCppName();
            else
                return m_cppName;
        }

        void setCppName(const QString& s){
            m_cppName = s;
        }

        virtual bool preferredConversion() const {
            return false;
        }

        bool isBoundsChecked() const {
            return m_lower_bound.isEmpty() && m_upper_bound.isEmpty();
        }

        const QString& upperBound() const {
            return m_upper_bound;
        }
        void setUpperBound(const QString &bound) {
            m_upper_bound = bound;
        }

        const QString& lowerBound() const {
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

        bool isHiddenMetaObject() const {
            return m_noMetaObject;
        }
        void setHiddenMetaObject(bool is) {
            m_noMetaObject = is;
        }

        bool isEnumValueRejected(const QString &name) {
            return m_rejected_enums.contains(name);
        }
        bool isEnumValueRemoveRejected(const QString &name) {
            return m_rejected_and_removed_enums.contains(name);
        }
        void addEnumValueRejection(const QString &name, bool remove) {
            m_rejected_enums << name;
            if(remove)
                m_rejected_and_removed_enums << name;
        }
        const QStringList& enumValueRejections() const {
            return m_rejected_enums;
        }
        const QStringList& enumValueRemoveRejections() const {
            return m_rejected_and_removed_enums;
        }
        void addRenamedEnumValue(const QString &name, const QString & newName) {
            m_renamed_enums[name] = newName;
        }
        const QMap<QString,QString>& renamedEnumValues() const {
            return m_renamed_enums;
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

        const Include& include() const {
            return m_include;
        }
        void setInclude(const Include &inc) {
            m_include = inc;
        }
        bool isScopedEnum() const {
            return m_isEnumClass;
        }
        void setEnumClass(bool b){
            m_isEnumClass = b;
        }
        uint size() const {
            return m_size;
        }
        void setSize(uint s){
            m_size = s;
        }

        bool isPublic() const {
            return m_isPublic;
        }
        void setPublic(bool s){
            m_isPublic = s;
        }

        const QString& ppCondition() const {
            return m_pp_condition;
        }
        void setPPCondition(const QString &pp_condition) {
            m_pp_condition = pp_condition;
        }

        const QList<CodeSnip>& codeSnips() const {
            return m_code_snips;
        }
        void setCodeSnips(const QList<CodeSnip>&snips) {
            m_code_snips = snips;
        }

        void addCodeSnip(const CodeSnip &snip) {
            m_code_snips << snip;
        }

    private:
        QString m_implements;
        QString m_package_name;
        QString m_target_typesystem;
        QString m_qualifier;
        QString m_cppName;
        mutable const TypeEntry *m_qualifier_type;
        QString m_java_name;

        QString m_lower_bound;
        QString m_upper_bound;

        QStringList m_rejected_enums;
        QStringList m_rejected_and_removed_enums;
        QMap<QString,QString> m_renamed_enums;
        QList<EnumValueRedirection> m_enum_redirections;

        FlagsTypeEntry *m_flags;

        bool m_extensible;
        bool m_noMetaObject;
        bool m_force_integer;
        bool m_isEnumClass;
        bool m_isPublic;
        uint m_size;
        Include m_include;
        QString m_pp_condition;
        QList<CodeSnip> m_code_snips;
};

class FlagsTypeEntry : public TypeEntry {
    public:
        FlagsTypeEntry(const QString &name) : TypeEntry(name, FlagsType), m_enum(nullptr) {
        }

        QString qualifiedTargetLangName() const;
        QString qualifiedTargetLangJNIName() const;
        QString targetLangName() const {
            return m_java_name;
        }
        QString jniName() const;
        virtual bool preferredConversion() const {
            return false;
        }

        const QString& originalName() const {
            return m_original_name;
        }
        void setOriginalName(const QString &s) {
            m_original_name = s;
        }

        const QString& flagsName() const {
            return m_java_name;
        }
        void setFlagsName(const QString &name) {
            m_java_name = name;
        }

        bool forceInteger() const {
            return m_enum->forceInteger();
        }

        const EnumTypeEntry *originator() const {
            return m_enum;
        }
        void setOriginator(EnumTypeEntry *e) {
            m_enum = e;
        }

        QString javaPackage() const {
            return m_enum->javaPackage();
        }

        QString targetTypeSystem() const {
            return m_enum->targetTypeSystem();
        }

        QString qualifiedCppName() const{
            if(m_cppName.isEmpty())
                return TypeEntry::qualifiedCppName();
            else
                return m_cppName;
        }

        void setCppName(const QString& s){
            m_cppName = s;
        }

    private:
        QString m_original_name;
        QString m_java_name;
        EnumTypeEntry *m_enum;
        QString m_cppName;
};

class ComplexTypeEntry : public TypeEntry {
    public:
        enum TypeFlag {
            ForceAbstract       = 0x01,
            ThreadAffine        = 0x02,
            Deprecated          = 0x04,
            ForceFriendly       = 0x10,
        };
        typedef QFlags<TypeFlag> TypeFlags;

        ComplexTypeEntry(const QString &name, Type t)
            : TypeEntry(QString(name).replace("::", "$"), t), // the A$B notation is the java binary name of an embedded class
                m_pp_condition(),
                m_qualified_cpp_name(name),
                m_is_qobject(false),
                m_is_qwidget(false),
                m_is_qwindow(false),
                m_is_qapplication(false),
                m_polymorphic_base(false),
                m_generic_class(false),
                m_type_flags() {
            Include inc;
            inc.name = "QtCore/QVariant";
            inc.type = Include::IncludePath;

            addExtraInclude(inc);
        }

        bool isComplex() const {
            return true;
        }

        const IncludeList& extraIncludes() const {
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
            centry->setPPCondition(ppCondition());
            centry->setExtraIncludes(extraIncludes());
            centry->setFunctionModifications(functionModifications());
            centry->setFieldModifications(fieldModifications());
            centry->setQObject(isQObject());
            centry->setQWidget(isQWidget());
            centry->setQWindow(isQWindow());
            centry->setQCoreApplication(isQCoreApplication());
            centry->setDefaultSuperclass(defaultSuperclass());
            centry->setCodeSnips(codeSnips());
            centry->setTargetLangPackage(javaPackage());
            centry->setTargetTypeSystem(targetTypeSystem());

            return centry;
        }

        void setLookupName(const QString &name) {
            m_lookup_name = name;
        }

        virtual QString lookupName() const override {
            return m_lookup_name.isEmpty() ? targetLangName() : m_lookup_name;
        }

        QString jniName() const override {
            return strings_jobject;
        }


        const Include& include() const {
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

        const CodeSnipList& codeSnips() const {
            return m_code_snips;
        }
        void setCodeSnips(const CodeSnipList &codeSnips) {
            m_code_snips = codeSnips;
        }
        void addCodeSnips(const CodeSnipList &codeSnips) {
            m_code_snips << codeSnips;
        }
        void addCodeSnip(const CodeSnip &codeSnip) {
            m_code_snips << codeSnip;
        }

        const FunctionModificationList& functionModifications() const {
            return m_function_mods;
        }
        void setFunctionModifications(const FunctionModificationList &functionModifications) {
            m_function_mods = functionModifications;
        }
        void addFunctionModifications(const FunctionModificationList &functionModifications) {
            m_function_mods << functionModifications;
        }
        void addFunctionModification(const FunctionModification &functionModification) {
            m_function_mods << functionModification;
        }
        FunctionModificationList functionModifications(const QString &signature) const;

        bool hasFunctionCodeInjections(const QString &methodSignature, TypeSystem::Language language, const QSet<CodeSnip::Position>& positions) const;

        FieldModification fieldModification(const QString &name) const;
        void setFieldModifications(const FieldModificationList &mods) {
            m_field_mods = mods;
        }
        void addFieldModifications(const FieldModificationList &mods) {
            m_field_mods << mods;
        }
        void addFieldModification(const FieldModification &mod) {
            m_field_mods << mod;
        }
        const FieldModificationList& fieldModifications() const {
            return m_field_mods;
        }

        QString javaPackage() const override {
            return m_package;
        }
        void setTargetLangPackage(const QString &package) {
            m_package = package;
        }

        QString targetTypeSystem() const override {
            return m_target_typesystem;
        }

        void setTargetTypeSystem(const QString &qt_jambi_library) {
            m_target_typesystem = qt_jambi_library;
        }

        bool isQObject() const {
            return m_is_qobject;
        }
        void setQObject(bool qobject) {
            m_is_qobject = qobject;
        }

        bool isQWidget() const {
            return m_is_qwidget;
        }
        void setQWidget(bool qWidget) {
            m_is_qwidget = qWidget;
        }

        bool isQWindow() const {
            return m_is_qwindow;
        }
        void setQWindow(bool qw) {
            m_is_qwindow = qw;
        }

        bool isQCoreApplication() const {
            return m_is_qapplication;
        }
        void setQCoreApplication(bool qw) {
            m_is_qapplication = qw;
        }

        const QString& defaultSuperclass() const {
            return m_default_superclass;
        }
        void setDefaultSuperclass(const QString &sc) {
            m_default_superclass = sc;
        }

        QString qualifiedCppName() const override;

        /**
         * the class-type attribute 'implements' can be used to extend the list of interfaces used by a class
         * @brief setImplements
         * @param implements
         */
        void setImplements(const QString &implements) {
            m_implements = implements;
        }
        const QString& implements() const {
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
        const QString& polymorphicIdValue() const {
            return m_polymorphic_id_value;
        }

        void addInterfacePolymorphicIdValue(const QString &iface, const QString &value) {
            m_interface_polymorphic_id_values[iface] = value;
        }
        const QMap<QString,QString>& interfacePolymorphicIdValues() const {
            return m_interface_polymorphic_id_values;
        }

        void setExpensePolicy(const ExpensePolicy &policy) {
            m_expense_policy = policy;
        }
        const ExpensePolicy &expensePolicy() const {
            return m_expense_policy;
        }

        const QString& targetType() const {
            return m_target_type;
        }
        void setTargetType(const QString &code) {
            m_target_type = code;
        }

        QString targetLangName() const override {
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

        const QString& threadAffinity() const {
            return m_threadAffinity;
        }
        void setThreadAffinity(const QString &code) {
            m_threadAffinity = code;
        }

        const QString& ppCondition() const override {
            return m_pp_condition;
        }
        void setPPCondition(const QString &pp_condition) {
            m_pp_condition = pp_condition;
        }

        void addDelegatedBaseClass(QString baseClass, QString delegate){
            m_deletatedBaseClasses.insert(baseClass, delegate);
        }

        const QMap<QString,QString>& deletatedBaseClasses() const{
            return m_deletatedBaseClasses;
        }

    private:
        IncludeList m_extra_includes;
        Include m_include;
        QHash<QString, bool> m_includes_used;
        FunctionModificationList m_function_mods;
        FieldModificationList m_field_mods;
        CodeSnipList m_code_snips;
        QString m_pp_condition;
        QString m_package;
        QString m_target_typesystem;
        QString m_default_superclass;
        QString m_qualified_cpp_name;
        QString m_java_name;
        QString m_implements;
        QString m_threadAffinity;

        uint m_is_qobject : 1;
        uint m_is_qwidget : 1;
        uint m_is_qwindow : 1;
        uint m_is_qapplication : 1;
        uint m_polymorphic_base : 1;
        uint m_generic_class : 1;

        QString m_polymorphic_id_value;
        QMap<QString,QString> m_interface_polymorphic_id_values;
        QString m_lookup_name;
        QString m_target_type;
        ExpensePolicy m_expense_policy;
        TypeFlags m_type_flags;
        QMap<QString,QString> m_deletatedBaseClasses;
};

class AliasTypeEntry : public ComplexTypeEntry {
    public:
        AliasTypeEntry(const QString &name, QString typeSystemPackage)
                : ComplexTypeEntry(name, AliasType) {
            setCodeGeneration(GenerateNothing);
            setTargetLangPackage(typeSystemPackage);
        }

        QString targetLangName() const override {
            return "";
        }

        QString jniName() const override {
            return "";
        }

        virtual bool preferredTargetLangType() const {
            return false;
        }
        virtual void setPreferredTargetLangType(bool) {
        }
};

class NamespaceTypeEntry : public ComplexTypeEntry {
    public:
        NamespaceTypeEntry(const QString &name) : ComplexTypeEntry(name, NamespaceType) { }
};

class ImplementorTypeEntry;

class InterfaceTypeEntry : public ComplexTypeEntry {
    public:
        InterfaceTypeEntry(const QString &name)
                : ComplexTypeEntry(name, InterfaceType), m_origin(nullptr) {
        }

        static QString implName(const QString &name) {
            return name + "::Impl";
        }

        bool noImpl() const{
            return m_noImpl;
        }

        void setNoImpl(bool noImpl){
            m_noImpl = noImpl;
        }

        ImplementorTypeEntry *origin() const {
            return m_origin;
        }
        void setOrigin(ImplementorTypeEntry *origin) {
            m_origin = origin;
        }

        virtual bool isNativeIdBased() const {
            return USE_NATIVE_IDS;
        }

    private:
        ImplementorTypeEntry *m_origin;
        bool m_noImpl;
};

class ImplementorTypeEntry : public ComplexTypeEntry {
public:
    ImplementorTypeEntry(const QString &name, Type t)
            : ComplexTypeEntry(name, t), m_interface(nullptr) {
    }

    InterfaceTypeEntry *designatedInterface() const {
        return m_interface;
    }
    void setDesignatedInterface(InterfaceTypeEntry *entry) {
        m_interface = entry;
    }

    virtual bool isNativeIdBased() const {
        return m_interface==nullptr ? USE_NATIVE_IDS : m_interface->isNativeIdBased();
    }

private:
    InterfaceTypeEntry *m_interface;
};

class GlobalTypeEntry : public ComplexTypeEntry {
public:
    GlobalTypeEntry(const QString &name)
            : ComplexTypeEntry(name, GlobalType) {
    }

    virtual bool isNativeIdBased() const {
        return false;
    }

private:
};

class TemplateTypeEntry : public ImplementorTypeEntry {
    public:
        TemplateTypeEntry(const QString &name)
                : ImplementorTypeEntry(name, TemplateType) {
        }
};

class ObjectTypeEntry : public ImplementorTypeEntry {
    public:
        ObjectTypeEntry(const QString &name)
                : ImplementorTypeEntry(name, ObjectType) {
        }
        bool isValueOwner() const;
        void setIsValueOwner(bool is_value_owner);
private:
        bool m_is_value_owner;
};

class ValueTypeEntry : public ImplementorTypeEntry {
    public:
        ValueTypeEntry(const QString &name) : ImplementorTypeEntry(name, BasicValueType) { }

        bool isValue() const {
            return true;
        }

    protected:
        ValueTypeEntry(const QString &name, Type t) : ImplementorTypeEntry(name, t) { }
};

class StringTypeEntry : public ValueTypeEntry {
    public:
        StringTypeEntry(const QString &name)
                : ValueTypeEntry(name,
                                 name=="QString" ? StringType :
                                (name=="QLatin1String" ? Latin1StringType :
                                (name=="QStringView" ? StringViewType : StringRefType))) {
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

class JWrapperTypeEntry: public ValueTypeEntry {
    public:
        JWrapperTypeEntry(Type type, const QString &name, const QString &javaPackage, const QString &targetLangName) :
            ValueTypeEntry(name, type),
            m_javaPackage(javaPackage),
            m_targetLangName(targetLangName) { }

        QString jniName() const {
            return strings_jobject;
        }
        QString targetLangName() const {
            return m_targetLangName;
        }
        QString javaPackage() const {
            return m_javaPackage;
        }

        bool isNativeIdBased() const {
            return false;
        }

private:
        QString m_javaPackage;
        QString m_targetLangName;
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
            QWeakPointer,
            QScopedPointer,
            QPointer,
            unique_ptr,
            shared_ptr,
            weak_ptr
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

class InitializerListTypeEntry: public ValueTypeEntry {
    public:
        InitializerListTypeEntry() : ValueTypeEntry("std::initializer_list", InitializerListType) {}

        QString jniName() const {
            return strings_jobjectArray;
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
    private:
};


class ContainerTypeEntry : public ComplexTypeEntry {
    public:
        enum Type {
            NoContainer,
            InitializerListContainer,
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
            QDBusReplyContainer,
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
            QDeclarativeListPropertyContainer,
            QArrayDataContainer,
            QTypedArrayDataContainer,
            QModelRoleDataSpanContainer,
            QBindableContainer,
            QPropertyBindingContainer,
            std_atomic,
            std_optional,
            std_vector
        };

        ContainerTypeEntry(const QString &name, Type type)
                : ComplexTypeEntry(name, ContainerType), m_type(type) {
            setCodeGeneration(GenerateForSubclass);
        }

        Type type() const { return m_type; }
        QString targetLangName() const;
        QString javaPackage() const;
        QString qualifiedCppName() const;
        QString collectionWrapperClass() const;

    private:
        Type m_type;
};

class IteratorTypeEntry : public ComplexTypeEntry {
    public:
        IteratorTypeEntry(const QString &name, const ComplexTypeEntry* containerType) :
            ComplexTypeEntry(name, IteratorType),
            m_containerType(containerType),
            m_qualifiedCppContainerName(),
            m_isPointer(false)
        {
            setCodeGeneration(GenerateForSubclass);
        }
        IteratorTypeEntry(const QString &name, const QString& qualifiedCppContainerName, const ComplexTypeEntry* containerType, bool isPointer) :
            ComplexTypeEntry(name, IteratorType),
            m_containerType(containerType),
            m_qualifiedCppContainerName(qualifiedCppContainerName),
            m_isPointer(isPointer)
        {
//            setCodeGeneration(GenerateNothing);
        }
        IteratorTypeEntry* clone(const ComplexTypeEntry* containerType, const QString& qualifiedCppContainerName) const;
        QString targetLangName() const;
        QString javaPackage() const;
        QString qualifiedCppName() const;
        QString iteratorName() const;
        const QString& qualifiedCppContainerName() const;
        const ComplexTypeEntry* containerType() const {return m_containerType;}
        void setContainerType(const ComplexTypeEntry* t) {m_containerType = t;}
        void setQualifiedCppContainerName(const QString& t) {m_qualifiedCppContainerName = t;}
        void setIsPointer(bool isPointer) {m_isPointer = isPointer;}
        bool isPointer() const {return m_isPointer;}
    private:
        const ComplexTypeEntry* m_containerType;
        QString m_qualifiedCppContainerName;
        bool m_isPointer;
};

#endif
