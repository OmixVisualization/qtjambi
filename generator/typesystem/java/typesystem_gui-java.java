/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
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
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package generator;

import org.qtjambi.qt.QNativePointer;
import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.Utilities;
import org.qtjambi.qt.internal.QtJambiObject.QPrivateConstructor;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.gui.*;

class QAbstractTextDocumentLayout___ extends QAbstractTextDocumentLayout {

        // Injected methods
        public final QTextObjectInterface handlerForObject(int objectType) {
            org.qtjambi.qt.GeneratorUtilities.threadCheck(this);
            if (nativeId() == 0)
                throw new QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
            return __qt_handlerForObject(nativeId(), objectType);
        }
        native QTextObjectInterface __qt_handlerForObject(long __this__nativeId, int objectType);

        /**
         * Registers the given component as a handler for items of the given objectType.
         * Note: registerHandler() has to be called once for each object type. This means that there is only one handler for multiple replacement characters of the same object type.
         *
         * @param objectType The type for which to register a handler.
         * @param component The handler for the given type.
         **/
        @SuppressWarnings("deprecated")
        public final void registerHandler(int objectType, QTextObjectInterface component) {
            registerHandler(objectType, (org.qtjambi.qt.core.QObject) component);
        }

}// class

class QBitmap___ extends QBitmap {

        public QBitmap(String fileName, String format) {
            this(fileName, format == null ? null : org.qtjambi.qt.QNativePointer.createCharPointer(format));
        }

        public QBitmap(String fileName) {
            this(fileName, (String) null);
        }

        public static QBitmap fromData(org.qtjambi.qt.core.QSize size, byte bits[], QImage.Format monoFormat) {
            return fromData(size, org.qtjambi.qt.internal.QtJambiInternal.byteArrayToNativePointer(bits), monoFormat);
        }

        public static QBitmap fromData(org.qtjambi.qt.core.QSize size, byte bits[]) {
            return fromData(size, bits, QImage.Format.Format_MonoLSB);
        }
}// class

class QBrush___ extends QBrush {


}// class

class QClipboard___ extends QClipboard {

        public static class Text {
                public String text;
                public String subtype;
        }

        public final Text text(String subtype, Mode mode) {
            QNativePointer np = new QNativePointer(QNativePointer.Type.String);
            np.setStringValue(subtype != null ? subtype : "");

            Text returned = new Text();
            returned.text = text(np, mode);
            returned.subtype = np.stringValue();
            return returned;
        }

        public final Text text(String subtype) {
            return text(subtype, Mode.Clipboard);
        }

}// class

class QColor___ extends QColor {

}// class

class QDesktopServices___ extends QDesktopServices {

        private static java.util.Hashtable<String, InternalUrlHandler> __rcUrlHandlers = new java.util.Hashtable<String, InternalUrlHandler>();

        public static interface UrlHandler {
            public void handleUrl(org.qtjambi.qt.core.QUrl url);
        }

        private static class InternalUrlHandler extends org.qtjambi.qt.core.QObject {

                private UrlHandler urlHandler;

                private InternalUrlHandler(UrlHandler urlHandler) {
                    this.urlHandler = urlHandler;
                }

                private void handleUrl(org.qtjambi.qt.core.QUrl url) {
                    urlHandler.handleUrl(url);
                }

        }

        public static void setUrlHandler(String scheme, UrlHandler urlHandler) {
            InternalUrlHandler receiver = urlHandler != null ? new InternalUrlHandler(urlHandler) : null;

            if (receiver == null)
                __rcUrlHandlers.remove(scheme);
            else
                __rcUrlHandlers.put(scheme, receiver);
            setUrlHandler(scheme, receiver, QNativePointer.createCharPointer("handleUrl"));
        }

}// class

class QGuiApplication___ extends QGuiApplication {

        public static void initialize(String args[]) {
            if (m_instance != null)
                throw new RuntimeException("QGuiApplication can only be initialized once");

            org.qtjambi.qt.internal.HelperFunctions.setAsMainThread();

            List<String> paths = Utilities.unpackPlugins();
            if (paths != null) {
                Collections.reverse(paths);  // Qt prepends but our list is in highest priority first order
                for (String p : paths)
                    addLibraryPath(p);
            }
            org.qtjambi.qt.internal.QtJambiInternal.setupDefaultPluginPath();
            m_instance = new QGuiApplication(args);
            m_instance.aboutToQuit.connect(m_instance, "disposeOfMyself()");
        }

