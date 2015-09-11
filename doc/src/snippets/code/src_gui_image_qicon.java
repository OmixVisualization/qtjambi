import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class src_gui_image_qicon {
    public static void main(String args[]) {
        QApplication.initialize(args);

//! [0]
        QToolButton button = new QToolButton();
        button.setIcon(new QIcon("open.xpm"));
//! [0]


//! [1]
        button.setIcon(null);
//! [1]

    }

//! [2]
    void drawIcon(QPainter ainter, QPoint pos)
    {
        QPixmap pixmap = icon.pixmap(new QSize(22, 22),
                                     isEnabled() ? QIcon.Mode.Normal : QIcon.Mode.Disabled,
                                     isOn() ? QIcon.State.On : QIcon.State.Off);
        painter.drawPixmap(pos, pixmap);
    }
//! [2]

    private boolean isEnabled() { return false; }
    private boolean isOn() { return false; }

    private QPainter painter;
    private QIcon icon;
}
