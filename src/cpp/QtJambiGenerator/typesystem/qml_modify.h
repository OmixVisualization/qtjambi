#ifndef MODIFYARGUMENT_H
#define MODIFYARGUMENT_H

#include "qml_abstractobject.h"

class AddImpliciteCall : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_CLASSINFO("DefaultProperty", "type")
public:
    explicit AddImpliciteCall(QObject *parent = nullptr);
    QString getType() const;
    void setType(const QString &newType);

signals:
    void typeChanged();

private:
    QString type;
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY typeChanged REQUIRED)
    Q_DISABLE_COPY(AddImpliciteCall)
};

class ModifyArgument : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ModifyArgument(QObject *parent = nullptr);

    const QVariant &getIndex() const;
    void setIndex(const QVariant &newIndex);

    const QString &getReplaceValue() const;
    void setReplaceValue(const QString &newReplaceValue);

    const QVariant &getThreadAffinity() const;
    void setThreadAffinity(const QVariant &newThreadAffinity);

    const QString &getRename() const;
    void setRename(const QString &newRename);

    const QString &getReplaceType() const;
    void setReplaceType(const QString &newReplaceType);

    bool getInvalidateAfterUse() const;
    void setInvalidateAfterUse(bool newInvalidateAfterUse);

    bool getValueAsPointer() const;
    void setValueAsPointer(bool newValueAsPointer);

signals:
    void indexChanged();

    void replaceValueChanged();

    void threadAffinityChanged();

    void renameChanged();

    void replaceTypeChanged();

    void invalidateAfterUseChanged();

    void valueAsPointerChanged();

private:
    QVariant index;
    QString replaceValue;
    QVariant threadAffinity = false;
    QString rename;
    QString replaceType;
    bool invalidateAfterUse = false;
    bool valueAsPointer = false;
    Q_PROPERTY(QVariant index READ getIndex WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(QString replaceValue READ getReplaceValue WRITE setReplaceValue NOTIFY replaceValueChanged)
    Q_PROPERTY(QVariant threadAffinity READ getThreadAffinity WRITE setThreadAffinity NOTIFY threadAffinityChanged)
    Q_PROPERTY(QString rename READ getRename WRITE setRename NOTIFY renameChanged)
    Q_PROPERTY(QString replaceType READ getReplaceType WRITE setReplaceType NOTIFY replaceTypeChanged)
    Q_PROPERTY(bool invalidateAfterUse READ getInvalidateAfterUse WRITE setInvalidateAfterUse NOTIFY invalidateAfterUseChanged)
    Q_PROPERTY(bool valueAsPointer READ getValueAsPointer WRITE setValueAsPointer NOTIFY valueAsPointerChanged)
};

class RemoveDefaultExpression : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RemoveDefaultExpression(QObject *parent = nullptr):AbstractObject{parent}{}
};


class ReplaceDefaultExpression : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ReplaceDefaultExpression(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getExpression() const;
    void setExpression(const QString &newExpression);

signals:
    void expressionChanged();

private:
    QString expression;
    Q_PROPERTY(QString expression READ getExpression WRITE setExpression NOTIFY expressionChanged)
};

class ReferenceCount : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum Action { // 0x01 - 0xff
        Invalid     = 0x00,
        Add         = 0x01,
        AddAll      = 0x02,
        Take        = 0x04,
        Set         = 0x08,
        Ignore      = 0x10,
        ClearAdd    = 0x20,
        ClearAddAll = 0x40,
        Put         = 0x80,

        ActionsMask = 0xff,

        Padding     = 0xfffffff
    };
    Q_ENUM(Action)
    explicit ReferenceCount(QObject *parent = nullptr):AbstractObject{parent}{}
    bool getIsThreadSafe() const;
    void setIsThreadSafe(bool newIsThreadSafe);

    const QString &getVariableName() const;
    void setVariableName(const QString &newVariableName);

    uint getKeyArgument() const;
    void setKeyArgument(uint newKeyArgument);

    const QString &getDeclareVariable() const;
    void setDeclareVariable(const QString &newDeclareVariable);

    Action getAction() const;
    void setAction(Action newAction);

    const QString &getCondition() const;
    void setCondition(const QString &newCondition);

    AccessModifications getAccess() const;
    void setAccess(AccessModifications newAccess);

signals:
    void isThreadSafeChanged();

    void variableNameChanged();

    void keyArgumentChanged();

    void declareVariableChanged();

    void actionChanged();

    void conditionChanged();

    void accessChanged();

