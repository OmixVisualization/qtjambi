package io.qt.core;

import io.qt.QNoImplementationException;

/**
 * <p>Allows monitoring a QFuture using signals and slots</p>
 * <p>Java wrapper for Qt class <code><a href="http://doc.qt.io/qt/qfuturewatcher.html">QFutureWatcher</a></code></p>
 */
public final class QFutureWatcher<T> extends io.qt.core.QFutureWatcherBase
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
    public QFutureWatcher(io.qt.core.QObject parent){
        super((QPrivateConstructor)null);
        long[] functions = {0,0,0};
        initialize_native(this, parent, functions);
    	this.futureSetter = functions[0];
    	this.futureResult = functions[1];
    	this.futureGetter = functions[2];
    }
    
    private native static <T> void initialize_native(QFutureWatcher<T> instance, io.qt.core.QObject parent, long[] functions);
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturewatcher.html#future">QFutureWatcher::<wbr>future()const</a></code></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.QFuture<T> future(){
        return future(QtJambi_LibraryUtilities.internal.nativeId(this), futureGetter);
    }
    
    @io.qt.QtUninvokable
    private static native <T> io.qt.core.QFuture<T> future(long nativeId, long futureGetter);
    
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
    public final T resultAt(int index){
		if(futureResult==0)
			throw new QNoImplementationException("resultAt(int) not available for QFutureWatcher<void>.");
		return resultAt(QtJambi_LibraryUtilities.internal.nativeId(this), futureResult, index);
    }
    
    @io.qt.QtUninvokable
    private static native <T> T resultAt(long nativeId, long futureResult, int index);
    
    /**
     * <p>See <code><a href="http://doc.qt.io/qt/qfuturewatcher.html#setFuture">QFutureWatcher::<wbr>setFuture(QFuture&lt;T&gt;)</a></code></p>
     */
    @io.qt.QtUninvokable
    public final void setFuture(io.qt.core.QFuture<T> future){
		setFuture(QtJambi_LibraryUtilities.internal.nativeId(this), futureSetter, future);
    }
    
    @io.qt.QtUninvokable
    private static native <T> void setFuture(long nativeId, long futureSetter, io.qt.core.QFuture<T> future);
    
    /**
     * Constructor for internal use only.
     * @param p expected to be <code>null</code>.
     */
    @io.qt.NativeAccess
    private QFutureWatcher(long futureSetter, long futureResult, long futureGetter) { 
    	super((QPrivateConstructor)null);
    	this.futureSetter = futureSetter;
    	this.futureResult = futureResult;
    	this.futureGetter = futureGetter;
	} 
    
	private final long futureSetter;
    private final long futureResult;
    private final long futureGetter;
}
