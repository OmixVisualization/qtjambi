#include "qml_types.h"

const QString &PrimitiveType::getJavaName() const
{
    return javaName;
}

void PrimitiveType::setJavaName(const QString &newJavaName)
{
    if (javaName == newJavaName)
        return;
    javaName = newJavaName;
    emit javaNameChanged();
}

const QString &PrimitiveType::getJniName() const
{
    return jniName;
}

void PrimitiveType::setJniName(const QString &newJniName)
{
    if (jniName == newJniName)
        return;
    jniName = newJniName;
    emit jniNameChanged();
}

bool PrimitiveType::getPreferredConversion() const
{
    return preferredConversion;
}

void PrimitiveType::setPreferredConversion(bool newPreferredConversion)
{
    if (preferredConversion == newPreferredConversion)
        return;
    preferredConversion = newPreferredConversion;
    emit preferredConversionChanged();
}

bool PrimitiveType::getPreferredJavaType() const
{
    return preferredJavaType;
}

void PrimitiveType::setPreferredJavaType(bool newPreferredJavaType)
{
    if (preferredJavaType == newPreferredJavaType)
        return;
    preferredJavaType = newPreferredJavaType;
    emit preferredJavaTypeChanged();
}

ComplexType::ComplexType(QObject *parent)
    : AbstractType{parent}
{

}

const QString &ComplexType::getPackageName() const
{
    return packageName;
}

void ComplexType::setPackageName(const QString &newPackageName)
{
    if (packageName == newPackageName)
        return;
    packageName = newPackageName;
    emit packageNameChanged();
}

const QString &ComplexType::getImplementing() const
{
    return implementing;
}

void ComplexType::setImplementing(const QString &newImplementing)
{
    if (implementing == newImplementing)
        return;
    implementing = newImplementing;
    emit implementingChanged();
}

const QString &ComplexType::getUsing() const
{
    return m_using;
}

void ComplexType::setUsing(const QString &newUsing)
{
    if (m_using == newUsing)
        return;
    m_using = newUsing;
    emit usingChanged();
}

const QString &ComplexType::getJavaName() const
{
    return javaName;
}

void ComplexType::setJavaName(const QString &newJavaName)
{
    if (javaName == newJavaName)
        return;
    javaName = newJavaName;
    emit javaNameChanged();
}

const QString &ComplexType::getDefaultSuperClass() const
{
    return defaultSuperClass;
}

void ComplexType::setDefaultSuperClass(const QString &newDefaultSuperClass)
{
    if (defaultSuperClass == newDefaultSuperClass)
        return;
    defaultSuperClass = newDefaultSuperClass;
    emit defaultSuperClassChanged();
}

bool ComplexType::getIsGeneric() const
{
    return isGeneric;
}

void ComplexType::setIsGeneric(bool newIsGeneric)
{
    if (isGeneric == newIsGeneric)
        return;
    isGeneric = newIsGeneric;
    emit isGenericChanged();
}

const QString &ComplexType::getTargetType() const
{
    return targetType;
}

void ComplexType::setTargetType(const QString &newTargetType)
{
    if (targetType == newTargetType)
        return;
    targetType = newTargetType;
    emit targetTypeChanged();
}

bool ComplexType::getDisableNativeIdUsage() const
{
    return disableNativeIdUsage;
}

void ComplexType::setDisableNativeIdUsage(bool newDisableNativeIdUsage)
{
    if (disableNativeIdUsage == newDisableNativeIdUsage)
        return;
    disableNativeIdUsage = newDisableNativeIdUsage;
    emit disableNativeIdUsageChanged();
}

bool ComplexType::getForceAbstract() const
{
    return forceAbstract;
}

void ComplexType::setForceAbstract(bool newForceAbstract)
{
    if (forceAbstract == newForceAbstract)
        return;
    forceAbstract = newForceAbstract;
    emit forceAbstractChanged();
}