private:
    bool isThreadSafe = false;
    Action action;
    QString condition;
    AccessModifications access;
    QString variableName;
    uint keyArgument = 0;
    QString declareVariable;
    Q_PROPERTY(bool isThreadSafe READ getIsThreadSafe WRITE setIsThreadSafe NOTIFY isThreadSafeChanged)
    Q_PROPERTY(QString variableName READ getVariableName WRITE setVariableName NOTIFY variableNameChanged)
    Q_PROPERTY(uint keyArgument READ getKeyArgument WRITE setKeyArgument NOTIFY keyArgumentChanged)
    Q_PROPERTY(QString declareVariable READ getDeclareVariable WRITE setDeclareVariable NOTIFY declareVariableChanged)
    Q_PROPERTY(ReferenceCount::Action action READ getAction WRITE setAction NOTIFY actionChanged)
    Q_PROPERTY(QString condition READ getCondition WRITE setCondition NOTIFY conditionChanged)
    Q_PROPERTY(AccessModifications access READ getAccess WRITE setAccess NOTIFY accessChanged)
};


class ArrayType : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ArrayType(QObject *parent = nullptr):AbstractObject{parent}{}
    bool getDeref() const;
    void setDeref(bool newDeref);

    bool getVarargs() const;
    void setVarargs(bool newVarargs);

    bool getAsBuffer() const;
    void setAsBuffer(bool newAsBuffer);

    int getLengthParameter() const;
    void setLengthParameter(int newLengthParameter);

    int getMinLength() const;
    void setMinLength(int newMinLength);

    int getMaxLength() const;
    void setMaxLength(int newMaxLength);

signals:
    void derefChanged();

    void varargsChanged();

    void asBufferChanged();

    void lengthParameterChanged();

    void minLengthChanged();

    void maxLengthChanged();

private:
    bool deref = false;
    bool varargs = false;
    bool asBuffer = false;
    int lengthParameter = 0;
    int minLength = -1;
    int maxLength = -1;
    Q_PROPERTY(bool deref READ getDeref WRITE setDeref NOTIFY derefChanged)
    Q_PROPERTY(bool varargs READ getVarargs WRITE setVarargs NOTIFY varargsChanged)
    Q_PROPERTY(bool asBuffer READ getAsBuffer WRITE setAsBuffer NOTIFY asBufferChanged)
    Q_PROPERTY(int lengthParameter READ getLengthParameter WRITE setLengthParameter NOTIFY lengthParameterChanged)
    Q_PROPERTY(int minLength READ getMinLength WRITE setMinLength NOTIFY minLengthChanged)
    Q_PROPERTY(int maxLength READ getMaxLength WRITE setMaxLength NOTIFY maxLengthChanged)
};

class ReplaceType : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ReplaceType(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getModifiedType() const;
    void setModifiedType(const QString &newModifiedType);

    const QString &getModifiedJniType() const;
    void setModifiedJniType(const QString &newModifiedJniType);

signals:
    void modifiedTypeChanged();

    void modifiedJniTypeChanged();

private:
    QString modifiedType;
    QString modifiedJniType;
    Q_PROPERTY(QString modifiedType READ getModifiedType WRITE setModifiedType NOTIFY modifiedTypeChanged)
    Q_PROPERTY(QString modifiedJniType READ getModifiedJniType WRITE setModifiedJniType NOTIFY modifiedJniTypeChanged)
};

class NoNullPointer : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit NoNullPointer(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getDefaultValue() const;
    void setDefaultValue(const QString &newDefaultValue);

signals:
    void defaultValueChanged();

private:
    QString defaultValue;
    Q_PROPERTY(QString defaultValue READ getDefaultValue WRITE setDefaultValue NOTIFY defaultValueChanged)
};

class DefineOwnership : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DefineOwnership(QObject *parent = nullptr):AbstractObject{parent}{}
    CodeClass::Entries getCodeClass() const;
    void setCodeClass(CodeClass::Entries newCodeClass);

    Ownership::Entries getOwnership() const;
    void setOwnership(Ownership::Entries newOwner);

    const QString &getCondition() const;
    void setCondition(const QString &newCondition);

signals:
    void codeClassChanged();

    void ownershipChanged();

    void conditionChanged();

