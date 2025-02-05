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

#include "qml_injectcode.h"

InjectCode::InjectCode(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &InjectCode::getPackageName() const
{
    return packageName;
}

void InjectCode::setPackageName(const QString &newPackageName)
{
    if (packageName == newPackageName)
        return;
    packageName = newPackageName;
    emit packageNameChanged();
}

CodeClass::Entries InjectCode::getTarget() const
{
    return target;
}

void InjectCode::setTarget(CodeClass::Entries newTarget)
{
    if (target == newTarget)
        return;
    target = newTarget;
    emit targetChanged();
}

Position::Entries InjectCode::getPosition() const
{
    return position;
}

void InjectCode::setPosition(Position::Entries newPosition)
{
    if (position == newPosition)
        return;
    position = newPosition;
    emit positionChanged();
}

ArgumentMap::ArgumentMap(QObject *parent)
    : AbstractObject{parent}
{

}

int ArgumentMap::getIndex() const
{
    return index;
}

void ArgumentMap::setIndex(int newIndex)
{
    if (index == newIndex)
        return;
    index = newIndex;
    emit indexChanged();
}

const QString &ArgumentMap::getMetaName() const
{
    return metaName;
}

void ArgumentMap::setMetaName(const QString &newMetaName)
{
    if (metaName == newMetaName)
        return;
    metaName = newMetaName;
    emit metaNameChanged();
}

Text::Text(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &Text::getContent() const
{
    return content;
}

void Text::setContent(const QString &newContent)
{
    if (content == newContent)
        return;
    content = newContent;
    emit contentChanged();
}

const QString &ImportFile::getQuoteAfterLine() const
{
    return quoteAfterLine;
}

void ImportFile::setQuoteAfterLine(const QString &newQuoteAfterLine)
{
    if (quoteAfterLine == newQuoteAfterLine)
        return;
    quoteAfterLine = newQuoteAfterLine;
    emit quoteAfterLineChanged();
}

const QString &ImportFile::getQuoteBeforeLine() const
{
    return quoteBeforeLine;
}

void ImportFile::setQuoteBeforeLine(const QString &newQuoteBeforeLine)
{
    if (quoteBeforeLine == newQuoteBeforeLine)
        return;
    quoteBeforeLine = newQuoteBeforeLine;
    emit quoteBeforeLineChanged();
}

const QString &ImportFile::getName() const
{
    return name;
}

void ImportFile::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}
