import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_io_qfileinfo {
    public static void main(String args[]) {
        QApplication.initialize(args);

        {
//! [0]
        QFileInfo info1 = new QFileInfo("/home/bob/bin/untabify");
        info1.isSymLink();          // returns true
        info1.absoluteFilePath();   // returns "/home/bob/bin/untabify"
        info1.size();               // returns 56201
        info1.symLinkTarget();      // returns "/opt/pretty++/bin/untabify"

        QFileInfo info2 = new QFileInfo(info1.symLinkTarget());
        info1.isSymLink();          // returns false
        info1.absoluteFilePath();   // returns "/opt/pretty++/bin/untabify"
        info1.size();               // returns 56201
//! [0]
        }

        {
//! [1]
        QFileInfo info1 = new QFileInfo("C:\\Documents and Settings\\Bob\\untabify.lnk");
        info1.isSymLink();          // returns true
        info1.absoluteFilePath();   // returns "C:/Documents and Settings/Bob/untabify.lnk"
        info1.size();               // returns 743
        info1.symLinkTarget();      // returns "C:/Pretty++/untabify"

        QFileInfo info2 = new QFileInfo(info1.symLinkTarget());
        info1.isSymLink();          // returns false
        info1.absoluteFilePath();   // returns "C:/Pretty++/untabify"
        info1.size();               // returns 63942
//! [1]
        }
    {
//! [2]
    String absolute = "/local/bin";
    String relative = "local/bin";
    QFileInfo absFile = new QFileInfo(absolute);
    QFileInfo relFile = new QFileInfo(relative);

    QDir.setCurrent(QDir.rootPath());
    // absFile and relFile now point to the same file

    QDir.setCurrent("/tmp");
    // absFile now points to "/local/bin",
    // while relFile points to "/tmp/local/bin"
//! [2]
    }
    {
//! [3]
        QFileInfo fi = new QFileInfo("/tmp/archive.tar.gz");
        String name = fi.fileName();                // name = "archive.tar.gz"
//! [3]
    }
    {
//! [4]
        QFileInfo fi = new QFileInfo("/Applications/Safari.app");
        String bundle = fi.bundleName();                // name = "Safari"
//! [4]
    }
    {
//! [5]
        QFileInfo fi = new QFileInfo("/tmp/archive.tar.gz");
        String base = fi.baseName();  // base = "archive"
//! [5]
    }
    {
//! [6]
        QFileInfo fi = new QFileInfo("/tmp/archive.tar.gz");
        String base = fi.completeBaseName();  // base = "archive.tar"
//! [6]
    }
    {
//! [7]
        QFileInfo fi = new QFileInfo("/tmp/archive.tar.gz");
        String ext = fi.completeSuffix();  // ext = "tar.gz"
//! [7]
    }
    {
//! [8]
        QFileInfo fi = new QFileInfo("/tmp/archive.tar.gz");
        String ext = fi.suffix();  // ext = "gz"
//! [8]
    }
    {
    String fileName = "";
//! [9]
        QFileInfo info = new QFileInfo(fileName);
        if (info.isSymLink())
            fileName = info.symLinkTarget();
//! [9]
    }
    {
//! [10]
        QFileInfo fi = new QFileInfo("/tmp/archive.tar.gz");
        QFile.Permissions permissions1
            = new QFile.Permissions(QFile.Permission.WriteUser, QFile.Permission.ReadGroup);
        if (fi.permission(permissions1))
            System.out.println("I can change the file; my group can read the file");
        QFile.Permissions permissions2
            = new QFile.Permissions(QFile.Permission.WriteGroup, QFile.Permission.WriteOther);
        if (fi.permission(permissions2))
            System.out.println("The group or others can change the file");
//! [10]
    }

    }
}
