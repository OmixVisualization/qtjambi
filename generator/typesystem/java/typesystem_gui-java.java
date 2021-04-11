/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.*;
import io.qt.internal.QtJambiObject.QPrivateConstructor;
import io.qt.core.QCoreApplication;
import io.qt.core.QPoint;
import io.qt.core.QPointF;
import io.qt.gui.*;

class QAbstractTextDocumentLayout___ extends QAbstractTextDocumentLayout {
    /**
     * Registers the given component as a handler for items of the given objectType.
     * Note: registerHandler() has to be called once for each object type. This means that there is only one handler for multiple replacement characters of the same object type.
     *
     * @param objectType The type for which to register a handler.
     * @param component The handler for the given type.
     **/
    public final <Handler extends io.qt.core.QObject & QTextObjectInterface> void registerHandler(int objectType, Handler component) {
        _registerHandler(objectType, component);
    }
    
    public final <Handler extends io.qt.core.QObject & QTextObjectInterface> void unregisterHandler(int objectType, Handler component) {
        _unregisterHandler(objectType, component);
    }
}// class

class QBrush___ extends QBrush {


}// class

class QClipboard___ extends QClipboard {

        public final static class Text {
            private Text(String text, String subtype) {
                super();
                this.text = text;
                this.subtype = subtype;
            }
            public final String text;
            public final String subtype;
        }
}// class

class QColor___ extends QColor {

}// class

class QDesktopServices___ extends QDesktopServices {

        private static java.util.TreeMap<String, io.qt.core.QObject> __rcUrlHandlers = new java.util.TreeMap<String, io.qt.core.QObject>();

        public static interface UrlHandler {
            public void handleUrl(io.qt.core.QUrl url);
        }

        private static class InternalUrlHandler extends io.qt.core.QObject {

                private UrlHandler urlHandler;

                private InternalUrlHandler(UrlHandler urlHandler) {
                    this.urlHandler = urlHandler;
                }

                @io.qt.internal.NativeAccess
                public void handleUrl(io.qt.core.QUrl url) {
                    urlHandler.handleUrl(url);
                }

        }
        
        public static void setUrlHandler(String scheme, UrlHandler urlHandler) {
            if(urlHandler instanceof io.qt.core.QObject) {
                __rcUrlHandlers.put(scheme, (io.qt.core.QObject)urlHandler);
                setUrlHandler(scheme, (io.qt.core.QObject)urlHandler);
            }else {
                InternalUrlHandler receiver = urlHandler != null ? new InternalUrlHandler(urlHandler) : null;
        
                if (receiver == null)
                    __rcUrlHandlers.remove(scheme);
                else
                    __rcUrlHandlers.put(scheme, receiver);
                setUrlHandler(scheme, receiver);
            }
        }

}// class

class QGuiApplication___ extends QGuiApplication {

        public static QGuiApplication initialize(String args[]) {
            return io.qt.core.QCoreApplication.initialize(null, args, QGuiApplication::new);
        }

        public static QGuiApplication initialize(String applicationName, String args[]) {
            return io.qt.core.QCoreApplication.initialize(applicationName, args, QGuiApplication::new);
        }

        public static void shutdown() {
            io.qt.core.QCoreApplication.shutdown();
        }
        
        public static int exec() {
            io.qt.core.QCoreApplication instance = io.qt.core.QCoreApplication.instance();
            if (instance == null)
                throw new RuntimeException("QGuiApplication has not been initialized with QGuiApplication.initialize()");
            else if(instance.thread()!=io.qt.core.QThread.currentThread())
                throw new RuntimeException("exec() must be called from the main thread.");
            else if(io.qt.internal.QtJambiInternal.countEventLoops(instance.thread())>0)
                throw new RuntimeException("The event loop is already running.");
            else if(instance instanceof io.qt.widgets.QApplication)
                return io.qt.widgets.QApplication.exec();
            else if(instance instanceof io.qt.gui.QGuiApplication)
                return exec_internal();
            else
                return io.qt.core.QCoreApplication.exec();
        }
        
