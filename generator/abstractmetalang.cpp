/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "abstractmetalang.h"
#include "abstractmetabuilder.h"
#include "reporthandler.h"
#include "jumptable.h"
#include "typesystem/typedatabase.h"

/*******************************************************************************
 * AbstractMetaType
 */
AbstractMetaType *AbstractMetaType::copy() const {
    AbstractMetaType *cpy = new AbstractMetaType;
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

QString AbstractMetaType::cppSignature() const {
    QString s;

    if (isConstant())
        s += "const ";

    if(typeEntry()->qualifiedCppName().contains("qtjambireal")){
        s += typeEntry()->qualifiedCppName().replace("qtjambireal", "qreal");
    }else{
        s += typeEntry()->qualifiedCppName();
    }

    if (hasInstantiationInCpp()) {
        const QList<const AbstractMetaType *>& types = instantiations();
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
        if (getReferenceType()==AbstractMetaType::Reference)
            s += '&';
        if (getReferenceType()==AbstractMetaType::RReference)
            s += "&&";
    }
    return s;
}

/*******************************************************************************
 * AbstractMetaTemplateParameter
 */
AbstractMetaTemplateParameter *AbstractMetaTemplateParameter::copy() const {
    AbstractMetaTemplateParameter *cpy = new AbstractMetaTemplateParameter;
    cpy->setName(AbstractMetaTemplateParameter::name());
    cpy->setInstantiation(instantiation());
    cpy->setType(type()->copy());
    cpy->setDefaultType(defaultType());

    return cpy;
}

/*******************************************************************************
 * AbstractMetaArgument
 */
AbstractMetaArgument *AbstractMetaArgument::copy() const {
    AbstractMetaArgument *cpy = new AbstractMetaArgument;
    cpy->setName(AbstractMetaVariable::name());
    cpy->setModifiedName(m_modifiedName);
    cpy->setDefaultValueExpression(defaultValueExpression());
    cpy->setOriginalDefaultValueExpression(originalDefaultValueExpression());
    cpy->setType(type()->copy());
    cpy->setArgumentIndex(argumentIndex());

    return cpy;
}

QString AbstractMetaArgument::argumentName() const {
    QString n = AbstractMetaVariable::name();
    if (n.isEmpty()) {
        if(!m_modifiedName.isEmpty()){
            return m_modifiedName;
        }
        return QString("arg__%2").arg(m_argument_index + 1);
    }
    return n;
}

QString AbstractMetaArgument::modifiedArgumentName() const {
    if(!m_modifiedName.isEmpty()){
        return m_modifiedName;
    }
    return argumentName();
}


QString AbstractMetaArgument::indexedName() const {
    QString n = AbstractMetaVariable::name();
    if (n.isEmpty())
        return QString("arg__%2").arg(m_argument_index + 1);
    return QString("%1%2").arg(n).arg(m_argument_index);
}

QString AbstractMetaArgument::name() const {
    Q_ASSERT_X(0, "AbstractMetaArgument::name()", "use argumentName() or indexedName() instead");
    return QString();
}

QString AbstractMetaClass::fullName() const {
    if(package().isEmpty()) return name();
    return package() + "." + name();
}



/*******************************************************************************
 * AbstractMetaFunction
 */
AbstractMetaFunction::~AbstractMetaFunction() {
    qDeleteAll(m_arguments);
    delete m_type;
}

/*******************************************************************************
 * Indicates that this function has a modification that removes it
 */
bool AbstractMetaFunction::isModifiedRemoved(int types) const {
    FunctionModificationList mods = modifications(implementingClass());
    for(const FunctionModification& mod : mods) {
        if (!mod.isRemoveModifier())
            continue;

        if ((mod.removal & types) == types)
            return true;
    }

    return false;
}

bool AbstractMetaFunction::needsCallThrough() const {
    if (ownerClass()->isInterface())
        return false;
    if (referenceCounts(implementingClass()).size() > 0)
        return true;
    if (argumentsHaveNativeId() || !isStatic())
        return true;
    if (JumpTableGenerator::isJumpTableActive())
        return true;

    for(const AbstractMetaArgument *arg : arguments()) {
        if (arg->type()->isArray() || arg->type()->isTargetLangEnum() || arg->type()->isTargetLangFlags())
            return true;
    }

    if (type() && (type()->isArray() || type()->isTargetLangEnum() || type()->isTargetLangFlags()))
        return true;

    for (int i = -1; i <= arguments().size(); ++i) {
        OwnershipRule ownershipRule = this->ownership(implementingClass(), TypeSystem::TargetLangCode, i);
        if (ownershipRule.ownership != TypeSystem::InvalidOwnership && ownershipRule.ownership != TypeSystem::IgnoreOwnership)
            return true;
    }

    const AbstractMetaClass *cls = implementingClass();
    while (cls) {
        for(const FunctionModification& mod : this->modifications(cls)) {
            if (mod.snips.count() <= 0)
                continue ;
            for(const CodeSnip& snip : mod.snips) {
                if (snip.language == TypeSystem::TargetLangCode)
                    return true;
            }
        }

        if (cls == cls->baseClass())
            break;
        cls = cls->baseClass();
    }

    return false;
}

bool AbstractMetaFunction::needsSuppressRawTypeWarning() const {
    AbstractMetaArgumentList arguments = this->arguments();
    for(const AbstractMetaArgument *arg : arguments) {
        if(arg->type()->typeEntry()->isComplex()){
            const ComplexTypeEntry* ctype = dynamic_cast<const ComplexTypeEntry*>(arg->type()->typeEntry());
            if(ctype->isGenericClass() && arg->type()->instantiations().isEmpty()){
                return true;
            }
        }
    }
    return false;
}

bool AbstractMetaFunction::needsSuppressUncheckedWarning() const {
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

QString AbstractMetaFunction::marshalledName() const {
    QString returned;
    if(this->isConstructor()){
        //returned = "__qt_" + name();
        if(this->ownerClass()->typeEntry()->designatedInterface()){
            returned = "__qt_" + this->ownerClass()->typeEntry()->designatedInterface()->targetLangName().replace("::", "_").replace("[]", "_3").replace(".", "_") + "_new";
        }else{
            returned = "__qt_" + this->ownerClass()->name().replace("[]", "_3").replace(".", "_") + "_new";
        }
    }else{
        if(this->ownerClass()->typeEntry()->designatedInterface()){
            returned = "__qt_" + this->ownerClass()->typeEntry()->designatedInterface()->targetLangName().replace("::", "_").replace("[]", "_3").replace(".", "_") + "_" + name();
        }else{
            returned = "__qt_" + this->ownerClass()->name().replace("[]", "_3").replace(".", "_") + "_" + name();
        }
    }
    AbstractMetaArgumentList arguments = this->arguments();
    for(const AbstractMetaArgument *arg : arguments) {
        if(arg->type()->getReferenceType()==AbstractMetaType::Reference){
            if(arg->type()->isConstant()){
                returned += "_cref";
            }else{
                returned += "_ref";
            }
        }else if(arg->type()->getReferenceType()==AbstractMetaType::RReference){
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
    return returned;
}

bool AbstractMetaFunction::operator<(const AbstractMetaFunction &other) const {
    uint result = compareTo(&other);
    return result & NameLessThan;
}


/*!
    Returns a mask of CompareResult describing how this function is
    compares to another function
*/
uint AbstractMetaFunction::compareTo(const AbstractMetaFunction *other) const {
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
    AbstractMetaType *t = type();
    AbstractMetaType *ot = other->type();
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
    const AbstractMetaArgumentList& min_arguments = arguments().size() < other->arguments().size() ? arguments() : other->arguments();
    const AbstractMetaArgumentList& max_arguments = arguments().size() < other->arguments().size() ? other->arguments(): arguments();

    auto min_count = min_arguments.size();
    auto max_count = max_arguments.size();
    bool same = true;
    for (int i = 0; i < max_count; ++i) {
        if (i < min_count) {
            const AbstractMetaArgument *min_arg = min_arguments.at(i);
            const AbstractMetaArgument *max_arg = max_arguments.at(i);
            if (min_arg->type()->typeEntry() != max_arg->type()->typeEntry()
                    && min_arg->type()->typeEntry()->qualifiedTargetLangName() != max_arg->type()->typeEntry()->qualifiedTargetLangName()
                    && (min_arg->defaultValueExpression().isEmpty() || max_arg->defaultValueExpression().isEmpty())) {
                same = false;
                break;
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

AbstractMetaFunction *AbstractMetaFunction::copy() const {
    AbstractMetaFunction *cpy = new AbstractMetaFunction;
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
    cpy->setDeclaringTemplate(declaringTemplate());
    cpy->setDeprecatedComment(deprecatedComment());
    cpy->setHref(href());
    cpy->setBrief(brief());

    for(const AbstractMetaArgument *arg : arguments())
        cpy->addArgument(arg->copy());

    for(const AbstractMetaTemplateParameter *param : templateParameters())
        cpy->addTemplateParameter(param->copy());

    Q_ASSERT((!type() && !cpy->type())
             || (type()->instantiations() == cpy->type()->instantiations()));

    return cpy;
}

QStringList AbstractMetaFunction::introspectionCompatibleSignatures(const QStringList &resolvedArguments) const {
    const AbstractMetaArgumentList& arguments = this->arguments();
    if (arguments.size() == resolvedArguments.size()) {
        return (QStringList() << QMetaObject::normalizedSignature((name() + "(" + resolvedArguments.join(",") + ")").toUtf8().constData()));
    } else {
        QStringList returned;

        AbstractMetaArgument *argument = arguments.at(resolvedArguments.size());
        QStringList minimalTypeSignature = argument->type()->minimalSignature().split("::");
        for (int i = 0; i < minimalTypeSignature.size(); ++i) {
            returned += introspectionCompatibleSignatures(QStringList(resolvedArguments)
                        << QStringList(minimalTypeSignature.mid(minimalTypeSignature.size() - i - 1)).join("::"));
        }

        return returned;
    }
}

QString AbstractMetaFunction::signature(bool skipName) const {
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
        AbstractMetaArgument *a = m_arguments.at(i);
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

bool AbstractMetaFunction::isCopyConstructor() const {
    if(this->isConstructor()){
        const AbstractMetaArgumentList& arguments = this->arguments();
        if(arguments.size()==1){
            AbstractMetaArgument* argument = arguments.at(0);
            if(argument->type()->getReferenceType()==AbstractMetaType::Reference
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

bool AbstractMetaFunction::hasRReferences() const {
    const AbstractMetaArgumentList& arguments = this->arguments();
    for(int i=0; i<arguments.size(); i++){
        AbstractMetaArgument* argument = arguments.at(i);
        if(argument->type()->getReferenceType()==AbstractMetaType::RReference){
            return true;
        }
    }
    return false;
}


int AbstractMetaFunction::actualMinimumArgumentCount() const {
    if(m_actualMinimumArgumentCount==-1){
        FunctionModificationList modifications = this->modifications(implementingClass());
        if(implementingClass()!=declaringClass())
            modifications << this->modifications(declaringClass());
        QSet<int> removedArguments;
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
                if (argument_modification.removed)
                    removedArguments.insert(argument_modification.index);
            }
        }

        const AbstractMetaArgumentList& arguments = this->arguments();

        m_actualMinimumArgumentCount = 0;
        for (int i = 0; i < arguments.size(); ++i) {
            if (!removedArguments.contains(i + 1) && arguments.at(i)->originalDefaultValueExpression().isEmpty())
                ++m_actualMinimumArgumentCount;
        }
    }
    return m_actualMinimumArgumentCount;
}

// Returns reference counts for argument at idx, or all arguments if idx == -2
QList<ReferenceCount> AbstractMetaFunction::referenceCounts(const AbstractMetaClass *cls, int idx) const {
    QList<ReferenceCount> returned;

    if(m_accessedField){
        if((idx==1 || idx == -2) && (this->attributes() & AbstractMetaAttributes::SetterFunction) == AbstractMetaAttributes::SetterFunction){
            FieldModificationList modifications = m_accessedField->modifications();
            for(const FieldModification& modification : modifications) {
                returned += modification.referenceCounts;
            }
        }
    }else{
        FunctionModificationList mods = this->modifications(cls);
        for(const FunctionModification& mod : mods) {
            for(const ArgumentModification& argument_mod : mod.argument_mods) {
                if (argument_mod.index != idx && idx != -2)
                    continue;
                returned += argument_mod.referenceCounts;
            }
        }
    }
    return returned;
}

QList<TemplateInstantiation> AbstractMetaFunction::templateInstantiations(const AbstractMetaClass *cls) const{
    QList<TemplateInstantiation> returned;

    FunctionModificationList mods = this->modifications(cls);
    for(const FunctionModification& mod : mods) {
        returned.append(mod.template_instantiations);
    }

    return returned;
}

QString AbstractMetaFunction::replacedDefaultExpression(const AbstractMetaClass *cls, int key) const {
    FunctionModificationList modifications = this->modifications(cls);
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key
                    && !argument_modification.replaced_default_expression.isEmpty()) {
                return argument_modification.replaced_default_expression;
            }
        }
    }

    return QString();
}

bool AbstractMetaFunction::removedDefaultExpression(const AbstractMetaClass *cls, int key) const {
    FunctionModificationList modifications = this->modifications(cls);
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key
                    && argument_modification.removed_default_expression) {
                return true;
            }
        }
    }

    return false;
}

bool AbstractMetaFunction::resetObjectAfterUse(int argument_idx) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argumentModification : modification.argument_mods) {
            if (argumentModification.index == argument_idx && argumentModification.reset_after_use)
                return true;
        }
    }

    return false;
}

bool AbstractMetaFunctional::resetObjectAfterUse(int argument_idx) const {
    for(const ArgumentModification& argumentModification : typeEntry()->argumentModification()) {
        if (argumentModification.index == argument_idx && argumentModification.reset_after_use)
            return true;
    }

    return false;
}

QString AbstractMetaFunction::nullPointerDefaultValue(const AbstractMetaClass *mainClass, int argument_idx) const {
    const AbstractMetaClass *cls = mainClass;
    if (!cls)
        cls = implementingClass();

    do {
        FunctionModificationList modifications = this->modifications(cls);
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
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

bool AbstractMetaFunctional::nullPointersDisabled(int argument_idx) const {
    for(const ArgumentModification& argument_modification : typeEntry()->argumentModification()) {
        if (argument_modification.index == argument_idx
                && argument_modification.no_null_pointers) {
            return true;
        }
    }
    return false;
}

bool AbstractMetaFunction::nullPointersDisabled(const AbstractMetaClass *mainClass, int argument_idx) const {
    if(m_accessedField){
        if(argument_idx==1 && (this->attributes() & AbstractMetaAttributes::SetterFunction) == AbstractMetaAttributes::SetterFunction){
            FieldModificationList modifications = m_accessedField->modifications();
            for(const FieldModification& modification : modifications) {
                if(modification.no_null_pointers)
                    return true;
            }
        }
    }else{
        const AbstractMetaClass *cls = mainClass;
        if (!cls)
            cls = implementingClass();

        do {
            FunctionModificationList modifications = this->modifications(cls);
            for(const FunctionModification& modification : modifications) {
                for(const ArgumentModification& argument_modification : modification.argument_mods) {
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

QString AbstractMetaFunction::conversionRule(TypeSystem::Language language, int key) const {
    if(m_accessedField){
        if( (key==1 && (this->attributes() & AbstractMetaAttributes::SetterFunction) == AbstractMetaAttributes::SetterFunction)
                || (key==0 && (this->attributes() & AbstractMetaAttributes::GetterFunction) == AbstractMetaAttributes::GetterFunction) ){
            FieldModificationList modifications = m_accessedField->modifications();
            for(const FieldModification& modification : modifications) {
                for(const CodeSnip& snip : modification.conversion_rules) {
                    if (snip.language == language){
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

bool AbstractMetaFunction::hasConversionRule(TypeSystem::Language language, int key) const {
    if(m_accessedField){
        if( (key==1 && (this->attributes() & AbstractMetaAttributes::SetterFunction) == AbstractMetaAttributes::SetterFunction)
                || (key==0 && (this->attributes() & AbstractMetaAttributes::GetterFunction) == AbstractMetaAttributes::GetterFunction) ){
            FieldModificationList modifications = m_accessedField->modifications();
            for(const FieldModification& modification : modifications) {
                for(const CodeSnip& snip : modification.conversion_rules) {
                    if (snip.language == language && snip.hasCode())
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

QString AbstractMetaFunctional::conversionRule(TypeSystem::Language language, int key) const {
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

bool AbstractMetaFunctional::hasConversionRule(TypeSystem::Language language, int key) const {
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

QString AbstractMetaFunction::argumentReplaced(int key) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key && !argument_modification.replace_value.isEmpty()) {
                return argument_modification.replace_value;
            }
        }
    }

    return "";
}

QList<const ArgumentModification*> AbstractMetaFunction::addedArguments() const
{
    QList<const ArgumentModification*> result;
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if(argument_modification.index==ArgumentModification::ADDED_ARGUMENT){
                result << &argument_modification;
            }
        }
    }
    return result;
}

ArgumentRemove AbstractMetaFunction::argumentRemoved(int key) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
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
        AbstractMetaType* type = arguments()[key-1]->type();
        if(type && type->isNullPtr()){
            return ArgumentRemove_Remove;
        }
    }

    return ArgumentRemove_No;
}

ThreadAffinity AbstractMetaFunction::argumentThreadAffinity(int key) const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key) {
                return argument_modification.thread_affine;
            }
        }
    }
    return ThreadAffinity::None;
}

ArgumentRemove AbstractMetaFunctional::argumentRemoved(int key) const {
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

bool AbstractMetaFunction::isVirtualSlot() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isVirtualSlot())
            return true;
    }

    return false;
}

bool AbstractMetaFunction::isUIThreadAffine() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isUIThreadAffine())
            return true;
    }

    return false;
}

bool AbstractMetaFunction::isPixmapThreadAffine() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isPixmapThreadAffine())
            return true;
    }

    return false;
}

bool AbstractMetaFunction::isThreadAffine() const {
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if (modification.isThreadAffine())
            return true;
    }

    return false;
}

bool AbstractMetaFunction::isAllowedAsSlot() const {
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

bool AbstractMetaFunction::isDeprecated() const {
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

OwnershipRule AbstractMetaFunction::ownership(const AbstractMetaClass *cls, TypeSystem::Language language, int key) const {
    if(m_accessedField){
        if(key==1 && (this->attributes() & AbstractMetaAttributes::SetterFunction) == AbstractMetaAttributes::SetterFunction){
            for(const FieldModification& modification : m_accessedField->modifications()) {
                OwnershipRule o = modification.ownerships.value(language, OwnershipRule{TypeSystem::InvalidOwnership, QString()});
                if(o.ownership!=TypeSystem::InvalidOwnership)
                    return o;
            }
        }
    }else{
        FunctionModificationList modifications = this->modifications(cls);
        for(const FunctionModification& modification : modifications) {
            for(const ArgumentModification& argument_modification : modification.argument_mods) {
                if (argument_modification.index == key){
                    OwnershipRule o = argument_modification.ownerships.value(language, OwnershipRule{TypeSystem::InvalidOwnership, QString()});
                    if(o.ownership!=TypeSystem::InvalidOwnership)
                        return o;
                }
            }
        }
    }

    return OwnershipRule{TypeSystem::InvalidOwnership, QString()};
}

bool AbstractMetaFunction::isRemovedFromAllLanguages(const AbstractMetaClass *cls) const {
    return isRemovedFrom(cls, TypeSystem::All);
}

bool AbstractMetaFunction::isRemovedFrom(const AbstractMetaClass *cls, TypeSystem::Language language) const {
    FunctionModificationList modifications = this->modifications(cls);
    for(const FunctionModification& modification : modifications) {
        if ((modification.removal & language) == language)
            return true;
    }

    return false;

}

int AbstractMetaFunction::argumentTypeArrayLengthIndex(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key
                    && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
                return argument_modification.arrayLengthParameter;
            }
        }
    }

    return -1;
}

