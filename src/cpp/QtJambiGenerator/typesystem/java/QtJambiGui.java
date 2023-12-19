/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
**
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

class QShortcut__{
    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent){
        this(key, parent, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }
    
    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent){
        this(key, parent, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }
    
    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent, io.qt.core.Qt.ShortcutContext context){
        this(key, parent, (java.lang.Object)null, (java.lang.Object)null, context);
    }
    
    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent, java.lang.String member, io.qt.core.Qt.ShortcutContext context){
        this(key, parent, member, (java.lang.String)null, context);
    }
    
    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent, java.lang.String member, java.lang.String ambiguousMember){
        this(key, parent, member, ambiguousMember, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }
    
    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent, java.lang.String member){
        this(key, parent, member, (java.lang.String)null, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }
    
    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent, java.lang.String member, java.lang.String ambiguousMember, io.qt.core.Qt.ShortcutContext context){
        this(key, parent, context);
        if(member!=null && !member.isEmpty())
            activated.connect(parent, member);
        if(ambiguousMember!=null && !ambiguousMember.isEmpty())
            activatedAmbiguously.connect(parent, ambiguousMember);
    }
    
    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent, io.qt.core.Qt.ShortcutContext context){
        this(key, parent, (java.lang.Object)null, (java.lang.Object)null, context);
    }
    
    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent, java.lang.String member, io.qt.core.Qt.ShortcutContext context){
        this(key, parent, member, (java.lang.String)null, context);
    }
    
    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent, java.lang.String member, java.lang.String ambiguousMember){
        this(key, parent, member, ambiguousMember, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }
    
    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent, java.lang.String member){
        this(key, parent, member, (java.lang.String)null, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }
    
    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent, java.lang.String member, java.lang.String ambiguousMember, io.qt.core.Qt.ShortcutContext context){
        this(key, parent, context);
        if(member!=null && !member.isEmpty())
            activated.connect(parent, member);
        if(ambiguousMember!=null && !ambiguousMember.isEmpty())
            activatedAmbiguously.connect(parent, ambiguousMember);
    }
    
    private static io.qt.core.QObject parent(io.qt.core.QMetaObject.Slot0 slot){
        io.qt.core.QObject parent = QtJambi_LibraryUtilities.internal.lambdaContext(java.util.Objects.requireNonNull(slot));
        if(parent==null)
            throw new IllegalArgumentException("Slot needs to be member of a QObject instance.");
        return parent;
    }
    
    public QShortcut(io.qt.gui.QKeySequence key,
            io.qt.core.QMetaObject.Slot0 slot,
            io.qt.core.Qt.ShortcutContext context)
    {
        this(key, parent(slot), context);
        activated.connect(slot);
    }

    public QShortcut(io.qt.gui.QKeySequence key,
            io.qt.core.QMetaObject.Slot0 slot)
    {
        this(key, slot, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }

    public QShortcut(io.qt.gui.QKeySequence key,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.QMetaObject.Slot0 slot2,
            io.qt.core.Qt.ShortcutContext context)
    {
        this(key, parent(slot), context);
        activated.connect(slot);
        activatedAmbiguously.connect(slot2);
    }

    public QShortcut(io.qt.gui.QKeySequence key,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.QMetaObject.Slot0 slot2)
    {
        this(key, slot, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }

    public QShortcut(io.qt.gui.QKeySequence.StandardKey key,
            io.qt.core.QMetaObject.Slot0 slot,
            io.qt.core.Qt.ShortcutContext context)
    {
        this(key, parent(slot), context);
        activated.connect(slot);
    }

    public QShortcut(io.qt.gui.QKeySequence.StandardKey key,
            io.qt.core.QMetaObject.Slot0 slot)
    {
        this(key, slot, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }

    public QShortcut(io.qt.gui.QKeySequence.StandardKey key,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.QMetaObject.Slot0 slot2,
            io.qt.core.Qt.ShortcutContext context)
    {
        this(key, parent(slot), context);
        activated.connect(slot);
        activatedAmbiguously.connect(slot2);
    }

    public QShortcut(io.qt.gui.QKeySequence.StandardKey key,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.QMetaObject.Slot0 slot2)
    {
        this(key, slot, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }
    
    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent,
            io.qt.core.QMetaObject.Slot0 slot,
            io.qt.core.Qt.ShortcutContext context)
    {
        this(key, parent, context);
        activated.connect(slot);
    }

    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent,
            io.qt.core.QMetaObject.Slot0 slot)
    {
        this(key, parent, slot, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }

    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.QMetaObject.Slot0 slot2,
            io.qt.core.Qt.ShortcutContext context)
    {
        this(key, parent, context);
        activated.connect(slot);
        activatedAmbiguously.connect(slot2);
    }

    public QShortcut(io.qt.gui.QKeySequence key, io.qt.core.QObject parent,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.QMetaObject.Slot0 slot2)
    {
        this(key, parent, slot, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }

    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent,
            io.qt.core.QMetaObject.Slot0 slot,
            io.qt.core.Qt.ShortcutContext context)
    {
        this(key, parent, context);
        activated.connect(slot);
    }

    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent,
            io.qt.core.QMetaObject.Slot0 slot)
    {
        this(key, parent, slot, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }

    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.QMetaObject.Slot0 slot2,
            io.qt.core.Qt.ShortcutContext context)
    {
        this(key, parent, context);
        activated.connect(slot);
        activatedAmbiguously.connect(slot2);
    }

    public QShortcut(io.qt.gui.QKeySequence.StandardKey key, io.qt.core.QObject parent,
            io.qt.core.QMetaObject.Slot0 slot, io.qt.core.QMetaObject.Slot0 slot2)
    {
        this(key, parent, slot, io.qt.core.Qt.ShortcutContext.WindowShortcut);
    }
    
    /**
     * <p>Type-casting overload of <a href="@docRoot/qobject.html#parent"><code>QObject::parent()const</code></a></p>
     */
    @QtUninvokable
    public final <T> T parent(Class<T> type) {
        return type.cast(parent());
    }
}// class

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
    /**
     * Result type for {@link QClipboard#text(String, Mode)}.
     */
    @Deprecated
    public final static class Text {
        private Text(String text, String subtype) {
            super();
            this.text = text;
            this.subtype = subtype;
        }
        public final String text;
        public final String subtype;
    }

    /**
     * @deprecated Use {@link #text(io.qt.core.QString, Mode)} instead
     */
    @Deprecated
    @QtUninvokable
    public final io.qt.gui.QClipboard.@StrictNonNull Text text(java.lang.String subtype, io.qt.gui.QClipboard.@NonNull Mode mode){
        io.qt.core.QString _subType = new io.qt.core.QString(subtype);
        String text = text(_subType, mode);
        return new Text(text, _subType.toString());
    }

    /**
     * @deprecated Use {@link #text(io.qt.core.QString)} instead
     */
    @Deprecated
    @QtUninvokable
    public final io.qt.gui.QClipboard.@StrictNonNull Text text(java.lang.String subtype){
        io.qt.core.QString _subType = new io.qt.core.QString(subtype);
        String text = text(_subType);
        return new Text(text, _subType.toString());
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

                @NativeAccess
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

        public static @NonNull QGuiApplication initialize(String args @StrictNonNull[]) {
            return io.qt.core.QCoreApplication.initialize(null, args, QGuiApplication::new);
        }

        public static @NonNull QGuiApplication initialize(String applicationName, String args @StrictNonNull[]) {
            return io.qt.core.QCoreApplication.initialize(applicationName, args, QGuiApplication::new);
        }

        public static void shutdown() {
            io.qt.core.QCoreApplication.shutdown();
        }
        
        public static @Nullable QGuiApplication instance() {
            io.qt.core.QCoreApplication app = io.qt.core.QCoreApplication.instance();
            if (app instanceof QGuiApplication)
                return (QGuiApplication) app;
            return null;
        }
}// class

