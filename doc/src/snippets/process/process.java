import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class process
{

    static boolean zip()
    {
    //! [0]
        QProcess gzip = new QProcess();
        List<String> args = new Vector<String>();
        args.add("-c");

        gzip.start("gzip", args);
        if (!gzip.waitForStarted())
            return false;

        gzip.write(new QByteArray("Qt rocks!"));
        gzip.closeWriteChannel();

        if (!gzip.waitForFinished())
            return false;

        QByteArray result = gzip.readAll();
    //! [0]

        /*gzip.start("gzip", QStringList() << "-d" << "-c");
        gzip.write(result);
        gzip.closeWriteChannel();

        if (!gzip.waitForFinished())
            return false;

        qDebug("Result: %s", gzip.readAll().data());*/
        return true;
    }

    public static void main(String args[])
    {
        zip();
    }
}
