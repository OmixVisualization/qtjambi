/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

#include "qml_include.h"

Include::Include(QObject *parent)
    : AbstractObject{parent}
{

}
const QString &Include::getFileName() const
{
    return fileName;
}

void Include::setFileName(const QString &newFileName)
{
    if (fileName == newFileName)
        return;
    fileName = newFileName;
    emit fileNameChanged();
}

const QString &Include::getPpCondition() const
{
    return ppCondition;
}

void Include::setPpCondition(const QString &newPpCondition)
{
    if (ppCondition == newPpCondition)
        return;
    ppCondition = newPpCondition;
    emit ppConditionChanged();
}

Include::IncludeType Include::getLocation() const
{
    return location;
}

void Include::setLocation(IncludeType newLocation)
{
    if (location == newLocation)
        return;
    location = newLocation;
    emit locationChanged();
}

bool Include::getInherited() const
{
    return inherited;
}

void Include::setInherited(bool newInherited)
{
    if (inherited == newInherited)
        return;
    inherited = newInherited;
    emit inheritedChanged();
}

bool Include::getSuppressed() const
{
    return suppressed;
}

void Include::setSuppressed(bool newSuppressed)
{
    if (suppressed == newSuppressed)
        return;
    suppressed = newSuppressed;
    emit suppressedChanged();
}

const QStringList &Include::getRequiredFeatures() const
{
    return requiredFeatures;
}

void Include::setRequiredFeatures(const QStringList &newRequiredFeatures)
{
    if (requiredFeatures == newRequiredFeatures)
        return;
    requiredFeatures = newRequiredFeatures;
    emit requiredFeaturesChanged();
}

bool Include::getCkeckAvailability() const
{
    return ckeckAvailability;
}

void Include::setCkeckAvailability(bool newCkeckAvailability)
{
    if (ckeckAvailability == newCkeckAvailability)
        return;
    ckeckAvailability = newCkeckAvailability;
    emit ckeckAvailabilityChanged();
}

ExtraIncludes::ExtraIncludes(QObject *parent)
    : AbstractObject{parent}
{

}