        public static void initialize(String applicationName, String args[]) {
            if (m_instance != null)
                throw new RuntimeException("QGuiApplication can only be initialized once");

            org.qtjambi.qt.internal.HelperFunctions.setAsMainThread();

            List<String> paths = Utilities.unpackPlugins();
            if (paths != null) {
                Collections.reverse(paths);  // Qt prepends but our list is in highest priority first order
                for (String p : paths)
                    addLibraryPath(p);
            }
            org.qtjambi.qt.internal.QtJambiInternal.setupDefaultPluginPath();
            m_instance = new QGuiApplication(applicationName, args);
            m_instance.aboutToQuit.connect(m_instance, "disposeOfMyself()");
        }

        public static void shutdown() {
            org.qtjambi.qt.core.QCoreApplication.shutdown();
        }
        
        /**
         *
         * @see #execStatic()
         */
        public int exec() {
            return exec_internal();
        }

        /**
         *
         * @see #exec()
         */
        public static int execStatic() {
            if (m_instance == null)
                throw new RuntimeException("QGuiApplication has not been initialized with QGuiApplication.initialize()");
            return exec_internal();
        }

        public static QGuiApplication instance() {
            if (m_instance instanceof QGuiApplication)
                return (QGuiApplication) m_instance;
            return null;
        }

        public QGuiApplication(String args[]) {
            this(argc(args), argv(args));
        }

        public QGuiApplication(String applicationName, String args[]) {
            this(argc(args), argv(applicationName, args));
        }

        public static QCursor overrideCursor() {
            QNativePointer np = overrideCursor_private();
            return np == null ? null : QCursor.fromNativePointer(np);
        }

}// class

class QImage___ extends QImage {
        public QImage(String xpm[]) {
            this(org.qtjambi.qt.QNativePointer.createCharPointerPointer(xpm));
        }

        public final byte[] copyOfBytes() {
            QNativePointer bits = bits();
            byte bytes[] = new byte[byteCount()];
            for (int i = 0; i < bytes.length; ++i)
                bytes[i] = bits.byteAt(i);
            return bytes;
        }

        public QImage(byte data[], int width, int height, Format format) {
            this(org.qtjambi.qt.internal.QtJambiInternal.byteArrayToNativePointer(data), width, height, format);
        }

        public QImage(String fileName, String format) {
            this(fileName, format == null ? null : QNativePointer.createCharPointer(format));
        }

        public QImage(String fileName) {
            this(fileName, (String) null);
        }

	private QImage(org.qtjambi.qt.QNativePointer data, int width, int height, org.qtjambi.qt.gui.QImage.Format format) {
		super((QPrivateConstructor)null);
		__qt_QImage_nativepointer_int_int_Format(data, width, height, format.value());
		org.qtjambi.qt.GeneratorUtilities.countExpense(org.qtjambi.qt.gui.QImage.class, height()*bytesPerLine(), 67108864);
	}

	native void __qt_QImage_nativepointer_int_int_Format(org.qtjambi.qt.QNativePointer data, int width, int height, int format);
}// class