        public static QGuiApplication instance() {
            io.qt.core.QCoreApplication app = io.qt.core.QCoreApplication.instance();
            if (app instanceof QGuiApplication)
                return (QGuiApplication) app;
            return null;
        }
}// class

class QImage___ extends QImage {
        public QImage(byte data[], int width, int height, Format format) {
            __qt_QImage_new(this, data, width, height, format);
        }

        private static native void __qt_QImage_new(Object instance, byte data[], int width, int height, Format format);
        
        public QImage(java.nio.Buffer data, int width, int height, Format format) {
            __qt_QImage_new(this, data, width, height, format);
        }

        private static native void __qt_QImage_new(Object instance, java.nio.Buffer data, int width, int height, Format format);
}// class

class QImage::JNI{

void __qt_create_new_QImage_7(void* __qtjambi_ptr, JNIEnv* env, jobject, jvalue* arguments){
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QImage(unsigned char * data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)");
    jobject data = arguments[0].l;
    jint width = arguments[1].i;
    jint height = arguments[2].i;
    jobject format = arguments[3].l;
    JByteArrayPointer* qt_data = new JByteArrayPointer(env, jbyteArray(data), false);
    new(__qtjambi_ptr) QImage_shell(*qt_data, width, height, qtjambi_cast<QImage::Format>(env, format), [](void* ptr){ delete reinterpret_cast<JByteArrayPointer*>(ptr); }, qt_data);

}

void __qt_create_new_QImage_8(void* __qtjambi_ptr, JNIEnv* env, jobject, jvalue* arguments){
    QTJAMBI_DEBUG_METHOD_PRINT("native", "new QImage(unsigned char * data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)");
    jobject data = arguments[0].l;
    jint width = arguments[1].i;
    jint height = arguments[2].i;
    jobject format = arguments[3].l;
    JBufferConstData* qt_data = new JBufferConstData(env, data);
    new(__qtjambi_ptr) QImage_shell(*qt_data, width, height, qtjambi_cast<QImage::Format>(env, format), [](void* ptr){ delete reinterpret_cast<JBufferConstData*>(ptr); }, qt_data);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QImage__1_1qt_1QImage_1new__Ljava_lang_Object_2_3BIILio_qt_gui_QImage_00024Format_2)
(JNIEnv *env, jclass __jni_class, jobject java_object, jobject data, jint width, jint height, jobject format)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QImage::QImage(unsigned char * data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)");
    try{
        jvalue arguments[4];
        arguments[0].l = data;
        arguments[1].i = width;
        arguments[2].i = height;
        arguments[3].l = format;
        qtjambi_initialize_native_value(env, __jni_class, java_object, &__qt_create_new_QImage_7, sizeof(QImage_shell), typeid(QImage), true, arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

// QImage::QImage(unsigned char * data, int width, int height, QImage::Format format)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QImage__1_1qt_1QImage_1new__Ljava_lang_Object_2Ljava_nio_Buffer_2IILio_qt_gui_QImage_00024Format_2)
(JNIEnv *env, jclass __jni_class, jobject java_object, jobject data, jint width, jint height, jobject format)
{
    QTJAMBI_DEBUG_METHOD_PRINT("native", "QImage::QImage(unsigned char * data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)");
    try{
        jvalue arguments[4];
        arguments[0].l = data;
        arguments[1].i = width;
        arguments[2].i = height;
        arguments[3].l = format;
        qtjambi_initialize_native_value(env, __jni_class, java_object, &__qt_create_new_QImage_8, sizeof(QImage_shell), typeid(QImage), true, arguments);
    }catch(const JavaException& exn){
        exn.raiseInJava(env);
    }
}

}// class

class QPointerEvent___{

    @io.qt.QtUninvokable
    public final void setPoint(long i, QEventPoint point) {
        __qt_QPointerEvent_setPoint(nativeId(this), i, checkedNativeId(point));
    }
    @io.qt.QtUninvokable
    private native void __qt_QPointerEvent_setPoint(long __this__nativeId, long i, long pointId);

}// class

class QPointerEvent_native__{

}// class

class QKeySequence_5__ extends QKeySequence {

        @io.qt.QtUninvokable
        public final int at(int i) {
            return operator_subscript(i);
        }

}// class

class QKeySequence_6__ extends QKeySequence {

        @io.qt.QtUninvokable
        public final QKeyCombination at(int i) {
            return operator_subscript(i);
        }

}// class

class QPainter_redir__ extends QPainter {
    private static java.util.Map<QPaintDevice,QPaintDevice> __rcRedirections = new java.util.HashMap<>();
}// class

class QPainter___ extends QPainter {

    @io.qt.QtUninvokable
    public final void setBrush(QColor color) {
        setBrush(new QBrush(color));
    }

    @io.qt.QtUninvokable
    public final void setBrush(io.qt.core.Qt.GlobalColor color) {
        setBrush(new QBrush(new QColor(color)));
    }

    @io.qt.QtUninvokable
    public final void setBrush(QGradient gradient) {
        setBrush(new QBrush(gradient));
    }

    @io.qt.QtUninvokable
    public final void setBrush(QPixmap pm) {
        setBrush(new QBrush(pm));
    }

    @io.qt.QtUninvokable
    private static void beginPaint(QPaintDevice paintedWidget){
        __paintedDevices.put(paintedWidget, java.util.Collections.emptyList());
    }
    
    @io.qt.QtUninvokable
    private static void endPaint(QPaintDevice paintedWidget){
        java.util.List<QPainter> painters = __paintedDevices.remove(paintedWidget);
        if(painters!=null){
            for(QPainter painter : painters){
                if(!painter.isDisposed() && painter.isActive()){
                    painter.end();
                    painter.dispose();
                }
            }
        }
    }
    
    @io.qt.QtUninvokable
    private void initialize(QPaintDevice device, boolean inConstructor){
        if(device instanceof io.qt.widgets.QWidget || device instanceof QPaintDeviceWindow){
            java.util.List<QPainter> painters = __paintedDevices.get(device);
            if(painters==null){
                if(inConstructor)
                    throw new QPaintingOutsidePaintEventException();
            }else if(painters.size()==0){
                threadCheck(nativeId((io.qt.core.QObject)device));
                painters = java.util.Collections.singletonList(this);
                __paintedDevices.put(device, painters);
            }else{
                threadCheck(nativeId((io.qt.core.QObject)device));
                if(painters.size()==1){
                    painters = new java.util.LinkedList<>(painters);
                    __paintedDevices.put(device, painters);
                }
                painters.add(this);
            }
        }else if(device instanceof io.qt.core.QObject){
            threadCheck(nativeId((io.qt.core.QObject)device));
        }else{
            java.util.Objects.requireNonNull(device, "Argument 'QPaintDevice': null not expected.");
        }
    }
    
    @io.qt.QtUninvokable
    private static native void threadCheck(long objectId);
    
    private static java.util.Map<QPaintDevice,java.util.List<QPainter>> __paintedDevices = new java.util.HashMap<>();
}// class

class QPen___ extends QPen {
        public QPen(QColor color, double width, io.qt.core.Qt.PenStyle s, io.qt.core.Qt.PenCapStyle c, io.qt.core.Qt.PenJoinStyle j) {
            this(new QBrush(color), width, s, c, j);
        }

        public QPen(QColor color, double width, io.qt.core.Qt.PenStyle s, io.qt.core.Qt.PenCapStyle c) {
            this(new QBrush(color), width, s, c);
        }

        public QPen(QColor color, double width, io.qt.core.Qt.PenStyle s) {
            this(new QBrush(color), width, s);
        }

        public QPen(QColor color, double width) {
            this(new QBrush(color), width);
        }

//        public static final QPen NoPen = new QPen(io.qt.core.Qt.PenStyle.NoPen);
}// class

class QPixmap___ extends QPixmap {
         
}// class

class QPixmapFilter___ extends QPixmapFilter {
        public final void setConvolutionKernel(double[] kernel, int rows, int columns) {
            int length = rows * columns;

            io.qt.QNativePointer kernelPtr = new io.qt.QNativePointer(io.qt.QNativePointer.Type.double, length);
            for (int i = 0; i < length; ++i)
                kernelPtr.setdoubleAt(i, kernel[i]);

            setConvolutionKernel_private(kernelPtr, rows, columns);
        }
}// class

class QTextCursor___ extends QTextCursor {
    public final static class SelectedTableCells {
        private SelectedTableCells(int firstRow, int numRows, int firstColumn, int numColumns) {
            this.firstRow = firstRow;
            this.numRows = numRows;
            this.firstColumn = firstColumn;
            this.numColumns = numColumns;
        }
        public final int firstRow;
        public final int numRows;
        public final int firstColumn;
        public final int numColumns;
    }
}// class

class QTextBlock___ extends QTextBlock {

    @Override
    public java.util.Iterator<QTextFragment> iterator(){
    return new java.util.Iterator<QTextFragment>() {
        
        iterator it = begin();
        
        @Override
        public QTextFragment next() {
            QTextFragment next = it.fragment();
            it.next();
            return next;
        }
        
        @Override
        public boolean hasNext() {
            return !it.atEnd();
        }
    };
}

}// class

class QPolygon___ extends QPolygon {
        @io.qt.QtUninvokable
        public final void append(int x, int y) {
            append(new QPoint(x, y));
        }
}// class

class QPolygonF___ extends QPolygonF {
        @io.qt.QtUninvokable
        public final void append(double x, double y) {
            append(new QPointF(x, y));
        }
}// class

class QValidator___ extends QValidator {

        public static class QValidationData {
                @io.qt.internal.NativeAccess
                public QValidationData(String input, int pos) {
                    string = input;
                    position = pos;
                }

                public @io.qt.internal.NativeAccess String string;
                public @io.qt.internal.NativeAccess int position;
        }

}// class

class QOpenGLShaderProgram___ extends QGLColormap {
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
            io.qt.QNativePointer ok = new io.qt.QNativePointer(io.qt.QNativePointer.Type.Boolean);
            QMatrix returned = inverted(ok);
            if (!ok.booleanValue())
                throw new IllegalArgumentException("Matrix is not invertible");
            return returned;
        }

        @io.qt.QtUninvokable
        public final QMatrix multiply(QMatrix other) {
            operator_multiply_assign(other);
            return this;
        }

        @io.qt.QtUninvokable
        public final QMatrix multiplied(QMatrix other) {
            return operator_multiply(other);
        }
}// class

class QPaintDeviceWindow___ extends QPaintDeviceWindow {
}// class

class QQuaternion___ extends QQuaternion {
    public static class Axes{
        private Axes(QVector3D xAxis, QVector3D yAxis, QVector3D zAxis) {
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
        private AxisAndAngle(QVector3D axis, float angle) {
            super();
            this.axis = axis;
            this.angle = angle;
        }
        public final QVector3D axis;
        public final float angle;
    }
    
