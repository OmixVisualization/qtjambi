/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

#include "metalang.h"
#include "metabuilder.h"
#include "reporthandler.h"
#include "typesystem/typedatabase.h"

/*******************************************************************************
 * AbstractMetaType
 */
MetaType *MetaType::copy() const {
    MetaType *cpy = new MetaType;
    cpy->setTypeEntry(typeEntry());
    cpy->setTypeUsagePattern(typeUsagePattern());
    cpy->setConstant(isConstant());
    cpy->setReferenceType(getReferenceType());
    cpy->setIndirections(indirections());
    cpy->setInstantiations(instantiations());
    cpy->setArrayElementCount(arrayElementCount());
    cpy->setOriginalTypeDescription(originalTypeDescription());
    cpy->setOriginalTemplateType(originalTemplateType() ? originalTemplateType()->copy() : nullptr);
    cpy->setForceBoxedPrimitives(forceBoxedPrimitives());
    cpy->setIteratorInstantiations(iteratorInstantiations());

    cpy->setArrayElementType(arrayElementType() ? arrayElementType()->copy() : nullptr);

    return cpy;
}

QString MetaType::cppSignature() const {
    QString s;

    if (isConstant())
        s += "const ";

    s += typeEntry()->qualifiedCppName();

    if (hasInstantiationInCpp()) {
        const QList<const MetaType *>& types = instantiations();
        s += "<";
        for (int i = 0; i < types.count(); ++i) {
            if (i > 0)
                s += ", ";
            s += types.at(i)->cppSignature();
        }
        s += ">";
    }

    if (actualIndirections()) {
        s += ' ';
        for(int i=0; i<indirections().size(); i++){
            if(indirections()[i]){
                s += "*const";
            }else{
                s += "*";
            }
        }
    }
    if (getReferenceType()==MetaType::Reference)
        s += '&';
    if (getReferenceType()==MetaType::RReference)
        s += "&&";
    return s;
}

/*******************************************************************************
 * AbstractMetaTemplateParameter
 */
MetaTemplateParameter *MetaTemplateParameter::copy() const {
    MetaTemplateParameter *cpy = new MetaTemplateParameter;
    cpy->setName(MetaTemplateParameter::name());
    cpy->setInstantiation(instantiation());
    cpy->setImplicit(isImplicit());
    cpy->setType(type()->copy());
    cpy->setDefaultType(defaultType());

    return cpy;
}

/*******************************************************************************
 * AbstractMetaArgument
 */
MetaArgument *MetaArgument::copy() const {
    MetaArgument *cpy = new MetaArgument;
    cpy->setName(MetaVariable::name());
    cpy->setModifiedName(m_modifiedName);
    cpy->setDefaultValueExpression(defaultValueExpression());
    cpy->setOriginalDefaultValueExpression(originalDefaultValueExpression());
    cpy->setType(type()->copy());
    cpy->setArgumentIndex(argumentIndex());

    return cpy;
}

QString MetaArgument::argumentName() const {
    QString n = MetaVariable::name();
    if (n.isEmpty()) {
        if(!m_modifiedName.isEmpty()){
            n = m_modifiedName;
        }else{
            n = QString("arg__%2").arg(m_argument_index + 1);
        }
    }
    if(n=="interface"
            || n=="extends"
            || n=="strictfp"
            || n=="assert"
            || n=="abstract"
            || n=="boolean"
            || n=="byte"
            || n=="final"
            || n=="finally"
            || n=="implements"
            || n=="instanceof"
            || n=="import"
            || n=="native"
            || n=="package"
            || n=="super"
            || n=="synchronized"
            || n=="throws"
            || n=="throw"
            || n=="enum"
            || n=="const"
            || n=="null"){
        return "arg__"+n;
    }else{
        return n;
    }
}

QString MetaArgument::modifiedArgumentName() const {
    if(!m_modifiedName.isEmpty()){
        return m_modifiedName;
    }
    return argumentName();
}


QString MetaArgument::indexedName() const {
    QString n = MetaVariable::name();
    if (n.isEmpty())
        return QString("arg__%2").arg(m_argument_index + 1);
    return QString("%1%2").arg(n).arg(m_argument_index);
}

QString MetaArgument::name() const {
    Q_ASSERT_X(0, "AbstractMetaArgument::name()", "use argumentName() or indexedName() instead");
    return QString();
}

QString MetaClass::fullName() const {
    if(package().isEmpty()) return name();
    return package() + "." + name();
}



/*******************************************************************************
 * AbstractMetaFunction
 */
MetaFunction::~MetaFunction() {
    qDeleteAll(m_arguments);
    delete m_type;
}

/*******************************************************************************
 * Indicates that this function has a modification that removes it
 */
bool MetaFunction::isModifiedRemoved(int types) const {
    FunctionModificationList mods = modifications(implementingClass());
    for(const FunctionModification& mod : mods) {
        if (!mod.isRemoveModifier())
            continue;

        if ((mod.removal & types) == types)
            return true;
    }

    return false;
}

bool MetaFunction::isProxyCall() const {
    FunctionModificationList mods = modifications(implementingClass());
    for(const FunctionModification& mod : mods) {
        if (!mod.proxyCall.isEmpty())
            return true;
    }
    return false;
}

bool MetaFunction::needsCallThrough() const {
    if (ownerClass()->isInterface())
        return false;
    bool hasReferenceCount = false;
    for(const ReferenceCount& rc : referenceCounts(implementingClass())){
        if(rc.action!=ReferenceCount::Ignore && rc.action!=ReferenceCount::Invalid){
            hasReferenceCount = true;
            break;
        }
    }
    if (hasReferenceCount
            || argumentsHaveNativeId()
            || isConstructor()
            || (((ownerClass()->typeEntry()->designatedInterface() && !this->isAbstract()) || implementingClass()->typeEntry()->isNativeIdBased()) && !isStatic()))
        return true;
    if(this->argumentReplaced(0)=="this" && implementingClass()->typeEntry()->isNativeIdBased())
        return true;

    for(const MetaArgument *arg : arguments()) {
        if (arg->type()->isArray()
                || arg->type()->isTargetLangEnum()
                || arg->type()->isTargetLangFlags()
                || arg->type()->typeEntry()->isNativeIdBased())
            return true;
        if(argumentTypeArray(arg->argumentIndex()+1) || argumentTypeBuffer(arg->argumentIndex()+1)){
            int minArrayLength = argumentTypeArrayLengthMinValue(arg->argumentIndex()+1);
            if(minArrayLength>0)
                return true;
        }
    }

    if (type() && (type()->isArray() || type()->isTargetLangEnum() || type()->isTargetLangFlags()))
        return true;

    for (int i = -1; i <= arguments().size(); ++i) {
        OwnershipRule ownershipRule = this->ownership(implementingClass(), TS::TargetLangCode, i);
        if (ownershipRule.ownership != TS::InvalidOwnership && ownershipRule.ownership != TS::IgnoreOwnership)
            return true;
    }

    const MetaClass *cls = implementingClass();
    while (cls) {
        for(const FunctionModification& mod : this->modifications(cls)) {
            if (mod.snips.count() <= 0)
                continue ;
            for(const CodeSnip& snip : mod.snips) {
                if (snip.language == TS::TargetLangCode)
                    return true;
            }
        }

        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }

    return false;
}

bool MetaFunction::needsSuppressRawTypeWarning() const {
    MetaArgumentList arguments = this->arguments();
    for(const MetaArgument *arg : arguments) {
        if(arg->type()->typeEntry()->isComplex()){
            const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(arg->type()->typeEntry());
            if(ctype->isGenericClass() && arg->type()->instantiations().isEmpty()){
                return true;
            }
        }
    }
    return false;
}

bool MetaFunction::needsSuppressUncheckedWarning() const {
    /*
    for (int i = -1; i <= arguments().size(); ++i) {
        QList<ReferenceCount> referenceCounts = this->referenceCounts(implementingClass(), i);
        for(ReferenceCount referenceCount : referenceCounts) {
            if (referenceCount.action != ReferenceCount::Set)
                return true;
        }
    }
    */
    return false;
}

QString MetaFunction::marshalledName() const {
    QString returned;
    if(this->isConstructor()){
        returned = "initialize_native";
    }else{
        returned = name() + "_native";
        for(const MetaArgument *arg : this->arguments()) {
            if(arg->type()->getReferenceType()==MetaType::Reference){
                if(arg->type()->isConstant()){
                    returned += "_cref";
                }else{
                    returned += "_ref";
                }
            }else if(arg->type()->getReferenceType()==MetaType::RReference){
                if(arg->type()->isConstant()){
                    returned += "_crval";
                }else{
                    returned += "_rval";
                }
            }else{
                if(arg->type()->isConstant()){
                    returned += "_const";
                }
            }
            QString qualifiedCppName;
            if(arg->type()->typeEntry()->isFunctional()){
                qualifiedCppName = arg->type()->typeEntry()->name();
            }else{
                qualifiedCppName = arg->type()->typeEntry()->qualifiedCppName();
            }
            if(qualifiedCppName=="qtjamireal")
                qualifiedCppName = "double";
            returned += "_" + qualifiedCppName
                    .replace("::", "_")
                    .replace("<", "_")
                    .replace(">", "_")
                    .replace(",", "_")
                    .replace(".", "_")
                    .replace("*", "_ptr")
                    .replace("&", "_ref")
                    .replace(" ", "_");
            for(bool ind : arg->type()->indirections()){
                returned += ind ? "_cptr" : "_ptr";
            }
        }
        if(this->isConstant()){
            returned += "_constfct";
        }
    }
    return returned;
}

bool MetaFunction::operator<(const MetaFunction &other) const {
    uint result = compareTo(&other);
    return result & NameLessThan;
}


/*!
    Returns a mask of CompareResult describing how this function is
    compares to another function
*/
uint MetaFunction::compareTo(const MetaFunction *other) const {
    uint result = 0;

    // Enclosing class...
    if (ownerClass() == other->ownerClass()) {
        result |= EqualImplementor;
    }

    // Attributes
    if (attributes() == other->attributes()) {
        result |= EqualAttributes;
    }

    // Compare types
    MetaType *t = type();
    MetaType *ot = other->type();
    if ((!t && !ot) || ((t && ot && t->name() == ot->name()))) {
        result |= EqualReturnType;
    }

    // Compare names
    int cmp = originalName().compare(other->originalName());

    if (cmp < 0) {
        result |= NameLessThan;
    } else if (cmp == 0) {
        result |= EqualName;
    }

    // compare name after modification...
    cmp = modifiedName().compare(other->modifiedName());
    if (cmp == 0)
        result |= EqualModifiedName;

    // Compare arguments...
    const MetaArgumentList& min_arguments = arguments().size() < other->arguments().size() ? arguments() : other->arguments();
    const MetaArgumentList& max_arguments = arguments().size() < other->arguments().size() ? other->arguments(): arguments();

    auto min_count = min_arguments.size();
    auto max_count = max_arguments.size();
    bool same = true;
    for (int i = 0; i < max_count; ++i) {
        if (i < min_count) {
            const MetaArgument *min_arg = min_arguments.at(i);
            const MetaArgument *max_arg = max_arguments.at(i);
            if (min_arg->type()->typeEntry() != max_arg->type()->typeEntry()
                    && min_arg->type()->typeEntry()->qualifiedTargetLangName() != max_arg->type()->typeEntry()->qualifiedTargetLangName()
                    && (min_arg->defaultValueExpression().isEmpty() || max_arg->defaultValueExpression().isEmpty())) {
                same = false;
                break;
            }
            if(min_arg->type()->typeEntry()==max_arg->type()->typeEntry()
                    && min_arg->type()->typeEntry()->isContainer()){
                if(reinterpret_cast<const ContainerTypeEntry*>(min_arg->type()->typeEntry())->type()==ContainerTypeEntry::InitializerListContainer){
                    if(min_arg->type()->instantiations().size()==1
                            && max_arg->type()->instantiations().size()==1){
                        if (min_arg->type()->instantiations()[0]->typeEntry() != max_arg->type()->instantiations()[0]->typeEntry()
                                && min_arg->type()->instantiations()[0]->typeEntry()->qualifiedTargetLangName() != max_arg->type()->instantiations()[0]->typeEntry()->qualifiedTargetLangName()) {
                            same = false;
                            break;
                        }
                    }
                }
            }
        } else {
            if (max_arguments.at(i)->defaultValueExpression().isEmpty()) {
                same = false;
                break;
            }
        }
    }

    if (same)
        result |= ( (min_count == max_count) ? EqualArguments : EqualDefaultValueOverload);

    return result;
}

