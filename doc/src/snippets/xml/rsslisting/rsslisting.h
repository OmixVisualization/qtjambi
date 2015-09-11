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

#ifndef RSSLISTING_H
#define RSSLISTING_H

#include <QHttp>
#include <QWidget>
#include <QXmlInputSource>
#include <QXmlSimpleReader>

#include "handler.h"

class QLineEdit;
class QTreeWidget;
class QTreeWidgetItem;
class QPushButton;

class RSSListing : public QWidget
{
    Q_OBJECT
public:
    RSSListing(QWidget *widget = 0);

public slots:
    void addItem(QString &title, QString &link);
    void fetch();
    void finished(int id, bool error);
    void readData(const QHttpResponseHeader &);

private:
    Handler *handler;
    QXmlInputSource xmlInput;
    QXmlSimpleReader xmlReader;

    bool newInformation;

    QHttp http;
    int connectionId;

    QLineEdit *lineEdit;
    QTreeWidget *treeWidget;
    QTreeWidgetItem *lastItemCreated;
    QPushButton *abortButton;
    QPushButton *fetchButton;
};

#endif

