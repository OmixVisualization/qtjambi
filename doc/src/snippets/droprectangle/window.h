#ifndef WINDOW_H
#define WINDOW_H

#include <QString>
#include <QStringList>
#include <QWidget>

class QComboBox;
class QFrame;
class QTextBrowser;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);

protected:
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

private:
    QComboBox *mimeTypeCombo;
    QFrame *dropFrame;
    QTextBrowser *textBrowser;
    QString oldText;
    QStringList oldMimeTypes;
};

#endif
