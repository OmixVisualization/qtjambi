import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;

import java.util.*;

public class src_corelib_io_qprocess {
    public static void main(String args[]) {
        QApplication.initialize(args);

        if (true)
        {
//! [0]
        QProcess builder = new QProcess();
        builder.setProcessChannelMode(QProcess.ProcessChannelMode.MergedChannels);

        List<String> arguments = new Vector<String>();
        arguments.add("-j2");

        builder.start("make", arguments);

        if (!builder.waitForFinished())
            System.err.println("Make failed:" + builder.errorString());
        else
            System.err.println("Make output:" + builder.readAll());
//! [0]
    }

    if (true)
    {
//! [1]
        QProcess more = new QProcess();
        more.start("more");
        more.write(new QByteArray("Text to display"));
        more.closeWriteChannel();
        // QProcess will emit readyRead() once "more" starts printing
//! [1]
    }

    /*
//! [2]
        command1 | command2
//! [2]
    */

    if (true)
    {
//! [3]
        QProcess process1 = new QProcess();
        QProcess process2 = new QProcess();

        process1.setStandardOutputProcess(process2);

        process1.start("command1");
        process2.start("command2");
//! [3]
    }


    if (true)
    {
//! [5]
        QProcess process = new QProcess();
        process.start("del /s *.txt");
        // same as process.start("del", List<String>() << "/s" << "*.txt");
        // ...
//! [5]
    }


    if (true)
    {
//! [6]
        QProcess process = new QProcess();
        process.start("dir \"My Documents\"");
//! [6]
    }

    if (true)
    {
//! [7]
        QProcess process = new QProcess();
        process.start("dir \"\"\"My Documents\"\"\"");
//! [7]
    }

    if (true)
    {
//! [8]
        List<String> environment = QProcess.systemEnvironment();
        // environment = {"PATH=/usr/bin:/usr/local/bin",
        //               "USER=greg", "HOME=/home/greg"}
//! [8]
    }

    }
    /*
//! [4]
        This code is not relevant for Qt Jambi.
        class SandboxProcess extends QProcess
        {

            protected void setupChildProcess()
            {
            // Drop all privileges in the child process, and enter
            // a chroot jail.
                if (QSysInfo.operatingSystem() == QSysInfo.OS_LINUX) {
                    setgroups(0, 0);
                    chroot("/etc/safe");
                    chdir("/");
                    setgid(safeGid);
                    setuid(safeUid);
                }
            }
        }

//! [4]
*/
}
