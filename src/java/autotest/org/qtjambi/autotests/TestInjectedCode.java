/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;

import org.junit.Test;

import org.qtjambi.autotests.generated.IODeviceSubclass;
//import org.qtjambi.autotests.generated.TextCodecSubclass;
import org.qtjambi.autotests.generated.PictureSubclass;
import org.qtjambi.autotests.generated.GraphicsItemSubclass;
import org.qtjambi.autotests.generated.AccessibleInterfaceSubclass;
import org.qtjambi.autotests.generated.SomeQObject;
import org.qtjambi.autotests.generated.TextCodecSubclass;
/*import org.qtjambi.autotests.generated.Relation;
import org.qtjambi.autotests.generated.Role;
import org.qtjambi.autotests.generated.State;*/
import org.qtjambi.autotests.generated.ValidatorSubclass;
import org.qtjambi.autotests.generated.ImageIOHandlerSubclass;
import org.qtjambi.autotests.generated.XmlReaderSubclass;
//import org.qtjambi.autotests.generated.AccessibleTableInterfaceSubclass;
import org.qtjambi.qt.QNativePointer;
import org.qtjambi.qt.core.QAbstractFileEngine;
import org.qtjambi.qt.core.QBuffer;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDataStream;
import org.qtjambi.qt.core.QDate;
import org.qtjambi.qt.core.QFile;
import org.qtjambi.qt.core.QIODevice;
import org.qtjambi.qt.core.QLocale;
import org.qtjambi.qt.core.QMimeData;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QTemporaryFile;
import org.qtjambi.qt.core.QTextCodec;
import org.qtjambi.qt.core.QTextCodec.ConverterState;
import org.qtjambi.qt.core.QUrl;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QAccessible;
import org.qtjambi.qt.gui.QAccessible.RelationFlag;
import org.qtjambi.qt.gui.QAccessibleInterface;
import org.qtjambi.qt.widgets.QAction;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.gui.QBitmap;
import org.qtjambi.qt.gui.QBrush;
import org.qtjambi.qt.gui.QClipboard;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QTableArea;
import org.qtjambi.qt.core.QMargins;
import org.qtjambi.qt.gui.QCursor;
import org.qtjambi.qt.gui.QFontMetrics;
import org.qtjambi.qt.gui.QGradient;
import org.qtjambi.qt.widgets.QGridLayout;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.gui.QKeySequence;
import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.gui.QLinearGradient;
import org.qtjambi.qt.widgets.QMenu;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPalette;
import org.qtjambi.qt.gui.QPicture;
import org.qtjambi.qt.widgets.QPictureIO;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.gui.QPixmapCache;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.gui.QRadialGradient;
import org.qtjambi.qt.gui.QRegion;
import org.qtjambi.qt.widgets.QShortcut;
import org.qtjambi.qt.widgets.QStyleOptionButton;
import org.qtjambi.qt.widgets.QStyleOptionGraphicsItem;
import org.qtjambi.qt.gui.QTextCursor;
import org.qtjambi.qt.gui.QTextDocument;
import org.qtjambi.qt.gui.QValidator;
import org.qtjambi.qt.widgets.QWidget;
import org.qtjambi.qt.network.QHostAddress;
import org.qtjambi.qt.network.QHostAddress.SpecialAddress;
import org.qtjambi.qt.network.QTcpServer;
import org.qtjambi.qt.xml.QDomDocument;
import org.qtjambi.qt.xml.QDomElement;
import org.qtjambi.qt.xml.QXmlContentHandlerInterface;
import org.qtjambi.qt.xml.QXmlDTDHandlerInterface;
import org.qtjambi.qt.xml.QXmlDeclHandlerInterface;
import org.qtjambi.qt.xml.QXmlEntityResolverInterface;
import org.qtjambi.qt.xml.QXmlErrorHandlerInterface;
import org.qtjambi.qt.xml.QXmlInputSource;
import org.qtjambi.qt.xml.QXmlLexicalHandlerInterface;

public class TestInjectedCode extends QApplicationTest {
    static class IODeviceSubclassSubclass extends IODeviceSubclass {
        public byte buffer[];
        public int inputBufferSize;

        public IODeviceSubclassSubclass(int bufferLength) {
            super(bufferLength);
        }

        @Override
        protected int readData(byte[] data) {
            inputBufferSize = data.length;

            int size = super.readData(data);
            buffer = new byte[size];
            for (int i=0; i<size; ++i)
                buffer[i] = data[i];

            return size;
        }

        @Override
        protected int writeData(byte[] data) {
            inputBufferSize = data.length;
            buffer = new byte[data.length];

            for (int i=0; i<data.length; ++i)
                buffer[i] = data[i];

            return super.writeData(data);
        }

        @Override
        protected int readLineData(byte[] data) {
            inputBufferSize = data.length;

            int size = super.readLineData(data);
            buffer = new byte[size];
            for (int i=0; i<size; ++i)
                buffer[i] = data[i];

            return size;
        }
    }

    static class TextCodecSubclassSubclass extends TextCodecSubclass {
        char receivedChar[];
        byte receivedByte[];
        QTextCodec.ConverterState receivedState;

        @Override
        protected QByteArray convertFromUnicode(char[] data, int size, QTextCodec.ConverterState state) {
            receivedChar = data;
            receivedState = state;
            return super.convertFromUnicode(data, size, state);
        }

        @Override
        protected String convertToUnicode(byte[] data, int size, QTextCodec.ConverterState state) {
            receivedByte = data;
            receivedState = state;
            return super.convertToUnicode(data, size, state);
        }

        @Override
        public int mibEnum() {
            return 0;
        }

        @Override
        public QByteArray name() {
            return null;
        }
    }

    static class OtherIODeviceSubclass extends QIODevice {
        public byte[] bytes = {(byte) 'h', (byte) 'a', (byte) 'l' };
        public int i = 0;
        public QByteArray ba = new QByteArray();

        @Override
        protected int readData(byte[] data) {
            if (i == bytes.length) return -1;
            data[0] = bytes[i++];
            return 1;
        }

        @Override
        protected int writeData(byte[] data) {
            if (data.length == 0) return -1;
            ba.append(data[0]);
            return 1;
        }
    }

    static class PictureSubclassSubclass extends PictureSubclass {
        public byte[] data;

        @Override
        public void setData(byte[] data) {
            this.data = data;
            super.setData(data);
        }

    }

    static class GraphicsItemSubclassSubclass extends GraphicsItemSubclass {

        public QPainter painter;
        public QStyleOptionGraphicsItem option;
        public QWidget widget;

        @Override
        public QRectF boundingRect() {
            return null;
        }

        @Override
        public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
            this.painter = painter;
            this.option = option;
            this.widget = widget;
            painter.fillRect(new QRect(51, 0, 50, 50), new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.green)));
            super.paint(painter, option, widget);
        }
    }

