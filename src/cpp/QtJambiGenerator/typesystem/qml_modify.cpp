#include "qml_modify.h"

ModifyArgument::ModifyArgument(QObject *parent)
    : AbstractObject{parent}
{

}

const QVariant &ModifyArgument::getIndex() const
{
    return index;
}

void ModifyArgument::setIndex(const QVariant &newIndex)
{
    if (index == newIndex)
        return;
    index = newIndex;
    emit indexChanged();
}

const QString &ModifyArgument::getReplaceValue() const
{
    return replaceValue;
}

void ModifyArgument::setReplaceValue(const QString &newReplaceValue)
{
    if (replaceValue == newReplaceValue)
        return;
    replaceValue = newReplaceValue;
    emit replaceValueChanged();
}

const QVariant &ModifyArgument::getThreadAffinity() const
{
    return threadAffinity;
}

void ModifyArgument::setThreadAffinity(const QVariant &newThreadAffinity)
{
    if (threadAffinity == newThreadAffinity)
        return;
    threadAffinity = newThreadAffinity;
    emit threadAffinityChanged();
}

const QString &ModifyArgument::getRename() const
{
    return rename;
}

void ModifyArgument::setRename(const QString &newRename)
{
    if (rename == newRename)
        return;
    rename = newRename;
    emit renameChanged();
}

const QString &ModifyArgument::getReplaceType() const
{
    return replaceType;
}

void ModifyArgument::setReplaceType(const QString &newReplaceType)
{
    if (replaceType == newReplaceType)
        return;
    replaceType = newReplaceType;
    emit replaceTypeChanged();
}

bool ModifyArgument::getInvalidateAfterUse() const
{
    return invalidateAfterUse;
}

void ModifyArgument::setInvalidateAfterUse(bool newInvalidateAfterUse)
{
    if (invalidateAfterUse == newInvalidateAfterUse)
        return;
    invalidateAfterUse = newInvalidateAfterUse;
    emit invalidateAfterUseChanged();
}

bool ModifyArgument::getValueAsPointer() const
{
    return valueAsPointer;
}

void ModifyArgument::setValueAsPointer(bool newValueAsPointer)
{
    if (valueAsPointer == newValueAsPointer)
        return;
    valueAsPointer = newValueAsPointer;
    emit valueAsPointerChanged();
}

const QString &ReplaceDefaultExpression::getExpression() const
{
    return expression;
}

void ReplaceDefaultExpression::setExpression(const QString &newExpression)
{
    if (expression == newExpression)
        return;
    expression = newExpression;
    emit expressionChanged();
}

bool ReferenceCount::getIsThreadSafe() const
{
    return isThreadSafe;
}

void ReferenceCount::setIsThreadSafe(bool newIsThreadSafe)
{
    if (isThreadSafe == newIsThreadSafe)
        return;
    isThreadSafe = newIsThreadSafe;
    emit isThreadSafeChanged();
}

const QString &ReferenceCount::getVariableName() const
{
    return variableName;
}

void ReferenceCount::setVariableName(const QString &newVariableName)
{
    if (variableName == newVariableName)
        return;
    variableName = newVariableName;
    emit variableNameChanged();
}

uint ReferenceCount::getKeyArgument() const
{
    return keyArgument;
}

void ReferenceCount::setKeyArgument(uint newKeyArgument)
{
    if (keyArgument == newKeyArgument)
        return;
    keyArgument = newKeyArgument;
    emit keyArgumentChanged();
}

const QString &ReferenceCount::getDeclareVariable() const
{
    return declareVariable;
}

void ReferenceCount::setDeclareVariable(const QString &newDeclareVariable)
{
    if (declareVariable == newDeclareVariable)
        return;
    declareVariable = newDeclareVariable;
    emit declareVariableChanged();
}

ReferenceCount::Action ReferenceCount::getAction() const
{
    return action;
}

void ReferenceCount::setAction(Action newAction)
{
    if (action == newAction)
        return;
    action = newAction;
    emit actionChanged();
}

const QString &ReferenceCount::getCondition() const
{
    return condition;
}

void ReferenceCount::setCondition(const QString &newCondition)
{
    if (condition == newCondition)
        return;
    condition = newCondition;
    emit conditionChanged();
}

AccessModifications ReferenceCount::getAccess() const
{
    return access;
}

void ReferenceCount::setAccess(AccessModifications newAccess)
{
    if (access == newAccess)
        return;
    access = newAccess;
    emit accessChanged();
}

