package io.qt.tasktree;

import io.qt.*;
import io.qt.core.QMetaObject;
import io.qt.core.QObject;
import io.qt.core.Qt;


/**
 * <p>A class template providing default task adapter used in QCustomTask</p>
 * <p>Java wrapper for Qt class <code><a href="https://doc.qt.io/qt/qttasktree-qdefaulttaskadapter.html">QtTaskTree::QDefaultTaskAdapter</a></code></p>
 * @since This class was introduced in Qt 6.11.
 */
public class QDefaultTaskAdapter<Task extends QObject> implements java.util.function.BiConsumer<@Nullable Task, io.qt.tasktree.@Nullable QTaskInterface>
{
    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
	@Override
	public void accept(@Nullable Task t, @Nullable QTaskInterface _iface) {
		QMetaObject.AbstractPrivateSignal1<QtTaskTree.DoneResult> signal1 = QMetaObject.findSignal(t, "done", QtTaskTree.DoneResult.class);
		if(signal1==null) {
			QMetaObject.AbstractPrivateSignal1<Boolean> signal2 = QMetaObject.findSignal(t, "done", boolean.class);
			if(signal2!=null) {
				signal2.connect(_iface, (iface,result)->{iface.reportDone(QtTaskTree.toDoneResult(result));}, Qt.ConnectionType.SingleShotConnection);
			}else {
				QMetaObject.AbstractPrivateSignal1<QtTaskTree.DoneWith> signal3 = QMetaObject.findSignal(t, "done", QtTaskTree.DoneWith.class);
				if(signal3!=null) {
					signal3.connect(_iface, (iface,result)->{iface.reportDone(QtTaskTree.DoneResult.resolve(result.value()));}, Qt.ConnectionType.SingleShotConnection);
				}
			}
		}else {
			signal1.connect(_iface, QTaskInterface::reportDone, Qt.ConnectionType.SingleShotConnection);
		}
		try {
			QtJambi_LibraryUtilities.internal.invokeMethod(t.getClass().getDeclaredMethod("start"), t);
		} catch (RuntimeException | Error e) {
			throw e;
		} catch (Throwable e) {
			throw new RuntimeException(e);
		}
	}
	
	@SuppressWarnings("unchecked")
	static <Task extends QObject> Class<QDefaultTaskAdapter<Task>> typedClass(){
		return (Class<QDefaultTaskAdapter<Task>>)(Class<?>)QDefaultTaskAdapter.class;
	}
}
