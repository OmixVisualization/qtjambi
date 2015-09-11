import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_io_qtextstream {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QFile data = new QFile("output.txt");
        if (data.open(QIODevice.OpenModeFlag.WriteOnly, QIODevice.OpenModeFlag.Truncate)) {
            QTextStream out = new QTextStream(data);
            out.writeString("Result: ");
            out.setFieldWidth(10);
            out.setFieldAlignment(QTextStream.FieldAlignment.AlignLeft);
            out.writeDouble(3.14).writeDouble(2.7).writeString("\n");
            // writes "Result: 3.14      2.7       \n"
        }
//! [0]

/*
//! [1]
        QTextStream stream(stdin);
        Stringsline;
        do {
            line = stream.readLine();
        } while (!line.isNull());
//! [1]
*/

//! [2]
        QTextStream in = new QTextStream(new QByteArray("0x50 0x20"));
        int firstNumber, secondNumber;

        firstNumber = in.readInt();     // firstNumber == 80
        in.setIntegerBase(10);
        secondNumber = in.readInt();    // secondNumber == 0

        String str;
        str = in.readString();          // str == "x20"
//! [2]


    class bla {
//! [3]
        public void processArguments(String[] args)
        {
            // read numeric arguments (123, 0x20, 4.5...)
            for (int i = 1; i < args.length; ++i) {
                  int number;
                  QTextStream in = new QTextStream(new QByteArray(args[i]));
                  number = in.readInt();
                  //...
            }
        }
//! [3]
    } //class

/*
//! [4]
        QString str;
        QTextStream in(stdin);
        in >> str;
//! [4]
*/

{
//! [5]
        QByteArray b = new QByteArray();
        QTextStream out = new QTextStream(b);
        out.setFieldWidth(10);
        out.setPadChar('-');
        out.writeString("Qt");
        out.writeString("\n");
        out.writeString("rocks!");
        out.writeString("\n");
//! [5]
}

/*
//! [6]
        ----Qt----
        --rocks!--
//! [6]
*/

/*
//! [7]
        QTextStream in(file);
        QChar ch1, ch2, ch3;
        in >> ch1 >> ch2 >> ch3;
//! [7]
*/
/*
//! [8]
        QTextStream out(stdout);
        out << "Qt rocks!" << endl;
//! [8]
*/

        QTextStream stream = new QTextStream();
//! [9]
        stream.writeString("\n");
        stream.flush();
//! [9]

{
        QFile file = new QFile();
//! [10]
        QTextStream out = new QTextStream(file);
        out.setCodec("UTF-8");
//! [10]
}

    }
}
