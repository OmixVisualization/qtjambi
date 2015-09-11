import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_io_qdatastream {
    public static void main(String args[]) {
        QApplication.initialize(args);

        {
//! [0]
        QFile file = new QFile("file.dat");
        QFile.OpenMode mode = new QFile.OpenMode();
        mode.set(QFile.OpenModeFlag.WriteOnly);
        file.open(mode);
        QDataStream out = new QDataStream(file);   // we will serialize the data into the file
        out.writeString("the answer is");   // serialize a string
        out.writeInt(42);        // serialize an integer
//! [0]
        }


        {
//! [1]
        QFile file = new QFile("file.dat");
        QFile.OpenMode mode = new QFile.OpenMode();
        mode.set(QFile.OpenModeFlag.ReadOnly);
        file.open(mode);
        QDataStream in = new QDataStream(file);    // read the data serialized from the file
        // extract "the answer is" and 42
        String str = in.readString();
        int a = in.readInt();
//! [1]
        }

        {
        QDataStream stream = new QDataStream();
//! [2]
        stream.setVersion(QDataStream.Version.Qt_4_0.value());
//! [2]
        }


        QByteArray lots_of_interesting_data = new QByteArray();
        {
//! [3]
        QFile file = new QFile("file.xxx");
        QFile.OpenMode mode = new QFile.OpenMode();
        mode.set(QFile.OpenModeFlag.WriteOnly);
        file.open(mode);
        QDataStream out = new QDataStream(file);

        // Write a header with a "magic number" and a version
        out.writeInt(0xA0B0C0D0);
        out.writeInt(123);

        out.setVersion(QDataStream.Version.Qt_4_0.value());

        // Write the data
        out.writeBytes(lots_of_interesting_data.toByteArray());
//! [3]
        }
    }

    private int foo() {
        int XXX_BAD_FILE_FORMAT = 0;
        int XXX_BAD_FILE_TOO_OLD = 0;
        int XXX_BAD_FILE_TOO_NEW = 0;

        {
//! [4]
        QFile file = new QFile("file.xxx");
        QFile.OpenMode mode = new QFile.OpenMode();
        mode.set(QFile.OpenModeFlag.ReadOnly);
        file.open(mode);
        QDataStream in = new QDataStream(file);

        // Read and check the header
        int magic = in.readInt();
        if (magic != 0xA0B0C0D0)
            return XXX_BAD_FILE_FORMAT;

        // Read the version
        int version = in.readInt();
        if (version < 100)
            return XXX_BAD_FILE_TOO_OLD;
        if (version > 123)
            return XXX_BAD_FILE_TOO_NEW;

        if (version <= 110)
            in.setVersion(QDataStream.Version.Qt_3_1.value());
        else
            in.setVersion(QDataStream.Version.Qt_4_0.value());

        // Read the data
        QByteArray lots_of_interesting_data = new QByteArray();
        QByteArray data_new_in_XXX_version_1_2 = new QByteArray();
        QByteArray other_interesting_data = new QByteArray();

        lots_of_interesting_data.readFrom(in);
        if (version >= 120)
            data_new_in_XXX_version_1_2.readFrom(in);
        other_interesting_data.readFrom(in);
//! [4]


//! [5]
        QDataStream out = new QDataStream(file);
        out.setVersion(QDataStream.Version.Qt_4_0.value());
//! [5]
        }

        return 0;
    }
}