int AbstractMetaFunction::argumentTypeArrayLengthMinValue(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key
                    && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
                return argument_modification.minArrayLength;
            }
        }
    }
    return -1;
}

int AbstractMetaFunction::argumentTypeArrayLengthMaxValue(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key
                    && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
                return argument_modification.maxArrayLength;
            }
        }
    }
    return -1;
}

int AbstractMetaFunctional::argumentTypeArrayLengthIndex(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key
                && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
            return argument_modification.arrayLengthParameter;
        }
    }
    return -1;
}

int AbstractMetaFunctional::argumentTypeArrayLengthMinValue(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key
                && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
            return argument_modification.minArrayLength;
        }
    }
    return -1;
}

int AbstractMetaFunctional::argumentTypeArrayLengthMaxValue(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key
                && argument_modification.useAsArrayType.testFlag(AsArrayType::Yes)) {
            return argument_modification.maxArrayLength;
        }
    }
    return -1;
}

bool AbstractMetaFunction::argumentTypeArrayDeref(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key) {
                return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::Deref);
            }
        }
    }

    return false;
}

bool AbstractMetaFunctional::isNoExcept() const{
    for(const Modification& modification : this->typeEntry()->modifications()) {
        if(modification.isNoExcept())
            return true;
    }
    return false;
}