    public static class EulerAngles{
        private EulerAngles(float pitch, float yaw, float roll) {
            super();
            this.pitch = pitch;
            this.yaw = yaw;
            this.roll = roll;
        }
        public final float pitch;
        public final float yaw;
        public final float roll;
    }
}// class

class QOpenGLExtraFunctions___ extends QOpenGLExtraFunctions {

}// class

class QOpenGLFunctions_ES2___{
}// class

class QGradient_java__{
    @io.qt.QtUninvokable
    public static QGradient create(io.qt.gui.QGradient.Preset preset){
        return create(preset.value());
    }

    @io.qt.QtUninvokable
    private native static QGradient create(int preset);
}// class


class QOpenGLContext___{

    @io.qt.QtUninvokable
    public final <T extends io.qt.gui.QAbstractOpenGLFunctions> T versionFunctions(Class<T> type){
        return versionFunctions(nativeId(this), java.util.Objects.requireNonNull(type));
    }
    @io.qt.QtUninvokable
    private native <T extends io.qt.gui.QAbstractOpenGLFunctions> T versionFunctions(long __this__nativeId, Class<T> type);

}// class


class QWindow___ extends QWindow {
    /**
     * <p>Overloaded function for {@link #parent(io.qt.gui.QWindow.AncestorMode)}
     *  with <code>mode = io.qt.gui.QWindow.AncestorMode.ExcludeTransients</code>.</p>
     */
    @io.qt.QtUninvokable
    public final io.qt.gui.QWindow nonTransientParent(){
        return parent(io.qt.gui.QWindow.AncestorMode.ExcludeTransients);
    }
}// class


class QAction___ extends QAction {