private:
    CodeClass::Entries codeClass = CodeClass::NoLanguage;
    Ownership::Entries owner = Ownership::Invalid;
    QString condition;
    Q_PROPERTY(CodeClass::Entries codeClass READ getCodeClass WRITE setCodeClass NOTIFY codeClassChanged)
    Q_PROPERTY(Ownership::Entries ownership READ getOwnership WRITE setOwnership NOTIFY ownershipChanged)
    Q_PROPERTY(QString condition READ getCondition WRITE setCondition NOTIFY conditionChanged)
};

class Delegate : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Delegate(QObject *parent = nullptr):AbstractObject{parent}{}
    QString name() const;
    void setName(const QString &newName);

    bool isDeprecated() const;
    void setDeprecated(bool newIsDeprecated);

    AccessModifications access() const;
    void setAccess(const AccessModifications &newAccess);

    bool isSelfReturning() const;
    void setIsSelfReturning(bool newIsSelfReturning);

signals:
    void nameChanged();

    void isDeprecatedChanged();

    void accessChanged();

    void isSelfReturningChanged();

private:
    QString m_name;
    AccessModifications m_access;
    bool m_isDeprecated = false;
    bool m_isSelfReturning = false;
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool deprecated READ isDeprecated WRITE setDeprecated NOTIFY isDeprecatedChanged)
    Q_PROPERTY(AccessModifications access READ access WRITE setAccess NOTIFY accessChanged)
    Q_PROPERTY(bool selfReturning READ isSelfReturning WRITE setIsSelfReturning NOTIFY isSelfReturningChanged)
};

class RemoveArgument : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RemoveArgument(QObject *parent = nullptr):AbstractObject{parent}{}
private:
};

class ConversionRule : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ConversionRule(QObject *parent = nullptr):AbstractObject{parent}{}
    CodeClass::Entries getCodeClass() const;
    void setCodeClass(CodeClass::Entries newCodeClass);

signals:
    void codeClassChanged();

private:
    CodeClass::Entries codeClass = CodeClass::NoLanguage;
    Q_PROPERTY(CodeClass::Entries codeClass READ getCodeClass WRITE setCodeClass NOTIFY codeClassChanged)
};

class Instantiation : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Instantiation(QObject *parent = nullptr):AbstractObject{parent}{}
    AccessModifications getAccess() const;
    void setAccess(AccessModifications newAccess);

    const QString &getRename() const;
    void setRename(const QString &newRename);

    bool getNoExcept() const;
    void setNoExcept(bool newNoExcept);

    bool getBlockExceptions() const;
    void setBlockExceptions(bool newBlockExceptions);

    bool getRethrowExceptions() const;
    void setRethrowExceptions(bool newRethrowExceptions);

    bool getDeprecated() const;
    void setDeprecated(bool newDeprecated);

    bool getVirtualSlot() const;
    void setVirtualSlot(bool newVirtualSlot);

    const QString &getAssociatedTo() const;
    void setAssociatedTo(const QString &newAssociatedTo);

    bool getAllowAsSlot() const;
    void setAllowAsSlot(bool newAllowAsSlot);

    bool getPrivateSignal() const;
    void setPrivateSignal(bool newPrivateSignal);

    const QVariant &getThreadAffinity() const;
    void setThreadAffinity(const QVariant &newThreadAffinity);

    const QString &getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

    const QString &getThrowing() const;
    void setThrowing(const QString &newThrowing);

    const QString &getProxyCall() const;
    void setProxyCall(const QString &newProxyCall);

    const QString &getTargetType() const;
    void setTargetType(const QString &newTargetType);

signals:
    void accessChanged();

    void renameChanged();

    void noExceptChanged();

    void blockExceptionsChanged();

    void rethrowExceptionsChanged();

    void deprecatedChanged();

    void virtualSlotChanged();

    void associatedToChanged();

    void allowAsSlotChanged();

    void privateSignalChanged();

    void threadAffinityChanged();

    void ppConditionChanged();

    void throwingChanged();

    void proxyCallChanged();

    void targetTypeChanged();

