/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
extern QString strings_jobjectArray;

class GeneratorApplication;

namespace TS {

class InterfaceTypeEntry;
class ObjectTypeEntry;

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
            AutoType,
            FlagsType,
            EnumType,
            FunctionalType,
            TemplateArgumentType,
            InstantiatedTemplateArgumentType,
            BasicValueType,
            QStringType,
            QLatin1StringType,
            QLatin1StringViewType,
            QStringViewType,
            QAnyStringViewType,
            QUtf8StringViewType,
            QStringRefType,
            ContainerType,
            IteratorType,
            InterfaceType,
            ObjectType,
            TemplateType,
            NamespaceType,
            QVariantType,
            PointerContainerType,
            InitializerListType,
            JObjectWrapperType,
            QAndroidJniObjectType,
            JMapWrapperType,
            JCollectionWrapperType,
            JIteratorWrapperType,
            JEnumWrapperType,
            JQFlagsWrapperType,
            QCharType,
            ArrayType,
            TypeSystemType,
            QMetaObjectType,
            QMetaObjectConnectionType,
            GlobalType,
            NativePointerType,
            UnknownType,
        };

        enum CodeGeneration {
            GenerateTargetLang      = 0x000001,
            GenerateCpp             = 0x000002,
            GenerateForSubclass     = 0x000004,
            GenerateNoShell         = 0x000010,
            InheritedByTypeSystem   = 0x010000,

            GenerateNothing         = 0x000000,
            GenerateAll             = 0x00ff0f,
            GenerateCode            = GenerateTargetLang | GenerateCpp
        };

        TypeEntry(const QString &name, Type t)
                : m_name(name),
                m_type(t),
                m_code_generation(GenerateAll),
                m_isDeclaredDeprecated(false),
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
        bool isQMetaObjectType() const {
            return m_type == QMetaObjectType;
        }
        bool isQMetaObjectConnectionType() const {
            return m_type == QMetaObjectConnectionType;
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
            return m_type == QStringType
                    || m_type == QLatin1StringType
                    || m_type == QLatin1StringViewType
                    || m_type == QStringViewType
                    || m_type == QAnyStringViewType
                    || m_type == QUtf8StringViewType
                    || m_type == QStringRefType;
        }
        bool isQString() const {
            return m_type == QStringType;
        }
        bool isQLatin1String() const {
            return m_type == QLatin1StringType;
        }
        bool isQLatin1StringView() const {
            return m_type == QLatin1StringViewType;
        }
        bool isQStringView() const {
            return m_type == QStringViewType;
        }
        bool isQAnyStringView() const {
            return m_type == QAnyStringViewType;
        }
        bool isQUtf8StringView() const {
            return m_type == QUtf8StringViewType;
        }
        bool isQStringRef() const {
            return m_type == QStringRefType;
        }
        bool isQChar() const {
            return m_type == QCharType;
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
        bool isQVariant() const {
            return m_type == QVariantType;
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
        bool isAuto() const {
            return m_type == AutoType;
        }
        bool isUnknown() const {
            return m_type == UnknownType;
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

        bool isNativePointer() const {
            return m_type == NativePointerType;
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

        void setDeclDeprecated(bool declaredDeprecated){
            m_isDeclaredDeprecated = declaredDeprecated;
        }

        bool isDeclDeprecated() const{
            return m_isDeclaredDeprecated;
        }

        void setInUse(bool inUse){
            m_in_use = inUse;
        }

        bool isInUse() const{
            return m_in_use;
        }
    private:
        QString m_name;
        Type m_type;
        uint m_code_generation;
        bool m_isDeclaredDeprecated;
        // currently not yet in use:
        bool m_preferred_conversion;
        bool m_in_use;
};
typedef QMap<QString, QList<TypeEntry *> > TypeEntryHash;
typedef QMap<QString, TypeEntry *> SingleTypeEntryHash;

struct Dependency{
    enum Mode{
        Mandatory,
        Optional,
        ProvideOnly
    };

    QString entry;
    Mode mode;
    QStringList platforms;
    Dependency(QString&& _entry, Mode _mode = Mandatory, QStringList&& _platforms = {})
        : entry(std::move(_entry)),
          mode(_mode),
          platforms(std::move(_platforms))
    {}
    Dependency(const Dependency& d)
        : entry(d.entry),
          mode(d.mode),
          platforms(d.platforms)
    {}
    Dependency(Dependency&& d)
        : entry(std::move(d.entry)),
          mode(std::move(d.mode)),
          platforms(std::move(d.platforms))
    {}
    Dependency()
        : entry(),
          mode(Mandatory),
          platforms()
    {}

    Dependency& operator=(const Dependency& d){
        entry=d.entry;
        mode=d.mode;
        platforms=d.platforms;
        return *this;
    }

    Dependency& operator=(Dependency&& d){
        entry=std::move(d.entry);
        mode=std::move(d.mode);
        platforms=std::move(d.platforms);
        return *this;
    }
};

class TypeSystemTypeEntry : public TypeEntry {
    public:

        TypeSystemTypeEntry(const QString &name);
        TypeSystemTypeEntry(const QString &name, const QString &lib, const QString &module);

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
            m_extra_includes << include;
        }

        const QString& qtLibrary() const {
            return m_qtLibrary;
        }

        const QString& module() const {
            return m_module;
        }

        const QString& description() const {
            return m_description;
        }
        void setDescription(const QString &description) {
            m_description = description;
        }

        const QList<const TypeSystemTypeEntry*>& requiredTypeSystems() const{
            return m_requiredTypeSystems;
        }

        void addRequiredTypeSystem(const TypeSystemTypeEntry* entry){
            if(entry && entry!=this)
                m_requiredTypeSystems.append(entry);
        }

        const QList<Dependency>& requiredQtLibraries() const{
            return m_requiredQtLibraries;
        }

        void addRequiredQtLibrary(QString&& entry, Dependency::Mode mode = Dependency::Mandatory, QStringList&& platforms = {}){
            m_requiredQtLibraries.append(Dependency(std::move(entry), mode, std::move(platforms)));
        }

        void addCodeSnip(const QString& package, const CodeSnip &snip) {
            snips[package] << snip;
        }

        void addForwardDeclaration(const QString &forwardDeclaration) {
            m_forwardDeclarations << forwardDeclaration;
        }

        const QList<QString>& forwardDeclarations() const{
            return m_forwardDeclarations;
        }

        bool isNoExports() const{
            return m_noExports;
        }

        void setNoExports(bool noExports) {
            m_noExports = noExports;
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

        bool hasFunctionCodeInjections(const QString &methodSignature, TS::Language language, const QSet<CodeSnip::Position>& positions) const;

        QMap<QString,QList<CodeSnip>> snips;
        Include m_include;
        IncludeList m_extra_includes;
        QString m_qtLibrary;
        QString m_module;
        QList<const TypeSystemTypeEntry*> m_requiredTypeSystems;
        QList<Dependency> m_requiredQtLibraries;
        QString m_description;
        QList<QString> m_forwardDeclarations;
        FunctionModificationList m_function_mods;
        bool m_noExports;
        QString m_targetName;
        const QString &targetName() const;
        void setTargetName(const QString &newTargetName);
};

class VoidTypeEntry : public TypeEntry {
    public:
        VoidTypeEntry() : TypeEntry("void", VoidType) {
            setCodeGeneration(GenerateNothing);
        }
};

class AutoTypeEntry : public TypeEntry {
    public:
        AutoTypeEntry() : TypeEntry("auto", AutoType) {
            setCodeGeneration(GenerateNothing);
        }
};

class NativePointerTypeEntry : public TypeEntry {
public:
    NativePointerTypeEntry(const QString& name) : TypeEntry(name, NativePointerType) {
        setCodeGeneration(GenerateNothing);
    }
    QString javaPackage() const override {
        return QStringLiteral("io.qt");
    }
    QString jniName() const override {
        return QStringLiteral("jobject");
    }
    QString targetLangName() const override {
        return QStringLiteral("QNativePointer");
    }

    const QString& ppCondition() const override {
        return m_pp_condition;
    }
    void setPPCondition(const QString &pp_condition) {
        m_pp_condition = pp_condition;
    }
    QString m_pp_condition;
};

class UnknownTypeEntry : public TypeEntry {
    public:
        UnknownTypeEntry(const QString &name) : TypeEntry(name, UnknownType) {
            setCodeGeneration(GenerateNothing);
        }
};

class TemplateArgumentEntry : public TypeEntry {
    public:
        TemplateArgumentEntry(const QString &name)
            : TypeEntry(name, TemplateArgumentType), m_ordinal(0), m_variadic(false) {
        }

        int ordinal() const {
            return m_ordinal;
        }
        void setOrdinal(int o) {
            m_ordinal = o;
        }

        bool isVariadic() const {
            return m_variadic;
        }
        void setVariadic(bool o) {
            m_variadic = o;
        }

    private:
        int m_ordinal;
        bool m_variadic;
};

class ArrayTypeEntry : public TypeEntry {
    public:
        ArrayTypeEntry(const QString& name, const TypeEntry *nested_type, int indirections) : TypeEntry(name, ArrayType), m_nested_type(nested_type), m_indirections(indirections) {
            Q_ASSERT(m_nested_type);
        }

        void setNestedTypeEntry(TypeEntry *nested) {
            m_nested_type = nested;
        }
        const TypeEntry *nestedTypeEntry() const {
            return m_nested_type;
        }

        QString targetLangName() const override {
            QString targetLangName = m_nested_type->targetLangName();
            if(!targetLangName.endsWith("QNativePointer")){
                for(int i=0; i<m_indirections; ++i){
                    targetLangName += "[]";
                }
            }
            return targetLangName;
        }
        QString jniName() const override {
            if (m_nested_type->isPrimitive() && m_nested_type->jniName()!="void" && m_indirections==1)
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

        QString targetLangName() const override {
            return m_java_name;
        }
        void setTargetLangName(const QString &targetLangName) {
            m_java_name  = targetLangName;
        }

        QString jniName() const override {
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
        FunctionalTypeEntry(const QString &nspace, const QString &name);

        QString javaPackage() const override;
        void setTargetLangPackage(const QString &package) {
            m_package_name = package;
        }

        void setTargetLangName(const QString &name) {
            m_java_name = name;
        }

        QString targetLangName() const override {
            return m_java_name;
        }
        QString javaQualifier() const;
        QString qualifiedTargetLangName() const override {
            QString pkg = javaPackage();
            QString javaQual = javaQualifier();
            if (pkg.isEmpty()) return javaQual.isEmpty() ? targetLangName() : javaQual + '.' + targetLangName();
            return javaQual.isEmpty() ? pkg + '.' + targetLangName() : pkg + '.' + javaQual + '.' + targetLangName();
        }

        QString qualifiedTargetLangJNIName() const override {
            QString pkg = javaPackage().replace('.', '/');
            QString javaQual = javaQualifier();
            if (pkg.isEmpty()) return javaQual.isEmpty() ? targetLangName() : javaQual + '$' + targetLangName();
            return javaQual.isEmpty() ? pkg + '/' + targetLangName() : pkg + '/' + javaQual + '$' + targetLangName();
        }

        QString jniName() const override {
            return "jobject";
        }

        const QString& qualifier() const {
            return m_qualifier;
        }
        void setQualifier(const QString &q) {
            m_qualifier = q;
        }

        bool preferredConversion() const override {
            return false;
        }

        QString implements() const {
            return m_implements;
        }

        void setImplements(const QString & implements){
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

        void disableNativeIdUsage() {
        }

        bool isNativeIdBased() const override {
            return false;
        }

        const QString& ppCondition() const override {
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

        QString qualifiedCppName() const override {
            return m_using.isEmpty() ? name() : m_using;
        }
        QString targetTypeSystem() const override {
            return m_target_typesystem;
        }
        void setTargetTypeSystem(const QString &qt_jambi_library) {
            m_target_typesystem = qt_jambi_library;
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
        const QString& functionName() const { return m_functionName; }
        void setFunctionName(const QString &names) { m_functionName = names; }

        bool isGenericClass() const {
            return m_generic_class;
        }
        void setGenericClass(bool isGeneric) {
            m_generic_class = isGeneric;
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
        QMap<QString, bool> m_includes_used;
        QList<ArgumentModification> argument_mods;
        uint m_count;
        QString m_pp_condition;
        QString m_using;
        QString m_normalizedSignature;
        QString m_target_typesystem;
        CodeSnipList m_code_snips;
        QString m_functionName;
        uint m_generic_class : 1;
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
                m_force_noFlags(false),
                m_isEnumClass(false),
                m_isPublic(true),
                m_size(32),
                m_include(),
                m_pp_condition(){
            m_qualifier = nspace;
            m_java_name = enumName;
        }

        QString javaPackage() const override;
        void setTargetLangPackage(const QString &package) {
            m_package_name = package;
        }
        QString targetTypeSystem() const override {
            return m_target_typesystem;
        }
        void setTargetTypeSystem(const QString &qt_jambi_library) {
            m_target_typesystem = qt_jambi_library;
        }

        QString targetLangName() const override {
            return m_java_name;
        }
        QString javaQualifier() const;
        QString qualifiedTargetLangName() const override {
            QString pkg = javaPackage();
            QString javaQual = javaQualifier().replace('$', '.');
            if (pkg.isEmpty()) return javaQual.isEmpty() ? targetLangName() : javaQual + '.' + targetLangName();
            return javaQual.isEmpty() ? pkg + '.' + targetLangName() : pkg + '.' + javaQual + '.' + targetLangName();
        }

        QString qualifiedTargetLangJNIName() const override {
            QString pkg = javaPackage().replace('.', '/');
            QString javaQual = javaQualifier();
            if (pkg.isEmpty()) return javaQual.isEmpty() ? targetLangName() : javaQual + '$' + targetLangName();
            return javaQual.isEmpty() ? pkg + '/' + targetLangName() : pkg + '/' + javaQual + '$' + targetLangName();
        }

        QString jniName() const override;

        const QString& qualifier() const {
            return m_qualifier;
        }

        QString qualifiedCppName() const override {
           return TypeEntry::qualifiedCppName();
        }

        void setJavaScope(const QString& s){
            m_javaScope = s;
        }

        const QString&javaScope() const {
            return m_javaScope;
        }

        bool preferredConversion() const override {
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

        bool forceNoFlags() const {
            return m_force_noFlags;
        }
        void setForceNoFlags(bool force) {
            m_force_noFlags = force;
        }

        QString implements() const {
            return m_implements;
        }

        void setImplements(const QString & implements){
            m_implements = implements;
        }

        const Include& include() const {
            return m_include;
        }
        void setInclude(const Include &inc) {
            m_include = inc;
        }
        bool isScopedEnum() const override {
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

        const QString& ppCondition() const override {
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
        QString m_javaScope;
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
        bool m_force_noFlags;
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

        QString qualifiedTargetLangName() const override;
        QString qualifiedTargetLangJNIName() const override;
        QString targetLangName() const override {
            return m_java_name;
        }
        QString jniName() const override;
        virtual bool preferredConversion() const override {
            return false;
        }

        const QString& flagsTemplate() const {
            return m_flagsTemplate;
        }
        void setFlagsTemplate(const QString &s) {
            m_flagsTemplate = s;
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

        QString javaPackage() const override {
            return m_enum->javaPackage();
        }

        QString targetTypeSystem() const override {
            return m_enum->targetTypeSystem();
        }

        QString qualifiedCppName() const override {
            return TypeEntry::qualifiedCppName();
        }
    private:
        QString m_flagsTemplate;
        QString m_java_name;
        EnumTypeEntry *m_enum;
};

class ComplexTypeEntry : public TypeEntry {
    public:
        enum ConstructorType{
            LegacyCopyConstructor,
            DefaultConstructor,
            CopyConstructor,
            MoveConstructor
        };
        enum TypeFlag {
            ForceAbstract       = 0x01,
            ThreadAffine        = 0x02,
            Deprecated          = 0x04,
            ForceFriendly       = 0x10,
            NestedNonPublic     = 0x20,
            HasNonPublicFields  = 0x40,
            HasFields           = 0x80,
        };
        typedef QFlags<TypeFlag> TypeFlags;

        ComplexTypeEntry(const QString &name, Type t);

        QString qualifiedCppName() const override;

        bool isComplex() const override;
        const IncludeList& extraIncludes() const;
        void setExtraIncludes(const IncludeList &includes);
        void addExtraInclude(const Include &include);

        ComplexTypeEntry *copy() const;

        void setLookupName(const QString &name);

        QString lookupName() const override;

        QString jniName() const override;

        const Include& include() const;
        void setInclude(const Include &inc);

        void setForceAbstract();

        bool isForceAbstract() const;

        void setThreadAffine();

        bool isThreadAffine() const;

        void setDeprecated();

        bool isDeprecated() const;

        void setForceFriendly();

        bool isForceFriendly() const;

        void setHasNonPublicFields();

        bool hasNonPublicFields() const;

        void setHasFields();

        bool hasFields() const;

        TypeFlags typeFlags() const;

        const CodeSnipList& codeSnips() const;
        void setCodeSnips(const CodeSnipList &codeSnips);
        void addCodeSnips(const CodeSnipList &codeSnips);
        void addCodeSnip(const CodeSnip &codeSnip);

        const FunctionModificationList& functionModifications() const;
        void setFunctionModifications(const FunctionModificationList &functionModifications);
        void addFunctionModifications(const FunctionModificationList &functionModifications);
        void addFunctionModification(const FunctionModification &functionModification);
        FunctionModificationList functionModifications(const QString &signature) const;

        bool hasFunctionCodeInjections(const QString &methodSignature, TS::Language language, const QSet<CodeSnip::Position>& positions) const;

        FieldModification fieldModification(const QString &name) const;
        void setFieldModifications(const FieldModificationList &mods);
        void addFieldModifications(const FieldModificationList &mods);
        void addFieldModification(const FieldModification &mod);
        const FieldModificationList& fieldModifications() const;

        QString javaPackage() const override;
        void setTargetLangPackage(const QString &package);

        QString targetTypeSystem() const override;

        void setTargetTypeSystem(const QString &qt_jambi_library);

        bool isQObject() const;
        void setQObject(bool qobject);
        bool isQEvent() const;
        void setQEvent(bool b);
        bool isQByteArrayView() const;
        void setQByteArrayView(bool b);
        bool isGLsync() const;

        bool isQWidget() const;
        void setQWidget(bool qWidget);

        bool isQWindow() const;
        void setQWindow(bool qw);

        bool isQAction() const;
        void setQAction(bool qw);

        bool isQMediaControl() const;
        void setQMediaControl(bool qw);

        bool isQCoreApplication() const;
        void setQCoreApplication(bool qw);

        const QString& defaultSuperclass() const;
        void setDefaultSuperclass(const QString &sc);
        void setImplements(const QString &implements);
        const QString& implements() const;

        void setIsPolymorphicBase(bool on);
        bool isPolymorphicBase() const;

        void setPolymorphicIdValue(const QString &value);
        const QString& polymorphicIdValue() const;

        void addInterfacePolymorphicIdValue(const QString &iface, const QString &value);
        const QMap<QString,QString>& interfacePolymorphicIdValues() const;

        void setExpensePolicy(const ExpensePolicy &policy);
        const ExpensePolicy &expensePolicy() const;

        const QString& targetType() const;
        void setTargetType(const QString &code);

        QString targetLangName() const override;
        void setTargetLangName(const QString &name);

        bool isGenericClass() const;
        void setGenericClass(bool isGeneric);

        bool isTemplate() const;
        void setTemplate(bool isTemplate);

        bool isNativeInterface() const;
        void setNativeInterface(bool isNativeInterface);

        bool inhibitMetaobject() const;
        void setInhibitMetaobject(bool inhibitMetaobject);

        const QString& threadAffinity() const;
        void setThreadAffinity(const QString &code);

        const QString& ppCondition() const override;
        void setPPCondition(const QString &pp_condition);

        void addDelegatedBaseClass(QString baseClass, QString delegate);

        const QMap<QString,QString>& delegatedBaseClasses() const;

        virtual bool isNativeIdBased() const override;

        void disableNativeIdUsage();

        void addInstantiation(const QStringList& instantiation, const ComplexTypeEntry* typeEntry = nullptr);

        const QMap<QStringList,const ComplexTypeEntry*>& instantiations() const;
        void setExtendType(const QString& extendType);
        const QString& extendType() const;

        void setCustomConstructor(const CustomFunction &func, ConstructorType type = CopyConstructor);
        CustomFunction customConstructor(ConstructorType type = CopyConstructor) const;

        void setCustomDestructor(const CustomFunction &func);
        const CustomFunction& customDestructor() const;

        bool skipMetaTypeRegistration() const;

        void setSkipMetaTypeRegistration(bool skipMetaTypeRegistration);

        void setHasPrivateConstructors();

        void setHasNonPrivateConstructors();

        bool hasPrivateConstructors() const;

        bool hasNonPrivateConstructors() const;

        bool hasJustPrivateConstructors() const;

        bool hasPublicDefaultConstructor() const;

        bool hasProtectedDefaultConstructor() const;

        bool hasPrivateDefaultConstructor() const;

        void setHasPublicDefaultConstructor();

        void setHasProtectedDefaultConstructor();

        void setHasPrivateDefaultConstructor();

        bool hasPublicCopyConstructor() const;

        bool hasProtectedCopyConstructor() const;

        bool hasPrivateCopyConstructor() const;

        void setHasPublicCopyConstructor();

        void setHasProtectedCopyConstructor();

        void setHasPrivateCopyConstructor();

        bool hasPublicMoveConstructor() const;

        bool hasProtectedMoveConstructor() const;

        bool hasPrivateMoveConstructor() const;

        void setHasPublicMoveConstructor();

        void setHasProtectedMoveConstructor();

        void setHasPrivateMoveConstructor();

        bool hasPublicMoveAssignment() const;

        bool hasProtectedMoveAssignment() const;

        bool hasPrivateMoveAssignment() const;

        void setHasPublicMoveAssignment();

        void setHasProtectedMoveAssignment();

        void setHasPrivateMoveAssignment();

        bool hasPublicDefaultAssignment() const;

        bool hasProtectedDefaultAssignment() const;

        bool hasPrivateDefaultAssignment() const;

        void setHasPublicDefaultAssignment();

        void setHasProtectedDefaultAssignment();

        void setHasPrivateDefaultAssignment();

        void setDestructorPrivate();

        bool isDestructorPrivate() const;

        void setDestructorProtected();

        bool isDestructorProtected() const;

        void setDestructorVirtual();

        bool isDestructorVirtual() const;

        bool isDestructorPublic() const;

        void setHasEquals();

        bool hasEquals() const;

        void setHasHash();

        bool hasHash() const;

        void setHasVirtualFunctions();

        bool hasVirtualFunctions() const;

        void setHasFinalFunctions();

        bool hasFinalFunctions() const;

        bool hasPureVirtualFunctions(bool nonPrivate = true) const;

        void setHasPureVirtualFunctions(bool nonPrivate = true);

        const QStringList& implicitCasts() const;

        void addImplicitCast(const QString& cast);

        template<typename Type>
        const QList<Type*>& declImplicitCasts() const {
            return *reinterpret_cast<const QList<Type*>*>(&m_declImplicitCasts);
        }

        template<typename Type>
        void addDeclImplicitCast(Type* cast){
            m_declImplicitCasts.append(cast);
        }
        bool getNoImplicitConstructors() const;
        void setNoImplicitConstructors(bool newNoImplicitConstructors);

        bool getNotAssignable() const;
        void setNotAssignable(bool newNotAssignable);

        bool getNotMoveAssignable() const;
        void setNotMoveAssignable(bool newNotMoveAssignable);

        bool getNotCloneable() const;
        void setNotCloneable(bool newNotCloneable);

        bool getPushUpStatics() const;
        void setPushUpStatics(bool newPushUpStatics);

    protected:
        enum ComplexAttributeFlag{
            IsQObject = 0x01,
            IsQWidget = 0x02,
            IsQWindow = 0x04,
            IsQAction = 0x08,
            IsQCoreApplication = 0x010,
            IsGLsync = 0x020,
            IsPolymorphicBase = 0x0200,
            IsGenericClass = 0x0400,
            IsTemplate = 0x0800,
            IsNativeInterface = 0x01000,
            InhibitMetaobject = 0x02000,
            IsNativeIdBased = 0x04000,
            IsNoImpl = 0x08000,
            IsValueOwner = 0x010000,
            SkipMetaTypeRegistration = 0x20000,
            ForceMetaTypeRegistration = 0x40000,
            IsQMediaControl = 0x80000,
            IsQEvent = 0x100000,
            IsQByteArrayView = 0x200000
        };
        QFlags<ComplexAttributeFlag> m_attributes;

        enum FunctionAttributeFlag{
            HasPublicDefaultConstructor = 0x1,
            HasPublicCopyConstructor = 0x2,
            HasPublicMoveConstructor = 0x4,
            HasProtectedDefaultConstructor = 0x8,
            HasProtectedCopyConstructor = 0x10,
            HasProtectedMoveConstructor = 0x20,
            HasPublicDefaultAssignment = 0x40,
            HasPublicMoveAssignment = 0x80,
            HasProtectedDefaultAssignment = 0x100,
            HasProtectedMoveAssignment = 0x200,
            HasPrivateDestructor = 0x400,
            HasProtectedDestructor = 0x800,
            HasVirtualDestructor = 0x1000,
            HasAnyNonPrivateConstructor = 0x2000,
            HasAnyPrivateConstructor = 0x4000,
            HasEquals = 0x8000,
            HasHash = 0x10000,
            HasVirtuals = 0x20000,
            HasPureVirtuals = 0x40000,
            HasFinals = 0x80000,
            HasPrivatePureVirtuals = 0x100000,
            HasPrivateDefaultConstructor = 0x200000,
            HasPrivateCopyConstructor = 0x400000,
            HasPrivateMoveConstructor = 0x800000,
            HasPrivateDefaultAssignment = 0x1000000,
            HasPrivateMoveAssignment = 0x2000000,
        };
        QFlags<FunctionAttributeFlag> m_functionAttributes;

private:
        IncludeList m_extra_includes;
        Include m_include;
        QMap<QString, bool> m_includes_used;
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
        QHash<ConstructorType,CustomFunction> m_customConstructors;
        CustomFunction m_customDestructor;
        QString m_polymorphic_id_value;
        QMap<QString,QString> m_interface_polymorphic_id_values;
        QString m_lookup_name;
        QString m_target_type;
        ExpensePolicy m_expense_policy;
        TypeFlags m_type_flags;
        QMap<QString,QString> m_delegatedBaseClasses;
        QMap<QStringList,const ComplexTypeEntry*> m_instantiations;
        QString m_extendType;
        QList<void*> m_declImplicitCasts;
        QStringList m_implicitCasts;
        bool noImplicitConstructors = false;
        bool notAssignable = false;
        bool notMoveAssignable = false;
        bool notCloneable = false;
        bool pushUpStatics = false;
        friend GeneratorApplication;
        friend class FunctionalTypeEntry;
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

        bool getAsNativePointer() const;
        void setAsNativePointer(bool newAsNativePointer);

        bool getHasIndirections() const;
        void setHasIndirections(bool newHasIndirections);

    private:
        bool asNativePointer = false;
        bool hasIndirections = false;
};

class NamespaceTypeEntry : public ComplexTypeEntry {
public:
        NamespaceTypeEntry(const QString &name, bool isHeader, bool hasMetaObject) : ComplexTypeEntry(name, NamespaceType), m_isHeader(isHeader), m_hasMetaObject(hasMetaObject) {
            disableNativeIdUsage();
        }
        bool isHeader() const{return m_isHeader;}
        bool hasMetaObject() const{return m_hasMetaObject;}
private:
        uint m_isHeader : 1;
        uint m_hasMetaObject : 1;
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
            return m_attributes.testFlag(IsNoImpl);
        }

        void setNoImpl(bool noImpl){
            m_attributes.setFlag(IsNoImpl, noImpl);
        }

        ImplementorTypeEntry *origin() const {
            return m_origin;
        }
        void setOrigin(ImplementorTypeEntry *origin) {
            m_origin = origin;
        }

    private:
        ImplementorTypeEntry *m_origin;
};

class ImplementorTypeEntry : public ComplexTypeEntry {
public:
    ImplementorTypeEntry(const QString &name, Type t)
            : ComplexTypeEntry(name, t), m_interface(nullptr) {
    }

    InterfaceTypeEntry *designatedInterface() const override {
        return m_interface;
    }
    void setDesignatedInterface(InterfaceTypeEntry *entry) {
        m_interface = entry;
    }

    virtual bool isNativeIdBased() const override {
        return m_interface==nullptr ? ComplexTypeEntry::isNativeIdBased() : m_interface->isNativeIdBased();
    }

    bool isValueOwner() const;
    void setIsValueOwner(bool is_value_owner);

private:
    InterfaceTypeEntry *m_interface;
};

class GlobalTypeEntry : public ComplexTypeEntry {
public:
    GlobalTypeEntry(const QString &name)
            : ComplexTypeEntry(name, GlobalType) {
        disableNativeIdUsage();
    }

    bool isNativeIdBased() const override {
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
};

class ValueTypeEntry : public ImplementorTypeEntry {
    public:
        ValueTypeEntry(const QString &name) : ImplementorTypeEntry(name, BasicValueType) { }

        bool isValue() const override {
            return true;
        }
    protected:
        ValueTypeEntry(const QString &name, Type t) : ImplementorTypeEntry(name, t) { }
};

class StringTypeEntry : public ValueTypeEntry {
    public:
        StringTypeEntry(const QString &name)
                : ValueTypeEntry(name,
                                 name=="QString" ? QStringType :
                                (name=="QLatin1String" ? QLatin1StringType :
                                (name=="QLatin1StringView" ? QLatin1StringViewType :
                                (name=="QStringView" ? QStringViewType :
                                (name=="QAnyStringView" ? QAnyStringViewType :
                                (name=="QUtf8StringView" ? QUtf8StringViewType : QStringRefType)))))) {
            setCodeGeneration(GenerateNothing);
        }

        QString jniName() const override {
            return QStringLiteral("jstring");
        }
        QString targetLangName() const override {
            return QStringLiteral("String");
        }
        QString javaPackage() const override {
            return QStringLiteral("java.lang");
        }

        bool isNativeIdBased() const override {
            return false;
        }
};

class CharTypeEntry : public ValueTypeEntry {
    public:
        CharTypeEntry(const QString &name) : ValueTypeEntry(name, QCharType) {
            setCodeGeneration(GenerateNothing);
        }

        QString jniName() const override {
            return strings_jchar;
        }
        QString targetLangName() const override {
            return strings_char;
        }
        QString javaPackage() const override {
            return QString();
        }

        bool isNativeIdBased() const override {
            return false;
        }
};

class JWrapperTypeEntry: public ValueTypeEntry {
    public:
        JWrapperTypeEntry(Type type, const QString &name, const QString &javaPackage, const QString &targetLangName) :
            ValueTypeEntry(name, type),
            m_javaPackage(javaPackage),
            m_targetLangName(targetLangName) { }

        QString jniName() const override {
            return strings_jobject;
        }
        QString targetLangName() const override {
            return m_targetLangName;
        }
        QString javaPackage() const override {
            return m_javaPackage;
        }

        bool isNativeIdBased() const override {
            return false;
        }

private:
        QString m_javaPackage;
        QString m_targetLangName;
};

class VariantTypeEntry: public ValueTypeEntry {
    public:
        VariantTypeEntry(const QString &name) : ValueTypeEntry(name, QVariantType) { }

        QString jniName() const override {
            return strings_jobject;
        }
        QString targetLangName() const override {
            return strings_Object;
        }
        QString javaPackage() const override {
            return strings_java_lang;
        }

        bool isNativeIdBased() const override {
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

        QString jniName() const override {
            return strings_jobject;
        }
        QString targetLangName() const override {
            return "";
        }
        QString javaPackage() const override {
            return "";
        }

        bool isNativeIdBased() const override {
            return false;
        }

        Type type() const { return m_type; }

    private:
        Type m_type;
};

class InitializerListTypeEntry: public ValueTypeEntry {
    public:
        InitializerListTypeEntry() : ValueTypeEntry("std::initializer_list", InitializerListType) {}

        QString jniName() const override {
            return strings_jobjectArray;
        }
        QString targetLangName() const override {
            return "";
        }
        QString javaPackage() const override {
            return "";
        }

        bool isNativeIdBased() const override {
            return false;
        }
    private:
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
            QDBusReplyContainer,
            /*
              For QDeclarative and QQml module.
              This entry is to support the QDeclarativeListProperty / QQmlListProperty
             */
            QQmlListPropertyContainer,
            QArrayDataContainer,
            QTypedArrayDataContainer,
            QModelRoleDataSpanContainer,
            QBindableContainer,
            QPropertyBindingContainer,
            std_atomic,
            std_optional,
            std_vector,
            std_array,
            std_chrono,
            std_chrono_template
        };

        ContainerTypeEntry(const QString &name, Type type);
        Type type() const;
        QString targetLangName() const override;
        QString javaPackage() const override;
        QString qualifiedCppName() const override;

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
            disableNativeIdUsage();
        }
        IteratorTypeEntry(const QString &name, const QString& qualifiedCppContainerName, const ComplexTypeEntry* containerType, bool isPointer) :
            ComplexTypeEntry(name, IteratorType),
            m_containerType(containerType),
            m_qualifiedCppContainerName(qualifiedCppContainerName),
            m_isPointer(isPointer)
        {
//            setCodeGeneration(GenerateNothing);
            disableNativeIdUsage();
        }
        IteratorTypeEntry* clone(const ComplexTypeEntry* containerType, const QString& qualifiedCppContainerName) const;
        QString targetLangName() const override;
        QString javaPackage() const override;
        QString qualifiedCppName() const override;
        QString iteratorName() const;
        const QString& qualifiedCppContainerName() const;
        const ComplexTypeEntry* containerType() const {return m_containerType;}
        void setContainerType(const ComplexTypeEntry* t) {m_containerType = t;}
        void setQualifiedCppContainerName(const QString& t) {m_qualifiedCppContainerName = t;}
        void setIsPointer(bool isPointer) {m_isPointer = isPointer;}
        bool isPointer() const {return m_isPointer;}
        void setIsConst(bool newIsConst);

private:
        const ComplexTypeEntry* m_containerType;
        QString m_qualifiedCppContainerName;
        bool m_isPointer;
        bool m_isConst = true;
};

class InstantiatedTemplateArgumentEntry : public ComplexTypeEntry {
    public:
    InstantiatedTemplateArgumentEntry(int ordinal, TypeEntry * templateArg, TypeEntry *instantiation, const QString & javaInstantiationBaseType = {"java.lang.Object"})
                : ComplexTypeEntry(instantiation->name(), InstantiatedTemplateArgumentType),
                  m_ordinal(ordinal),
                  m_templateArg(templateArg),
                  m_instantiation(instantiation),
                  m_javaInstantiationBaseType(javaInstantiationBaseType)
        {
            this->disableNativeIdUsage();
        }

        int ordinal() const {
            return m_ordinal;
        }

        QString targetLangName() const override {
            return m_templateArg->name();
        }
        QString javaPackage() const override {
            return {};
        }
        QString qualifiedCppName() const override {
            return m_instantiation->qualifiedCppName();
        }
        const QString & javaInstantiationBaseType() const{
            return m_javaInstantiationBaseType;
        }
    private:
        int m_ordinal;
        TypeEntry * m_templateArg;
        TypeEntry * m_instantiation;
        QString m_javaInstantiationBaseType;
};

class GLsyncTypeEntry : public ObjectTypeEntry {
    public:
        GLsyncTypeEntry() : ObjectTypeEntry("__GLsync") {
            setCodeGeneration(GenerateNothing);
            setTargetLangName("GLsync");
            setTargetTypeSystem("io.qt.gui");
            setTargetLangPackage("io.qt.gui.gl");
            setCodeGeneration(TypeEntry::GenerateNothing);
            m_attributes.setFlag(ComplexTypeEntry::IsGLsync);
        }
};

class QMetaObjectTypeEntry : public ComplexTypeEntry {
    public:
        QMetaObjectTypeEntry() : ComplexTypeEntry("QMetaObject", QMetaObjectType) {
            setCodeGeneration(GenerateNothing);
            disableNativeIdUsage();
        }

        QString javaPackage() const override { return "io.qt.core"; }

        bool isValue() const override { return false; }
};

class QMetaObjectConnectionTypeEntry : public ComplexTypeEntry {
    public:
        QMetaObjectConnectionTypeEntry() : ComplexTypeEntry("QMetaObject::Connection", QMetaObjectConnectionType) {
            setCodeGeneration(GenerateNothing);
            disableNativeIdUsage();
        }

        QString targetLangName() const override {
            return "QMetaObject$Connection";
        }

        virtual QString javaPackage() const override { return "io.qt.core"; }

        virtual bool isValue() const override { return true; }
};

}

#endif