bool ArrayType::getDeref() const
{
    return deref;
}

void ArrayType::setDeref(bool newDeref)
{
    if (deref == newDeref)
        return;
    deref = newDeref;
    emit derefChanged();
}

bool ArrayType::getVarargs() const
{
    return varargs;
}

void ArrayType::setVarargs(bool newVarargs)
{
    if (varargs == newVarargs)
        return;
    varargs = newVarargs;
    emit varargsChanged();
}

bool ArrayType::getAsBuffer() const
{
    return asBuffer;
}

void ArrayType::setAsBuffer(bool newAsBuffer)
{
    if (asBuffer == newAsBuffer)
        return;
    asBuffer = newAsBuffer;
    emit asBufferChanged();
}

int ArrayType::getLengthParameter() const
{
    return lengthParameter;
}

void ArrayType::setLengthParameter(int newLengthParameter)
{
    if (lengthParameter == newLengthParameter)
        return;
    lengthParameter = newLengthParameter;
    emit lengthParameterChanged();
}

int ArrayType::getMinLength() const
{
    return minLength;
}

void ArrayType::setMinLength(int newMinLength)
{
    if (minLength == newMinLength)
        return;
    minLength = newMinLength;
    emit minLengthChanged();
}

int ArrayType::getMaxLength() const
{
    return maxLength;
}

void ArrayType::setMaxLength(int newMaxLength)
{
    if (maxLength == newMaxLength)
        return;
    maxLength = newMaxLength;
    emit maxLengthChanged();
}

const QString &ReplaceType::getModifiedType() const
{
    return modifiedType;
}

void ReplaceType::setModifiedType(const QString &newModifiedType)
{
    if (modifiedType == newModifiedType)
        return;
    modifiedType = newModifiedType;
    emit modifiedTypeChanged();
}

const QString &ReplaceType::getModifiedJniType() const
{
    return modifiedJniType;
}

void ReplaceType::setModifiedJniType(const QString &newModifiedJniType)
{
    if (modifiedJniType == newModifiedJniType)
        return;
    modifiedJniType = newModifiedJniType;
    emit modifiedJniTypeChanged();
}

const QString &NoNullPointer::getDefaultValue() const
{
    return defaultValue;
}

void NoNullPointer::setDefaultValue(const QString &newDefaultValue)
{
    if (defaultValue == newDefaultValue)
        return;
    defaultValue = newDefaultValue;
    emit defaultValueChanged();
}

CodeClass::Entries DefineOwnership::getCodeClass() const
{
    return codeClass;
}

void DefineOwnership::setCodeClass(CodeClass::Entries newCodeClass)
{
    if (codeClass == newCodeClass)
        return;
    codeClass = newCodeClass;
    emit codeClassChanged();
}

Ownership::Entries DefineOwnership::getOwnership() const
{
    return owner;
}

void DefineOwnership::setOwnership(Ownership::Entries newOwner)
{
    if (owner == newOwner)
        return;
    owner = newOwner;
    emit ownershipChanged();
}

const QString &DefineOwnership::getCondition() const
{
    return condition;
}

void DefineOwnership::setCondition(const QString &newCondition)
{
    if (condition == newCondition)
        return;
    condition = newCondition;
    emit conditionChanged();
}

CodeClass::Entries ConversionRule::getCodeClass() const
{
    return codeClass;
}

void ConversionRule::setCodeClass(CodeClass::Entries newCodeClass)
{
    if (codeClass == newCodeClass)
        return;
    codeClass = newCodeClass;
    emit codeClassChanged();
}

AccessModifications Instantiation::getAccess() const
{
    return access;
}

void Instantiation::setAccess(AccessModifications newAccess)
{
    if (access == newAccess)
        return;
    access = newAccess;
    emit accessChanged();
}

const QString &Instantiation::getRename() const
{
    return rename;
}

void Instantiation::setRename(const QString &newRename)
{
    if (rename == newRename)
        return;
    rename = newRename;
    emit renameChanged();
}

bool Instantiation::getNoExcept() const
{
    return noExcept;
}

void Instantiation::setNoExcept(bool newNoExcept)
{
    if (noExcept == newNoExcept)
        return;
    noExcept = newNoExcept;
    emit noExceptChanged();
}

bool Instantiation::getBlockExceptions() const
{
    return blockExceptions;
}

void Instantiation::setBlockExceptions(bool newBlockExceptions)
{
    if (blockExceptions == newBlockExceptions)
        return;
    blockExceptions = newBlockExceptions;
    emit blockExceptionsChanged();
}