        private QActionGroup __rcActionGroup = null;

        /**
         * Sets the shortcut to the key sequence for the given key string. For
         * example "Ctrl+O" gives CTRL+'O'. The strings "Ctrl", "Shift", "Alt" and
         * "Meta" are recognized, as well as their translated equivalents in the
         * "QShortcut" context (using QObject::tr()). Up to four key codes may be
         * entered by separating them with commas, e.g. "Alt+X,Ctrl+S,Q".
         *
         * @param key
         *            The description of the key sequence. Typically used with tr()
         *            so key sequences can be locale aware.
         */
        public final void setShortcut(String key) {
            setShortcut(new QKeySequence(key));
        }

        /**
         * Sets the shortcut to the key sequence for the given key. The result will
         * depend on the currently running platform. The key sequence will be based
         * on the first element in the list of key bindings for the key.
         *
         * @param key
         *            The key for which to select a key sequence
         */
        public final void setShortcut(QKeySequence.StandardKey key) {
            setShortcut(new QKeySequence(key));
        }

        public final void setIcon(QPixmap pm) {
            setIcon(new QIcon(pm));
        }

        @io.qt.QtUninvokable
        public final QMenu menu(){
            return menu(QMenu.class);
        }
        
        @io.qt.QtUninvokable
        public final <T extends QMenu> T menu(Class<T> type) {
            io.qt.core.QObject result = menu(nativeId(this));
            if(!type.isInstance(result))
                return type.cast(result);
            return null;
        }
        
