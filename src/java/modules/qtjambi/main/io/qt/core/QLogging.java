/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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

package io.qt.core;

import java.util.Arrays;
import java.util.EnumMap;
import java.util.EnumSet;
import java.util.IllegalFormatException;
import java.util.Map;
import java.util.Objects;
import java.util.function.Supplier;
import java.util.logging.Level;
import java.util.logging.LogRecord;

import io.qt.NativeAccess;
import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.QNoImplementationException;
import io.qt.QtUninvokable;
import io.qt.StrictNonNull;

/**
 * <p>Java wrapper for Qt's logging functionality.</p>
 * @see <a href="http://doc.qt.io/qt/qmessagelogger.html">QMessageLogger</a>
 */
public final class QLogging {
	private QLogging() {throw new QNoImplementationException();}
	
	private static final java.util.logging.Logger logger = java.util.logging.Logger.getLogger("io.qt");
    
    static {
    	QtJambi_LibraryUtilities.initialize();
    	
    	String config = System.getProperty("io.qt.exceptions-for-messages");
        if (config != null) {
            config = config.trim().toUpperCase();
            boolean all = config.equals("") || config.equals("ALL") || config.equals("TRUE");
            EnumSet<QtMsgType> supportedMessageTypes = EnumSet.noneOf(QtMsgType.class);
            if(all || config.contains("CRITICAL")) {
            	supportedMessageTypes.add(QtMsgType.QtCriticalMsg);
            }
            if(all || config.contains("DEBUG")) {
            	supportedMessageTypes.add(QtMsgType.QtDebugMsg);
            }
            if(all || config.contains("WARNING")) {
            	supportedMessageTypes.add(QtMsgType.QtWarningMsg);
            }
            if(all || config.contains("FATAL")) {
            	supportedMessageTypes.add(QtMsgType.QtFatalMsg);
            }
            if(all || config.contains("INFO")) {
            	supportedMessageTypes.add(QtMsgType.QtInfoMsg);
            }
            if(all || config.contains("SYSTEM")) {
            	supportedMessageTypes.add(QtMsgType.QtCriticalMsg);
            }
            if(!supportedMessageTypes.isEmpty())
            	qInstallExceptionMessageHandler(supportedMessageTypes);
        }
    	config = System.getProperty("io.qt.log-messages");
    	if (config == null || config.isEmpty())
    		config = System.getProperty("io.qt.log.messages");
    		
    	if (config != null) {
            config = config.trim().toUpperCase();
            boolean all = config.equals("") || config.equals("ALL") || config.equals("TRUE");
            EnumSet<QtMsgType> supportedMessageTypes = EnumSet.noneOf(QtMsgType.class);
            if(all || config.contains("CRITICAL")) {
            	supportedMessageTypes.add(QtMsgType.QtCriticalMsg);
            }
            if(all || config.contains("DEBUG")) {
            	supportedMessageTypes.add(QtMsgType.QtDebugMsg);
            }
            if(all || config.contains("WARNING")) {
            	supportedMessageTypes.add(QtMsgType.QtWarningMsg);
            }
            if(all || config.contains("FATAL")) {
            	supportedMessageTypes.add(QtMsgType.QtFatalMsg);
            }
            if(all || config.contains("INFO")) {
            	supportedMessageTypes.add(QtMsgType.QtInfoMsg);
            }
            if(all || config.contains("SYSTEM")) {
            	supportedMessageTypes.add(QtMsgType.QtCriticalMsg);
            }
            if(!supportedMessageTypes.isEmpty())
            	qInstallLoggingMessageHandler(supportedMessageTypes);
        }
    }
    
    @NativeAccess
    private static QtMessageHandler installedMessageHandler;
    
    @NativeAccess
    private static void acceptInstalled(io.qt.core.QtMsgType msgType, io.qt.core.QMessageLogContext context, java.lang.String message){
    	installedMessageHandler.accept(msgType, context, message);
    }
    
    @NativeAccess
    private static void shutdown() {
    	installedMessageHandler = null;
    }

