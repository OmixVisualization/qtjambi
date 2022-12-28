#ifndef PRIMITIVETYPE_H
#define PRIMITIVETYPE_H

#include "qml_abstractobject.h"

class AbstractType : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AbstractType(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getName() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private:
    QString name;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_DISABLE_COPY(AbstractType)
};

class TemplateType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TemplateType(QObject *parent = nullptr):AbstractType{parent}{}
signals:
private:
    Q_DISABLE_COPY(TemplateType)
};

class PrimitiveType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit PrimitiveType(QObject *parent = nullptr):AbstractType{parent}{}
    const QString &getJavaName() const;
    void setJavaName(const QString &newJavaName);

    const QString &getJniName() const;
    void setJniName(const QString &newJniName);

    bool getPreferredConversion() const;
    void setPreferredConversion(bool newPreferredConversion);

    bool getPreferredJavaType() const;
    void setPreferredJavaType(bool newPreferredJavaType);

signals:

    void javaNameChanged();

    void jniNameChanged();

    void preferredConversionChanged();

    void preferredJavaTypeChanged();

private:
    QString javaName;
    QString jniName;
    bool preferredConversion = true;
    bool preferredJavaType = true;

    Q_PROPERTY(QString javaName READ getJavaName WRITE setJavaName NOTIFY javaNameChanged)
    Q_PROPERTY(QString jniName READ getJniName WRITE setJniName NOTIFY jniNameChanged)
    Q_PROPERTY(bool preferredConversion READ getPreferredConversion WRITE setPreferredConversion NOTIFY preferredConversionChanged)
    Q_PROPERTY(bool preferredJavaType READ getPreferredJavaType WRITE setPreferredJavaType NOTIFY preferredJavaTypeChanged)
};

class ComplexType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ComplexType(QObject *parent = nullptr);
    const QString &getPackageName() const;
    void setPackageName(const QString &newPackageName);

    const QString &getImplementing() const;
    void setImplementing(const QString &newImplementing);

    const QString &getUsing() const;
    void setUsing(const QString &newUsing);

    const QString &getJavaName() const;
    void setJavaName(const QString &newJavaName);

    const QString &getDefaultSuperClass() const;
    void setDefaultSuperClass(const QString &newDefaultSuperClass);

    bool getIsGeneric() const;
    void setIsGeneric(bool newIsGeneric);

    const QString &getTargetType() const;
    void setTargetType(const QString &newTargetType);

    bool getDisableNativeIdUsage() const;
    void setDisableNativeIdUsage(bool newDisableNativeIdUsage);

    bool getForceAbstract() const;
    void setForceAbstract(bool newForceAbstract);

    bool getDeprecated() const;
    void setDeprecated(bool newDeprecated);

    bool getForceFriendly() const;
    void setForceFriendly(bool newForceFriendly);

    const QString &getThreadAffinity() const;
    void setThreadAffinity(const QString &newThreadAffinity);

    bool getIsNativeInterface() const;
    void setIsNativeInterface(bool newIsNativeInterface);

    bool getTemplate() const;
    void setTemplate(bool newTemplate);

    const QVariant &getGenerate() const;
    void setGenerate(const QVariant &newGenerate);

    const QString &getExtendType() const;
    void setExtendType(const QString &newExtendType);

    const QString &getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

signals:
    void packageNameChanged();

    void implementingChanged();

    void usingChanged();

    void javaNameChanged();

    void defaultSuperClassChanged();

    void isGenericChanged();

    void targetTypeChanged();

    void disableNativeIdUsageChanged();

    void forceAbstractChanged();

    void deprecatedChanged();

    void forceFriendlyChanged();

    void threadAffinityChanged();

    void isNativeInterfaceChanged();

    void templateChanged();

    void generateChanged();

    void extendTypeChanged();

    void ppConditionChanged();