class QImage___ extends QImage {
        public QImage(byte data[], int width, int height, Format format) {
            initialize_native(this, data, width, height, format);
        }

        private static native void initialize_native(QImage instance, byte data[], int width, int height, Format format);
        
        public QImage(java.nio.Buffer data, int width, int height, Format format) {
            initialize_native(this, data, width, height, format);
        }

        private static native void initialize_native(QImage instance, java.nio.Buffer data, int width, int height, Format format);
}// class

class QImage::JNI{

void __qt_create_new_QImage_7(void* __qtjambi_ptr, JNIEnv* env, jobject, jvalue* arguments){
    QTJAMBI_NATIVE_METHOD_CALL("construct QImage(unsigned char * data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)");
    jobject data = arguments[0].l;
    jint width = arguments[1].i;
    jint height = arguments[2].i;
    jobject format = arguments[3].l;
    JByteArrayPointer* qt_data = new JByteArrayPointer(env, jbyteArray(data), false);
    new(__qtjambi_ptr) QImage_shell(*qt_data, width, height, qtjambi_cast<QImage::Format>(env, format), [](void* ptr){ delete reinterpret_cast<JByteArrayPointer*>(ptr); }, qt_data);

}

void __qt_create_new_QImage_8(void* __qtjambi_ptr, JNIEnv* env, jobject, jvalue* arguments){
    QTJAMBI_NATIVE_METHOD_CALL("construct QImage(unsigned char * data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)");
    jobject data = arguments[0].l;
    jint width = arguments[1].i;
    jint height = arguments[2].i;
    jobject format = arguments[3].l;
    JBufferConstData* qt_data = new JBufferConstData(env, data);
    new(__qtjambi_ptr) QImage_shell(*qt_data, width, height, qtjambi_cast<QImage::Format>(env, format), [](void* ptr){ delete reinterpret_cast<JBufferConstData*>(ptr); }, qt_data);
}

extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QImage_initialize_1native__Lio_qt_gui_QImage_2_3BIILio_qt_gui_QImage_00024Format_2)
(JNIEnv *env, jclass __jni_class, jobject java_object, jobject data, jint width, jint height, jobject format)
{
    QTJAMBI_NATIVE_METHOD_CALL("QImage::QImage(unsigned char * data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)");
    QTJAMBI_TRY{
        jvalue arguments[4];
        arguments[0].l = data;
        arguments[1].i = width;
        arguments[2].i = height;
        arguments[3].l = format;
        QtJambiShell::initialize(env, __jni_class, java_object, &__qt_create_new_QImage_7, sizeof(QImage_shell), typeid(QImage), true, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

// QImage::QImage(unsigned char * data, int width, int height, QImage::Format format)
extern "C" Q_DECL_EXPORT void JNICALL QTJAMBI_FUNCTION_PREFIX(Java_io_qt_gui_QImage_initialize_1native__Lio_qt_gui_QImage_2Ljava_nio_Buffer_2IILio_qt_gui_QImage_00024Format_2)
(JNIEnv *env, jclass __jni_class, jobject java_object, jobject data, jint width, jint height, jobject format)
{
    QTJAMBI_NATIVE_METHOD_CALL("QImage::QImage(unsigned char * data, int width, int height, QImage::Format format, QImageCleanupFunction cleanupFunction = nullptr, void *cleanupInfo = nullptr)");
    QTJAMBI_TRY{
        jvalue arguments[4];
        arguments[0].l = data;
        arguments[1].i = width;
        arguments[2].i = height;
        arguments[3].l = format;
        QtJambiShell::initialize(env, __jni_class, java_object, &__qt_create_new_QImage_8, sizeof(QImage_shell), typeid(QImage), true, arguments);
    }QTJAMBI_CATCH(const JavaException& exn){
        exn.raiseInJava(env);
    }QTJAMBI_TRY_END
}

}// class

class QPointerEvent___{
    @QtUninvokable
    public final native void setPoint(long i, QEventPoint point);
}// class

class QPointerEvent_native__{

}// class

class QKeySequence_5__ extends QKeySequence {

        @QtUninvokable
        public final int at(int i) {
            return get(i);
        }

}// class

class QKeySequence_6__ extends QKeySequence {

        @QtUninvokable
        public final QKeyCombination at(int i) {
            return get(i);
        }

}// class

class QPainter_redir__ extends QPainter {
    private static java.util.Map<QPaintDevice,QPaintDevice> __rcRedirections = new java.util.HashMap<>();
}// class

class QPainter___ extends QPainter {

    @QtUninvokable
    private static void beginPaint(QPaintDevice paintedWidget){
        __paintedDevices.put(paintedWidget, java.util.Collections.emptyList());
    }
    
    @QtUninvokable
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
    
    @QtUninvokable
    private void initialize(QPaintDevice device, boolean inConstructor){
        if(device instanceof io.qt.core.QObject){
            io.qt.core.QObject object = (io.qt.core.QObject)device;
            if(object.isWidgetType() || device instanceof QPaintDeviceWindow) {
                java.util.List<QPainter> painters = __paintedDevices.get(device);
                if(painters==null){
                    if(inConstructor) {
                        if(QPaintDevice.Impl.sharedPainter(device)!=null)
                            return;
                        QPaintDevice rpd = QPaintDevice.Impl.redirected(device, null);
                        if(rpd!=null && rpd.paintEngine()!=null)
                            return;
                        throw new QPaintingOutsidePaintEventException();
                    }
                }else if(painters.size()==0){
                    threadCheck((io.qt.core.QObject)device);
                    painters = java.util.Collections.singletonList(this);
                    __paintedDevices.put(device, painters);
                }else{
                    threadCheck((io.qt.core.QObject)device);
                    if(painters.size()==1){
                        painters = new java.util.LinkedList<>(painters);
                        __paintedDevices.put(device, painters);
                    }
                    painters.add(this);
                }
            }else {
                threadCheck((io.qt.core.QObject)device);
            }
        }else if(device==null){
            java.util.Objects.requireNonNull(device, "Argument 'QPaintDevice': null not expected.");
        }
    }
    
    @QtUninvokable
    private static native void threadCheck(io.qt.core.QObject object);
    
    private static java.util.Map<QPaintDevice,java.util.List<QPainter>> __paintedDevices = new java.util.HashMap<>();
}// class

class QPaintDevice___{
    @QtUninvokable
    static io.qt.gui.QPainter sharedPainter(QPaintDevice instance){
        return sharedPainter_native_constfct(QtJambi_LibraryUtilities.internal.nativeId(instance));
    }
    
    @QtUninvokable
    static io.qt.gui.QPaintDevice redirected(QPaintDevice instance, io.qt.core.QPoint offset){
        return redirected_native_QPoint_ptr_constfct(QtJambi_LibraryUtilities.internal.nativeId(instance), QtJambi_LibraryUtilities.internal.checkedNativeId(offset));
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

class QTextCursor___ extends QTextCursor {

    /**
     * Result type for {@link QTextCursor#selectedTableCells()}.
     */
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
        @QtUninvokable
        public final void append(int x, int y) {
            append(new QPoint(x, y));
        }
}// class

class QPolygonF___ extends QPolygonF {
        @QtUninvokable
        public final void append(double x, double y) {
            append(new QPointF(x, y));
        }
}// class

class QValidator___ extends QValidator {

    /**
     Result type for {@link QValidator#validate(QValidationData)}.
     */
    public static class QValidationData {
            @NativeAccess
            public QValidationData(String input, int pos) {
                string = input;
                position = pos;
            }

            public @NativeAccess String string;
            public @NativeAccess int position;
    }

    /**
     * @see #fixup(io.qt.core.QString)
     */
    @QtUninvokable
    public final java.lang.@NonNull String fixup(java.lang.@Nullable String string){
        io.qt.core.QString _string = new io.qt.core.QString(string);
        fixup(_string);
        return _string.toString();
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

class QPaintDeviceWindow___ extends QPaintDeviceWindow {
}// class

class QQuaternion___ extends QQuaternion {
    /**
     * Result type for {@link QQuaternion#getAxes()}.
     */
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
    
    /**
     * Result type for {@link QQuaternion#getAxisAndAngle()}.
     */
    public static class AxisAndAngle{
        private AxisAndAngle(QVector3D axis, float angle) {
            super();
            this.axis = axis;
            this.angle = angle;
        }
        public final QVector3D axis;
        public final float angle;
    }
    
    /**
     * Result type for {@link QQuaternion#getEulerAngles()}.
     */
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
    @QtUninvokable
    public static QGradient create(io.qt.gui.QGradient.Preset preset){
        return create(preset.value());
    }

    @QtUninvokable
    private native static QGradient create(int preset);
}// class

class QWindow___ extends QWindow {
    /**
     * <p>Overloaded function for {@link #parent(io.qt.gui.QWindow.AncestorMode)}
     *  with <code>mode = io.qt.gui.QWindow.AncestorMode.ExcludeTransients</code>.</p>
     */
    @QtUninvokable
    public final io.qt.gui.QWindow nonTransientParent(){
        return parent(io.qt.gui.QWindow.AncestorMode.ExcludeTransients);
    }
}// class


class QAction___ extends QAction {

    private QActionGroup __rcActionGroup = null;
    
    /**
     * <p>Type-casting overload of <a href="@docRoot/qobject.html#parent"><code>QObject::parent()const</code></a></p>
     */
    @QtUninvokable
    public final <T> T parent(Class<T> type) {
        io.qt.core.QObject result = parent();
        while (result!=null && !type.isInstance(result))
            result = result.parent();
        return type.cast(result);
    }
    
    /**
     * <p>Type-casting overload of <a href="@docRoot/qaction.html#associatedObjects"><code>QAction::associatedObjects()const</code></a></p>
     */
    @QtUninvokable
    public final <T> io.qt.core.QList<T> associatedObjects(Class<T> type) {
        io.qt.core.QList<T> result = new io.qt.core.QList<>(type);
        for (io.qt.core.QObject object : associatedObjects())
            if (type.isInstance(object))
                result.add(type.cast(object));
        return result;
    }
}// class

class QTransform___{

    /**
     * @see #squareToQuad(QPolygonF, QTransform)
     */
    public static io.qt.gui.@Nullable QTransform squareToQuad(io.qt.gui.@NonNull QPolygonF square){
        QTransform transform = new QTransform();
        if(squareToQuad(square, transform))
            return transform;
        else return null;
    }

    /**
     * @see #quadToSquare(QPolygonF, QTransform)
     */
    public static io.qt.gui.@Nullable QTransform quadToSquare(io.qt.gui.@NonNull QPolygonF quad){
        QTransform transform = new QTransform();
        if(quadToSquare(quad, transform))
            return transform;
        else return null;
    }

    /**
     * @see #quadToQuad(QPolygonF, QPolygonF, QTransform)
     */
    public static io.qt.gui.@Nullable QTransform quadToQuad(io.qt.gui.@NonNull QPolygonF one, io.qt.gui.@NonNull QPolygonF two){
        QTransform transform = new QTransform();
        if(quadToQuad(one, two, transform))
            return transform;
        else return null;
    }
}// class

class QPixmapCache___{
    /**
     * @see #find(Key,QPixmap)
     */
    public static io.qt.gui.@Nullable QPixmap find(io.qt.gui.QPixmapCache.@NonNull Key key){
        QPixmap pm = new QPixmap();
        if(find(key, pm))
            return pm;
        else return null;
    }

    /**
     * @see #find(String,QPixmap)
     */
    public static io.qt.gui.@Nullable QPixmap find(java.lang.@NonNull String key){
        QPixmap pm = new QPixmap();
        if(find(key, pm))
            return pm;
        else return null;
    }
}// class

class QImageIOHandler_6__{

    /**
     * @see #allocateImage(io.qt.core.QSize, io.qt.gui.QImage.Format, io.qt.gui.QImage)
     */
    public static io.qt.gui.@io.qt.Nullable QImage allocateImage(io.qt.core.@io.qt.NonNull QSize size, io.qt.gui.QImage.@io.qt.NonNull Format format){
        io.qt.gui.QImage im = new io.qt.gui.QImage();
        if(allocateImage(size, format, im))
            return im;
        else return null;
    }
}// class

