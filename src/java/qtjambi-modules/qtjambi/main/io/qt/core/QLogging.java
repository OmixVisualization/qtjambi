/****************************************************************************
 **
 ** Copyright (C) 1992-2009 Nokia. All rights reserved.
 ** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.core;

import java.util.Arrays;
import java.util.EnumSet;
import java.util.IllegalFormatException;
import java.util.Objects;
import java.util.Set;
import java.util.function.Supplier;
import java.util.logging.Level;
import java.util.logging.LogRecord;

import io.qt.QNoImplementationException;
import io.qt.QtUninvokable;

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
            	supportedMessageTypes.add(QtMsgType.QtSystemMsg);
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
            	supportedMessageTypes.add(QtMsgType.QtSystemMsg);
            }
            if(!supportedMessageTypes.isEmpty())
            	qInstallLoggingMessageHandler(supportedMessageTypes);
        }
    }

    /**
     * <p>Installs the specified message handler as a receiver for message notification.</p>
     * <p>Optionally, you can specify all message types treated by the handler to improve performance.
     * All message types not specified here remain hidden. 
     * Without specifying any message type here, all message types are sent to the handler.</p>
     * @see <a href="https://doc.qt.io/qt/qtglobal.html#qInstallMessageHandler">qInstallMessageHandler</a>.
     */
    @QtUninvokable
    public static QtMessageHandler qInstallMessageHandler(QtMessageHandler handler, QtMsgType... supportedMessageTypes) {
    	if(supportedMessageTypes==null || supportedMessageTypes.length==0)
    		return qInstallMessageHandler(EnumSet.allOf(QtMsgType.class), handler);
    	else
    		return qInstallMessageHandler(EnumSet.copyOf(Arrays.asList(supportedMessageTypes)), handler);
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
    public static QtMessageHandler qInstallExceptionMessageHandler(QtMsgType... supportedMessageTypes) {
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
    public static QtMessageHandler qInstallLoggingMessageHandler(QtMsgType... supportedMessageTypes) {
    	if(supportedMessageTypes==null || supportedMessageTypes.length==0)
    		return qInstallLoggingMessageHandler(EnumSet.allOf(QtMsgType.class));
    	else
    		return qInstallLoggingMessageHandler(EnumSet.copyOf(Arrays.asList(supportedMessageTypes)));
    }
    
    @QtUninvokable
    private static QtMessageHandler qInstallExceptionMessageHandler(Set<QtMsgType> supportedMessageTypes) {
    	return qInstallMessageHandler(supportedMessageTypes, (messageType, context, message) -> {
			switch(messageType) {
			case QtSystemMsg:
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
		});
    }
    
    @QtUninvokable
    private static QtMessageHandler qInstallLoggingMessageHandler(Set<QtMsgType> supportedMessageTypes) {
    	return qInstallMessageHandler(supportedMessageTypes, (messageType, context, message) -> {
			Level level;
			switch(messageType) {
			case QtSystemMsg:
			case QtCriticalMsg:
				level = Level.SEVERE;
				break;
			case QtDebugMsg:
				level = Level.FINEST;
				break;
			case QtFatalMsg:
				level = Level.SEVERE;
				break;
			case QtInfoMsg:
				level = Level.INFO;
				break;
			case QtWarningMsg:
				level = Level.WARNING;
				break;
			default:
				level = Level.FINE;
				break;
			}
			LogRecord logRecord = new LogRecord(level, message);
			String file = context.file();
			logRecord.setSourceClassName(file!=null ? file : "");
    		logRecord.setSourceMethodName(context.function());
    		logger.log(logRecord);
		});
    }
    
    @QtUninvokable
    private static native QtMessageHandler qInstallMessageHandler(Set<QtMsgType> supportedMessageTypes, QtMessageHandler handler);

    @QtUninvokable
    static void initialize() {
    }
    
    @QtUninvokable
    private static Supplier<String> format(String message, Object...args) {
    	return ()->{
    		try {
				return String.format(message, args);
			} catch (IllegalFormatException e) {
				return QString.format(message, args).toString();
			}
    	};
    }

    /**
     * Calls the message handler with the warning message.
     * @see https://doc.qt.io/qt/qtglobal.html#qWarning
     * @param message
     */
    @QtUninvokable
    public static void qWarning(String message) {
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
    public static void qWarning(String message, Object...args) {
    	showMessageFromSupplier(QtMsgType.QtWarningMsg.value(), format(message, args));
    }
    
    /**
     * Calls the message handler with the debug message.
     * @see https://doc.qt.io/qt/qtglobal.html#qDebug
     * @param message
     */
    @QtUninvokable
    public static void qDebug(String message) {
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
    public static void qDebug(String message, Object...args) {
    	showMessageFromSupplier(QtMsgType.QtDebugMsg.value(), format(message, args));
    }
    
    /**
     * Calls the message handler with the info message.
     * @see https://doc.qt.io/qt/qtglobal.html#qInfo
     * @param message
     */
    @QtUninvokable
    public static void qInfo(String message) {
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
    public static void qInfo(String message, Object...args) {
    	showMessageFromSupplier(QtMsgType.QtInfoMsg.value(), format(message, args));
    }
    
    /**
     * Calls the message handler with the critical message.
     * @see https://doc.qt.io/qt/qtglobal.html#qCritical
     * @param message
     */
    @QtUninvokable
    public static void qCritical(String message) {
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
    public static void qCritical(String message, Object...args) {
    	showMessageFromSupplier(QtMsgType.QtCriticalMsg.value(), format(message, args));
    }
    
    /**
     * Calls the message handler with the fatal message.
     * If you are using the default message handler this function will abort to create a core dump.
     * @see https://doc.qt.io/qt/qtglobal.html#qFatal
     * @param message
     */
    @QtUninvokable
    public static void qFatal(String message) {
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
    public static void qFatal(String message, Object...args) {
    	showMessageFromSupplier(QtMsgType.QtFatalMsg.value(), format(message, args));
    }
    
    /**
     * Logs a warning message in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCWarning
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCWarning(QLoggingCategory category, String message) {
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
    public static void qCWarning(QLoggingCategory category, String message, Object...args) {
    	showCMessageFromSupplier(QtMsgType.QtWarningMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), format(message, args));
    }
    
    /**
     * Logs a debug message in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCDebug
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCDebug(QLoggingCategory category, String message) {
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
    public static void qCDebug(QLoggingCategory category, String message, Object...args) {
    	showCMessageFromSupplier(QtMsgType.QtDebugMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), format(message, args));
    }
    
    /**
     * Logs an info message in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCInfo
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCInfo(QLoggingCategory category, String message) {
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
    public static void qCInfo(QLoggingCategory category, String message, Object...args) {
    	showCMessageFromSupplier(QtMsgType.QtInfoMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), format(message, args));
    }
    
    /**
     * Logs a critical message in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCCritical
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCCritical(QLoggingCategory category, String message) {
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
    public static void qCCritical(QLoggingCategory category, String message, Object...args) {
    	showCMessageFromSupplier(QtMsgType.QtCriticalMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), format(message, args));
    }
    
    /**
     * Calls the message handler with the warning message provided by {@link Supplier}.
     * @see https://doc.qt.io/qt/qtglobal.html#qWarning
     * @param message
     */
    @QtUninvokable
    public static void qWarning(Supplier<String> message) {
    	showMessageFromSupplier(QtMsgType.QtWarningMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the debug message provided by {@link Supplier}.
     * @see https://doc.qt.io/qt/qtglobal.html#qDebug
     * @param message
     */
    @QtUninvokable
    public static void qDebug(Supplier<String> message) {
    	showMessageFromSupplier(QtMsgType.QtDebugMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the info message provided by {@link Supplier}.
     * @see https://doc.qt.io/qt/qtglobal.html#qInfo
     * @param message
     */
    @QtUninvokable
    public static void qInfo(Supplier<String> message) {
    	showMessageFromSupplier(QtMsgType.QtInfoMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the critical message provided by {@link Supplier}.
     * @see https://doc.qt.io/qt/qtglobal.html#qCritical
     * @param message
     */
    @QtUninvokable
    public static void qCritical(Supplier<String> message) {
    	showMessageFromSupplier(QtMsgType.QtCriticalMsg.value(), message);
    }
    
    /**
     * Calls the message handler with the fatal message provided by {@link Supplier}.
     * If you are using the default message handler this function will abort to create a core dump.
     * @see https://doc.qt.io/qt/qtglobal.html#qFatal
     * @param message
     */
    @QtUninvokable
    public static void qFatal(Supplier<String> message) {
    	showMessageFromSupplier(QtMsgType.QtFatalMsg.value(), message);
    }
    
    /**
     * Logs a warning message provided by {@link Supplier} in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCWarning
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCWarning(QLoggingCategory category, Supplier<String> message) {
    	showCMessageFromSupplier(QtMsgType.QtWarningMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), message);
    }
    
    /**
     * Logs a debug message provided by {@link Supplier} in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCDebug
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCDebug(QLoggingCategory category, Supplier<String> message) {
    	showCMessageFromSupplier(QtMsgType.QtDebugMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), message);
    }
    
    /**
     * Logs a info message provided by {@link Supplier} in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCInfo
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCInfo(QLoggingCategory category, Supplier<String> message) {
    	showCMessageFromSupplier(QtMsgType.QtInfoMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), message);
    }
    
    /**
     * Logs a critical message provided by {@link Supplier} in the logging category.
     * @see https://doc.qt.io/qt/qtglobal.html#qCCritical
     * @param category
     * @param message
     */
    @QtUninvokable
    public static void qCCritical(QLoggingCategory category, Supplier<String> message) {
    	showCMessageFromSupplier(QtMsgType.QtCriticalMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)), message);
    }
    
    /**
     * Logs a warning message using a QDebug stream
     * @return QDebug stream
     */
    @QtUninvokable
    public static QDebug qWarning() {
    	return getDebug(QtMsgType.QtWarningMsg.value());
    }
    
    /**
     * Logs a debug message using a QDebug stream
     * @return QDebug stream
     */
    @QtUninvokable
    public static QDebug qDebug() {
    	return getDebug(QtMsgType.QtDebugMsg.value());
    }
    
    /**
     * Logs an info message using a QDebug stream
     * @return QDebug stream
     */
    @QtUninvokable
    public static QDebug qInfo() {
    	return getDebug(QtMsgType.QtInfoMsg.value());
    }
    
    /**
     * Logs a critical message using a QDebug stream
     * @return QDebug stream
     */
    @QtUninvokable
    public static QDebug qCritical() {
    	return getDebug(QtMsgType.QtCriticalMsg.value());
    }
    
    /**
     * Logs a warning message into given category using a QDebug stream
     * @param category
     * @return QDebug stream
     */
    @QtUninvokable
    public static QDebug qCWarning(QLoggingCategory category) {
    	return getCDebug(QtMsgType.QtWarningMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)));
    }
    
    /**
     * Logs a debug message into given category using a QDebug stream
     * @param category
     * @return QDebug stream
     */
    @QtUninvokable
    public static QDebug qCDebug(QLoggingCategory category) {
    	return getCDebug(QtMsgType.QtDebugMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)));
    }
    
    /**
     * Logs an info message into given category using a QDebug stream
     * @param category
     * @return QDebug stream
     */
    @QtUninvokable
    public static QDebug qCInfo(QLoggingCategory category) {
    	return getCDebug(QtMsgType.QtInfoMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)));
    }
    
    /**
     * Logs a critical message into given category using a QDebug stream
     * @param category
     * @return QDebug stream
     */
    @QtUninvokable
    public static QDebug qCCritical(QLoggingCategory category) {
    	return getCDebug(QtMsgType.QtCriticalMsg.value(), QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(category)));
    }
    
    @QtUninvokable
    private native static QDebug getDebug(int messageType);
    @QtUninvokable
    private native static QDebug getCDebug(int messageType, long categoryId);
    @QtUninvokable
    private native static void showMessage(int messageType, String message);
    @QtUninvokable
    private native static void showMessageFromSupplier(int messageType, Supplier<String> message);
    
    @QtUninvokable
    private native static void showCMessage(int messageType, long categoryId, String message);
    @QtUninvokable
    private native static void showCMessageFromSupplier(int messageType, long categoryId, Supplier<String> message);
}
