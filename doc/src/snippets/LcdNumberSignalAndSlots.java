import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class LcdNumberSignalAndSlots extends QWidget
{


    public LcdNumberSignalAndSlots()
    {
        LcdNumber lcd = new LcdNumber();
        QSpinBox box = new QSpinBox();

        QHBoxLayout layout = new QHBoxLayout();
        layout.addWidget(lcd);
        layout.addWidget(box);

        box.valueChanged.connect(lcd, "display(int)");

        setLayout(layout);
    }

    public void printValue(int value)
    {
        System.err.println("Value is set to "+value);
    }

    enum Mode { Hex, Decimal }

    class LcdNumber extends QFrame
    {
        public Signal0 overflow = new Signal0();

        Mode mode = Mode.Decimal;
        String currentDisplay = "100";

        public LcdNumber()
        {
            System.err.println("We are constructing");
            setFrameShape(Shape.Panel);
            setFrameShadow(Shadow.Sunken);

            setSizePolicy(new QSizePolicy(QSizePolicy.Policy.Expanding,
                                          QSizePolicy.Policy.Maximum));
        }

        void display(int num)
        {
            if (mode.equals(Mode.Decimal)) {
                currentDisplay = Integer.toString(num);
            } else {
                currentDisplay = Integer.toHexString(num);
            }
            repaint();
        }

        void setHexMode()
        {
            mode = Mode.Hex;
        }

        void setDecMode()
        {
            mode = Mode.Decimal;
        }

        public QSize sizeHint()
        {
            return new QFontMetrics(fontMetrics()).
                size(Qt.TextFlag.TextSingleLine.value(), currentDisplay).
                add(new QSize(5,5));
        }

        protected void paintEvent(QPaintEvent event)
        {
            super.paintEvent(event);

            QPainter painter = new QPainter(this);

            painter.drawText(new QRectF(rect()).adjusted(1, 1, -1, -1),
                             currentDisplay,
                             new QTextOption( new Qt.Alignment(
                             Qt.AlignmentFlag.AlignRight,
                             Qt.AlignmentFlag.AlignVCenter)));
        }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new LcdNumberSignalAndSlots().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
