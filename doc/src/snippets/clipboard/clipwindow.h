#ifndef CLIPWINDOW_H
#define CLIPWINDOW_H

#include <QMainWindow>

class QClipboard;
class QComboBox;
class QLabel;
class QListWidget;
class QMimeData;
class QWidget;

class ClipWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClipWindow(QWidget *parent = 0);

public slots:
    void updateClipboard();
    void updateData(const QString &format);

private:
    int currentItem;
    QClipboard *clipboard;
    QComboBox *mimeTypeCombo;
    QLabel *dataInfoLabel;
    QListWidget *previousItems;
};

#endif
