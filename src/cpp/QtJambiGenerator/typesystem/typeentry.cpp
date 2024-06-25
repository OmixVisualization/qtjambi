/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
    centry->m_forceFinal = m_forceFinal;
    centry->m_attributes = m_attributes;
    return centry;
}

bool ComplexTypeEntry::isComplex() const {
    return true;
}

const IncludeList& ComplexTypeEntry::extraIncludes() const {
    return m_extra_includes;
}

void ComplexTypeEntry::setExtraIncludes(const IncludeList &includes) {
    m_extra_includes = includes;
}

void ComplexTypeEntry::addExtraInclude(const Include &include) {
    if (!m_includes_used.value(include.name, false)) {
        m_extra_includes << include;
        m_includes_used[include.name] = true;
    }
}

void ComplexTypeEntry::setLookupName(const QString &name) {
    m_lookup_name = name;
}

QString ComplexTypeEntry::lookupName() const {
    return m_lookup_name.isEmpty() ? targetLangName() : m_lookup_name;
}

QString ComplexTypeEntry::jniName() const {
    return strings_jobject;
}


const Include& ComplexTypeEntry::include() const {
    return m_include;
}
void ComplexTypeEntry::setInclude(const Include &inc) {
    m_include = inc;
}

void ComplexTypeEntry::setForceAbstract(){
    m_type_flags.setFlag(ForceAbstract);
}

bool ComplexTypeEntry::isForceAbstract() const {
    return m_type_flags.testFlag(ForceAbstract);
}

void ComplexTypeEntry::setThreadAffine(){
    m_type_flags.setFlag(ThreadAffine);
}

bool ComplexTypeEntry::isThreadAffine() const {
    return m_type_flags.testFlag(ThreadAffine);
}

void ComplexTypeEntry::setDeprecated(){
    m_type_flags.setFlag(Deprecated);
}

bool ComplexTypeEntry::isDeprecated() const {
    return m_type_flags.testFlag(Deprecated);
}

void ComplexTypeEntry::setForceFriendly(){
    m_type_flags.setFlag(ForceFriendly);
}

bool ComplexTypeEntry::isForceFriendly() const {
    return m_type_flags.testFlag(ForceFriendly);
}

void ComplexTypeEntry::setHasNonPublicFields(){
    m_type_flags.setFlag(HasFields);
    m_type_flags.setFlag(HasNonPublicFields);
}

bool ComplexTypeEntry::hasNonPublicFields() const {
    return m_type_flags.testFlag(HasNonPublicFields);
}

void ComplexTypeEntry::setHasFields(){
    m_type_flags.setFlag(HasFields);
}

bool ComplexTypeEntry::hasFields() const {
    return m_type_flags.testFlag(HasFields);
}

ComplexTypeEntry::TypeFlags ComplexTypeEntry::typeFlags() const {
    return m_type_flags;
}

const CodeSnipList& ComplexTypeEntry::codeSnips() const {
    return m_code_snips;
}
void ComplexTypeEntry::setCodeSnips(const CodeSnipList &codeSnips) {
    m_code_snips = codeSnips;
}
void ComplexTypeEntry::addCodeSnips(const CodeSnipList &codeSnips) {
    m_code_snips << codeSnips;
}
void ComplexTypeEntry::addCodeSnip(const CodeSnip &codeSnip) {
    m_code_snips << codeSnip;
}

const FunctionModificationList& ComplexTypeEntry::functionModifications() const {
    return m_function_mods;
}
void ComplexTypeEntry::setFunctionModifications(const FunctionModificationList &functionModifications) {
    m_function_mods = functionModifications;
}
void ComplexTypeEntry::addFunctionModifications(const FunctionModificationList &functionModifications) {
    m_function_mods << functionModifications;
}
void ComplexTypeEntry::addFunctionModification(const FunctionModification &functionModification) {
    m_function_mods << functionModification;
}

void ComplexTypeEntry::setFieldModifications(const FieldModificationList &mods) {
    m_field_mods = mods;
}
void ComplexTypeEntry::addFieldModifications(const FieldModificationList &mods) {
    m_field_mods << mods;
}
void ComplexTypeEntry::addFieldModification(const FieldModification &mod) {
    m_field_mods << mod;
}
const FieldModificationList& ComplexTypeEntry::fieldModifications() const {
    return m_field_mods;
}

