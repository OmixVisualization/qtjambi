#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H

#include <QWindowsStyle>

//! [0]
public class CustomStyle extends QWindowsStyle
{
    public CustomStyle() { ... }

    public void drawPrimitive(QStyle.PrimitiveElement element, QStyleOption option,
                              QPainter painter, QWidget widget)
    {
        ...
    }
}
//! [0]

#endif