private:
    AccessModifications access;
    QString rename;
    bool noExcept = false;
    bool blockExceptions = false;
    bool rethrowExceptions = false;
    bool deprecated = false;
    bool virtualSlot = false;
    QString associatedTo;
    bool allowAsSlot = false;
    bool privateSignal = false;
    QVariant threadAffinity = false;
    QString ppCondition;
    QString throwing;
    QString proxyCall;
    QString targetType;
    Q_PROPERTY(AccessModifications access READ getAccess WRITE setAccess NOTIFY accessChanged)
    Q_PROPERTY(QString rename READ getRename WRITE setRename NOTIFY renameChanged)
    Q_PROPERTY(bool noExcept READ getNoExcept WRITE setNoExcept NOTIFY noExceptChanged)
    Q_PROPERTY(bool blockExceptions READ getBlockExceptions WRITE setBlockExceptions NOTIFY blockExceptionsChanged)
    Q_PROPERTY(bool rethrowExceptions READ getRethrowExceptions WRITE setRethrowExceptions NOTIFY rethrowExceptionsChanged)
    Q_PROPERTY(bool deprecated READ getDeprecated WRITE setDeprecated NOTIFY deprecatedChanged)
    Q_PROPERTY(bool virtualSlot READ getVirtualSlot WRITE setVirtualSlot NOTIFY virtualSlotChanged)
    Q_PROPERTY(QString associatedTo READ getAssociatedTo WRITE setAssociatedTo NOTIFY associatedToChanged)
    Q_PROPERTY(bool allowAsSlot READ getAllowAsSlot WRITE setAllowAsSlot NOTIFY allowAsSlotChanged)
    Q_PROPERTY(bool privateSignal READ getPrivateSignal WRITE setPrivateSignal NOTIFY privateSignalChanged)
    Q_PROPERTY(QVariant threadAffinity READ getThreadAffinity WRITE setThreadAffinity NOTIFY threadAffinityChanged)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged)
    Q_PROPERTY(QString throwing READ getThrowing WRITE setThrowing NOTIFY throwingChanged)
    Q_PROPERTY(QString proxyCall READ getProxyCall WRITE setProxyCall NOTIFY proxyCallChanged)
    Q_PROPERTY(QString targetType READ getTargetType WRITE setTargetType NOTIFY targetTypeChanged)
};

class Argument : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Argument(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getType() const;
    void setType(const QString &newType);

    const QString &getParameter() const;
    void setParameter(const QString &newParameter);

    const QString &getExtending() const;
    void setExtending(const QString &newExtending);

    bool getIsImplicit() const;
    void setIsImplicit(bool newIsImplicit);

signals:
    void typeChanged();

    void parameterChanged();

    void extendingChanged();

    void isImplicitChanged();

private:
    QString type;
    QString parameter;
    QString extending;
    bool isImplicit = false;
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString parameter READ getParameter WRITE setParameter NOTIFY parameterChanged)
    Q_PROPERTY(QString extending READ getExtending WRITE setExtending NOTIFY extendingChanged)
    Q_PROPERTY(bool isImplicit READ getIsImplicit WRITE setIsImplicit NOTIFY isImplicitChanged)
};

class Rename : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Rename(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getTo() const;
    void setTo(const QString &newTo);

signals:
    void toChanged();

private:
    QString to;
    Q_PROPERTY(QString to READ getTo WRITE setTo NOTIFY toChanged)
};

class Access : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Access(QObject *parent = nullptr):AbstractObject{parent}{}
    AccessModifications getModifier() const;
    void setModifier(AccessModifications newModifier);

signals:
    void modifierChanged();

private:
    AccessModifications modifier;
    Q_PROPERTY(AccessModifications modifiers READ getModifier WRITE setModifier NOTIFY modifierChanged)
};

class AddArgument : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AddArgument(QObject *parent = nullptr):AbstractObject{parent}{}
    int getIndex() const;
    void setIndex(int newIndex);

    const QString &getDefaultExpression() const;
    void setDefaultExpression(const QString &newDefaultExpression);

    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getType() const;
    void setType(const QString &newType);

    const QString &getJniType() const;
    void setJniType(const QString &newJniType);

    bool getInvalidateAfterUse() const;
    void setInvalidateAfterUse(bool newInvalidateAfterUse);

signals:
    void indexChanged();

    void defaultExpressionChanged();

    void nameChanged();

    void typeChanged();

    void jniTypeChanged();

    void invalidateAfterUseChanged();

private:
    int index = std::numeric_limits<int>::max();
    QString defaultExpression;
    QString name;
    QString type;
    QString jniType;
    bool invalidateAfterUse = false;
    Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(QString defaultExpression READ getDefaultExpression WRITE setDefaultExpression NOTIFY defaultExpressionChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString jniType READ getJniType WRITE setJniType NOTIFY jniTypeChanged)
    Q_PROPERTY(bool invalidateAfterUse READ getInvalidateAfterUse WRITE setInvalidateAfterUse NOTIFY invalidateAfterUseChanged)
};

