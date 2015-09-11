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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QDockWidget;
class QListWidget;
class QListWidgetItem;
class QTextBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

private slots:
    void updateText(QListWidgetItem *item);

private:
    void setupContents();
    void setupDockWindow();
    void setupMenus();

    QDockWidget *contentsWindow;
    QListWidget *headingList;
    QTextBrowser *textBrowser;
};

#endif