QString ComplexTypeEntry::javaPackage() const {
    return m_package;
}
void ComplexTypeEntry::setTargetLangPackage(const QString &package) {
    m_package = package;
}

QString ComplexTypeEntry::targetTypeSystem() const {
    return m_target_typesystem;
}

void ComplexTypeEntry::setTargetTypeSystem(const QString &qt_jambi_library) {
    m_target_typesystem = qt_jambi_library;
}

bool ComplexTypeEntry::isQObject() const {
    return m_attributes.testFlag(IsQObject);
}
void ComplexTypeEntry::setQObject(bool qobject) {
    m_attributes.setFlag(IsQObject, qobject);
}
bool ComplexTypeEntry::isQModelIndex() const {
    return m_attributes.testFlag(IsQModelIndex);
}
void ComplexTypeEntry::setQModelIndex(bool b) {
    m_attributes.setFlag(IsQModelIndex, b);
}
bool ComplexTypeEntry::isQAbstractItemModel() const {
    return m_attributes.testFlag(IsQAbstractItemModel);
}
void ComplexTypeEntry::setQAbstractItemModel(bool b) {
    m_attributes.setFlag(IsQAbstractItemModel, b);
}
bool ComplexTypeEntry::isQEvent() const {
    return m_attributes.testFlag(IsQEvent);
}
void ComplexTypeEntry::setQEvent(bool b) {
    m_attributes.setFlag(IsQEvent, b);
}
bool ComplexTypeEntry::isQByteArrayView() const {
    return m_attributes.testFlag(IsQByteArrayView);
}
void ComplexTypeEntry::setQByteArrayView(bool b) {
    m_attributes.setFlag(IsQByteArrayView, b);
}
bool ComplexTypeEntry::isGLsync() const {
    return m_attributes.testFlag(IsGLsync);
}

bool ComplexTypeEntry::isQWidget() const {
    return m_attributes.testFlag(IsQWidget);
}
void ComplexTypeEntry::setQWidget(bool qWidget) {
    m_attributes.setFlag(IsQWidget, qWidget);
}

bool ComplexTypeEntry::isQWindow() const {
    return m_attributes.testFlag(IsQWindow);
}
void ComplexTypeEntry::setQWindow(bool qw) {
    m_attributes.setFlag(IsQWindow, qw);
}

bool ComplexTypeEntry::isQAction() const {
    return m_attributes.testFlag(IsQAction);
}
void ComplexTypeEntry::setQAction(bool qw) {
    m_attributes.setFlag(IsQAction, qw);
}

bool ComplexTypeEntry::isQMediaControl() const {
    return m_attributes.testFlag(IsQMediaControl);
}
void ComplexTypeEntry::setQMediaControl(bool qw) {
    m_attributes.setFlag(IsQMediaControl, qw);
}

bool ComplexTypeEntry::isQCoreApplication() const {
    return m_attributes.testFlag(IsQCoreApplication);
}
void ComplexTypeEntry::setQCoreApplication(bool qw) {
    m_attributes.setFlag(IsQCoreApplication, qw);
}

const QString& ComplexTypeEntry::defaultSuperclass() const {
    return m_default_superclass;
}
void ComplexTypeEntry::setDefaultSuperclass(const QString &sc) {
    m_default_superclass = sc;
}

void ComplexTypeEntry::setImplements(const QString &implements) {
    m_implements = implements;
}
const QString& ComplexTypeEntry::implements() const {
    return m_implements;
}

void ComplexTypeEntry::setIsPolymorphicBase(bool on) {
    m_attributes.setFlag(IsPolymorphicBase, on);
}
bool ComplexTypeEntry::isPolymorphicBase() const {
    return m_attributes.testFlag(IsPolymorphicBase);
}

void ComplexTypeEntry::setPolymorphicIdValue(const QString &value) {
    m_polymorphic_id_value = value;
}
const QString& ComplexTypeEntry::polymorphicIdValue() const {
    return m_polymorphic_id_value;
}

void ComplexTypeEntry::addInterfacePolymorphicIdValue(const QString &iface, const QString &value) {
    m_interface_polymorphic_id_values[iface] = value;
}
const QMap<QString,QString>& ComplexTypeEntry::interfacePolymorphicIdValues() const {
    return m_interface_polymorphic_id_values;
}

void ComplexTypeEntry::setExpensePolicy(const ExpensePolicy &policy) {
    m_expense_policy = policy;
}
const ExpensePolicy &ComplexTypeEntry::expensePolicy() const {
    return m_expense_policy;
}

