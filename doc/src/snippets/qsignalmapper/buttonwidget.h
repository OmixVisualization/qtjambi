#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include <qwidget.h>

class QSignalMapper;
class QString;
class QStringList;

//! [0]
class ButtonWidget extends QWidget
{
    ...

    public ButtonWidget(List<String> texts) {...};

    public Signal1<String> clicked = new Qignal1<String>();

    private QSignalMapper signalMapper;
//! [0] //! [1]
}
//! [1]

#endif
