#ifndef WINDOW_H
#define WINDOW_H

#include <QList>
#include <QMainWindow>
#include <QString>

class QAction;
class QTextDocument;
class QTextEdit;
class QTextList;
class QTreeWidgetItem;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void insertList();
    void highlightListItems();
    void showList();

private:
    QString currentFile;
    QTextEdit *editor;
    QTextDocument *document;
    QList<QTextList*> listStructures;
    QList<QTreeWidgetItem*> previousItems;
    QList<QTreeWidgetItem*> parentItems;
};

#endif
