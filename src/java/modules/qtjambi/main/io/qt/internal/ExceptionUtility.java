/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.internal;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.PrintStream;
import java.lang.Thread.UncaughtExceptionHandler;
import java.util.logging.LogRecord;

import io.qt.NativeAccess;

/**
 * @hidden
 */
abstract class ExceptionUtility {
	
	private ExceptionUtility() {
		throw new RuntimeException();
	}
	
	static {
		QtJambi_LibraryUtilities.initialize();
	}
	
	private static class StaticContainer{
		private static final Class<?> ThreadDeathClass;
		static {
			Class<?> threadDeathClass = null;
			try {
				threadDeathClass = Class.forName("java.lang.ThreadDeath");
			}catch(Throwable t) {}
			ThreadDeathClass = threadDeathClass;
		}
	}
	
	@NativeAccess
	private static byte[] printException(Throwable e) throws IOException {
		try(ByteArrayOutputStream s = new ByteArrayOutputStream();
			PrintStream p = new PrintStream(s)){
			e.printStackTrace(p);
			return s.toByteArray();
		}
	}
	
//	@NativeAccess
//	private static byte[] printStackTrace() throws IOException {
//		try(ByteArrayOutputStream s = new ByteArrayOutputStream();
//			PrintStream p = new PrintStream(s)){
//			e.printStackTrace(p);
//			return s.toByteArray();
//		}
//	}

	@NativeAccess
	private static void reportException(String methodName, Throwable e) {
		if(StaticContainer.ThreadDeathClass==null || !StaticContainer.ThreadDeathClass.isInstance(e))
		try {
			UncaughtExceptionHandler handler = Thread.currentThread().getUncaughtExceptionHandler();
			while(handler != null && (Object)AccessUtility.instance.getClass(handler)==ThreadGroup.class) {
				try {
					ThreadGroup tg = (ThreadGroup)handler;
					handler = tg.getParent();
				}catch(Throwable e1) {break;}
			}
			if(handler==null)
                handler = Thread.getDefaultUncaughtExceptionHandler();
			if (handler != null 
					&& (Object)AccessUtility.instance.getClass(handler)!=ThreadGroup.class
					&& !AccessUtility.instance.getClass(handler).getName().startsWith("com.android")) {
				handler.uncaughtException(Thread.currentThread(), e);
			} else{
				LogRecord logRecord = new LogRecord(java.util.logging.Level.SEVERE, AccessUtility.instance.getClass(e).getTypeName()+(methodName==null ? " has been thrown" : " has been thrown in "+methodName));
				logRecord.setThrown(e);
				if(methodName!=null) {
					logRecord.setSourceClassName(methodName);
		    		logRecord.setSourceMethodName("");
				}
	    		java.util.logging.Logger.getLogger("io.qt").log(logRecord);
			}
		} catch (Throwable e1) {
			e1.addSuppressed(e);
			e1.printStackTrace();
		}
	}

	/**
	 * This method converts a native std::exception to it's causing java exception
	 * if any.
	 * 
	 * @param exception
	 */
	@NativeAccess
	private native static Throwable convertNativeException(long exception);

	@NativeAccess
	private static void extendStackTrace(Throwable e, String methodName, String fileName, int lineNumber) {
		if (fileName == null)
			return;
		fileName = new java.io.File(fileName).getName();
		StackTraceElement[] threadStackTrace = Thread.currentThread().getStackTrace();
		StackTraceElement[] stackTrace = e.getStackTrace();
		StackTraceElement[] newStackTrace = new StackTraceElement[stackTrace.length + 1];
		int cursor = 0;
		for (; cursor < stackTrace.length && cursor < threadStackTrace.length; ++cursor) {
			if (!stackTrace[stackTrace.length - cursor - 1]
					.equals(threadStackTrace[threadStackTrace.length - cursor - 1])) {
				break;
			}
		}
		StackTraceElement newElement = new StackTraceElement("<native>",
				methodName == null ? "<unknown_method>" : methodName, fileName, lineNumber);
		if (cursor > 0) {
			System.arraycopy(stackTrace, 0, newStackTrace, 0, stackTrace.length - cursor);
			newStackTrace[stackTrace.length - cursor] = newElement;
			System.arraycopy(stackTrace, stackTrace.length - cursor, newStackTrace, stackTrace.length - cursor + 1,
					cursor);
		} else {
			System.arraycopy(stackTrace, 0, newStackTrace, 0, stackTrace.length);
			newStackTrace[stackTrace.length] = newElement;
		}
		e.setStackTrace(newStackTrace);
	}
}
