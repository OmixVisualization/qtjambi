#ifndef WINDOW_H
#define WINDOW_H

#include <QString>
#include <QStringList>
#include <QWidget>

class QComboBox;
class QFrame;
class QLabel;

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    MyWidget(QWidget *parent = 0);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QComboBox *mimeTypeCombo;
    QFrame *dropFrame;
    QLabel *dataLabel;
    QString oldText;
    QStringList oldMimeTypes;
};

#endif