MetaFunction *MetaFunction::copy() const {
    MetaFunction *cpy = new MetaFunction;
    cpy->setName(name());
    cpy->setOriginalName(originalName());
    cpy->setOwnerClass(ownerClass());
    cpy->setImplementingClass(implementingClass());
    cpy->setInterfaceClass(interfaceClass());
    cpy->setFunctionType(functionType());
    cpy->setAttributes(attributes());
    cpy->setDeclaringClass(declaringClass());
    if (type())
        cpy->setType(type()->copy());
    cpy->setConstant(isConstant());
    cpy->setOriginalAttributes(originalAttributes());
    cpy->setDeprecatedComment(deprecatedComment());
    cpy->setHref(href());
    cpy->setBrief(brief());

    for(const MetaArgument *arg : arguments())
        cpy->addArgument(arg->copy());

    for(const MetaTemplateParameter *param : templateParameters())
        cpy->addTemplateParameter(param->copy());

    Q_ASSERT((!type() && !cpy->type())
             || (type()->instantiations() == cpy->type()->instantiations()));

    return cpy;
}

QStringList MetaFunction::introspectionCompatibleSignatures(const QStringList &resolvedArguments) const {
    const MetaArgumentList& arguments = this->arguments();
    if (arguments.size() == resolvedArguments.size()) {
        return (QStringList() << QMetaObject::normalizedSignature((name() + "(" + resolvedArguments.join(",") + ")").toUtf8().constData()));
    } else {
        QStringList returned;

        MetaArgument *argument = arguments.at(resolvedArguments.size());
        QStringList minimalTypeSignature = argument->type()->minimalSignature().split("::");
        for (int i = 0; i < minimalTypeSignature.size(); ++i) {
            returned += introspectionCompatibleSignatures(QStringList(resolvedArguments)
                        << QStringList(minimalTypeSignature.mid(minimalTypeSignature.size() - i - 1)).join("::"));
        }

        return returned;
    }
}

QString MetaFunction::signature(bool skipName) const {
    if(!skipName){
        if(!m_cached_full_signature.isEmpty())
            return m_cached_full_signature;
    }else{
        if(!m_cached_full_signature_no_name.isEmpty())
            return m_cached_full_signature_no_name;
    }
    QString s(m_original_name);

    s += "(";

    for (int i = 0; i < m_arguments.count(); ++i) {
        if (i > 0)
            s += ", ";
        MetaArgument *a = m_arguments.at(i);
        s += a->type()->cppSignature();

        if(!skipName){
            // We need to have the argument names in the qdoc files
            s += " ";
            s += a->argumentName();
        }
    }
    s += ")";

    if (isConstant())
        s += " const";

    if(!skipName)
        m_cached_full_signature = s;
    else
        m_cached_full_signature_no_name = s;
    return s;
}

bool MetaFunction::isCopyConstructor() const {
    if(this->isConstructor()){
        const MetaArgumentList& arguments = this->arguments();
        if(arguments.size()==1){
            MetaArgument* argument = arguments.at(0);
            if(argument->type()->getReferenceType()==MetaType::Reference
                    && argument->type()->indirections().isEmpty()
                    && argument->type()->typeEntry()->name()==this->declaringClass()->typeEntry()->name()
                    && argument->defaultValueExpression().isEmpty()
                    && argument->originalDefaultValueExpression().isEmpty()){
                return true;
            }
        }
    }
    return false;
}

bool MetaFunction::hasRReferences() const {
    const MetaArgumentList& arguments = this->arguments();
    for(int i=0; i<arguments.size(); i++){
        MetaArgument* argument = arguments.at(i);
        if(argument->type()->getReferenceType()==MetaType::RReference){
            return true;
        }
    }
    return false;
}


int MetaFunction::actualMinimumArgumentCount() const {
    if(m_actualMinimumArgumentCount==-1){
        FunctionModificationList modifications = this->modifications(implementingClass());
        if(implementingClass()!=declaringClass())
            modifications << this->modifications(declaringClass());
        QSet<int> removedArguments;
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
                if (argument_modification.type==ArgumentModification::Default && argument_modification.removed)
                    removedArguments.insert(argument_modification.index);
            }
        }

        const MetaArgumentList& arguments = this->arguments();

        m_actualMinimumArgumentCount = 0;
        for (int i = 0; i < arguments.size(); ++i) {
            if (!removedArguments.contains(i + 1) && arguments.at(i)->originalDefaultValueExpression().isEmpty())
                ++m_actualMinimumArgumentCount;
        }
    }
    return m_actualMinimumArgumentCount;
}

// Returns reference counts for argument at idx, or all arguments if idx == -2
QList<ReferenceCount> MetaFunction::referenceCounts(const MetaClass *cls, int idx) const {
    QList<ReferenceCount> returned;

    if(m_accessedField){
        if((idx==1 || idx == -2) && (this->attributes() & MetaAttributes::SetterFunction) == MetaAttributes::SetterFunction){
            FieldModificationList modifications = m_accessedField->modifications();
            for(const FieldModification& modification : modifications) {
                returned += modification.referenceCounts;
            }
        }
    }else{
        FunctionModificationList mods = this->modifications(cls);
        for(const FunctionModification& mod : mods) {
            for(const ArgumentModification& argument_mod : mod.argument_mods) {
                if(argument_mod.type!=ArgumentModification::Default)
                    continue;
                if (argument_mod.index != idx && idx != -2)
                    continue;
                returned += argument_mod.referenceCounts;
            }
        }
    }
    return returned;
}

QList<TemplateInstantiation> MetaFunction::templateInstantiations(const MetaClass *cls) const{
    QList<TemplateInstantiation> returned;

    FunctionModificationList mods = this->modifications(cls);
    for(const FunctionModification& mod : mods) {
        returned.append(mod.template_instantiations);
    }

    return returned;
}

QString MetaFunction::replacedDefaultExpression(const MetaClass *cls, int key) const {
    FunctionModificationList modifications = this->modifications(cls);
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key
                    && !argument_modification.replaced_default_expression.isEmpty()) {
                return argument_modification.replaced_default_expression;
            }
        }
    }

    return QString();
}

bool MetaFunction::removedDefaultExpression(const MetaClass *cls, int key) const {
    FunctionModificationList modifications = this->modifications(cls);
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key
                    && argument_modification.removed_default_expression) {
                return true;
            }
        }
    }

    return false;
}

bool MetaFunction::resetObjectAfterUse(int argument_idx) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argumentModification : modification.argument_mods) {
            if(argumentModification.type!=ArgumentModification::Default)
                continue;
            if (argumentModification.index == argument_idx && argumentModification.reset_after_use)
                return true;
        }
    }

    return false;
}

bool MetaFunction::valueAsPointer(int argument_idx) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argumentModification : modification.argument_mods) {
            if(argumentModification.type!=ArgumentModification::Default)
                continue;
            if (argumentModification.index == argument_idx && argumentModification.value_as_pointer)
                return true;
        }
    }

    return false;
}

bool MetaFunctional::resetObjectAfterUse(int argument_idx) const {
    for(const ArgumentModification& argumentModification : typeEntry()->argumentModification()) {
        if (argumentModification.index == argument_idx && argumentModification.reset_after_use)
            return true;
    }

    return false;
}

QString MetaFunction::nullPointerDefaultValue(const MetaClass *mainClass, int argument_idx) const {
    const MetaClass *cls = mainClass;
    if (!cls)
        cls = implementingClass();

    do {
        FunctionModificationList modifications = this->modifications(cls);
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
                if(argument_modification.type!=ArgumentModification::Default)
                    continue;
                if (argument_modification.index == argument_idx
                        && argument_modification.no_null_pointers) {
                    return argument_modification.null_pointer_default_value;
                }
            }
        }

        cls = cls->baseClass();
    } while (cls && !mainClass); // Once when mainClass != 0, or once for all base classes of implementing class

    return QString();

}

bool MetaFunctional::needsCallThrough() const{
    bool needsCallThrough = typeEntry()->isNativeIdBased();
    if(!needsCallThrough){
        for(MetaArgument * arg : arguments()){
            if(argumentRemoved(arg->argumentIndex() + 1))
                continue;
            if(typeReplaced(arg->argumentIndex()+1).isEmpty()
                    && arg->type()->hasNativeId()) {
                needsCallThrough = true;
                break;
            }
        }
    }
    return needsCallThrough;
}

bool MetaFunctional::nullPointersDisabled(int argument_idx) const {
    for(const ArgumentModification& argument_modification : typeEntry()->argumentModification()) {
        if (argument_modification.index == argument_idx
                && argument_modification.no_null_pointers) {
            return true;
        }
    }
    return false;
}

bool MetaFunction::nullPointersDisabled(const MetaClass *mainClass, int argument_idx) const {
    if(m_accessedField){
        if(argument_idx==1 && (this->attributes() & MetaAttributes::SetterFunction) == MetaAttributes::SetterFunction){
            FieldModificationList modifications = m_accessedField->modifications();
            for(const FieldModification& modification : modifications) {
                if(modification.no_null_pointers)
                    return true;
            }
        }
    }else{
        const MetaClass *cls = mainClass;
        if (!cls)
            cls = implementingClass();

        do {
            FunctionModificationList modifications = this->modifications(cls);
            for(const FunctionModification& modification : modifications) {
                for(const ArgumentModification& argument_modification : modification.argument_mods) {
                    if(argument_modification.type!=ArgumentModification::Default)
                        continue;
                    if (argument_modification.index == argument_idx
                            && argument_modification.no_null_pointers) {
                        return true;
                    }
                }
            }

            cls = cls->baseClass();
        } while (cls && !mainClass); // Once when mainClass != 0, or once for all base classes of implementing class
    }
    return false;
}

QString MetaFunction::conversionRule(TS::Language language, int key) const {
    if(m_accessedField){
        if( (key==1 && (this->attributes() & MetaAttributes::SetterFunction) == MetaAttributes::SetterFunction)
                || (key==0 && (this->attributes() & MetaAttributes::GetterFunction) == MetaAttributes::GetterFunction) ){
            TS::Language _language = language;
            if(key==0)
                _language = TS::Language(language | TS::Getter);
            else
                _language = TS::Language(language | TS::Setter);
            FieldModificationList modifications = m_accessedField->modifications();
            for(const FieldModification& modification : modifications) {
                for(const CodeSnip& snip : modification.conversion_rules) {
                    if (snip.language == _language){
                        QString core = snip.code();
                        if(!core.isEmpty())
                            return snip.code();
                    }
                }
            }
        }
    }else{
        FunctionModificationList modifications = this->modifications(implementingClass());
        if(implementingClass()!=declaringClass())
            modifications << this->modifications(declaringClass());
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
                if(argument_modification.type!=ArgumentModification::Default)
                    continue;
                if (argument_modification.index != key)
                    continue;

                for(const CodeSnip& snip : argument_modification.conversion_rules) {
                    if (snip.language == language){
                        QString core = snip.code();
                        if(!core.isEmpty())
                            return snip.code();
                    }
                }
            }
        }
    }

    return QString();
}

bool MetaFunction::hasConversionRule(TS::Language language, int key) const {
    if(m_accessedField){
        if( (key==1 && (this->attributes() & MetaAttributes::SetterFunction) == MetaAttributes::SetterFunction)
                || (key==0 && (this->attributes() & MetaAttributes::GetterFunction) == MetaAttributes::GetterFunction) ){
            TS::Language _language = language;
            if(key==0)
                _language = TS::Language(language | TS::Getter);
            else
                _language = TS::Language(language | TS::Setter);
            FieldModificationList modifications = m_accessedField->modifications();
            for(const FieldModification& modification : modifications) {
                for(const CodeSnip& snip : modification.conversion_rules) {
                    if (snip.language == _language && snip.hasCode())
                        return true;
                }
            }
        }
    }else{
        FunctionModificationList modifications = this->modifications(implementingClass());
        if(implementingClass()!=declaringClass())
            modifications << this->modifications(declaringClass());
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
                if(argument_modification.type!=ArgumentModification::Default)
                    continue;
                if (key>=0 && argument_modification.index != key)
                    continue;

                for(const CodeSnip& snip : argument_modification.conversion_rules) {
                    if (snip.language == language && snip.hasCode())
                        return true;
                }
            }
        }
    }
    return false;
}

QString MetaFunctional::conversionRule(TS::Language language, int key) const {
    for(const ArgumentModification& argument_modification : typeEntry()->argumentModification()) {
        if (argument_modification.index != key)
            continue;

        for(const CodeSnip& snip : argument_modification.conversion_rules) {
            if (snip.language == language && !snip.code().isEmpty())
                return snip.code();
        }
    }

    return QString();
}

bool MetaFunctional::hasConversionRule(TS::Language language, int key) const {
    for(const ArgumentModification& argument_modification : typeEntry()->argumentModification()) {
        if (key>=0 && argument_modification.index != key)
            continue;

        for(const CodeSnip& snip : argument_modification.conversion_rules) {
            if (snip.language == language && snip.hasCode())
                return true;
        }
    }

    return false;
}

QString MetaFunction::proxyCall() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if(!modification.proxyCall.isEmpty())
            return modification.proxyCall;
    }

    return {};
}

QString MetaFunction::argumentReplaced(int key) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key && !argument_modification.replace_value.isEmpty()) {
                return argument_modification.replace_value;
            }
        }
    }

    return "";
}

QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> MetaFunction::addedArguments() const
{
    QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> result;
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type==ArgumentModification::AdditionalArgument){
                if(argument_modification.index==std::numeric_limits<int>::max())
                    result.second << argument_modification;
                else
                    result.first[argument_modification.index] = argument_modification;
            }
        }
    }
    return result;
}

