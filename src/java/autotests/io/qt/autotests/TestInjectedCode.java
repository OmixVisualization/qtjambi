/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
package io.qt.autotests;

import static org.junit.Assert.*;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.math.BigInteger;
import java.nio.BufferOverflowException;
import java.nio.ByteBuffer;
import java.nio.ReadOnlyBufferException;
import java.util.Iterator;

import org.junit.*;

import io.qt.*;
import io.qt.autotests.generated.*;
import io.qt.core.*;
import io.qt.gui.*;
import io.qt.widgets.*;

public class TestInjectedCode extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    static class IODeviceSubclassSubclass extends IODeviceSubclass {
        public byte buffer[];
        public int inputBufferSize;

        public IODeviceSubclassSubclass(int bufferLength) {
            super(bufferLength);
        }

        @Override
        protected int readData(java.nio.ByteBuffer data) {
            inputBufferSize = data.limit() - data.position();

            int size = super.readData(data);
            buffer = new byte[size];
            for (int i=0; i<size; ++i)
                buffer[i] = data.get(i);

            return size;
        }

        @Override
        protected int writeData(java.nio.ByteBuffer data) {
            inputBufferSize = data.limit() - data.position();
            buffer = new byte[data.limit() - data.position()];

            for (int i=0; i<inputBufferSize; ++i)
                buffer[i] = data.get(i);

            return super.writeData(data);
        }

