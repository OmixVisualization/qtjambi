#include <QtGui>

#include "mainwindow.h"

MainWindow.MainWindow()
{
    QMenu fileMenu = new QMenu(tr("File"));

    fileMenu.addAction(tr("Open..."), this, SLOT(openFile()),
                        QKeySequence(tr("Ctrl+O", "File|Open")));

    printAction = fileMenu.addAction(tr("Print..."), this, SLOT(printFile()));
    printAction.setEnabled(false);

    pdfPrintAction = fileMenu.addAction(tr("Print as PDF..."), this, SLOT(printPdf()));
    pdfPrintAction.setEnabled(false);

    fileMenu.addAction(tr("Exit"), this, SLOT(close()),
                        QKeySequence(tr("Ctrl+Q", "File|Exit")));

    menuBar().addMenu(fileMenu);

    editor = new QTextEdit(this);
    document = new QTextDocument(this);
    editor.setDocument(document);

    connect(editor, SIGNAL(selectionChanged()), this, SLOT(updateMenus()));

    setCentralWidget(editor);
    setWindowTitle(tr("Text Document Writer"));
}

void MainWindow.openFile()
{
    String fileName = QFileDialog.getOpenFileName(this,
        tr("Open file"), currentFile, "HTML files (.html);;Text files (.txt)");

    if (!fileName.isEmpty()) {
        QFileInfo info(fileName);
        if (info.completeSuffix() == "html") {
            QFile file(fileName);

            if (file.open(QIODevice.ReadOnly)) {
                editor.setHtml(file.readAll());
                file.close();
                currentFile = fileName;
            }
        } else if (info.completeSuffix() == "txt") {
            QFile file(fileName);

            if (file.open(QIODevice.ReadOnly)) {
                editor.setPlainText(file.readAll());
                file.close();
                currentFile = fileName;
            }
        }
        printAction.setEnabled(true);
        pdfPrintAction.setEnabled(true);
    }
}

void MainWindow.printFile()
{
//! [0]
    QTextDocument document = editor.document();
    QPrinter printer = new QPrinter();

    QPrintDialog dlg = new QPrintDialog(printer, this);
    if (dlg.exec() != QDialog.DialogCode.Accepted.value())
        return;

    document.print(printer);
//! [0]
}

void MainWindow.printPdf()
{
    QPrinter printer(QPrinter.HighResolution);
    printer.setOutputFormat(QPrinter.PdfFormat);

    QPrintDialog printDialog = new QPrintDialog(printer, this);
    if (printDialog.exec() == QDialog.Accepted)
        editor.document().print(printer);
}