        private Object __rcMenu;
    
        @io.qt.QtUninvokable
        public final void setMenu(QMenu menu){
            setMenu(nativeId(this), checkedNativeId(menu));
            __rcMenu = menu();
        }
        
        @io.qt.QtUninvokable
        private native io.qt.core.QObject menu(long __this__nativeId);
        
        @io.qt.QtUninvokable
        private native void setMenu(long __this__nativeId, long menu);
        
        @io.qt.QtUninvokable
        public final QWidget parentWidget(){
            return parentWidget(QWidget.class);
        }
        
        @io.qt.QtUninvokable
        public final <T extends QWidget> T parentWidget(Class<T> type) {
            io.qt.core.QObject result = parent();
            while (result!=null && !type.isInstance(result))
                result = result.parent();
            return type.cast(result);
        }
        
        @io.qt.QtUninvokable
        public final java.util.List<QWidget> associatedWidgets() {
            return associatedWidgets(QWidget.class);
        }
        
        @io.qt.QtUninvokable
        public final <T extends QWidget> java.util.List<T> associatedWidgets(Class<T> type) {
            java.util.List<T> result = new java.util.ArrayList<>();
            for (io.qt.core.QObject object : associatedObjects())
                if (type.isInstance(object))
                    result.add(type.cast(object));
            return result;
        }
        
        @io.qt.QtUninvokable
        public final java.util.List<QGraphicsWidget> associatedGraphicsWidgets() {
            return associatedGraphicsWidgets(QGraphicsWidget.class);
        }
        
        @io.qt.QtUninvokable
        public final <T extends QGraphicsWidget> java.util.List<T> associatedGraphicsWidgets(Class<T> type) {
            java.util.List<T> result = new java.util.ArrayList<>();
            for (io.qt.core.QObject object : associatedObjects())
                if (type.isInstance(object))
                    result.add(type.cast(object));
            return result;
        }
}// class


