import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class styles extends QWidget
{

    //! [0] //! [1]
    protected void paintEvent(QPaintEvent event)
    //! [0]
    {
    //! [2]
        QPainter painter = new QPainter(this);
    //! [2]

        QStyleOptionFocusRect option = new QStyleOptionFocusRect();
        option.initFrom(this);
        option.setBackgroundColor(palette().color(QPalette.ColorRole.Window));

    //! [3]
        style().drawPrimitive(QStyle.PrimitiveElement.PE_FrameFocusRect,
                              option, painter, this);
    //! [3]
    }
    //! [1]

    protected void paintEvent2(QPaintEvent event)
    //! [4]
    {
    //! [4] //! [5] //! [6]
        QStylePainter painter = new QStylePainter(this);
    //! [5]

        QStyleOptionFocusRect option = new QStyleOptionFocusRect();
        option.initFrom(this);
        option.setBackgroundColor(palette().color(QPalette.ColorRole.Window));

    //! [7]
        painter.drawPrimitive(QStyle.PrimitiveElement.PE_FrameFocusRect, option);
    //! [7]
    }
    //! [6]

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new styles().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
