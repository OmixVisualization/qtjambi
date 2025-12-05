package io.qt.core;

import java.util.Objects;

import io.qt.NonNull;
import io.qt.Nullable;

/**
 * <p>Allows monitoring a QFuture using signals and slots</p>
 * <p>Java wrapper for Qt class <code><a href="http://doc.qt.io/qt/qfuturewatcher.html">QFutureWatcher</a></code></p>
 */
public class QFutureWatcher<T> extends io.qt.core.QFutureWatcherBase
{
	/**
     * This variable stores the meta-object for the class.
     */
    public static final io.qt.core.QMetaObject staticMetaObject = io.qt.core.QMetaObject.forType(QFutureWatcher.class);
    
    /**
     * <p>Overloaded constructor for {@link #QFutureWatcher(io.qt.core.QObject)}
     *  with <code>_parent = null</code>.</p>
     */
    public QFutureWatcher() {
        this((io.qt.core.QObject)null);
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturewatcher.html#QFutureWatcher">QFutureWatcher::<wbr>QFutureWatcher(QObject*)</a></code></p>
     */
    public QFutureWatcher(io.qt.core.@Nullable QObject parent){
        super((QPrivateConstructor)null);
        initialize_native(this, parent);
    }
    
    private native static <T> void initialize_native(QFutureWatcher<T> instance, io.qt.core.QObject parent);
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturewatcher.html#future">QFutureWatcher::<wbr>future()const</a></code></p>
     */
    @io.qt.QtUninvokable
    public native final io.qt.core.@NonNull QFuture<T> future();
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturewatcher.html#result">QFutureWatcher::<wbr>result()const</a></code></p>
     */
    @io.qt.QtUninvokable
    public final T result(){
        return resultAt(0);
    }
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturewatcher.html#resultAt">QFutureWatcher::<wbr>resultAt(int)const</a></code></p>
     */
    @io.qt.QtUninvokable
    public native final T resultAt(int index);
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturewatcher.html#setFuture">QFutureWatcher::<wbr>setFuture(QFuture&lt;T&gt;)</a></code></p>
     */
    @io.qt.QtUninvokable
    public native final void setFuture(io.qt.core.@NonNull QFuture<T> future);
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     */
    @io.qt.NativeAccess
    protected QFutureWatcher(QPrivateConstructor p) { 
    	super(p);
	} 
    
    @io.qt.NativeAccess
	private final long futureSetter = 0;
    @io.qt.NativeAccess
    private final long futureResult = 0;
    @io.qt.NativeAccess
    private final long futureGetter = 0;
    @io.qt.NativeAccess
    private final long futureInterfaceGetter = 0;
}
