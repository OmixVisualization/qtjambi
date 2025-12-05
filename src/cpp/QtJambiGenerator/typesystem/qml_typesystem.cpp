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

#include "qml_typesystem.h"

TypeSystem::TypeSystem(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &TypeSystem::getPackageName() const
{
    return packageName;
}

void TypeSystem::setPackageName(const QString &newPackageName)
{
    if (packageName == newPackageName)
        return;
    packageName = newPackageName;
    emit packageNameChanged();
}

const QString &TypeSystem::getDefaultSuperClass() const
{
    return defaultSuperClass;
}

void TypeSystem::setDefaultSuperClass(const QString &newDefaultSuperClass)
{
    if (defaultSuperClass == newDefaultSuperClass)
        return;
    defaultSuperClass = newDefaultSuperClass;
    emit defaultSuperClassChanged();
}

const QString &TypeSystem::getQtLibrary() const
{
    return qtLibrary;
}

void TypeSystem::setQtLibrary(const QString &newQtLibrary)
{
    if (qtLibrary == newQtLibrary)
        return;
    qtLibrary = newQtLibrary;
    emit qtLibraryChanged();
}

bool TypeSystem::getNoExports() const
{
    return noExports;
}

void TypeSystem::setNoExports(bool newNoExports)
{
    if (noExports == newNoExports)
        return;
    noExports = newNoExports;
    emit noExportsChanged();
}

const QString &TypeSystem::getModule() const
{
    return module;
}

void TypeSystem::setModule(const QString &newModule)
{
    if (module == newModule)
        return;
    module = newModule;
    emit moduleChanged();
}

const QString &TypeSystem::getDescription() const
{
    return description;
}

void TypeSystem::setDescription(const QString &newDescription)
{
    if (description == newDescription)
        return;
    description = newDescription;
    emit descriptionChanged();
}

const QString &TypeSystem::getTargetName() const
{
    return targetName;
}

void TypeSystem::setTargetName(const QString &newTargetName)
{
    if (targetName == newTargetName)
        return;
    targetName = newTargetName;
    emit targetNameChanged();
}

QString TypeSystem::getDefaultPPCondition() const
{
    return defaultPPCondition;
}

void TypeSystem::setDefaultPPCondition(const QString &newDefaultPPCondition)
{
    if (defaultPPCondition == newDefaultPPCondition)
        return;
    defaultPPCondition = newDefaultPPCondition;
    emit defaultPPConditionChanged();
}

QString TypeSystem::getPrecompiledHeader() const
{
    return precompiledHeader;
}

void TypeSystem::setPrecompiledHeader(const QString &newPrecompiledHeader)
{
    if (precompiledHeader == newPrecompiledHeader)
        return;
    precompiledHeader = newPrecompiledHeader;
    emit precompiledHeaderChanged();
}

Module::Module(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &Module::getName() const
{
    return name;
}

void Module::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

Description::Description(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &Description::getText() const
{
    return text;
}

void Description::setText(const QString &newText)
{
    if (text == newText)
        return;
    text = newText;
    emit textChanged();
}

const QString &RequiredLibrary::getName() const
{
    return name;
}

void RequiredLibrary::setName(const QString &newText)
{
    if (name == newText)
        return;
    name = newText;
    emit nameChanged();
}

RequiredLibrary::Mode RequiredLibrary::getMode() const
{
    return mode;
}

void RequiredLibrary::setMode(Mode newMode)
{
    if (mode == newMode)
        return;
    mode = newMode;
    emit modeChanged();
}

const QStringList &RequiredLibrary::getPlatforms() const
{
    return platforms;
}

void RequiredLibrary::setPlatforms(const QStringList &newPlatforms)
{
    if (platforms == newPlatforms)
        return;
    platforms = newPlatforms;
    emit platformsChanged();
}

const QString &RequiredPackage::getName() const
{
    return name;
}

void RequiredPackage::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

const QString &ForwardDeclaration::getClassName() const
{
    return className;
}

void ForwardDeclaration::setClassName(const QString &newCodeClass)
{
    if (className == newCodeClass)
        return;
    className = newCodeClass;
    emit codeClassChanged();
}

const QString &NamespacePrefix::getPrefix() const
{
    return prefix;
}

void NamespacePrefix::setPrefix(const QString &newPrefix)
{
    if (prefix == newPrefix)
        return;
    prefix = newPrefix;
    emit prefixChanged();
}

const QString &NamespacePrefix::getNamespace() const
{
    return m_namespace;
}

void NamespacePrefix::setNamespace(const QString &newNamespace)
{
    if (m_namespace == newNamespace)
        return;
    m_namespace = newNamespace;
    emit namespaceChanged();
}

NamespacePrefix::NamingPolicy NamespacePrefix::getNamingPolicy() const
{
    return namingPolicy;
}

void NamespacePrefix::setNamingPolicy(NamingPolicy newNamingPolicy)
{
    if (namingPolicy == newNamingPolicy)
        return;
    namingPolicy = newNamingPolicy;
    emit namingPolicyChanged();
}

const QString &Rejection::getClassName() const
{
    return className;
}

void Rejection::setClassName(const QString &newClassName)
{
    if (className == newClassName)
        return;
    className = newClassName;
    emit classNameChanged();
}

const QString &Rejection::getFunctionName() const
{
    return functionName;
}

void Rejection::setFunctionName(const QString &newFunctionName)
{
    if (functionName == newFunctionName)
        return;
    functionName = newFunctionName;
    emit functionNameChanged();
}

const QString &Rejection::getFieldName() const
{
    return fieldName;
}

void Rejection::setFieldName(const QString &newFieldName)
{
    if (fieldName == newFieldName)
        return;
    fieldName = newFieldName;
    emit fieldNameChanged();
}

const QString &Rejection::getEnumName() const
{
    return enumName;
}

void Rejection::setEnumName(const QString &newEnumName)
{
    if (enumName == newEnumName)
        return;
    enumName = newEnumName;
    emit enumNameChanged();
}
