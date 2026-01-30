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

#ifndef INCLUDE_H
#define INCLUDE_H

#include "qml_abstractobject.h"

class Include : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    enum IncludeType {
        Global,
        Local,
        Java
    };
    Q_ENUM(IncludeType)
    explicit Include(QObject *parent = nullptr);
    const QString &getFileName() const;
    void setFileName(const QString &newFileName);

    const QString &getPpCondition() const;
    void setPpCondition(const QString &newPpCondition);

    IncludeType getLocation() const;
    void setLocation(IncludeType newLocation);

    bool getInherited() const;
    void setInherited(bool newInherited);

    bool getSuppressed() const;
    void setSuppressed(bool newSuppressed);

    const QStringList &getRequiredFeatures() const;
    void setRequiredFeatures(const QStringList &newRequiredFeatures);

    bool getCkeckAvailability() const;
    void setCkeckAvailability(bool newCkeckAvailability);

signals:
    void fileNameChanged();

    void ppConditionChanged();

    void locationChanged();

    void inheritedChanged();

    void suppressedChanged();

    void requiredFeaturesChanged();

    void ckeckAvailabilityChanged();

private:
    QString fileName;
    QString ppCondition;
    IncludeType location;
    bool inherited = false;
    bool suppressed = false;
    bool ckeckAvailability = false;
    QStringList requiredFeatures;
    Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QString ppCondition READ getPpCondition WRITE setPpCondition NOTIFY ppConditionChanged)
    Q_PROPERTY(IncludeType location READ getLocation WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(bool inherited READ getInherited WRITE setInherited NOTIFY inheritedChanged)
    Q_PROPERTY(bool suppressed READ getSuppressed WRITE setSuppressed NOTIFY suppressedChanged)
    Q_PROPERTY(QStringList requiredFeatures READ getRequiredFeatures WRITE setRequiredFeatures NOTIFY requiredFeaturesChanged)
    Q_PROPERTY(bool ckeckAvailability READ getCkeckAvailability WRITE setCkeckAvailability NOTIFY ckeckAvailabilityChanged)
};

class ExtraIncludes : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ExtraIncludes(QObject *parent = nullptr);

};

QML_DECLARE_TYPE(Include)
QML_DECLARE_TYPE(ExtraIncludes)

Q_DECLARE_METATYPE(Include::IncludeType)

#endif // INCLUDE_H
