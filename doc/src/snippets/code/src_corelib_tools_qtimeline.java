import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_tools_qtimeline extends QWidget{
    public void mainy(String args[]) {
        QApplication.initialize(args);

        QPushButton pushButton;
        QProgressBar progressBar;
//! [0]
        // ...
        progressBar = new QProgressBar(this);
        progressBar.setRange(0, 100);

        // Construct a 1-second timeline with a frame range of 0 - 100
        QTimeLine timeLine = new QTimeLine(1000, this);
        timeLine.setFrameRange(0, 100);
        timeLine.frameChanged.connect(progressBar, "setValue(int)");

        // Clicking the push button will start the progress bar animation
        pushButton = new QPushButton(tr("Start animation"), this);
        pushButton.clicked.connect(timeLine, "start()");
        // ...
//! [0]

    }
}

