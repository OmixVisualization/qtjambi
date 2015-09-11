#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class QListWidget;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    void setupListItems();

    QListWidget *listWidget;
};

#endif