QList<Parameter> MetaFunction::addedParameterTypes() const
{
    QList<Parameter> result;
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type==ArgumentModification::TypeParameter){
                result << Parameter{{}, argument_modification.modified_name, argument_modification.modified_type};
            }
        }
    }
    return result;
}

ArgumentRemove MetaFunction::argumentRemoved(int key) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key) {
                if (argument_modification.removed) {
                    return ArgumentRemove_Remove;
                }
            }else if (argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)
                          && argument_modification.arrayLengthParameter == key) {
                return ArgumentRemove_UseAsLength;
            }
        }
    }
    if(arguments().size()>=key){
        MetaType* type = arguments()[key-1]->type();
        if(type && type->isNullPtr()){
            return ArgumentRemove_Remove;
        }
    }

    return ArgumentRemove_No;
}

ThreadAffinity MetaFunction::argumentThreadAffinity(int key) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key) {
                return argument_modification.thread_affine;
            }
        }
    }
    return ThreadAffinity::None;
}

ArgumentRemove MetaFunctional::argumentRemoved(int key) const {
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            if (argument_modification.removed) {
                return ArgumentRemove_Remove;
            }else if (argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)
                        && argument_modification.arrayLengthParameter == key) {
                return ArgumentRemove_UseAsLength;
            }
        }
    }
    return ArgumentRemove_No;
}

bool MetaFunction::isVirtualSlot() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isVirtualSlot())
            return true;
    }

    return false;
}

bool MetaFunction::isUIThreadAffine() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isUIThreadAffine())
            return true;
    }

    return false;
}

bool MetaFunction::isPixmapThreadAffine() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isPixmapThreadAffine())
            return true;
    }

    return false;
}

bool MetaFunction::isThreadAffine() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isThreadAffine())
            return true;
    }

    return false;
}

bool MetaFunction::isPaintMethod() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isPaintMethod()) {
            return true;
        }
    }

    return false;
}

bool MetaFunction::isAllowedAsSlot() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isAllowedAsSlot()) {
            return true;
        }
    }

    return false;
}

bool MetaFunction::isDeprecated() const {
    if(isDeclDeprecated()){
       return true;
    }
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isDeprecated())
            return true;
    }
    return false;
}

OwnershipRule MetaFunction::ownership(const MetaClass *cls, TS::Language language, int key) const {
    if(m_accessedField){
        if(key==1 && (this->attributes() & MetaAttributes::SetterFunction) == MetaAttributes::SetterFunction){
            for(const FieldModification& modification : m_accessedField->modifications()) {
                OwnershipRule o = modification.ownerships.value(language, OwnershipRule{TS::InvalidOwnership, QString()});
                if(o.ownership!=TS::InvalidOwnership)
                    return o;
            }
        }
    }else{
        FunctionModificationList modifications = this->modifications(cls);
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
                if(argument_modification.type!=ArgumentModification::Default)
                    continue;
                if (argument_modification.index == key){
                    OwnershipRule o = argument_modification.ownerships.value(language, OwnershipRule{TS::InvalidOwnership, QString()});
                    if(o.ownership!=TS::InvalidOwnership)
                        return o;
                }
            }
        }
    }

    return OwnershipRule{TS::InvalidOwnership, QString()};
}

bool MetaFunction::isRemovedFromAllLanguages(const MetaClass *cls) const {
    return isRemovedFrom(cls, TS::All);
}

bool MetaFunction::isRemovedFrom(const MetaClass *cls, TS::Language language) const {
    FunctionModificationList modifications = this->modifications(cls);
    for(const FunctionModification& modification : modifications) {
        if ((modification.removal & language) == language)
            return true;
    }

    return false;

}

int MetaFunction::argumentTypeArrayLengthIndex(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key
                    && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
                return argument_modification.arrayLengthParameter;
            }
        }
    }

    return -1;
}

int MetaFunction::argumentTypeArrayLengthMinValue(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key
                    && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
                return argument_modification.minArrayLength;
            }
        }
    }
    return -1;
}

int MetaFunction::argumentTypeArrayLengthMaxValue(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key
                    && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
                return argument_modification.maxArrayLength;
            }
        }
    }
    return -1;
}

int MetaFunctional::argumentTypeArrayLengthIndex(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key
                && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
            return argument_modification.arrayLengthParameter;
        }
    }
    return -1;
}

int MetaFunctional::argumentTypeArrayLengthMinValue(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key
                && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
            return argument_modification.minArrayLength;
        }
    }
    return -1;
}

int MetaFunctional::argumentTypeArrayLengthMaxValue(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key
                && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
            return argument_modification.maxArrayLength;
        }
    }
    return -1;
}

bool MetaFunction::argumentTypeArrayDeref(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key) {
                return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::Deref);
            }
        }
    }

    return false;
}

bool MetaFunctional::isNoExcept() const{
    for(const Modification& modification : this->typeEntry()->modifications()) {
        if(modification.isNoExcept())
            return true;
    }
    return false;
}

bool MetaFunctional::isBlockExceptions() const{
    for(const Modification& modification : this->typeEntry()->modifications()) {
        if(modification.isBlockExceptions())
            return true;
    }
    return false;
}

bool MetaFunctional::isRethrowExceptions() const{
    for(const Modification& modification : this->typeEntry()->modifications()) {
        if(modification.isRethrowExceptions())
            return true;
    }
    return false;
}

bool MetaFunction::isNoExcept() const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if(modification.isNoExcept())
            return true;
    }
    return false;
}

bool MetaFunction::isBlockExceptions() const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if(modification.isBlockExceptions())
            return true;
    }
    return false;
}

bool MetaFunction::isRethrowExceptions() const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if(modification.isRethrowExceptions())
            return true;
    }
    return false;
}

bool MetaFunction::argumentTypeArray(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key) {
                return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && !argument_modification.useAsArrayType.testFlag(AsArrayType::Buffer);
            }
        }
    }

    return false;
}

bool MetaFunction::argumentTypeBuffer(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key) {
                return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::Buffer);
            }
        }
    }

    return false;
}

bool MetaFunction::argumentTypeArrayVarArgs(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.type!=ArgumentModification::Default)
                continue;
            if (argument_modification.index == key) {
                return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::VarArgs);
            }
        }
    }

    return false;
}

bool MetaFunctional::argumentTypeArrayVarArgs(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::VarArgs);
        }
    }
    return false;
}

bool MetaFunctional::argumentTypeArrayDeref(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::Deref);
        }
    }
    return false;
}

bool MetaFunctional::argumentTypeArray(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && !argument_modification.useAsArrayType.testFlag(AsArrayType::Buffer);
        }
    }
    return false;
}

bool MetaFunctional::argumentTypeBuffer(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::Buffer);
        }
    }
    return false;
}

QString MetaFunction::typeReplaced(int key, QString* jniType) const {
    if(m_accessedField){
        if( (key==1 && (this->attributes() & MetaAttributes::SetterFunction) == MetaAttributes::SetterFunction)
                || (key==0 && (this->attributes() & MetaAttributes::GetterFunction) == MetaAttributes::GetterFunction) ){
            FieldModificationList modifications = m_accessedField->modifications();
            if(!modifications.isEmpty()) {
                if(jniType && !modifications.first().modified_jni_type.isEmpty()){
                    *jniType = modifications.first().modified_jni_type;
                }
                return modifications.first().modified_type;
            }
        }
    }else{
        FunctionModificationList modifications = this->modifications(implementingClass());
        if(implementingClass()!=declaringClass())
            modifications << this->modifications(declaringClass());
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
                if (argument_modification.type==ArgumentModification::Default
                        && argument_modification.index == key
                        && !argument_modification.modified_type.isEmpty()) {
                    if(jniType && !argument_modification.modified_jni_type.isEmpty()){
                        *jniType = argument_modification.modified_jni_type;
                    }
                    return argument_modification.modified_type;
                }
            }
        }
    }
    return QString();
}

QString MetaFunctional::typeReplaced(int key, QString* jniType) const {
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key
                && !argument_modification.modified_type.isEmpty()) {
            if(jniType
                    && argument_modification.type==ArgumentModification::Default
                    && !argument_modification.modified_jni_type.isEmpty()){
                *jniType = argument_modification.modified_jni_type;
            }
            return argument_modification.modified_type;
        }
    }

    return QString();
}

void writeTypeInfo(QTextStream &s, const MetaType *type);

QString MetaFunction::minimalSignature() const {
    if (!m_cached_minimal_signature.isEmpty())
        return m_cached_minimal_signature;

    QString tmpl;
    for(int i=0; i<m_templateParameters.size(); i++){
        MetaTemplateParameter* param = m_templateParameters[i];
        if(param->type()){
            if(!tmpl.isEmpty()){
                tmpl += ",";
            }
            QTextStream s2(&tmpl);
            writeTypeInfo(s2, param->type());
        }else{
            tmpl.clear();
            break;
        }
    }
    if(!tmpl.isEmpty())
        tmpl = "<" + tmpl.trimmed() + ">";
    QString minimalSignature = originalName() + tmpl + "(";
    const MetaArgumentList& arguments = this->arguments();

    for (int i = 0; i < arguments.count(); ++i) {
        MetaType *t = arguments.at(i)->type();

        if (i > 0)
            minimalSignature += ",";

        minimalSignature += t->minimalSignature();
    }
    minimalSignature += ")";
    if (isConstant())
        minimalSignature += "const";

    minimalSignature = QMetaObject::normalizedSignature(minimalSignature.toLocal8Bit().constData());
    m_cached_minimal_signature = minimalSignature;

    return minimalSignature;
}

QString MetaFunction::throws(const MetaClass *implementor) const {
    if(!implementor)
        implementor = this->declaringClass();
    Q_ASSERT(implementor);
    for(const FunctionModification& fm : implementor->typeEntry()->functionModifications(minimalSignature())){
        if(!fm.throws.isEmpty()){
            return fm.throws;
        }
    }
    return QString();
}

bool MetaFunction::hasCodeInjections(const MetaClass *implementor, TS::Language language, const QSet<CodeSnip::Position>& positions) const{
    return implementor->typeEntry()->hasFunctionCodeInjections(minimalSignature(), language, positions);
}

FunctionModificationList MetaFunction::modifications(const MetaClass *implementor) const {
    Q_ASSERT(implementor);
    FunctionModificationList result = implementor->typeEntry()->functionModifications(minimalSignature());
    if(result.isEmpty() && (implementor->qualifiedCppName().startsWith("QOpenGLFunctions") || implementor->qualifiedCppName()=="QOpenGLExtraFunctions")){
        const MetaArgumentList& arguments = this->arguments();
        QList<ArgumentModification> argumentMods;
        for (int i = 0; i < arguments.count(); ++i) {
            MetaType *t = arguments.at(i)->type();
            if(t->isNativePointer() && (t->typeEntry()->isPrimitive() || t->typeEntry()->isVoid()) && t->typeEntry()->qualifiedCppName()!="GLboolean" && t->indirections().size()==1){
                ArgumentModification mod(i+1);
                mod.useAsArrayType |= AsArrayType::Yes;
                mod.useAsArrayType |= AsArrayType::Buffer;
                argumentMods << mod;
            }
        }
        if(!argumentMods.isEmpty()){
            FunctionModification mod;
            mod.signature = minimalSignature();
            mod.argument_mods = argumentMods;
            result << mod;
        }
    }
    return result;
}

bool MetaFunction::hasModifications(const MetaClass *implementor) const {
    FunctionModificationList mods = modifications(implementor);
    return mods.count() > 0;
}

QString MetaFunction::modifiedName() const {
    if (m_cached_modified_name.isEmpty()) {
        FunctionModificationList mods = modifications(implementingClass());
        for(const FunctionModification& mod : mods) {
            if (mod.isRenameModifier()) {
                m_cached_modified_name = mod.renamedToName;
                break;
            }
        }
        if (m_cached_modified_name.isEmpty())
            m_cached_modified_name = name();
    }
    return m_cached_modified_name;
}

QString MetaFunction::targetLangSignature(bool minimal) const {
    QString s;

    // Attributes...
    if (!minimal) {
        if (isPublic()) s += "public ";
        else if (isProtected()) s += "protected ";
        else if (isPrivate()) s += "private ";

//     if (isNative()) s += "native ";
//     else
        if (isFinalInTargetLang()) s += "final ";
        else if (isAbstract()) s += "abstract ";

        if (isStatic()) s += "static ";

        // Return type
        if (type())
            s += type()->fullName() + " ";
        else
            s += "void ";
    }

    s += name();
    s += "(";

    for (int i = 0; i < m_arguments.size(); ++i) {
        if (i != 0) {
            s += ",";
            if (!minimal)
                s += QLatin1Char(' ');
        }
        s += m_arguments.at(i)->type()->fullName();

        if (!minimal) {
            s += " ";
            s += m_arguments.at(i)->argumentName();
        }
    }

    s += ")";

    return s;
}


/*******************************************************************************
 * AbstractMetaClass
 */
MetaClass::~MetaClass() {
    qDeleteAll(m_functions);
    qDeleteAll(m_fields);
}

