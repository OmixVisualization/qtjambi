#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QTextDocumentFragment>

class QAction;
class QTextDocument;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void cutSelection();
    void copySelection();
    void openFile();
    void pasteSelection();
    void selectWord();
    void selectLine();
    void selectBlock();
    void selectFrame();
    void updateMenus();

private:
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QString currentFile;
    QTextEdit *editor;
    QTextDocument *document;
    QTextDocumentFragment selection;
};

#endif