bool ComplexType::getDeprecated() const
{
    return deprecated;
}

void ComplexType::setDeprecated(bool newDeprecated)
{
    if (deprecated == newDeprecated)
        return;
    deprecated = newDeprecated;
    emit deprecatedChanged();
}

bool ComplexType::getForceFriendly() const
{
    return forceFriendly;
}

void ComplexType::setForceFriendly(bool newForceFriendly)
{
    if (forceFriendly == newForceFriendly)
        return;
    forceFriendly = newForceFriendly;
    emit forceFriendlyChanged();
}

const QString &ComplexType::getThreadAffinity() const
{
    return threadAffinity;
}

void ComplexType::setThreadAffinity(const QString &newThreadAffinity)
{
    if (threadAffinity == newThreadAffinity)
        return;
    threadAffinity = newThreadAffinity;
    emit threadAffinityChanged();
}

bool ComplexType::getIsNativeInterface() const
{
    return isNativeInterface;
}

void ComplexType::setIsNativeInterface(bool newIsNativeInterface)
{
    if (isNativeInterface == newIsNativeInterface)
        return;
    isNativeInterface = newIsNativeInterface;
    emit isNativeInterfaceChanged();
}

bool ComplexType::getTemplate() const
{
    return m_template;
}

void ComplexType::setTemplate(bool newTemplate)
{
    if (m_template == newTemplate)
        return;
    m_template = newTemplate;
    emit templateChanged();
}

const QVariant &ComplexType::getGenerate() const
{
    return generate;
}

void ComplexType::setGenerate(const QVariant &newGenerate)
{
    if (generate == newGenerate)
        return;
    generate = newGenerate;
    emit generateChanged();
}

const QString &ComplexType::getExtendType() const
{
    return extendType;
}

void ComplexType::setExtendType(const QString &newExtendType)
{
    if (extendType == newExtendType)
        return;
    extendType = newExtendType;
    emit extendTypeChanged();
}

const QString &ComplexType::getPpCondition() const
{
    return ppCondition;
}

void ComplexType::setPpCondition(const QString &newPpCondition)
{
    if (ppCondition == newPpCondition)
        return;
    ppCondition = newPpCondition;
    emit ppConditionChanged();
}

bool ObjectType::getIsValueOwner() const
{
    return isValueOwner;
}

void ObjectType::setIsValueOwner(bool newIsValueOwner)
{
    if (isValueOwner == newIsValueOwner)
        return;
    isValueOwner = newIsValueOwner;
    emit isValueOwnerChanged();
}

bool ObjectType::getIsPolymorphicBase() const
{
    return isPolymorphicBase;
}

void ObjectType::setIsPolymorphicBase(bool newIsPolymorphicBase)
{
    if (isPolymorphicBase == newIsPolymorphicBase)
        return;
    isPolymorphicBase = newIsPolymorphicBase;
    emit isPolymorphicBaseChanged();
}

const QString &ObjectType::getPolymorphicIdExpression() const
{
    return polymorphicIdExpression;
}

void ObjectType::setPolymorphicIdExpression(const QString &newPolymorphicIdExpression)
{
    if (polymorphicIdExpression == newPolymorphicIdExpression)
        return;
    polymorphicIdExpression = newPolymorphicIdExpression;
    emit polymorphicIdExpressionChanged();
}

const QVariant &FunctionalType::getGenerate() const
{
    return generate;
}

void FunctionalType::setGenerate(const QVariant &newGenerate)
{
//    if (generate == newGenerate)
//        return;
    generate = newGenerate;
    emit generateChanged();
}

const QString &FunctionalType::getPackageName() const
{
    return packageName;
}

void FunctionalType::setPackageName(const QString &newPackageName)
{
    if (packageName == newPackageName)
        return;
    packageName = newPackageName;
    emit packageNameChanged();
}

const QString &FunctionalType::getImplementing() const
{
    return implementing;
}