/*AbstractMetaClass *AbstractMetaClass::copy() const
{
    AbstractMetaClass *cls = new AbstractMetaClass;
    cls->setAttributes(attributes());
    cls->setBaseClass(baseClass());
    cls->setTypeEntry(typeEntry());
    for(AbstractMetaFunction *function : functions()) {
        AbstractMetaFunction *copy = function->copy();
        function->setImplementingClass(cls);
        cls->addFunction(copy);
    }
    cls->setEnums(enums());
    for(const AbstractMetaField *field : fields()) {
        AbstractMetaField *copy = field->copy();
        copy->setEnclosingClass(cls);
        cls->addField(copy);
    }
    cls->setInterfaces(interfaces());

    return cls;
}*/

/*******************************************************************************
 * Returns true if this class is a subclass of the given class
 */
bool MetaClass::inheritsFrom(const MetaClass *cls) const {
    Q_ASSERT(cls);

    const MetaClass *clazz = this;
    while (clazz) {
        if (clazz == cls)
            return true;

        clazz = clazz->baseClass();
    }

    return false;
}

bool MetaClass::inheritsFromInterface(const MetaClass *cls) const {
    Q_ASSERT(cls);

    const MetaClass *clazz = this;
    while (clazz) {
        if (clazz == cls || clazz->interfaces().contains(const_cast<MetaClass *>(cls)))
            return true;

        clazz = clazz->baseClass();
    }

    return false;
}

/*******************************************************************************
 * Constructs an interface based on the functions and enums in this
 * class and returns it...
 */
MetaClass *MetaClass::extractInterface() {
    Q_ASSERT(typeEntry()->designatedInterface());

    if (!m_extracted_interface) {
        MetaClass *iface = new MetaClass;
        iface->setAttributes(attributes());
        iface->setBaseClass(nullptr);
        iface->setPrimaryInterfaceImplementor(this);

        iface->setTypeEntry(typeEntry()->designatedInterface());

        for(MetaFunction *function : functions()) {
            if (!function->isConstructor())
                iface->addFunction(function->copy());
        }

//         iface->setEnums(enums());
//         setEnums(AbstractMetaEnumList());

        for(const MetaField *field : fields()) {
            if (field->isPublic()) {
                MetaField *new_field = field->copy();
                new_field->setEnclosingClass(iface);
                iface->addField(new_field);
            }
        }

        if(iface->isInterface()){
            m_extracted_interface = iface;
            addInterface(iface);
        }
    }

    return m_extracted_interface;
}

MetaClass *MetaClass::extractInterface() const {
    return m_extracted_interface;
}

/*******************************************************************************
 * Constructs an implementation class based on the functions and enums in this
 * interface and returns it...
 */
MetaClass *MetaClass::extractInterfaceImpl() {
    Q_ASSERT(typeEntry()->isInterface());
    const InterfaceTypeEntry* itype = static_cast<const InterfaceTypeEntry*>(typeEntry());
    Q_ASSERT(itype->origin());

    if (!m_extracted_interface_impl) {
        MetaClass *ifaceImpl = new MetaClass;
        ifaceImpl->setAttributes(attributes());
        ifaceImpl->setBaseClass(nullptr);
        ifaceImpl->setOriginalAttributes(originalAttributes());
        ifaceImpl->setAttributes(attributes());
        ifaceImpl->setHasCloneOperator(hasCloneOperator());
        ifaceImpl->setHasEqualsOperator(hasEqualsOperator());
        ifaceImpl->setHasHashFunction(hasHashFunction());
        ifaceImpl->setHasJustPrivateConstructors(hasJustPrivateConstructors());
        ifaceImpl->setHasPrivateDestructor(hasPrivateDestructor());
        ifaceImpl->setHasVirtualDestructor(hasVirtualDestructor());
        ifaceImpl->setHasPublicDestructor(hasPublicDestructor());
        ifaceImpl->setHasVirtuals(hasVirtuals());
        ifaceImpl->setHasVirtualSlots(hasVirtualSlots());
        ifaceImpl->setHas_Q_GADGET(has_Q_GADGET());
        ifaceImpl->setHas_Q_OBJECT(has_Q_OBJECT());
        ifaceImpl->setBaseClassNames(this->baseClassNames());
        ifaceImpl->setHref(href());
        ifaceImpl->setBrief(brief());
        itype->origin()->setInclude(itype->include());
        itype->origin()->setExtraIncludes(itype->extraIncludes());
        itype->origin()->setExpensePolicy(itype->expensePolicy());
        setPrimaryInterfaceImplementor(ifaceImpl);

        ifaceImpl->setTypeEntry(itype->origin());

        for(MetaFunction *function : functions()) {
//            if (!function->isConstructor())
                function = function->copy();
                if (function->isConstructor())
                    function->setName(InterfaceTypeEntry::implName("X").split("::").last());
                else
                    *function += MetaAttributes::InterfaceFunction;
                function->setInterfaceClass(this);
                function->setImplementingClass(ifaceImpl);
                function->setDeclaringClass(ifaceImpl);
                ifaceImpl->addFunction(function);
        }

//         iface->setEnums(enums());
//         setEnums(AbstractMetaEnumList());

        for(const MetaField *field : fields()) {
//            if (field->isPublic()) {
                MetaField *new_field = field->copy();
                new_field->setEnclosingClass(ifaceImpl);
                ifaceImpl->addField(new_field);
//            }
        }

        if(!ifaceImpl->isInterface()){
            ifaceImpl->addInterface(this);
            this->addEnclosedClass(ifaceImpl);
            m_extracted_interface_impl = ifaceImpl;
            ifaceImpl->m_extracted_interface = this;
        }
    }

    return m_extracted_interface_impl;
}

MetaClass *MetaClass::extractInterfaceImpl() const {
    return m_extracted_interface_impl;
}

/*******************************************************************************
 * Returns a list of all the functions with a given name
 */
MetaFunctionList MetaClass::queryFunctionsByName(const QString &name) const {
    MetaFunctionList returned;
    for(MetaFunction *function : this->functions()) {
        if (function->name() == name)
            returned.append(function);
    }

    return returned;
}

MetaFunctionList MetaClass::queryFunctionsByOriginalName(const QString &name) const {
    MetaFunctionList returned;
    for(MetaFunction *function : this->functions()) {
        if (function->originalName() == name)
            returned.append(function);
    }

    return returned;
}

MetaFunctionList MetaClass::queryOtherFunctions(const MetaFunction* fun) const {
    MetaFunctionList returned;
    for(MetaFunction *function : this->functions()) {
        if (function!=fun && function->originalSignature() == fun->originalSignature() && !function->isRemovedFromAllLanguages(this))
            returned.append(function);
    }

    return returned;
}

/*******************************************************************************
 * Returns all reference count modifications for any function in the class
 */
QList<ReferenceCount> MetaClass::referenceCounts() const {
    QList<ReferenceCount> returned;

    for(MetaFunction *function : this->functions()) {
        returned += function->referenceCounts(this);
    }

    return returned;
}

QList<TemplateInstantiation> MetaClass::templateInstantiations() const {
    QList<TemplateInstantiation> returned;

    for(MetaFunction *function : this->functions()) {
        returned += function->templateInstantiations(this);
    }

    return returned;
}

/*******************************************************************************
 * Returns a list of all the functions retrieved during parsing which should
 * be added to the Java API.
 */
MetaFunctionList MetaClass::functionsInTargetLang() const {
    uint default_flags = NormalFunctions | Visible | NotRemovedFromTargetLang;

    // Interfaces don't implement functions
    default_flags |= isInterface() ? 0 : ClassImplements;

    // Only public functions in final classes
    // default_flags |= isFinal() ? WasPublic : 0;
    uint public_flags = isFinal() ? WasPublic : 0;

    // Constructors
    MetaFunctionList returned = queryFunctions(Constructors | default_flags | public_flags);

    // Final functions
    returned += queryFunctions(FinalInTargetLangFunctions | NonStaticFunctions | default_flags | public_flags);

    // Virtual functions
    returned += queryFunctions(VirtualInTargetLangFunctions | NonStaticFunctions | default_flags | public_flags);

    // Static functions
    returned += queryFunctions(StaticFunctions | default_flags | public_flags);

    // Empty, private functions, since they aren't caught by the other ones
    returned += queryFunctions(Empty | Invisible);

    return returned;
}

MetaFunctionList MetaClass::implementableFunctions() const {
    MetaFunctionList list = functionsInShellClass();

    MetaFunctionList returned;
    for(MetaFunction *f : list) {
        if (!f->isRemovedFrom(f->declaringClass(), TS::TargetLangCode)
                && (!f->isFinalInCpp() || f->isVirtualSlot()))
            returned += f;
    }

    return returned;
}

MetaFunctionList MetaClass::virtualFunctions() const {
    MetaFunctionList list = functionsInShellClass();

    MetaFunctionList returned;
    for(MetaFunction *f : list) {
        if (!f->isFinalInCpp() || f->isVirtualSlot())
            returned += f;
    }

    return returned;
}

MetaFunctionList MetaClass::nonVirtualShellFunctions() const {
    MetaFunctionList list = functionsInShellClass();
    MetaFunctionList returned;
    for(MetaFunction *f : list) {
        if (f->isFinalInCpp() && !f->isVirtualSlot())
            returned += f;
    }

    return returned;
}

/*******************************************************************************
 * Returns a list of all functions that should be declared and implemented in
 * the shell class which is generated as a wrapper on top of the actual C++ class
 */
MetaFunctionList MetaClass::functionsInShellClass() const {
    // Only functions and only protected and public functions
    uint default_flags = NormalFunctions | Visible | WasVisible | NotRemovedFromShell;

    // All virtual functions
    MetaFunctionList returned = queryFunctions(VirtualFunctions | default_flags);

    // All functions explicitly set to be implemented by the shell class
    // (mainly superclass functions that are hidden by other declarations)
    returned += queryFunctions(ForcedShellFunctions | default_flags);

    // All functions explicitly set to be virtual slots
    returned += queryFunctions(VirtualSlots | default_flags);

    return returned;
}

/*******************************************************************************
 * Returns a list of all functions that require a public override function to
 * be generated in the shell class. This includes all functions that were originally
 * protected in the superclass.
 */
MetaFunctionList MetaClass::publicOverrideFunctions() const {
    return queryFunctions(NormalFunctions | WasProtected | FinalInCppFunctions | NotRemovedFromTargetLang)
           + queryFunctions(Signals | WasProtected | FinalInCppFunctions | NotRemovedFromTargetLang);
}

MetaFunctionList MetaClass::virtualOverrideFunctions() const {
    return queryFunctions(NormalFunctions | NonEmptyFunctions | Visible | VirtualInCppFunctions | NotRemovedFromShell) +
           queryFunctions(Signals | NonEmptyFunctions | Visible | VirtualInCppFunctions | NotRemovedFromShell);
}

bool function_sorter(MetaFunction *a, MetaFunction *b){
    return a->signature() < b->signature();
};

void MetaClass::sortFunctions() {
    std::sort(m_functions.begin(), m_functions.end(), &function_sorter);
    std::sort(m_invalidfunctions.begin(), m_invalidfunctions.end(), &function_sorter);
}

void MetaClass::sortEnums() {
    std::sort(m_enums.begin(), m_enums.end(), [](MetaEnum *a, MetaEnum *b) -> bool {
        return a->name() < b->name();
    });
}

void MetaClass::sortFields() {
    std::sort(m_fields.begin(), m_fields.end(), [](MetaField *a, MetaField *b) -> bool {
        return a->name() < b->name();
    });
}

void MetaClass::sortFunctionals() {
    std::sort(m_functionals.begin(), m_functionals.end(), [](MetaFunctional *a, MetaFunctional *b) -> bool {
        return a->name() < b->name();
    });
}

const ContainerTypeEntry* MetaClass::findContainerSuperClass(QList<const MetaType *>* instantiations) const{
    if(templateBaseClass() && templateBaseClass()->typeEntry()->isContainer()){
        const ContainerTypeEntry* ctype = static_cast<const ContainerTypeEntry*>(templateBaseClass()->typeEntry());
        switch(ctype->type()){
        case ContainerTypeEntry::MultiMapContainer:
        case ContainerTypeEntry::MapContainer:
        case ContainerTypeEntry::ListContainer:
        case ContainerTypeEntry::LinkedListContainer:
        case ContainerTypeEntry::VectorContainer:
        case ContainerTypeEntry::StackContainer:
        case ContainerTypeEntry::QueueContainer:
        case ContainerTypeEntry::SetContainer:
        case ContainerTypeEntry::HashContainer:
        case ContainerTypeEntry::MultiHashContainer:
            if(instantiations){
                (*instantiations) << templateBaseClassInstantiations();
            }
            return ctype;
        default:
            break;
        }
    }
    if(baseClass())
        return baseClass()->findContainerSuperClass(instantiations);
    return nullptr;
}

