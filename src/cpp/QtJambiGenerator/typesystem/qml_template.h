/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "qml_types.h"

class CodeTemplate : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit CodeTemplate(QObject *parent = nullptr);
    const QString &getName() const;
    void setName(const QString &newName);

signals:
    void nameChanged();

private:
    QString name;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
};

class TemplateArguments : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TemplateArguments(QObject *parent = nullptr);
    const QStringList &getArguments() const;
    void setArguments(const QStringList &newArguments);

signals:
    void argumentsChanged();

private:
    QStringList arguments;
    Q_PROPERTY(QStringList arguments READ getArguments WRITE setArguments NOTIFY argumentsChanged)
};

class InsertTemplate : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit InsertTemplate(QObject *parent = nullptr):AbstractObject(parent){}
    const QString &getName() const;
    void setName(const QString &newName);

    uint getIndents() const;
    void setIndents(uint newIndents);

signals:
    void nameChanged();

    void indentsChanged();

private:
    QString name;
    uint indents = 0;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(uint indents READ getIndents WRITE setIndents NOTIFY indentsChanged FINAL)
};

class Replace : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Replace(QObject *parent = nullptr):AbstractObject(parent){}
    const QString &getFrom() const;
    void setFrom(const QString &newFrom);

    const QString &getTo() const;
    void setTo(const QString &newTo);

signals:
    void fromChanged();

    void toChanged();

private:
    QString from;
    QString to;
    Q_PROPERTY(QString from READ getFrom WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QString to READ getTo WRITE setTo NOTIFY toChanged)
};

class TypeTemplate : public AbstractType
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TypeTemplate(QObject *parent = nullptr):AbstractType{parent}{}
    bool getNonSealed() const;
    void setNonSealed(bool newNonSealed);

    bool getSealed() const;
    void setSealed(bool newSealed);

    bool getAddTextStreamFunctions() const;
    void setAddTextStreamFunctions(bool newAddTextStreamFunctions);

    bool getNoInstance() const;
    void setNoInstance(bool newNoInstance);

    bool getPushUpStatics() const;
    void setPushUpStatics(bool newPushUpStatics);

    bool getNotCloneable() const;
    void setNotCloneable(bool newNotCloneable);

    bool getNotMoveAssignable() const;
    void setNotMoveAssignable(bool newNotMoveAssignable);

    bool getNotAssignable() const;
    void setNotAssignable(bool newNotAssignable);

    bool getNoImplicitConstructors() const;
    void setNoImplicitConstructors(bool newNoImplicitConstructors);

    bool getNoMetaType() const;
    void setNoMetaType(bool newNoMetaType);

    QString getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

    QString getExtendType() const;
    void setExtendType(const QString &newExtendType);

    bool getIsNativeInterface() const;
    void setIsNativeInterface(bool newIsNativeInterface);

    QVariant getThreadAffinity() const;
    void setThreadAffinity(const QVariant &newThreadAffinity);

    bool getDeprecated() const;
    void setDeprecated(bool newDeprecated);

    bool getForceFriendly() const;
    void setForceFriendly(bool newForceFriendly);

    bool getForceAbstract() const;
    void setForceAbstract(bool newForceAbstract);

    bool getDisableNativeIdUsage() const;
    void setDisableNativeIdUsage(bool newDisableNativeIdUsage);

    bool getForceFinal() const;
    void setForceFinal(bool newForceFinal);

    QVariant getGenerate() const;
    void setGenerate(const QVariant &newGenerate);

    bool getIsGeneric() const;
    void setIsGeneric(bool newIsGeneric);

    QString getImplementing() const;
    void setImplementing(const QString &newImplementing);

    QString getPermitting() const;
    void setPermitting(const QString &newPermitting);

    QString getDefaultSuperClass() const;
    void setDefaultSuperClass(const QString &newDefaultSuperClass);

signals:
    void nonSealedChanged();

    void sealedChanged();

    void addTextStreamFunctionsChanged();

    void noInstanceChanged();

    void pushUpStaticsChanged();

    void notCloneableChanged();

    void notMoveAssignableChanged();

    void notAssignableChanged();

    void noImplicitConstructorsChanged();

    void noMetaTypeChanged();

    void ppConditionChanged();

    void extendTypeChanged();

    void isNativeInterfaceChanged();

    void threadAffinityChanged();

    void deprecatedChanged();

    void forceFriendlyChanged();

    void forceAbstractChanged();

    void disableNativeIdUsageChanged();

    void forceFinalChanged();

    void generateChanged();

    void isGenericChanged();

    void implementingChanged();

    void permittingChanged();

    void defaultSuperClassChanged();