bool Instantiation::getRethrowExceptions() const
{
    return rethrowExceptions;
}

void Instantiation::setRethrowExceptions(bool newRethrowExceptions)
{
    if (rethrowExceptions == newRethrowExceptions)
        return;
    rethrowExceptions = newRethrowExceptions;
    emit rethrowExceptionsChanged();
}

bool Instantiation::getDeprecated() const
{
    return deprecated;
}

void Instantiation::setDeprecated(bool newDeprecated)
{
    if (deprecated == newDeprecated)
        return;
    deprecated = newDeprecated;
    emit deprecatedChanged();
}

bool Instantiation::getVirtualSlot() const
{
    return virtualSlot;
}

void Instantiation::setVirtualSlot(bool newVirtualSlot)
{
    if (virtualSlot == newVirtualSlot)
        return;
    virtualSlot = newVirtualSlot;
    emit virtualSlotChanged();
}

const QString &Instantiation::getAssociatedTo() const
{
    return associatedTo;
}

void Instantiation::setAssociatedTo(const QString &newAssociatedTo)
{
    if (associatedTo == newAssociatedTo)
        return;
    associatedTo = newAssociatedTo;
    emit associatedToChanged();
}

bool Instantiation::getAllowAsSlot() const
{
    return allowAsSlot;
}

void Instantiation::setAllowAsSlot(bool newAllowAsSlot)
{
    if (allowAsSlot == newAllowAsSlot)
        return;
    allowAsSlot = newAllowAsSlot;
    emit allowAsSlotChanged();
}

bool Instantiation::getPrivateSignal() const
{
    return privateSignal;
}

void Instantiation::setPrivateSignal(bool newPrivateSignal)
{
    if (privateSignal == newPrivateSignal)
        return;
    privateSignal = newPrivateSignal;
    emit privateSignalChanged();
}

const QVariant &Instantiation::getThreadAffinity() const
{
    return threadAffinity;
}

void Instantiation::setThreadAffinity(const QVariant &newThreadAffinity)
{
    if (threadAffinity == newThreadAffinity)
        return;
    threadAffinity = newThreadAffinity;
    emit threadAffinityChanged();
}

const QString &Instantiation::getPpCondition() const
{
    return ppCondition;
}

void Instantiation::setPpCondition(const QString &newPpCondition)
{
    if (ppCondition == newPpCondition)
        return;
    ppCondition = newPpCondition;
    emit ppConditionChanged();
}

const QString &Instantiation::getThrowing() const
{
    return throwing;
}

void Instantiation::setThrowing(const QString &newThrowing)
{
    if (throwing == newThrowing)
        return;
    throwing = newThrowing;
    emit throwingChanged();
}

const QString &Instantiation::getProxyCall() const
{
    return proxyCall;
}

void Instantiation::setProxyCall(const QString &newProxyCall)
{
    if (proxyCall == newProxyCall)
        return;
    proxyCall = newProxyCall;
    emit proxyCallChanged();
}

const QString &Instantiation::getTargetType() const
{
    return targetType;
}

void Instantiation::setTargetType(const QString &newTargetType)
{
    if (targetType == newTargetType)
        return;
    targetType = newTargetType;
    emit targetTypeChanged();
}

const QString &Argument::getType() const
{
    return type;
}

void Argument::setType(const QString &newType)
{
    if (type == newType)
        return;
    type = newType;
    emit typeChanged();
}

const QString &Argument::getParameter() const
{
    return parameter;
}

void Argument::setParameter(const QString &newParameter)
{
    if (parameter == newParameter)
        return;
    parameter = newParameter;
    emit parameterChanged();
}

const QString &Argument::getExtending() const
{
    return extending;
}

void Argument::setExtending(const QString &newExtending)
{
    if (extending == newExtending)
        return;
    extending = newExtending;
    emit extendingChanged();
}

bool Argument::getIsImplicit() const
{
    return isImplicit;
}

void Argument::setIsImplicit(bool newIsImplicit)
{
    if (isImplicit == newIsImplicit)
        return;
    isImplicit = newIsImplicit;
    emit isImplicitChanged();
}

const QString &Rename::getTo() const
{
    return to;
}

void Rename::setTo(const QString &newTo)
{
    if (to == newTo)
        return;
    to = newTo;
    emit toChanged();
}

AccessModifications Access::getModifier() const
{
    return modifier;
}