bool ComplexTypeEntry::forceFinal() const {
    return m_forceFinal;
}
void ComplexTypeEntry::setForceFinal(bool isFinal) {
    m_forceFinal = isFinal;
}

QString ComplexTypeEntry::targetLangName() const {
    if(m_java_name.isEmpty()){
        return TypeEntry::targetLangName();
    }else{
        return m_java_name;
    }
}
void ComplexTypeEntry::setTargetLangName(const QString &name) {
    m_java_name = name;
}

bool ComplexTypeEntry::isGenericClass() const {
    return m_attributes.testFlag(IsGenericClass);
}
void ComplexTypeEntry::setGenericClass(bool isGeneric) {
    m_attributes.setFlag(IsGenericClass, isGeneric);
}

bool ComplexTypeEntry::isTemplate() const {
    return m_attributes.testFlag(IsTemplate);
}
void ComplexTypeEntry::setTemplate(bool isTemplate) {
    m_attributes.setFlag(IsTemplate, isTemplate);
}

bool ComplexTypeEntry::isNativeInterface() const {
    return m_attributes.testFlag(IsNativeInterface);
}
void ComplexTypeEntry::setNativeInterface(bool isNativeInterface) {
    m_attributes.setFlag(IsNativeInterface, isNativeInterface);
}

bool ComplexTypeEntry::inhibitMetaobject() const {
    return m_attributes.testFlag(InhibitMetaobject);
}
void ComplexTypeEntry::setInhibitMetaobject(bool inhibitMetaobject) {
    m_attributes.setFlag(InhibitMetaobject, inhibitMetaobject);
}

const QString& ComplexTypeEntry::threadAffinity() const {
    return m_threadAffinity;
}
void ComplexTypeEntry::setThreadAffinity(const QString &code) {
    m_threadAffinity = code;
}

const QString& ComplexTypeEntry::ppCondition() const {
    return m_pp_condition;
}
void ComplexTypeEntry::setPPCondition(const QString &pp_condition) {
    m_pp_condition = pp_condition;
}

void ComplexTypeEntry::addDelegatedBaseClass(QString baseClass, QString delegate){
    m_delegatedBaseClasses.insert(baseClass, delegate);
}

const QMap<QString,QString>& ComplexTypeEntry::delegatedBaseClasses() const{
    return m_delegatedBaseClasses;
}

bool ComplexTypeEntry::isNativeIdBased() const {
    return m_attributes.testFlag(IsNativeIdBased);
}

void ComplexTypeEntry::disableNativeIdUsage() {
    m_attributes.setFlag(IsNativeIdBased, false);
}

void ComplexTypeEntry::addInstantiation(const QStringList& instantiation, const ComplexTypeEntry* typeEntry){
    m_instantiations[instantiation] = typeEntry;
}

const QMap<QStringList,const ComplexTypeEntry*>& ComplexTypeEntry::instantiations() const {
    return m_instantiations;
}
void ComplexTypeEntry::setExtendType(const QString& extendType){ m_extendType = extendType; }
const QString& ComplexTypeEntry::extendType() const { return m_extendType; }

void ComplexTypeEntry::setCustomConstructor(const CustomFunction &func, ConstructorType type) {
    m_customConstructors[type] = func;
}
CustomFunction ComplexTypeEntry::customConstructor(ConstructorType type) const {
    return m_customConstructors[type];
}

void ComplexTypeEntry::setCustomDestructor(const CustomFunction &func) {
    m_customDestructor = func;
}
const CustomFunction& ComplexTypeEntry::customDestructor() const {
    return m_customDestructor;
}

bool ComplexTypeEntry::skipMetaTypeRegistration() const {
    return m_attributes.testFlag(SkipMetaTypeRegistration);
}

void ComplexTypeEntry::setSkipMetaTypeRegistration(bool skipMetaTypeRegistration){
    m_attributes.setFlag(SkipMetaTypeRegistration, skipMetaTypeRegistration);
}

void ComplexTypeEntry::setHasPrivateConstructors(){
    m_functionAttributes.setFlag(HasAnyPrivateConstructor);
}

void ComplexTypeEntry::setHasNonPrivateConstructors(){
    m_functionAttributes.setFlag(HasAnyNonPrivateConstructor);
}

bool ComplexTypeEntry::hasPrivateConstructors() const {
    return m_functionAttributes.testFlag(HasAnyPrivateConstructor);
}