class QImage::JNI{

// QImage::QImage(unsigned char * data, int width, int height, QImage::Format format)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_org_qtjambi_qt_gui_QImage__1_1qt_1QImage_1nativepointer_1int_1int_1Format__Lorg_qtjambi_qt_QNativePointer_2III)
(JNIEnv *env,
 jobject java_object,
 jobject data,
 jint width,
 jint height,
 jint format)
{
    QTJAMBI_DEBUG_TRACE("(native) entering: QImage::QImage(unsigned char * data, int width, int height, QImage::Format format)");
    uchar*  qt_data = (uchar* ) qtjambi_to_cpointer(env, data, 1);
    QTJAMBI_EXCEPTION_CHECK(env);
    QImage::Format __qt_format = (QImage::Format) format;
    QTJAMBI_EXCEPTION_CHECK(env);
    QtJambiShell_QImage *qt_this = new QtJambiShell_QImage((uchar* )qt_data, (int )width, (int )height, (QImage::Format )__qt_format);
    QtJambiLink *qt_java_link = qtjambi_construct_object(env, java_object, qt_this, "QImage");
    if (!qt_java_link) {
        qWarning("object construction failed for type: QImage");
        return;
    }
    qt_java_link->setJavaOwnership(env, java_object);
    qt_this->m_link = qt_java_link;
    qt_this->m_link->setCreatedByJava(true);
    qt_this->m_vtable = qtjambi_setup_vtable(env, 
                        java_object, 
                        0, 0, 0, // no inconsistent functions
                        qtjambi_method_count, 
                        qtjambi_method_names, 
                        qtjambi_method_signatures
                        );
    QTJAMBI_DEBUG_TRACE("(native) -> leaving: QImage::QImage(unsigned char * data, int width, int height, QImage::Format format)");

}

QtJambiShell_QImage::QtJambiShell_QImage(uchar*  data, int  width, int  height, QImage::Format  format): QImage(data, width, height, format),
      m_vtable(0),
      m_link(0)
{
    QTJAMBI_DEBUG_TRACE("(shell) entering: QtJambiShell_QImage::QtJambiShell_QImage::QtJambiShell_QImage(uchar*  data, int  width, int  height, QImage::Format  format)");
    QTJAMBI_DEBUG_TRACE("(shell) leaving: QtJambiShell_QImage::QtJambiShell_QImage::QtJambiShell_QImage(uchar*  data, int  width, int  height, QImage::Format  format)");
}

}// class

class QImage::Shell{

    QtJambiShell_QImage(uchar*  data, int  width, int  height, QImage::Format  format);

}// class

class QKeySequence___ extends QKeySequence {

        @QtBlockedSlot
        public final int at(int i) {
            return operator_subscript(i);
        }

}// class

class QPainter___ extends QPainter {

        public final void setBrush(QColor color) {
            setBrush(new QBrush(color));
        }

        public final void setBrush(QGradient gradient) {
            setBrush(new QBrush(gradient));
        }

        public final void setBrush(QPixmap pm) {
            setBrush(new QBrush(pm));
        }

        public static QPaintDeviceInterface redirected(QPaintDeviceInterface device, org.qtjambi.qt.core.QPoint offset) {
            return redirected(device, offset == null ? null : offset.nativePointer());
        }

/*        public QPainter(QPaintDeviceInterface device) {
            this();
            begin(widget);
        }

        public boolean begin(QPaintDeviceInterface device) { // can't be final because of QStylePainter
            return org.qtjambi.qt.QtJambiGuiInternal.beginPaint(device, this);
        }
*/
        private static java.util.Stack<QPaintDeviceInterface> __rcRedirections = new java.util.Stack<QPaintDeviceInterface>();

}// class

class QPicture___ extends QPicture {

        public final boolean load(QIODevice dev) {
            return load(dev, (QNativePointer) null);
        }

        public final boolean load(String fileName) {
            return load(fileName, (org.qtjambi.qt.QNativePointer) null);
        }

        public final boolean save(QIODevice dev) {
            return save(dev, (org.qtjambi.qt.QNativePointer) null);
        }

        public final boolean save(String fileName) {
            return save(fileName, (org.qtjambi.qt.QNativePointer) null);
        }

        public final byte[] data() {
            QNativePointer npData = data_private();
            if (npData == null)
                return null;
            byte returned[] = new byte[size()];
            for (int i = 0; i < returned.length; ++i)
                returned[i] = npData.byteAt(i);
            return returned;
        }

}// class

class QRegion___ extends QRegion {

        public void setRects(org.qtjambi.qt.core.QRect[] rects) {
            setRects(org.qtjambi.qt.core.QRect.nativePointerArray(rects), rects.length);
        }

}// class

class QPen___ extends QPen {
        public QPen(QColor color, double width, org.qtjambi.qt.core.Qt.PenStyle s, org.qtjambi.qt.core.Qt.PenCapStyle c, org.qtjambi.qt.core.Qt.PenJoinStyle j) {
            this(new QBrush(color), width, s, c, j);
        }

