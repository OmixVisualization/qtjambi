import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class splitter extends QSplitter
{
    public splitter(Qt.Orientation orientation, QWidget parent)
    {
        super(orientation, parent);
    }

    //! [1]
    protected QSplitterHandle createHandle()
    {
        return new SplitterHandle(orientation(), this);
    }
    //! [1]
}

class SplitterHandle extends QSplitterHandle
{
    QLinearGradient gradient = new QLinearGradient();

    public SplitterHandle(Qt.Orientation orientation, QSplitter parent)
    {
        super(orientation, parent);

        gradient.setColorAt(0.0, new QColor(Qt.GlobalColor.darkGreen));
        gradient.setColorAt(0.25, new QColor(Qt.GlobalColor.white));
        gradient.setColorAt(1.0, new QColor(Qt.GlobalColor.darkGreen));
    }

    //! [0]
    protected void paintEvent(QPaintEvent event)
    {
        QPainter painter = new QPainter(this);

        if (orientation().equals(Qt.Orientation.Horizontal)) {
            gradient.setStart(rect().left(), rect().height() / 2);
            gradient.setFinalStop(rect().right(), rect().height() / 2);
        } else {
            gradient.setStart(rect().width() / 2, rect().top());
            gradient.setFinalStop(rect().width() / 2, rect().bottom());
        }
        painter.fillRect(event.rect(), new QBrush(gradient));
    }
    //! [0]
}