        @Override
        protected int readLineData(java.nio.ByteBuffer data) {
            inputBufferSize = data.limit() - data.position();

            int size = super.readLineData(data);
            buffer = new byte[size];
            for (int i=0; i<size; ++i)
                buffer[i] = data.get(i);

            return size;
        }
    }

    static class OtherIODeviceSubclass extends QIODevice {
        public byte[] bytes = {(byte) 'h', (byte) 'a', (byte) 'l' };
        public int i = 0;
        public QByteArray ba = new QByteArray();

        @Override
        protected int readData(java.nio.ByteBuffer data) {
            if (i == bytes.length) return -1;
            data.put(bytes[i++]);
            return 1;
        }

        @Override
        protected int writeData(java.nio.ByteBuffer data) {
            if (data.limit() - data.position() == 0) return -1;
            ba.append(data);
            return 1;
        }
    }

    static class PictureSubclassSubclass extends PictureSubclass {
        public int dataLength;

        @Override
        public void setData(java.nio.ByteBuffer data) {
            this.dataLength = data.limit() - data.position();
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
            painter.fillRect(new QRect(51, 0, 50, 50), new QBrush(new QColor(io.qt.core.Qt.GlobalColor.green)));
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

        @SuppressWarnings("unused")
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

        public final Signal0 myActionTriggered = new Signal0();
    }

    static class ValidatorSubclassSubclass extends ValidatorSubclass {

        public String inputString;
        public int inputPos;

        @Override
        public void fixup(QString input) {
            inputString = input.toString();
            super.fixup(input);
            input.append("javaPostfix");
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
        public QImage read() {
        	QImage image = new QImage();
            this.image = super.read();
            return this.image!=null && image.load(":io/qt/autotests/blue_angle_swirl.jpg") ? image : null;
        }

        @Override
        public boolean canRead() {
            return false;
        }

    }

    private static QTemporaryFile setUpPicture() {
        QTemporaryFile file = new QTemporaryFile();

        assertTrue(file.open(QIODevice.OpenModeFlag.WriteOnly));

        {
            QPicture picture = new QPicture();
            QPainter painter = new QPainter();
            painter.begin(picture);
            painter.fillRect(new QRect(0, 0, 10, 10), new QBrush(new QColor(io.qt.core.Qt.GlobalColor.green)));
            painter.fillRect(new QRect(11, 0, 10, 10), new QBrush(new QColor(io.qt.core.Qt.GlobalColor.red)));
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

        assertEquals(new QColor(io.qt.core.Qt.GlobalColor.green).rgba(), img.pixel(4, 4));
        assertEquals(new QColor(io.qt.core.Qt.GlobalColor.red).rgba(), img.pixel(12, 4));
    }

    private static class MyAccessibleInterface extends AccessibleTextInterfaceSubclass
    {
		@Override
		public String attributes(int offset, int[] startOffset, int[] endOffset) {
			return super.attributes(offset+1, startOffset, endOffset);
		}

		@Override
		public void selection(int selectionIndex, int[] startOffset, int[] endOffset) {
			super.selection(selectionIndex+1, startOffset, endOffset);
		}

		@Override
		public String textAfterOffset(int offset, QAccessible.TextBoundaryType boundaryType, int[] startOffset, int[] endOffset) {
			return super.textAfterOffset(offset+1, boundaryType, startOffset, endOffset);
		}

		@Override
		public String textAtOffset(int offset, QAccessible.TextBoundaryType boundaryType, int[] startOffset, int[] endOffset) {
			return super.textAtOffset(offset+1, boundaryType, startOffset, endOffset);
		}

		@Override
		public String textBeforeOffset(int offset, QAccessible.TextBoundaryType boundaryType, int[] startOffset, int[] endOffset) {
			return super.textBeforeOffset(offset+1, boundaryType, startOffset, endOffset);
		}

		@Override
		public void addSelection(int startOffset, int endOffset) {
		}

		@Override
		public int characterCount() {
			return 0;
		}

		@Override
		public QRect characterRect(int offset) {
			return null;
		}

		@Override
		public int cursorPosition() {
			return 0;
		}

		@Override
		public int offsetAtPoint(QPoint point) {
			return 0;
		}

		@Override
		public void removeSelection(int selectionIndex) {
		}

		@Override
		public void scrollToSubstring(int startIndex, int endIndex) {
		}

		@Override
		public int selectionCount() {
			return 0;
		}

		@Override
		public void setCursorPosition(int position) {
		}

		@Override
		public void setSelection(int selectionIndex, int startOffset, int endOffset) {
		}

		@Override
		public String text(int startOffset, int endOffset) {
			return null;
		}
        
        
    }

    @Test
    public void testQAccessibleAttributes()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        int[] startOffset = {0};
        int[] endOffset = {0};
        AccessibleTextInterfaceSubclass.callAttributes(m, 10, startOffset, endOffset);
        assertEquals(12, startOffset[0]);
        assertEquals(13, endOffset[0]);
    }

    @Test
    public void testQAccessibleAttributesDontCrashOnNull()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        AccessibleTextInterfaceSubclass.callAttributes(m, 10, null, null);
    }

    @Test
    public void testQAccessibleSelection()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        int[] startOffset = {0};
        int[] endOffset = {0};
        AccessibleTextInterfaceSubclass.callSelection(m, 10, startOffset, endOffset);
        assertEquals(12, startOffset[0]);
        assertEquals(13, endOffset[0]);
    }
    
    @Test
    public void testQAccessibleSelectionDontCrashOnNull()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        AccessibleTextInterfaceSubclass.callSelection(m, 10, null, null);
    }

    @Test
    public void testQAccessibleTextBeforeOffset()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        int[] startOffset = {0};
        int[] endOffset = {0};
        AccessibleTextInterfaceSubclass.callTextBeforeOffset(m, 10, QAccessible.TextBoundaryType.CharBoundary, startOffset, endOffset);
        assertEquals(12, startOffset[0]);
        assertEquals(13, endOffset[0]);
    }
    
    @Test
    public void testQAccessibleTextBeforeOffsetDontCrashOnNull()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        AccessibleTextInterfaceSubclass.callTextBeforeOffset(m, 10, QAccessible.TextBoundaryType.CharBoundary, null, null);
    }

    @Test
    public void testQAccessibleTextAfterOffset()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        int[] startOffset = {0};
        int[] endOffset = {0};
        AccessibleTextInterfaceSubclass.callTextAfterOffset(m, 10, QAccessible.TextBoundaryType.CharBoundary, startOffset, endOffset);
        assertEquals(12, startOffset[0]);
        assertEquals(13, endOffset[0]);
    }
    
    @Test
    public void testQAccessibleTextAfterOffsetDontCrashOnNull()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        AccessibleTextInterfaceSubclass.callTextAfterOffset(m, 10, QAccessible.TextBoundaryType.CharBoundary, null, null);
    }

    @Test
    public void testQAccessibleTextAtOffset()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        int[] startOffset = {0};
        int[] endOffset = {0};
        AccessibleTextInterfaceSubclass.callTextAtOffset(m, 10, QAccessible.TextBoundaryType.CharBoundary, startOffset, endOffset);
        assertEquals(12, startOffset[0]);
        assertEquals(13, endOffset[0]);
    }
    
    @Test
    public void testQAccessibleTextAtOffsetDontCrashOnNull()
    {
        MyAccessibleInterface m = new MyAccessibleInterface();
        AccessibleTextInterfaceSubclass.callTextAtOffset(m, 10, QAccessible.TextBoundaryType.CharBoundary, null, null);
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
        cursor = null;
        ApplicationInitializer.runGC();
        ApplicationInitializer.runGC();
    }
    
    @Test
    public void testQCborValue__fromCbor() {
    	QByteArray data = new QByteArray();
    	QCborStreamWriter writer = new QCborStreamWriter(data);
    	writer.append("TEST");
    	QCborValue.FromCborResult result = QCborValue.fromCbor(data);
    	assertEquals(QCborError.Code.NoError, result.error.error().code());
    	assertTrue(result.value.isString());
    	assertEquals("TEST", result.value.toString());
    }
    
    @Test
    public void testQCborStreamReader__readByteArray() {
    	QByteArray data = new QByteArray();
    	QCborStreamWriter writer = new QCborStreamWriter(data);
    	writer.startArray();
    	writer.append("TEST");
    	writer.append(true);
    	writer.append(Float.MAX_VALUE);
    	writer.endArray();
    	
    	QCborStreamReader reader = new QCborStreamReader(data);
    	assertTrue(reader.isArray());
    	assertTrue(reader.enterContainer());
    	assertTrue(reader.isString());
    	QCborStreamReader.StringResult<String> result = reader.readString();
    	assertTrue(result!=null);
    	assertEquals(QCborStreamReader.StringResultCode.Ok, result.status());
    	assertEquals("TEST", result.data());
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.isBoolean());
    	assertTrue(reader.toBoolean());
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.isFloat());
    	assertEquals(Float.MAX_VALUE, reader.toFloat(), 0.00001f);
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.leaveContainer());
    }
    
    @Test
    public void testQCborStreamReader__BigInteger() {
    	QByteArray data = new QByteArray();
    	QCborStreamWriter writer = new QCborStreamWriter(data);
    	writer.startArray();
    	BigInteger bigInteger = new BigInteger("0fcafebabe012345f", 16);
    	writer.append(bigInteger);
    	BigInteger bigNegInteger = new BigInteger("-0f35014541fedcba1", 16);
    	writer.append(bigNegInteger);
    	BigInteger bigInteger2 = BigInteger.valueOf(1);
    	writer.append(bigInteger2);
    	BigInteger bigNegInteger2 = BigInteger.valueOf(-1);
    	writer.append(bigNegInteger2);
    	BigInteger bigIntegerNull = BigInteger.valueOf(0);
    	writer.append(bigIntegerNull);
    	writer.endArray();
    	
    	QCborStreamReader reader = new QCborStreamReader(data);
    	assertTrue(reader.isArray());
    	assertTrue(reader.enterContainer());
    	assertTrue(reader.isBigInteger());
    	assertEquals(bigInteger, reader.toBigInteger());
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.isBigInteger());
    	assertEquals(bigNegInteger, reader.toBigInteger());
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.isBigInteger());
    	assertEquals(bigInteger2, reader.toBigInteger());
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.isBigInteger());
    	assertEquals(bigNegInteger2, reader.toBigInteger());
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.isBigInteger());
    	assertEquals(bigIntegerNull, reader.toBigInteger());
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.leaveContainer());
    	assertFalse(reader.hasNext());
    	assertFalse(reader.next());
    }
    
    
    @Test
    public void testQCborStreamReader__fromByteBuffer() {
    	ByteBuffer data = ByteBuffer.allocateDirect(1024*1024);
    	QIODevice device = QIODevice.fromBuffer(data);
    	assertTrue(device.open(QIODevice.OpenModeFlag.WriteOnly));
    	assertTrue(device.isOpen());
    	QCborStreamWriter writer = new QCborStreamWriter(device);
    	writer.startArray();
    	QByteArray test = new QByteArray("TEST");
    	writer.append(test);
    	writer.endArray();
    	QCborStreamReader reader = new QCborStreamReader(data, 1024*1024);
    	assertTrue(reader.isArray());
    	assertTrue(reader.enterContainer());
    	assertTrue(reader.isByteArray());
    	QCborStreamReader.StringResult<QByteArray> result = reader.readByteArray();
    	assertTrue(result!=null);
    	assertEquals(QCborStreamReader.StringResultCode.Ok, result.status());
    	assertEquals(test, result.data());
    	assertEquals(QCborError.Code.NoError, reader.lastError().code());
    	assertTrue(reader.hasNext());
    	assertTrue(reader.next());
    	assertTrue(reader.leaveContainer());
    	assertFalse(reader.hasNext());
    	assertFalse(reader.next());
    }
    
    @Test
    public void testQLockFile_getLockInfo() {
    	QLockFile lockFile = new QLockFile("qtjambi.lock");
    	lockFile.lock();
    	QLockFile.LockInfo info = lockFile.getLockInfo();
    	lockFile.unlock();
    	lockFile.removeStaleLockFile();
    	assertEquals(QCoreApplication.applicationPid(), info.pid);
    	if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
	    	QFileInfo applicationFilePath = new QFileInfo(QCoreApplication.applicationFilePath());
	    	String fileName = applicationFilePath.fileName();
	    	if(fileName.endsWith(".exe")) {
	    		fileName = fileName.substring(0, fileName.length()-4);
	    	}
	    	assertEquals(info.appname, fileName);
    	}
    }
    
    @Test
    public void testQQuaternion() {
    	QQuaternion quarternion = new QQuaternion(1,2,3,4);
    	QQuaternion.Axes axes = quarternion.getAxes();
    	QQuaternion.AxisAndAngle axisAndAngle = quarternion.getAxisAndAngle();
    	QQuaternion.EulerAngles eulerAngles = quarternion.getEulerAngles();
    	assertEquals(new QVector3D(-49,20,10), axes.xAxis);
    	assertEquals(new QVector3D(4,-39,28), axes.yAxis);
    	assertEquals(new QVector3D(22,20,-25), axes.zAxis);
    	assertTrue(axisAndAngle.axis!=null);
    	assertEquals(0.371391f, axisAndAngle.axis.x(), 0.001);
    	assertEquals(0.557086f, axisAndAngle.axis.y(), 0.001);
    	assertEquals(0.742781f, axisAndAngle.axis.z(), 0.001);
    	if(QLibraryInfo.version().compareTo(new QVersionNumber(6,7,0))>=0)
    		assertEquals(158.96054077148438f, axisAndAngle.angle, 0.001);
    	assertEquals(-41.81031799316406f, eulerAngles.pitch, 0.001);
    	assertEquals(79.69515228271484f, eulerAngles.yaw, 0.001);
    	assertEquals(116.56504821777344f, eulerAngles.roll, 0.001);
    }

	@Test
	public void testTextObjectInterface() {
        QTextDocument document = new QTextDocument();
        QAbstractTextDocumentLayout layout = new QPlainTextDocumentLayout(document);

        class TextObjectImpl extends io.qt.core.QObject implements QTextObjectInterface{
            @QtUninvokable
            public void drawObject(QPainter painter, QRectF rect, QTextDocument doc, int posInDocument,
                                   QTextFormat format) {
            }

            @QtUninvokable
            public QSizeF intrinsicSize(QTextDocument doc, int posInDocument, QTextFormat format) {
                return null;
            }
        };

        TextObjectImpl myTextObjectInterface = new TextObjectImpl();
        layout.registerHandler(QTextFormat.ObjectTypes.UserObject.value()+1, myTextObjectInterface);
        assertEquals(myTextObjectInterface, layout.handlerForObject(QTextFormat.ObjectTypes.UserObject.value()+1));
        layout.unregisterHandler(QTextFormat.ObjectTypes.UserObject.value()+1, myTextObjectInterface);
        assertEquals(null, layout.handlerForObject(QTextFormat.ObjectTypes.UserObject.value()+1));
    }


    @Test
    public void QStylesItemDelegateInitStyleOption() {
        QStyleOptionViewItem item = new QStyleOptionViewItem();
        StyledItemDelegateSubclass delegate = new StyledItemDelegateSubclass() {

            @Override
            protected void initStyleOption(QStyleOptionViewItem item, QModelIndex index) {
                item.setDecorationSize(new QSize(123, 456));
            }
        };

        delegate.initStyleOptionInStyledDelegate(item);

        assertEquals(123, item.decorationSize().width());
        assertEquals(456, item.decorationSize().height());
    }


    static class GraphicsWidgetSubclassSubclass extends GraphicsWidgetSubclass {

        @Override
        protected void initStyleOption(QStyleOption option) {
            if (option instanceof QStyleOptionGroupBox) {
                QStyleOptionGroupBox box = (QStyleOptionGroupBox) option;
                box.setLineWidth(321);
            }

            super.initStyleOption(option);
        }

    }

    @Test
    public void QGraphicsWidget_callInitStyleOption() {
        GraphicsWidgetSubclassSubclass gwss = new GraphicsWidgetSubclassSubclass();
        int ret = GraphicsWidgetSubclass.callInitStyleOption(gwss);
        assertEquals(444, ret);
    }

    @Test
    public void testQJsonDocument__fromJson() {
    	QJsonDocument.FromJsonResult result = QJsonDocument.fromJson(new QByteArray("asdasd"));
    	assertTrue(result!=null);
    	assertTrue(result.document!=null);
    	assertTrue(result.error!=null);
    	assertTrue(result.document.isEmpty());
    	assertEquals(QJsonParseError.ParseError.IllegalValue, result.error.error());
    	assertEquals(1, result.error.offset());
    	result = QJsonDocument.fromJson(
    			new QJsonDocument(
	    			new QJsonObject(new QPair<>("A", new QJsonValue("A")),
					    			new QPair<>("B", new QJsonValue("B")),
					    			new QPair<>("C", new QJsonValue("C"))
	    			)
    			).toJson());
    	assertTrue(result!=null);
    	assertTrue(result.document!=null);
    	assertTrue(result.error!=null);
    	assertFalse(result.document.isEmpty());
    	assertTrue(result.document.isObject());
    	assertEquals(QJsonParseError.ParseError.NoError, result.error.error());
    	assertEquals(0, result.error.offset());
    }

    @Test
    @Deprecated
    public void testQClipboard_text() {
        QClipboard clipboard = QApplication.clipboard();
        clipboard.clear();

        QClipboard.Text text = clipboard.text("html");
        assertEquals("html", text.subtype);
    	assertTrue(text.text!=null);
        
        text = clipboard.text("plain");
        assertEquals("plain", text.subtype);
    	assertTrue(text.text!=null);

    	if(!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android)) {
	    	text = clipboard.text((String)null);
	        assertEquals("", text.subtype);
	    	assertTrue(text.text!=null);
    	}
    }

    @Test
    public void testQImageIOHandlerRead() {
        QImage image = new QImage();
        ImageIOHandlerSubclassSubclass iihss = new ImageIOHandlerSubclassSubclass();
        iihss.callRead(null);
        assertTrue(iihss.callRead(image));
        
        QImage ref1 = new QImage(":io/qt/autotests/svgcards-example.png");
        QImage ref2 = new QImage(":io/qt/autotests/blue_angle_swirl.jpg");

        assertEquals(ref2.width(), image.width());
        assertEquals(ref2.height(), image.height());
        assertTrue(ref2.equals(image));

        assertEquals(ref1.width(), iihss.image.width());
        assertEquals(ref1.height(), iihss.image.height());
        assertTrue(ref1.equals(iihss.image));

    }

    @Test
    public void testQWidget_contentsMargins() {
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
    public void testQGridLayout_getItemPosition() {
        QGridLayout layout = new QGridLayout();

        QWidget w1 = new QWidget();
        QWidget w2 = new QWidget();

        layout.addWidget(w1, 2, 2);
        layout.addWidget(w2, 3, 4, 5, 6);

        {
        	QGridLayout.ItemInfo ip = layout.getItemPosition(0);
            assertEquals(2, ip.row);
            assertEquals(2, ip.column);
            assertEquals(1, ip.rowSpan);
            assertEquals(1, ip.columnSpan);
        }

        {
        	QGridLayout.ItemInfo ip = layout.getItemPosition(1);
            assertEquals(3, ip.row);
            assertEquals(4, ip.column);
            assertEquals(5, ip.rowSpan);
            assertEquals(6, ip.columnSpan);
        }
    }

    @Test // crashes for unknown reason
    public void testValidatorFixup() {
        ValidatorSubclassSubclass vss = new ValidatorSubclassSubclass();

        final QString str = new QString("acceptable");
        int[] i = {13};
        assertEquals(QValidator.State.Acceptable, vss.callValidate(str, i));
        assertEquals("javaPrefixacceptablesomePostfix", str.toString());
        assertEquals("acceptable".length(), i[0]);
        assertEquals("acceptable", vss.inputString);
        assertEquals(13, vss.inputPos);
        assertEquals("acceptable", vss.inputString());
        assertEquals(13, vss.inputPos());

        str.assign("intermediate");
        i[0] = 14;
        assertEquals(QValidator.State.Intermediate, vss.callValidate(str, i));
        assertEquals("javaPrefixintermediatesomePostfix", str.toString());
        assertEquals("intermediate".length(), i[0]);
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
            QPixmap pm = new QPixmap(":io/qt/autotests/svgcards-example.png");
            assertEquals(418, pm.width());
            QPixmapCache.insert("myPixmap", pm);
        }

        {
        	//FIX: Warning: QPixmap::operator=: Cannot assign to pixmap during painting
            QPixmap pm = QPixmapCache.find("myPixmap");
            assertTrue(pm!=null);
            assertEquals(418, pm.width());
            
            pm = QPixmapCache.find("noSuchPixmap");
            assertFalse(pm!=null);
        }
    }

    static boolean called = false;
    void myReceiver() {
        called = true;
    }

    @Test
    public void testQMenuAddActionJavaSignal() {
		AnyClass ac = new AnyClass();
		ac.myActionTriggered.connect(this::myReceiver);

		QMenu menu = new QMenu();
		QAction action = menu.addAction(new QIcon(), "blah", ac.myActionTriggered);

		called = false;
		action.activate(QAction.ActionEvent.Trigger);
		assertTrue(called);
    }

    @Test
    public void testQMenuAddActionJavaSlot() {
        AnyClass ac = new AnyClass();
        ac.myActionTriggered.connect(this::myReceiver);

        QMenu menu = new QMenu();
        QAction action = menu.addAction(null, "blah", ac::actionTriggered);

        called = false;
        action.activate(QAction.ActionEvent.Trigger);
        assertTrue(called);
    }

    @Test
    public void testQMenuAddActionCpp() {
        SomeQObject sqo = new SomeQObject();
        QMenu menu = new QMenu();
        QAction action = menu.addAction(null, "blah", sqo::actionTriggered);
        sqo.myActionTriggered.connect(this::myReceiver);

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
        assertTrue(option.features().testFlag(QStyleOptionButton.ButtonFeature.Flat));

        accessor.setFlat(false);
        accessor.callInitStyleOption(option);
        assertFalse(option.features().testFlag(QStyleOptionButton.ButtonFeature.Flat));
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

        assertEquals(new QColor(io.qt.core.Qt.GlobalColor.red).rgba(), image.pixel(2, 2));
        assertEquals(new QColor(io.qt.core.Qt.GlobalColor.green).rgba(), image.pixel(52, 2));
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
        QBrush brush = new QBrush(new QColor(io.qt.core.Qt.GlobalColor.red));
        assertEquals(null, brush.gradient());

        QGradient gradient = new QLinearGradient(0, 0, 11, 12);
        brush = new QBrush(gradient);

        assertTrue(brush.gradient() != null);
        assertEquals(QLinearGradient.class, brush.gradient().getClass());
        QLinearGradient lg = (QLinearGradient) brush.gradient();
        assertEquals(0.0, lg.start().x(), 0.0);
        assertEquals(0.0, lg.start().y(), 0.0);
        assertEquals(11.0, lg.finalStop().x(), 0.0);
        assertEquals(12.0, lg.finalStop().y(), 0.0);

        gradient = new QRadialGradient(0, 0, 1);
        brush = new QBrush(gradient);

        assertTrue(brush.gradient() != null);
        assertEquals(QRadialGradient.class, brush.gradient().getClass());
        
        brush = new QBrush((QGradient)null);

        assertTrue(brush.gradient() == null);
    }

    @Test
    public void testQImageLoadFromData() {
        QImage img;
        QFile file = new QFile(":io/qt/autotests/svgcards-example.png");

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
        img = new QImage();
        img.load(file, "png");
        assertFalse(img.isNull());
        assertEquals(418, img.width());

        file.reset();
        QByteArray data = file.readAll();
        img = new QImage();
        img.loadFromData(data);
        assertFalse(img.isNull());
        assertEquals(418, img.width());
        file.reset();

        img = new QImage();
        img.load(file, "JPEG");
        assertTrue(img.isNull());  // this assert fails when plugins did not load

        img = new QImage();
        file.reset();
        img.load(file, "png");
        assertFalse(img.isNull());
        assertEquals(418, img.width());

        img = new QImage();
        file.reset();
        img.loadFromData(file.readAll(), "JPEG");
        assertTrue(img.isNull());

        img = new QImage();
        file.reset();
        img.loadFromData(file.readAll());
        assertFalse(img.isNull());
        assertEquals(418, img.width());
    }

    @SuppressWarnings("deprecation")
	@Test
    public void testQCursorBitmap() {
        QCursor cursor = new QCursor(Qt.CursorShape.CrossCursor);
        assertTrue(cursor.bitmap().isNull());

        QBitmap bm = new QBitmap(":io/qt/autotests/svgcards-example.png");
        cursor = new QCursor(bm,
                             new QBitmap(":io/qt/autotests/svgcards-example.png"));

        assertFalse(cursor.bitmap().isNull());
        assertEquals(bm.width(), cursor.bitmap().width());
        assertEquals(bm.height(), cursor.bitmap().height());

        QImage bmImage = bm.toImage();
        QImage otherImage = cursor.bitmap().toImage();

        assertTrue(bmImage.equals(otherImage));
    }

    @Test
    public void testQImageConstructedFromStringAndStuff() {
        QImage img = new QImage(":io/qt/autotests/svgcards-example.png", "JPEG");
        assertTrue(img.isNull());  // this assert fails when plugins did not load
        assertEquals(0, img.width());

        img = new QImage(":io/qt/autotests/svgcards-example.png", "PNG");
        assertFalse(img.isNull());
        assertEquals(418, img.width());
    }

    @Test
    public void testQImageConstructedFromByteArray() {
        QImage img = new QImage(":io/qt/autotests/svgcards-example.png");

        assertFalse(img.isNull());
        
        byte bytes[] = img.bytes();
        
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

        QTextCursor.SelectedTableCells cells = cursor.selectedTableCells();
        assertEquals(2, cells.numColumns);
        assertEquals(1, cells.numRows);
        assertEquals(0, cells.firstColumn);
        assertEquals(0, cells.firstRow);
    }

    @Test
    public void testQPixmapSaveLoadIODevice() {
        QBuffer buffer = new QBuffer();
        buffer.open(QIODevice.OpenModeFlag.WriteOnly);

        QPixmap pmSave = new QPixmap(":io/qt/autotests/svgcards-example.png");
        assertFalse(pmSave.isNull());
        assertEquals(418, pmSave.width());
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
        assertEquals(418, pmLoad.width());

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
        assertEquals(418, pmLoad.width());
    }

    @Test
    public void testQRegionSetRects() {
        QRegion region = new QRegion();

        {
            QRect rects[] = new QRect[2];
            rects[0] = new QRect(0, 0, 10, 10);
            rects[1] = new QRect(5, 11, 20, 20);

            region.setRects(rects);
        }

        {
            Iterator<QRect> iterator = region.iterator();
            assertTrue(iterator.hasNext());
            QRect current = iterator.next();
            assertEquals(0, current.x());
            assertEquals(0, current.y());
            assertEquals(10, current.width());
            assertEquals(10, current.height());

            assertTrue(iterator.hasNext());
            current = iterator.next();
            assertEquals(5, current.x());
            assertEquals(11, current.y());
            assertEquals(20, current.width());
            assertEquals(20, current.height());

            assertFalse(iterator.hasNext());
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

        assertEquals(ba.size(), pss.dataLength);

        verifyQPicture(pss);
    }

    @Test
    public void testQPictureSetDataNormalCall() {
        QTemporaryFile file = setUpPicture();

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
        byte bytes[] = file.readAll().toArray();
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
    public void testQDataStreamReadWriteBytes() {
        QByteArray ba = new QByteArray();

        {
            QDataStream stream = new QDataStream(ba, QIODevice.OpenModeFlag.WriteOnly);
            byte bytes[] = "abra ka dabra".getBytes();
            stream.writeInt(bytes.length);
            stream.writeRawData(bytes);
        }

        {
            QDataStream stream = new QDataStream(ba);
            byte bytes[] = new byte[stream.readInt()];
            stream.readRawData(bytes);
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
            ApplicationInitializer.runGC();

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
            QBitmap bm = new QBitmap(":io/qt/autotests/svgcards-example.png", "PNG");
            assertFalse(bm.isNull());
            assertEquals(418, bm.width());
        }

        {
            QBitmap bm = new QBitmap(":io/qt/autotests/svgcards-example.png");
            assertFalse(bm.isNull());
            assertEquals(418, bm.width());
        }

        {
            QBitmap bm = new QBitmap(":io/qt/autotests/svgcards-misspelling.png");
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
        ApplicationInitializer.runGC();
        assertEquals("ABC", buffer.buffer().toString());
        ApplicationInitializer.runGC();
        buffer.setData( new byte[] {(byte) 'a', (byte) 'b', (byte) 'c'} );
        assertEquals("abc", buffer.buffer().toString());

        {
            QByteArray ba2 = new QByteArray("HIJ");
            buffer.setBuffer(ba2);
        }
        ApplicationInitializer.runGC();
        assertEquals("HIJ", buffer.buffer().toString());

        buffer.setData(new QByteArray("KLM"));
        assertEquals("KLM", buffer.buffer().toString());
    }

	@Test
    public void testQBufferUseBuffer() {
        QByteArray ba = new QByteArray("CDE".getBytes());
        QBuffer buffer = new QBuffer(ba);
        assertEquals("CDE", buffer.buffer().toString());

        ba.append("fgh");
        assertEquals("CDEfgh", buffer.buffer().toString());

        buffer.setData(new QByteArray("cdeFGH".getBytes()));
        assertEquals("cdeFGH", ba.toString());

        QByteArray ba2 = new QByteArray("HIJ");
        buffer.setBuffer(ba2);

        assertEquals("HIJ", buffer.buffer().toString());

        ba2.append("KLM");
        assertEquals("HIJKLM", buffer.buffer().toString());
    }

    @Test
    public void testIODeviceWriteData() {
        IODeviceSubclassSubclass iodss = new IODeviceSubclassSubclass(128);

        QByteArray ba = new QByteArray("Evil draws men together");
        assertEquals(23, (int) iodss.callWriteData(ba));
        assertEquals(23, iodss.inputBufferSize);
        assertEquals("Evil draws men together", new QByteArray(iodss.buffer).toString());
        java.nio.ByteBuffer np = iodss.buffer();
        assertTrue(np.capacity()>=23);
        byte data[] = new byte[23];
        for (int i=0; i<data.length; ++i)
            data[i] = np.get(i);
        assertEquals("Evil draws men together", new QByteArray(data).toString());
    }

    @Test
    public void testIODeviceReadLineData() {
        IODeviceSubclassSubclass iodss = new IODeviceSubclassSubclass(128);

        assertEquals(45, (int) iodss.callReadLineData());
        assertEquals(128, iodss.inputBufferSize);
        assertEquals(45, iodss.buffer.length);
        assertEquals("Confucius say: Don't go outside with wet hair", new QByteArray(iodss.buffer).toString());

        java.nio.ByteBuffer np = iodss.buffer();
        assertTrue(np.capacity()>=45);
        byte data[] = new byte[45];
        for (int i=0; i<data.length; ++i)
            data[i] = np.get(i);
        assertEquals("Confucius say: Don't go outside with wet hair", new QByteArray(data).toString());
    }

    @Test
    public void testIODeviceReadData() {
        IODeviceSubclassSubclass iodss = new IODeviceSubclassSubclass(128);

        assertEquals(10, (int) iodss.callReadData());
        assertEquals(128, iodss.inputBufferSize);
        assertEquals(10, iodss.buffer.length);
        assertEquals("I am a boy", new QByteArray(iodss.buffer).toString());

        java.nio.ByteBuffer np = iodss.buffer();
        assertTrue(np.capacity()>=10);
        byte data[] = new byte[10];
        for (int i=0; i<data.length; ++i)
            data[i] = np.get(i);
        assertEquals("I am a boy", new QByteArray(data).toString());
    }

    @Test
    public void testIODeviceRW() {
        QTemporaryFile file = new QTemporaryFile();
        file.setAutoRemove(true);
        byte data[] = "I am a boy".getBytes();
        try {
	        assertTrue(file.open(QIODevice.OpenModeFlag.WriteOnly));
	        assertEquals(10, file.write(data));
	        file.close();
	
	        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));
	        QByteArray all = file.readAll();
	        assertEquals("I am a boy", all.toString());
        }finally {
        	file.dispose();
        }
    }
    
    @Test
    public void testStreamBasedIODeviceRW() {
    	ByteArrayOutputStream out = new ByteArrayOutputStream();
        QIODevice iodevice = QIODevice.fromOutputStream(out);
        byte data[] = "I am a girl".getBytes();
        try {
	        assertTrue(iodevice.openMode().testFlag(QIODevice.OpenModeFlag.WriteOnly));
	        assertEquals(11, iodevice.write(data));
        }finally {
	        iodevice.close();
        	iodevice.dispose();
        }
        iodevice = QIODevice.fromInputStream(new ByteArrayInputStream(out.toByteArray()));
        try {
	        assertTrue(iodevice.openMode().testFlag(QIODevice.OpenModeFlag.ReadOnly));
	        QByteArray all = iodevice.readAll();
	        assertEquals("I am a girl", all.toString());
        }finally {
        	iodevice.dispose();
        }
    }

    @Test
    public void testIODevicePeek() {
    	QResource.addClassPath(".");
        // This was ":io/qt/autotests/TestInjectedCode.java" by it is not
        // normal to expect *.java files in the classpath.
        QFile qfile = new QFile(":io/qt/autotests/TestInjectedCode.dat");
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

        Byte b = file.getByte();
        assertEquals(null, b);
    }

    @Test
    public void testIODeviceGetByteSuccess() {
    	QResource.addClassPath(".");
        // This was ":io/qt/autotests/TestInjectedCode.java" by it is not
        // normal to expect *.java files in the classpath.
        QIODevice file = new QFile(":io/qt/autotests/TestInjectedCode.dat");

        assertTrue(file.open(QIODevice.OpenModeFlag.ReadOnly));

        file.seek(file.bytesAvailable() - 3); //instead of 4. On systems with 2 line end chars may fail

        Byte b = file.getByte();
        // if this fails check you have Unix end-of-line sequence (just <NL>) of your checked out copy of 
        //  TestInjectedCode.dat (file size should be 65963 bytes) as git can use your native EOL sequence
        //  and make this fail due to character offsets being wrong.
        assertEquals(Byte.valueOf((byte) 't'), b);

        b = file.getByte();
        assertEquals(Byte.valueOf((byte) '!'), b);

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
        QByteArray ba = new QByteArray("hello".getBytes());

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

        QByteArray ba2 = ba.insert(3, "gefyl".getBytes());
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
    public void testByteArray() {
		io.qt.core.QByteArray bv = new io.qt.core.QByteArray("Byte\0Array");
		assertArrayEquals("Byte\0Array".getBytes(), bv.toArray());
		assertEquals("Byte\0Array", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArray("Byte\0Array".getBytes());
		assertArrayEquals("Byte\0Array".getBytes(), bv.toArray());
		assertEquals("Byte\0Array", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArray(ByteBuffer.wrap("Byte\0Array".getBytes()));
		assertArrayEquals("Byte\0Array".getBytes(), bv.toArray());
		assertEquals("Byte\0Array", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArray("ByteArray".getBytes(), 2, 4);
		assertEquals(new io.qt.core.QByteArray("teAr"), bv);
		assertEquals("teAr", bv.toString());
		assertEquals("teAr", new String(bv.toArray()));
		bv.dispose();
		bv = new io.qt.core.QByteArray(ByteBuffer.wrap("ByteArray".getBytes(), 2, 4));
		assertEquals(new io.qt.core.QByteArray("teAr"), bv);
		assertEquals("teAr", bv.toString());
		assertEquals("teAr", new String(bv.toArray()));
		bv.dispose();
	}
    
    @Test
    public void testByteArrayBuffers() {
    	io.qt.core.QByteArray bv = new io.qt.core.QByteArray();
    	bv.fill((byte)'X', 8);
    	assertEquals("XXXXXXXX", bv.toString());
    	assertEquals(8, bv.size());
    	ByteBuffer rbuffer = bv.data();
    	try{
    		rbuffer.put((byte)'A');
    		Assert.fail("ReadOnlyBufferException expected to be thrown");
    	} catch (ReadOnlyBufferException e) {
		}
    	assertEquals((byte)'X', rbuffer.get());
    	ByteBuffer wbuffer = General.internalAccess.mutableData(bv);
//    	assertEquals(bv.size(), buffer.limit());
    	assertEquals(bv.capacity(), wbuffer.capacity());
    	wbuffer.put((byte)'A');
    	assertEquals("AXXXXXXX", bv.toString());
    	bv.dispose();
    	try {
			wbuffer.put((byte)'M');
			Assert.fail("BufferOverflowException expected to be thrown");
		} catch (BufferOverflowException e) {
		}
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
            QLocale locale = new QLocale(QLocale.Language.NorwegianBokmal);

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
        QLocale locale = new QLocale(QLocale.Language.NorwegianBokmal, QLocale.Country.Norway);

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
    public void testQDate_WeekNumber() {
        QDate date = new QDate(2000, 1, 1);

        QDate.Week week = date.weekNumber();
        assertEquals(52, week.weekNumber);

        assertEquals(1999, week.yearNumber);

        date.setDate(2002, 12, 31);
        week = date.weekNumber();
        assertEquals(1, week.weekNumber);
        assertEquals(2003, week.yearNumber);
    }

    @Test
    public void testOperatorAssignOtherTypeTemplate() {
//        QDir in = new QDir(":io/qt/");
//        QDir other = new QDir(":io/qt/examples/");
//        assertFalse(other.equals(in));
//
//        String out = ":io/qt/examples/";
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
    
    @Test
    public void testHashQMatrix2x2() {
    	QMatrix2x2 value = new QMatrix2x2();
    	value.fill(2);
    	assertFalse(0==value.hashCode());
    	QMatrix2x2 value2 = new QMatrix2x2();
    	value2.fill(2);
    	assertEquals(value2.hashCode(), value.hashCode());
    	QMatrix2x2 value3 = new QMatrix2x2();
    	value3.fill(3);
    	assertFalse(value3.hashCode()==value.hashCode());
    }
    
    @Test
    public void testHashQMatrix4x3() {
    	QMatrix4x3 value = new QMatrix4x3();
    	value.fill(2);
    	assertFalse(0==value.hashCode());
    	QMatrix4x3 value2 = new QMatrix4x3();
    	value2.fill(2);
    	assertEquals(value2.hashCode(), value.hashCode());
    	QMatrix4x3 value3 = new QMatrix4x3();
    	value3.fill(3);
    	assertFalse(value3.hashCode()==value.hashCode());
    }
    
    @Test
    public void testHashQPainterPath() {
    	QPainterPath value = new QPainterPath();
    	value.moveTo(2,9);
    	value.lineTo(66,45);
    	assertFalse(0==value.hashCode());
    	QPainterPath value2 = new QPainterPath();
    	value2.moveTo(2,9);
    	value2.lineTo(66,45);
    	assertEquals(value2.hashCode(), value.hashCode());
    	QPainterPath value3 = new QPainterPath();
    	value3.moveTo(21,64);
    	value3.lineTo(66,45);
    	assertFalse(value3.hashCode()==value.hashCode());
    	assertFalse(value3.elementAt(0).hashCode()==value.elementAt(0).hashCode());
    	assertEquals(value3.elementAt(1).hashCode(), value.elementAt(1).hashCode());
    }
    
    @Test
    public void testHashQAccessibleState() {
    	QAccessible.State value = new QAccessible.State();
    	value.setExtSelectable(true);
    	assertFalse(0==value.hashCode());
    	QAccessible.State value2 = new QAccessible.State();
    	value2.setExtSelectable(true);
    	assertEquals(value2.hashCode(), value.hashCode());
    	QAccessible.State value3 = new QAccessible.State();
    	value3.setSelfVoicing(true);
    	assertFalse(value3.hashCode()==value.hashCode());
    }
    
    @Test
    public void testQAbstractGraphicsShapeItem_isBlockedByModalPanel() {
    	QGraphicsItem.BlockedByModalPanelInfo info = new QGraphicsEllipseItem(1, 2, 3, 4).isBlockedByModalPanel();
    	Assert.assertTrue(info!=null);
    }
    
    @Test
    public void testQGraphicsPixmapItem_isBlockedByModalPanel() {
    	QGraphicsItem.BlockedByModalPanelInfo info = new QGraphicsPixmapItem(new QPixmap(100, 100)).isBlockedByModalPanel();
    	Assert.assertTrue(info!=null);
    }
    
    @Test
    public void testQGraphicsLineItem_isBlockedByModalPanel() {
    	QGraphicsItem.BlockedByModalPanelInfo info = new QGraphicsLineItem().isBlockedByModalPanel();
    	Assert.assertTrue(info!=null);
    }
    
    @Test
    public void testQGraphicsGroup_isBlockedByModalPanel() {
    	QGraphicsItem.BlockedByModalPanelInfo info = new QGraphicsItemGroup().isBlockedByModalPanel();
    	Assert.assertTrue(info!=null);
    }
    
    @Test
    public void testQGraphicsItem_isBlockedByModalPanel() {
    	QGraphicsItem item = new QGraphicsItem.Impl() {
			@Override
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
			
			@Override
			public QRectF boundingRect() {
				return new QRectF(1,2,3,4);
			}
		};
    	QGraphicsItem.BlockedByModalPanelInfo info = item.isBlockedByModalPanel();
    	Assert.assertTrue(info!=null);
    }
    
    @Test
    public void testQSplitter_getRange() {
    	QSplitter edit = new QSplitter();
    	QSplitter.Range range = edit.getRange(0);
    	Assert.assertTrue(range!=null);
    }
    
    @Test
    public void testActionMenu() {
    	QAction action = new QAction((QObject)null);
    	action.setMenu(null);
    	action.menu();
    }
    
    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestInjectedCode.class.getName());
    }
}

// next line is part of test.. leav it
//test!
