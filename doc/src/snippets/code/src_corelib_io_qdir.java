import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;

import java.util.*;

public class src_corelib_io_qdir {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
    new QDir("/home/user/Documents");
    new QDir("C:/Documents and Settings");
//! [0]


//! [1]
    new QDir("images/landscape.png");
//! [1]


//! [2]
    new QDir("Documents/Letters/Applications").dirName(); // "Applications"
    new QDir().dirName();                                 // "."
//! [2]


//! [3]
    QDir directory = new QDir("Documents/Letters");
    String path = directory.filePath("contents.txt");
    String absolutePath = directory.absoluteFilePath("contents.txt");
//! [3]


    if (true)
    {
//! [4]
        QDir dir = new QDir("example");
        if (!dir.exists())
            System.err.println("Cannot find the example directory");
//! [4]
    }


    if (true)
    {
//! [5]
        QDir dir = QDir.root();                 // "/"
        if (!dir.cd("tmp")) {                    // "/tmp"
            System.err.println("Cannot find the \"/tmp\" directory");
        } else {
            QFile file = new QFile(dir.filePath("ex1.txt")); // "/tmp/ex1.txt"
            if (!file.open(QIODevice.OpenModeFlag.ReadWrite))
                System.err.println("Cannot create the file " + file.fileName());
        }
//! [5]
    }


//! [6]
        String bin = "/local/bin";         // where /local/bin is a symlink to /usr/bin
        QDir binDir = new QDir(bin);
        String canonicalBin = binDir.canonicalPath();
        // canonicalBin now equals "/usr/bin"

        String ls = "/local/bin/ls";       // where ls is the executable "ls"
        QDir lsDir = new QDir(ls);
        String canonicalLs = lsDir.canonicalPath();
        // canonicalLS now equals "/usr/bin/ls".
//! [6]


        if (true)
        {
//! [7]
        QDir dir = new QDir("/home/bob");
        String s;

        s = dir.relativeFilePath("images/file.jpg");     // s is "images/file.jpg"
        s = dir.relativeFilePath("/home/mary/file.txt"); // s is "../mary/file.txt"
//! [7]
        }


//! [8]
        List<String> imagesSearchPaths = new Vector<String>();
        imagesSearchPaths.add(QDir.homePath() + "/images");

        List<String> docSearchPaths = new Vector<String>();
        docSearchPaths.add(":/embeddedDocuments");

        QDir.setSearchPaths("icons", imagesSearchPaths);
        QDir.setSearchPaths("docs", docSearchPaths);
        //...
        QPixmap pixmap = new QPixmap("icons:undo.png"); // will look for undo.png in QDir.homePath() + "/images"
        QFile file = new QFile("docs:design.odf"); // will look in the :/embeddedDocuments resource path
//! [8]


        if (true)
        {
//! [9]
        QDir dir = new QDir("/tmp/root_link");
        String dirPath = dir.canonicalPath();
        if (new QDir(dirPath).isRoot())
            System.err.println("It is a root link");
//! [9]
        }

        if (true)
        {
//! [10]
        // The current directory is "/usr/local"
        QDir d1 = new QDir("/usr/local/bin");
        QDir d2 = new QDir("bin");
        if (d1.equals(d2))
            System.out.println("They're the same");
//! [10]
        }

        if (true)
        {
//! [11]
        // The current directory is "/usr/local"
        QDir d1 = new QDir("/usr/local/bin");
        QDir d2 = new QDir("bin");
        if (!d1.equals(d2))
            System.out.println("They differ");
//! [11]
        }

        /*
//! [12]
        C:/Documents and Settings/Username
//! [12]
*/

//! [13]
        // This snippet is not relevant for Jambi
        //Q_INIT_RESOURCE(myapp);
//! [13]


//! [14]
    // This is not Relevant for Jambi
    /*void initMyResource() { Q_INIT_RESOURCE(myapp); }

    namespace MyNamespace
    {
        ...

        void myFunction()
        {
            initMyResource();
        }
    }*/
//! [14]


//! [15]
       // Q_CLEANUP_RESOURCE(myapp);
//! [15]


    }
}
