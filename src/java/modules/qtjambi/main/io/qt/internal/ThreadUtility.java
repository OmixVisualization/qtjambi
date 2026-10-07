/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import java.lang.reflect.*;
import java.util.function.*;

import io.qt.NativeAccess;
import io.qt.core.QThread;

/**
 * @hidden
 */
abstract class ThreadUtility {
    
	private ThreadUtility() {
		throw new RuntimeException();
	}

	private static BiConsumer<NativeUtility.Object,Thread> threadInterruptibleSetter;
	private static BiConsumer<NativeUtility.Object,Thread> threadInterruptibleResetter;
	static {
		Object interruptible = null;
		BiConsumer<Thread,Object> setter = null;
		for(Field field : Thread.class.getDeclaredFields()) {
			if(!Modifier.isStatic(field.getModifiers())) {
				switch(field.getType().getName()) {
				case "sun.nio.ch.Interruptible":
					try {
			            interruptible = java.lang.reflect.Proxy.newProxyInstance(
			            		field.getType().getClassLoader(), 
			                    new Class[] { field.getType() }, 
			                    (proxy, method, args) -> {
			            			if(args!=null && args.length==1 && args[0] instanceof Thread) {
			            	            Thread _thread = (Thread)args[0];
			            	            if(_thread.isAlive()) {
			            	            	try {
			            		                QThread _qthread = QThread.thread(_thread);
			            		                NativeUtility.Object no = _qthread;
			            		                if(no!=null && !no.isDisposed()){
		            			                	synchronized(NativeUtility.monitor(no)){
		            			                		if(!_qthread.isInterruptionRequested())
		            			                			_qthread.requestInterruption();
		            			                    }
			            		                }
			            	                } catch (Throwable e) {}
			            	            }
			            	        }
			            	        return null;
			            		});
						setter = ReflectionUtility.methodInvocationHandler.getFieldSetter(field);
			        } catch (Throwable e) {
			        }
					break;
				}
			}
		}
		if(interruptible!=null && setter!=null) {
			BiConsumer<Thread,Object> c = setter;
			Object o = interruptible;
			threadInterruptibleSetter = (no, t) -> {
				if(no!=null) {
					synchronized(NativeUtility.monitor(no)){
						c.accept(t, o);
					}
				}
			};
			threadInterruptibleResetter = (no, t) -> {
				if(no!=null) {
					synchronized(NativeUtility.monitor(no)){
						c.accept(t, null);
					}
				}
			};
		}else {
			threadInterruptibleSetter = ThreadUtility::empty;
			threadInterruptibleResetter = ThreadUtility::empty;
		}
	}
	
	private static void empty(NativeUtility.Object qthread, Thread t) {}
	
	@NativeAccess
	private static void setThreadInterruptible(QThread qthread, Thread thread, boolean set) {
		if(set) {
			threadInterruptibleSetter.accept(qthread, thread);
		}else {
			threadInterruptibleResetter.accept(qthread, thread);
		}
	}
}