class AddTypeParameter : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AddTypeParameter(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getExtending() const;
    void setExtending(const QString &newExtending);

signals:
    void nameChanged();

    void extendingChanged();

private:
    QString name;
    QString extending;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString extending READ getExtending WRITE setExtending NOTIFY extendingChanged)
};

class Remove : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Remove(QObject *parent = nullptr):AbstractObject{parent}{}
    RemoveFlag::Entries getCodeClass() const;
    void setCodeClass(RemoveFlag::Entries newCodeClass);

signals:
    void codeClassChanged();

private:
    RemoveFlag::Entries codeClass = RemoveFlag::All;
    Q_PROPERTY(RemoveFlag::Entries codeClass READ getCodeClass WRITE setCodeClass NOTIFY codeClassChanged)
};

class ModifyFunction : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ModifyFunction(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getSignature() const;
    void setSignature(const QString &newSignature);

    AccessModifications getAccess() const;
    void setAccess(AccessModifications newAccess);

    bool getNoExcept() const;
    void setNoExcept(bool newNoExcept);

    bool getBlockExceptions() const;
    void setBlockExceptions(bool newBlockExceptions);

    bool getRethrowExceptions() const;
    void setRethrowExceptions(bool newRethrowExceptions);

    const QString &getRename() const;
    void setRename(const QString &newRename);

    RemoveFlag::Entries getRemove() const;
    void setRemove(RemoveFlag::Entries newRemove);

    bool getDeprecated() const;
    void setDeprecated(bool newDeprecated);

    const QString &getAssociatedTo() const;
    void setAssociatedTo(const QString &newAssociatedTo);

    bool getVirtualSlot() const;
    void setVirtualSlot(bool newVirtualSlot);

    bool getAllowAsSlot() const;
    void setAllowAsSlot(bool newAllowAsSlot);

    bool getPrivateSignal() const;
    void setPrivateSignal(bool newPrivateSignal);

    const QVariant &getThreadAffinity() const;
    void setThreadAffinity(const QVariant &newThreadAffinity);

    const QString &getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

    const QString &getThrowing() const;
    void setThrowing(const QString &newThrowing);

    const QString &getProxyCall() const;
    void setProxyCall(const QString &newProxyCall);

    const QString &getTargetType() const;
    void setTargetType(const QString &newTargetType);

    bool getIsPaintMethod() const;
    void setIsPaintMethod(bool newIsPaintMethod);

    bool noKotlinGetter() const;
    void setNoKotlinGetter(bool newNoKotlinGetter);

signals:
    void signatureChanged();

    void accessChanged();

    void noExceptChanged();

    void blockExceptionsChanged();

    void rethrowExceptionsChanged();

    void renameChanged();

    void removeChanged();

    void deprecatedChanged();

    void associatedToChanged();

    void virtualSlotChanged();

    void allowAsSlotChanged();

    void privateSignalChanged();

    void threadAffinityChanged();

    void ppConditionChanged();

    void throwingChanged();

    void proxyCallChanged();

    void targetTypeChanged();

    void isPaintMethodChanged();

    void noKotlinGetterChanged();

private:
    QString signature;
    AccessModifications access;
    bool noExcept = false;
    bool blockExceptions = false;
    bool rethrowExceptions = false;
    QString rename;
    RemoveFlag::Entries remove = RemoveFlag::None;
    bool deprecated = false;
    QString associatedTo;
    bool virtualSlot = false;
    bool allowAsSlot = false;
    bool privateSignal = false;
    QVariant threadAffinity = false;
    QString ppCondition;
    QString throwing;
    QString proxyCall;
    QString targetType;
    bool isPaintMethod = false;
    bool m_noKotlinGetter = false;
    Q_PROPERTY(QString signature READ getSignature WRITE setSignature NOTIFY signatureChanged)
    Q_PROPERTY(AccessModifications access READ getAccess WRITE setAccess NOTIFY accessChanged)
    Q_PROPERTY(bool noExcept READ getNoExcept WRITE setNoExcept NOTIFY noExceptChanged)
    Q_PROPERTY(bool blockExceptions READ getBlockExceptions WRITE setBlockExceptions NOTIFY blockExceptionsChanged)
    Q_PROPERTY(bool rethrowExceptions READ getRethrowExceptions WRITE setRethrowExceptions NOTIFY rethrowExceptionsChanged)
    Q_PROPERTY(QString rename READ getRename WRITE setRename NOTIFY renameChanged)
    Q_PROPERTY(RemoveFlag::Entries remove READ getRemove WRITE setRemove NOTIFY removeChanged)
    Q_PROPERTY(bool deprecated READ getDeprecated WRITE setDeprecated NOTIFY deprecatedChanged)
    Q_PROPERTY(QString associatedTo READ getAssociatedTo WRITE setAssociatedTo NOTIFY associatedToChanged)
    Q_PROPERTY(bool virtualSlot READ getVirtualSlot WRITE setVirtualSlot NOTIFY virtualSlotChanged)
    Q_PROPERTY(bool allowAsSlot READ getAllowAsSlot WRITE setAllowAsSlot NOTIFY allowAsSlotChanged)
    Q_PROPERTY(bool privateSignal READ getPrivateSignal WRITE setPrivateSignal NOTIFY privateSignalChanged)
    Q_PROPERTY(QVariant threadAffinity READ getThreadAffinity WRITE setThreadAffinity NOTIFY threadAffinityChanged)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged)
    Q_PROPERTY(QString throwing READ getThrowing WRITE setThrowing NOTIFY throwingChanged)
    Q_PROPERTY(QString proxyCall READ getProxyCall WRITE setProxyCall NOTIFY proxyCallChanged)
    Q_PROPERTY(QString targetType READ getTargetType WRITE setTargetType NOTIFY targetTypeChanged)
    Q_PROPERTY(bool isPaintMethod READ getIsPaintMethod WRITE setIsPaintMethod NOTIFY isPaintMethodChanged)
    Q_PROPERTY(bool noKotlinGetter READ noKotlinGetter WRITE setNoKotlinGetter NOTIFY noKotlinGetterChanged)
};

