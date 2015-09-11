#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class QAction;
class QListWidget;
class QListWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void insertItem();
    void removeItem();
    void sortAscending();
    void sortDescending();
    void updateMenus(QListWidgetItem *current);

private:
    void setupListItems();

    QAction *insertAction;
    QAction *removeAction;
    QListWidget *listWidget;
};

#endif