/*    static class AccessibleInterfaceSubclassSubclass extends AccessibleInterfaceSubclass {
        public RelationFlag relation;
        public Target target;
        @Override
        public Target navigate(RelationFlag relation, int entry) {
            this.relation  = relation;
            target = super.navigate(relation, entry);
            return target;
        }

        @Override
        public String actionText(int action, Text t, int child) {
            return null;
        }

        @Override
        public int childAt(int x, int y) {
            return 0;
        }

        @Override
        public int childCount() {
            return 0;
        }

        @Override
        public boolean doAction(int action, int child, List<Object> params) {
            return false;
        }

        @Override
        public int indexOfChild(QAccessibleInterface arg__1) {
            return 0;
        }

        @Override
        public boolean isValid() {
            return false;
        }

        @Override
        public QObject object() {
            return null;
        }

        @Override
        public QRect rect(int child) {
            return null;
        }

        @Override
        public Relation relationTo(int child, QAccessibleInterface other, int otherChild) {
            return null;
        }

        @Override
        public Role role(int child) {
            return null;
        }

        @Override
        public void setText(Text t, int child, String text) {

        }

        @Override
        public State state(int child) {
            return null;
        }

        @Override
        public String text(Text t, int child) {
            return null;
        }

        @Override
        public int userActionCount(int child) {
            return 0;
        }

    }
    */

    static class AnyClass extends QWidget {
        public QUrl url;

        private void myUrlHandler(QUrl url) {
            this.url = url;
        }

        public void actionTriggered() {
            myActionTriggered.emit();
        }

        public void somethingElse() {
            // nothing here
            System.out.println("here");
        }

        public Signal0 myActionTriggered = new Signal0();
    }

    static class ValidatorSubclassSubclass extends ValidatorSubclass {

        public String inputString;
        public int inputPos;

        @Override
        public String fixup(String input) {
            inputString = input;
            return super.fixup(input) + "javaPostfix";
        }

        @Override
        public QValidator.State validate(QValidator.QValidationData input) {
            inputString = input.string;
            inputPos = input.position;
            QValidator.State state = super.validate(input);
            input.string = "javaPrefix" + input.string;

            return state;
        }

    }

    static class ImageIOHandlerSubclassSubclass extends ImageIOHandlerSubclass {
        public QImage image;

        @Override
        public boolean read(QImage image) {
            this.image = new QImage();

            super.read(null); // don't crash
            boolean result = super.read(this.image);
            return result && (image != null ? image.load("classpath:org/qtjambi/examples/images/bg1.png") : true);
        }

        @Override
        public boolean canRead() {
            return false;
        }

    }

    static class XmlReaderSubclassSubclass extends XmlReaderSubclass {

        @Override
        public boolean feature(String name) {

            return super.feature(name) || name.equals("javaTrue");
        }

        @Override
        public QXmlContentHandlerInterface contentHandler() {
            return null;
        }

        @Override
        public QXmlDeclHandlerInterface declHandler() {
            return null;
        }

        @Override
        public QXmlDTDHandlerInterface DTDHandler() {
            return null;
        }

        @Override
        public QXmlEntityResolverInterface entityResolver() {
            return null;
        }

        @Override
        public QXmlErrorHandlerInterface errorHandler() {
            return null;
        }

        @Override
        public boolean hasFeature(String name) {
            return name.equals("javaTrue");
        }

        @Override
        public boolean hasProperty(String name) {
            return false;
        }

        @Override
        public QXmlLexicalHandlerInterface lexicalHandler() {
            return null;
        }

        @Override
        public boolean parse(QXmlInputSource input) {
            return false;
        }

        @Override
        public void setContentHandler(QXmlContentHandlerInterface handler) {

        }

        @Override
        public void setDeclHandler(QXmlDeclHandlerInterface handler) {

        }

        @Override
        public void setDTDHandler(QXmlDTDHandlerInterface handler) {

        }

        @Override
        public void setEntityResolver(QXmlEntityResolverInterface handler) {

        }

        @Override
        public void setErrorHandler(QXmlErrorHandlerInterface handler) {

        }

        @Override
        public void setFeature(String name, boolean value) {

        }

        @Override
        public void setLexicalHandler(QXmlLexicalHandlerInterface handler) {

        }

    }

    private static QTemporaryFile setUpPicture() {
        QTemporaryFile file = new QTemporaryFile();

        assertTrue(file.open(QIODevice.OpenModeFlag.WriteOnly));

        {
            QPicture picture = new QPicture();
            QPainter painter = new QPainter();
            painter.begin(picture);
            painter.fillRect(new QRect(0, 0, 10, 10), new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.green)));
            painter.fillRect(new QRect(11, 0, 10, 10), new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red)));
            painter.end();

            assertTrue(picture.save(file));
        }
        file.close();

        return file;
    }

    private static void verifyQPicture(QPicture picture) {
        QImage img = new QImage(100, 100, QImage.Format.Format_ARGB32_Premultiplied);
        QPainter painter = new QPainter();
        painter.begin(img);
        painter.drawPicture(new QPoint(0, 0), picture);
        painter.end();

        assertEquals(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.green).rgba(), img.pixel(4, 4));
        assertEquals(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red).rgba(), img.pixel(12, 4));
    }

    /*
    private static class MyAccessibleTableInterface extends AccessibleTableInterfaceSubclass
    {


        @Override
        public QAccessibleInterface accessibleAt(int row, int column) {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public QAccessibleInterface caption() {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public int childIndex(int rowIndex, int columnIndex) {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public int columnCount() {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public String columnDescription(int column) {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public QAccessibleInterface columnHeader() {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public int columnIndex(int childIndex) {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public int columnSpan(int row, int column) {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public boolean isColumnSelected(int column) {
            // TODO Auto-generated method stub
            return false;
        }

        @Override
        public boolean isRowSelected(int row) {
            // TODO Auto-generated method stub
            return false;
        }

        @Override
        public boolean isSelected(int row, int column) {
            // TODO Auto-generated method stub
            return false;
        }

        @Override
        public int rowCount() {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public String rowDescription(int row) {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public QAccessibleInterface rowHeader() {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public int rowIndex(int childIndex) {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public int rowSpan(int row, int column) {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public void selectColumn(int column) {
            // TODO Auto-generated method stub

        }

        @Override
        public void selectRow(int row) {
            // TODO Auto-generated method stub

        }

        @Override
        public int selectedColumnCount() {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public int selectedColumns(int maxColumns, List<Integer> columns) {
            columns.add(maxColumns);
            return super.selectedColumns(maxColumns + 2, columns);
        }

        @Override
        public int selectedRowCount() {
            // TODO Auto-generated method stub
            return 0;
        }

        @Override
        public int selectedRows(int maxRows, List<Integer> rows) {
            rows.add(maxRows);
            return super.selectedRows(maxRows - 2, rows);
        }

        @Override
        public QAccessibleInterface summary() {
            // TODO Auto-generated method stub
            return null;
        }

        @Override
        public void unselectColumn(int column) {
            // TODO Auto-generated method stub

        }

        @Override
        public void unselectRow(int row) {

        }

        @Override
        public CellAtIndex cellAtIndex(int index) {
            return super.cellAtIndex(index + 1);
        }

    }

    @Test
    public void testQAccessibleSelectedColumns()
    {
        MyAccessibleTableInterface m = new MyAccessibleTableInterface();

        List<Integer> ints = new ArrayList<Integer>();
        ints.add(43);

        ints = AccessibleTableInterfaceSubclass.callSelectedColumns(m, 55, ints);

        assertEquals(3, ints.size());
        assertEquals((Integer)43, ints.get(0));
        assertEquals((Integer)55, ints.get(1));
        assertEquals((Integer)57, ints.get(2));
    }
    */