private:
    Q_DISABLE_COPY(TypeTemplate)
    bool forceFinal = false;
    bool disableNativeIdUsage = false;
    bool forceAbstract = false;
    bool forceFriendly = false;
    bool deprecated = false;
    QVariant generate = true;
    QVariant threadAffinity;
    bool isNativeInterface = false;
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
    bool sealed = false;
    bool nonSealed = false;
    bool isGeneric = false;
    QString defaultSuperClass;
    QString implementing;
    QString permitting;
    Q_PROPERTY(bool nonSealed READ getNonSealed WRITE setNonSealed NOTIFY nonSealedChanged FINAL)
    Q_PROPERTY(bool sealed READ getSealed WRITE setSealed NOTIFY sealedChanged FINAL)
    Q_PROPERTY(bool addTextStreamFunctions READ getAddTextStreamFunctions WRITE setAddTextStreamFunctions NOTIFY addTextStreamFunctionsChanged FINAL)
    Q_PROPERTY(bool noInstance READ getNoInstance WRITE setNoInstance NOTIFY noInstanceChanged FINAL)
    Q_PROPERTY(bool pushUpStatics READ getPushUpStatics WRITE setPushUpStatics NOTIFY pushUpStaticsChanged FINAL)
    Q_PROPERTY(bool notCloneable READ getNotCloneable WRITE setNotCloneable NOTIFY notCloneableChanged FINAL)
    Q_PROPERTY(bool notMoveAssignable READ getNotMoveAssignable WRITE setNotMoveAssignable NOTIFY notMoveAssignableChanged FINAL)
    Q_PROPERTY(bool notAssignable READ getNotAssignable WRITE setNotAssignable NOTIFY notAssignableChanged FINAL)
    Q_PROPERTY(bool noImplicitConstructors READ getNoImplicitConstructors WRITE setNoImplicitConstructors NOTIFY noImplicitConstructorsChanged FINAL)
    Q_PROPERTY(bool noMetaType READ getNoMetaType WRITE setNoMetaType NOTIFY noMetaTypeChanged FINAL)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged FINAL)
    Q_PROPERTY(QString extendType READ getExtendType WRITE setExtendType NOTIFY extendTypeChanged FINAL)
    Q_PROPERTY(bool isNativeInterface READ getIsNativeInterface WRITE setIsNativeInterface NOTIFY isNativeInterfaceChanged FINAL)
    Q_PROPERTY(QVariant threadAffinity READ getThreadAffinity WRITE setThreadAffinity NOTIFY threadAffinityChanged FINAL)
    Q_PROPERTY(bool deprecated READ getDeprecated WRITE setDeprecated NOTIFY deprecatedChanged FINAL)
    Q_PROPERTY(bool forceFriendly READ getForceFriendly WRITE setForceFriendly NOTIFY forceFriendlyChanged FINAL)
    Q_PROPERTY(bool forceAbstract READ getForceAbstract WRITE setForceAbstract NOTIFY forceAbstractChanged FINAL)
    Q_PROPERTY(bool disableNativeIdUsage READ getDisableNativeIdUsage WRITE setDisableNativeIdUsage NOTIFY disableNativeIdUsageChanged FINAL)
    Q_PROPERTY(bool forceFinal READ getForceFinal WRITE setForceFinal NOTIFY forceFinalChanged FINAL)
    Q_PROPERTY(QVariant generate READ getGenerate WRITE setGenerate NOTIFY generateChanged FINAL)
    Q_PROPERTY(bool isGeneric READ getIsGeneric WRITE setIsGeneric NOTIFY isGenericChanged FINAL)
    Q_PROPERTY(QString implementing READ getImplementing WRITE setImplementing NOTIFY implementingChanged FINAL)
    Q_PROPERTY(QString permitting READ getPermitting WRITE setPermitting NOTIFY permittingChanged FINAL)
    Q_PROPERTY(QString defaultSuperClass READ getDefaultSuperClass WRITE setDefaultSuperClass NOTIFY defaultSuperClassChanged FINAL)
};

QML_DECLARE_TYPE(CodeTemplate)
QML_DECLARE_TYPE(InsertTemplate)
QML_DECLARE_TYPE(TemplateArguments)
QML_DECLARE_TYPE(Replace)
QML_DECLARE_TYPE(TypeTemplate)

#endif // TEMPLATE_H
