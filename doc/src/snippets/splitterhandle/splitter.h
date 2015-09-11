#ifndef SPLITTER_H
#define SPLITTER_H

#include <QLinearGradient>
#include <QSplitter>
#include <QSplitterHandle>

class QPaintEvent;

//! [0]
public class Splitter extends QSplitter
{

    public Splitter(Qt.Orientation orientation, QWidget parent)
    {
        ...
    }

    protected QSplitterHandle createHandle()
    {
        ...
    }
};
//! [0]

class SplitterHandle : public QSplitterHandle
{
public:
    SplitterHandle(Qt::Orientation orientation, QSplitter *parent);

protected:
    void paintEvent(QPaintEvent *event);

private:
    QLinearGradient gradient;
};

#endif
