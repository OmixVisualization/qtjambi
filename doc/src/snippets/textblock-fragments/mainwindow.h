#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

public slots:
    void insertCalendar();
    void saveFile();

private:
    bool writeXml(const QString &fileName);

    QTextEdit *editor;
};

#endif
