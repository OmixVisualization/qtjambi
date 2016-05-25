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

import org.qtjambi.qt.*;
import org.qtjambi.qt.internal.QtJambiObject.QPrivateConstructor;
import org.qtjambi.qt.core.*;

class QObject___ extends QObject {
        @org.qtjambi.qt.QtBlockedSlot
        public final java.util.List<QObject> findChildren() {
            return findChildren(null, (QRegExp) null);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final java.util.List<QObject> findChildren(Class < ? > cl) {
            return findChildren(cl, (QRegExp) null);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final java.util.List<QObject> findChildren(Class < ? > cl, String name) {
            return org.qtjambi.qt.internal.QtJambiInternal.findChildren(this, cl, name);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final java.util.List<QObject> findChildren(Class < ? > cl, QRegExp name) {
            return org.qtjambi.qt.internal.QtJambiInternal.findChildren(this, cl, name);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QObject findChild() {
            return findChild(null, null);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QObject findChild(Class < ? > cl) {
            return findChild(cl, null);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QObject findChild(Class < ? > cl, String name) {
            return org.qtjambi.qt.internal.QtJambiInternal.findChild(this, cl, name);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final void setProperty(String name, Object value) {
            setProperty(QNativePointer.createCharPointer(name), value);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final Object property(String name) {
            return property(QNativePointer.createCharPointer(name));
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QtProperty userProperty() {
            return super.qtJambiUserProperty();
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final java.util.List<org.qtjambi.qt.QtProperty> properties() {
            return super.qtJambiProperties();
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final int indexOfProperty(String name) {
            return super.qtJambiIndexOfProperty(name);
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final void connectSlotsByName() {
            org.qtjambi.qt.internal.QtJambiInternal.connectSlotsByName(this);
        }
}// class

abstract class QUrl___ extends QUrl{
    public interface FormattingOptionInterface extends QtEnumerator{}
    
    public static class FormattingOptions extends org.qtjambi.qt.QFlags<FormattingOptionInterface> {
        private static final long serialVersionUID = 1L;
        public FormattingOptions(FormattingOptionInterface ... args) { super(args); }
        public FormattingOptions(int value) { setValue(value); }
    }

    @QtBlockedSlot
    public final java.lang.String toDisplayString()    {
    	return toDisplayString(ComponentFormattingOption.PrettyDecoded.value());
    }

    @QtBlockedSlot
    public final java.lang.String toDisplayString(FormattingOptions options)    {
    	return toDisplayString(options.value());
    }

    @QtBlockedSlot
    public final java.lang.String toString(FormattingOptions options)    {
    	return toString(options.value());
    }

    @QtBlockedSlot
    public final java.lang.String url()    {
    	return url(ComponentFormattingOption.PrettyDecoded.value());
    }

    @QtBlockedSlot
    public final java.lang.String url(FormattingOptions options)    {
    	return url(options.value());
    }

    @QtBlockedSlot
    public final org.qtjambi.qt.core.QByteArray toEncoded()    {
    	return toEncoded(ComponentFormattingOption.PrettyDecoded.value());
    }
    
    @QtBlockedSlot
    public final org.qtjambi.qt.core.QByteArray toEncoded(FormattingOptions options)    {
    	return toEncoded(options.value());
    }

    public static java.util.List<java.lang.String> toStringList(java.util.List<org.qtjambi.qt.core.QUrl> uris, FormattingOptions options){
    	return toStringList(uris, options.value());
    }
    
    public static java.util.List<java.lang.String> toStringList(java.util.List<org.qtjambi.qt.core.QUrl> uris){
    	return toStringList(uris, ComponentFormattingOption.PrettyDecoded.value());
    }
}// class

abstract class QAbstractItemModel___ extends QAbstractItemModel {
        private native boolean setData_native(long id, int row, int col, Object value, int role);

        public final boolean setData(int row, int col, Object value) {
            return setData_native(nativeId(), row, col, value, org.qtjambi.qt.core.Qt.ItemDataRole.DisplayRole);
        }

        public final boolean setData(int row, int col, Object value, int role) {
            return setData_native(nativeId(), row, col, value, role);
        }

        private native Object data_native(long id, int row, int col, int role);

        public final Object data(int row, int col, int role) {
            return data_native(nativeId(), row, col, role);
        }

        public final Object data(int row, int col) {
            return data_native(nativeId(), row, col, Qt.ItemDataRole.DisplayRole);
        }
}// class

class QTimer___ extends QTimer {
        static private class QSingleShotTimer extends QObject {
                private int timerId = -1;
                public final Signal0 timeout = new Signal0();

                public QSingleShotTimer(int msec, QObject obj, String method) {
                    super(obj);
                    timeout.connect(obj, method);
                    timerId = startTimer(msec, Qt.TimerType.PreciseTimer);
                }

                protected void disposed() {
                    if (timerId > 0)
                        killTimer(timerId);
                    super.disposed();
                }

                protected void timerEvent(QTimerEvent e) {
                    if (timerId > 0)
                        killTimer(timerId);
                    timerId = -1;
                    timeout.emit();
                    disposeLater();
                }
        }

        public static void singleShot(int msec, QObject obj, String method) {
            new QSingleShotTimer(msec, obj, method);
        }
}// class

class QCoreApplication___ extends QCoreApplication {

        protected static QCoreApplication m_instance = null;

        public QCoreApplication(String args[]) {
            this(argc(args), argv(args));
        }

        public QCoreApplication(String applicationName, String args[]) {
            this(argc(args), argv(applicationName, args));
        }

        public static String translate(String context, String sourceText, String comment) {
		QTextCodec codec = QTextCodec.codecForName("UTF-8");
		return translate(context != null ? codec.fromUnicode(context).data() : null, sourceText != null ? codec.fromUnicode(sourceText).data() : null,
			comment != null ? codec.fromUnicode(comment).data() : null/*, Encoding.CodecForTr*/);
        }

        public static String translate(String context, String sourceText) {
            return translate(context, sourceText, null);
        }

        public static String translate(String context, String sourceText, String comment, int n) {
		QTextCodec codec = QTextCodec.codecForName("UTF-8");
		return translate(context != null ? codec.fromUnicode(context).data() : null, sourceText != null ? codec.fromUnicode(sourceText).data() : null,
			comment != null ? codec.fromUnicode(comment).data() : null, /*Encoding.CodecForTr,*/ n);
        }

        public static void initialize(String applicationName, String args[]) {
            if (m_instance != null)
                throw new RuntimeException("QCoreApplication can only be initialized once");

            org.qtjambi.qt.internal.HelperFunctions.setAsMainThread();

            List<String> paths = Utilities.unpackPlugins();
            if (paths != null) {
                Collections.reverse(paths);  // Qt prepends but our list is in highest priority first order
                for (String p : paths)
                    addLibraryPath(p);
            }
            org.qtjambi.qt.internal.QtJambiInternal.setupDefaultPluginPath();
            m_instance = new QCoreApplication(applicationName, args);
            m_instance.aboutToQuit.connect(m_instance, "disposeOfMyself()");
        }

        public static void initialize(String args[]) {
            if (m_instance != null)
                throw new RuntimeException("QCoreApplication can only be initialized once");

            org.qtjambi.qt.internal.HelperFunctions.setAsMainThread();

            List<String> paths = Utilities.unpackPlugins();
            if (paths != null) {
                Collections.reverse(paths);  // Qt prepends but our list is in highest priority first order
                for (String p : paths)
                    addLibraryPath(p);
            }
            org.qtjambi.qt.internal.QtJambiInternal.setupDefaultPluginPath();
            m_instance = new QCoreApplication(args);
            m_instance.aboutToQuit.connect(m_instance, "disposeOfMyself()");
        }

        public static void shutdown() {
            QCoreApplication app = instance();
            if(app != null) {
                app = null;		// discard hard-reference
		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());	// allow deleteLater() to work some magic
                quit();			// finish up cause an aboutToQuit() to fire
                processEvents();	// process quit
		QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());	// allow deleteLater() to work some magic
            }
            if (m_instance != null)	// this should be a QtJambi qWarning()
                System.err.println("WARNING: QCoreApplication.shutdown() m_instance!=null");
            System.gc();
            System.runFinalization();
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
                throw new RuntimeException("QCoreApplication has not been initialized with QCoreApplication.initialize()");
            return exec_internal();
        }

        private void disposeOfMyself() {
            synchronized(this) {
                // To force compiler/JVM not to move/optimized when
                m_instance = null;
            }
            System.gc();
            System.runFinalization();
            this.disposeLater();
        }

        protected final static org.qtjambi.qt.QNativePointer argv(String args[]) {
            String newArgs[] = new String[args.length + 1];
            System.arraycopy(args, 0, newArgs, 1, args.length);
            try {
                newArgs[0] = System.getProperty("qt.application.path", "Qt Jambi application");
            } catch (Exception e) {
                newArgs[0] = "Qt Jambi application";
            }
            argv = org.qtjambi.qt.QNativePointer.createCharPointerPointer(newArgs);
            return argv;
        }

        protected final static org.qtjambi.qt.QNativePointer argv(String applicationName, String args[]) {
            String newArgs[] = new String[args.length + 1];
            System.arraycopy(args, 0, newArgs, 1, args.length);
            if (applicationName != null && applicationName.length() != 0) {
                newArgs[0] = applicationName;
            } else {
                newArgs[0] = "Qt Jambi application";
            }
            argv = org.qtjambi.qt.QNativePointer.createCharPointerPointer(newArgs);
            return argv;
        }

        protected final static org.qtjambi.qt.QNativePointer argc(String args[]) {
            if (argc != null) {
                throw new RuntimeException("There can only exist one QCoreApplication instance");
            }
            argc = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Int);
            argc.setIntValue(args.length + 1);
            return argc;
        }

        @Override
        protected void disposed() {
            argc = null;
            argv = null;
            m_instance = null;
            super.disposed();
        }

        public boolean isDisposed() {
            if (m_instance == null)
                return true;
            return false;
        }

        public static void invokeLater(java.lang.Runnable runnable) {
            postEvent(new QInvokable(runnable), new QEvent(QInvokable.INVOKABLE_EVENT));
        }

        /**
         * Executes the runnable's run() method in the main thread and waits for it
         * to return. If the current thread is not the main thread, an event loop
         * must be running in the main thread, or this method will wait
         * indefinitely.
         */
        public static void invokeAndWait(Runnable runnable) {
            // Specialcase invoke and wait for the case of running on the current thread...
            if (Thread.currentThread() == instance().thread()) {
                runnable.run();
                return;
            }

            QSynchronousInvokable invokable = new QSynchronousInvokable(runnable);
            QCoreApplication.postEvent(invokable, new QEvent(QSynchronousInvokable.SYNCHRONOUS_INVOKABLE_EVENT));
            invokable.waitForInvoked();
            invokable.disposeLater();
        }

		public static void invokeAndWaitOrInterrupt(Runnable runnable) throws InterruptedException{
            // Specialcase invoke and wait for the case of running on the current thread...
            if (Thread.currentThread() == instance().thread()) {
                runnable.run();
                return;
            }

            QSynchronousInvokable invokable = new QSynchronousInvokable(runnable);
            QCoreApplication.postEvent(invokable, new QEvent(QSynchronousInvokable.SYNCHRONOUS_INVOKABLE_EVENT));
            try {
                invokable.waitForInvokedOrInterrupt();
            }finally{
				invokable.disposeLater();
			}
        }


        /**
         * Executes the task in the application's main thread after the
         * specified timeout. This is done by starting a timer so this
         * method does not block.
         * @param timeout The time to wait, in milliseconds
         * @param task The task to perform...
         */
        public static void invokeLater(final int timeout, final Runnable task) {
			invokeAndWait(
				new Runnable(){
					public void run(){
						QTimer.singleShot(timeout, new QObject(QCoreApplication.instance()) {
                public void todo() {
                    task.run();
                    disposeLater();
                }
            }
            , "todo()");
        }
				}
			);
        }


        private static org.qtjambi.qt.QNativePointer argc, argv;

}// class

class QTranslator___ extends QTranslator {
        public final boolean load(byte data[]) {
            return load(org.qtjambi.qt.internal.QtJambiInternal.byteArrayToNativePointer(data), data.length);
        }
}// class

class QItemSelection___ extends QItemSelection {

        public static void split(QItemSelectionRange range, QItemSelectionRange other, QItemSelection result) {
            org.qtjambi.qt.QNativePointer np = result.nativePointer();
            split(range, other, np);
        }

}// class


class QProcess___ extends QProcess {
        public static Long startDetached(String program, java.util.List<String> arguments, String workingDirectory) {
            QNativePointer pid = new QNativePointer(QNativePointer.Type.Long);
            boolean success = startDetached(program, arguments, workingDirectory, pid);
            return success ? pid.longValue() : null;
        }
}// class

class QDataStream___ extends QDataStream {

        private QNativePointer srb = new QNativePointer(QNativePointer.Type.Byte, 32);
		{
			srb.setVerificationEnabled(false);
		}

        public final boolean readBoolean() {
            operator_shift_right_boolean(srb);
            return srb.booleanValue();
        }

        public final byte readByte() {
            operator_shift_right_byte(srb);
            return srb.byteValue();
        }

        public final short readShort() {
            operator_shift_right_short(srb);
            return srb.shortValue();
        }

        public final int readInt() {
            operator_shift_right_int(srb);
            return srb.intValue();
        }

        public final long readLong() {
            operator_shift_right_long(srb);
            return srb.longValue();
        }

        public final char readChar() {
            operator_shift_right_short(srb);
            return srb.charValue();
        }

        public final float readFloat() {
            operator_shift_right_float(srb);
            return srb.floatValue();
        }

        public final double readDouble() {
            operator_shift_right_double(srb);
            return srb.doubleValue();
        }

        public final QDataStream writeChar(char c) {
            writeShort((short) c);
            return this;
        }

        private native String readString_private(long nativeId);

        private native void writeString_private(long nativeId, String string);

        public final String readString() {
            if (nativeId() == 0)
                throw new QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
            return readString_private(nativeId());
        }

        public final void writeString(String string) {
            if (nativeId() == 0)
                throw new QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
            writeString_private(nativeId(), string);
        }

        private native int writeBytes(long id, byte buffer[], int length);

        private native int readBytes(long id, byte buffer[], int length);

        public final int writeBytes(byte buffer[]) {
            return writeBytes(buffer, buffer.length);
        }

        public final int writeBytes(byte buffer[], int length) {
            return writeBytes(nativeId(), buffer, length);
        }

        public final int readBytes(byte buffer[]) {
            return readBytes(buffer, buffer.length);
        }

        public final int readBytes(byte buffer[], int length) {
            return readBytes(nativeId(), buffer, length);
        }
}// class

class QTextStream___ extends QTextStream {
        public final void setCodec(String codecName) {
            setCodec(QNativePointer.createCharPointer(codecName));
            if (codec() != __rcCodec)
                __rcCodec = null;
        }

        private QNativePointer srb = new QNativePointer(QNativePointer.Type.Byte, 32) {
            {
                setVerificationEnabled(false);
            }
        };

        public final byte readByte() {
            operator_shift_right_byte(srb);
            return srb.byteValue();
        }

        public final short readShort() {
            operator_shift_right_short(srb);
            return srb.shortValue();
        }

        public final int readInt() {
            operator_shift_right_int(srb);
            return srb.intValue();
        }

        public final long readLong() {
            operator_shift_right_long(srb);
            return srb.longValue();
        }

        public final float readFloat() {
            operator_shift_right_float(srb);
            return srb.floatValue();
        }

        public final double readDouble() {
            operator_shift_right_double(srb);
            return srb.doubleValue();
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QTextStream writeChar(char c) {
            writeShort((short) c);
            return this;
        }

        public final char readChar() {
            operator_shift_right_short(srb);
            return srb.charValue();
        }

        public final String readString() {
            return readString_native(nativeId());
        }

        public final void writeString(String string) {
            writeString_native(nativeId(), string);
        }

        /**
            This function makes the text tream operate on a string instead
            of a QIODevice.
            <p>
            The <tt>string</tt> parameter is the initial contents of the string that the text stream
            will work on. If <tt>string</tt> is <tt>null</tt>, an empty string will be created.
            The stream can both be written to and read from, depending on <tt>openMode</tt>.
            The contents of the string can at any time be retrieved by <tt>string()</tt>.
            For example:
            <pre class="snippet">
                QTextStream stream = QTextStream.createStringStream("TestString\n55",
                    new QIODevice.OpenMode(QIODevice.OpenModeFlag.ReadWrite));
                stream.writeString(" ");
                stream.writeString("Hei");
                stream.seek(0);
                System.err.println(stream.readString()); // == "TestString"
                System.err.println(stream.string()); // == "TestString\n55 Hei"
            </pre>
            <p>
            If the text stream already has a QIODevice set, it will flush this.
            @param string The initial content of the string.
            @param openMode Specifies access priveledges to the stream.
            @return A QTextStream operating on a String instead of a QIODevice.
        */
        public static QTextStream createStringStream(String string, org.qtjambi.qt.core.QIODevice.OpenMode openMode) {
            QTextStream instance = new StringStream(string, openMode);
            return instance;
        }

        /**
            Returns the contents of the string that this text stream is working on.
            <p>
            After creating a string stream with the createStringStream() method,
            you can fetch the contents of the string the text stream is working on with this
            method.
            <p>
            If this QTextStream was not created using createStringStream(), this function returns <tt>null</tt>.
            @return Returns the contents of QTextStreams created with createStringStream()
        */
        public String string() {
            return null;
        }

        private static final class StringStream extends QTextStream {
                protected StringStream(String string, org.qtjambi.qt.core.QIODevice.OpenMode openMode) {
                    stringPtr = createString(nativeId(), string, openMode.value());
                }

                public String string() {
                    return string_native(nativeId());
                }

                @Override
                public void disposed() {
                    deleteString(stringPtr);
                }

                private native long createString(long id, String str, int openMode);
                private native void deleteString(long strPtr);
                private native String string_native(long id);

                private long stringPtr = 0;
        }

        private final native String readString_native(long id);
        private final native void writeString_native(long id, String string);
}// class

class QBitArray___ extends QBitArray {

        @org.qtjambi.qt.QtBlockedSlot
        public final QBitArray xor(QBitArray other) {
            operator_xor_assign(other);
			return this;
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QBitArray and(QBitArray other) {
            operator_and_assign(other);
			return this;
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QBitArray or(QBitArray other) {
            operator_or_assign(other);
			return this;
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QBitArray set(QBitArray other) {
            operator_assign(other);
			return this;
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QBitArray inverted() {
            return operator_negate();
        }

}// class

// hfr

class QDate___ extends QDate {

        public final int weekNumber() {
            return weekNumber(null);
        }

        public final int yearOfWeekNumber() {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Int);
            weekNumber(np);
            return np.intValue();
        }

}// class

class QDir___ extends QDir {

        @org.qtjambi.qt.QtBlockedSlot
        public String at(int i) {
            return operator_subscript(i);
        }

}// class

class QByteArray___ extends QByteArray {
        public QByteArray(String s) {
            this();
            append(s);
        }

        public QByteArray(byte data[]) {
            this(org.qtjambi.qt.internal.QtJambiInternal.byteArrayToNativePointer(data), data.length);
        }

        public final boolean contains(String str) {
            return contains(new QByteArray(str));
        }

        public final int count(String str) {
            return count(new QByteArray(str));
        }

        public final boolean endsWith(String str) {
            return endsWith(new QByteArray(str));
        }

        public final QByteArray prepend(String str) {
            return prepend(new QByteArray(str));
        }

        public final QByteArray replace(QByteArray before, String after) {
            return replace(before, new QByteArray(after));
        }

        public final QByteArray replace(String before, String after) {
            return replace(new QByteArray(before), new QByteArray(after));
        }

        public final boolean startsWith(String str) {
            return startsWith(new QByteArray(str));
        }

        public final byte[] toByteArray() {
            byte[] res = new byte[size()];

            for (int i = 0; i < size(); i++) {
                res[i] = at(i);
            }
            return res;
        }

        @org.qtjambi.qt.QtBlockedSlot
        public final QByteArray set(QByteArray other) {
            operator_assign(other);
            return this;
        }
		
		private static final org.qtjambi.qt.QNativePointer toNativePointer(byte[] data){
			if(data==null){
				return null;
			}
			org.qtjambi.qt.QNativePointer pointer = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Byte, data.length);
			for (int i = 0; i < data.length; i++) {
				pointer.setByteAt(i, data[i]);
			}
			return pointer;
		}

		public final QByteArray prepend(byte[] data) {
			this.prepend(toNativePointer(data), data.length);
			return this;
		}

		public final QByteArray replace(int index, int len, byte[] data) {
			this.replace(index, len, toNativePointer(data), data.length);
			return this;
		}

		public final QByteArray insert(int i, byte[] data) {
			this.insert(i, toNativePointer(data), data.length);
			return this;
		}

}// class

class QFile___ extends QFile {

        public static String decodeName(String localFileName) {
            return decodeName(org.qtjambi.qt.QNativePointer.createCharPointer(localFileName));
        }

}// class

class QIODevice___ extends QIODevice {

        /**
         * Gets a byte from the device.
         *
         * @return -1 on failure, or the value of the byte on success
         */
        public final int getByte() {
            QNativePointer np = new QNativePointer(QNativePointer.Type.Byte);
            boolean success = getByte(np);
            return success ? np.byteValue() : -1;
        }

}// class

class QCryptographicHash___ extends QCryptographicHash {

        public final void addData(byte data[]) {
            QNativePointer np = org.qtjambi.qt.internal.QtJambiInternal.byteArrayToNativePointer(data);
            addData(np, data.length);
        }

}// class

class QTextCodec___ extends QTextCodec {

        public static QTextCodec codecForName(String name) {
            return codecForName(org.qtjambi.qt.QNativePointer.createCharPointer(name));
        }

}// class

class QBuffer___ extends QBuffer {

        // retain a reference to avoid gc
        private Object strongDataReference = null;

        public QBuffer(QByteArray byteArray, QObject parent) {
            this(byteArray.nativePointer(), parent);
            strongDataReference = byteArray;
        }

        public QBuffer(QByteArray byteArray) {
            this(byteArray, null);
        }

        public final void setBuffer(QByteArray byteArray) {
            setBuffer(byteArray.nativePointer());
            strongDataReference = byteArray;
        }

        public final void setData(byte data[]) {
            QNativePointer np = org.qtjambi.qt.internal.QtJambiInternal.byteArrayToNativePointer(data);
            setData(np, data.length);
        }

}// class

class QSignalMapper___ extends QSignalMapper {

        private java.util.Hashtable<QObject, QObject> __rcObjectForObject = new java.util.Hashtable<QObject, QObject>();

        private java.util.Hashtable<QObject, Object> __rcWidgetForObject = new java.util.Hashtable<QObject, Object>();

}// class

class QAbstractFileEngine_MapExtensionReturn___ extends QAbstractFileEngine_MapExtensionReturn {
        private QNativePointer currentAddressNativePointer; // don't garbage collect while in use
        public final void setAddress(String address) {
            currentAddressNativePointer = address != null ? QNativePointer.createCharPointer(address) : null;
            address_private(currentAddressNativePointer);
        }

        public final String address() {
            QNativePointer np = address_private();
            return np != null ? org.qtjambi.qt.internal.QtJambiInternal.charPointerToString(np) : null;
        }
}// class

class QAbstractFileEngine___ extends QAbstractFileEngine {
        /**
         * Adds <tt>path</tt> to the set of paths in which Qt Jambi should search for resources. Resources
         * can be accessed using the "classpath:" scheme.
         */
        public static void addSearchPathForResourceEngine(String path) {
            org.qtjambi.qt.internal.fileengine.QClassPathEngine.addSearchPath(path, false);
        }

        /**
         * Removes <tt>path</tt> from the set of paths in which Qt Jambi searches
         * for resources.
         */
        public static void removeSearchPathForResourceEngine(String path) {
            org.qtjambi.qt.internal.fileengine.QClassPathEngine.removeSearchPath(path);
        }

}// class

class QAbstractFileEngine_UnMapExtensionOption___ extends QAbstractFileEngine_UnMapExtensionOption {
        private QNativePointer currentAddressNativePointer; // don't garbage collect while in use
        public final void setAddress(String address) {
            currentAddressNativePointer = address != null ? QNativePointer.createCharPointer(address) : null;
            address_private(currentAddressNativePointer);
        }

        public final String address() {
            QNativePointer np = address_private();
            return np != null ? org.qtjambi.qt.internal.QtJambiInternal.charPointerToString(np) : null;
        }
}// class

class QFutureWatcher___ extends QFutureWatcher {

        public final QFuture<T> future() {
            if (nativeId() == 0)
                throw new QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
            return __qt_future(nativeId());
        }
        private native QFuture<T> __qt_future(long nativeId);

}// class

class QFutureWatcherVoid___ extends QFutureWatcherVoid {

        public final QFutureVoid future() {
            if (nativeId() == 0)
                throw new QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
            return __qt_future(nativeId());
        }

        private native QFutureVoid __qt_future(long nativeId);

}// class

class QFutureSynchronizer___ extends QFutureSynchronizer {

        public final java.util.List<QFuture<T>> futures() {
            if (nativeId() == 0)
                throw new QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
            return __qt_futures(nativeId());
        }
        private native java.util.List<QFuture<T>> __qt_futures(long nativeId);

}// class

class QFutureSynchronizerVoid___ extends QFutureSynchronizerVoid {

        public final java.util.List<QFutureVoid> futures() {
            if (nativeId() == 0)
                throw new QNoNativeResourcesException("Function call on incomplete object of type: " + getClass().getName());
            return __qt_futures(nativeId());
        }
        private native java.util.List<QFutureVoid> __qt_futures(long nativeId);

}// class

class QMargins___{

    @QtBlockedSlot
    public final QMargins subtract(org.qtjambi.qt.core.QMargins margins)    {
    	operator_subtract_assign(margins);
    	return this;
    }
    
    @QtBlockedSlot
    public final QMargins add(org.qtjambi.qt.core.QMargins margins)    {
    	operator_add_assign(margins);
    	return this;
    }
    
    @QtBlockedSlot
    public final QMargins multiply(double arg__1)    {
    	operator_multiply_assign(arg__1);
	return this;
    }
    
    @QtBlockedSlot
    public final QMargins divide(double arg__1)    {
    	operator_divide_assign(arg__1);
	return this;
    }
    
    @QtBlockedSlot
    public final QMargins multiply(int arg__1)    {
    	operator_multiply_assign(arg__1);
	return this;
    }
    
    @QtBlockedSlot
    public final QMargins divide(int arg__1)    {
    	operator_divide_assign(arg__1);
	return this;
    }

}// class

class QMarginsF___{

    @QtBlockedSlot
    public final QMarginsF subtract(org.qtjambi.qt.core.QMarginsF margins)    {
    	operator_subtract_assign(margins);
    	return this;
    }
    
    @QtBlockedSlot
    public final QMarginsF add(org.qtjambi.qt.core.QMarginsF margins)    {
    	operator_add_assign(margins);
    	return this;
    }
    
    @QtBlockedSlot
    public final QMarginsF multiply(double arg__1)    {
    	operator_multiply_assign(arg__1);
	return this;
    }
    
    @QtBlockedSlot
    public final QMarginsF divide(double arg__1)    {
    	operator_divide_assign(arg__1);
	return this;
    }
    
    @QtBlockedSlot
    public final QMarginsF multiply(int arg__1)    {
    	operator_multiply_assign(arg__1);
	return this;
    }
    
    @QtBlockedSlot
    public final QMarginsF divide(int arg__1)    {
    	operator_divide_assign(arg__1);
	return this;
    }

}// class


class QRect___{
	

    @QtBlockedSlot
    public final QRect subtract(org.qtjambi.qt.core.QMargins margins)    {
    	operator_subtract_assign(margins);
    	return this;
    }
    
    @QtBlockedSlot
    public final QRect add(org.qtjambi.qt.core.QMargins margins)    {
    	operator_add_assign(margins);
    	return this;
    }

}// class

class QRectF___{
	
/*
    @QtBlockedSlot
    public final QRectF subtract(org.qtjambi.qt.core.QMarginsF margins)    {
    	operator_subtract_assign(margins);
    	return this;
    }
    
    @QtBlockedSlot
    public final QRectF add(org.qtjambi.qt.core.QMarginsF margins)    {
    	operator_add_assign(margins);
    	return this;
    }*/

}// class

class QLineF___ extends QLineF {

        public final QLineF.IntersectType intersect(QLineF line, org.qtjambi.qt.core.QPointF intersectionPoint) {
            return intersect(line, intersectionPoint != null ? intersectionPoint.nativePointer() : null);
        }

}// class

class QXmlStreamWriter___ extends QXmlStreamWriter {

        public QXmlStreamWriter(org.qtjambi.qt.core.QByteArray array) {
            this(array.nativePointer());
            __rcDevice = array;
        }

        public final void setCodec(String codecName) {
            setCodec(QNativePointer.createCharPointer(codecName));
            __rcCodec = null;
        }

}// class


class QJsonDocument___{

    public static class FromJsonResult{
    	public FromJsonResult(QJsonDocument document, QJsonParseError error) {
			super();
			this.document = document;
			this.error = error;
		}
		public final org.qtjambi.qt.core.QJsonDocument document;
    	public final org.qtjambi.qt.core.QJsonParseError error;
    }
    
    public static FromJsonResult fromJson(org.qtjambi.qt.core.QByteArray json) {
    	org.qtjambi.qt.QNativePointer pointer = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Pointer);
    	org.qtjambi.qt.core.QJsonDocument result = fromJson(json, pointer);
    	QJsonParseError error = QJsonParseError.fromNativePointer(pointer);
    	return new FromJsonResult(result, error);
    }

    public static org.qtjambi.qt.core.QJsonDocument fromRawData(String data) {
        return fromRawData(data, org.qtjambi.qt.core.QJsonDocument.DataValidation.Validate);
    }
    
    public static org.qtjambi.qt.core.QJsonDocument fromRawData(String data, org.qtjambi.qt.core.QJsonDocument.DataValidation validation) {
    	return fromRawData(org.qtjambi.qt.QNativePointer.createCharPointer(data), data!=null ? data.length()+1 : -1, validation);
    }

    @QtBlockedSlot
    public final String rawData()    {
    	org.qtjambi.qt.QNativePointer size = new org.qtjambi.qt.QNativePointer(org.qtjambi.qt.QNativePointer.Type.Int);
    	org.qtjambi.qt.QNativePointer result = rawData(size);
    	return result!=null && size.intValue()>=0 ? result.stringValue() : null;
    }
}// class

class QLockFile__{
	
	public static class LockInfo {
			public LockInfo(long pid, String hostname, String appname) {
				this.pid = pid;
				this.hostname = hostname;
				this.appname = appname;
			}
			
			public long pid;
			public String hostname;
			public String appname;
	}

	@QtBlockedSlot
    public final LockInfo getLockInfo() {
		QNativePointer pid = new QNativePointer(QNativePointer.Type.Long);
		QNativePointer hostname = new QNativePointer(QNativePointer.Type.String);
		QNativePointer appname = new QNativePointer(QNativePointer.Type.String);
		boolean success = getLockInfo(pid, hostname, appname);
		return success ? new LockInfo(pid.longValue(), hostname.stringValue(), appname.stringValue()) : null;
	}
		
}// class


class QMessageAuthenticationCode__{
	@QtBlockedSlot
	public final void addData(byte[] data)    {
		QNativePointer pointer = new QNativePointer(QNativePointer.Type.Byte, data.length);
		for (int i = 0; i < data.length; i++) {
			pointer.setByteAt(i, data[i]);
		}
		addData(pointer, data.length);
	}
}// class
