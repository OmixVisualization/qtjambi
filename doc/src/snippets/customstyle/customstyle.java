import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class customstyle extends QWindowsStyle
{
    QSpinBox widget;

    public customstyle()
    {
    //! [0]
        if (widget instanceof QSpinBox) {
            QSpinBox spinBox = (QSpinBox) widget;
    //! [0] //! [1]
        }
    //! [1]
    }

    //! [2]
    public void drawPrimitive(QStyle.PrimitiveElement element, QStyleOption option,
                              QPainter painter, QWidget widget)
    {
        if (element.equals(QStyle.PrimitiveElement.PE_IndicatorSpinUp) ||
            element.equals(QStyle.PrimitiveElement.PE_IndicatorSpinDown)) {
        QPolygon points = new QPolygon(3);
        int x = option.rect().x();
        int y = option.rect().y();
        int w = option.rect().width() / 2;
        int h = option.rect().height() / 2;
        x += (option.rect().width() - w) / 2;
        y += (option.rect().height() - h) / 2;

        if (element.equals(QStyle.PrimitiveElement.PE_IndicatorSpinUp)) {
            points.add(new QPoint(x, y + h));
            points.add(new QPoint(x + w, y + h));
            points.add(new QPoint(x + w / 2, y));
        } else { // PE_SpinBoxDown
            points.add(new QPoint(x, y));
            points.add(new QPoint(x + w, y));
            points.add(new QPoint(x + w / 2, y + h));
        }

        if (option.state().isSet(QStyle.StateFlag.State_Enabled)) {
            painter.setPen(option.palette().mid().color());
            painter.setBrush(option.palette().buttonText());
        } else {
            painter.setPen(option.palette().buttonText().color());
            painter.setBrush(option.palette().mid());
        }
        painter.drawPolygon(points);
        } else {
        super.drawPrimitive(element, option, painter, widget);
    //! [2] //! [3]
        }
    //! [3] //! [4]
    }
    //! [4]
}
