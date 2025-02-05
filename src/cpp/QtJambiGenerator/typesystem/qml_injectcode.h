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

#ifndef INJECTCODE_H
#define INJECTCODE_H

#include "qml_abstractobject.h"

class InjectCode : public AbstractObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit InjectCode(QObject *parent = nullptr);
    const QString &getPackageName() const;
    void setPackageName(const QString &newPackageName);

    CodeClass::Entries getTarget() const;
    void setTarget(CodeClass::Entries newTarget);

    Position::Entries getPosition() const;
    void setPosition(Position::Entries newPosition);

signals:
    void packageNameChanged();

    void targetChanged();

    void positionChanged();

private:
    QString packageName;
    CodeClass::Entries target = CodeClass::Java;
    Position::Entries position = Position::Beginning;
    Q_PROPERTY(QString packageName READ getPackageName WRITE setPackageName NOTIFY packageNameChanged)
    Q_PROPERTY(CodeClass::Entries target READ getTarget WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(Position::Entries position READ getPosition WRITE setPosition NOTIFY positionChanged)
};

class ArgumentMap : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ArgumentMap(QObject *parent = nullptr);
    int getIndex() const;
    void setIndex(int newIndex);

    const QString &getMetaName() const;
    void setMetaName(const QString &newMetaName);

signals:
    void indexChanged();

    void metaNameChanged();

private:
    int index = 0;
    QString metaName;
    Q_PROPERTY(int index READ getIndex WRITE setIndex NOTIFY indexChanged REQUIRED)
    Q_PROPERTY(QString metaName READ getMetaName WRITE setMetaName NOTIFY metaNameChanged REQUIRED)
};

class Text : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Text(QObject *parent = nullptr);
    const QString &getContent() const;
    void setContent(const QString &newContent);
signals:
    void contentChanged();
private:
    QString content;
    Q_PROPERTY(QString content READ getContent WRITE setContent NOTIFY contentChanged REQUIRED)
    Q_CLASSINFO("DefaultProperty", "content")
};

class ImportFile : public AbstractObject{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ImportFile(QObject *parent = nullptr):AbstractObject(parent){};
    const QString &getQuoteAfterLine() const;
    void setQuoteAfterLine(const QString &newQuoteAfterLine);

    const QString &getQuoteBeforeLine() const;
    void setQuoteBeforeLine(const QString &newQuoteBeforeLine);

    const QString &getName() const;
    void setName(const QString &newName);

signals:
    void quoteAfterLineChanged();

    void quoteBeforeLineChanged();

    void nameChanged();

private:
    QString name;
    QString quoteAfterLine;
    QString quoteBeforeLine;
    Q_PROPERTY(QString quoteAfterLine READ getQuoteAfterLine WRITE setQuoteAfterLine NOTIFY quoteAfterLineChanged)
    Q_PROPERTY(QString quoteBeforeLine READ getQuoteBeforeLine WRITE setQuoteBeforeLine NOTIFY quoteBeforeLineChanged)
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged REQUIRED)
};

QML_DECLARE_TYPE(ImportFile)
QML_DECLARE_TYPE(InjectCode)
QML_DECLARE_TYPE(Text)
QML_DECLARE_TYPE(ArgumentMap)

#endif // INJECTCODE_H