/*    @Test
    public void testQAccessibleSelectedRows()
    {
        MyAccessibleTableInterface m = new MyAccessibleTableInterface();

        List<Integer> ints = new ArrayList<Integer>();
        ints.add(43);

        ints = AccessibleTableInterfaceSubclass.callSelectedRows(m, 55, ints);

        assertEquals(3, ints.size());
        assertEquals((Integer)43, ints.get(0));
        assertEquals((Integer)55, ints.get(1));
        assertEquals((Integer)53, ints.get(2));
    }

    @Test
    public void testQAccessibleCellAtIndexIsSelectedFalse()
    {
        MyAccessibleTableInterface m = new MyAccessibleTableInterface();

        QNativePointer row = new QNativePointer(QNativePointer.Type.Int);
        QNativePointer col = new QNativePointer(QNativePointer.Type.Int);
        QNativePointer rowSpan = new QNativePointer(QNativePointer.Type.Int);
        QNativePointer columnSpan = new QNativePointer(QNativePointer.Type.Int);
        QNativePointer isSelected = new QNativePointer(QNativePointer.Type.Boolean);

        AccessibleTableInterfaceSubclass.callCellAtIndex(m, 10, row, col, rowSpan, columnSpan, isSelected);

        assertEquals(12, row.intValue());
        assertEquals(13, col.intValue());
        assertEquals(14, rowSpan.intValue());
        assertEquals(15, columnSpan.intValue());
        assertEquals(false, isSelected.booleanValue());
    }

    @Test
    public void testQAccessibleCellAtIndexIsSelectedTrue()
    {
        MyAccessibleTableInterface m = new MyAccessibleTableInterface();

        QNativePointer row = new QNativePointer(QNativePointer.Type.Int);
        QNativePointer col = new QNativePointer(QNativePointer.Type.Int);
        QNativePointer rowSpan = new QNativePointer(QNativePointer.Type.Int);
        QNativePointer columnSpan = new QNativePointer(QNativePointer.Type.Int);
        QNativePointer isSelected = new QNativePointer(QNativePointer.Type.Boolean);

        AccessibleTableInterfaceSubclass.callCellAtIndex(m, 21, row, col, rowSpan, columnSpan, isSelected);

        assertEquals(23, row.intValue());
        assertEquals(24, col.intValue());
        assertEquals(25, rowSpan.intValue());
        assertEquals(26, columnSpan.intValue());
        assertEquals(true, isSelected.booleanValue());
    }

    @Test
    public void testQAccessibleCellAtIndexDontCrashOnNull()
    {
        MyAccessibleTableInterface m = new MyAccessibleTableInterface();
        AccessibleTableInterfaceSubclass.callCellAtIndex(m, 10, null, null, null, null, null);
    }
*/

    @Test
    public void testQXmlReaderFeature() {
        XmlReaderSubclassSubclass xrss = new XmlReaderSubclassSubclass();

        assertFalse(xrss.callFeature("javaFalse"));
        assertEquals("javaFalse", xrss.myName());
        assertFalse(xrss.myOk());

        assertTrue(xrss.callFeature("javaTrue"));
        assertEquals("javaTrue", xrss.myName());
        assertTrue(xrss.myOk());

        assertTrue(xrss.callFeature("true"));
        assertEquals("true", xrss.myName());
        assertFalse(xrss.myOk());
    }

    @Test
    public void testQDomElementAttributeNS() {
        QDomDocument document = new QDomDocument();
        QDomElement element = document.createElement("tag");
        assertTrue(element!=null);
        element.setAttributeNS("something", "foo", "bar");
        assertEquals("bar", element.attributeNS("something", "foo"));
    }

    @Test
    public void testQTcpServerWaitForConnection() {
        QTcpServer server = new QTcpServer();

        QHostAddress address = new QHostAddress(SpecialAddress.Any);

        assertTrue(server.listen(address, 0));

        QTcpServer.Result result = server.waitForNewConnection(100);

        assertEquals(QTcpServer.Result.TimedOut, result);

        // QTcpServer seems to be pinned/leaked ?
    }

    @Test
    public void testQTextDocumentUndoRedo() {
        QTextDocument textDocument = new QTextDocument();
        textDocument.setPlainText("i have plain text");

        QTextCursor cursor = new QTextCursor(textDocument);
        cursor.movePosition(QTextCursor.MoveOperation.Start);

        assertTrue(cursor.atStart());
        cursor.insertText("A");
        assertEquals("Ai have plain text", textDocument.toPlainText());
        assertFalse(cursor.atStart());

        textDocument.undo(cursor);
        assertEquals("i have plain text", textDocument.toPlainText());
        assertTrue(cursor.atStart());

        textDocument.redo(cursor);
        assertEquals("Ai have plain text", textDocument.toPlainText());
        assertFalse(cursor.atStart());
    }


    @Test
    public void testQClipboardTextSpecificSubtype() {
        QMimeData data = new QMimeData();
        data.setHtml("some text"); // text/html

        QClipboard clipboard = QApplication.clipboard();

        clipboard.setText("text 1 2 3");

        clipboard.clear();

        QClipboard.Text text = clipboard.text("html");
        assertEquals("html", text.subtype);
        assertEquals("", text.text);

        text = clipboard.text("plain");
        assertEquals("plain", text.subtype);
        // FIXME: This is probably due to the QClipboard bug
        //assertEquals("", text.text); // unstable when doing something on the clipboard while the test is running

        clipboard.setMimeData(data);

        text = clipboard.text("plain");
        assertEquals("plain", text.subtype);
        assertEquals("", text.text);

        text = clipboard.text("html");
        assertEquals("html", text.subtype);
        assertEquals("some text", text.text);

        data = new QMimeData();
        data.setText("some plain text");
        clipboard.setMimeData(data);

        text = clipboard.text("html");
        assertEquals("html", text.subtype);
        assertEquals("", text.text);

        text = clipboard.text("plain");
        assertEquals("plain", text.subtype);
        assertEquals("some plain text", text.text);
    }

    @Test
    public void testQClipboardTextAnySubtype() {
        QMimeData data = new QMimeData();
        data.setHtml("some text"); // text/html

        QClipboard clipboard = QApplication.clipboard();

        clipboard.setText("text 1 2 3");

        clipboard.clear();

        QClipboard.Text text = clipboard.text("");
        //assertEquals("", text.subtype); // If subtype is null, any subtype is acceptable, and subtype is set to the chosen subtype.
        assertEquals("", text.text);

        text = clipboard.text((String) null);
        assertEquals("", text.text);

        clipboard.setMimeData(data);

        text = clipboard.text("");
        assertEquals("html", text.subtype);
        assertEquals("some text", text.text);

        data = new QMimeData();
        data.setText("some plain text");
        clipboard.setMimeData(data);

        text = clipboard.text((String) null);
        assertEquals("plain", text.subtype);
        assertEquals("some plain text", text.text);
    }

    @Test
    public void testQImageIOHandlerRead() {
        QImage image = new QImage();
        ImageIOHandlerSubclassSubclass iihss = new ImageIOHandlerSubclassSubclass();
        
        assertTrue(iihss.callRead(image.nativePointer()));
        
        QImage ref1 = new QImage("classpath:org/qtjambi/examples/images/cheese.png");
        QImage ref2 = new QImage("classpath:org/qtjambi/examples/images/bg1.png");

        assertEquals(ref2.width(), image.width());
        assertEquals(ref2.height(), image.height());
        assertTrue(ref2.equals(image));

        assertEquals(ref1.width(), iihss.image.width());
        assertEquals(ref1.height(), iihss.image.height());
        assertTrue(ref1.equals(iihss.image));

    }

    @Test
    public void testQWidgetGetContentsMargins() {
        QWidget w = new QWidget();

        w.setContentsMargins(10, 11, 12, 13);

        QMargins cm = w.contentsMargins();
        assertTrue(cm!=null);
        assertEquals(cm.left(), 10);
        assertEquals(cm.top(), 11);
        assertEquals(cm.right(), 12);
        assertEquals(cm.bottom(), 13);
    }

    @Test
    public void testQGridLayoutGetItemPosition() {
        QGridLayout layout = new QGridLayout();

        QWidget w1 = new QWidget();
        QWidget w2 = new QWidget();

        layout.addWidget(w1, 2, 2);
        layout.addWidget(w2, 3, 4, 5, 6);

        {
            QTableArea ip = layout.getItemPosition(0);
            assertEquals(2, ip.row);
            assertEquals(2, ip.column);
            assertEquals(1, ip.rowCount);
            assertEquals(1, ip.columnCount);
        }

        {
            QTableArea ip = layout.getItemPosition(1);
            assertEquals(3, ip.row);
            assertEquals(4, ip.column);
            assertEquals(5, ip.rowCount);
            assertEquals(6, ip.columnCount);
        }
    }

    @Test
    public void testValidatorFixup() {
        ValidatorSubclassSubclass vss = new ValidatorSubclassSubclass();

        QNativePointer input = new QNativePointer(QNativePointer.Type.String);
        input.setStringValue("acceptable");
        QNativePointer pos = new QNativePointer(QNativePointer.Type.Int);
        pos.setIntValue(13);
        assertEquals(QValidator.State.Acceptable, vss.callValidate(input, pos));
        assertEquals("javaPrefixacceptablesomePostfix", input.stringValue());
        assertEquals("acceptable".length(), pos.intValue());
        assertEquals("acceptable", vss.inputString);
        assertEquals(13, vss.inputPos);
        assertEquals("acceptable", vss.inputString());
        assertEquals(13, vss.inputPos());

        input.setStringValue("intermediate");
        pos.setIntValue(14);
        assertEquals(QValidator.State.Intermediate, vss.callValidate(input, pos));
        assertEquals("javaPrefixintermediatesomePostfix", input.stringValue());
        assertEquals("intermediate".length(), pos.intValue());
        assertEquals("intermediate", vss.inputString);
        assertEquals(14, vss.inputPos);
        assertEquals("intermediate", vss.inputString());
        assertEquals(14, vss.inputPos());

    }

    @Test
    public void testQShortcutConstructor() {
        QShortcut shortcut = new QShortcut(new QKeySequence("Ctrl+F1"), new QWidget());
        assertEquals("Ctrl+F1", shortcut.key().toString());
        assertEquals(Qt.ShortcutContext.WindowShortcut, shortcut.context());

        shortcut = new QShortcut(new QKeySequence("Ctrl+F2"), new QWidget(), Qt.ShortcutContext.ApplicationShortcut);
        assertEquals("Ctrl+F2", shortcut.key().toString());
        assertEquals(Qt.ShortcutContext.ApplicationShortcut, shortcut.context());

    }

    @Test
    public void testQPixmapCacheFind() {
        {
            QPixmap pm = new QPixmap("classpath:org/qtjambi/examples/images/cheese.png");
            QPixmapCache.insert("myPixmap", pm);
        }

        {
        	//FIX: Warning: QPixmap::operator=: Cannot assign to pixmap during painting
            QPixmap pm = QPixmapCache.find("myPixmap");
            assertTrue(pm!=null);
            assertEquals(94, pm.width());
            
            pm = QPixmapCache.find("noSuchPixmap");
            assertFalse(pm!=null);
        }
    }

    @Test
    public void testQPictureParameters() {
        QPictureIO pictureIO = new QPictureIO("someFile", "PNG");

        pictureIO.setParameters("my parameters");
        assertEquals("my parameters", pictureIO.parameters());
    }

    @Test
    public void testQPictureConstructor() {
        QPictureIO pictureIO = new QPictureIO(new QTemporaryFile(), "JPEG");
        assertEquals("JPEG", pictureIO.format());
        assertEquals("", pictureIO.fileName());

        pictureIO = new QPictureIO("someFile", "PNG");
        assertEquals("PNG", pictureIO.format());
        assertEquals("someFile", pictureIO.fileName());
    }

    static boolean called = false;
    void myReceiver() {
        called = true;
    }

    @Test
    public void testQMenuAddActionJavaSignal() {
		AnyClass ac = new AnyClass();
		ac.myActionTriggered.connect(this, "myReceiver()");

		QMenu menu = new QMenu();
		QAction action = menu.addAction(null, "blah", ac.myActionTriggered);

		called = false;
		action.activate(QAction.ActionEvent.Trigger);
		assertTrue(called);
    }

    @Test
    public void testQMenuAddActionJavaSlot() {
        AnyClass ac = new AnyClass();
        ac.myActionTriggered.connect(this, "myReceiver()");

        QMenu menu = new QMenu();
        QAction action = menu.addAction(null, "blah", ac, "actionTriggered()");

        called = false;
        action.activate(QAction.ActionEvent.Trigger);
        assertTrue(called);
    }

    @Test
    public void testQMenuAddActionCpp() {
        SomeQObject sqo = new SomeQObject();
        QMenu menu = new QMenu();
        QAction action = menu.addAction(null, "blah", sqo, "actionTriggered()");
        sqo.myActionTriggered.connect(this, "myReceiver()");

        called = false;
        action.activate(QAction.ActionEvent.Trigger);
        assertTrue(called);
    }

    static class PushButtonAccessor extends QPushButton {
        public void callInitStyleOption(QStyleOptionButton option) {
            initStyleOption(option);
        }
    }

    @Test
    public void testInitStyleOption() {
        QStyleOptionButton option = new QStyleOptionButton();
        PushButtonAccessor accessor = new PushButtonAccessor();

        accessor.setFlat(true);
        accessor.callInitStyleOption(option);
        assertTrue(option.features().isSet(QStyleOptionButton.ButtonFeature.Flat));

        accessor.setFlat(false);
        accessor.callInitStyleOption(option);
        assertFalse(option.features().isSet(QStyleOptionButton.ButtonFeature.Flat));
    }

