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

#include "qml_loadtypesystem.h"

LoadTypeSystem::LoadTypeSystem(QObject *parent)
    : AbstractObject{parent}
{

}

const QString &LoadTypeSystem::getName() const
{
    return name;
}

void LoadTypeSystem::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();
}

bool LoadTypeSystem::getGenerate() const
{
    return generate;
}

void LoadTypeSystem::setGenerate(bool newGenerate)
{
    if (generate == newGenerate)
        return;
    generate = newGenerate;
    emit generateChanged();
}

bool LoadTypeSystem::getOptional() const
{
    return optional;
}

void LoadTypeSystem::setOptional(bool newOptional)
{
    if (optional == newOptional)
        return;
    optional = newOptional;
    emit optionalChanged();
}