    /**
     * <p>Installs the specified message handler as a receiver for message notification.</p>
     * <p>Optionally, you can specify all message types treated by the handler to improve performance.
     * All message types not specified here remain hidden. 
     * Without specifying any message type here, all message types are sent to the handler.</p>
     * @see <a href="https://doc.qt.io/qt/qtglobal.html#qInstallMessageHandler">qInstallMessageHandler</a>.
     */
    @QtUninvokable
    public static @Nullable QtMessageHandler qInstallMessageHandler(@Nullable QtMessageHandler handler, @StrictNonNull QtMsgType @StrictNonNull... supportedMessageTypes) {
    	if(supportedMessageTypes==null || supportedMessageTypes.length==0)
    		return qInstallMessageHandler(handler, EnumSet.allOf(QtMsgType.class));
    	else
    		return qInstallMessageHandler(handler, EnumSet.copyOf(Arrays.asList(supportedMessageTypes)));
    }
    
    /**
     * <p>Installs an exception-throwing message handler as a receiver for message notification.</p>
     * <p>Optionally, you can specify all message types treated by the handler to improve performance.
     * All message types not specified here remain hidden. 
     * Without specifying any message type here, all message types are sent to the handler.</p>
     * <p>Alternatively, you can use the following Java VM argument to let QtJambi install an exception-throwing 
     * message handler:</p>
     * <code>-Dio.qt.exceptions-for-messages=TYPE</code>
     * <p>where <code>TYPE</code> is either <code>ALL</code> or a combination of 
     * <code>CRITICAL</code>,
     * <code>DEBUG</code>,
     * <code>FATAL</code>,
     * <code>WARNING</code> and
     * <code>INFO</code>.</p>
     * @see <a href="https://doc.qt.io/qt/qtglobal.html#qInstallMessageHandler">qInstallMessageHandler</a>.
     */
    @QtUninvokable
    public static @Nullable QtMessageHandler qInstallExceptionMessageHandler(@StrictNonNull QtMsgType @StrictNonNull... supportedMessageTypes) {
    	if(supportedMessageTypes==null || supportedMessageTypes.length==0)
    		return qInstallExceptionMessageHandler(EnumSet.allOf(QtMsgType.class));
    	else
    		return qInstallExceptionMessageHandler(EnumSet.copyOf(Arrays.asList(supportedMessageTypes)));
    }
    
    /**
     * <p>Installs a message handler using the Java logging feature as a receiver for message notification.</p>
     * <p>Optionally, you can specify all message types treated by the handler to improve performance.
     * All message types not specified here remain hidden. 
     * Without specifying any message type here, all message types are sent to the handler.</p>
     * <p>Alternatively, you can use the following Java VM argument to let QtJambi install a Java logging
     * message handler:</p>
     * <code>-Dio.qt.log-messages=TYPE</code>
     * <p>where <code>TYPE</code> is either <code>ALL</code> or a combination of 
     * <code>CRITICAL</code>,
     * <code>DEBUG</code>,
     * <code>FATAL</code>,
     * <code>WARNING</code> and
     * <code>INFO</code>.</p>
     * @see <a href="https://doc.qt.io/qt/qtglobal.html#qInstallMessageHandler">qInstallMessageHandler</a>.
     */
    @QtUninvokable
    public static @Nullable QtMessageHandler qInstallLoggingMessageHandler(@StrictNonNull QtMsgType @StrictNonNull... supportedMessageTypes) {
    	if(supportedMessageTypes==null || supportedMessageTypes.length==0)
    		return qInstallLoggingMessageHandler(EnumSet.allOf(QtMsgType.class));
    	else
    		return qInstallLoggingMessageHandler(EnumSet.copyOf(Arrays.asList(supportedMessageTypes)));
    }
    
	@QtUninvokable
    private static QtMessageHandler qInstallExceptionMessageHandler(EnumSet<QtMsgType> supportedMessageTypes) {
    	return qInstallMessageHandler((messageType, context, message) -> {
			switch(messageType) {
			case QtCriticalMsg:
				throw new RuntimeException("Critical: " + message);
			case QtDebugMsg:
				throw new RuntimeException("Debug: " + message);
			case QtFatalMsg:
				throw new Error("Fatal: " + message);
			case QtInfoMsg:
				throw new RuntimeException("Info: " + message);
			case QtWarningMsg:
				throw new RuntimeException("Warning: " + message);
			default:
				break;
			}
		}, supportedMessageTypes);
    }
    
