import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main extends QMainWindow
{

    //! [0] //! [1]
    public main()
    {
    //! [0]
        QWidget myWidget = new QWidget();
        myWidget.setStatusTip(tr("This is my widget."));

        setCentralWidget(myWidget);
    //! [1]

    //! [2]
        QMenu fileMenu = menuBar().addMenu(tr("File"));

        QAction newAct = new QAction(tr("New"), this);
        newAct.setStatusTip(tr("Create a new file."));
        fileMenu.addAction(newAct);
    //! [2]

        statusBar().showMessage(tr("Ready"));
        setWindowTitle(tr("QStatusTipEvent"));
    //! [3]
    }
    //! [3]

    public static void  main(String args[])
    {
        QApplication.initialize(args);

        MainWindow main = new main();
        main.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
