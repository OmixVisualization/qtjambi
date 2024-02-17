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

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "qml_abstractobject.h"

class Template : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Template(QObject *parent = nullptr);
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

signals:
    void nameChanged();

private:
    QString name;
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
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

QML_DECLARE_TYPE(Template)
QML_DECLARE_TYPE(InsertTemplate)
QML_DECLARE_TYPE(TemplateArguments)
QML_DECLARE_TYPE(Replace)

#endif // TEMPLATE_H
