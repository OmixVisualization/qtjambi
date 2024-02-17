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

#ifndef TS_LOADTYPESYSTEM_H
#define TS_LOADTYPESYSTEM_H

#include "qml_abstractobject.h"

class LoadTypeSystem : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit LoadTypeSystem(QObject *parent = nullptr);
    const QString &getName() const;
    void setName(const QString &newName);

    bool getGenerate() const;
    void setGenerate(bool newGenerate);

    bool getOptional() const;
    void setOptional(bool newOptional);

signals:
    void nameChanged();

    void generateChanged();

    void optionalChanged();

private:
    QString name;
    bool generate = false;
    bool optional = false;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged REQUIRED)
    Q_PROPERTY(bool generate READ getGenerate WRITE setGenerate NOTIFY generateChanged)
    Q_PROPERTY(bool optional READ getOptional WRITE setOptional NOTIFY optionalChanged)
};

QML_DECLARE_TYPE(LoadTypeSystem)

#endif // TS_LOADTYPESYSTEM_H