void FunctionalType::setImplementing(const QString &newImplementing)
{
    if (implementing == newImplementing)
        return;
    implementing = newImplementing;
    emit implementingChanged();
}

const QString &FunctionalType::getPpCondition() const
{
    return ppCondition;
}

void FunctionalType::setPpCondition(const QString &newPpCondition)
{
    if (ppCondition == newPpCondition)
        return;
    ppCondition = newPpCondition;
    emit ppConditionChanged();
}

const QString &FunctionalType::getUsing() const
{
    return m_using;
}

void FunctionalType::setUsing(const QString &newUsing)
{
    if (m_using == newUsing)
        return;
    m_using = newUsing;
    emit usingChanged();
}

const QString &FunctionalType::getJavaName() const
{
    return javaName;
}

void FunctionalType::setJavaName(const QString &newJavaName)
{
    if (javaName == newJavaName)
        return;
    javaName = newJavaName;
    emit javaNameChanged();
}

int FunctionalType::getCount() const
{
    return count;
}

void FunctionalType::setCount(int newCount)
{
    if (count == newCount)
        return;
    count = newCount;
    emit countChanged();
}

bool FunctionalType::getNoExcept() const
{
    return noExcept;
}

void FunctionalType::setNoExcept(bool newNoExcept)
{
    if (noExcept == newNoExcept)
        return;
    noExcept = newNoExcept;
    emit noExceptChanged();
}

bool FunctionalType::getBlockExceptions() const
{
    return blockExceptions;
}

void FunctionalType::setBlockExceptions(bool newBlockExceptions)
{
    if (blockExceptions == newBlockExceptions)
        return;
    blockExceptions = newBlockExceptions;
    emit blockExceptionsChanged();
}

bool FunctionalType::getRethrowExceptions() const
{
    return rethrowExceptions;
}

void FunctionalType::setRethrowExceptions(bool newRethrowExceptions)
{
    if (rethrowExceptions == newRethrowExceptions)
        return;
    rethrowExceptions = newRethrowExceptions;
    emit rethrowExceptionsChanged();
}

const QString &FunctionalType::getFunctionName() const
{
    return functionName;
}

void FunctionalType::setFunctionName(const QString &newFunctionName)
{
    if (functionName == newFunctionName)
        return;
    functionName = newFunctionName;
    emit functionNameChanged();
}

bool FunctionalType::getDisableNativeIdUsage() const
{
    return disableNativeIdUsage;
}

void FunctionalType::setDisableNativeIdUsage(bool newDisableNativeIdUsage)
{
    if (disableNativeIdUsage == newDisableNativeIdUsage)
        return;
    disableNativeIdUsage = newDisableNativeIdUsage;
    emit disableNativeIdUsageChanged();
}

bool IteratorType::getGenericClass() const
{
    return genericClass;
}

void IteratorType::setGenericClass(bool newGenericClass)
{
    if (genericClass == newGenericClass)
        return;
    genericClass = newGenericClass;
    emit genericClassChanged();
}

bool IteratorType::getIsConst() const
{
    return isConst;
}

void IteratorType::setIsConst(bool newIsConst)
{
    if (isConst == newIsConst)
        return;
    isConst = newIsConst;
    emit isConstChanged();
}

bool ComplexType::getNoMetaType() const
{
    return noMetaType;
}

void ComplexType::setNoMetaType(bool newNoMetaType)
{
    if (noMetaType == newNoMetaType)
        return;
    noMetaType = newNoMetaType;
    emit noMetaTypeChanged();
}

bool ValueType::getIsPolymorphicBase() const
{
    return isPolymorphicBase;
}

void ValueType::setIsPolymorphicBase(bool newIsPolymorphicBase)
{
    if (isPolymorphicBase == newIsPolymorphicBase)
        return;
    isPolymorphicBase = newIsPolymorphicBase;
    emit isPolymorphicBaseChanged();
}