void MetaClass::setBaseClass(MetaClass *base_class) {
    m_base_class = base_class;
    if(base_class){
        base_class->m_has_subClasses = true;
        if(m_usingProtectedBaseConstructors || m_usingPublicBaseConstructors){
            for(const MetaFunction* fun : base_class->functions()){
                if(fun->isConstructor()){
                    MetaFunction* constructor = fun->copy();
                    constructor->setImplementingClass(this);
                    constructor->setDeclaringClass(this);
                    constructor->setName(qualifiedCppName().split("::").last());
                    constructor->setOriginalName(constructor->name());
                    if(m_usingPublicBaseConstructors){
                        constructor->setVisibility(MetaAttributes::Public);
                    }else{
                        constructor->setVisibility(MetaAttributes::Protected);
                    }
                    addFunction(constructor);
                }
            }
        }
    }
}

void MetaClass::setFunctions(const MetaFunctionList &functions) {
    m_functions = functions;
    sortFunctions();

    // Functions must be sorted by name before next loop
    QString currentName;
    bool hasVirtuals = false;
    MetaFunctionList final_functions;
    for(MetaFunction *f : m_functions) {
        f->setOwnerClass(this);

        m_has_virtual_slots |= f->isVirtualSlot();
        m_has_virtuals |= !f->isFinal() || f->isVirtualSlot();
        m_has_nonpublic |= !f->isPublic();

        // If we have non-virtual overloads of a virtual function, we have to implement
        // all the overloads in the shell class to override the hiding rule
        if (currentName == f->name()) {
            hasVirtuals = hasVirtuals || !f->isFinal();
            if (f->isFinal())
                final_functions += f;
        } else {
            if (hasVirtuals && final_functions.size() > 0) {
                for(MetaFunction *final_function : final_functions) {
                    *final_function += MetaAttributes::ForceShellImplementation;

                    QString warn = QString("hiding of function '%1' in class '%2'")
                                   .arg(final_function->name(), name());
                    ReportHandler::warning(warn);
                }
            }

            hasVirtuals = !f->isFinal();
            final_functions.clear();
            if (f->isFinal())
                final_functions += f;
            currentName = f->name();
        }
        if((this->isQObject() || this->m_has_Q_GADGET) && f->name()==QLatin1String("metaObject") && f->arguments().isEmpty()){
            m_has_metaObject = true;
            m_has_private_metaObject = f->wasPrivate();
            *f += MetaAttributes::Private;
        }else if((this->isQObject() || this->m_has_Q_GADGET)
                 && f->name()==QLatin1String("qt_metacall")
                 && f->arguments().size()==3
                 && f->arguments()[0]->type()->cppSignature()==QLatin1String("QMetaObject::Call")
                 && f->arguments()[1]->type()->cppSignature()==QLatin1String("int")
                 && f->arguments()[2]->type()->cppSignature()==QLatin1String("void **")){
            m_has_metacall = true;
            m_has_private_metacall = f->wasPrivate();
            *f += MetaAttributes::Private;
        }else if((this->isQObject() || this->m_has_Q_GADGET)
                 && f->name()==QLatin1String("qt_metacast")
                 && f->arguments().size()==1
                 && f->arguments()[0]->type()->cppSignature()==QLatin1String("const char *")){
            m_has_metacast = true;
            m_has_private_metacast = f->wasPrivate();
            *f += MetaAttributes::Private;
        }else{
            if(f->type() && f->type()->isNativePointer() && f->type()->typeEntry()->isValue() && f->valueAsPointer(0)){
                f->type()->setTypeUsagePattern(MetaType::ObjectPattern);
            }
            for(MetaArgument* arg : f->arguments()){
                if(arg->type()->isNativePointer() && arg->type()->typeEntry()->isValue() && f->valueAsPointer(arg->argumentIndex()+1)){
                    arg->type()->setTypeUsagePattern(MetaType::ObjectPattern);
                }
            }
        }
    }

#ifndef QT_NO_DEBUG
    //bool duplicate_function = false;
    for (int j = 0; j < m_functions.size(); ++j) {
        FunctionModificationList mods = m_functions.at(j)->modifications(m_functions.at(j)->implementingClass());

        bool removed = false;
        for(const FunctionModification &mod : mods) {
            if (mod.isRemoveModifier()) {
                removed = true;
                break ;
            }
        }
        if (removed)
            continue ;

        if(!m_functions.at(j)->templateParameters().isEmpty())
            continue;

        if(!m_functions.at(j)->hasRReferences())
            continue;

        for (int i = 0; i < m_functions.size() - 1; ++i) {
            if (j == i)
                continue;

            if(!m_functions.at(i)->templateParameters().isEmpty())
                continue;

            if(!m_functions.at(i)->hasRReferences())
                continue;

            mods = m_functions.at(i)->modifications(m_functions.at(i)->implementingClass());
            bool removed = false;
            for(const FunctionModification &mod : mods) {
                if (mod.isRemoveModifier()) {
                    removed = true;
                    break ;
                }
            }
            if (removed)
                continue ;

            uint cmp = m_functions.at(i)->compareTo(m_functions.at(j));
            if ((cmp & MetaFunction::EqualName) && (cmp & MetaFunction::EqualArguments)) {
                ReportHandler::warning(QString("%1.%2 mostly equal to %3.%4\n").arg(
                       m_functions.at(i)->implementingClass()->typeEntry()->qualifiedCppName(),
                       m_functions.at(i)->signature(),
                       m_functions.at(j)->implementingClass()->typeEntry()->qualifiedCppName(),
                       m_functions.at(j)->signature()));
                //duplicate_function = true;
            }
        }
    }
    //Q_ASSERT(!duplicate_function);
#endif
}

bool MetaClass::hasHashFunction() const {
    if(m_template_base_class && m_template_base_class->hasHashFunction()){
        if(!m_template_base_class->typeEntry()->isContainer()){
            return true;
        }else if(!m_template_base_class_instantiations.isEmpty()){
            bool has_hash = true;
            for(auto instantiation : m_template_base_class_instantiations){
                if(instantiation->indirections().isEmpty()){
                    if(instantiation->typeEntry()->isVariant()){
                        has_hash = false;
                    }
                }
            }
            return has_hash;
        }
    }
    return m_has_hash_function;
}

bool MetaClass::hasEqualsOperator() const {
    if(m_template_base_class && m_template_base_class->hasEqualsOperator()){
        if(!m_template_base_class->typeEntry()->isContainer()){
            return true;
        }else if(!m_template_base_class_instantiations.isEmpty()){
            bool has_eq = true;
            for(auto instantiation : m_template_base_class_instantiations){
                if(instantiation->indirections().isEmpty()){
                    if(instantiation->typeEntry()->isVariant()){
                        has_eq = false;
                    }
                }
            }
            return has_eq;
        }
    }
    return m_has_equals_operator;
}

bool MetaClass::hasCloneOperator() const {
    if(m_template_base_class && m_template_base_class->hasCloneOperator())
        return true;
    return m_has_clone_operator;
}

bool MetaClass::hasProtectedFieldAccessors() const {
    for(const MetaField *field : fields()) {
        if (!field->wasPublic() && (field->getter() || field->setter()))
            return true;
    }

    return false;
}

bool MetaClass::hasDefaultToStringFunction() const {
    for(MetaFunction *f : queryFunctionsByName("toString")) {
        if (f->actualMinimumArgumentCount() == 0) {
            return true;
        }

    }
    return false;
}

void MetaClass::addFunction(MetaFunction *function) {
    function->setOwnerClass(this);

    if (!function->isDestructor()){
        m_functions << function;
        if(function->type() && function->type()->isNativePointer() && function->type()->typeEntry()->isValue() && function->valueAsPointer(0)){
            function->type()->setTypeUsagePattern(MetaType::ObjectPattern);
        }
        for(MetaArgument* arg : function->arguments()){
            if(arg->type()->isNativePointer() && arg->type()->typeEntry()->isValue() && function->valueAsPointer(arg->argumentIndex()+1)){
                arg->type()->setTypeUsagePattern(MetaType::ObjectPattern);
            }
        }
        sortFunctions();
    }

    m_has_virtual_slots |= function->isVirtualSlot();
    m_has_virtuals |= !function->isFinal() || function->isVirtualSlot();
    m_has_nonpublic |= !function->isPublic();
    if(function->name()=="metaObject" && function->arguments().isEmpty() && function->wasPrivate()){
        m_has_private_metaObject = true;
    }else if(function->name()=="qt_metacall"
             && function->arguments().size()==3
             && function->wasPrivate()
             && function->arguments()[0]->type()->cppSignature()=="QMetaObject::Call"
             && function->arguments()[1]->type()->cppSignature()=="int"
             && function->arguments()[2]->type()->cppSignature()=="void**"){
        m_has_private_metacall = true;
    }
}

void MetaClass::addInvalidFunction(MetaFunction *function) {
    function->setOwnerClass(this);

    if (!function->isDestructor()) {
        m_invalidfunctions << function;
    }
}

bool MetaClass::hasSignal(const MetaFunction *other) const {
    if (!other->isSignal())
        return false;

    for(const MetaFunction *f : m_functions) {
        if (f->isSignal() && f->compareTo(other) & MetaFunction::EqualName)
            return other->modifiedName() == f->modifiedName();
    }

    return false;
}


QString MetaClass::name() const {
    return QString(m_type_entry->targetLangName()).replace("::", "$");
}

QString MetaClass::simpleName() const {
    QString name = QString(m_type_entry->targetLangName());
    return name.remove(0, name.lastIndexOf('$')+1);
}

bool MetaClass::hasFunction(const QString &str) const {
    for(const MetaFunction *f : m_functions) {
        if (f->name() == str)
            return true;
    }
    return false;
}

/* Returns true if this class has one or more functions that are
   protected. If a class has protected members we need to generate a
   shell class with public accessors to the protected functions, so
   they can be called from the native functions.
*/
bool MetaClass::hasProtectedFunctions() const {
    for(const MetaFunction *func : m_functions) {
        if (func->wasProtected())
            return true;
    }
    return false;
}

bool MetaClass::hasProtectedConstructors() const {
    for(const MetaFunction *func : m_functions) {
        if (func->isConstructor() && func->wasProtected())
            return true;
    }
    return false;
}

bool MetaClass::instantiateShellClass() const {
    return !isFinal()
            //&& !m_has_private_metacall
            //&& !m_has_private_metaObject
            && !(typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)
            && (hasVirtualFunctions()
                || hasVirtualDestructor()
                || hasProtectedConstructors()
            //    || typeEntry()->designatedInterface()
            //    || typeEntry()->isInterface()
            );
}

bool MetaClass::generateShellClass() const {
    return !isFinal()
            //&& !m_has_private_metacall
            //&& !m_has_private_metaObject
            && !(typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)
            && (hasVirtualFunctions()
                || hasVirtualDestructor()
                || hasProtectedFunctions()
                || hasProtectedFieldAccessors()
            //    || typeEntry()->designatedInterface()
            //    || typeEntry()->isInterface()
            );
}

QPropertySpec *MetaClass::propertySpecForRead(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i)
        if (name == m_property_specs.at(i)->read())
            return m_property_specs.at(i);
    return nullptr;
}

QPropertySpec *MetaClass::propertySpecForBindable(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i)
        if (name == m_property_specs.at(i)->bindable())
            return m_property_specs.at(i);
    return nullptr;
}

QPropertySpec *MetaClass::propertySpecForWrite(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i)
        if (name == m_property_specs.at(i)->write())
            return m_property_specs.at(i);
    return nullptr;
}

QPropertySpec *MetaClass::propertySpecForReset(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i) {
        if (name == m_property_specs.at(i)->reset())
            return m_property_specs.at(i);
    }
    return nullptr;
}

QPropertySpec *MetaClass::propertySpecForNotify(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i) {
        if (name == m_property_specs.at(i)->notify())
            return m_property_specs.at(i);
    }
    return nullptr;
}

static bool functions_contains(const MetaFunctionList &l, const MetaFunction *func) {
    for(const MetaFunction *f : l) {
        if ((f->compareTo(func) & MetaFunction::PrettySimilar) == MetaFunction::PrettySimilar)
            return true;
    }
    return false;
}

MetaField::MetaField() :
    MetaVariable(), MetaAttributes(),
    m_getter(nullptr), m_setter(nullptr), m_class(nullptr) {
}

MetaField::~MetaField() {
    delete m_setter;
    delete m_getter;
}

MetaField *MetaField::copy() const {
    MetaField *returned = new MetaField;
    returned->setEnclosingClass(nullptr);
    returned->setAttributes(attributes());
    returned->setName(name());
    returned->setType(type()->copy());
    returned->setOriginalAttributes(originalAttributes());
    returned->setHref(href());
    returned->setBrief(brief());

    return returned;
}

static QString upCaseFirst(const QString &str) {
    Q_ASSERT(!str.isEmpty());
    QString s = str;
    s[0] = s.at(0).toUpper();
    return s;
}