private:
    QString packageName;
    QString implementing;
    QString m_using;
    QString javaName;
    QString defaultSuperClass;
    bool isGeneric = false;
    QString targetType;
    bool disableNativeIdUsage = false;
    bool forceAbstract = false;
    bool forceFriendly = false;
    bool deprecated = false;
    QString threadAffinity;
    bool isNativeInterface = false;
    bool m_template = false;
    QVariant generate = true;
    QString extendType;
    QString ppCondition;
    Q_PROPERTY(QString packageName READ getPackageName WRITE setPackageName NOTIFY packageNameChanged)
    Q_PROPERTY(QString implementing READ getImplementing WRITE setImplementing NOTIFY implementingChanged)
    Q_PROPERTY(QString using READ getUsing WRITE setUsing NOTIFY usingChanged)
    Q_PROPERTY(QString javaName READ getJavaName WRITE setJavaName NOTIFY javaNameChanged)
    Q_PROPERTY(QString defaultSuperClass READ getDefaultSuperClass WRITE setDefaultSuperClass NOTIFY defaultSuperClassChanged)
    Q_PROPERTY(bool isGeneric READ getIsGeneric WRITE setIsGeneric NOTIFY isGenericChanged)
    Q_PROPERTY(QString targetType READ getTargetType WRITE setTargetType NOTIFY targetTypeChanged)
    Q_PROPERTY(bool disableNativeIdUsage READ getDisableNativeIdUsage WRITE setDisableNativeIdUsage NOTIFY disableNativeIdUsageChanged)
    Q_PROPERTY(bool forceAbstract READ getForceAbstract WRITE setForceAbstract NOTIFY forceAbstractChanged)
    Q_PROPERTY(bool deprecated READ getDeprecated WRITE setDeprecated NOTIFY deprecatedChanged)
    Q_PROPERTY(bool forceFriendly READ getForceFriendly WRITE setForceFriendly NOTIFY forceFriendlyChanged)
    Q_PROPERTY(QString threadAffinity READ getThreadAffinity WRITE setThreadAffinity NOTIFY threadAffinityChanged)
    Q_PROPERTY(bool isNativeInterface READ getIsNativeInterface WRITE setIsNativeInterface NOTIFY isNativeInterfaceChanged)
    Q_PROPERTY(bool template READ getTemplate WRITE setTemplate NOTIFY templateChanged)
    Q_PROPERTY(QVariant generate READ getGenerate WRITE setGenerate NOTIFY generateChanged)
    Q_PROPERTY(QString extendType READ getExtendType WRITE setExtendType NOTIFY extendTypeChanged)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged)
};

class ObjectType : public ComplexType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ObjectType(QObject *parent = nullptr):ComplexType{parent}{}
    bool getIsValueOwner() const;
    void setIsValueOwner(bool newIsValueOwner);

    bool getIsPolymorphicBase() const;
    void setIsPolymorphicBase(bool newIsPolymorphicBase);

    const QString &getPolymorphicIdExpression() const;
    void setPolymorphicIdExpression(const QString &newPolymorphicIdExpression);

signals:
    void isValueOwnerChanged();

    void isPolymorphicBaseChanged();

    void polymorphicIdExpressionChanged();

private:
    bool isValueOwner = false;
    bool isPolymorphicBase = false;
    QString polymorphicIdExpression;
    Q_PROPERTY(bool isValueOwner READ getIsValueOwner WRITE setIsValueOwner NOTIFY isValueOwnerChanged)
    Q_PROPERTY(bool isPolymorphicBase READ getIsPolymorphicBase WRITE setIsPolymorphicBase NOTIFY isPolymorphicBaseChanged)
    Q_PROPERTY(QString polymorphicIdExpression READ getPolymorphicIdExpression WRITE setPolymorphicIdExpression NOTIFY polymorphicIdExpressionChanged)
};

class FunctionalType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit FunctionalType(QObject *parent = nullptr):AbstractType{parent}{}

    const QVariant &getGenerate() const;
    void setGenerate(const QVariant &newGenerate);

    const QString &getPackageName() const;
    void setPackageName(const QString &newPackageName);

    const QString &getImplementing() const;
    void setImplementing(const QString &newImplementing);

    const QString &getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

    const QString &getUsing() const;
    void setUsing(const QString &newUsing);

    const QString &getJavaName() const;
    void setJavaName(const QString &newJavaName);

    int getCount() const;
    void setCount(int newCount);

    bool getNoExcept() const;
    void setNoExcept(bool newNoExcept);

    bool getBlockExceptions() const;
    void setBlockExceptions(bool newBlockExceptions);

    bool getRethrowExceptions() const;
    void setRethrowExceptions(bool newRethrowExceptions);

    const QString &getFunctionName() const;
    void setFunctionName(const QString &newFunctionName);

    bool getDisableNativeIdUsage() const;
    void setDisableNativeIdUsage(bool newDisableNativeIdUsage);