void Access::setModifier(AccessModifications newModifier)
{
    if (modifier == newModifier)
        return;
    modifier = newModifier;
    emit modifierChanged();
}

int AddArgument::getIndex() const
{
    return index;
}

void AddArgument::setIndex(int newIndex)
{
    if (index == newIndex)
        return;
    index = newIndex;
    emit indexChanged();
}

const QString &AddArgument::getDefaultExpression() const
{
    return defaultExpression;
}

void AddArgument::setDefaultExpression(const QString &newDefaultExpression)
{
    if (defaultExpression == newDefaultExpression)
        return;
    defaultExpression = newDefaultExpression;
    emit defaultExpressionChanged();
}

const QString &AddArgument::getName() const
{
    return name;
}

void AddArgument::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

const QString &AddArgument::getType() const
{
    return type;
}

void AddArgument::setType(const QString &newType)
{
    if (type == newType)
        return;
    type = newType;
    emit typeChanged();
}

const QString &AddArgument::getJniType() const
{
    return jniType;
}

void AddArgument::setJniType(const QString &newJniType)
{
    if (jniType == newJniType)
        return;
    jniType = newJniType;
    emit jniTypeChanged();
}

bool AddArgument::getInvalidateAfterUse() const
{
    return invalidateAfterUse;
}

void AddArgument::setInvalidateAfterUse(bool newInvalidateAfterUse)
{
    if (invalidateAfterUse == newInvalidateAfterUse)
        return;
    invalidateAfterUse = newInvalidateAfterUse;
    emit invalidateAfterUseChanged();
}

const QString &AddTypeParameter::getName() const
{
    return name;
}

void AddTypeParameter::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

const QString &AddTypeParameter::getExtending() const
{
    return extending;
}

void AddTypeParameter::setExtending(const QString &newExtending)
{
    if (extending == newExtending)
        return;
    extending = newExtending;
    emit extendingChanged();
}

const QString &ModifyFunction::getSignature() const
{
    return signature;
}

void ModifyFunction::setSignature(const QString &newSignature)
{
    if (signature == newSignature)
        return;
    signature = newSignature;
    emit signatureChanged();
}

AccessModifications ModifyFunction::getAccess() const
{
    return access;
}

void ModifyFunction::setAccess(AccessModifications newAccess)
{
    if (access == newAccess)
        return;
    access = newAccess;
    emit accessChanged();
}

bool ModifyFunction::getNoExcept() const
{
    return noExcept;
}

void ModifyFunction::setNoExcept(bool newNoExcept)
{
    if (noExcept == newNoExcept)
        return;
    noExcept = newNoExcept;
    emit noExceptChanged();
}

bool ModifyFunction::getBlockExceptions() const
{
    return blockExceptions;
}

void ModifyFunction::setBlockExceptions(bool newBlockExceptions)
{
    if (blockExceptions == newBlockExceptions)
        return;
    blockExceptions = newBlockExceptions;
    emit blockExceptionsChanged();
}

bool ModifyFunction::getRethrowExceptions() const
{
    return rethrowExceptions;
}

void ModifyFunction::setRethrowExceptions(bool newRethrowExceptions)
{
    if (rethrowExceptions == newRethrowExceptions)
        return;
    rethrowExceptions = newRethrowExceptions;
    emit rethrowExceptionsChanged();
}

const QString &ModifyFunction::getRename() const
{
    return rename;
}

void ModifyFunction::setRename(const QString &newRename)
{
    if (rename == newRename)
        return;
    rename = newRename;
    emit renameChanged();
}

RemoveFlag::Entries ModifyFunction::getRemove() const
{
    return remove;
}

void ModifyFunction::setRemove(RemoveFlag::Entries newRemove)
{
    if (remove == newRemove)
        return;
    remove = newRemove;
    emit removeChanged();
}

bool ModifyFunction::getDeprecated() const
{
    return deprecated;
}

void ModifyFunction::setDeprecated(bool newDeprecated)
{
    if (deprecated == newDeprecated)
        return;
    deprecated = newDeprecated;
    emit deprecatedChanged();
}

const QString &ModifyFunction::getAssociatedTo() const
{
    return associatedTo;
}

void ModifyFunction::setAssociatedTo(const QString &newAssociatedTo)
{
    if (associatedTo == newAssociatedTo)
        return;
    associatedTo = newAssociatedTo;
    emit associatedToChanged();
}

bool ModifyFunction::getVirtualSlot() const
{
    return virtualSlot;
}

