import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

class SimpleExecution
{

    public static void main(String args[])
    {
        QApplication.initialize(args);
//! [0]
        QObject parent = QApplication.instance();
//! [0]

//! [1]
        String program = "./path/to/my/favorite/program";
//! [1]
        program = "./../../../../examples/widgets/analogclock/analogclock";

//! [2]
        List<String> arguments = new Vector<String>();
        arguments.add("-style");
        arguments.add("motif");

        QProcess myProcess = new QProcess(parent);
        myProcess.start(program, arguments);
//! [2]

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