/*    @Test
    public void testQAccessibleInterfaceNavigate() {
        AccessibleInterfaceSubclassSubclass aiss = new AccessibleInterfaceSubclassSubclass();

        int i = aiss.callNavigate(QAccessible.RelationFlag.Self, 33);
        assertEquals(QAccessible.RelationFlag.Self, aiss.relation);
        assertEquals(33, i);
        assertEquals(33, aiss.target.childIndex);
        assertTrue(aiss == aiss.target());
        assertTrue(aiss == aiss.target.target);

        i = aiss.callNavigate(QAccessible.RelationFlag.Ancestor, 103);
        assertEquals(-1, i);
        assertEquals(-1, aiss.target.childIndex);
        assertTrue(null == aiss.target());
        assertTrue(null == aiss.target.target);

    }*/

    @Test
    public void testQGraphicsItemPaint() {
        GraphicsItemSubclassSubclass giss = new GraphicsItemSubclassSubclass();

        QImage image = new QImage(100, 100, QImage.Format.Format_ARGB32_Premultiplied);
        QPainter painter = new QPainter();
        painter.begin(image);

        QStyleOptionGraphicsItem item = new QStyleOptionGraphicsItem();
        item.setExposedRect(new QRectF(0, 1, 2, 3));
        QWidget w = new QPushButton();

        giss.callPaint(painter, item, w);
        painter.end();

        assertEquals(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red).rgba(), image.pixel(2, 2));
        assertEquals(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.green).rgba(), image.pixel(52, 2));
        assertTrue(w == giss.widget);
        assertTrue(w == giss.widget());
        assertTrue(giss.widget instanceof QPushButton);
        assertTrue(giss.widget() instanceof QPushButton);
        assertTrue(painter == giss.painter);
        assertTrue(painter == giss.painter());
        assertEquals(0.0, giss.option.exposedRect().x(), 0.0);
        assertEquals(1.0, giss.option.exposedRect().y(), 0.0);
        assertEquals(2.0, giss.option.exposedRect().width(), 0.0);
        assertEquals(3.0, giss.option.exposedRect().height(), 0.0);
        assertEquals(0.0, giss.option().exposedRect().x(), 0.0);
        assertEquals(1.0, giss.option().exposedRect().y(), 0.0);
        assertEquals(2.0, giss.option().exposedRect().width(), 0.0);
        assertEquals(3.0, giss.option().exposedRect().height(), 0.0);
    }

    @Test
    public void testQFontMetricsBoundingRect() {
        QFontMetrics metrics = new QFontMetrics(QApplication.font());

        QRect wideRect = new QRect(0, 0, 1000, 1000);
        QRect brect = metrics.boundingRect(wideRect.x(), wideRect.y(), wideRect.width(), wideRect.height(), Qt.AlignmentFlag.AlignLeft.value(), "Some text");
        assertTrue(brect.isValid());
        assertTrue(wideRect.contains(brect));

        int[] tabArray = new int[5];
        for (int i=0; i<tabArray.length; ++i)
            tabArray[i] = i + 1;
        brect = metrics.boundingRect(wideRect.x(), wideRect.y(), wideRect.width(), wideRect.height(),
                Qt.AlignmentFlag.AlignLeft.value() + Qt.TextFlag.TextExpandTabs.value(), "Some text", 0,
                tabArray);
        assertTrue(brect.isValid());
        assertTrue(wideRect.contains(brect));
    }

    @Test
    public void testQBrushGradient() {
        QBrush brush = new QBrush(new QColor(org.qtjambi.qt.core.Qt.GlobalColor.red));
        assertEquals(null, brush.gradient());

        QGradient gradient = new QLinearGradient(0, 0, 11, 12);
        brush = new QBrush(gradient);

        assertTrue(brush.gradient() != null);
        assertEquals(QGradient.Type.LinearGradient, brush.gradient().type());
        assertTrue(brush.gradient() instanceof QLinearGradient);
        QLinearGradient lg = (QLinearGradient) brush.gradient();
        assertEquals(0.0, lg.start().x(), 0.0);
        assertEquals(0.0, lg.start().y(), 0.0);
        assertEquals(11.0, lg.finalStop().x(), 0.0);
        assertEquals(12.0, lg.finalStop().y(), 0.0);

        gradient = new QRadialGradient(0, 0, 1);
        brush = new QBrush(gradient);

        assertTrue(brush.gradient() != null);
        assertEquals(QGradient.Type.RadialGradient, brush.gradient().type());
        assertTrue(brush.gradient() instanceof QRadialGradient);

    }

    @Test
    public void testQImageLoadFromData() {
        QImage img = new QImage();
        QFile file = new QFile("classpath:org/qtjambi/examples/images/cheese.png");

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));

        img.load(file, "JPEG");
        assertTrue(img.isNull());  // this assert fails when plugins did not load

        img = new QImage();
        file.reset();
        img.load(file);
        assertFalse(img.isNull());
        assertEquals(94, img.width());

        img = new QImage();
        file.reset();
        img.loadFromData(file.readAll().toByteArray(), "JPEG");
        assertTrue(img.isNull());

        img = new QImage();
        file.reset();
        img.loadFromData(file.readAll().toByteArray());
        assertFalse(img.isNull());
        assertEquals(94, img.width());
    }

    @Test
    public void testQCursorBitmap() {
        QCursor cursor = new QCursor(Qt.CursorShape.CrossCursor);
        assertEquals(null, cursor.bitmap());

        QBitmap bm = new QBitmap("classpath:org/qtjambi/examples/images/cheese.png");
        cursor = new QCursor(bm,
                             new QBitmap("classpath:org/qtjambi/examples/images/cheese.png"));

        assertEquals(bm.width(), cursor.bitmap().width());
        assertEquals(bm.height(), cursor.bitmap().height());

        QImage bmImage = bm.toImage();
        QImage otherImage = cursor.bitmap().toImage();

        assertTrue(bmImage.equals(otherImage));
    }

    @Test
    public void testQImageConstructedFromStringAndStuff() {
        QImage img = new QImage("classpath:org/qtjambi/examples/images/cheese.png", "JPEG");
        assertTrue(img.isNull());  // this assert fails when plugins did not load
        assertEquals(0, img.width());

        img = new QImage("classpath:org/qtjambi/examples/images/cheese.png", "PNG");
        assertFalse(img.isNull());
        assertEquals(94, img.width());
    }

    @Test
    public void testQImageConstructedFromByteArray() {
        QImage img = new QImage("classpath:org/qtjambi/examples/images/cheese.png");

        assertFalse(img.isNull());
        
        byte bytes[] = img.copyOfBytes();
        
        assertTrue(bytes.length>0);
        
        QImage img2 = new QImage(bytes, img.width(), img.height(), img.format());

        assertEquals(img, img2);
    }

    @Test
    public void testQTextCursorSelectedTableCells() {
        QTextDocument document = new QTextDocument();
        QTextCursor cursor = new QTextCursor(document);
        cursor.insertTable(15, 20);

        cursor.movePosition(QTextCursor.MoveOperation.Start);
        cursor.movePosition(QTextCursor.MoveOperation.NextBlock);
        cursor.movePosition(QTextCursor.MoveOperation.NextBlock, QTextCursor.MoveMode.KeepAnchor);
        assertTrue(cursor.hasComplexSelection());

        QTableArea cells = cursor.selectedTableCells();
        assertEquals(2, cells.columnCount);
        assertEquals(1, cells.rowCount);
        assertEquals(0, cells.column);
        assertEquals(0, cells.row);
    }

    @Test
    public void testQPixmapSaveLoadIODevice() {
        QBuffer buffer = new QBuffer();
        buffer.open(QIODevice.OpenModeFlag.WriteOnly);

        QPixmap pmSave = new QPixmap("classpath:org/qtjambi/examples/images/cheese.png");
        assertFalse(pmSave.isNull());
        assertEquals(94, pmSave.width());
        assertTrue(pmSave.save(buffer, "PNG"));

        buffer.close();
        buffer.open(QIODevice.OpenModeFlag.ReadOnly);

        QPixmap pmLoad = new QPixmap();
        pmLoad.loadFromData(buffer.buffer(), "JPEG");
        assertTrue(pmLoad.isNull());  // this assert fails when plugins did not load
        assertEquals(0, pmLoad.width());

        pmLoad.loadFromData(buffer.buffer(), "PNG");
        buffer.close();

        assertFalse(pmLoad.isNull());
        assertEquals(94, pmLoad.width());

        QImage imgSave = pmSave.toImage();
        QImage imgLoad = pmLoad.toImage();

        assertTrue(imgSave.equals(imgLoad));

        buffer = new QBuffer();
        buffer.open(QIODevice.OpenModeFlag.WriteOnly);

        pmSave.save(buffer, "JPEG");
        pmLoad = new QPixmap();
        pmLoad.loadFromData(buffer.buffer(), "PNG");
        assertTrue(pmLoad.isNull());

        pmLoad.loadFromData(buffer.buffer(), "JPEG");
        buffer.close();

        assertFalse(pmLoad.isNull());
        assertEquals(94, pmLoad.width());
    }

    @Test
    public void testQRegionSetRects() {
        QRegion region = new QRegion();

        {
            QRect rects[] = new QRect[2];
            rects[0] = new QRect(0, 0, 10, 10);
            rects[1] = new QRect(5, 11, 10, 10);

            region.setRects(rects);
        }

        {
            List<QRect> rects = region.rects();

            assertEquals(2, rects.size());

            assertEquals(0, rects.get(0).x());
            assertEquals(0, rects.get(0).y());
            assertEquals(10, rects.get(0).width());
            assertEquals(10, rects.get(0).height());

            assertEquals(5, rects.get(1).x());
            assertEquals(11, rects.get(1).y());
            assertEquals(10, rects.get(1).width());
            assertEquals(10, rects.get(1).height());

        }
    }

    @Test
    public void testQPictureData() {
        QTemporaryFile file = setUpPicture();
        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));

        QPicture picture = new QPicture();
        picture.load(file);
        file.close();

        QPicture otherPicture = new QPicture();
        otherPicture.setData(picture.data());

        verifyQPicture(otherPicture);
    }

    @Test
    public void testQPictureSetDataVirtualCall() {
        QTemporaryFile file = setUpPicture();
        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));

        QByteArray ba = file.readAll();

        PictureSubclassSubclass pss = new PictureSubclassSubclass();
        pss.callSetData(ba);

        assertEquals(ba.size(), pss.data.length);

        verifyQPicture(pss);
    }

    @Test
    public void testQPictureSetDataNormalCall() {
        QTemporaryFile file = setUpPicture();

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
        byte bytes[] = file.readAll().toByteArray();
        file.close();

        QPicture picture = new QPicture();
        picture.setData(bytes);

        verifyQPicture(picture);
    }

    @Test
    public void testQPictureLoadFromIODevice() {
        QTemporaryFile file = setUpPicture();

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
        {
            QPicture picture = new QPicture();
            assertTrue(picture.load(file));

            verifyQPicture(picture);
            file.close();
        }
    }

    @Test
    public void testQLineFintersection() {
        QLineF line1 = new QLineF(10, 0, 10, 20);
        QLineF line2 = new QLineF(0, 10, 20, 10);

        QPointF intersectionPoint = new QPointF();

        assertEquals(QLineF.IntersectType.BoundedIntersection, line1.intersect(line2, intersectionPoint));
        assertEquals(10.0, intersectionPoint.x(), 0.0);
        assertEquals(10.0, intersectionPoint.y(), 0.0);

        line2 = new QLineF(0, 30, 20, 30);
        assertEquals(QLineF.IntersectType.UnboundedIntersection, line1.intersect(line2, intersectionPoint));
        assertEquals(10.0, intersectionPoint.x(), 0.0);
        assertEquals(30.0, intersectionPoint.y(), 0.0);

        line2 = new QLineF(11, 0, 11, 20);
        assertEquals(QLineF.IntersectType.NoIntersection, line1.intersect(line2, null));
    }

    @Test
    public void testQDataStreamReadWriteBytes() {
        QByteArray ba = new QByteArray();

        {
            QDataStream stream = new QDataStream(ba, QIODevice.OpenModeFlag.WriteOnly);
            byte bytes[] = "abra ka dabra".getBytes();
            stream.writeInt(bytes.length);
            stream.writeBytes(bytes);
        }

        {
            QDataStream stream = new QDataStream(ba);
            byte bytes[] = new byte[stream.readInt()];
            stream.readBytes(bytes);
            String s = new String(bytes);
            assertEquals("abra ka dabra".length(), s.length());
            assertEquals("abra ka dabra", s);
        }
    }


    @Test
    public void testQDataStreamByteArrayConstruction() {
        {
            QByteArray ba = new QByteArray();

            QDataStream stream = new QDataStream(ba, QIODevice.OpenModeFlag.WriteOnly);
            stream.writeDouble(1.2);
            stream.writeDouble(3.2);
            stream.dispose();

            stream = new QDataStream(ba);

            double f1 = stream.readDouble();
            double f2 = stream.readDouble();
            assertEquals(1.2, f1, 0.0);
            assertEquals(3.2, f2, 0.0);
        }

        for (int i=0; i<100; ++i) {
            QDataStream stream = new QDataStream(new QByteArray(), QIODevice.OpenModeFlag.WriteOnly);
            System.gc();

            stream.writeDouble(1.2);
            stream.writeDouble(3.2);
        }

    }

    @Test
    public void testQBitmapFromData() {
        byte bits[] = { 0x01, 0x02, 0x4, 0x8, 0x10, 0x20, 0x40, (byte) 0x80 };

        QBitmap bm = QBitmap.fromData(new QSize(8, 8), bits);
        assertFalse(bm.isNull());
        assertEquals(8, bm.width());
        assertEquals(8, bm.height());

        QImage img = bm.toImage();
        assertEquals(1, img.pixelIndex(0, 0));
        assertEquals(0, img.pixelIndex(1, 0));
        assertEquals(1, img.pixelIndex(1, 1));


    }

    @Test
    public void testQBitmapStringStringConstructor() {
        {
            QBitmap bm = new QBitmap("classpath:org/qtjambi/examples/images/cheese.png", "PNG");
            assertFalse(bm.isNull());
            assertEquals(94, bm.width());
        }

        {
            QBitmap bm = new QBitmap("classpath:org/qtjambi/examples/images/cheese.png");
            assertFalse(bm.isNull());
            assertEquals(94, bm.width());
        }

        {
            QBitmap bm = new QBitmap("classpath:org/qtjambi/examples/images/cheesemisspelling.png");
            assertTrue(bm.isNull());
        }
    }


    @Test
    public void testQBufferRetainBuffer() {
        QBuffer buffer;
        {
            QByteArray ba = new QByteArray("ABC");
            buffer = new QBuffer(ba, null);
        }
        System.gc();
        assertEquals("ABC", buffer.buffer().toString());
        System.gc();
        buffer.setData( new byte[] {(byte) 'a', (byte) 'b', (byte) 'c'} );
        assertEquals("abc", buffer.buffer().toString());

        {
            QByteArray ba2 = new QByteArray("HIJ");
            buffer.setBuffer(ba2);
        }
        System.gc();
        assertEquals("HIJ", buffer.buffer().toString());

        buffer.setData(new QByteArray("KLM"));
        assertEquals("KLM", buffer.buffer().toString());
    }

    @Test
    public void testQBufferUseBuffer() {
        QByteArray ba = new QByteArray("CDE");
        QBuffer buffer = new QBuffer(ba);
        assertEquals("CDE", buffer.buffer().toString());

        ba.append("fgh");
        assertEquals("CDEfgh", buffer.buffer().toString());

        buffer.setData(new QByteArray("cdeFGH"));
        assertEquals("cdeFGH", ba.toString());

        QByteArray ba2 = new QByteArray("HIJ");
        buffer.setBuffer(ba2);

        assertEquals("HIJ", buffer.buffer().toString());

        ba2.append("KLM");
        assertEquals("HIJKLM", buffer.buffer().toString());
    }

    @Test
    public void testQTextCodecForNameString() {
        QTextCodec codec = QTextCodec.codecForName("UTF-8");

        assertTrue(codec != null);
        assertEquals("UTF-8", codec.name().toString());

        codec = QTextCodec.codecForName("Magic Text Codec Which Successfully Improves What You've Written");
        assertTrue(codec == null);
    }

    @Test
    public void testTextCodecConvertToUnicode() {
        TextCodecSubclassSubclass tcss = new TextCodecSubclassSubclass();

        QTextCodec.ConverterState state = new QTextCodec.ConverterState();

        assertEquals("abba", tcss.callToUnicode(new QByteArray("baab"), state));
        assertTrue(state == tcss.receivedState);
        assertTrue(state == tcss.receivedState());
        assertEquals("baab", new QByteArray(tcss.receivedByte).toString());
    }

    @Test
    public void testTextCodecConvertFromUnicode() {
        TextCodecSubclassSubclass tcss = new TextCodecSubclassSubclass();
        QTextCodec.ConverterState state = new QTextCodec.ConverterState();

        assertEquals("sas", tcss.callFromUnicode("asa", state).toString());
        assertTrue(state == tcss.receivedState);
        assertTrue(state == tcss.receivedState());
        assertEquals("asa", new String(tcss.receivedChar));
    }


    @Test
    public void testIODeviceWriteData() {
        IODeviceSubclassSubclass iodss = new IODeviceSubclassSubclass(128);

        QByteArray ba = new QByteArray("Evil draws men together");
        assertEquals(23, (int) iodss.callWriteData(ba));
        assertEquals(23, iodss.inputBufferSize);
        assertEquals("Evil draws men together", new QByteArray(iodss.buffer).toString());
        QNativePointer np = iodss.buffer();
        byte data[] = new byte[23];
        for (int i=0; i<data.length; ++i)
            data[i] = np.byteAt(i);
        assertEquals("Evil draws men together", new QByteArray(data).toString());
    }

    @Test
    public void testIODeviceReadLineData() {
        IODeviceSubclassSubclass iodss = new IODeviceSubclassSubclass(128);

        assertEquals(45, (int) iodss.callReadLineData());
        assertEquals(128, iodss.inputBufferSize);
        assertEquals(45, iodss.buffer.length);
        assertEquals("Confucius say: Don't go outside with wet hair", new QByteArray(iodss.buffer).toString());

        QNativePointer np = iodss.buffer();
        byte data[] = new byte[45];
        for (int i=0; i<data.length; ++i)
            data[i] = np.byteAt(i);
        assertEquals("Confucius say: Don't go outside with wet hair", new QByteArray(data).toString());
    }

    @Test
    public void testIODeviceReadData() {
        IODeviceSubclassSubclass iodss = new IODeviceSubclassSubclass(128);

        assertEquals(10, (int) iodss.callReadData());
        assertEquals(128, iodss.inputBufferSize);
        assertEquals(10, iodss.buffer.length);
        assertEquals("I am a boy", new QByteArray(iodss.buffer).toString());

        QNativePointer np = iodss.buffer();
        byte data[] = new byte[10];
        for (int i=0; i<data.length; ++i)
            data[i] = np.byteAt(i);
        assertEquals("I am a boy", new QByteArray(data).toString());
    }

    @Test
    public void testIODeviceWrite() {
        QTemporaryFile file = new QTemporaryFile();
        file.setAutoRemove(true);

        assertTrue(file.open(QIODevice.OpenModeFlag.WriteOnly));
        byte data[] = new QByteArray("I am a boy").toByteArray();
        assertEquals(10, file.write(data));
        file.close();

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
        QByteArray all = file.readAll();
        assertEquals("I am a boy", all.toString());

        file.dispose();
    }

    @Test
    public void testIODevicePeek() {
        QAbstractFileEngine.addSearchPathForResourceEngine(".");
        // This was "classpath:org/qtjambi/autotests/TestInjectedCode.java" by it is not
        // normal to expect *.java files in the classpath.
        QFile qfile = new QFile("classpath:org/qtjambi/autotests/TestInjectedCode.dat");
        QIODevice file = qfile;

        boolean bf = qfile.exists();
        assertTrue(bf);

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));

        file.seek(file.bytesAvailable() - 8); //instead of 9. On systems with 2 line end chars may fail

        byte bytes[] = new byte[7];
        assertEquals(7, file.peek(bytes));
        assertEquals((byte) '/', bytes[0]);
        // if this fails check you have Unix end-of-line sequence (just <NL>) of your checked out copy of 
        //  TestInjectedCode.dat (file size should be 65963 bytes) as git can use your native EOL sequence
        //  and make this fail due to character offsets being wrong.
        assertEquals((byte) '/', bytes[1]);
        assertEquals((byte) 't', bytes[2]);
        assertEquals((byte) 'e', bytes[3]);
        assertEquals((byte) 's', bytes[4]);
        assertEquals((byte) 't', bytes[5]);
        assertEquals((byte) '!', bytes[6]);

        file.close();
    }

    @Test
    public void testIODeviceGetByteFail() {
        QIODevice file = new QFile();

        int b = file.getByte();
        assertEquals(-1, b);
    }

    @Test
    public void testIODeviceGetByteSuccess() {
        QAbstractFileEngine.addSearchPathForResourceEngine(".");
        // This was "classpath:org/qtjambi/autotests/TestInjectedCode.java" by it is not
        // normal to expect *.java files in the classpath.
        QIODevice file = new QFile("classpath:org/qtjambi/autotests/TestInjectedCode.dat");

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));

        file.seek(file.bytesAvailable() - 3); //instead of 4. On systems with 2 line end chars may fail

        byte b = (byte) file.getByte();
        // if this fails check you have Unix end-of-line sequence (just <NL>) of your checked out copy of 
        //  TestInjectedCode.dat (file size should be 65963 bytes) as git can use your native EOL sequence
        //  and make this fail due to character offsets being wrong.
        assertEquals((byte) 't', b);

        b = (byte) file.getByte();
        assertEquals((byte) '!', b);

        file.close();
    }

    @Test
    public void testByteArrayStartsWithString() {
        QByteArray ba = new QByteArray("hello");

        assertTrue(ba.startsWith("hell"));
        assertFalse(ba.startsWith("heaven"));
    }

    @Test
    public void testByteArraySetNumOverloads() {
        QByteArray ba = new QByteArray();

        QByteArray ba2 = ba.setNum(11);
        assertEquals("11", ba.toString());
        assertEquals("11", ba2.toString());

        QByteArray ba3 = ba.setNum(1.333333333);
        assertEquals("1.33333", ba.toString());
        assertEquals("1.33333", ba2.toString());
        assertEquals("1.33333", ba3.toString());

        ba3.setNum(13.3333333333, 'e');
        assertEquals("1.333333e+01", ba.toString());
        assertEquals("1.333333e+01", ba2.toString());
        assertEquals("1.333333e+01", ba3.toString());
    }

    @Test
    public void testByteArrayReplaceString() {
        QByteArray ba = new QByteArray("hello");

        QByteArray ba2 = ba.replace(new QByteArray("e"), "a");

        assertEquals("hallo", ba.toString());
        assertEquals("hallo", ba2.toString());
    }

    @Test
    public void testByteArrayPushBackString() {
        QByteArray ba = new QByteArray("hello");

        ba.append("h nice");
        assertEquals("helloh nice", ba.toString());
    }

    @Test
    public void testByteArrayPushFrontString() {
        QByteArray ba = new QByteArray("hello");

        ba.prepend("c");
        assertEquals("chello", ba.toString());
    }

    @Test
    public void testByteArrayPrependString() {
        QByteArray ba = new QByteArray("hello");

        QByteArray ba2 = ba.prepend("c");
        assertEquals("chello", ba.toString());
        assertEquals("chello", ba2.toString());
    }

    @Test
    public void testByteArrayInsert() {
        QByteArray ba = new QByteArray("hello");

        QByteArray ba2 = ba.insert(3, new QByteArray("gefyl"));
        assertEquals("helgefyllo", ba2.toString());
        assertEquals("helgefyllo", ba.toString());
    }

    @Test
    public void testByteArrayFill() {
        QByteArray ba = new QByteArray("hello");

        QByteArray ba2 = ba.fill((byte) 'a');
        assertEquals(ba.toString(), "aaaaa");
        assertEquals(ba2.toString(), "aaaaa");

        QByteArray ba3 = ba.fill((byte) 'b', 3);
        assertEquals(ba.toString(), "bbb");
        assertEquals(ba2.toString(), "bbb");
        assertEquals(ba3.toString(), "bbb");
    }

    @Test
    public void testByteArrayEndsWithString() {
        QByteArray ba = new QByteArray("hello");

        assertTrue(ba.endsWith("lo"));
        assertFalse(ba.endsWith("lol"));
    }

    @Test
    public void testByteArrayCountString() {
        QByteArray ba = new QByteArray("hello");

        assertEquals(0, ba.count("heaven"));
        assertEquals(1, ba.count("hell"));
        assertEquals(2, ba.count("l"));
    }

    @Test
    public void testByteArrayContainsString() {
        QByteArray ba = new QByteArray("hello");

        assertFalse(ba.contains("heaven"));
        assertTrue(ba.contains("hell"));
    }

    @Test
    public void testByteArrayFrombytearray() {
        byte hello[] = new byte[5];
        hello[0] = (byte) 'h';
        hello[1] = (byte) 'e';
        hello[2] = (byte) 'l';
        hello[3] = (byte) 'l';
        hello[4] = (byte) 'o';

        QByteArray ba = new QByteArray(hello);
        assertEquals(5, ba.size());
        assertEquals((byte) 'h', ba.at(0));
        assertEquals((byte) 'e', ba.at(1));
        assertEquals((byte) 'l', ba.at(2));
        assertEquals((byte) 'l', ba.at(3));
        assertEquals((byte) 'o', ba.at(4));
    }

    @Test
    public void testQLocaleToDouble() {
        {
            QLocale locale = new QLocale(QLocale.Language.C);

            boolean caughtException = false;
            double d = 0.0;
            try {
                d = locale.toDouble("1234,56");
            } catch (NumberFormatException e) {
                caughtException = true;
            }

            assertTrue(caughtException);
            assertEquals(0.0, d, 0.0);
            d = 0.0;

            caughtException = false;
            try {
                d = locale.toDouble("1234.56");
            } catch (NumberFormatException e) {
                caughtException = true;
            }

            assertFalse(caughtException);
            assertEquals(1234.56, d, 0.0);
        }

        {
            QLocale locale = new QLocale(QLocale.Language.Norwegian);

            boolean caughtException = false;
            double d = 0.0;
            try {
                d = locale.toDouble("1234,56");
            } catch (NumberFormatException e) {
                caughtException = true;
            }

            assertFalse(caughtException);
            assertEquals(1234.56, d, 0.0);
            d = 0.0;


            caughtException = false;
            try {
                d = locale.toDouble("1234.56");
            } catch (NumberFormatException e) {
                caughtException = true;
            }

            assertTrue(caughtException);
            assertEquals(0.0, d, 0.0);
        }

    }

    @Test
    public void testQLocaleToInt() {
        QLocale locale = new QLocale(QLocale.Language.Norwegian, QLocale.Country.Norway);

//        assertEquals(16, locale.toInt("0x10"));
//        assertEquals(16, locale.toInt("10", 16));
        assertEquals(10, locale.toInt("10"));
//        assertEquals(10, locale.toInt("10", 10));
    }

    @Test
    public void testNestedOperators() {
        QPoint p = new QPoint(2, 3);
        assertEquals(2, p.x());
        assertEquals(3, p.y());

        QPoint self = p.multiply(3).add(new QPoint(3, 6)).divide(3).subtract(new QPoint(2, 1));
        assertEquals(1, self.x());
        assertEquals(4, self.y());
        assertEquals(1, p.x());
        assertEquals(4, p.y());
    }

    @Test
    public void testUnarySelfType() {
        QPoint p = new QPoint(2, 3);
        assertEquals(2, p.x());
        assertEquals(3, p.y());

        QPoint self = p.add(new QPoint(5, 6));
        assertEquals(7, self.x());
        assertEquals(9, self.y());
        assertEquals(7, p.x());
        assertEquals(9, p.y());
    }

    @Test
    public void testUnaryOtherType() {
        QPoint p = new QPoint(2, 3);
        assertEquals(2, p.x());
        assertEquals(3, p.y());

        QPoint self = p.multiply(3);
        assertEquals(6, self.x());
        assertEquals(9, self.y());
        assertEquals(6, p.x());
        assertEquals(9, p.y());
    }

    @Test
    public void testWeekNumber() {
        QDate date = new QDate(2000, 1, 1);

        int weekNumber = date.weekNumber();
        assertEquals(52, weekNumber);

        int yearNumber = date.yearOfWeekNumber();
        assertEquals(1999, yearNumber);

        date.setDate(2002, 12, 31);
        weekNumber = date.weekNumber();
        assertEquals(1, weekNumber);

        yearNumber = date.yearOfWeekNumber();
        assertEquals(2003, yearNumber);
    }

    @Test
    public void testOperatorAssignOtherTypeTemplate() {
//        QDir in = new QDir("classpath:org/qtjambi/");
//        QDir other = new QDir("classpath:org/qtjambi/examples/");
//        assertFalse(other.equals(in));
//
//        String out = "classpath:org/qtjambi/examples/";
//        QDir self = in.operator_assign(out);
//        assertTrue(self.equals(in));
//        assertEquals(self.absolutePath(), in.absolutePath());
//
//        assertTrue(other.equals(in));
//        assertEquals(other.count(), in.count());
//        assertEquals(other.absolutePath(), in.absolutePath());
    }

    @Test
    public void testOperatorAssignSelfTypeTemplate() {
//        QDate date = new QDate(2006, 11, 30);
//        QTime time = new QTime(8, 19);
//
//        QDateTime in = new QDateTime(date, time);
//        assertEquals(30, in.date().day());
//        assertEquals(11, in.date().month());
//        assertEquals(2006, in.date().year());
//        assertEquals(8, in.time().hour());
//        assertEquals(19, in.time().minute());
//
//        QDate date_out = new QDate(1963, 11, 22);
//        QTime time_out = new QTime(12, 30);
//        QDateTime out = new QDateTime(date_out, time_out);
//        assertEquals(22, out.date().day());
//        assertEquals(11, out.date().month());
//        assertEquals(1963, out.date().year());
//        assertEquals(12, out.time().hour());
//        assertEquals(30, out.time().minute());
//
//        QDateTime self = out.operator_assign(in);
//        assertTrue(self.equals(out));
//        assertTrue(self.equals(in));
//        assertTrue(in.equals(out));
//
//        assertEquals(30, out.date().day());
//        assertEquals(11, out.date().month());
//        assertEquals(2006, out.date().year());
//        assertEquals(8, out.time().hour());
//        assertEquals(19, out.time().minute());
//
//        assertEquals(30, self.date().day());
//        assertEquals(11, self.date().month());
//        assertEquals(2006, self.date().year());
//        assertEquals(8, self.time().hour());
//        assertEquals(19, self.time().minute());
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestInjectedCode.class.getName());
    }
}

// next line is part of test.. leav it
//test!
