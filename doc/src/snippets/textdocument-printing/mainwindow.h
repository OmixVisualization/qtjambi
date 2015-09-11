#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QHash>
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
    void openFile();
    void printFile();
    void printPdf();

private:
    QAction *printAction;
    QAction *pdfPrintAction;
    QString currentFile;
    QTextEdit *editor;
    QTextDocument *document;
};

#endif
