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

#include "qml_template.h"

CodeTemplate::CodeTemplate(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &CodeTemplate::getName() const
{
    return name;
}

void CodeTemplate::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

TemplateArguments::TemplateArguments(QObject *parent)
    : AbstractObject{parent}
{

}

const QStringList &TemplateArguments::getArguments() const
{
    return arguments;
}

void TemplateArguments::setArguments(const QStringList &newArguments)
{
    if (arguments == newArguments)
        return;
    arguments = newArguments;
    emit argumentsChanged();
}

const QString &InsertTemplate::getName() const
{
    return name;
}

void InsertTemplate::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

const QString &Replace::getFrom() const
{
    return from;
}

void Replace::setFrom(const QString &newFrom)
{
    if (from == newFrom)
        return;
    from = newFrom;
    emit fromChanged();
}

const QString &Replace::getTo() const
{
    return to;
}

void Replace::setTo(const QString &newTo)
{
    if (to == newTo)
        return;
    to = newTo;
    emit toChanged();
}

bool TypeTemplate::getNonSealed() const
{
    return nonSealed;
}

void TypeTemplate::setNonSealed(bool newNonSealed)
{
    if (nonSealed == newNonSealed)
        return;
    nonSealed = newNonSealed;
    emit nonSealedChanged();
}

bool TypeTemplate::getSealed() const
{
    return sealed;
}

void TypeTemplate::setSealed(bool newSealed)
{
    if (sealed == newSealed)
        return;
    sealed = newSealed;
    emit sealedChanged();
}

bool TypeTemplate::getAddTextStreamFunctions() const
{
    return addTextStreamFunctions;
}

void TypeTemplate::setAddTextStreamFunctions(bool newAddTextStreamFunctions)
{
    if (addTextStreamFunctions == newAddTextStreamFunctions)
        return;
    addTextStreamFunctions = newAddTextStreamFunctions;
    emit addTextStreamFunctionsChanged();
}

bool TypeTemplate::getNoInstance() const
{
    return noInstance;
}

void TypeTemplate::setNoInstance(bool newNoInstance)
{
    if (noInstance == newNoInstance)
        return;
    noInstance = newNoInstance;
    emit noInstanceChanged();
}

bool TypeTemplate::getPushUpStatics() const
{
    return pushUpStatics;
}

void TypeTemplate::setPushUpStatics(bool newPushUpStatics)
{
    if (pushUpStatics == newPushUpStatics)
        return;
    pushUpStatics = newPushUpStatics;
    emit pushUpStaticsChanged();
}

bool TypeTemplate::getNotCloneable() const
{
    return notCloneable;
}

void TypeTemplate::setNotCloneable(bool newNotCloneable)
{
    if (notCloneable == newNotCloneable)
        return;
    notCloneable = newNotCloneable;
    emit notCloneableChanged();
}

bool TypeTemplate::getNotMoveAssignable() const
{
    return notMoveAssignable;
}

void TypeTemplate::setNotMoveAssignable(bool newNotMoveAssignable)
{
    if (notMoveAssignable == newNotMoveAssignable)
        return;
    notMoveAssignable = newNotMoveAssignable;
    emit notMoveAssignableChanged();
}

bool TypeTemplate::getNotAssignable() const
{
    return notAssignable;
}

void TypeTemplate::setNotAssignable(bool newNotAssignable)
{
    if (notAssignable == newNotAssignable)
        return;
    notAssignable = newNotAssignable;
    emit notAssignableChanged();
}

bool TypeTemplate::getNoImplicitConstructors() const
{
    return noImplicitConstructors;
}

void TypeTemplate::setNoImplicitConstructors(bool newNoImplicitConstructors)
{
    if (noImplicitConstructors == newNoImplicitConstructors)
        return;
    noImplicitConstructors = newNoImplicitConstructors;
    emit noImplicitConstructorsChanged();
}

bool TypeTemplate::getNoMetaType() const
{
    return noMetaType;
}

void TypeTemplate::setNoMetaType(bool newNoMetaType)
{
    if (noMetaType == newNoMetaType)
        return;
    noMetaType = newNoMetaType;
    emit noMetaTypeChanged();
}

QString TypeTemplate::getPpCondition() const
{
    return ppCondition;
}

void TypeTemplate::setPpCondition(const QString &newPpCondition)
{
    if (ppCondition == newPpCondition)
        return;
    ppCondition = newPpCondition;
    emit ppConditionChanged();
}

QString TypeTemplate::getExtendType() const
{
    return extendType;
}

void TypeTemplate::setExtendType(const QString &newExtendType)
{
    if (extendType == newExtendType)
        return;
    extendType = newExtendType;
    emit extendTypeChanged();
}

bool TypeTemplate::getIsNativeInterface() const
{
    return isNativeInterface;
}

void TypeTemplate::setIsNativeInterface(bool newIsNativeInterface)
{
    if (isNativeInterface == newIsNativeInterface)
        return;
    isNativeInterface = newIsNativeInterface;
    emit isNativeInterfaceChanged();
}

QVariant TypeTemplate::getThreadAffinity() const
{
    return threadAffinity;
}

void TypeTemplate::setThreadAffinity(const QVariant &newThreadAffinity)
{
    if (threadAffinity == newThreadAffinity)
        return;
    threadAffinity = newThreadAffinity;
    emit threadAffinityChanged();
}

bool TypeTemplate::getDeprecated() const
{
    return deprecated;
}

void TypeTemplate::setDeprecated(bool newDeprecated)
{
    if (deprecated == newDeprecated)
        return;
    deprecated = newDeprecated;
    emit deprecatedChanged();
}

bool TypeTemplate::getForceFriendly() const
{
    return forceFriendly;
}

void TypeTemplate::setForceFriendly(bool newForceFriendly)
{
    if (forceFriendly == newForceFriendly)
        return;
    forceFriendly = newForceFriendly;
    emit forceFriendlyChanged();
}

bool TypeTemplate::getForceAbstract() const
{
    return forceAbstract;
}

void TypeTemplate::setForceAbstract(bool newForceAbstract)
{
    if (forceAbstract == newForceAbstract)
        return;
    forceAbstract = newForceAbstract;
    emit forceAbstractChanged();
}

bool TypeTemplate::getDisableNativeIdUsage() const
{
    return disableNativeIdUsage;
}

void TypeTemplate::setDisableNativeIdUsage(bool newDisableNativeIdUsage)
{
    if (disableNativeIdUsage == newDisableNativeIdUsage)
        return;
    disableNativeIdUsage = newDisableNativeIdUsage;
    emit disableNativeIdUsageChanged();
}

bool TypeTemplate::getForceFinal() const
{
    return forceFinal;
}

void TypeTemplate::setForceFinal(bool newForceFinal)
{
    if (forceFinal == newForceFinal)
        return;
    forceFinal = newForceFinal;
    emit forceFinalChanged();
}

QVariant TypeTemplate::getGenerate() const
{
    return generate;
}

void TypeTemplate::setGenerate(const QVariant &newGenerate)
{
    if (generate == newGenerate)
        return;
    generate = newGenerate;
    emit generateChanged();
}

bool TypeTemplate::getIsGeneric() const
{
    return isGeneric;
}

void TypeTemplate::setIsGeneric(bool newIsGeneric)
{
    if (isGeneric == newIsGeneric)
        return;
    isGeneric = newIsGeneric;
    emit isGenericChanged();
}

QString TypeTemplate::getImplementing() const
{
    return implementing;
}

void TypeTemplate::setImplementing(const QString &newImplementing)
{
    if (implementing == newImplementing)
        return;
    implementing = newImplementing;
    emit implementingChanged();
}

QString TypeTemplate::getPermitting() const
{
    return permitting;
}

void TypeTemplate::setPermitting(const QString &newPermitting)
{
    if (permitting == newPermitting)
        return;
    permitting = newPermitting;
    emit permittingChanged();
}

QString TypeTemplate::getDefaultSuperClass() const
{
    return defaultSuperClass;
}

void TypeTemplate::setDefaultSuperClass(const QString &newDefaultSuperClass)
{
    if (defaultSuperClass == newDefaultSuperClass)
        return;
    defaultSuperClass = newDefaultSuperClass;
    emit defaultSuperClassChanged();
}
