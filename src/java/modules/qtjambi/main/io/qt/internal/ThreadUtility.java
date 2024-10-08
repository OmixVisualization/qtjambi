/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.function.BiConsumer;

import io.qt.NativeAccess;
import io.qt.core.QThread;

/**
 * @hidden
 */
abstract class ThreadUtility {
    
	private ThreadUtility() {
		throw new RuntimeException();
	}

	private static final Object threadInterruptibleSetterMonitor = new Object();
	private static BiConsumer<Thread,Object> threadInterruptibleSetter;
	private static Object interruptible;
	
	private static Object interruptibleInvoke(Object proxy, Method method, Object[] args){
		if(args.length==1 && args[0] instanceof Thread) {
            Thread _thread = (Thread)args[0];
            if(_thread.isAlive()) {
            	try {
	                QThread _qthread = QThread.thread(_thread);
	                if(_qthread!=null && !_qthread.isDisposed()){
	                	Object monitor = NativeUtility.findInterfaceLink(_qthread, true);
	                	if(monitor!=null) {
		                	synchronized(monitor){
		                		if(!_qthread.isInterruptionRequested())
		                			_qthread.requestInterruption();
		                    }
	                	}
	                }
                } catch (Throwable e) {}
            }
        }
        return null;
	}
	
	private static void empty(Thread t,Object o) {}

	@NativeAccess
	private static void setThreadInterruptible(QThread qthread, Thread thread, boolean set) {
		BiConsumer<Thread,Object> setter;
		synchronized(threadInterruptibleSetterMonitor){
			if(threadInterruptibleSetter==null) {
				for(Field field : Thread.class.getDeclaredFields()) {
					if(!Modifier.isStatic(field.getModifiers())) {
						switch(field.getType().getName()) {
						case "sun.nio.ch.Interruptible":
							try {
					            interruptible = java.lang.reflect.Proxy.newProxyInstance(
					            		field.getType().getClassLoader(), 
					                    new Class[] { field.getType() }, 
					                    ThreadUtility::interruptibleInvoke);
					        } catch (Throwable e) {
					        }
							threadInterruptibleSetter = ReflectionUtility.methodInvocationHandler.getFieldSetter(field);
							break;
						}
					}
				}
				if(threadInterruptibleSetter==null) {
					threadInterruptibleSetter = ThreadUtility::empty;
				}
			}
			setter = threadInterruptibleSetter;
		}
		Object monitor = qthread==null ? null : NativeUtility.findInterfaceLink(qthread, true);
		if(monitor==null){
			setter.accept(thread, set ? interruptible : null);
		}else {
			synchronized(monitor){
				setter.accept(thread, set ? interruptible : null);
			}
		}
	}
}