const QString &ValueType::getPolymorphicIdExpression() const
{
    return polymorphicIdExpression;
}

void ValueType::setPolymorphicIdExpression(const QString &newPolymorphicIdExpression)
{
    if (polymorphicIdExpression == newPolymorphicIdExpression)
        return;
    polymorphicIdExpression = newPolymorphicIdExpression;
    emit polymorphicIdExpressionChanged();
}

bool InterfaceType::getIsValue() const
{
    return isValue;
}

void InterfaceType::setIsValue(bool newIsValue)
{
    if (isValue == newIsValue)
        return;
    isValue = newIsValue;
    emit isValueChanged();
}

bool InterfaceType::getNoImpl() const
{
    return noImpl;
}

void InterfaceType::setNoImpl(bool newNoImpl)
{
    if (noImpl == newNoImpl)
        return;
    noImpl = newNoImpl;
    emit noImplChanged();
}

const QString &EnumType::getPackageName() const
{
    return packageName;
}

void EnumType::setPackageName(const QString &newPackageName)
{
    if (packageName == newPackageName)
        return;
    packageName = newPackageName;
    emit packageNameChanged();
}

const QString &EnumType::getFlags() const
{
    return flags;
}

void EnumType::setFlags(const QString &newFlags)
{
    if (flags == newFlags)
        return;
    flags = newFlags;
    emit flagsChanged();
}

bool EnumType::getGenerate() const
{
    return generate;
}

void EnumType::setGenerate(bool newGenerate)
{
    if (generate == newGenerate)
        return;
    generate = newGenerate;
    emit generateChanged();
}

const QString &EnumType::getPpCondition() const
{
    return ppCondition;
}

void EnumType::setPpCondition(const QString &newPpCondition)
{
    if (ppCondition == newPpCondition)
        return;
    ppCondition = newPpCondition;
    emit ppConditionChanged();
}

const QString &EnumType::getUpperBound() const
{
    return upperBound;
}

void EnumType::setUpperBound(const QString &newUpperBound)
{
    if (upperBound == newUpperBound)
        return;
    upperBound = newUpperBound;
    emit upperBoundChanged();
}

const QString &EnumType::getLowerBound() const
{
    return lowerBound;
}

void EnumType::setLowerBound(const QString &newLowerBound)
{
    if (lowerBound == newLowerBound)
        return;
    lowerBound = newLowerBound;
    emit lowerBoundChanged();
}

const QString &EnumType::getJavaScope() const
{
    return javaScope;
}

void EnumType::setJavaScope(const QString &newJavaScope)
{
    if (javaScope == newJavaScope)
        return;
    javaScope = newJavaScope;
    emit javaScopeChanged();
}

const QString &EnumType::getImplementing() const
{
    return implementing;
}

void EnumType::setImplementing(const QString &newImplementing)
{
    if (implementing == newImplementing)
        return;
    implementing = newImplementing;
    emit implementingChanged();
}

bool EnumType::getForceInteger() const
{
    return forceInteger;
}

void EnumType::setForceInteger(bool newForceInteger)
{
    if (forceInteger == newForceInteger)
        return;
    forceInteger = newForceInteger;
    emit forceIntegerChanged();
}

bool EnumType::getExtensible() const
{
    return extensible;
}

void EnumType::setExtensible(bool newExtensible)
{
    if (extensible == newExtensible)
        return;
    extensible = newExtensible;
    emit extensibleChanged();
}

bool EnumType::getHiddenMetaobject() const
{
    return hiddenMetaobject;
}

void EnumType::setHiddenMetaobject(bool newHiddenMetaobject)
{
    if (hiddenMetaobject == newHiddenMetaobject)
        return;
    hiddenMetaobject = newHiddenMetaobject;
    emit hiddenMetaobjectChanged();
}

const QString &AbstractType::getName() const
{
    return name;
}

void AbstractType::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}