bool ComplexTypeEntry::hasNonPrivateConstructors() const {
    return m_functionAttributes.testFlag(HasAnyNonPrivateConstructor);
}

bool ComplexTypeEntry::hasJustPrivateConstructors() const {
    return hasPrivateConstructors() && !hasNonPrivateConstructors();
}

bool ComplexTypeEntry::hasPublicDefaultConstructor() const {
    return m_functionAttributes.testFlag(HasPublicDefaultConstructor);
}

bool ComplexTypeEntry::hasProtectedDefaultConstructor() const {
    return m_functionAttributes.testFlag(HasProtectedDefaultConstructor);
}

bool ComplexTypeEntry::hasPrivateDefaultConstructor() const {
    return m_functionAttributes.testFlag(HasPrivateDefaultConstructor);
}

void ComplexTypeEntry::setHasPublicDefaultConstructor(){
    m_functionAttributes.setFlag(HasProtectedDefaultConstructor, false);
    m_functionAttributes.setFlag(HasPrivateDefaultConstructor, false);
    m_functionAttributes.setFlag(HasPublicDefaultConstructor);
}

void ComplexTypeEntry::setHasProtectedDefaultConstructor(){
    m_functionAttributes.setFlag(HasPublicDefaultConstructor, false);
    m_functionAttributes.setFlag(HasPrivateDefaultConstructor, false);
    m_functionAttributes.setFlag(HasProtectedDefaultConstructor);
}

void ComplexTypeEntry::setHasPrivateDefaultConstructor(){
    m_functionAttributes.setFlag(HasProtectedDefaultConstructor, false);
    m_functionAttributes.setFlag(HasPublicDefaultConstructor, false);
    m_functionAttributes.setFlag(HasPrivateDefaultConstructor);
}

bool ComplexTypeEntry::hasPublicCopyConstructor() const {
    return m_functionAttributes.testFlag(HasPublicCopyConstructor);
}

bool ComplexTypeEntry::hasProtectedCopyConstructor() const {
    return m_functionAttributes.testFlag(HasProtectedCopyConstructor);
}

bool ComplexTypeEntry::hasPrivateCopyConstructor() const {
    return m_functionAttributes.testFlag(HasPrivateCopyConstructor);
}

void ComplexTypeEntry::setHasPublicCopyConstructor(){
    m_functionAttributes.setFlag(HasProtectedCopyConstructor, false);
    m_functionAttributes.setFlag(HasPrivateCopyConstructor, false);
    m_functionAttributes.setFlag(HasPublicCopyConstructor);
}

void ComplexTypeEntry::setHasProtectedCopyConstructor(){
    m_functionAttributes.setFlag(HasPublicCopyConstructor, false);
    m_functionAttributes.setFlag(HasPrivateCopyConstructor, false);
    m_functionAttributes.setFlag(HasProtectedCopyConstructor);
}

void ComplexTypeEntry::setHasPrivateCopyConstructor(){
    m_functionAttributes.setFlag(HasProtectedCopyConstructor, false);
    m_functionAttributes.setFlag(HasPublicCopyConstructor, false);
    m_functionAttributes.setFlag(HasPrivateCopyConstructor);
}

bool ComplexTypeEntry::hasPublicMoveConstructor() const {
    return m_functionAttributes.testFlag(HasPublicMoveConstructor);
}

bool ComplexTypeEntry::hasProtectedMoveConstructor() const {
    return m_functionAttributes.testFlag(HasProtectedMoveConstructor);
}

bool ComplexTypeEntry::hasPrivateMoveConstructor() const {
    return m_functionAttributes.testFlag(HasPrivateMoveConstructor);
}

void ComplexTypeEntry::setHasPublicMoveConstructor(){
    m_functionAttributes.setFlag(HasProtectedMoveConstructor, false);
    m_functionAttributes.setFlag(HasPrivateMoveConstructor, false);
    m_functionAttributes.setFlag(HasPublicMoveConstructor);
}

void ComplexTypeEntry::setHasProtectedMoveConstructor(){
    m_functionAttributes.setFlag(HasPublicMoveConstructor, false);
    m_functionAttributes.setFlag(HasPrivateMoveConstructor, false);
    m_functionAttributes.setFlag(HasProtectedMoveConstructor);
}

