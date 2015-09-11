import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

class timers extends QWidget
{

    timers()
    {
        if (true)
        {
    //! [0]
        QTimer timer = new QTimer(this);
    //! [0] //! [1]
        timer.timeout.connect(this, "updateCaption()");
    //! [1] //! [2]
        timer.start(1000);
    //! [2]
        }

        if (true)
        {
    //! [3]
        QTimer.singleShot(200, this, "updateCaption()");
    //! [3]
        }

        if (true)
        {
        // ZERO-CASE
    //! [4]
        QTimer timer = new QTimer(this);
    //! [4] //! [5]
        timer.timeout.connect(this, "processOneThing()");
    //! [5] //! [6]
        timer.start();
    //! [6]
        }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new timers();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
