import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;


public class file
{

    static void process_line(QByteArray ba)
    {
    }

    static void process_line(String s)
    {
    }

    static void noStream_snippet()
    {
    //! [0]
        QFile file = new QFile("in.txt");
        if (!file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly,
                                              QIODevice.OpenModeFlag.Text)))
            return;

        while (!file.atEnd()) {
            QByteArray line = file.readLine();
            process_line(line);
        }
    //! [0]
    }

    static void readTextStream_snippet()
    {
    //! [1]
        QFile file = new QFile("in.txt");
        if (!file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly,
                                              QIODevice.OpenModeFlag.Text)))
            return;

        QTextStream in = new QTextStream(file);
        while (!in.atEnd()) {
            String line = in.readLine();
            process_line(line);
        }
    //! [1]
    }

    static void writeTextStream_snippet()
    {
    //! [2]
        QFile file = new QFile("out.txt");
        if (!file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly,
                                              QIODevice.OpenModeFlag.Text)))
            return;

        QTextStream out = new QTextStream(file);
        out.writeString("The magic number is: " + 49 + "\n");
    //! [2]
    }

    static void writeTextStream_snippet2()
    {
        QFile file = new QFile("out.dat");
        if (!file.open(QIODevice.OpenModeFlag.WriteOnly))
            return;

        QDataStream out = new QDataStream(file);
        out.writeString("The magic number is: " + 49 + "\n");
    }

    static void readRegularEmptyFile_snippet()
    {
    //! [3]
        QFile file = new QFile("/proc/modules");
        if (!file.open(new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadOnly,
                                              QIODevice.OpenModeFlag.Text)))
            return;

        QTextStream in = new QTextStream(file);
        String line = in.readLine();
        while (line != null) {
            process_line(line);
            line = in.readLine();
        }
    //! [3]
    }

    public static void main(String args[])
    {
        //lineByLine_snippet();
        //writeStream_snippet();
    }
}