void ComplexTypeEntry::setHasPrivateMoveConstructor(){
    m_functionAttributes.setFlag(HasProtectedMoveConstructor, false);
    m_functionAttributes.setFlag(HasPublicMoveConstructor, false);
    m_functionAttributes.setFlag(HasPrivateMoveConstructor);
}

bool ComplexTypeEntry::hasPublicMoveAssignment() const {
    return m_functionAttributes.testFlag(HasPublicMoveAssignment);
}

bool ComplexTypeEntry::hasProtectedMoveAssignment() const {
    return m_functionAttributes.testFlag(HasProtectedMoveAssignment);
}

bool ComplexTypeEntry::hasPrivateMoveAssignment() const {
    return m_functionAttributes.testFlag(HasPrivateMoveAssignment);
}

void ComplexTypeEntry::setHasPublicMoveAssignment(){
    m_functionAttributes.setFlag(HasPrivateMoveAssignment, false);
    m_functionAttributes.setFlag(HasProtectedMoveAssignment, false);
    m_functionAttributes.setFlag(HasPublicMoveAssignment);
}

void ComplexTypeEntry::setHasProtectedMoveAssignment(){
    m_functionAttributes.setFlag(HasPrivateMoveAssignment, false);
    m_functionAttributes.setFlag(HasPublicMoveAssignment, false);
    m_functionAttributes.setFlag(HasProtectedMoveAssignment);
}

void ComplexTypeEntry::setHasPrivateMoveAssignment(){
    m_functionAttributes.setFlag(HasPublicMoveAssignment, false);
    m_functionAttributes.setFlag(HasProtectedMoveAssignment, false);
    m_functionAttributes.setFlag(HasPrivateMoveAssignment);
}

bool ComplexTypeEntry::hasPublicDefaultAssignment() const {
    return m_functionAttributes.testFlag(HasPublicDefaultAssignment);
}

bool ComplexTypeEntry::hasProtectedDefaultAssignment() const {
    return m_functionAttributes.testFlag(HasProtectedDefaultAssignment);
}

bool ComplexTypeEntry::hasPrivateDefaultAssignment() const {
    return m_functionAttributes.testFlag(HasPrivateDefaultAssignment);
}

void ComplexTypeEntry::setHasPublicDefaultAssignment(){
    m_functionAttributes.setFlag(HasPrivateDefaultAssignment, false);
    m_functionAttributes.setFlag(HasProtectedDefaultAssignment, false);
    m_functionAttributes.setFlag(HasPublicDefaultAssignment);
}

void ComplexTypeEntry::setHasProtectedDefaultAssignment(){
    m_functionAttributes.setFlag(HasPrivateDefaultAssignment, false);
    m_functionAttributes.setFlag(HasPublicDefaultAssignment, false);
    m_functionAttributes.setFlag(HasProtectedDefaultAssignment);
}

void ComplexTypeEntry::setHasPrivateDefaultAssignment(){
    m_functionAttributes.setFlag(HasPublicDefaultAssignment, false);
    m_functionAttributes.setFlag(HasProtectedDefaultAssignment, false);
    m_functionAttributes.setFlag(HasPrivateDefaultAssignment);
}

void ComplexTypeEntry::setDestructorPrivate(){
    m_functionAttributes.setFlag(HasProtectedDestructor, false);
    m_functionAttributes.setFlag(HasPrivateDestructor, true);
}

bool ComplexTypeEntry::isDestructorPrivate() const {
    return m_functionAttributes.testFlag(HasPrivateDestructor);
}

void ComplexTypeEntry::setDestructorProtected(){
    m_functionAttributes.setFlag(HasPrivateDestructor, false);
    m_functionAttributes.setFlag(HasProtectedDestructor, true);
}

bool ComplexTypeEntry::isDestructorProtected() const {
    return m_functionAttributes.testFlag(HasProtectedDestructor);
}

void ComplexTypeEntry::setDestructorVirtual(){
    m_functionAttributes.setFlag(HasVirtualDestructor, true);
}

bool ComplexTypeEntry::isDestructorVirtual() const {
    return m_functionAttributes.testFlag(HasVirtualDestructor);
}

bool ComplexTypeEntry::isDestructorPublic() const {
    return !m_functionAttributes.testFlag(HasPrivateDestructor) && !m_functionAttributes.testFlag(HasProtectedDestructor);
}

void ComplexTypeEntry::setHasEquals(){
    m_functionAttributes.setFlag(HasEquals, true);
}