        public QPen(QColor color, double width, org.qtjambi.qt.core.Qt.PenStyle s, org.qtjambi.qt.core.Qt.PenCapStyle c) {
            this(new QBrush(color), width, s, c);
        }

        public QPen(QColor color, double width, org.qtjambi.qt.core.Qt.PenStyle s) {
            this(new QBrush(color), width, s);
        }

        public QPen(QColor color, double width) {
            this(new QBrush(color), width);
        }

//        public static final QPen NoPen = new QPen(org.qtjambi.qt.core.Qt.PenStyle.NoPen);
}// class

class QPixmap___ extends QPixmap {

        public QPixmap(String xpm[]) {
            this(org.qtjambi.qt.QNativePointer.createCharPointerPointer(xpm));
        }
		
		@QtBlockedSlot
		public final QRegion scroll(int dx, int dy, int x, int y, int width, int height) {
			QRegion region = new QRegion();
			scroll(dx, dy, x, y, width, height, region.nativePointer());
			return region;
		}
		
		@QtBlockedSlot
		public final QRegion scroll(int dx, int dy, org.qtjambi.qt.core.QRect rect) {
			QRegion region = new QRegion();
			scroll(dx, dy, rect, region.nativePointer());
			return region;
		}
}// class

class QPixmapFilter___ extends QPixmapFilter {
        public final void setConvolutionKernel(double[] kernel, int rows, int columns) {
            int length = rows * columns;

            QNativePointer kernelPtr = new QNativePointer(QNativePointer.Type.double, length);
            for (int i = 0; i < length; ++i)
                kernelPtr.setdoubleAt(i, kernel[i]);

            setConvolutionKernel_private(kernelPtr, rows, columns);
        }
}// class

class QPixmapCache___ extends QPixmapCache {

        public static QPixmap find(String key) {
			QPixmap pixmap = new QPixmap();
			if(find(key, pixmap.nativePointer())){
				return pixmap;
			}else{
				return null;
			}
        }

		public static QPixmap find(org.qtjambi.qt.gui.QPixmapCache.Key key) {
			QPixmap pixmap = new QPixmap();
			if(find(key, pixmap.nativePointer())){
				return pixmap;
			}else{
				return null;
			}
		}
		
}// class

class QTextCursor___ extends QTextCursor {
        public final QTableArea selectedTableCells() {
            QNativePointer firstRow = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer numRows = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer firstColumn = new QNativePointer(QNativePointer.Type.Int);
            QNativePointer numColumns = new QNativePointer(QNativePointer.Type.Int);

            selectedTableCells(firstRow, numRows, firstColumn, numColumns);

            return new QTableArea(firstRow.intValue(), firstColumn.intValue(), numRows.intValue(), numColumns.intValue());
        }

}// class

class QTextLine___ extends QTextLine {

        public final void draw(QPainter painter, org.qtjambi.qt.core.QPointF position) {
            draw(painter, position, null);
        }

}// class

class QTextFrame_iterator___ extends QTextFrame_iterator {

        @QtBlockedSlot
        public final void next() {
            operator_increment();
        }

        @QtBlockedSlot
        public final void previous() {
            operator_decrement();
        }

}// class

class QTextBlock_iterator___ extends QTextBlock_iterator {

        @QtBlockedSlot
        public final void next() {
            operator_increment();
        }

        @QtBlockedSlot
        public final void previous() {
            operator_decrement();
        }

}// class

class QTextDocument___ extends QTextDocument {

        public final void redo(QTextCursor cursor) {
            redo(cursor.nativePointer());
        }

        public final void undo(QTextCursor cursor) {
            undo(cursor.nativePointer());
        }

}// class

class QPolygon___ extends QPolygon {

        private native void add_private(long nid, int x, int y);

        @QtBlockedSlot
        public final QPolygon add(int x, int y) {
            add_private(nativeId(), x, y);
            return this;
        }

        @QtBlockedSlot
        public final QPolygon add(QPoint pt) {
            add_private(nativeId(), pt.x(), pt.y());
            return this;
        }

