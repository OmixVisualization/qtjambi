/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
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

    bool getDisableNativeIdUsage() const;
    void setDisableNativeIdUsage(bool newDisableNativeIdUsage);

    bool getForceAbstract() const;
    void setForceAbstract(bool newForceAbstract);

    bool getDeprecated() const;
    void setDeprecated(bool newDeprecated);

    bool getForceFriendly() const;
    void setForceFriendly(bool newForceFriendly);

    const QVariant &getThreadAffinity() const;
    void setThreadAffinity(const QVariant &newThreadAffinity);

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

    bool getNoMetaType() const;
    void setNoMetaType(bool newNoMetaType);

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

    bool getNoInstance() const;
    void setNoInstance(bool newNoInstance);

    bool getForceFinal() const;
    void setForceFinal(bool newIsFinal);

    bool getAddTextStreamFunctions() const;
    void setAddTextStreamFunctions(bool newAddTextStreamFunctions);

signals:
    void packageNameChanged();

    void implementingChanged();

    void usingChanged();

    void javaNameChanged();

    void defaultSuperClassChanged();

    void isGenericChanged();

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

    void noMetaTypeChanged();

    void noImplicitConstructorsChanged();

    void notAssignableChanged();

    void notMoveAssignableChanged();

    void notCloneableChanged();

    void pushUpStaticsChanged();

    void noInstanceChanged();

    void forceFinalChanged();

    void addTextStreamFunctionsChanged();

private:
    QString packageName;
    QString implementing;
    QString m_using;
    QString javaName;
    QString defaultSuperClass;
    bool isGeneric = false;
    bool forceFinal = false;
    bool disableNativeIdUsage = false;
    bool forceAbstract = false;
    bool forceFriendly = false;
    bool deprecated = false;
    QVariant threadAffinity;
    bool isNativeInterface = false;
    bool m_template = false;
    QVariant generate = true;
    QString extendType;
    QString ppCondition;
    bool noMetaType = false;
    bool noImplicitConstructors = false;
    bool notAssignable = false;
    bool notMoveAssignable = false;
    bool notCloneable = false;
    bool pushUpStatics = false;
    bool noInstance = false;
    bool addTextStreamFunctions = false;
    Q_PROPERTY(QString packageName READ getPackageName WRITE setPackageName NOTIFY packageNameChanged)
    Q_PROPERTY(QString implementing READ getImplementing WRITE setImplementing NOTIFY implementingChanged)
    Q_PROPERTY(QString using READ getUsing WRITE setUsing NOTIFY usingChanged)
    Q_PROPERTY(QString javaName READ getJavaName WRITE setJavaName NOTIFY javaNameChanged)
    Q_PROPERTY(QString defaultSuperClass READ getDefaultSuperClass WRITE setDefaultSuperClass NOTIFY defaultSuperClassChanged)
    Q_PROPERTY(bool isGeneric READ getIsGeneric WRITE setIsGeneric NOTIFY isGenericChanged)
    Q_PROPERTY(bool disableNativeIdUsage READ getDisableNativeIdUsage WRITE setDisableNativeIdUsage NOTIFY disableNativeIdUsageChanged)
    Q_PROPERTY(bool forceAbstract READ getForceAbstract WRITE setForceAbstract NOTIFY forceAbstractChanged)
    Q_PROPERTY(bool deprecated READ getDeprecated WRITE setDeprecated NOTIFY deprecatedChanged)
    Q_PROPERTY(bool forceFriendly READ getForceFriendly WRITE setForceFriendly NOTIFY forceFriendlyChanged)
    Q_PROPERTY(QVariant threadAffinity READ getThreadAffinity WRITE setThreadAffinity NOTIFY threadAffinityChanged)
    Q_PROPERTY(bool isNativeInterface READ getIsNativeInterface WRITE setIsNativeInterface NOTIFY isNativeInterfaceChanged)
    Q_PROPERTY(bool template READ getTemplate WRITE setTemplate NOTIFY templateChanged)
    Q_PROPERTY(QVariant generate READ getGenerate WRITE setGenerate NOTIFY generateChanged)
    Q_PROPERTY(QString extendType READ getExtendType WRITE setExtendType NOTIFY extendTypeChanged)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged)
    Q_PROPERTY(bool noMetaType READ getNoMetaType WRITE setNoMetaType NOTIFY noMetaTypeChanged)
    Q_PROPERTY(bool noImplicitConstructors READ getNoImplicitConstructors WRITE setNoImplicitConstructors NOTIFY noImplicitConstructorsChanged FINAL)
    Q_PROPERTY(bool notAssignable READ getNotAssignable WRITE setNotAssignable NOTIFY notAssignableChanged FINAL)
    Q_PROPERTY(bool notMoveAssignable READ getNotMoveAssignable WRITE setNotMoveAssignable NOTIFY notMoveAssignableChanged FINAL)
    Q_PROPERTY(bool notCloneable READ getNotCloneable WRITE setNotCloneable NOTIFY notCloneableChanged FINAL)
    Q_PROPERTY(bool pushUpStatics READ getPushUpStatics WRITE setPushUpStatics NOTIFY pushUpStaticsChanged FINAL)
    Q_PROPERTY(bool noInstance READ getNoInstance WRITE setNoInstance NOTIFY noInstanceChanged FINAL)
    Q_PROPERTY(bool forceFinal READ getForceFinal WRITE setForceFinal NOTIFY forceFinalChanged FINAL)
    Q_PROPERTY(bool addTextStreamFunctions READ getAddTextStreamFunctions WRITE setAddTextStreamFunctions NOTIFY addTextStreamFunctionsChanged FINAL)
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

    bool getIsPolymorphicBase() const;
    void setIsPolymorphicBase(bool newIsPolymorphicBase);

    const QString &getPolymorphicIdExpression() const;
    void setPolymorphicIdExpression(const QString &newPolymorphicIdExpression);