bool ComplexTypeEntry::hasEquals() const {
    return m_functionAttributes.testFlag(HasEquals);
}

void ComplexTypeEntry::setHasHash(){
    m_functionAttributes.setFlag(HasHash, true);
}

bool ComplexTypeEntry::hasHash() const {
    return m_functionAttributes.testFlag(HasHash);
}

void ComplexTypeEntry::setHasVirtualFunctions(){
    m_functionAttributes.setFlag(HasVirtuals, true);
}

bool ComplexTypeEntry::hasVirtualFunctions() const {
    return m_functionAttributes.testFlag(HasVirtuals);
}

void ComplexTypeEntry::setHasFinalFunctions(){
    m_functionAttributes.setFlag(HasFinals, true);
}

bool ComplexTypeEntry::hasFinalFunctions() const {
    return m_functionAttributes.testFlag(HasFinals);
}

bool ComplexTypeEntry::hasPureVirtualFunctions(bool nonPrivate) const {
    return m_functionAttributes.testFlag(nonPrivate ? HasPureVirtuals : HasPrivatePureVirtuals);
}

void ComplexTypeEntry::setHasPureVirtualFunctions(bool nonPrivate){
    m_functionAttributes.setFlag(nonPrivate ? HasPureVirtuals : HasPrivatePureVirtuals);
}

const QStringList& ComplexTypeEntry::implicitCasts() const {
    return m_implicitCasts;
}

void ComplexTypeEntry::addImplicitCast(const QString& cast){
    m_implicitCasts.append(cast);
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
        case std_array: return "[";
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
            Q_FALLTHROUGH();
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
        if (mod.signature == methodSignature || (!mod.originalSignature.isEmpty() && mod.originalSignature == methodSignature)) {
            for(const CodeSnip& snip : mod.snips) {
                if (positions.contains(snip.position)
                    && (snip.language & language)
                    && !snip.code().isEmpty())
                    return true;
            }
        }
    }
    return false;
}

bool TypeSystemTypeEntry::hasFunctionCodeInjections(const QString &methodSignature, TS::Language language, const QSet<CodeSnip::Position>& positions) const{
    for (const FunctionModification& mod : m_function_mods) {
        if (mod.signature == methodSignature || (!mod.originalSignature.isEmpty() && mod.originalSignature == methodSignature)) {
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
        if (mod.signature == signature || (!mod.originalSignature.isEmpty() && mod.originalSignature == signature)) {
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
        if (mod.signature == signature || (!mod.originalSignature.isEmpty() && mod.originalSignature == signature)) {
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

bool ComplexTypeEntry::getNotAssignable() const
{
    return notAssignable;
}

void ComplexTypeEntry::setNotAssignable(bool newNotAssignable)
{
    notAssignable = newNotAssignable;
}

bool ComplexTypeEntry::getNotMoveAssignable() const
{
    return notMoveAssignable;
}

void ComplexTypeEntry::setNotMoveAssignable(bool newNotMoveAssignable)
{
    notMoveAssignable = newNotMoveAssignable;
}

bool ComplexTypeEntry::getNotCloneable() const
{
    return notCloneable;
}

void ComplexTypeEntry::setNotCloneable(bool newNotCloneable)
{
    notCloneable = newNotCloneable;
}

bool ComplexTypeEntry::getPushUpStatics() const
{
    return pushUpStatics;
}

void ComplexTypeEntry::setPushUpStatics(bool newPushUpStatics)
{
    pushUpStatics = newPushUpStatics;
}

bool ComplexTypeEntry::getNoInstance() const
{
    return noInstance;
}

void ComplexTypeEntry::setNoInstance(bool newNoInstance)
{
    noInstance = newNoInstance;
}

bool ImplementorTypeEntry::isValueOwner() const
{
    return m_attributes.testFlag(IsValueOwner);
}

void ImplementorTypeEntry::setIsValueOwner(bool is_value_owner)
{
    m_attributes.setFlag(IsValueOwner, is_value_owner);
}

bool AliasTypeEntry::getAsNativePointer() const
{
    return asNativePointer;
}

void AliasTypeEntry::setAsNativePointer(bool newAsNativePointer)
{
    asNativePointer = newAsNativePointer;
}

bool AliasTypeEntry::getHasIndirections() const
{
    return hasIndirections;
}

void AliasTypeEntry::setHasIndirections(bool newHasIndirections)
{
    hasIndirections = newHasIndirections;
}

}