        @QtBlockedSlot
        public final QPolygon add(QPolygon p) {
            int size = p.size();
            long nid = nativeId();
            for (int i = 0; i < size; ++i) {
                QPoint pt = p.at(i);
                add_private(nid, pt.x(), pt.y());
            }
            return this;
        }
}// class

class QPolygonF___ extends QPolygonF {
        private native void add_private(long nid, double x, double y);

        @QtBlockedSlot
        public final QPolygonF add(double x, double y) {
            add_private(nativeId(), x, y);
            return this;
        }

        @QtBlockedSlot
        public final QPolygonF add(QPointF pt) {
            add_private(nativeId(), pt.x(), pt.y());
            return this;
        }

        @QtBlockedSlot
        public final QPolygonF add(QPolygonF p) {
            int size = p.size();
            long nid = nativeId();
            for (int i = 0; i < size; ++i) {
                QPointF pt = p.at(i);
                add_private(nid, pt.x(), pt.y());
            }
            return this;
        }
}// class

class QTransform___ extends QTransform {

        public final QTransform multiply(double d) {
            operator_multiply_assign(d);
            return this;
        }

        public final QTransform multiply(QTransform matrix) {
            operator_multiply_assign(matrix);
            return this;
        }

        public final QTransform add(double d) {
            operator_add_assign(d);
            return this;
        }

        public final QTransform divide(double d) {
            operator_divide_assign(d);
            return this;
        }

        public final QTransform subtract(double d) {
            operator_subtract_assign(d);
            return this;
        }

        /**
         * Returns an inverted copy of this transformation.
         *
         * @return The inverse of the transformation.
         * @throws IllegalArgumentException
         *             If this transformation is not invertible.
         */
        public final QTransform inverted() {
            QNativePointer ok = new QNativePointer(QNativePointer.Type.Boolean);
            QTransform returned = inverted(ok);
            if (!ok.booleanValue())
                throw new IllegalArgumentException("Transformation is not invertible");
            return returned;
        }

        /**
         * Creates a transformation mapping one arbitrary quad into another.
         *
         * @return The transformation.
         * @throws IllegalArgumentException
         *             If this transformation is not possible.
         */
        public static final QTransform quadToQuad(QPolygonF from, QPolygonF to) {
            QTransform res = new QTransform();
            QNativePointer resPointer = res.nativePointer();
            if (quadToQuadPrivate(from, to, resPointer)) {
                return res;
            } else
                throw new IllegalArgumentException("Transformation is not possible");
        }

        /**
         * Creates a transformation that maps a quad to a unit square.
         *
         * @return The transformation.
         * @throws IllegalArgumentException If this transformation is not possible.
         */
        public static final QTransform quadToSquare(QPolygonF quad) {
            QTransform res = new QTransform();
            QNativePointer resPointer = res.nativePointer();
            if (quadToSquarePrivate(quad, resPointer)) {
                return res;
            } else
                throw new IllegalArgumentException("Transformation is not possible");
        }

        /**
         * Creates a transformation that maps a unit square to a the given quad.
         *
         * @return The transformation.
         * @throws IllegalArgumentException
         *             If this transformation is not possible.
         */
        public static final QTransform squareToQuad(QPolygonF quad) {
            QTransform res = new QTransform();
            QNativePointer resPointer = res.nativePointer();
            if (squareToQuadPrivate(quad, resPointer)) {
                return res;
            } else
                throw new IllegalArgumentException("Transformation is not possible");
        }
}// class

class QValidator___ extends QValidator {

        public static class QValidationData {
                public QValidationData(String input, int pos) {
                    string = input;
                    position = pos;
                }

                public String string;
                public int position;
        }

}// class

class QOpenGLShaderProgram___ extends QGLColormap {
		
