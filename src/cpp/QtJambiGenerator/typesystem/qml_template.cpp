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

#include "qml_template.h"

Template::Template(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &Template::getName() const
{
    return name;
}

void Template::setName(const QString &newName)
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
