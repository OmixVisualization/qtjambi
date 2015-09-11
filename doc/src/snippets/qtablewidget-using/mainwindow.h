#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class QAction;
class QTableWidget;
class QTableWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void averageItems();
    void sumItems();

private:
    void setupTableItems();

    QAction *removeAction;
//! [0]
    QTableWidget tableWidget;
//! [0]
};

#endif
