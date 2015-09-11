import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{

    //! [0]
    public static void main(String args[])
    {
        QApplication.initialize(args);
        QPixmap pixmap = new QPixmap("classpath:splash.png");
        QSplashScreen splash = new QSplashScreen(pixmap);
        splash.show();
        QApplication.processEvents();
    //! [0]

        try {
            Thread.currentThread().sleep(5000);
        } catch(Exception e) {}
    //! [1]

        QMainWindow window = new QMainWindow();
        window.show();
        splash.finish(window);
        QApplication.execStatic();
        QApplication.shutdown();
    }
    //! [1]

}
