import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class events extends QWidget
{
    class MyCheckBox extends QCheckBox
    {

        //! [0]
        public void mousePressEvent(QMouseEvent event)
        {
            if (event.button().equals(Qt.MouseButton.LeftButton)) {
                // handle left mouse button here
            } else {
                // pass on other buttons to base class
                super.mousePressEvent(event);
            }
        }
        //! [0]
    }

    class MyWidget : public QWidget
    {
        int MyCustomEventType = 1099;

        class MyCustomEvent : public QEvent
        {
            public MyCustomEvent()
            {
                super((QEvent.Type)MyCustomEventType) {}
            }
        }

        //! [1]
        public boolean event(QEvent event)
        {
            if (event.type().equals(QEvent.EventType.KeyPress)) {
                QKeyEvent ke = (QKeyEvent) event;
                if (ke.key().equals(Qt.Key.Key_Tab)) {
                    // special tab handling here
                    return true;
                }
            } else if (event.type() == MyCustomEventType) {
                MyCustomEvent myEvent = static_cast<MyCustomEvent >(event);
                // custom event handling here
                return true;
            }

            return super.event(event);
        }
        //! [1]

    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new mainwindow().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