void ModifyFunction::setVirtualSlot(bool newVirtualSlot)
{
    if (virtualSlot == newVirtualSlot)
        return;
    virtualSlot = newVirtualSlot;
    emit virtualSlotChanged();
}

bool ModifyFunction::getAllowAsSlot() const
{
    return allowAsSlot;
}

void ModifyFunction::setAllowAsSlot(bool newAllowAsSlot)
{
    if (allowAsSlot == newAllowAsSlot)
        return;
    allowAsSlot = newAllowAsSlot;
    emit allowAsSlotChanged();
}

bool ModifyFunction::getPrivateSignal() const
{
    return privateSignal;
}

void ModifyFunction::setPrivateSignal(bool newPrivateSignal)
{
    if (privateSignal == newPrivateSignal)
        return;
    privateSignal = newPrivateSignal;
    emit privateSignalChanged();
}

const QVariant &ModifyFunction::getThreadAffinity() const
{
    return threadAffinity;
}

void ModifyFunction::setThreadAffinity(const QVariant &newThreadAffinity)
{
    if (threadAffinity == newThreadAffinity)
        return;
    threadAffinity = newThreadAffinity;
    emit threadAffinityChanged();
}

const QString &ModifyFunction::getPpCondition() const
{
    return ppCondition;
}

void ModifyFunction::setPpCondition(const QString &newPpCondition)
{
    if (ppCondition == newPpCondition)
        return;
    ppCondition = newPpCondition;
    emit ppConditionChanged();
}

const QString &ModifyFunction::getThrowing() const
{
    return throwing;
}

void ModifyFunction::setThrowing(const QString &newThrowing)
{
    if (throwing == newThrowing)
        return;
    throwing = newThrowing;
    emit throwingChanged();
}

const QString &ModifyFunction::getProxyCall() const
{
    return proxyCall;
}

void ModifyFunction::setProxyCall(const QString &newProxyCall)
{
    if (proxyCall == newProxyCall)
        return;
    proxyCall = newProxyCall;
    emit proxyCallChanged();
}

const QString &ModifyFunction::getTargetType() const
{
    return targetType;
}

void ModifyFunction::setTargetType(const QString &newTargetType)
{
    if (targetType == newTargetType)
        return;
    targetType = newTargetType;
    emit targetTypeChanged();
}

bool ModifyFunction::getIsPaintMethod() const
{
    return isPaintMethod;
}

void ModifyFunction::setIsPaintMethod(bool newIsPaintMethod)
{
    if (isPaintMethod == newIsPaintMethod)
        return;
    isPaintMethod = newIsPaintMethod;
    emit isPaintMethodChanged();
}

bool ModifyFunction::noKotlinGetter() const
{
    return m_noKotlinGetter;
}

void ModifyFunction::setNoKotlinGetter(bool newNoKotlinGetter)
{
    if (m_noKotlinGetter == newNoKotlinGetter)
        return;
    m_noKotlinGetter = newNoKotlinGetter;
    emit noKotlinGetterChanged();
}

RemoveFlag::Entries Remove::getCodeClass() const
{
    return codeClass;
}

void Remove::setCodeClass(RemoveFlag::Entries newCodeClass)
{
    if (codeClass == newCodeClass)
        return;
    codeClass = newCodeClass;
    emit codeClassChanged();
}

const QString &ModifyField::getName() const
{
    return name;
}

void ModifyField::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

bool ModifyField::getRead() const
{
    return read;
}

void ModifyField::setRead(bool newRead)
{
    if (read == newRead)
        return;
    read = newRead;
    emit readChanged();
}

bool ModifyField::getWrite() const
{
    return write;
}

void ModifyField::setWrite(bool newWrite)
{
    if (write == newWrite)
        return;
    write = newWrite;
    emit writeChanged();
}

AccessModifications ModifyField::getAccess() const
{
    return access;
}

void ModifyField::setAccess(AccessModifications newAccess)
{
    if (access == newAccess)
        return;
    access = newAccess;
    emit accessChanged();
}

const QString &ModifyField::getRename() const
{
    return rename;
}

void ModifyField::setRename(const QString &newRename)
{
    if (rename == newRename)
        return;
    rename = newRename;
    emit renameChanged();
}

bool ModifyField::noKotlinGetter() const
{
    return m_noKotlinGetter;
}

void ModifyField::setNoKotlinGetter(bool newNoKotlinGetter)
{
    if (m_noKotlinGetter == newNoKotlinGetter)
        return;
    m_noKotlinGetter = newNoKotlinGetter;
    emit noKotlinGetterChanged();
}

