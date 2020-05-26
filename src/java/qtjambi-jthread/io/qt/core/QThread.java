/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.core;

import java.util.List;

import io.qt.QtBlockedSlot;
import io.qt.QtObjectInterface;
import io.qt.core.QAbstractEventDispatcher;
import io.qt.core.QByteArray;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QRegExp;
import io.qt.core.Qt.TimerType;

/**
 * The QThread class extends the java.lang.Thread class and should be used when
 * QObjects, i.e., instances of classes that inherit from QObject, are used in
 * threads. Thread may be used for all other classes that inherit from QtJambiObject.
 * <p>
 * QObjects have object affinity, i.e., they belong to a given thread, which is
 * accessed through their thread() method. It is only allowed to access a QObject
 * from the thread to which it belongs.
 * <p>
 * The QThread class was introduced to ensure that native resources are freed when
 * QObjects are garbage collected. The garbage collector thread posts an event to
 * the native QObject, which then deletes itself. Before exiting, the thread will
 * flush all events - causing all native QObjects to be deleted.
 * <p>
 * QThread has two convenience signals: starting and finished. Started is emitted
 * just before the runnable target is invoked. Finished is emitted just before the
 * thread shuts down - after the execution of the runnable target and the flushing
 * of the event loop.
 *
 * @See io.qt.core.QObject#thread()
 * @See io.qt.core.QThreadAffinityException
 * @See <a href="../threads.html">Threading support in Qt</a>
 *
 * @author gunnar
 */
public final class QThread extends Thread implements QtObjectInterface {
    static {
        io.qt.core.QtJambi_LibraryInitializer.init();
    }
    
    @io.qt.QtBlockedEnum
    public enum Priority implements io.qt.QtEnumerator {
        IdlePriority(0),
        LowestPriority(1),
        LowPriority(2),
        NormalPriority(3),
        HighPriority(4),
        HighestPriority(5),
        TimeCriticalPriority(6),
        InheritPriority(7);

        private Priority(int value) { this.value = value; }
        public int value() { return value; }

        public static Priority resolve(int value) {
            switch (value) {
            case 0: return IdlePriority;
            case 1: return LowestPriority;
            case 2: return LowPriority;
            case 3: return NormalPriority;
            case 4: return HighPriority;
            case 5: return HighestPriority;
            case 6: return TimeCriticalPriority;
            case 7: return InheritPriority;
            default: throw new io.qt.QNoSuchEnumValueException(value);
            }
        }
        private final int value;

    }
    
    public static final io.qt.core.QMetaObject staticMetaObject = io.qt.core.QMetaObject.forType(Wrapper.class);
    
    private static QEvent.Type InvokeInThread = QEvent.Type.resolve(QEvent.registerEventType(), "InvokeInThread");

    /**
     * The starting signal is emitted when before the QThread invokes
     * its runnable target. The signal is emitted from the running
     * thread.
     */
    public final QObject.PrivateSignal0 started;


    /**
     * The finished signal is emitted after the QThread has finished
     * executing its runnable target. The signal is emitted from the
     * running thread.
     */
    public final QObject.PrivateSignal0 finished;
    
    public final QObject.PrivateSignal1<java.lang.String> objectNameChanged;
    
    public final QMetaObject.AbstractPrivateSignal0 disposed;
    
    private final Wrapper wrapper;
    {
        wrapper = getQThreadObject(this);
        io.qt.internal.QtJambiInternal.setJavaOwnership(wrapper);
        wrapper.objectNameChanged.connect(this::setName);
        started = wrapper.started;
        finished = wrapper.finished;
        disposed = wrapper.disposed;
        objectNameChanged = wrapper.objectNameChanged;
    }
    
    private final boolean runExec;
    
    public QThread() {
        super();
        runExec = true;
    }
    
    public QThread(ThreadGroup group) {
        super(group, (Runnable)null);
        runExec = true;
    }
    
    public QThread(String name) {
        super(name);
        runExec = true;
    }
    
    public QThread(ThreadGroup group, String name) {
        super(group, name);
        runExec = true;
    }
    
    private QThread(Runnable target) {
        super(target);
        runExec = false;
    }
    
    /**
     * Creates a new QThread with the specified invokable target
     * @param target The invokable target.
     */
    public static QThread create(Runnable target) {
        return new QThread(target);
    }