static MetaFunction *createXetter(const MetaField *g, const QString &name, uint type) {
    MetaFunction *f = new MetaFunction;

    f->setBrief(g->brief());
    f->setHref(g->href());

    f->setName(name);
    f->setOriginalName(name);
    f->setOwnerClass(g->enclosingClass());
    f->setImplementingClass(g->enclosingClass());
    f->setDeclaringClass(g->enclosingClass());

    uint attr = MetaAttributes::Native
                | MetaAttributes::Final
                | type;
    if (g->isStatic())
        attr |= MetaAttributes::Static;
    if (g->isPublic())
        attr |= MetaAttributes::Public;
    else if (g->isProtected())
        attr |= MetaAttributes::Protected;
    else
        attr |= MetaAttributes::Private;
    f->setAttributes(attr);
    f->setOriginalAttributes(attr);

    FieldModificationList mods = g->modifications();
    for(const FieldModification& mod : mods) {
        if (mod.isRenameModifier())
            f->setName(mod.renamedTo());
        if (mod.isAccessModifier()) {
            if (mod.isPrivate())
                f->setVisibility(MetaAttributes::Private);
            else if (mod.isProtected())
                f->setVisibility(MetaAttributes::Protected);
            else if (mod.isPublic())
                f->setVisibility(MetaAttributes::Public);
            else if (mod.isFriendly())
                f->setVisibility(MetaAttributes::Friendly);
        }

    }

    f->setAccessedField(g);

    return f;
}

FieldModificationList MetaField::modifications() const {
    FieldModificationList mods = enclosingClass()->typeEntry()->fieldModifications();
    FieldModificationList returned;

    for(const FieldModification& mod : mods) {
        if (mod.name == name())
            returned += mod;
    }

    return returned;
}

const MetaFunction *MetaField::setter() const {
    if (!m_setter) {
        QString name = this->name();
        if(name.startsWith("m_")){
            name = name.mid(2);
        }
        Modification::Modifiers accessModifier = Modification::Modifiers(0);
        FieldModificationList modifications = MetaField::modifications();
        for(const FieldModification& modification : modifications) {
            if(!modification.renamedTo().isEmpty()){
                name = modification.renamedTo();
            }
            accessModifier = Modification::Modifiers(accessModifier & modification.accessModifier());
        }
        QString varName = name;
        QString newName = "set" + upCaseFirst(name);
        m_setter = createXetter(this,
                                newName,
                                MetaAttributes::SetterFunction);
        if(accessModifier & Modification::AccessModifierMask){
            m_setter->setVisibility(accessModifier & Modification::AccessModifierMask);
        }
        MetaArgumentList arguments;
        MetaArgument *argument = new MetaArgument;
        argument->setType(type()->copy());
        if(type()->getReferenceType()==MetaType::NoReference && !type()->isPrimitive() && type()->indirections().isEmpty()){
            argument->type()->setReferenceType(MetaType::Reference);
            argument->type()->setConstant(true);
        }
        MetaBuilder::decideUsagePattern(argument->type());
        argument->setName(varName);
        arguments.append(argument);
        m_setter->setArguments(arguments);
        m_setter->setName(newName);
        m_setter->setOriginalName(newName);
        m_setter->setOriginalSignature(this->name());
        if(m_class && (m_class->hasFunction(m_setter) || (attributes() & MetaAttributes::ConstExpr) || (isStatic() && type()->isConstant() && type()->indirections().isEmpty()))){
            bool success = false;
            for(FieldModification& mod : const_cast<FieldModificationList&>(m_class->typeEntry()->fieldModifications())){
                if(mod.name==this->name()){
                    mod.modifiers &= ~uint(Modification::Writable);
                    success = true;
                    break;
                }
            }
            if(!success){
                FieldModification mod;
                mod.name = this->name();
                mod.modifiers = FieldModification::Readable;
                const_cast<ComplexTypeEntry *>(m_class->typeEntry())->addFieldModification(mod);
            }
        }
    }
    return m_setter;
}

const MetaFunction *MetaField::getter() const {
    if (!m_getter) {
        QString name = this->name();
        if(name.startsWith("m_")){
            name = name.mid(2);
        }
        Modification::Modifiers accessModifier = Modification::Modifiers(0);
        FieldModificationList modifications = MetaField::modifications();
        for(const FieldModification& modification : modifications) {
            if(!modification.renamedTo().isEmpty()){
                name = modification.renamedTo();
            }
            accessModifier = Modification::Modifiers(accessModifier & modification.accessModifier());
        }
        m_getter = createXetter(this,
                                name,
                                MetaAttributes::GetterFunction);
        if(accessModifier & Modification::AccessModifierMask){
            m_getter->setVisibility(accessModifier & Modification::AccessModifierMask);
        }
        if((attributes() & MetaAttributes::ConstExpr || (isStatic() && type()->isConstant() && type()->indirections().isEmpty()))
                && (
                    type()->isPrimitive()
                    || type()->isEnum()
                    || type()->isTargetLangString()
                    )){
            m_getter->setVisibility(MetaAttributes::Private);
        }
        m_getter->setType(type()->copy());
        if(type()->getReferenceType()==MetaType::NoReference && !type()->isPrimitive() && type()->indirections().isEmpty()){
            m_getter->type()->setReferenceType(MetaType::Reference);
            m_getter->type()->setConstant(true);
        }
        MetaBuilder::decideUsagePattern(m_getter->type());
        m_getter->setName(name);
        m_getter->setOriginalName(name);
        m_getter->setOriginalSignature(this->name());
        if(m_class && m_class->hasFunction(m_getter)){
            bool success = false;
            for(FieldModification& mod : const_cast<FieldModificationList&>(m_class->typeEntry()->fieldModifications())){
                if(mod.name==this->name()){
                    mod.modifiers &= ~uint(Modification::Readable);
                    success = true;
                    break;
                }
            }
            if(!success){
                FieldModification mod;
                mod.name = this->name();
                mod.modifiers &= ~uint(Modification::Readable);
                const_cast<ComplexTypeEntry *>(m_class->typeEntry())->addFieldModification(mod);
            }
        }
    }

    return m_getter;
}


bool MetaClass::hasConstructors() const {
    return queryFunctions(Constructors).size() != 0;
}

bool MetaClass::hasStandardConstructor() const {
    if(m_has_standardconstructor==0){
        MetaFunctionList functions = queryFunctions(Constructors);
        m_has_standardconstructor = -1;
        for(MetaFunction *f : functions) {
            if ((f->wasPublic() || f->isPublic() || f->wasProtected() || f->isProtected()) && !f->isInvalid() && !f->isEmptyFunction() && !f->isFake() && f->implementingClass()==this && f->actualMinimumArgumentCount()==0 && !f->isRemovedFromAllLanguages(this)) {
                m_has_standardconstructor = 1;
            }
        }
    }
    return m_has_standardconstructor>0;
}

bool MetaClass::hasPublicStandardConstructor() const {
    if(m_has_publicstandardconstructor==0){
        MetaFunctionList functions = queryFunctions(Constructors);
        m_has_publicstandardconstructor = -1;
        for(MetaFunction *f : functions) {
            if ((f->wasPublic() || f->isPublic())
                    && !f->isInvalid()
                    && !f->isEmptyFunction()
                    && !f->isFake()
                    && f->implementingClass()==this
                    && f->actualMinimumArgumentCount()==0
                    && !f->isRemovedFromAllLanguages(this)) {
                m_has_publicstandardconstructor = 1;
            }
        }
    }
    return m_has_publicstandardconstructor>0;
}

bool MetaClass::hasExplicitStandardConstructor() const {
    if(m_has_explicitstandardconstructor==0){
        MetaFunctionList functions = queryFunctions(Constructors);
        m_has_explicitstandardconstructor = -1;
        for(MetaFunction *f : functions) {
            if (f->isExplicit()
                    && !f->isInvalid()
                    && !f->isEmptyFunction()
                    && !f->isFake()
                    && f->implementingClass()==this
                    && f->actualMinimumArgumentCount()==0) {
                m_has_explicitstandardconstructor = 1;
            }
        }
    }
    return m_has_explicitstandardconstructor>0;
}

bool MetaClass::hasPublicCopyConstructor() const {
    if(m_has_publiccopyconstructor==0){
        MetaFunctionList functions = queryFunctions(Constructors);
        m_has_publiccopyconstructor = -1;
        for(MetaFunction *f : functions) {
            if ((f->wasPublic() || f->isPublic())
                    && !f->isInvalid()
                    && !f->isEmptyFunction() && !f->isFake()
                    && f->implementingClass()==this
                    && f->actualMinimumArgumentCount()==1
                    && f->arguments()[0]->type()->isConstant()
                    && f->arguments()[0]->type()->getReferenceType()==MetaType::Reference
                    && f->arguments()[0]->type()->typeEntry()==typeEntry()) {
                m_has_publiccopyconstructor = 1;
            }
        }
    }
    return m_has_publiccopyconstructor>0;
}

bool MetaClass::hasPublicAssignment() const {
    if(m_has_publicassignment==0){
        MetaFunctionList functions = queryFunctionsByName("operator=");
        m_has_publicassignment = -1;
        for(MetaFunction *f : functions) {
            if (!f->isInvalid()
                && !f->isEmptyFunction() && !f->isFake()
                && f->implementingClass()==this
                && f->actualMinimumArgumentCount()==1
                && f->arguments()[0]->type()->isConstant()
                && f->arguments()[0]->type()->getReferenceType()==MetaType::Reference
                && f->arguments()[0]->type()->typeEntry()==typeEntry()) {
                m_has_publicassignment = 1;
            }
        }
    }
    return m_has_publicassignment>0;
}

bool MetaClass::hasExplicitCopyConstructor() const {
    if(m_has_explicitcopyconstructor==0){
        MetaFunctionList functions = queryFunctions(Constructors);
        m_has_explicitcopyconstructor = -1;
        for(MetaFunction *f : functions) {
            if (f->isExplicit()
                    && !f->isInvalid()
                    && !f->isEmptyFunction() && !f->isFake()
                    && f->implementingClass()==this
                    && f->actualMinimumArgumentCount()==1
                    && f->arguments()[0]->type()->isConstant()
                    && f->arguments()[0]->type()->getReferenceType()==MetaType::Reference
                    && f->arguments()[0]->type()->typeEntry()==typeEntry()) {
                m_has_explicitcopyconstructor = 1;
            }
        }
    }
    return m_has_explicitcopyconstructor>0;
}

void MetaClass::addDefaultConstructor() {
    FunctionModificationList result = typeEntry()->functionModifications(qualifiedCppName()+"()");
    for(FunctionModification mod : result){
        if(mod.isRemoveModifier())
            return;
    }
    MetaFunction *f = new MetaFunction;
    f->setName(simpleName());
    f->setOriginalName(qualifiedCppName().split("::").last());
    f->setOwnerClass(this);
    f->setFunctionType(MetaFunction::ConstructorFunction);
    f->setArguments(MetaArgumentList());
    f->setDeclaringClass(this);

    uint attr = MetaAttributes::Native | MetaAttributes::Final;
    if(hasJustPrivateConstructors()){
        attr |= MetaAttributes::Private;
    }else{
        attr |= MetaAttributes::Public;
    }
    f->setAttributes(attr);
    f->setImplementingClass(this);
    f->setOriginalAttributes(f->attributes());

    addFunction(f);
}

bool MetaClass::hasFunction(const MetaFunction *f) const {
    return functions_contains(m_functions, f);
}

/* Goes through the list of functions and returns a list of all
   functions matching all of the criteria in \a query.
 */

MetaFunctionList MetaClass::queryFunctions(uint query) const {
    MetaFunctionList functions;

    for(MetaFunction *f : m_functions) {
        if ((query & VirtualSlots) && !f->isVirtualSlot())
            continue;

        if ((query & NotRemovedFromTargetLang) && f->isRemovedFrom(f->implementingClass(), TS::TargetLangCode)) {
            continue;
        }

        if ((query & NotRemovedFromTargetLang) && !f->isFinal() && f->isRemovedFrom(f->declaringClass(), TS::TargetLangCode)) {
            continue;
        }

        if ((query & NotRemovedFromShell) && f->isRemovedFrom(f->implementingClass(), TS::ShellCode)) {
            continue;
        }

        if ((query & NotRemovedFromShell) && !f->isFinal() && f->isRemovedFrom(f->declaringClass(), TS::ShellCode)) {
            continue;
        }

        if ((query & Visible) && f->isPrivate() && !f->isAbstract()) {
            continue;
        }

        if ((query & VirtualInTargetLangFunctions) && f->isFinalInTargetLang()) {
            continue;
        }

        if ((query & Invisible) && !f->isPrivate()) {
            continue;
        }

        if ((query & Empty) && !f->isEmptyFunction()) {
            continue;
        }

        if ((query & WasPublic) && !f->wasPublic()) {
            continue;
        }

        if ((query & WasVisible) && f->wasPrivate() && !f->isAbstract()) {
            continue;
        }

        if ((query & WasProtected) && !f->wasProtected()) {
            continue;
        }

        if ((query & ClassImplements) && f->ownerClass() != f->implementingClass()) {
            continue;
        }

        if ((query & Inconsistent) && (f->isFinalInTargetLang() || !f->isFinalInCpp() || f->isStatic())) {
            continue;
        }

        if ((query & FinalInTargetLangFunctions) && !f->isFinalInTargetLang()) {
            continue;
        }

        if ((query & FinalInCppFunctions) && !f->isFinalInCpp()) {
            continue;
        }

        if ((query & VirtualInCppFunctions) && f->isFinalInCpp()) {
            continue;
        }

        if ((query & Signals) && (!f->isSignal())) {
            continue;
        }

        if ((query & ForcedShellFunctions)
                && (!f->isForcedShellImplementation()
                    || !f->isFinal())) {
            continue;
        }

        if (((query & Constructors) && (!f->isConstructor() || f->ownerClass() != f->implementingClass())) || 
                (f->isConstructor() && (query & Constructors) == 0)) {
            continue;
        }

        // Destructors are never included in the functions of a class currently
        /*
           if ((query & Destructors) && (!f->isDestructor()
                                       || f->ownerClass() != f->implementingClass())
            || f->isDestructor() && (query & Destructors) == 0) {
            continue;
        }*/

        if ((query & VirtualFunctions) && (f->isFinal() || f->isSignal() || f->isStatic())) {
            continue;
        }

        if ((query & StaticFunctions) && (!f->isStatic() || f->isSignal())) {
            continue;
        }

        if ((query & NonStaticFunctions) && (f->isStatic())) {
            continue;
        }

        if ((query & NonEmptyFunctions) && (f->isEmptyFunction())) {
            continue;
        }

        if ((query & NormalFunctions) && (f->isSignal())) {
            continue;
        }

        if ((query & AbstractFunctions) && !f->isAbstract()) {
            continue;
        }

        functions << f;
    }

//    qDebug() << "queried" << m_type_entry->qualifiedCppName() << "got" << functions.size() << "out of" << m_functions.size();

    return functions;
}

