#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QPoint>

class QComboBox;
class QLabel;
class QLineEdit;
class QMouseEvent;
class QTextEdit;
class DragWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

public slots:
    void setDragResult(const QString &actionText);
    void setMimeTypes(const QStringList &types);

private:
    QComboBox *mimeTypeCombo;
    DragWidget *dragWidget;
};

#endif