    private QThread(ThreadGroup group, Runnable target) {
        super(group, target);
        runExec = false;
    }
    
    /**
     * Creates a new QThread with the specified invokable target and thread group.
     * @param group The thread group.
     * @param target The target.
     */
    public static QThread create(ThreadGroup group, Runnable target) {
        return new QThread(group, target);
    }

    private QThread(Runnable target, String name) {
        super(target, name);
        runExec = false;
    }

    /**
     * Creates a new QThread with the specified invokable target and the given name.
     * @param target The invokable target.
     * @param name The name.
     */
    public static QThread create(Runnable target, String name) {
        return new QThread(target, name);
    }

    private QThread(ThreadGroup group, Runnable target, String name) {
        super(group, target, name);
        runExec = false;
    }
    
    /**
     * Creates a new QThread with the specified invokable target, name and thread group.
     * @param group The Thread group
     * @param target The invokable target
     * @param name The name.
     */
    public static QThread create(ThreadGroup group, Runnable target, String name) {
        return new QThread(group, target, name);
    }

    private QThread(ThreadGroup group, Runnable target, String name, long stackSize) {
        super(group, target, name, stackSize);
        runExec = false;
    }

    /**
     * Creates a new QThread with the specified invokable target, name, thread group and stack size.
     * @param group The Thread group
     * @param target The invokable target
     * @param name The name.
     * @param stackSize The stack size.
     */
    public static QThread create(ThreadGroup group, Runnable target, String name, long stackSize) {
        return new QThread(group, target, name, stackSize);
    }