bool AbstractMetaFunction::isNoExcept() const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        if(modification.isNoExcept())
            return true;
    }
    return false;
}

bool AbstractMetaFunction::argumentTypeArray(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key) {
                return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && !argument_modification.useAsArrayType.testFlag(AsArrayType::Buffer);
            }
        }
    }

    return false;
}

bool AbstractMetaFunction::argumentTypeBuffer(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key) {
                return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::Buffer);
            }
        }
    }

    return false;
}

bool AbstractMetaFunction::argumentTypeArrayVarArgs(int key) const{
    FunctionModificationList modifications = this->modifications(implementingClass());
    if(implementingClass()!=declaringClass())
        modifications << this->modifications(declaringClass());
    for(const FunctionModification& modification : modifications) {
        for(const ArgumentModification& argument_modification : modification.argument_mods) {
            if (argument_modification.index == key) {
                return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::VarArgs);
            }
        }
    }

    return false;
}

bool AbstractMetaFunctional::argumentTypeArrayVarArgs(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::VarArgs);
        }
    }
    return false;
}

bool AbstractMetaFunctional::argumentTypeArrayDeref(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::Deref);
        }
    }
    return false;
}

bool AbstractMetaFunctional::argumentTypeArray(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && !argument_modification.useAsArrayType.testFlag(AsArrayType::Buffer);
        }
    }
    return false;
}

bool AbstractMetaFunctional::argumentTypeBuffer(int key) const{
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key) {
            return argument_modification.useAsArrayType.testFlag(AsArrayType::Yes) && argument_modification.useAsArrayType.testFlag(AsArrayType::Buffer);
        }
    }
    return false;
}