const QString &RejectEnumValue::getName() const
{
    return name;
}

void RejectEnumValue::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

bool RejectEnumValue::getRemove() const
{
    return remove;
}

void RejectEnumValue::setRemove(bool newRemove)
{
    if (remove == newRemove)
        return;
    remove = newRemove;
    emit removeChanged();
}

const QString &RenameEnumValue::getName() const
{
    return name;
}

void RenameEnumValue::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

const QString &RenameEnumValue::getRename() const
{
    return rename;
}

void RenameEnumValue::setRename(const QString &newRename)
{
    if (rename == newRename)
        return;
    rename = newRename;
    emit renameChanged();
}

const QString &DelegateBaseClass::getBaseClass() const
{
    return baseClass;
}

void DelegateBaseClass::setBaseClass(const QString &newBaseClass)
{
    if (baseClass == newBaseClass)
        return;
    baseClass = newBaseClass;
    emit baseClassChanged();
}

const QString &DelegateBaseClass::getDelegate() const
{
    return delegate;
}

void DelegateBaseClass::setDelegate(const QString &newDelegate)
{
    if (delegate == newDelegate)
        return;
    delegate = newDelegate;
    emit delegateChanged();
}

const QString &InterfacePolymorphy::getInterfaceType() const
{
    return interfaceType;
}

void InterfacePolymorphy::setInterfaceType(const QString &newInterfaceType)
{
    if (interfaceType == newInterfaceType)
        return;
    interfaceType = newInterfaceType;
    emit interfaceTypeChanged();
}

const QString &InterfacePolymorphy::getCondition() const
{
    return condition;
}

void InterfacePolymorphy::setCondition(const QString &newCondition)
{
    if (condition == newCondition)
        return;
    condition = newCondition;
    emit conditionChanged();
}

const QString &AbstractStructor::getName() const
{
    return name;
}

void AbstractStructor::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

const QString &AbstractStructor::getParamName() const
{
    return paramName;
}

void AbstractStructor::setParamName(const QString &newParamName)
{
    if (paramName == newParamName)
        return;
    paramName = newParamName;
    emit paramNameChanged();
}

const QString &CustomConstructor::getPlacementName() const
{
    return placementName;
}

void CustomConstructor::setPlacementName(const QString &newPlacementName)
{
    if (placementName == newPlacementName)
        return;
    placementName = newPlacementName;
    emit placementNameChanged();
}

CustomConstructor::ConstructorType CustomConstructor::getType() const
{
    return type;
}

void CustomConstructor::setType(ConstructorType newType)
{
    if (type == newType)
        return;
    type = newType;
    emit typeChanged();
}

const QString &Import::getTemplate() const
{
    return m_template;
}

void Import::setTemplate(const QString &newTemplate)
{
    if (m_template == newTemplate)
        return;
    m_template = newTemplate;
    emit templateChanged();
}

const QString &Implements::getInterface() const
{
    return interface;
}

void Implements::setInterface(const QString &newInterface)
{
    if (interface == newInterface)
        return;
    interface = newInterface;
    emit interfaceChanged();
}

QString Delegate::name() const
{
    return m_name;
}

void Delegate::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

bool Delegate::isDeprecated() const
{
    return m_isDeprecated;
}

void Delegate::setDeprecated(bool newIsDeprecated)
{
    if (m_isDeprecated == newIsDeprecated)
        return;
    m_isDeprecated = newIsDeprecated;
    emit isDeprecatedChanged();
}

AccessModifications Delegate::access() const
{
    return m_access;
}

void Delegate::setAccess(const AccessModifications &newAccess)
{
    if (m_access == newAccess)
        return;
    m_access = newAccess;
    emit accessChanged();
}

bool Delegate::isSelfReturning() const
{
    return m_isSelfReturning;
}

void Delegate::setIsSelfReturning(bool newIsSelfReturning)
{
    if (m_isSelfReturning == newIsSelfReturning)
        return;
    m_isSelfReturning = newIsSelfReturning;
    emit isSelfReturningChanged();
}

AddImpliciteCall::AddImpliciteCall(QObject *parent) : AbstractObject(parent){}

QString AddImpliciteCall::getType() const
{
    return type;
}

void AddImpliciteCall::setType(const QString &newType)
{
    if (type == newType)
        return;
    type = newType;
    emit typeChanged();
}
