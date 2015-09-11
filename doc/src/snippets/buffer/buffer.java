import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class buffer
{

static void main_snippet()
{
//! [0]
    QBuffer buffer = new QBuffer();
    int b;

    buffer.open(QIODevice.OpenModeFlag.ReadWrite);
    buffer.write(new QByteArray("Qt rocks!"));
    buffer.seek(0);
    b = buffer.getByte();  // b == 'Q'
    b = buffer.getByte();  // b == 't'
    b = buffer.getByte();  // b == ' '
    b = buffer.getByte();  // b == 'r'
//! [0]
}

static void write_datastream_snippets()
{
//! [1]
    QByteArray byteArray = new QByteArray();
    QBuffer buffer = new QBuffer(byteArray);
    buffer.open(QIODevice.OpenModeFlag.WriteOnly);

    QDataStream out = new QDataStream(buffer);
    out.writeString("Any string will do.");
//! [1]
}

static void read_datastream_snippets()
{
    QByteArray byteArray = new QByteArray();

//! [2]
    String string = "";
    QBuffer buffer = new QBuffer(byteArray);
    buffer.open(QIODevice.OpenModeFlag.ReadOnly);

    QDataStream in = new QDataStream(buffer);
    string = in.readString();
//! [2]
}

static void bytearray_ptr_ctor_snippet()
{
//! [3]
    QByteArray byteArray = new QByteArray("abc");
    QBuffer buffer = new QBuffer(byteArray);
    buffer.open(QIODevice.OpenModeFlag.WriteOnly);
    buffer.seek(3);
    buffer.write(new QByteArray("def"));
    buffer.close();
    // byteArray == "abcdef"
//! [3]
}

static void setBuffer_snippet()
{
//! [4]
    QByteArray byteArray = new QByteArray("abc");
    QBuffer buffer = new QBuffer();
    buffer.setBuffer(byteArray);
    buffer.open(QIODevice.OpenModeFlag.WriteOnly);
    buffer.seek(3);
    buffer.write(new QByteArray("def"));
    buffer.close();
    // byteArray == "abcdef"
//! [4]
}

public static void main(String args[])
{
    QApplication.initialize(args);

    main_snippet();
    bytearray_ptr_ctor_snippet();
    write_datastream_snippets();
    read_datastream_snippets();
    setBuffer_snippet();

    QApplication.execStatic();
    QApplication.shutdown();
}

}