signals:
    void generateChanged();

    void packageNameChanged();

    void implementingChanged();

    void ppConditionChanged();

    void usingChanged();

    void javaNameChanged();

    void countChanged();

    void noExceptChanged();

    void blockExceptionsChanged();

    void rethrowExceptionsChanged();

    void functionNameChanged();

    void disableNativeIdUsageChanged();

private:
    QVariant generate = true;
    QString packageName;
    QString implementing;
    QString ppCondition;
    QString m_using;
    QString javaName;
    int count = 0;
    bool noExcept = false;
    bool blockExceptions = false;
    bool rethrowExceptions = false;
    QString functionName;
    bool disableNativeIdUsage = false;
    Q_PROPERTY(QVariant generate READ getGenerate WRITE setGenerate NOTIFY generateChanged)
    Q_PROPERTY(QString packageName READ getPackageName WRITE setPackageName NOTIFY packageNameChanged)
    Q_PROPERTY(QString implementing READ getImplementing WRITE setImplementing NOTIFY implementingChanged)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged)
    Q_PROPERTY(QString using READ getUsing WRITE setUsing NOTIFY usingChanged)
    Q_PROPERTY(QString javaName READ getJavaName WRITE setJavaName NOTIFY javaNameChanged)
    Q_PROPERTY(int count READ getCount WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(bool noExcept READ getNoExcept WRITE setNoExcept NOTIFY noExceptChanged)
    Q_PROPERTY(bool blockExceptions READ getBlockExceptions WRITE setBlockExceptions NOTIFY blockExceptionsChanged)
    Q_PROPERTY(bool rethrowExceptions READ getRethrowExceptions WRITE setRethrowExceptions NOTIFY rethrowExceptionsChanged)
    Q_PROPERTY(QString functionName READ getFunctionName WRITE setFunctionName NOTIFY functionNameChanged)
    Q_PROPERTY(bool disableNativeIdUsage READ getDisableNativeIdUsage WRITE setDisableNativeIdUsage NOTIFY disableNativeIdUsageChanged)
};

class IteratorType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit IteratorType(QObject *parent = nullptr):AbstractType{parent}{}
    bool getGenericClass() const;
    void setGenericClass(bool newGenericClass);

    bool getIsConst() const;
    void setIsConst(bool newIsConst);

signals:
    void genericClassChanged();

    void isConstChanged();

private:
    bool genericClass = false;
    bool isConst = true;
    Q_PROPERTY(bool genericClass READ getGenericClass WRITE setGenericClass NOTIFY genericClassChanged)
    Q_PROPERTY(bool isConst READ getIsConst WRITE setIsConst NOTIFY isConstChanged)
};

class ValueType : public ComplexType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ValueType(QObject *parent = nullptr):ComplexType{parent}{}
    bool getNoMetaType() const;
    void setNoMetaType(bool newNoMetaType);

    bool getIsPolymorphicBase() const;
    void setIsPolymorphicBase(bool newIsPolymorphicBase);

    const QString &getPolymorphicIdExpression() const;
    void setPolymorphicIdExpression(const QString &newPolymorphicIdExpression);

signals:
    void noMetaTypeChanged();

    void isPolymorphicBaseChanged();

    void polymorphicIdExpressionChanged();

private:
    bool noMetaType = false;
    bool isPolymorphicBase = false;
    QString polymorphicIdExpression;
    Q_PROPERTY(bool noMetaType READ getNoMetaType WRITE setNoMetaType NOTIFY noMetaTypeChanged)
    Q_PROPERTY(bool isPolymorphicBase READ getIsPolymorphicBase WRITE setIsPolymorphicBase NOTIFY isPolymorphicBaseChanged)
    Q_PROPERTY(QString polymorphicIdExpression READ getPolymorphicIdExpression WRITE setPolymorphicIdExpression NOTIFY polymorphicIdExpressionChanged)
};

class TypeAliasType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TypeAliasType(QObject *parent = nullptr):AbstractType{parent}{}
signals:
private:
};

class InterfaceType : public ComplexType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit InterfaceType(QObject *parent = nullptr):ComplexType{parent}{}
    bool getIsValue() const;
    void setIsValue(bool newIsValue);

    bool getNoMetaType() const;
    void setNoMetaType(bool newNoMetaType);

    bool getNoImpl() const;
    void setNoImpl(bool newNoImpl);

signals:
    void isValueChanged();

    void noMetaTypeChanged();

    void noImplChanged();