bool MetaClass::hasSignals() const {
    return cppSignalFunctions().size() > 0;
}

QString MetaClass::fullQualifiedCppName() const {
    if(this->isTypeAlias() && m_template_base_class && !m_template_base_class_instantiations.isEmpty()){
        QString instantiations = "<";
        for(int i=0; i<m_template_base_class_instantiations.size(); ++i){
            if(i>0)
                instantiations += ", ";
            instantiations += m_template_base_class_instantiations[i]->minimalSignature();
        }
        instantiations += ">";
        return m_template_base_class->qualifiedCppName() + instantiations;
    }
    return m_type_entry->qualifiedCppName();
}


/**
 * Adds the specified interface to this class by adding all the
 * functions in the interface to this class.
 */
void MetaClass::addInterface(MetaClass *interface) {
    Q_ASSERT(interface->isInterface());
    Q_ASSERT(!m_interfaces.contains(interface));
    m_interfaces << interface;

    if (m_extracted_interface && m_extracted_interface != interface)
        m_extracted_interface->addInterface(interface);
    if (interface!=this && m_extracted_interface_impl && !m_extracted_interface_impl->interfaces().contains(interface))
        m_extracted_interface_impl->addInterface(interface);

    for(MetaFunction *function : interface->functions()) {
        if (!hasFunction(function) && !function->isConstructor()) {
            MetaFunction *cpy = function->copy();

            // Setup that this function is an interface class.
            cpy->setInterfaceClass(interface);
            *cpy += MetaAttributes::InterfaceFunction;

            // Copy the modifications in interface into the implementing classes.
            FunctionModificationList mods = function->modifications(interface);
            for(const FunctionModification &mod : mods) {
                m_type_entry->addFunctionModification(mod);
            }

            // It should be mostly safe to assume that when we implement an interface
            // we don't "pass on" pure virtual functions to our sublcasses...
//                 *cpy -= AbstractMetaAttributes::Abstract;

            cpy->setImplementingClass(this);
            addFunction(cpy);
        }
    }
}


void MetaClass::setInterfaces(const MetaClassList &interfaces) {
    m_interfaces = interfaces;
}

/**
 * Adds the specified nested class to this class.
 */
void MetaClass::addEnclosedClass(MetaClass *enclosed_class) {
    Q_ASSERT(!m_enclosed_classes.contains(enclosed_class));
    m_enclosed_classes << enclosed_class;
    enclosed_class->setEnclosingClass(this);
}


void MetaClass::setEnclosedClasses(const MetaClassList &enclosed_classes) {
    m_enclosed_classes = enclosed_classes;
}


MetaEnum *MetaClass::findEnum(const QString &enumName) {
    for(MetaEnum *e : m_enums) {
        if (e->name() == enumName)
            return e;
    }

    if (typeEntry()->designatedInterface()){
        for(MetaEnum *e : extractInterface()->m_enums) {
            if (e->name() == enumName)
                return e;
        }
    }
    if (typeEntry()->isInterface()){
        for(MetaEnum *e : extractInterfaceImpl()->m_enums) {
            if (e->name() == enumName)
                return e;
        }
    }
    return nullptr;
}

MetaClass *MetaClass::findIterator(const IteratorTypeEntry* iteratorType) const{
    for(MetaClass * enclosed_class : m_enclosed_classes){
        if(enclosed_class->typeEntry() == iteratorType){
            return enclosed_class;
        }
    }
    if(this->baseClass())
        return baseClass()->findIterator(iteratorType);
    if(this->templateBaseClass()){
        return templateBaseClass()->findIterator(iteratorType);
    }
    return nullptr;
}

MetaFunctional *MetaClass::findFunctional(const QString &functionalName) {
    for(MetaFunctional *f : m_functionals) {
        if (f->name() == functionalName)
            return f;
    }

    if (typeEntry()->isInterface())
        return extractInterfaceImpl()->findFunctional(functionalName);

    return nullptr;
}




/*!  Recursivly searches for the enum value named \a enumValueName in
  this class and its superclasses and interfaces. Values belonging to
  \a meta_enum are excluded from the search.
*/
MetaEnumValue *MetaClass::findEnumValue(const QString &enumValueName, MetaEnum *meta_enum) {
    for(MetaEnum *e : m_enums) {
        if (e == meta_enum)
            continue;
        for(MetaEnumValue *v : e->values()) {
            if (v->cppName() == enumValueName)
                return v;
        }
        if(e->typeEntry()->isScopedEnum()){
            for(MetaEnumValue *v : e->values()) {
                if (v->name() == enumValueName)
                    return v;
            }
        }
    }

    if (typeEntry()->isInterface())
        return extractInterfaceImpl()->findEnumValue(enumValueName, meta_enum);

    if (baseClass())
        return baseClass()->findEnumValue(enumValueName, meta_enum);

    return nullptr;
}


/*!
 * Searches through all of this class' enums for a value matching the
 * name \a enumValueName. The name is excluding the class/namespace
 * prefix. The function recursivly searches interfaces and baseclasses
 * of this class.
 */
MetaEnum *MetaClass::findEnumForValue(const QString &enumValueName) {
    for(MetaEnum *e : m_enums) {
        for(MetaEnumValue *v : e->values()) {
            if (v->cppName() == enumValueName)
                return e;
        }
    }

    if (typeEntry()->isInterface())
        return extractInterfaceImpl()->findEnumForValue(enumValueName);

    if (baseClass())
        return baseClass()->findEnumForValue(enumValueName);

    return nullptr;
}

QString MetaEnumValue::cppName() const {
    return m_enum && m_enum->typeEntry()->isScopedEnum() ? m_enum->typeEntry()->name() + "::" + m_name : m_name;
}

void add_extra_include_for_type(MetaClass *meta_class, const MetaType *type) {

    if (!type)
        return;

    Q_ASSERT(meta_class);
    const TypeEntry *entry = (type ? type->typeEntry() : nullptr);
    if (entry && entry->isComplex()) {
        const ComplexTypeEntry *centry = static_cast<const ComplexTypeEntry *>(entry);
        ComplexTypeEntry *class_entry = meta_class->typeEntry();
        if (class_entry && centry->include().isValid())
            class_entry->addExtraInclude(centry->include());
    }

    if (type->hasInstantiations()) {
        for(const MetaType *instantiation : type->instantiations())
            add_extra_include_for_type(meta_class, instantiation);
    }
}

void add_extra_includes_for_function(MetaClass *meta_class, const MetaFunction *meta_function) {
    Q_ASSERT(meta_class);
    Q_ASSERT(meta_function);
    add_extra_include_for_type(meta_class, meta_function->type());

    for(MetaArgument *argument : meta_function->arguments())
        add_extra_include_for_type(meta_class, argument->type());
}

void MetaClass::fixUnimplmentablePureVirtualFunctions(){
    this->m_has_unimplmentablePureVirtualFunctions = !getAllUnimplmentablePureVirtualFunctions().isEmpty();
}

