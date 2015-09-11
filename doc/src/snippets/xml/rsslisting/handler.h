/****************************************************************************
**
** Copyright (C) 2004-2009 Nokia. All rights reserved.
**
** This file is part of an example program for Qt.
** EDITIONS: NOLIMITS
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef HANDLER_H
#define HANDLER_H

#include <QObject>
#include <QString>
#include <QXmlDefaultHandler>

/* Note that QObject must precede QXmlDefaultHandler in the following list. */

class Handler : public QObject, public QXmlDefaultHandler
{
    Q_OBJECT
public:
    bool startDocument();
    bool startElement(const QString &, const QString &, const QString &qName,
                       const QXmlAttributes &attr);
    bool endElement(const QString &, const QString &, const QString &qName);
    bool characters(const QString &chars);

    bool fatalError(const QXmlParseException &exception);

signals:
    void newItem(QString &title, QString &link);

private:
    QString titleString;
    QString linkString;
    bool inItem;
    bool inTitle;
    bool inLink;
};

#endif

