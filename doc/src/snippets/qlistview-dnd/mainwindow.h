#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class QListView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private:
    void setupListItems();

    QListView *listView;
};

#endif
