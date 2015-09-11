#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPoint>

class QLabel;
class QLineEdit;
class QMouseEvent;
class QTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

protected:
    void mousePressEvent(QMouseEvent *event);

private:
    QLabel *iconLabel;
    QLineEdit *nameEdit;
    QPixmap iconPixmap;
    QPoint dragStartPosition;
    QTextEdit *commentEdit;
};

#endif