    /**
     * Called by the thread to invoke the runnable target.
     *
     * This method emits starting before the runnable  is called and finished after
     * the runnable is called. After the runnable is called and before finished is emitted the
     * thread will flush any pending events in this thread. This will ensure cleanup of objects
     * that are deleted via disposeLater() or similar.
     *
     * @see io.qt.core.QObject#disposeLater()
     */
    @Override
    @QtBlockedSlot
    public final void run() {
        if(QCoreApplication.instance()==null){
            throw new RuntimeException("QThread cannot be used without QApplication!");
        }
        boolean isActive = Thread.currentThread()==this;
        if(isActive){
            wrapper.preStarted();
            wrapper.blockSignals(true);
            wrapper.setObjectName(this.getName());
            wrapper.blockSignals(false);
            wrapper.started();
        }

        try {
            if(isActive){
                final Throwable[] exception = {null};
                QObject invoker = null;
                if(!runExec){
                    invoker = new QObject(){
                        public final boolean event(io.qt.core.QEvent e) {
                            if (e.type() == InvokeInThread) {
                                try{
                                    runAndQuit();
                                }catch(Throwable exn){
                                    exception[0] = exn;
                                }finally {
                                    dispose();
                                }
                                return true;
                            }
                            return super.event(e);
                        }
                    };
                    QCoreApplication.postEvent(invoker, new QEvent(InvokeInThread), Integer.MAX_VALUE);
                }
                setTerminationEnabled();
                wrapper.exec();
                QCoreApplication.processEvents();
                QAbstractEventDispatcher.instance().closingDown();
                if(exception[0]!=null){
                    throw exception[0];
                }
            }else{
                if(runExec){
                    wrapper.exec();
                }else{
                    super.run();
                }
            }
        } catch(RuntimeException e) {
            throw e;
        } catch(Error e) {
            throw e;
        } catch(Throwable e) {
            if(super.getUncaughtExceptionHandler()!=null) {
                super.getUncaughtExceptionHandler().uncaughtException(this, e);
            }else if(Thread.getDefaultUncaughtExceptionHandler()!=null) {
                Thread.getDefaultUncaughtExceptionHandler().uncaughtException(this, e);
            }else {
                throw new RuntimeException(e);
            }
        } finally {
            System.gc();
            QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());

            if(isActive && !wrapper.isDisposed()){
                // never emit signal after deleting thread object!
                wrapper.finished();
            }
        }
    }
    
    private void runAndQuit(){
        try{
            super.run();
        }finally{
            quit();
        }
    }
    
    @QtBlockedSlot
    public void invoke(Runnable runnable){
        if(this.isAlive()){
            QCoreApplication.invokeLater(this.wrapper, runnable);
        }else{
            runnable.run();
        }
    }
    
    private static void setTerminationEnabled(){
        setTerminationEnabled(true);
    }
    
    private static native void setTerminationEnabled(boolean enabled);
    
    public final static class Wrapper extends QObject {
        private final Thread thread;
        public final PrivateSignal0 started = new PrivateSignal0();
        public final PrivateSignal0 finished = new PrivateSignal0();
        
        @QtBlockedSlot
        private final void preStarted()    {
            __qt_preStarted(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
        private native void __qt_preStarted(long __this__nativeId);
        
        @QtBlockedSlot
        private final void started()    {
            __qt_started(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
        private native void __qt_started(long __this__nativeId);
        
        @QtBlockedSlot
        private final void finished()    {
            __qt_finished(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
        private native void __qt_finished(long __this__nativeId);
        
        @QtBlockedSlot
        private int exec(){
            io.qt.internal.QtJambiInternal.threadCheck(this);
            return exec_internal(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
        
        @QtBlockedSlot
        private final native int exec_internal(long nativeId);
        
        @QtBlockedSlot
        public final int loopLevel(){
            return __qt_loopLevel(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
        
        @QtBlockedSlot
        private final native int __qt_loopLevel(long nativeId);
        
        @QtBlockedSlot
        public final QAbstractEventDispatcher eventDispatcher(){
            return __qt_eventDispatcher(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
        
        private final native QAbstractEventDispatcher __qt_eventDispatcher(long nativeId);
        
        @QtBlockedSlot
        public void requestInterruption() {
            __qt_requestInterruption(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
        
        public void terminate(){
            __qt_terminate(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
        
        private final native void __qt_requestInterruption(long nativeId);
        
        private final native void __qt_terminate(long nativeId);
        
        @QtBlockedSlot
        private final native void __qt_exit(long nativeId, int retcode);
        
        public final void quit(){
            exit();
        }
        
        @QtBlockedSlot
        public final void exit(){
            exit(0);
        }
        
        public final void exit(int retcode){
            __qt_exit(io.qt.internal.QtJambiInternal.checkedNativeId(this), retcode);
        }
        
        private Wrapper(Thread thread, QPrivateConstructor p) { 
            super(p);
            this.thread = thread;
        }
        
        private Wrapper(QPrivateConstructor p) { 
            super(p);
            this.thread = Thread.currentThread();
        }
        
        @io.qt.internal.NativeAccess
        private static native long __qt_originalMetaObject();
    }

    public final void quit(){
        exit();
    }
    
    @QtBlockedSlot
    public final void exit(){
        exit(0);
    }
    
    @QtBlockedSlot
    public final void exit(int retcode){
        wrapper.exit(retcode);
    }
    
    public static int execStatic(){
        Thread currentThread = Thread.currentThread();
        if(currentThread.isAlive() && !currentThread.isInterrupted() && currentThread instanceof QThread){
            QThread qThread = (QThread)currentThread;
            return qThread.wrapper.exec();
        }else{
            throw new RuntimeException("Function QThread.execStatic() can only be invoked from inside a running QThread.");
        }
    }
    
    @QtBlockedSlot
    public final QAbstractEventDispatcher eventDispatcher(){
        return wrapper.eventDispatcher();
    }
    
    @Override
    @QtBlockedSlot
    public void interrupt() {
        if (!wrapper.isDisposed())
            wrapper.requestInterruption();
        super.interrupt();
    }
    
    public void terminate(){
        if (!wrapper.isDisposed())
            wrapper.terminate();
    }
    
    @QtBlockedSlot
    public final int loopLevel(){
        return wrapper.loopLevel();
    }
    
    @QtBlockedSlot
    public static native Wrapper getQThreadObject(Thread thread);
    
    @QtBlockedSlot
    public static native Wrapper currentQThread();
    
    @QtBlockedSlot
    public final boolean blockSignals(boolean b) {
        return wrapper.blockSignals(b);
    }

    @QtBlockedSlot
    public final List<QObject> children() {
        return wrapper.children();
    }

    @QtBlockedSlot
    public final void dumpObjectInfo() {
        wrapper.dumpObjectInfo();
    }

    @QtBlockedSlot
    public final void dumpObjectTree() {
        wrapper.dumpObjectTree();
    }

    @QtBlockedSlot
    public final List<QByteArray> dynamicPropertyNames() {
        return wrapper.dynamicPropertyNames();
    }

    @QtBlockedSlot
    public final void installEventFilter(QObject arg__1) {
        wrapper.installEventFilter(arg__1);
    }

    @QtBlockedSlot
    public final boolean isWidgetType() {
        return wrapper.isWidgetType();
    }

    @QtBlockedSlot
    public final boolean isWindowType() {
        return wrapper.isWindowType();
    }

    @QtBlockedSlot
    public final void killTimer(int id) {
        wrapper.killTimer(id);
    }

    @QtBlockedSlot
    public final void moveToThread(Thread thread) {
        wrapper.moveToThread(thread);
    }

    @QtBlockedSlot
    public final String objectName() {
        return wrapper.objectName();
    }

    @QtBlockedSlot
    public final QObject parent() {
        return wrapper.parent();
    }

    @QtBlockedSlot
    public final void removeEventFilter(QObject arg__1) {
        wrapper.removeEventFilter(arg__1);
    }

    @QtBlockedSlot
    public final void setObjectName(String name) {
        wrapper.setObjectName(name);
    }

    @QtBlockedSlot
    public final boolean signalsBlocked() {
        return wrapper.signalsBlocked();
    }

    @QtBlockedSlot
    public final int startTimer(int interval) {
        return wrapper.startTimer(interval);
    }

    @QtBlockedSlot
    public final int startTimer(int interval, TimerType timerType) {
        return wrapper.startTimer(interval, timerType);
    }

    @QtBlockedSlot
    public final QThread thread() {
        return (QThread)wrapper.thread();
    }

    @QtBlockedSlot
    public final List<QObject> findChildren() {
        return wrapper.findChildren();
    }

    @QtBlockedSlot
    public final <T extends QObject> List<T> findChildren(Class<T> cl) {
        return wrapper.findChildren(cl);
    }

    @QtBlockedSlot
    public final <T extends QObject> List<T> findChildren(Class<T> cl, String name) {
        return wrapper.findChildren(cl, name);
    }

    @QtBlockedSlot
    public final <T extends QObject> List<T> findChildren(Class<T> cl, QRegExp name) {
        return wrapper.findChildren(cl, name);
    }

    @QtBlockedSlot
    public final QObject findChild() {
        return wrapper.findChild();
    }

    @QtBlockedSlot
    public final <T extends QObject> T findChild(Class<T> cl) {
        return wrapper.findChild(cl);
    }

    @QtBlockedSlot
    public final <T extends QObject> T findChild(Class<T> cl, String name) {
        return wrapper.findChild(cl, name);
    }

    @QtBlockedSlot
    public final void setProperty(String name, Object value) {
        wrapper.setProperty(name, value);
    }

    @QtBlockedSlot
    public final Object property(String name) {
        return wrapper.property(name);
    }

    @QtBlockedSlot
    public final void disconnect() {
        wrapper.disconnect();
    }

    @QtBlockedSlot
    public final void disconnect(Object other) {
        wrapper.disconnect(other);
    }

    public final void disposeLater() {
        wrapper.disposeLater();
    }

    public final void dispose() {
        wrapper.dispose();
    }
    
    public final boolean isDisposed() {
        return wrapper.isDisposed();
    }

    @QtBlockedSlot
    public final void setParent(QObject arg__1) {
        wrapper.setParent(arg__1);
    }

    @QtBlockedSlot
    public boolean event(QEvent arg__1) {
        return wrapper.event(arg__1);
    }

    @QtBlockedSlot
    public boolean eventFilter(QObject arg__1, QEvent arg__2) {
        return wrapper.eventFilter(arg__1, arg__2);
    }
    
    private final static Object interruptible;
    
    static{
        Object _interruptible = null;
        try {
            _interruptible = java.lang.reflect.Proxy.newProxyInstance(
                    QThread.class.getClassLoader(), 
                    new Class[] { Class.forName("sun.nio.ch.Interruptible") }, 
                    (proxy, method, args)->{
                        if(args.length==1 && args[0] instanceof Thread) {
                            QThread qthread = thread((Thread)args[0]);
                            if(qthread!=null && !qthread.isDisposed()){
                                qthread.__qt_QThread_requestInterruption(checkedNativeId(qthread));
                            }
                        }
                        return null;
                    });
        } catch (Throwable e) {
        }
        interruptible = _interruptible;
    }
    
    @io.qt.QtUninvokable
    static native void initializeCurrentThread();
}
