#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QByteArray>
#include <QFrame>
#include <QString>
#include <QStringList>

class QComboBox;
class QFrame;
class QLabel;
class QTextBrowser;

class DragWidget : public QFrame
{
    Q_OBJECT

public:
    DragWidget(QWidget *parent);
    void setData(const QString &mimetype, const QByteArray &newData);

signals:
    void dragResult(const QString &actionText);
    void mimeTypes(const QStringList &types);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

private:
    QByteArray data;
    QLabel *dragDropLabel;
    QPoint dragStartPosition;
    QString mimeType;
};

#endif