class GlobalFunction : public ModifyFunction
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit GlobalFunction(QObject *parent = nullptr):ModifyFunction{parent}{}
private:
};

class ModifyField : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ModifyField(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getName() const;
    void setName(const QString &newName);

    bool getRead() const;
    void setRead(bool newRead);

    bool getWrite() const;
    void setWrite(bool newWrite);

    AccessModifications getAccess() const;
    void setAccess(AccessModifications newAccess);

    const QString &getRename() const;
    void setRename(const QString &newRename);

    bool noKotlinGetter() const;
    void setNoKotlinGetter(bool newNoKotlinGetter);

signals:
    void nameChanged();

    void readChanged();

    void writeChanged();

    void accessChanged();

    void renameChanged();

    void noKotlinGetterChanged();

private:
    QString name;
    bool read = true;
    bool write = true;
    AccessModifications access;
    QString rename;
    bool m_noKotlinGetter = false;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool read READ getRead WRITE setRead NOTIFY readChanged)
    Q_PROPERTY(bool write READ getWrite WRITE setWrite NOTIFY writeChanged)
    Q_PROPERTY(AccessModifications access READ getAccess WRITE setAccess NOTIFY accessChanged)
    Q_PROPERTY(QString rename READ getRename WRITE setRename NOTIFY renameChanged)
    Q_PROPERTY(bool noKotlinGetter READ noKotlinGetter WRITE setNoKotlinGetter NOTIFY noKotlinGetterChanged)
};

class RejectEnumValue : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RejectEnumValue(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getName() const;
    void setName(const QString &newName);

    bool getRemove() const;
    void setRemove(bool newRemove);

signals:
    void nameChanged();

    void removeChanged();

private:
    QString name;
    bool remove = false;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool remove READ getRemove WRITE setRemove NOTIFY removeChanged)
};

class RenameEnumValue : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit RenameEnumValue(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getRename() const;
    void setRename(const QString &newRename);

signals:
    void nameChanged();

    void renameChanged();

private:
    QString name;
    QString rename;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString rename READ getRename WRITE setRename NOTIFY renameChanged)
};

class DelegateBaseClass : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit DelegateBaseClass(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getBaseClass() const;
    void setBaseClass(const QString &newBaseClass);

    const QString &getDelegate() const;
    void setDelegate(const QString &newDelegate);

signals:
    void baseClassChanged();

    void delegateChanged();

private:
    QString baseClass;
    QString delegate;
    Q_PROPERTY(QString baseClass READ getBaseClass WRITE setBaseClass NOTIFY baseClassChanged)
    Q_PROPERTY(QString delegate READ getDelegate WRITE setDelegate NOTIFY delegateChanged)
};