void MetaClass::fixFunctions(std::function<MetaArgument*(TypeEntry *)> argumentCreator) {
    if (m_functions_fixed)
        return;
    else
        m_functions_fixed = true;

    MetaClass *super_class = baseClass();
    MetaFunctionList funcs = functions();

//     printf("fix functions for %s\n", qPrintable(name()));

    if (super_class)
        super_class->fixFunctions(argumentCreator);
    int iface_idx = 0;

    while (super_class || iface_idx < interfaces().size()) {
//         printf(" - base: %s\n", qPrintable(super_class->name()));

        // Since we always traverse the complete hierarchy we are only
        // interrested in what each super class implements, not what
        // we may have propagated from their base classes again.
        MetaFunctionList super_funcs;
        if (super_class) {
            bool pullDown = super_class->isPullProtectedMethodsDown();
            // Super classes can never be final
            if (super_class->isFinalInTargetLang()) {
                ReportHandler::warning("Final class '" + super_class->name() + "' set to non-final, as it is extended by other classes");
                *super_class -= MetaAttributes::FinalInTargetLang;
                *super_class += MetaAttributes::PullProtectedMethodsDown;
                pullDown = true;
            }
            if(pullDown){
                // when a class cannot be subclassed from external but has subclasses inside Qt,
                // the protected functions of the class must be pulled down to the subclasses.
                super_funcs = super_class->queryFunctions(MetaClass::Protected | MetaClass::NormalFunctions);
                for (int sfi = 0; sfi < super_funcs.size(); ++sfi) {
                    MetaFunction *sf = super_funcs.at(sfi);
                    if(!sf->isAbstract() && !sf->isConstructor() && !sf->isEmptyFunction() && !sf->isInvalid()){
                        MetaFunction *sfcopy = sf->copy();
                        sfcopy->setDeclaringClass(this);
                        this->addFunction(sfcopy);
                    }
                }
            }
            super_funcs = super_class->queryFunctions(MetaClass::ClassImplements);
        } else {
            super_funcs = interfaces().at(iface_idx)->queryFunctions(MetaClass::NormalFunctions);
        }

        QSet<MetaFunction *> funcs_to_add;
        for (int sfi = 0; sfi < super_funcs.size(); ++sfi) {
            MetaFunction *sf = super_funcs.at(sfi);

            if (sf->isRemovedFromAllLanguages(sf->implementingClass()))
                continue;

            // we generally don't care about private functions, but we have to get the ones that are
            // virtual in case they override abstract functions.
            bool add = (sf->isNormal() || sf->isSignal() || sf->isEmptyFunction());
            for (int fi = 0; fi < funcs.size(); ++fi) {
                MetaFunction *f = funcs.at(fi);
                if (f->isRemovedFromAllLanguages(f->implementingClass()))
                    continue;

                const uint cmp = f->compareTo(sf);

                if (cmp & MetaFunction::EqualModifiedName) {
//                     printf("   - %s::%s similar to %s::%s %x vs %x\n",
//                            qPrintable(sf->implementingClass()->typeEntry()->qualifiedCppName()),
//                            qPrintable(sf->name()),
//                            qPrintable(f->implementingClass()->typeEntry()->qualifiedCppName()),
//                            qPrintable(f->name()),
//                            sf->attributes(),
//                            f->attributes());

                    add = sf->isSignal();
                    if (cmp & MetaFunction::EqualArguments) {

//                         if (!(cmp & AbstractMetaFunction::EqualReturnType)) {
//                             ReportHandler::warning(QString("%1::%2 and %3::%4 differ in retur type")
//                                                    .arg(sf->implementingClass()->name())
//                                                    .arg(sf->name())
//                                                    .arg(f->implementingClass()->name())
//                                                    .arg(f->name()));
//                         }

                        // Same function, propegate virtual...
                        if (!(cmp & MetaFunction::EqualAttributes)) {
                            if (!f->isEmptyFunction()) {
                                if (!sf->isFinalInCpp() && f->isFinalInCpp() && !f->isDeclaredFinalInCpp()) {
                                    *f -= MetaAttributes::FinalInCpp;
                                    //                                 printf("   --- inherit virtual\n");
                                }
                                if (!sf->isFinalInTargetLang() && f->isFinalInTargetLang() && !f->isDeclaredFinalInCpp()) {
                                    *f -= MetaAttributes::FinalInTargetLang;
                                    //                                 printf("   --- inherit virtual\n");
                                }
                                if (!f->isFinalInTargetLang() && f->isPrivate() && !sf->isPrivate()) {
                                    f->setFunctionType(MetaFunction::EmptyFunction);
                                    f->setVisibility(MetaAttributes::Protected);
                                    *f += MetaAttributes::FinalInTargetLang;
                                    ReportHandler::warning(QString("private virtual function '%1' in '%2'")
                                                           .arg(f->signature())
                                                           .arg(f->implementingClass()->name()));
                                }
                            }
                        }

                        if (f->visibility() != sf->visibility()) {
                            QString warn = QString("visibility of function '%1' modified in class '%2'")
                                           .arg(f->name()).arg(name());
                            ReportHandler::warning(warn);

                            // If new visibility is private, we can't
                            // do anything. If it isn't, then we
                            // prefer the parent class's visibility
                            // setting for the function.
                            if (!f->isPrivate() && !sf->isPrivate())
                                f->setVisibility(sf->visibility());

                            // Private overrides of abstract functions have to go into the class or
                            // the subclasses will not compile as non-abstract classes.
                            // But they don't need to be implemented, since they can never be called.
                            if (f->isPrivate() && sf->isAbstract()) {
                                f->setFunctionType(MetaFunction::EmptyFunction);
                                f->setVisibility(sf->visibility());
                                *f += MetaAttributes::FinalInTargetLang;
                                *f += MetaAttributes::FinalInCpp;
                            }
                        }

                        // Set the class which first declares this function, afawk
                        f->setDeclaringClass(sf->declaringClass());

                        if (sf->isFinalInTargetLang() && !sf->isPrivate() && !f->isPrivate() && !sf->isStatic() && !f->isStatic() && !f->isSignal()) {
                            // Shadowed funcion, need to make base class
                            // function non-virtual
                            if (f->implementingClass() != sf->implementingClass() && f->implementingClass()->inheritsFrom(sf->implementingClass())) {

                                // Check whether the superclass method has been redefined to non-final

                                bool hasNonFinalModifier = false;
                                bool isBaseImplPrivate = false;
                                FunctionModificationList mods = sf->modifications(sf->implementingClass());
                                for(const FunctionModification& mod : mods) {
                                    if (mod.isNonFinal()) {
                                        hasNonFinalModifier = true;
                                        break;
                                    } else if (mod.isPrivate()) {
                                        isBaseImplPrivate = true;
                                        break;
                                    }
                                }

                                QPair<QMap<int,ArgumentModification>,QList<ArgumentModification>> addedArguments;
                                if (!hasNonFinalModifier
                                        && !isBaseImplPrivate
                                        && ( addedArguments = f->addedArguments() ).first.isEmpty()
                                        && addedArguments.second.isEmpty()
                                        && (f->implementingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)
                                        && (sf->implementingClass()->typeEntry()->codeGeneration() & TypeEntry::GenerateTargetLang)) {
                                    ReportHandler::warning(QString::fromLatin1("Shadowing: %1::%2 and %3::%4; Java code will not compile")
                                                           .arg(sf->implementingClass()->qualifiedCppName())
                                                           .arg(sf->signature())
                                                           .arg(f->implementingClass()->qualifiedCppName())
                                                           .arg(f->signature()));
                                }
                            }
                        }

                    }

                    if (!f->isPrivate() && cmp & MetaFunction::EqualDefaultValueOverload) {
                        const MetaArgumentList& arguments = f->arguments().size() < sf->arguments().size() ? sf->arguments() : f->arguments();

                        for (int i = 0; i < arguments.size(); ++i)
                            arguments[i]->setDefaultValueExpression(QString());
                    }


                    // Otherwise we have function shadowing and we can
                    // skip the thing...
                } else if (cmp & MetaFunction::EqualName && !sf->isSignal()) {

                    // In the case of function shadowing where the function name has been altered to
                    // avoid conflict, we don't copy in the original.
                    add = false;
                }

                // this feature is necessary because
                // Q_DECL_FINAL (meaning the shell must not override the method)
                // is not yet recognized by generator.
                // Example: Qt3D::QColorMask::copy(const Qt3D::QNode *) Q_DECL_FINAL

                FunctionModificationList mods = f->modifications(this);
                for(const FunctionModification& mod : mods) {
                    if (mod.isDeclaredFinal()) {
                        *f += MetaAttributes::FinalInTargetLang;
                        *f += MetaAttributes::FinalInCpp;
//                        ReportHandler::warning(QString("final overriding method %1 in class %2").arg(f->signature()).arg(f->implementingClass()->qualifiedCppName()));
                        break;
                    }
                }
            }

            if (add)
                funcs_to_add << sf;
        }

        for(MetaFunction *f : funcs_to_add)
            funcs << f->copy();

        if (super_class)
            super_class = super_class->baseClass();
        else
            iface_idx++;
    }

    bool hasPrivateConstructors = false;
    bool hasPublicConstructors = false;
    for(MetaFunction *func : funcs) {
        FunctionModificationList mods = func->modifications(this);
        for(const FunctionModification &mod : mods) {
            if (mod.isRenameModifier()) {
                func->setName(mod.renamedTo());
            }
        }

        // Make sure class is abstract if one of the functions is
        if (func->isAbstract()) {
            (*this) += MetaAttributes::Abstract;
            (*this) -= MetaAttributes::Final;
        }

        if (func->isConstructor()) {
            if (func->isPrivate())
                hasPrivateConstructors = true;
            else
                hasPublicConstructors = true;
        }
    }

    if (hasPrivateConstructors && !hasPublicConstructors) {
        (*this) += MetaAttributes::Abstract;
        (*this) -= MetaAttributes::Final;
    }

    for(MetaFunction *f1 : funcs) {
        for(MetaFunction *f2 : funcs) {
            if (f1 != f2) {
                uint cmp = f1->compareTo(f2);
                if ((cmp & MetaFunction::EqualName)
                        && !f1->isFinalInCpp()
                        && f2->isFinalInCpp()) {
                    *f2 += MetaAttributes::FinalOverload;
                }
            }
        }
    }

    setFunctions(funcs);
    for(MetaFunction *func : m_functions) {
        // Make sure that we include files for all classes that are in use

        if (!func->isRemovedFrom(this, TS::ShellCode))
            add_extra_includes_for_function(this, func);
    }
}

QString MetaType::minimalSignature() const {
    QString minimalSignature;
    if (isConstant())
        minimalSignature += "const ";
    minimalSignature += typeEntry()->qualifiedCppName();
    if (hasInstantiations() && (!typeEntry()->isContainer()
                                || static_cast<const ContainerTypeEntry*>(typeEntry())->type()!=ContainerTypeEntry::StringListContainer)) {
        const QList<const MetaType *>& instantiations = this->instantiations();
        minimalSignature += "<";
        for (int i = 0;i < instantiations.size();++i) {
            if (i > 0)
                minimalSignature += ",";
            minimalSignature += instantiations.at(i)->minimalSignature();
        }
        minimalSignature += ">";
    }

    if (getReferenceType()==MetaType::Reference)
        minimalSignature += "&";
    if (getReferenceType()==MetaType::RReference)
        minimalSignature += "&&";
    for (int j = 0; j < indirections().size(); ++j){
        if(indirections()[j]){
            minimalSignature += "*const ";
        }else{
            minimalSignature += "*";
        }
    }

    return minimalSignature;
}

QString MetaType::normalizedSignature() const {
    QString normalizedSignature = typeEntry()->qualifiedCppName();
    if (hasInstantiations()) {
        const QList<const MetaType *>& instantiations = this->instantiations();
        normalizedSignature += "<";
        for (int i = 0;i < instantiations.size();++i) {
            if (i > 0)
                normalizedSignature += ",";
            normalizedSignature += instantiations.at(i)->normalizedSignature();
        }
        normalizedSignature += ">";
    }

    if (getReferenceType()==MetaType::Reference && !isConstant())
        normalizedSignature += "&";
    for (int j = 0; j < indirections().size(); ++j){
        if(indirections()[j]){
            normalizedSignature += "*const";
        }else{
            normalizedSignature += "*";
        }
    }
    return normalizedSignature;
}

bool MetaType::hasNativeId() const {
    return (isQObject() || isValue() || isObject() || isFunctional() || isRValue()) && typeEntry()->isNativeIdBased();
}


/*******************************************************************************
 * Other stuff...
 */


MetaEnum *MetaClassList::findEnum(const EnumTypeEntry *entry) const {
    Q_ASSERT(entry->isEnum());

    QString qualified_name = entry->name();
    auto pos = qualified_name.lastIndexOf("::");

    QString enum_name;
    QString class_name;

    if (pos > 0) {
        enum_name = qualified_name.mid(pos + 2);
        class_name = qualified_name.mid(0, pos);
    } else {
        enum_name = qualified_name;
        for(MetaClass *c : *this) {
            if (c->typeEntry()->isGlobal()){
                MetaEnum *enm = c->findEnum(enum_name);
                if(enm)
                    return enm;
            }
        }
    }

    MetaClass *meta_class = findClass(class_name);
    if (!meta_class && !entry->javaScope().isEmpty()) {
        meta_class = findClass(entry->javaScope(), FullName);
    }
    if (!meta_class) {
        ReportHandler::warning(QString("Unknown class '%1' for enum '%2'")
                               .arg(class_name, entry->qualifiedCppName()));
        return nullptr;
    }

    return meta_class->findEnum(enum_name);
}

MetaEnumValue *MetaEnumValueList::find(const QString &name) const {
    for (int i = 0; i < size(); ++i) {
        if (name == at(i)->name())
            return at(i);
    }
    return nullptr;
}

MetaEnumValue *MetaClassList::findEnumValue(const QString &name) const {
    QStringList lst = name.split(QLatin1String("::"));

    Q_ASSERT_X(lst.size() >= 2, "AbstractMetaClassList::findEnumValue()", "Expected qualified enum");

    if(lst.size() >= 2){
        QString prefixName;
        for(int i=0; i<lst.size()-1; i++){
            if(!prefixName.isEmpty())
                prefixName += "::";
            prefixName += lst.at(i);
        }
        QString enumName = lst.last();

        MetaClass *cl = findClass(prefixName);
        if (!cl){
            prefixName = "";
            for(int i=0; i<lst.size()-2; i++){
                if(!prefixName.isEmpty())
                    prefixName += "::";
                prefixName += lst.at(i);
            }
            enumName = lst.at(lst.length()-2) + "::" + lst.at(lst.length()-1);
            cl = findClass(prefixName);
        }
        if (cl)
            return cl->findEnumValue(enumName, nullptr);

        ReportHandler::warning(QString("no matching enum '%1'").arg(name));
    }
    return nullptr;
}

/*!
 * Searches the list after a class that mathces \a name; either as
 * C++, Java base name or complete Java package.class name.
 */

MetaClass *MetaClassList::findClass(const QString &name, NameFlag nameFlag) const {
    if (name.isEmpty())
        return nullptr;

    if(nameFlag & QualifiedCppName){
        for(MetaClass *c : *this) {
            if (c->qualifiedCppName() == name)
                return c;
        }
    }

    if(nameFlag & FullName){
        for(MetaClass *c : *this) {
            if (c->fullName() == name)
                return c;
        }
    }

    if(nameFlag & SimpleName){
        for(MetaClass *c : *this) {
            if (c->name() == name)
                return c;
        }
    }

    return nullptr;
}

bool MetaClass::hasQmlListProperty() const{
    for(MetaFunction* function : functions()){
        if(function->type()
                && function->type()->typeEntry()->isContainer()
                && reinterpret_cast<const TS::ContainerTypeEntry*>(function->type()->typeEntry())->type()==TS::ContainerTypeEntry::QQmlListPropertyContainer){
            return true;
        }
        for(MetaArgument* arg : function->arguments()){
            if(arg->type()
                    && arg->type()->typeEntry()->isContainer()
                    && reinterpret_cast<const TS::ContainerTypeEntry*>(arg->type()->typeEntry())->type()==TS::ContainerTypeEntry::QQmlListPropertyContainer){
                return true;
            }
        }
    }
    return false;
}

bool MetaClass::hasPaintMethod() const{
    for(MetaFunction* function : functions()){
        if(function->isPaintMethod())
            return true;
    }
    return false;
}

QSet<QString> MetaClass::getAllUnimplmentablePureVirtualFunctions()const{
    QSet<QString> allPrivatePureVirtualFunctions;
    allPrivatePureVirtualFunctions |= m_unimplmentablePureVirtualFunctions;
    if(baseClass()){
        QSet<QString> inheritedPrivatePureVirtualFunctions = baseClass()->getAllUnimplmentablePureVirtualFunctions();
        if(!inheritedPrivatePureVirtualFunctions.isEmpty()){
            MetaFunctionList functions;
            functions << m_functions;
            functions << m_invalidfunctions;
            if(this->templateBaseClass()){
                functions << this->templateBaseClass()->m_functions;
                functions << this->templateBaseClass()->m_invalidfunctions;
            }
            for(MetaFunction* function : functions){
                if(!function->isConstructor() &&
                        !function->isDestructor() &&
                        !function->isAbstract()){
                    inheritedPrivatePureVirtualFunctions.remove(function->signature());
                }
            }
            allPrivatePureVirtualFunctions |= inheritedPrivatePureVirtualFunctions;
        }
    }
    return allPrivatePureVirtualFunctions;
}