private:
    bool isValue = false;
    bool noMetaType = false;
    bool noImpl = false;
    Q_PROPERTY(bool isValue READ getIsValue WRITE setIsValue NOTIFY isValueChanged)
    Q_PROPERTY(bool noMetaType READ getNoMetaType WRITE setNoMetaType NOTIFY noMetaTypeChanged)
    Q_PROPERTY(bool noImpl READ getNoImpl WRITE setNoImpl NOTIFY noImplChanged)
};

class NamespaceType : public ComplexType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit NamespaceType(QObject *parent = nullptr):ComplexType{parent}{}
signals:
private:
};

class HeaderType : public NamespaceType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit HeaderType(QObject *parent = nullptr):NamespaceType{parent}{}
private:
};

class EnumType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit EnumType(QObject *parent = nullptr):AbstractType{parent}{}
    const QString &getPackageName() const;
    void setPackageName(const QString &newPackageName);

    const QString &getFlags() const;
    void setFlags(const QString &newFlags);

    bool getGenerate() const;
    void setGenerate(bool newGenerate);

    const QString &getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

    const QString &getUpperBound() const;
    void setUpperBound(const QString &newUpperBound);

    const QString &getLowerBound() const;
    void setLowerBound(const QString &newLowerBound);

    const QString &getJavaScope() const;
    void setJavaScope(const QString &newJavaScope);

    const QString &getImplementing() const;
    void setImplementing(const QString &newImplementing);

    bool getForceInteger() const;
    void setForceInteger(bool newForceInteger);

    bool getExtensible() const;
    void setExtensible(bool newExtensible);

    bool getHiddenMetaobject() const;
    void setHiddenMetaobject(bool newHiddenMetaobject);

signals:
    void packageNameChanged();

    void flagsChanged();

    void generateChanged();

    void ppConditionChanged();

    void upperBoundChanged();

    void lowerBoundChanged();

    void javaScopeChanged();

    void implementingChanged();

    void forceIntegerChanged();

    void extensibleChanged();

    void hiddenMetaobjectChanged();

private:
    QString packageName;
    QString flags;
    bool generate = true;
    QString ppCondition;
    QString upperBound;
    QString lowerBound;
    QString javaScope;
    QString implementing;
    bool forceInteger = false;
    bool extensible = false;
    bool hiddenMetaobject = false;
    Q_PROPERTY(QString packageName READ getPackageName WRITE setPackageName NOTIFY packageNameChanged)
    Q_PROPERTY(QString flags READ getFlags WRITE setFlags NOTIFY flagsChanged)
    Q_PROPERTY(bool generate READ getGenerate WRITE setGenerate NOTIFY generateChanged)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged)
    Q_PROPERTY(QString upperBound READ getUpperBound WRITE setUpperBound NOTIFY upperBoundChanged)
    Q_PROPERTY(QString lowerBound READ getLowerBound WRITE setLowerBound NOTIFY lowerBoundChanged)
    Q_PROPERTY(QString javaScope READ getJavaScope WRITE setJavaScope NOTIFY javaScopeChanged)
    Q_PROPERTY(QString implementing READ getImplementing WRITE setImplementing NOTIFY implementingChanged)
    Q_PROPERTY(bool forceInteger READ getForceInteger WRITE setForceInteger NOTIFY forceIntegerChanged)
    Q_PROPERTY(bool extensible READ getExtensible WRITE setExtensible NOTIFY extensibleChanged)
    Q_PROPERTY(bool hiddenMetaobject READ getHiddenMetaobject WRITE setHiddenMetaobject NOTIFY hiddenMetaobjectChanged)
};

QML_DECLARE_TYPE(AbstractType)
QML_DECLARE_TYPE(ComplexType)
QML_DECLARE_TYPE(ObjectType)
QML_DECLARE_TYPE(ValueType)
QML_DECLARE_TYPE(EnumType)
QML_DECLARE_TYPE(FunctionalType)
QML_DECLARE_TYPE(HeaderType)
QML_DECLARE_TYPE(InterfaceType)
QML_DECLARE_TYPE(NamespaceType)
QML_DECLARE_TYPE(IteratorType)
QML_DECLARE_TYPE(PrimitiveType)
QML_DECLARE_TYPE(TemplateType)
QML_DECLARE_TYPE(TypeAliasType)

#endif // PRIMITIVETYPE_H