class InterfacePolymorphy : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit InterfacePolymorphy(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getInterfaceType() const;
    void setInterfaceType(const QString &newInterfaceType);

    const QString &getCondition() const;
    void setCondition(const QString &newCondition);

signals:
    void interfaceTypeChanged();

    void conditionChanged();

private:
    QString interfaceType;
    QString condition;
    Q_PROPERTY(QString interfaceType READ getInterfaceType WRITE setInterfaceType NOTIFY interfaceTypeChanged)
    Q_PROPERTY(QString condition READ getCondition WRITE setCondition NOTIFY conditionChanged)
};

class AbstractStructor : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit AbstractStructor(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getParamName() const;
    void setParamName(const QString &newParamName);

signals:
    void nameChanged();

    void paramNameChanged();

private:
    QString name;
    QString paramName;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString paramName READ getParamName WRITE setParamName NOTIFY paramNameChanged)
};

class CustomConstructor : public AbstractStructor
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum ConstructorType{
        Legacy,
        Default,
        Copy,
        Move
    };
    Q_ENUM(ConstructorType);
    explicit CustomConstructor(QObject *parent = nullptr):AbstractStructor{parent}{}
    const QString &getPlacementName() const;
    void setPlacementName(const QString &newPlacementName);

    ConstructorType getType() const;
    void setType(ConstructorType newType);

signals:
    void placementNameChanged();

    void typeChanged();

private:
    QString placementName;
    ConstructorType type = Legacy;
    Q_PROPERTY(QString placementName READ getPlacementName WRITE setPlacementName NOTIFY placementNameChanged)
    Q_PROPERTY(ConstructorType type READ getType WRITE setType NOTIFY typeChanged)
};

class CustomDestructor : public AbstractStructor
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit CustomDestructor(QObject *parent = nullptr):AbstractStructor{parent}{}
};

class Import : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Import(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getTemplate() const;
    void setTemplate(const QString &newTemplate);

signals:
    void templateChanged();

private:
    QString m_template;
    Q_PROPERTY(QString template READ getTemplate WRITE setTemplate NOTIFY templateChanged)
};

class Implements : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Implements(QObject *parent = nullptr):AbstractObject{parent}{}
    const QString &getInterface() const;
    void setInterface(const QString &newInterface);

signals:
    void interfaceChanged();

private:
    QString interface;
    Q_PROPERTY(QString interface READ getInterface WRITE setInterface NOTIFY interfaceChanged)
};

QML_DECLARE_TYPE(Access)
QML_DECLARE_TYPE(AbstractStructor)
QML_DECLARE_TYPE(AddArgument)
QML_DECLARE_TYPE(AddTypeParameter)
QML_DECLARE_TYPE(Argument)
QML_DECLARE_TYPE(ArrayType)
QML_DECLARE_TYPE(ConversionRule)
QML_DECLARE_TYPE(CustomConstructor)
QML_DECLARE_TYPE(CustomDestructor)
QML_DECLARE_TYPE(DefineOwnership)
QML_DECLARE_TYPE(DelegateBaseClass)
QML_DECLARE_TYPE(Delegate)
QML_DECLARE_TYPE(GlobalFunction)
QML_DECLARE_TYPE(Import)
QML_DECLARE_TYPE(Instantiation)
QML_DECLARE_TYPE(InterfacePolymorphy)
QML_DECLARE_TYPE(ModifyArgument)
QML_DECLARE_TYPE(ModifyField)
QML_DECLARE_TYPE(ModifyFunction)
QML_DECLARE_TYPE(NoNullPointer)
QML_DECLARE_TYPE(ReferenceCount)
QML_DECLARE_TYPE(RejectEnumValue)
QML_DECLARE_TYPE(Remove)
QML_DECLARE_TYPE(RemoveArgument)
QML_DECLARE_TYPE(RemoveDefaultExpression)
QML_DECLARE_TYPE(Rename)
QML_DECLARE_TYPE(ReplaceDefaultExpression)
QML_DECLARE_TYPE(RenameEnumValue)
QML_DECLARE_TYPE(ReplaceType)

Q_DECLARE_METATYPE(ReferenceCount::Action)

#endif // MODIFYARGUMENT_H
