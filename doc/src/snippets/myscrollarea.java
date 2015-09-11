import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

class myscrollarea extends QAbstractScrollArea
{
    QWidget widget;

    public myscrollarea(QWidget widget)
    {
        this.widget = widget;
    }

    public void setWidget(QWidget w)
    {
        widget = w;
        widget.setParent(viewport());
        if (!widget.testAttribute(Qt.WidgetAttribute.WA_Resized))
            widget.resize(widget.sizeHint());

        verticalScrollBar().setValue(0);
        verticalScrollBar().setValue(0);

        updateArea();
    }

    public void updateWidgetPosition()
    {
    //! [0]
        int hvalue = horizontalScrollBar().value();
        int vvalue = verticalScrollBar().value();
        QPoint topLeft = viewport().rect().topLeft();

        widget.move(topLeft.x() - hvalue, topLeft.y() - vvalue);
    //! [0]
    }

    @Override
    public void scrollContentsBy(int dx, int dy)
    {
        updateWidgetPosition();
    }

    protected void updateArea()
    {
    //! [1]
        QSize areaSize = viewport().size();
        QSize  widgetSize = widget.size();

        verticalScrollBar().setPageStep(widgetSize.height());
        horizontalScrollBar().setPageStep(widgetSize.width());
        verticalScrollBar().setRange(0, widgetSize.height() - areaSize.height());
        horizontalScrollBar().setRange(0, widgetSize.width() - areaSize.width());
        updateWidgetPosition();
    //! [1]
    }

    public void resizeEvent(QResizeEvent event)
    {
        updateArea();
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QPixmap pixmap = new QPixmap("mypixmap.png");
        QLabel label = new QLabel();
        label.setPixmap(pixmap);
        myscrollarea area = new myscrollarea(label);
        area.resize(300, 300);
        area.show();

        area.setWidget(label);

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