    @QtUninvokable
    private static QtMessageHandler qInstallLoggingMessageHandler(EnumSet<QtMsgType> supportedMessageTypes) {
    	Map<QtMsgType,Level> messageLevels = new EnumMap<>(QtMsgType.class);
    	String value = System.getProperty("io.qt.log.debug.level");
		Level level = Level.FINEST;
    	if(value!=null) {
    		try {
				level = Level.parse(value);
			} catch (IllegalArgumentException e) {
			}
    	}
		messageLevels.put(QtMsgType.QtDebugMsg, level);
    	value = System.getProperty("io.qt.log.critical.level");
		level = Level.SEVERE;
    	if(value!=null) {
    		try {
				level = Level.parse(value);
			} catch (IllegalArgumentException e) {
			}
    	}
		messageLevels.put(QtMsgType.QtCriticalMsg, level);
		value = System.getProperty("io.qt.log.fatal.level");
		level = Level.SEVERE;
    	if(value!=null) {
    		try {
				level = Level.parse(value);
			} catch (IllegalArgumentException e) {
			}
    	}
		messageLevels.put(QtMsgType.QtFatalMsg, level);
		value = System.getProperty("io.qt.log.info.level");
		level = Level.INFO;
    	if(value!=null) {
    		try {
				level = Level.parse(value);
			} catch (IllegalArgumentException e) {
			}
    	}
		messageLevels.put(QtMsgType.QtInfoMsg, level);
		value = System.getProperty("io.qt.log.warning.level");
		level = Level.WARNING;
    	if(value!=null) {
    		try {
				level = Level.parse(value);
			} catch (IllegalArgumentException e) {
			}
    	}
		messageLevels.put(QtMsgType.QtWarningMsg, level);
    	return qInstallMessageHandler((messageType, context, message) -> {
			LogRecord logRecord = new LogRecord(messageLevels.getOrDefault(messageType, Level.FINE), message);
			String file = context.file();
			logRecord.setSourceClassName(file!=null ? file : "");
    		logRecord.setSourceMethodName(context.function());
    		String category = context.category();
    		if(category!=null && !category.isEmpty()) {
    			java.util.logging.Logger.getLogger(category).log(logRecord);
    		}else {
    			logger.log(logRecord);
    		}
		}, supportedMessageTypes);
    }
    
    @QtUninvokable
    private static QtMessageHandler qInstallMessageHandler(QtMessageHandler handler, EnumSet<QtMsgType> supportedMessageTypes) {
    	QtMessageHandler previousInstalledMessageHandler = installedMessageHandler;
    	installedMessageHandler = handler;
    	QtMessageHandler previous = installMessageHandler(supportedMessageTypes, handler);
    	if(previous==null)
    		previous = previousInstalledMessageHandler;
    	return previous;
    }
    
    @QtUninvokable
    private static native QtMessageHandler installMessageHandler(EnumSet<QtMsgType> supportedMessageTypes, QtMessageHandler handler);

    @QtUninvokable
    static void initialize() {
    }
    
    @QtUninvokable
    private static CharSequence format(String message, Object...args) {
    	int idx = message.indexOf('%');
    	if(idx>=0) {
    		char c = message.charAt(idx+1);
    		if(c>='0' && c<='9') {
        		while(c>='0' && c<='9') {
        			c = message.charAt((++idx)+1);
        		}
        		if(c!='$') {
        			return QString.format(message, args);
        		}    			
    		}
    	}
		try {
			return String.format(message, args);
		} catch (IllegalFormatException e) {
			return QString.format(message, args);
		}
    }
    
    @QtUninvokable
    private static Supplier<? extends CharSequence> messageSupplier(String message, Object...args) {
    	return ()->format(message, args);
    }