QString AbstractMetaFunction::typeReplaced(int key, QString* jniType) const {
    if(m_accessedField){
        if( (key==1 && (this->attributes() & AbstractMetaAttributes::SetterFunction) == AbstractMetaAttributes::SetterFunction)
                || (key==0 && (this->attributes() & AbstractMetaAttributes::GetterFunction) == AbstractMetaAttributes::GetterFunction) ){
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
                if (argument_modification.index == key
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

QString AbstractMetaFunctional::typeReplaced(int key, QString* jniType) const {
    for(const ArgumentModification& argument_modification : this->typeEntry()->argumentModification()) {
        if (argument_modification.index == key
                && !argument_modification.modified_type.isEmpty()) {
            if(jniType && !argument_modification.modified_jni_type.isEmpty()){
                *jniType = argument_modification.modified_jni_type;
            }
            return argument_modification.modified_type;
        }
    }

    return QString();
}

QString AbstractMetaFunction::minimalSignature() const {
    if (!m_cached_minimal_signature.isEmpty())
        return m_cached_minimal_signature;

    QString minimalSignature = originalName() + "(";
    const AbstractMetaArgumentList& arguments = this->arguments();

    for (int i = 0; i < arguments.count(); ++i) {
        AbstractMetaType *t = arguments.at(i)->type();

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

QString AbstractMetaFunction::throws(const AbstractMetaClass *implementor) const {
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

bool AbstractMetaFunction::hasCodeInjections(const AbstractMetaClass *implementor, TypeSystem::Language language, const QSet<CodeSnip::Position>& positions) const{
    return implementor->typeEntry()->hasFunctionCodeInjections(minimalSignature(), language, positions);
}

FunctionModificationList AbstractMetaFunction::modifications(const AbstractMetaClass *implementor) const {
    Q_ASSERT(implementor);
    FunctionModificationList result = implementor->typeEntry()->functionModifications(minimalSignature());
    if(result.isEmpty() && (implementor->qualifiedCppName().startsWith("QOpenGLFunctions") || implementor->qualifiedCppName()=="QOpenGLExtraFunctions")){
        const AbstractMetaArgumentList& arguments = this->arguments();
        QList<ArgumentModification> argumentMods;
        for (int i = 0; i < arguments.count(); ++i) {
            AbstractMetaType *t = arguments.at(i)->type();
            if(t->isNativePointer() && (t->typeEntry()->isPrimitive() || t->typeEntry()->isVoid()) && t->typeEntry()->qualifiedCppName()!="GLboolean" && t->indirections().size()==1){
                ArgumentModification mod(i+1);
                mod.useAsArrayType |= AsArrayType::Yes;
                mod.useAsArrayType |= AsArrayType::Buffer;
                mod.no_null_pointers = true;
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

bool AbstractMetaFunction::hasModifications(const AbstractMetaClass *implementor) const {
    FunctionModificationList mods = modifications(implementor);
    return mods.count() > 0;
}

QString AbstractMetaFunction::modifiedName() const {
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

QString AbstractMetaFunction::targetLangSignature(bool minimal) const {
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
AbstractMetaClass::~AbstractMetaClass() {
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
bool AbstractMetaClass::inheritsFrom(const AbstractMetaClass *cls) const {
    Q_ASSERT(cls);

    const AbstractMetaClass *clazz = this;
    while (clazz) {
        if (clazz == cls)
            return true;

        clazz = clazz->baseClass();
    }

    return false;
}

bool AbstractMetaClass::inheritsFromInterface(const AbstractMetaClass *cls) const {
    Q_ASSERT(cls);

    const AbstractMetaClass *clazz = this;
    while (clazz) {
        if (clazz == cls || clazz->interfaces().contains(const_cast<AbstractMetaClass *>(cls)))
            return true;

        clazz = clazz->baseClass();
    }

    return false;
}

/*******************************************************************************
 * Constructs an interface based on the functions and enums in this
 * class and returns it...
 */
AbstractMetaClass *AbstractMetaClass::extractInterface() {
    Q_ASSERT(typeEntry()->designatedInterface());

    if (!m_extracted_interface) {
        AbstractMetaClass *iface = new AbstractMetaClass;
        iface->setAttributes(attributes());
        iface->setBaseClass(nullptr);
        iface->setPrimaryInterfaceImplementor(this);

        iface->setTypeEntry(typeEntry()->designatedInterface());

        for(AbstractMetaFunction *function : functions()) {
            if (!function->isConstructor())
                iface->addFunction(function->copy());
        }

//         iface->setEnums(enums());
//         setEnums(AbstractMetaEnumList());

        for(const AbstractMetaField *field : fields()) {
            if (field->isPublic()) {
                AbstractMetaField *new_field = field->copy();
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

AbstractMetaClass *AbstractMetaClass::extractInterface() const {
    return m_extracted_interface;
}

/*******************************************************************************
 * Constructs an implementation class based on the functions and enums in this
 * interface and returns it...
 */
AbstractMetaClass *AbstractMetaClass::extractInterfaceImpl() {
    Q_ASSERT(typeEntry()->isInterface());
    const InterfaceTypeEntry* itype = static_cast<const InterfaceTypeEntry*>(typeEntry());
    Q_ASSERT(itype->origin());

    if (!m_extracted_interface_impl) {
        AbstractMetaClass *ifaceImpl = new AbstractMetaClass;
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

        for(AbstractMetaFunction *function : functions()) {
//            if (!function->isConstructor())
                function = function->copy();
                if (function->isConstructor())
                    function->setName(InterfaceTypeEntry::implName("X").split("::").last());
                else
                    *function += AbstractMetaAttributes::InterfaceFunction;
                function->setInterfaceClass(this);
                function->setImplementingClass(ifaceImpl);
                function->setDeclaringClass(ifaceImpl);
                ifaceImpl->addFunction(function);
        }

//         iface->setEnums(enums());
//         setEnums(AbstractMetaEnumList());

        for(const AbstractMetaField *field : fields()) {
//            if (field->isPublic()) {
                AbstractMetaField *new_field = field->copy();
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

AbstractMetaClass *AbstractMetaClass::extractInterfaceImpl() const {
    return m_extracted_interface_impl;
}

/*******************************************************************************
 * Returns a list of all the functions with a given name
 */
AbstractMetaFunctionList AbstractMetaClass::queryFunctionsByName(const QString &name) const {
    AbstractMetaFunctionList returned;
    for(AbstractMetaFunction *function : this->functions()) {
        if (function->name() == name)
            returned.append(function);
    }

    return returned;
}

AbstractMetaFunctionList AbstractMetaClass::queryFunctionsByOriginalName(const QString &name) const {
    AbstractMetaFunctionList returned;
    for(AbstractMetaFunction *function : this->functions()) {
        if (function->originalName() == name)
            returned.append(function);
    }

    return returned;
}

AbstractMetaFunctionList AbstractMetaClass::queryOtherFunctions(const AbstractMetaFunction* fun) const {
    AbstractMetaFunctionList returned;
    for(AbstractMetaFunction *function : this->functions()) {
        if (function!=fun && function->originalSignature() == fun->originalSignature() && !function->isRemovedFromAllLanguages(this))
            returned.append(function);
    }

    return returned;
}

/*******************************************************************************
 * Returns all reference count modifications for any function in the class
 */
QList<ReferenceCount> AbstractMetaClass::referenceCounts() const {
    QList<ReferenceCount> returned;

    for(AbstractMetaFunction *function : this->functions()) {
        returned += function->referenceCounts(this);
    }

    return returned;
}

QList<TemplateInstantiation> AbstractMetaClass::templateInstantiations() const {
    QList<TemplateInstantiation> returned;

    for(AbstractMetaFunction *function : this->functions()) {
        returned += function->templateInstantiations(this);
    }

    return returned;
}

/*******************************************************************************
 * Returns a list of all the functions retrieved during parsing which should
 * be added to the Java API.
 */
AbstractMetaFunctionList AbstractMetaClass::functionsInTargetLang() const {
    uint default_flags = NormalFunctions | Visible | NotRemovedFromTargetLang;

    // Interfaces don't implement functions
    default_flags |= isInterface() ? 0 : ClassImplements;

    // Only public functions in final classes
    // default_flags |= isFinal() ? WasPublic : 0;
    uint public_flags = isFinal() ? WasPublic : 0;

    // Constructors
    AbstractMetaFunctionList returned = queryFunctions(Constructors | default_flags | public_flags);

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

AbstractMetaFunctionList AbstractMetaClass::implementableFunctions() const {
    AbstractMetaFunctionList list = functionsInShellClass();

    AbstractMetaFunctionList returned;
    for(AbstractMetaFunction *f : list) {
        if (!f->hasRReferences()
                && !f->isRemovedFrom(f->declaringClass(), TypeSystem::TargetLangCode)
                && (!f->isFinalInCpp() || f->isVirtualSlot()))
            returned += f;
    }

    return returned;
}

AbstractMetaFunctionList AbstractMetaClass::virtualFunctions() const {
    AbstractMetaFunctionList list = functionsInShellClass();

    AbstractMetaFunctionList returned;
    for(AbstractMetaFunction *f : list) {
        if (!f->isFinalInCpp() || f->isVirtualSlot())
            returned += f;
    }

    return returned;
}

AbstractMetaFunctionList AbstractMetaClass::nonVirtualShellFunctions() const {
    AbstractMetaFunctionList list = functionsInShellClass();
    AbstractMetaFunctionList returned;
    for(AbstractMetaFunction *f : list) {
        if (f->isFinalInCpp() && !f->isVirtualSlot())
            returned += f;
    }

    return returned;
}

/*******************************************************************************
 * Returns a list of all functions that should be declared and implemented in
 * the shell class which is generated as a wrapper on top of the actual C++ class
 */
AbstractMetaFunctionList AbstractMetaClass::functionsInShellClass() const {
    // Only functions and only protected and public functions
    uint default_flags = NormalFunctions | Visible | WasVisible | NotRemovedFromShell;

    // All virtual functions
    AbstractMetaFunctionList returned = queryFunctions(VirtualFunctions | default_flags);

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
AbstractMetaFunctionList AbstractMetaClass::publicOverrideFunctions() const {
    return queryFunctions(NormalFunctions | WasProtected | FinalInCppFunctions | NotRemovedFromTargetLang)
           + queryFunctions(Signals | WasProtected | FinalInCppFunctions | NotRemovedFromTargetLang);
}

AbstractMetaFunctionList AbstractMetaClass::virtualOverrideFunctions() const {
    return queryFunctions(NormalFunctions | NonEmptyFunctions | Visible | VirtualInCppFunctions | NotRemovedFromShell) +
           queryFunctions(Signals | NonEmptyFunctions | Visible | VirtualInCppFunctions | NotRemovedFromShell);
}

bool function_sorter(AbstractMetaFunction *a, AbstractMetaFunction *b){
    return a->signature() < b->signature();
};

void AbstractMetaClass::sortFunctions() {
    std::sort(m_functions.begin(), m_functions.end(), &function_sorter);
    std::sort(m_invalidfunctions.begin(), m_invalidfunctions.end(), &function_sorter);
}

void AbstractMetaClass::sortEnums() {
    std::sort(m_enums.begin(), m_enums.end(), [](AbstractMetaEnum *a, AbstractMetaEnum *b) -> bool {
        return a->name() < b->name();
    });
}

void AbstractMetaClass::sortFields() {
    std::sort(m_fields.begin(), m_fields.end(), [](AbstractMetaField *a, AbstractMetaField *b) -> bool {
        return a->name() < b->name();
    });
}

void AbstractMetaClass::sortFunctionals() {
    std::sort(m_functionals.begin(), m_functionals.end(), [](AbstractMetaFunctional *a, AbstractMetaFunctional *b) -> bool {
        return a->name() < b->name();
    });
}

const ContainerTypeEntry* AbstractMetaClass::findContainerSuperClass(QList<const AbstractMetaType *>* instantiations) const{
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

void AbstractMetaClass::setFunctions(const AbstractMetaFunctionList &functions) {
    m_functions = functions;
    sortFunctions();

    // Functions must be sorted by name before next loop
    QString currentName;
    bool hasVirtuals = false;
    AbstractMetaFunctionList final_functions;
    for(AbstractMetaFunction *f : m_functions) {
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
                for(AbstractMetaFunction *final_function : final_functions) {
                    *final_function += AbstractMetaAttributes::ForceShellImplementation;

                    QString warn = QString("hiding of function '%1' in class '%2'")
                                   .arg(final_function->name()).arg(name());
                    ReportHandler::warning(warn);
                }
            }

            hasVirtuals = !f->isFinal();
            final_functions.clear();
            if (f->isFinal())
                final_functions += f;
            currentName = f->name();
        }
        if(f->name()=="metaObject" && f->arguments().isEmpty() && f->wasPrivate()){
            m_has_private_metaObject = true;
        }else if(f->name()=="qt_metacall"
                 && f->arguments().size()==3
                 && f->wasPrivate()
                 && f->arguments()[0]->type()->cppSignature()=="QMetaObject::Call"
                 && f->arguments()[1]->type()->cppSignature()=="int"
                 && f->arguments()[2]->type()->cppSignature()=="void**"){
            m_has_private_metacall = true;
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
            if ((cmp & AbstractMetaFunction::EqualName) && (cmp & AbstractMetaFunction::EqualArguments)) {
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

bool AbstractMetaClass::hasProtectedFieldAccessors() const {
    for(const AbstractMetaField *field : fields()) {
        if (!field->wasPublic() && (field->getter() || field->setter()))
            return true;
    }

    return false;
}

bool AbstractMetaClass::hasDefaultToStringFunction() const {
    for(AbstractMetaFunction *f : queryFunctionsByName("toString")) {
        if (f->actualMinimumArgumentCount() == 0) {
            return true;
        }

    }
    return false;
}

void AbstractMetaClass::addFunction(AbstractMetaFunction *function) {
    function->setOwnerClass(this);

    if (!function->isDestructor()){
        m_functions << function;
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

void AbstractMetaClass::addInvalidFunction(AbstractMetaFunction *function) {
    function->setOwnerClass(this);

    if (!function->isDestructor()) {
        m_invalidfunctions << function;
    }
}

bool AbstractMetaClass::hasSignal(const AbstractMetaFunction *other) const {
    if (!other->isSignal())
        return false;

    for(const AbstractMetaFunction *f : m_functions) {
        if (f->isSignal() && f->compareTo(other) & AbstractMetaFunction::EqualName)
            return other->modifiedName() == f->modifiedName();
    }

    return false;
}


QString AbstractMetaClass::name() const {
    return QString(m_type_entry->targetLangName()).replace("::", "$");
}

QString AbstractMetaClass::simpleName() const {
    QString name = QString(m_type_entry->targetLangName());
    return name.remove(0, name.lastIndexOf('$')+1);
}

bool AbstractMetaClass::hasFunction(const QString &str) const {
    for(const AbstractMetaFunction *f : m_functions) {
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
bool AbstractMetaClass::hasProtectedFunctions() const {
    for(const AbstractMetaFunction *func : m_functions) {
        if (func->wasProtected())
            return true;
    }
    return false;
}

bool AbstractMetaClass::hasProtectedConstructors() const {
    for(const AbstractMetaFunction *func : m_functions) {
        if (func->isConstructor() && func->wasProtected())
            return true;
    }
    return false;
}

bool AbstractMetaClass::instantiateShellClass() const {
    return !isFinal()
            && !m_has_private_metacall
            && !m_has_private_metaObject
            && !(typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)
            && (hasVirtualFunctions()
                || hasVirtualDestructor()
                || hasProtectedConstructors()
            //    || typeEntry()->designatedInterface()
            //    || typeEntry()->isInterface()
            );
}

bool AbstractMetaClass::generateShellClass() const {
    return !isFinal()
            && !m_has_private_metacall
            && !m_has_private_metaObject
            && !(typeEntry()->codeGeneration() & TypeEntry::GenerateNoShell)
            && (hasVirtualFunctions()
                || hasVirtualDestructor()
                || hasProtectedFunctions()
                || hasProtectedFieldAccessors()
            //    || typeEntry()->designatedInterface()
            //    || typeEntry()->isInterface()
            );
}

QPropertySpec *AbstractMetaClass::propertySpecForRead(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i)
        if (name == m_property_specs.at(i)->read())
            return m_property_specs.at(i);
    return nullptr;
}

QPropertySpec *AbstractMetaClass::propertySpecForBindable(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i)
        if (name == m_property_specs.at(i)->bindable())
            return m_property_specs.at(i);
    return nullptr;
}

QPropertySpec *AbstractMetaClass::propertySpecForWrite(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i)
        if (name == m_property_specs.at(i)->write())
            return m_property_specs.at(i);
    return nullptr;
}

QPropertySpec *AbstractMetaClass::propertySpecForReset(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i) {
        if (name == m_property_specs.at(i)->reset())
            return m_property_specs.at(i);
    }
    return nullptr;
}

QPropertySpec *AbstractMetaClass::propertySpecForNotify(const QString &name) const {
    for (int i = 0; i < m_property_specs.size(); ++i) {
        if (name == m_property_specs.at(i)->notify())
            return m_property_specs.at(i);
    }
    return nullptr;
}

static bool functions_contains(const AbstractMetaFunctionList &l, const AbstractMetaFunction *func) {
    for(const AbstractMetaFunction *f : l) {
        if ((f->compareTo(func) & AbstractMetaFunction::PrettySimilar) == AbstractMetaFunction::PrettySimilar)
            return true;
    }
    return false;
}

AbstractMetaField::AbstractMetaField() :
    AbstractMetaVariable(), AbstractMetaAttributes(),
    m_getter(nullptr), m_setter(nullptr), m_class(nullptr) {
}

AbstractMetaField::~AbstractMetaField() {
    delete m_setter;
    delete m_getter;
}

AbstractMetaField *AbstractMetaField::copy() const {
    AbstractMetaField *returned = new AbstractMetaField;
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

static AbstractMetaFunction *createXetter(const AbstractMetaField *g, const QString &name, uint type) {
    AbstractMetaFunction *f = new AbstractMetaFunction;

    f->setBrief(g->brief());
    f->setHref(g->href());

    f->setName(name);
    f->setOriginalName(name);
    f->setOwnerClass(g->enclosingClass());
    f->setImplementingClass(g->enclosingClass());
    f->setDeclaringClass(g->enclosingClass());

    uint attr = AbstractMetaAttributes::Native
                | AbstractMetaAttributes::Final
                | type;
    if (g->isStatic())
        attr |= AbstractMetaAttributes::Static;
    if (g->isPublic())
        attr |= AbstractMetaAttributes::Public;
    else if (g->isProtected())
        attr |= AbstractMetaAttributes::Protected;
    else
        attr |= AbstractMetaAttributes::Private;
    f->setAttributes(attr);
    f->setOriginalAttributes(attr);

    FieldModificationList mods = g->modifications();
    for(const FieldModification& mod : mods) {
        if (mod.isRenameModifier())
            f->setName(mod.renamedTo());
        if (mod.isAccessModifier()) {
            if (mod.isPrivate())
                f->setVisibility(AbstractMetaAttributes::Private);
            else if (mod.isProtected())
                f->setVisibility(AbstractMetaAttributes::Protected);
            else if (mod.isPublic())
                f->setVisibility(AbstractMetaAttributes::Public);
            else if (mod.isFriendly())
                f->setVisibility(AbstractMetaAttributes::Friendly);
        }

    }

    f->setAccessedField(g);

    return f;
}

FieldModificationList AbstractMetaField::modifications() const {
    FieldModificationList mods = enclosingClass()->typeEntry()->fieldModifications();
    FieldModificationList returned;

    for(const FieldModification& mod : mods) {
        if (mod.name == name())
            returned += mod;
    }

    return returned;
}

const AbstractMetaFunction *AbstractMetaField::setter() const {
    if (!m_setter) {
        QString name = this->name();
        if(name.startsWith("m_")){
            name = name.mid(2);
        }
        Modification::Modifiers accessModifier = Modification::Modifiers(0);
        FieldModificationList modifications = AbstractMetaField::modifications();
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
                                AbstractMetaAttributes::SetterFunction);
        if(accessModifier & Modification::AccessModifierMask){
            m_setter->setVisibility(accessModifier & Modification::AccessModifierMask);
        }
        AbstractMetaArgumentList arguments;
        AbstractMetaArgument *argument = new AbstractMetaArgument;
        argument->setType(type()->copy());
        if(type()->getReferenceType()==AbstractMetaType::NoReference && !type()->isPrimitive() && type()->indirections().isEmpty()){
            argument->type()->setReferenceType(AbstractMetaType::Reference);
            argument->type()->setConstant(true);
        }
        AbstractMetaBuilder::decideUsagePattern(argument->type());
        argument->setName(varName);
        arguments.append(argument);
        m_setter->setArguments(arguments);
        m_setter->setName(newName);
        m_setter->setOriginalName(newName);
        m_setter->setOriginalSignature(this->name());
        if(m_class && m_class->hasFunction(m_setter)){
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
                mod.modifiers &= ~uint(Modification::Writable);
                const_cast<ComplexTypeEntry *>(m_class->typeEntry())->addFieldModification(mod);
            }
        }
    }
    return m_setter;
}

const AbstractMetaFunction *AbstractMetaField::getter() const {
    if (!m_getter) {
        QString name = this->name();
        if(name.startsWith("m_")){
            name = name.mid(2);
        }
        Modification::Modifiers accessModifier = Modification::Modifiers(0);
        FieldModificationList modifications = AbstractMetaField::modifications();
        for(const FieldModification& modification : modifications) {
            if(!modification.renamedTo().isEmpty()){
                name = modification.renamedTo();
            }
            accessModifier = Modification::Modifiers(accessModifier & modification.accessModifier());
        }
        m_getter = createXetter(this,
                                name,
                                AbstractMetaAttributes::GetterFunction);
        if(accessModifier & Modification::AccessModifierMask){
            m_getter->setVisibility(accessModifier & Modification::AccessModifierMask);
        }
        m_getter->setType(type()->copy());
        if(type()->getReferenceType()==AbstractMetaType::NoReference && !type()->isPrimitive() && type()->indirections().isEmpty()){
            m_getter->type()->setReferenceType(AbstractMetaType::Reference);
            m_getter->type()->setConstant(true);
        }
        AbstractMetaBuilder::decideUsagePattern(m_getter->type());
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


bool AbstractMetaClass::hasConstructors() const {
    return queryFunctions(Constructors).size() != 0;
}

bool AbstractMetaClass::hasStandardConstructor() const {
    if(m_has_standardconstructor==0){
        AbstractMetaFunctionList functions = queryFunctions(Constructors);
        m_has_standardconstructor = -1;
        for(AbstractMetaFunction *f : functions) {
            if ((f->wasPublic() || f->isPublic() || f->wasProtected() || f->isProtected()) && !f->isInvalid() && !f->isEmptyFunction() && !f->isFake() && f->implementingClass()==this && f->actualMinimumArgumentCount()==0 && !f->isRemovedFromAllLanguages(this)) {
                m_has_standardconstructor = 1;
            }
        }
    }
    return m_has_standardconstructor>0;
}

bool AbstractMetaClass::hasPublicStandardConstructor() const {
    if(m_has_standardconstructor==0){
        AbstractMetaFunctionList functions = queryFunctions(Constructors);
        m_has_standardconstructor = -1;
        for(AbstractMetaFunction *f : functions) {
            if ((f->wasPublic() || f->isPublic()) && !f->isInvalid() && !f->isEmptyFunction() && !f->isFake() && f->implementingClass()==this && f->actualMinimumArgumentCount()==0 && !f->isRemovedFromAllLanguages(this)) {
                m_has_standardconstructor = 1;
            }
        }
    }
    return m_has_standardconstructor>0;
}

void AbstractMetaClass::addDefaultConstructor() {
    FunctionModificationList result = typeEntry()->functionModifications(qualifiedCppName()+"()");
    for(FunctionModification mod : result){
        if(mod.isRemoveModifier())
            return;
    }
    AbstractMetaFunction *f = new AbstractMetaFunction;
    f->setName(simpleName());
    f->setOriginalName(qualifiedCppName().split("::").last());
    f->setOwnerClass(this);
    f->setFunctionType(AbstractMetaFunction::ConstructorFunction);
    f->setArguments(AbstractMetaArgumentList());
    f->setDeclaringClass(this);

    uint attr = AbstractMetaAttributes::Native | AbstractMetaAttributes::Final;
    if(hasJustPrivateConstructors()){
        attr |= AbstractMetaAttributes::Private;
    }else{
        attr |= AbstractMetaAttributes::Public;
    }
    f->setAttributes(attr);
    f->setImplementingClass(this);
    f->setOriginalAttributes(f->attributes());

    addFunction(f);
}

bool AbstractMetaClass::hasFunction(const AbstractMetaFunction *f) const {
    return functions_contains(m_functions, f);
}

/* Goes through the list of functions and returns a list of all
   functions matching all of the criteria in \a query.
 */

AbstractMetaFunctionList AbstractMetaClass::queryFunctions(uint query) const {
    AbstractMetaFunctionList functions;

    for(AbstractMetaFunction *f : m_functions) {
        if ((query & VirtualSlots) && !f->isVirtualSlot())
            continue;

        if ((query & NotRemovedFromTargetLang) && f->isRemovedFrom(f->implementingClass(), TypeSystem::TargetLangCode)) {
            continue;
        }

        if ((query & NotRemovedFromTargetLang) && !f->isFinal() && f->isRemovedFrom(f->declaringClass(), TypeSystem::TargetLangCode)) {
            continue;
        }

        if ((query & NotRemovedFromShell) && f->isRemovedFrom(f->implementingClass(), TypeSystem::ShellCode)) {
            continue;
        }

        if ((query & NotRemovedFromShell) && !f->isFinal() && f->isRemovedFrom(f->declaringClass(), TypeSystem::ShellCode)) {
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

bool AbstractMetaClass::hasSignals() const {
    return cppSignalFunctions().size() > 0;
}


/**
 * Adds the specified interface to this class by adding all the
 * functions in the interface to this class.
 */
void AbstractMetaClass::addInterface(AbstractMetaClass *interface) {
    Q_ASSERT(interface->isInterface());
    Q_ASSERT(!m_interfaces.contains(interface));
    m_interfaces << interface;

    if (m_extracted_interface && m_extracted_interface != interface)
        m_extracted_interface->addInterface(interface);
    if (interface!=this && m_extracted_interface_impl && !m_extracted_interface_impl->interfaces().contains(interface))
        m_extracted_interface_impl->addInterface(interface);

    for(AbstractMetaFunction *function : interface->functions()) {
        if (!hasFunction(function) && !function->isConstructor()) {
            AbstractMetaFunction *cpy = function->copy();

            // Setup that this function is an interface class.
            cpy->setInterfaceClass(interface);
            *cpy += AbstractMetaAttributes::InterfaceFunction;

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


void AbstractMetaClass::setInterfaces(const AbstractMetaClassList &interfaces) {
    m_interfaces = interfaces;
}

/**
 * Adds the specified nested class to this class.
 */
void AbstractMetaClass::addEnclosedClass(AbstractMetaClass *enclosed_class) {
    Q_ASSERT(!m_enclosed_classes.contains(enclosed_class));
    m_enclosed_classes << enclosed_class;
    enclosed_class->setEnclosingClass(this);
}


void AbstractMetaClass::setEnclosedClasses(const AbstractMetaClassList &enclosed_classes) {
    m_enclosed_classes = enclosed_classes;
}


AbstractMetaEnum *AbstractMetaClass::findEnum(const QString &enumName) {
    for(AbstractMetaEnum *e : m_enums) {
        if (e->name() == enumName)
            return e;
    }

    if (typeEntry()->designatedInterface()){
        for(AbstractMetaEnum *e : extractInterface()->m_enums) {
            if (e->name() == enumName)
                return e;
        }
    }
    if (typeEntry()->isInterface()){
        for(AbstractMetaEnum *e : extractInterfaceImpl()->m_enums) {
            if (e->name() == enumName)
                return e;
        }
    }
    return nullptr;
}

AbstractMetaClass *AbstractMetaClass::findIterator(const IteratorTypeEntry* iteratorType) const{
    for(AbstractMetaClass * enclosed_class : m_enclosed_classes){
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

AbstractMetaFunctional *AbstractMetaClass::findFunctional(const QString &functionalName) {
    for(AbstractMetaFunctional *f : m_functionals) {
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
AbstractMetaEnumValue *AbstractMetaClass::findEnumValue(const QString &enumValueName, AbstractMetaEnum *meta_enum) {
    for(AbstractMetaEnum *e : m_enums) {
        if (e == meta_enum)
            continue;
        for(AbstractMetaEnumValue *v : e->values()) {
            if (v->cppName() == enumValueName)
                return v;
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
AbstractMetaEnum *AbstractMetaClass::findEnumForValue(const QString &enumValueName) {
    for(AbstractMetaEnum *e : m_enums) {
        for(AbstractMetaEnumValue *v : e->values()) {
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

QString AbstractMetaEnumValue::cppName() const {
    return m_enum && m_enum->typeEntry()->isScopedEnum() ? m_enum->typeEntry()->name() + "::" + m_name : m_name;
}

static void add_extra_include_for_type(AbstractMetaClass *meta_class, const AbstractMetaType *type) {

    if (!type)
        return;

    Q_ASSERT(meta_class);
    const TypeEntry *entry = (type ? type->typeEntry() : nullptr);
    if (entry && entry->isComplex()) {
        const ComplexTypeEntry *centry = static_cast<const ComplexTypeEntry *>(entry);
        ComplexTypeEntry *class_entry = meta_class->typeEntry();
        if (class_entry && centry->include().isValid())
            class_entry->addExtraInclude(centry->include());
        //qtjambi_qml/qqmllistproperty.h
    }

    if (type->hasInstantiations()) {
        for(const AbstractMetaType *instantiation : type->instantiations())
            add_extra_include_for_type(meta_class, instantiation);
    }
}

static void add_extra_includes_for_function(AbstractMetaClass *meta_class, const AbstractMetaFunction *meta_function) {
    Q_ASSERT(meta_class);
    Q_ASSERT(meta_function);
    add_extra_include_for_type(meta_class, meta_function->type());

    for(AbstractMetaArgument *argument : meta_function->arguments())
        add_extra_include_for_type(meta_class, argument->type());
}

void AbstractMetaClass::fixUnimplmentablePureVirtualFunctions(){
    this->m_has_unimplmentablePureVirtualFunctions = !getAllUnimplmentablePureVirtualFunctions().isEmpty();
}

void AbstractMetaClass::fixFunctions(std::function<AbstractMetaArgument*(TypeEntry *)> argumentCreator) {
    if (m_functions_fixed)
        return;
    else
        m_functions_fixed = true;

    AbstractMetaClass *super_class = baseClass();
    AbstractMetaFunctionList funcs = functions();

//     printf("fix functions for %s\n", qPrintable(name()));

    if (super_class)
        super_class->fixFunctions(argumentCreator);
    int iface_idx = 0;

    while (super_class || iface_idx < interfaces().size()) {
//         printf(" - base: %s\n", qPrintable(super_class->name()));

        // Since we always traverse the complete hierarchy we are only
        // interrested in what each super class implements, not what
        // we may have propagated from their base classes again.
        AbstractMetaFunctionList super_funcs;
        if (super_class) {
            bool pullDown = super_class->isPullProtectedMethodsDown();
            // Super classes can never be final
            if (super_class->isFinalInTargetLang()) {
                ReportHandler::warning("Final class '" + super_class->name() + "' set to non-final, as it is extended by other classes");
                *super_class -= AbstractMetaAttributes::FinalInTargetLang;
                *super_class += AbstractMetaAttributes::PullProtectedMethodsDown;
                pullDown = true;
            }
            if(pullDown){
                // when a class cannot be subclassed from external but has subclasses inside Qt,
                // the protected functions of the class must be pulled down to the subclasses.
                super_funcs = super_class->queryFunctions(AbstractMetaClass::Protected | AbstractMetaClass::NormalFunctions);
                for (int sfi = 0; sfi < super_funcs.size(); ++sfi) {
                    AbstractMetaFunction *sf = super_funcs.at(sfi);
                    if(!sf->isAbstract() && !sf->isConstructor() && !sf->isEmptyFunction() && !sf->isInvalid()){
                        AbstractMetaFunction *sfcopy = sf->copy();
                        sfcopy->setDeclaringClass(this);
                        this->addFunction(sfcopy);
                    }
                }
            }
            super_funcs = super_class->queryFunctions(AbstractMetaClass::ClassImplements);
        } else {
            super_funcs = interfaces().at(iface_idx)->queryFunctions(AbstractMetaClass::NormalFunctions);
        }

        QSet<AbstractMetaFunction *> funcs_to_add;
        for (int sfi = 0; sfi < super_funcs.size(); ++sfi) {
            AbstractMetaFunction *sf = super_funcs.at(sfi);

            if (sf->isRemovedFromAllLanguages(sf->implementingClass()))
                continue;

            // we generally don't care about private functions, but we have to get the ones that are
            // virtual in case they override abstract functions.
            bool add = (sf->isNormal() || sf->isSignal() || sf->isEmptyFunction());
            for (int fi = 0; fi < funcs.size(); ++fi) {
                AbstractMetaFunction *f = funcs.at(fi);
                if (f->isRemovedFromAllLanguages(f->implementingClass()))
                    continue;

                const uint cmp = f->compareTo(sf);

                if (cmp & AbstractMetaFunction::EqualModifiedName) {
//                     printf("   - %s::%s similar to %s::%s %x vs %x\n",
//                            qPrintable(sf->implementingClass()->typeEntry()->qualifiedCppName()),
//                            qPrintable(sf->name()),
//                            qPrintable(f->implementingClass()->typeEntry()->qualifiedCppName()),
//                            qPrintable(f->name()),
//                            sf->attributes(),
//                            f->attributes());

                    add = sf->isSignal();
                    if (cmp & AbstractMetaFunction::EqualArguments) {

//                         if (!(cmp & AbstractMetaFunction::EqualReturnType)) {
//                             ReportHandler::warning(QString("%1::%2 and %3::%4 differ in retur type")
//                                                    .arg(sf->implementingClass()->name())
//                                                    .arg(sf->name())
//                                                    .arg(f->implementingClass()->name())
//                                                    .arg(f->name()));
//                         }

                        // Same function, propegate virtual...
                        if (!(cmp & AbstractMetaFunction::EqualAttributes)) {
                            if (!f->isEmptyFunction()) {
                                if (!sf->isFinalInCpp() && f->isFinalInCpp() && !f->isDeclaredFinalInCpp()) {
                                    *f -= AbstractMetaAttributes::FinalInCpp;
                                    //                                 printf("   --- inherit virtual\n");
                                }
                                if (!sf->isFinalInTargetLang() && f->isFinalInTargetLang() && !f->isDeclaredFinalInCpp()) {
                                    *f -= AbstractMetaAttributes::FinalInTargetLang;
                                    //                                 printf("   --- inherit virtual\n");
                                }
                                if (!f->isFinalInTargetLang() && f->isPrivate() && !sf->isPrivate()) {
                                    f->setFunctionType(AbstractMetaFunction::EmptyFunction);
                                    f->setVisibility(AbstractMetaAttributes::Protected);
                                    *f += AbstractMetaAttributes::FinalInTargetLang;
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
                                f->setFunctionType(AbstractMetaFunction::EmptyFunction);
                                f->setVisibility(sf->visibility());
                                *f += AbstractMetaAttributes::FinalInTargetLang;
                                *f += AbstractMetaAttributes::FinalInCpp;
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

                                if (!hasNonFinalModifier && !isBaseImplPrivate && f->addedArguments().isEmpty()) {
                                    ReportHandler::warning(QString::fromLatin1("Shadowing: %1::%2 and %3::%4; Java code will not compile")
                                                           .arg(sf->implementingClass()->qualifiedCppName())
                                                           .arg(sf->signature())
                                                           .arg(f->implementingClass()->qualifiedCppName())
                                                           .arg(f->signature()));
                                }
                            }
                        }

                    }

                    if (!f->isPrivate() && cmp & AbstractMetaFunction::EqualDefaultValueOverload) {
                        const AbstractMetaArgumentList& arguments = f->arguments().size() < sf->arguments().size() ? sf->arguments() : f->arguments();

                        for (int i = 0; i < arguments.size(); ++i)
                            arguments[i]->setDefaultValueExpression(QString());
                    }


                    // Otherwise we have function shadowing and we can
                    // skip the thing...
                } else if (cmp & AbstractMetaFunction::EqualName && !sf->isSignal()) {

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
                        *f += AbstractMetaAttributes::FinalInTargetLang;
                        *f += AbstractMetaAttributes::FinalInCpp;
//                        ReportHandler::warning(QString("final overriding method %1 in class %2").arg(f->signature()).arg(f->implementingClass()->qualifiedCppName()));
                        break;
                    }
                }
            }

            if (add)
                funcs_to_add << sf;
        }

        for(AbstractMetaFunction *f : funcs_to_add)
            funcs << f->copy();

        if (super_class)
            super_class = super_class->baseClass();
        else
            iface_idx++;
    }

    bool hasPrivateConstructors = false;
    bool hasPublicConstructors = false;
    for(AbstractMetaFunction *func : funcs) {
        FunctionModificationList mods = func->modifications(this);
        for(const FunctionModification &mod : mods) {
            if (mod.isRenameModifier()) {
                func->setName(mod.renamedTo());
            }
        }

        // Make sure class is abstract if one of the functions is
        if (func->isAbstract()) {
            (*this) += AbstractMetaAttributes::Abstract;
            (*this) -= AbstractMetaAttributes::Final;
        }

        if (func->isConstructor()) {
            if (func->isPrivate())
                hasPrivateConstructors = true;
            else
                hasPublicConstructors = true;
        }
    }

    if (hasPrivateConstructors && !hasPublicConstructors) {
        (*this) += AbstractMetaAttributes::Abstract;
        (*this) -= AbstractMetaAttributes::Final;
    }

    for(AbstractMetaFunction *f1 : funcs) {
        for(AbstractMetaFunction *f2 : funcs) {
            if (f1 != f2) {
                uint cmp = f1->compareTo(f2);
                if ((cmp & AbstractMetaFunction::EqualName)
                        && !f1->isFinalInCpp()
                        && f2->isFinalInCpp()) {
                    *f2 += AbstractMetaAttributes::FinalOverload;
                }
            }
        }
    }

    setFunctions(funcs);
    for(AbstractMetaFunction *func : m_functions) {
        // Make sure that we include files for all classes that are in use

        if (!func->isRemovedFrom(this, TypeSystem::ShellCode))
            add_extra_includes_for_function(this, func);
    }
}

QString AbstractMetaType::minimalSignature() const {
    QString minimalSignature;
    if (isConstant())
        minimalSignature += "const ";
//    if(typeEntry()->qualifiedCppName()=="qtjambireal")
//        minimalSignature += "qreal";
//    else
    minimalSignature += typeEntry()->qualifiedCppName();
    if (hasInstantiations() && (!typeEntry()->isContainer()
                                || static_cast<const ContainerTypeEntry*>(typeEntry())->type()!=ContainerTypeEntry::StringListContainer)) {
        const QList<const AbstractMetaType *>& instantiations = this->instantiations();
        minimalSignature += "<";
        for (int i = 0;i < instantiations.size();++i) {
            if (i > 0)
                minimalSignature += ",";
            minimalSignature += instantiations.at(i)->minimalSignature();
        }
        minimalSignature += ">";
    }

    if (getReferenceType()==AbstractMetaType::Reference)
        minimalSignature += "&";
    if (getReferenceType()==AbstractMetaType::RReference)
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

QString AbstractMetaType::normalizedSignature() const {
    QString normalizedSignature = typeEntry()->qualifiedCppName();
    if (hasInstantiations()) {
        const QList<const AbstractMetaType *>& instantiations = this->instantiations();
        normalizedSignature += "<";
        for (int i = 0;i < instantiations.size();++i) {
            if (i > 0)
                normalizedSignature += ",";
            normalizedSignature += instantiations.at(i)->normalizedSignature();
        }
        normalizedSignature += ">";
    }

    if (getReferenceType()==AbstractMetaType::Reference && !isConstant())
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

bool AbstractMetaType::hasNativeId() const {
    return (isQObject() || isValue() || isObject() || isFunctional()) && typeEntry()->isNativeIdBased();
}


/*******************************************************************************
 * Other stuff...
 */


AbstractMetaEnum *AbstractMetaClassList::findEnum(const EnumTypeEntry *entry) const {
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
        for(AbstractMetaClass *c : *this) {
            if (c->typeEntry()->isGlobal()){
                AbstractMetaEnum *enm = c->findEnum(enum_name);
                if(enm)
                    return enm;
            }
        }
    }

    AbstractMetaClass *meta_class = findClass(class_name);
    if (!meta_class) {
        ReportHandler::warning(QString("Unknown class '%1' for enum '%2'")
                               .arg(class_name).arg(entry->qualifiedCppName()));
        return nullptr;
    }

    return meta_class->findEnum(enum_name);
}

AbstractMetaEnumValue *AbstractMetaEnumValueList::find(const QString &name) const {
    for (int i = 0; i < size(); ++i) {
        if (name == at(i)->name())
            return at(i);
    }
    return nullptr;
}

AbstractMetaEnumValue *AbstractMetaClassList::findEnumValue(const QString &name) const {
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

        AbstractMetaClass *cl = findClass(prefixName);
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

AbstractMetaClass *AbstractMetaClassList::findClass(const QString &name, NameFlag nameFlag) const {
    if (name.isEmpty())
        return nullptr;

    if(nameFlag & QualifiedCppName){
        for(AbstractMetaClass *c : *this) {
            if (c->qualifiedCppName() == name)
                return c;
        }
    }

    if(nameFlag & FullName){
        for(AbstractMetaClass *c : *this) {
            if (c->fullName() == name)
                return c;
        }
    }

    if(nameFlag & SimpleName){
        for(AbstractMetaClass *c : *this) {
            if (c->name() == name)
                return c;
        }
    }

    return nullptr;
}

QSet<QString> AbstractMetaClass::getAllUnimplmentablePureVirtualFunctions()const{
    QSet<QString> allPrivatePureVirtualFunctions;
    allPrivatePureVirtualFunctions |= m_unimplmentablePureVirtualFunctions;
    if(baseClass()){
        QSet<QString> inheritedPrivatePureVirtualFunctions = baseClass()->getAllUnimplmentablePureVirtualFunctions();
        if(!inheritedPrivatePureVirtualFunctions.isEmpty()){
            AbstractMetaFunctionList functions;
            functions << m_functions;
            functions << m_invalidfunctions;
            if(this->templateBaseClass()){
                functions << this->templateBaseClass()->m_functions;
                functions << this->templateBaseClass()->m_invalidfunctions;
            }
            for(AbstractMetaFunction* function : functions){
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
