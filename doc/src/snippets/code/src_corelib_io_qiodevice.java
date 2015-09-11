import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;
import java.util.*;


public class src_corelib_io_qiodevice {
    public static void main(String args[]) {
        QApplication.initialize(args);

        class Dummy {
        public boolean dummymethod() {
//! [0]
        QProcess gzip = new QProcess();
        gzip.start("gzip", Arrays.asList("-c"));
        if (!gzip.waitForStarted())
            return false;

        gzip.write(new QByteArray("uncompressed data"));

        QByteArray compressed = new QByteArray();
        while (gzip.waitForReadyRead(5000))
            compressed.append(gzip.readAll());
//! [0]
        return true;
        } //method
        } //class

        abstract class DummyDevice extends QIODevice {
//! [1]
        public long bytesAvailable()
        {
            return buffer.size() + super.bytesAvailable();
        }
//! [1]
        private QByteArray buffer;
        } //class


//! [2]
        QFile file = new QFile("box.txt");
        if (file.open(QIODevice.OpenModeFlag.ReadOnly)) {
            byte[] buf = new byte[1024];
            long lineLength = file.readLine(buf);
            if (lineLength != -1) {
                // the line is available in buf
            }
        }
//! [2]

    abstract class DummyDevice2 extends QIODevice {
//! [3]
        public boolean canReadLine()
        {
            return buffer.contains("\n") || super.canReadLine();
        }
//! [3]
        private QByteArray buffer;
    } //class


    class Dummy2 {
//! [4]
        public boolean isExeFile(QFile file)
        {
            byte[] buf = new byte[2];
            if (file.peek(buf) == buf.length)
                return (buf[0] == 'M' && buf[1] == 'Z');
            return false;
        }
//! [4]
    } //class
    class Dummy3 {
//! [5]
        public boolean isExeFile(QFile file)
        {
            return file.peek(2).equals("MZ");
        }
//! [5]
    } //class

    }
}