signals:

    void isPolymorphicBaseChanged();

    void polymorphicIdExpressionChanged();

private:
    bool isPolymorphicBase = false;
    QString polymorphicIdExpression;
    Q_PROPERTY(bool isPolymorphicBase READ getIsPolymorphicBase WRITE setIsPolymorphicBase NOTIFY isPolymorphicBaseChanged)
    Q_PROPERTY(QString polymorphicIdExpression READ getPolymorphicIdExpression WRITE setPolymorphicIdExpression NOTIFY polymorphicIdExpressionChanged)
};

class TypeAliasType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TypeAliasType(QObject *parent = nullptr):AbstractType{parent}{}
    bool getAsNativePointer() const;
    void setAsNativePointer(bool newAsNativePointer);

    QString getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

signals:
    void asNativePointerChanged();

    void ppConditionChanged();

private:
    bool asNativePointer = false;
    QString ppCondition;
    Q_PROPERTY(bool asNativePointer READ getAsNativePointer WRITE setAsNativePointer NOTIFY asNativePointerChanged FINAL)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged FINAL)
};

class NativePointerType : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit NativePointerType(QObject *parent = nullptr):AbstractType{parent}{}
    QString getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

signals:
    void ppConditionChanged();

private:
    QString ppCondition;
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged FINAL)
};

class InterfaceType : public ComplexType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit InterfaceType(QObject *parent = nullptr):ComplexType{parent}{}
    bool getIsValue() const;
    void setIsValue(bool newIsValue);

    bool getNoImpl() const;
    void setNoImpl(bool newNoImpl);

signals:
    void isValueChanged();

    void noImplChanged();

private:
    bool isValue = false;
    bool noImpl = false;
    Q_PROPERTY(bool isValue READ getIsValue WRITE setIsValue NOTIFY isValueChanged)
    Q_PROPERTY(bool noImpl READ getNoImpl WRITE setNoImpl NOTIFY noImplChanged)
};

class NamespaceType : public ComplexType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit NamespaceType(QObject *parent = nullptr);
    bool hasMetaObject() const;
    void setHasMetaObject(bool newHasMetaObject);

signals:
    void hasMetaObjectChanged();

private:
    bool m_hasMetaObject;
    Q_PROPERTY(bool hasMetaObject READ hasMetaObject WRITE setHasMetaObject NOTIFY hasMetaObjectChanged FINAL)
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
QML_DECLARE_TYPE(NativePointerType)

#endif // PRIMITIVETYPE_H