		@QtBlockedSlot
		public final void setAttributeValue(java.lang.String name, float[] values, int columns, int rows)    {
			this.setAttributeValue_GLfloat(name, floatsToNativePointer(values), columns, rows);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, float[] values, int tupleSize) {
			setAttributeArray(location, values, tupleSize, (int)0);
		}
		@QtBlockedSlot
		public final void setAttributeArray(int location, float[] values, int tupleSize, int stride) {
			setAttributeArray_GLfloat(location, floatsToNativePointer(values), tupleSize, stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, float[] values, int tupleSize) {
			setAttributeArray(name, values, tupleSize, (int)0);
		}
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, float[] values, int tupleSize, int stride) {
			setAttributeArray_GLfloat(name, floatsToNativePointer(values), tupleSize, stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector2D[] values)    {
			this.setAttributeArray(location, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector2D[] values, int stride)    {
			setAttributeArray_QVector2D(location, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector2D[] values)    {
			this.setAttributeArray(name, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector2D[] values, int stride)    {
			setAttributeArray_QVector2D(name, arrayToNativePointer(values), stride);
		}
    
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector3D[] values)    {
			this.setAttributeArray(location, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector3D[] values, int stride)    {
			setAttributeArray_QVector3D(location, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector4D[] values)    {
			this.setAttributeArray(location, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(int location, org.qtjambi.qt.gui.QVector4D[] values, int stride)    {
			setAttributeArray_QVector4D(location, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector3D[] values)    {
			this.setAttributeArray(name, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector3D[] values, int stride)    {
			setAttributeArray_QVector3D(name, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector4D[] values)    {
			this.setAttributeArray(name, values, 0);
		}
		
		@QtBlockedSlot
		public final void setAttributeArray(java.lang.String name, org.qtjambi.qt.gui.QVector4D[] values, int stride)    {
			setAttributeArray_QVector4D(name, arrayToNativePointer(values), stride);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, float[] values, int count, int tupleSize)    {
			this.setUniformValueArray_GLfloat(location, this.floatsToNativePointer(values), count, tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, float[] values, int count, int tupleSize)    {
			this.setUniformValueArray_GLfloat(name, this.floatsToNativePointer(values), count, tupleSize);
		}
    
		@QtBlockedSlot
		public final void setUniformValueArray(int location, int[] values, int tupleSize)    {
			this.setUniformValueArray_int(location, org.qtjambi.qt.internal.QtJambiInternal.intArrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, int[] values, int tupleSize)    {
			this.setUniformValueArray_int(name, org.qtjambi.qt.internal.QtJambiInternal.intArrayToNativePointer(values), tupleSize);
		}
		
/*		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix2x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x2(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix2x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x2(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix2x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x3(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix2x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x3(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix2x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x4(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix2x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix2x4(name, arrayToNativePointer(values), tupleSize);
		}
    
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix3x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x2(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix3x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x2(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix3x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x3(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix3x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x3(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix3x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x4(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix3x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix3x4(name, arrayToNativePointer(values), tupleSize);
		}
    
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix4x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x2(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix4x2[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x2(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix4x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x3(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix4x3[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x3(name, arrayToNativePointer(values), tupleSize);
		}
*/		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QMatrix4x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x4(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QMatrix4x4[] values, int tupleSize)    {
			this.setUniformValueArray_QMatrix4x4(name, arrayToNativePointer(values), tupleSize);
		}
    
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QVector2D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector2D(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QVector2D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector2D(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QVector3D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector3D(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QVector3D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector3D(name, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(int location, org.qtjambi.qt.gui.QVector4D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector4D(location, arrayToNativePointer(values), tupleSize);
		}
		
		@QtBlockedSlot
		public final void setUniformValueArray(String name, org.qtjambi.qt.gui.QVector4D[] values, int tupleSize)    {
			this.setUniformValueArray_QVector4D(name, arrayToNativePointer(values), tupleSize);
		}
		
		private org.qtjambi.qt.QNativePointer floatsToNativePointer(float[] values){
			org.qtjambi.qt.QNativePointer pointer = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Float, values.length);
			for (int i = 0; i < values.length; i++) {
				pointer.setFloatAt(i, values[i]);
			}
			return pointer;
		}
		
		private <T extends org.qtjambi.qt.internal.QtJambiObject> org.qtjambi.qt.QNativePointer arrayToNativePointer(T[] values){
			org.qtjambi.qt.QNativePointer pointer = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Pointer, values.length);
			for (int i = 0; i < values.length; i++) {
				pointer.setPointerAt(i, values[i].nativePointer());
			}
			return pointer;
		}
		
}// class

class QAccessibleTableInterface___ extends QAccessibleTableInterface {

        public static class CellAtIndex extends QTableArea {
                public CellAtIndex(int row, int column, int rowSpan, int columnSpan, boolean isSelected) {
                    super(row, column, rowSpan, columnSpan);
                    this.isSelected = isSelected;
                }

                public boolean isSelected;
        }

}// class

class QAccessibleInterface___ extends QAccessibleInterface {

        public static class Target {
                public Target(int childIndex, QAccessibleInterface target) {
                    this.childIndex = childIndex;
                    this.target = target;
                }

                public QAccessibleInterface target;
                public int childIndex;
        }

}// class

class QMatrix___ extends QMatrix {
        /**
         * Returns an inverted copy of this matrix.
         *
         * @return The inverse of the matrix.
         * @throws IllegalArgumentException
         *             If this matrix is not invertible.
         */
        public final QMatrix inverted() {
            QNativePointer ok = new QNativePointer(QNativePointer.Type.Boolean);
            QMatrix returned = inverted(ok);
            if (!ok.booleanValue())
                throw new IllegalArgumentException("Matrix is not invertible");
            return returned;
        }

        @QtBlockedSlot
        public final QMatrix multiply(QMatrix other) {
            operator_multiply_assign(other);
            return this;
        }

        @QtBlockedSlot
        public final QMatrix multiplied(QMatrix other) {
            return operator_multiply(other);
        }
}// class

class QMatrix4x4___ extends QMatrix4x4 {
	@QtBlockedSlot
	public final org.qtjambi.qt.gui.QMatrix4x4 inverted() {
		org.qtjambi.qt.QNativePointer pointer = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Boolean);
		pointer.setBooleanValue(false);
		QMatrix4x4 result = inverted(pointer);
		if(!pointer.booleanValue()){
			result = null;
		}
		return result;
	}
}// class

class QPaintDeviceWindow___ extends QPaintDeviceWindow {
}// class

class QQuaternion___ extends QQuaternion {
	public static class Axes{
		public Axes(QVector3D xAxis, QVector3D yAxis, QVector3D zAxis) {
			super();
			this.xAxis = xAxis;
			this.yAxis = yAxis;
			this.zAxis = zAxis;
		}
		public final QVector3D xAxis;
		public final QVector3D yAxis;
		public final QVector3D zAxis;
	}
	
	public static class AxisAndAngle{
		public AxisAndAngle(QVector3D axis, float angle) {
			super();
			this.axis = axis;
			this.angle = angle;
		}
		public final QVector3D axis;
		public final float angle;
	}
	
	public static class EulerAngles{
		public EulerAngles(float pitch, float yaw, float roll) {
			super();
			this.pitch = pitch;
			this.yaw = yaw;
			this.roll = roll;
		}
		public final float pitch;
		public final float yaw;
		public final float roll;
	}
	
	@QtBlockedSlot
	public final Axes getAxes(){
		QVector3D xAxis = new QVector3D();
		QVector3D yAxis = new QVector3D();
		QVector3D zAxis = new QVector3D();
		getAxes(xAxis.nativePointer(), yAxis.nativePointer(), zAxis.nativePointer());
		return new Axes(xAxis, yAxis, zAxis);
	}
	
	@QtBlockedSlot
	public final AxisAndAngle getAxisAndAngle()    {
		QVector3D axis = new QVector3D();
		org.qtjambi.qt.QNativePointer angle = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Float);
		getAxisAndAngle(axis.nativePointer(), angle);
		return new AxisAndAngle(axis, angle.floatValue());
	}
	
	@QtBlockedSlot
	public final EulerAngles getEulerAngles()    {
		org.qtjambi.qt.QNativePointer pitch = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Float);
		org.qtjambi.qt.QNativePointer yaw = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Float);
		org.qtjambi.qt.QNativePointer roll = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Float);
		getEulerAngles(pitch, yaw, roll);
		return new EulerAngles(pitch.floatValue(), yaw.floatValue(), roll.floatValue());
	}
}// class