    /**
     * Calls the message handler with the warning message.
     * @see https://doc.qt.io/qt/qtglobal.html#qWarning
     * @param message
     */
    @QtUninvokable
    public static void qWarning(@NonNull String message) {
    	showMessage(QtMsgType.QtWarningMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the warning message and arguments.
     * @see https://doc.qt.io/qt/qtglobal.html#qWarning
     * @see String#format(String, Object...)
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qWarning(@NonNull String message, @Nullable Object @NonNull...args) {
    	showMessageFromSupplier(QtMsgType.QtWarningMsg.value(), messageSupplier(message, args));
    }
    
    /**
     * Changes the output of the default message handler.
     * @see https://doc.qt.io/qt/qtglobal.html#qSetMessagePattern
     */
    @QtUninvokable
    public static native void qSetMessagePattern(@NonNull String pattern);
    
    /**
     * @see https://doc.qt.io/qt/qtglobal.html#qFormatLogMessage
     */
    @QtUninvokable
    public static String qFormatLogMessage(@NonNull QtMsgType type, @StrictNonNull QMessageLogContext context, @NonNull String str) {
    	return qFormatLogMessage(type==null ? 0 : type.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(context)), str);
    }
    
    @QtUninvokable
    private static native String qFormatLogMessage(int type, long context, String str);
    
    @QtUninvokable
    public static void qErrnoWarning(@NonNull String message, @Nullable Object @NonNull...args) {
    	qErrnoWarning(-1, message, args);
    }
    
    @QtUninvokable
    public static void qErrnoWarning(int code, @NonNull String message, @Nullable Object @NonNull...args) {
    	qErrnoWarning(code, args==null || args.length==0 ? message : format(message, args).toString());
    }
    
    @QtUninvokable
    private native static void qErrnoWarning(int code, String message);
    
    /**
     * Calls the message handler with the debug message.
     * @see https://doc.qt.io/qt/qtglobal.html#qDebug
     * @param message
     */
    @QtUninvokable
    public static void qDebug(@NonNull String message) {
    	showMessage(QtMsgType.QtDebugMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the debug message and arguments.
     * @see https://doc.qt.io/qt/qtglobal.html#qDebug
     * @see String#format(String, Object...)
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qDebug(@NonNull String message, @Nullable Object @NonNull...args) {
    	showMessageFromSupplier(QtMsgType.QtDebugMsg.value(), messageSupplier(message, args));
    }
    
    /**
     * Calls the message handler with the info message.
     * @see https://doc.qt.io/qt/qtglobal.html#qInfo
     * @param message
     */
    @QtUninvokable
    public static void qInfo(@NonNull String message) {
    	showMessage(QtMsgType.QtInfoMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the info message and arguments.
     * @see https://doc.qt.io/qt/qtglobal.html#qInfo
     * @see String#format(String, Object...)
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qInfo(@NonNull String message, @Nullable Object @NonNull ...args) {
    	showMessageFromSupplier(QtMsgType.QtInfoMsg.value(), messageSupplier(message, args));
    }
    
    /**
     * Calls the message handler with the critical message.
     * @see https://doc.qt.io/qt/qtglobal.html#qCritical
     * @param message
     */
    @QtUninvokable
    public static void qCritical(@NonNull String message) {
    	showMessage(QtMsgType.QtCriticalMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the critical message and arguments.
     * @see https://doc.qt.io/qt/qtglobal.html#qCritical
     * @see String#format(String, Object...)
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qCritical(@NonNull String message, @Nullable Object @NonNull...args) {
    	showMessageFromSupplier(QtMsgType.QtCriticalMsg.value(), messageSupplier(message, args));
    }
    
    /**
     * Calls the message handler with the fatal message.
     * If you are using the default message handler this function will abort to create a core dump.
     * @see https://doc.qt.io/qt/qtglobal.html#qFatal
     * @param message
     */
    @QtUninvokable
    public static void qFatal(@NonNull String message) {
    	showMessage(QtMsgType.QtFatalMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the fatal message and arguments.
     * @see https://doc.qt.io/qt/qtglobal.html#qFatal
     * @see String#format(String, Object...)
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qFatal(@NonNull String message, @Nullable Object @NonNull...args) {
    	showMessageFromSupplier(QtMsgType.QtFatalMsg.value(), messageSupplier(message, args));
    }
    
    /**
     * Logs a warning message in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCWarning
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCWarning(@StrictNonNull QLoggingCategory category, @NonNull String message) {
    	showCMessage(QtMsgType.QtWarningMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), message);
    }
    
    /**
     * Logs a warning message with arguments in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCWarning
     * @param category
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qCWarning(@StrictNonNull QLoggingCategory category, @NonNull String message, @Nullable Object @NonNull...args) {
    	showCMessageFromSupplier(QtMsgType.QtWarningMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), messageSupplier(message, args));
    }
    
    /**
     * Logs a debug message in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCDebug
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCDebug(@StrictNonNull QLoggingCategory category, @NonNull String message) {
    	showCMessage(QtMsgType.QtDebugMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), message);
    }
    
    /**
     * Logs a debug message with arguments in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCDebug
     * @param category
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qCDebug(@StrictNonNull QLoggingCategory category, @NonNull String message, @Nullable Object @NonNull...args) {
    	showCMessageFromSupplier(QtMsgType.QtDebugMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), messageSupplier(message, args));
    }
    
    /**
     * Logs an info message in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCInfo
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCInfo(@StrictNonNull QLoggingCategory category, @NonNull String message) {
    	showCMessage(QtMsgType.QtInfoMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), message);
    }
    
    /**
     * Logs an info message with arguments in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCInfo
     * @param category
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qCInfo(@StrictNonNull QLoggingCategory category, @NonNull String message, @Nullable Object @NonNull...args) {
    	showCMessageFromSupplier(QtMsgType.QtInfoMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), messageSupplier(message, args));
    }
    
    /**
     * Logs a critical message in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCCritical
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCCritical(@StrictNonNull QLoggingCategory category, @NonNull String message) {
    	showCMessage(QtMsgType.QtCriticalMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), message);
    }
    
    /**
     * Logs a critical message with arguments in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCCritical
     * @param category
     * @param message
     * @param args
     */
    @QtUninvokable
    public static void qCCritical(@StrictNonNull QLoggingCategory category, @NonNull String message, @Nullable Object @NonNull...args) {
    	showCMessageFromSupplier(QtMsgType.QtCriticalMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), messageSupplier(message, args));
    }
    
    /**
     * Calls the message handler with the warning message provided by {@link Supplier}.
     * @see https://doc.qt.io/qt/qtglobal.html#qWarning
     * @param message
     */
    @QtUninvokable
    public static void qWarning(@StrictNonNull Supplier<? extends CharSequence> message) {
    	showMessageFromSupplier(QtMsgType.QtWarningMsg.value(), Objects.requireNonNull(message));
    }
    
    /**
     * Calls the message handler with the debug message provided by {@link Supplier}.
     * @see https://doc.qt.io/qt/qtglobal.html#qDebug
     * @param message
     */
    @QtUninvokable
    public static void qDebug(@StrictNonNull Supplier<? extends CharSequence> message) {
    	showMessageFromSupplier(QtMsgType.QtDebugMsg.value(), Objects.requireNonNull(message));
    }
    
    /**
     * Calls the message handler with the info message provided by {@link Supplier}.
     * @see https://doc.qt.io/qt/qtglobal.html#qInfo
     * @param message
     */
    @QtUninvokable
    public static void qInfo(@StrictNonNull Supplier<? extends CharSequence> message) {
    	showMessageFromSupplier(QtMsgType.QtInfoMsg.value(), Objects.requireNonNull(message));
    }
    
    /**
     * Calls the message handler with the critical message provided by {@link Supplier}.
     * @see https://doc.qt.io/qt/qtglobal.html#qCritical
     * @param message
     */
    @QtUninvokable
    public static void qCritical(@StrictNonNull Supplier<? extends CharSequence> message) {
    	showMessageFromSupplier(QtMsgType.QtCriticalMsg.value(), Objects.requireNonNull(message));
    }
    
    /**
     * Calls the message handler with the fatal message provided by {@link Supplier}.
     * If you are using the default message handler this function will abort to create a core dump.
     * @see https://doc.qt.io/qt/qtglobal.html#qFatal
     * @param message
     */
    @QtUninvokable
    public static void qFatal(@StrictNonNull Supplier<? extends CharSequence> message) {
    	showMessageFromSupplier(QtMsgType.QtFatalMsg.value(), Objects.requireNonNull(message));
    }
    
    /**
     * Logs a warning message provided by {@link Supplier} in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCWarning
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCWarning(@StrictNonNull QLoggingCategory category, @StrictNonNull Supplier<? extends CharSequence> message) {
    	showCMessageFromSupplier(QtMsgType.QtWarningMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), Objects.requireNonNull(message));
    }
    
    /**
     * Logs a debug message provided by {@link Supplier} in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCDebug
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCDebug(@StrictNonNull QLoggingCategory category, @StrictNonNull Supplier<? extends CharSequence> message) {
    	showCMessageFromSupplier(QtMsgType.QtDebugMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), Objects.requireNonNull(message));
    }
    
    /**
     * Logs a info message provided by {@link Supplier} in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCInfo
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCInfo(@StrictNonNull QLoggingCategory category, @StrictNonNull Supplier<? extends CharSequence> message) {
    	showCMessageFromSupplier(QtMsgType.QtInfoMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), Objects.requireNonNull(message));
    }
    
    /**
     * Logs a critical message provided by {@link Supplier} in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCCritical
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCCritical(@StrictNonNull QLoggingCategory category, @StrictNonNull Supplier<? extends CharSequence> message) {
    	showCMessageFromSupplier(QtMsgType.QtCriticalMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), Objects.requireNonNull(message));
    }
    
    /**
     * Logs a warning message using a QDebug stream
     * @return QDebug stream
     */
    @QtUninvokable
    public static @NonNull QDebug qWarning() {
    	return getDebug(QtMsgType.QtWarningMsg.value());
    }
    
    /**
     * Logs a debug message using a QDebug stream
     * @return QDebug stream
     */
    @QtUninvokable
    public static @NonNull QDebug qDebug() {
    	return getDebug(QtMsgType.QtDebugMsg.value());
    }
    
    /**
     * Logs an info message using a QDebug stream
     * @return QDebug stream
     */
    @QtUninvokable
    public static @NonNull QDebug qInfo() {
    	return getDebug(QtMsgType.QtInfoMsg.value());
    }
    
    /**
     * Logs a critical message using a QDebug stream
     * @return QDebug stream
     */
    @QtUninvokable
    public static @NonNull QDebug qCritical() {
    	return getDebug(QtMsgType.QtCriticalMsg.value());
    }
    
    /**
     * Logs a warning message into given category using a QDebug stream
     * @param category
     * @return QDebug stream
     */
    @QtUninvokable
    public static @NonNull QDebug qCWarning(@StrictNonNull QLoggingCategory category) {
    	return getCDebug(QtMsgType.QtWarningMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)));
    }
    
    /**
     * Logs a debug message into given category using a QDebug stream
     * @param category
     * @return QDebug stream
     */
    @QtUninvokable
    public static @NonNull QDebug qCDebug(@StrictNonNull QLoggingCategory category) {
    	return getCDebug(QtMsgType.QtDebugMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)));
    }
    
    /**
     * Logs an info message into given category using a QDebug stream
     * @param category
     * @return QDebug stream
     */
    @QtUninvokable
    public static @NonNull QDebug qCInfo(@StrictNonNull QLoggingCategory category) {
    	return getCDebug(QtMsgType.QtInfoMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)));
    }
    
    /**
     * Logs a critical message into given category using a QDebug stream
     * @param category
     * @return QDebug stream
     */
    @QtUninvokable
    public static @NonNull QDebug qCCritical(@StrictNonNull QLoggingCategory category) {
    	return getCDebug(QtMsgType.QtCriticalMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)));
    }
    
    @QtUninvokable
    private native static QDebug getDebug(int messageType);
    @QtUninvokable
    private native static QDebug getCDebug(int messageType, long categoryId);
    @QtUninvokable
    private native static void showMessage(int messageType, String message);
    @QtUninvokable
    private native static void showMessageFromSupplier(int messageType, Supplier<? extends CharSequence> message);
    
    @QtUninvokable
    private native static void showCMessage(int messageType, long categoryId, String message);
    @QtUninvokable
    private native static void showCMessageFromSupplier(int messageType, long categoryId, Supplier<? extends CharSequence> message);
}
