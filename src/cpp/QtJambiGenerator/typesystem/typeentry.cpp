/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
** (in parts)
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

#include <QtCore/QSet>
#include "typeentry.h"
#include "codesnip.h"
#include "typedatabase.h"

namespace TS {

FunctionalTypeEntry::FunctionalTypeEntry(const QString &nspace, const QString &name)
        : TypeEntry(nspace.isEmpty() ? name : nspace + QLatin1String("::") + name,
                    FunctionalType),
        m_qualifier_type(nullptr),
        m_include(),
        m_extra_includes(),
        m_count(0),
        m_pp_condition(),
        m_using(),
        m_normalizedSignature(),
        m_generic_class(false)
{
    m_qualifier = nspace;
    m_java_name = name;
}

ComplexTypeEntry::ComplexTypeEntry(const QString &name, Type t)
    : TypeEntry(QString(name).replace("::", "$"), t), // the A$B notation is the java binary name of an embedded class
        m_pp_condition(),
        m_qualified_cpp_name(name),
        m_type_flags() {
    m_attributes.setFlag(IsNativeIdBased, TypeDatabase::instance()->useNativeIds());
    if(name==QLatin1String("QByteArrayView")){
        setQByteArrayView(true);
    }
    Include inc;
    inc.name = "QtCore/QVariant";
    inc.type = Include::IncludePath;

    addExtraInclude(inc);
}

ComplexTypeEntry *ComplexTypeEntry::copy() const {
    ComplexTypeEntry *centry = new ComplexTypeEntry(name(), type());
    centry->setInclude(include());
    centry->setPPCondition(ppCondition());
    centry->setExtraIncludes(extraIncludes());
    centry->setFunctionModifications(functionModifications());
    centry->setFieldModifications(fieldModifications());
    centry->setDefaultSuperclass(defaultSuperclass());
    centry->setCodeSnips(codeSnips());
    centry->setTargetLangPackage(javaPackage());
    centry->setTargetTypeSystem(targetTypeSystem());
    centry->m_attributes = m_attributes;
    return centry;
}

QString PrimitiveTypeEntry::javaObjectName() const {
    static QMap<QString, QString> table;
    if (table.isEmpty()) {
        table["boolean"] = "Boolean";
        table["byte"] = "Byte";
        table["char"] = "Character";
        table["short"] = "Short";
        table["int"] = "Integer";
        table["long"] = "Long";
        table["float"] = "Float";
        table["double"] = "Double";
        table["void"] = "Void";
    }
    Q_ASSERT(table.contains(targetLangName()));
    return table[targetLangName()];
}

QString EnumTypeEntry::jniName() const {
    if(forceInteger()){
        switch(m_size){
        case 8: return "jbyte";
        case 16: return "jshort";
        case 64: return "jlong";
        default: break;
        }
        return "jint";
    }else{
        return "jobject";
    }
}

QString FlagsTypeEntry::jniName() const {
    return "jobject";
}

void EnumTypeEntry::addEnumValueRedirection(const QString &rejected, const QString &usedValue) {
    m_enum_redirections << EnumValueRedirection(rejected, usedValue);
}

QString EnumTypeEntry::enumValueRedirection(const QString &value) const {
    for (int i = 0; i < m_enum_redirections.size(); ++i)
        if (m_enum_redirections.at(i).rejected == value)
            return m_enum_redirections.at(i).used;
    return QString();
}

QString FlagsTypeEntry::qualifiedTargetLangName() const {
    return javaPackage().isEmpty() ? (
                m_enum->javaQualifier().isEmpty() ? targetLangName()
                                                  : m_enum->javaQualifier() + "." + targetLangName()
            ) : (
                m_enum->javaQualifier().isEmpty() ? javaPackage() + "." + targetLangName()
                                                  : javaPackage() + "." + m_enum->javaQualifier() + "." + targetLangName()
            );
}

QString FlagsTypeEntry::qualifiedTargetLangJNIName() const {
    QString pkg = javaPackage().replace('.', '/');
    return pkg.isEmpty() ? (
                m_enum->javaQualifier().isEmpty() ? targetLangName()
                                                  : m_enum->javaQualifier() + "$" + targetLangName()
            ) : (
                m_enum->javaQualifier().isEmpty() ? pkg + "/" + targetLangName()
                                                  : pkg + "/" + m_enum->javaQualifier() + "$" + targetLangName()
            );
}

QString FunctionalTypeEntry::javaPackage() const {
    if(!m_qualifier_type){
        m_qualifier_type = TypeDatabase::instance()->findType(m_qualifier);
        if (m_qualifier_type){
            if(m_qualifier_type->isQString() && TypeDatabase::instance()->qstringType())
                m_qualifier_type = TypeDatabase::instance()->qstringType();
            else if(m_qualifier_type->isQVariant() && TypeDatabase::instance()->qvariantType())
                m_qualifier_type = TypeDatabase::instance()->qvariantType();
            else if(m_qualifier_type->isQChar() && TypeDatabase::instance()->qcharType())
                m_qualifier_type = TypeDatabase::instance()->qcharType();
        }
    }
    if (m_qualifier_type){
        if(m_qualifier_type->isQVariant())
            return "io.qt.core";
        if(m_qualifier_type->designatedInterface()){
            return m_qualifier_type->designatedInterface()->javaPackage();
        }
        return m_qualifier_type->javaPackage().isEmpty() ? m_package_name : m_qualifier_type->javaPackage();
    }else
        return m_package_name;
}

QString FunctionalTypeEntry::javaQualifier() const {
    if(!m_qualifier_type){
        m_qualifier_type = TypeDatabase::instance()->findType(m_qualifier);
        if (m_qualifier_type){
            if(m_qualifier_type->isQString() && TypeDatabase::instance()->qstringType())
                m_qualifier_type = TypeDatabase::instance()->qstringType();
            else if(m_qualifier_type->isQVariant() && TypeDatabase::instance()->qvariantType())
                m_qualifier_type = TypeDatabase::instance()->qvariantType();
            else if(m_qualifier_type->isQChar() && TypeDatabase::instance()->qcharType())
                m_qualifier_type = TypeDatabase::instance()->qcharType();
        }
    }
    if (m_qualifier_type){
        if(m_qualifier_type->isQVariant())
            return "QVariant";
        if(m_qualifier_type->designatedInterface()){
            return m_qualifier_type->designatedInterface()->targetLangName();
        }
        return m_qualifier_type->targetLangName();
    }else
        return m_qualifier;
}

QString EnumTypeEntry::javaPackage() const {
    if(!m_qualifier_type){
        if(m_javaScope.isEmpty()){
            m_qualifier_type = TypeDatabase::instance()->findType(m_qualifier);
        }else{
            m_qualifier_type = TypeDatabase::instance()->findType(m_javaScope);
        }
        if(m_qualifier_type && m_qualifier_type->isQVariant() && TypeDatabase::instance()->qvariantType())
            m_qualifier_type = TypeDatabase::instance()->qvariantType();
        else if(m_qualifier_type && m_qualifier_type->isQString() && TypeDatabase::instance()->qstringType())
            m_qualifier_type = TypeDatabase::instance()->qstringType();
        else if(m_qualifier_type && m_qualifier_type->isQChar() && TypeDatabase::instance()->qcharType())
            m_qualifier_type = TypeDatabase::instance()->qcharType();
    }
    if (m_qualifier_type){
        if(m_qualifier_type->isQVariant())
            return "io.qt.core";
        if(m_qualifier_type->designatedInterface()){
            return m_qualifier_type->designatedInterface()->javaPackage();
        }
        return m_qualifier_type->javaPackage().isEmpty() ? m_package_name : m_qualifier_type->javaPackage();
    }else
        return m_package_name;
}

QString EnumTypeEntry::javaQualifier() const {
    if(!m_qualifier_type){
        if(m_javaScope.isEmpty()){
            m_qualifier_type = TypeDatabase::instance()->findType(m_qualifier);
        }else{
            m_qualifier_type = TypeDatabase::instance()->findType(m_javaScope);
        }
        if (m_qualifier_type){
            if(m_qualifier_type->isQString() && TypeDatabase::instance()->qstringType())
                m_qualifier_type = TypeDatabase::instance()->qstringType();
            else if(m_qualifier_type->isQVariant() && TypeDatabase::instance()->qvariantType())
                m_qualifier_type = TypeDatabase::instance()->qvariantType();
            else if(m_qualifier_type->isQChar() && TypeDatabase::instance()->qcharType())
                m_qualifier_type = TypeDatabase::instance()->qcharType();
        }
    }
    if (m_qualifier_type){
        if(m_qualifier_type->isQVariant())
            return "QVariant";
        if(m_qualifier_type->designatedInterface()){
            return m_qualifier_type->designatedInterface()->targetLangName();
        }
        return m_qualifier_type->targetLangName();
    }else
        return m_qualifier;
}

ContainerTypeEntry::ContainerTypeEntry(const QString &name, Type type)
        : ComplexTypeEntry(name, ContainerType), m_type(type) {
    setCodeGeneration(GenerateForSubclass);
    disableNativeIdUsage();
}

ContainerTypeEntry::Type ContainerTypeEntry::type() const { return m_type; }

QString ContainerTypeEntry::javaPackage() const {
    if (m_type == PairContainer
            || m_type == StringListContainer
            || m_type == ListContainer
            || m_type == ByteArrayListContainer
            || m_type == LinkedListContainer
            || m_type == VectorContainer
            || m_type == StackContainer
            || m_type == QueueContainer
            || m_type == SetContainer
            || m_type == MapContainer
            || m_type == HashContainer
            || m_type == MultiMapContainer
            || m_type == MultiHashContainer
            || m_type == QBindableContainer
            || m_type == QPropertyBindingContainer)
        return "io.qt.core";
    if (m_type == QDBusReplyContainer)
        return "io.qt.dbus";
    if (m_type == QQmlListPropertyContainer)
        return "io.qt.qml";
    if (m_type == std_chrono
        || m_type == std_chrono_template)
        return "java.time";
    return "java.util";
}

QString ContainerTypeEntry::targetLangName() const {

    switch (m_type) {
        case StringListContainer: return "QStringList";
        case ByteArrayListContainer: return "QList";
        case ListContainer: return "QList";
        case std_vector: return "List";
        case QModelRoleDataSpanContainer: return "Map";
        case QPropertyBindingContainer: return "QPropertyBinding";
        case QBindableContainer: return "QBindable";
        case LinkedListContainer: return "QLinkedList";
        case VectorContainer:
            if(TypeDatabase::instance()->qtVersion() < QVersionNumber(6,0,0)){
                return "QVector";
            }else{
                return "QList";
            }
        case QArrayDataContainer: return "Collection";
        case QTypedArrayDataContainer: return "Collection";
        case StackContainer: return "QStack";
        case QueueContainer: return "QQueue";
        case SetContainer: return "QSet";
        case MapContainer: return "QMap";
        case QDBusReplyContainer: return "QDBusReply";
        case MultiMapContainer: return "QMultiMap";
        case HashContainer: return "QHash";
        case MultiHashContainer: return "QMultiHash";
            //     case MultiHashCollectio: return "MultiHash";
        case PairContainer: return "QPair";
        case std_optional: return "Optional";
        case std_atomic: return "Atomic";
        case std_chrono_template:
            if(qualifiedCppName()=="std::chrono::time_point")
                return "Instant";
        case std_chrono: return "Duration";
        case QQmlListPropertyContainer: return "QQmlListProperty"; // new for QtQml module
        default:
            qWarning("bad container type for %s: %d", qPrintable(ComplexTypeEntry::targetLangName()), m_type);
            break;
    }
    return ComplexTypeEntry::targetLangName();
}

QString ContainerTypeEntry::qualifiedCppName() const {
    if (m_type == StringListContainer)
        return "QStringList";
    if (m_type == ByteArrayListContainer)
        return "QByteArrayList";
    if (m_type == QModelRoleDataSpanContainer)
        return "QModelRoleDataSpan";
    if (m_type == QQmlListPropertyContainer)
        return "QQmlListProperty";   // new for QDeclarative module but not yet implemented
    return ComplexTypeEntry::qualifiedCppName();
}

QString IteratorTypeEntry::targetLangName() const {
//    return (m_containerType ? m_containerType->targetLangName()+"$" : "") + ComplexTypeEntry::targetLangName();
    return m_isConst ? "QSequentialConstIterator" : "QSequentialIterator";
}

QString IteratorTypeEntry::javaPackage() const {
//    return m_containerType ? m_containerType->javaPackage() : ComplexTypeEntry::javaPackage();
    return "io.qt.core";
}

const QString& IteratorTypeEntry::qualifiedCppContainerName() const {
    return m_qualifiedCppContainerName;
}

void IteratorTypeEntry::setIsConst(bool newIsConst)
{
    m_isConst = newIsConst;
}

QString IteratorTypeEntry::qualifiedCppName() const {
    if(!m_qualifiedCppContainerName.isEmpty()){
        return m_qualifiedCppContainerName+"::"+ComplexTypeEntry::qualifiedCppName().split("::").last();
    }else{
        return m_containerType ? m_containerType->qualifiedCppName() + "::" + ComplexTypeEntry::qualifiedCppName().split("::").last() : ComplexTypeEntry::qualifiedCppName();
    }
}

QString IteratorTypeEntry::iteratorName() const {
    return ComplexTypeEntry::qualifiedCppName();
}

IteratorTypeEntry* IteratorTypeEntry::clone(const ComplexTypeEntry* containerType, const QString& qualifiedCppContainerName) const {
    IteratorTypeEntry* entry = new IteratorTypeEntry(ComplexTypeEntry::qualifiedCppName(), qualifiedCppContainerName, containerType, m_isPointer);
    if(containerType->typeFlags()==ComplexTypeEntry::ContainerType){
        entry->setCodeGeneration(GenerateForSubclass);
    }else{
        entry->setCodeGeneration(containerType->codeGeneration());
    }
    entry->setCodeGeneration(GenerateNothing);
//    entry->setInclude(include());
//    entry->setTypeFlags(typeFlags());
//    entry->setPreferredConversion(preferredConversion());
    entry->setDefaultSuperclass(defaultSuperclass());
    entry->setImplements(implements());
//    entry->setPolymorphicIdValue(polymorphicIdValue());
//    entry->setExpensePolicy(expensePolicy());
    entry->setExtraIncludes(extraIncludes());
//    entry->setIsPolymorphicBase(isPolymorphicBase());
//    entry->setLookupName(lookupName());
    entry->setTargetLangName(ComplexTypeEntry::targetLangName());
    return entry;
}

bool ComplexTypeEntry::hasFunctionCodeInjections(const QString &methodSignature, TS::Language language, const QSet<CodeSnip::Position>& positions) const{
    for (const FunctionModification& mod : m_function_mods) {
        if (mod.signature == methodSignature) {
            if (mod.snips.count() <= 0)
                continue ;
            for(const CodeSnip& snip : mod.snips) {
                if (!positions.contains(snip.position))
                    continue;

                if (!(snip.language & language))
                    continue;

                if(!snip.code().isEmpty())
                    return true;
            }
        }
    }
    return false;
}

bool TypeSystemTypeEntry::hasFunctionCodeInjections(const QString &methodSignature, TS::Language language, const QSet<CodeSnip::Position>& positions) const{
    for (const FunctionModification& mod : m_function_mods) {
        if (mod.signature == methodSignature) {
            if (mod.snips.count() <= 0)
                continue ;
            for(const CodeSnip& snip : mod.snips) {
                if (!positions.contains(snip.position))
                    continue;

                if (!(snip.language & language))
                    continue;

                if(!snip.code().isEmpty())
                    return true;
            }
        }
    }
    return false;
}

const QString &TypeSystemTypeEntry::targetName() const
{
    return m_targetName;
}

void TypeSystemTypeEntry::setTargetName(const QString &newTargetName)
{
    m_targetName = newTargetName;
}

FunctionModificationList ComplexTypeEntry::functionModifications(const QString &signature) const {
    FunctionModificationList lst;
    for (const FunctionModification& mod : m_function_mods) {
        if (mod.signature == signature) {
            lst << mod;
        }
    }
    return lst;
}

TypeSystemTypeEntry::TypeSystemTypeEntry(const QString &name)
        : TypeEntry(name, TypeSystemType),
          snips(),
          m_include(),
          m_extra_includes(),
          m_qtLibrary(),
          m_module(),
          m_requiredTypeSystems(),
          m_noExports(false) {
}

TypeSystemTypeEntry::TypeSystemTypeEntry(const QString &name, const QString &lib, const QString &module)
        : TypeEntry(name, TypeSystemType),
          snips(),
          m_include(),
          m_extra_includes(),
          m_qtLibrary(lib),
          m_module(module),
          m_requiredTypeSystems(),
          m_noExports(false) {
}

FunctionModificationList TypeSystemTypeEntry::functionModifications(const QString &signature) const {
    FunctionModificationList lst;
    for (const FunctionModification& mod : m_function_mods) {
        if (mod.signature == signature) {
            lst << mod;
        }
    }
    return lst;
}

FieldModification ComplexTypeEntry::fieldModification(const QString &name) const {
    for (int i = 0; i < m_field_mods.size(); ++i)
        if (m_field_mods.at(i).name == name)
            return m_field_mods.at(i);
    if(isInterface()){
        return static_cast<const InterfaceTypeEntry*>(this)->origin()->fieldModification(name);
    }
    FieldModification mod;
    mod.name = name;
    mod.modifiers = FieldModification::Readable | FieldModification::Writable;
    return mod;
}

QString ComplexTypeEntry::qualifiedCppName() const {
    if(designatedInterface()){
        return designatedInterface()->qualifiedCppName();
    }
    return m_qualified_cpp_name;
}

bool ComplexTypeEntry::getNoImplicitConstructors() const
{
    return noImplicitConstructors;
}

void ComplexTypeEntry::setNoImplicitConstructors(bool newNoImplicitConstructors)
{
    noImplicitConstructors = newNoImplicitConstructors;
}

bool ImplementorTypeEntry::isValueOwner() const
{
    return m_attributes.testFlag(IsValueOwner);
}

void ImplementorTypeEntry::setIsValueOwner(bool is_value_owner)
{
    m_attributes.setFlag(IsValueOwner, is_value_owner);
}

}
