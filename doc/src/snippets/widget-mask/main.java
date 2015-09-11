import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class main
{

    public static void main(String args[])
    {
        QApplication.initialize(args);
    //! [0]
        QLabel topLevelLabel = new QLabel();
        QPixmap pixmap = new QPixmap("classpath:tux.png");
        topLevelLabel.setPixmap(pixmap);
        topLevelLabel.setMask(pixmap.mask());
    //! [0]
        topLevelLabel.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
