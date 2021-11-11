package io.qt.core;

import io.qt.QNoImplementationException;

/**
 * <p>Represents the result of an asynchronous computation</p>
 * <p>Java wrapper for Qt class <a href="http://doc.qt.io/qt-5/qfuture.html">QFuture</a></p>
 */
public final class QFuture<T>
    implements java.lang.Iterable<T>,
            java.lang.Cloneable
{
    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    private final io.qt.core.QFutureInterfaceBase d;
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#QFuture">QFuture::QFuture()</a></p>
     */
    public QFuture(){
    	d = QFutureInterface.canceledResult();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#QFuture-2">QFuture::QFuture(QFutureInterface&lt;T&gt;*)</a></p>
     */
    public QFuture(io.qt.core.QFutureInterface<T> d){
        super();
        this.d = d.clone();
    }
    
    QFuture(io.qt.core.QFutureInterfaceBase d, boolean internal){
        super();
        java.util.Objects.requireNonNull(d, "Argument 'd': null not expected.");
        this.d = d;
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#cancel">QFuture::cancel()</a></p>
     */
    @io.qt.QtUninvokable
    public final void cancel(){
		d.cancel();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#isCanceled">QFuture::isCanceled()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isCanceled(){
        return d.isCanceled();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#isFinished">QFuture::isFinished()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isFinished(){
        return d.isFinished();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#isPaused">QFuture::isPaused()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isPaused(){
        return d.isPaused();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#isResultReadyAt">QFuture::isResultReadyAt(int)const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isResultReadyAt(int resultIndex){
        return d.isResultReadyAt(resultIndex);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#isRunning">QFuture::isRunning()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isRunning(){
        return d.isRunning();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#isStarted">QFuture::isStarted()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isStarted(){
        return d.isStarted();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#pause">QFuture::pause()</a></p>
     */
    @io.qt.QtUninvokable
    public final void pause(){
        setPaused(true);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#progressMaximum">QFuture::progressMaximum()const</a></p>
     */
    @io.qt.QtUninvokable
    public final int progressMaximum(){
        return d.progressMaximum();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#progressMinimum">QFuture::progressMinimum()const</a></p>
     */
    @io.qt.QtUninvokable
    public final int progressMinimum(){
        return d.progressMinimum();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#progressText">QFuture::progressText()const</a></p>
     */
    @io.qt.QtUninvokable
    public final java.lang.String progressText(){
        return d.progressText();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#progressValue">QFuture::progressValue()const</a></p>
     */
    @io.qt.QtUninvokable
    public final int progressValue(){
        return d.progressValue();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#result">QFuture::result()const</a></p>
     */
	@io.qt.QtUninvokable
    public final T result(){
    	if(d instanceof QFutureInterface) {
	    	d.waitForResult(0);
            try{
                return QFutureInterface.result(QtJambi_LibraryUtilities.internal.nativeId(d), 0);
            }catch(QNoImplementationException e) {
	    		throw new QNoImplementationException("result() not available for QFuture<void>.");
	    	}
    	}else {
    		throw new QNoImplementationException("result() not available for QFuture<void>.");
    	}
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#resultAt">QFuture::resultAt(int)const</a></p>
     */
	@io.qt.QtUninvokable
    public final T resultAt(int index){
    	if(d instanceof QFutureInterface) {
	    	d.waitForResult(index);
	        try{
                return QFutureInterface.result(QtJambi_LibraryUtilities.internal.nativeId(d), index);
            }catch(QNoImplementationException e) {
	    		throw new QNoImplementationException("resultAt(int) not available for QFuture<void>.");
	    	}
    	}else {
    		throw new QNoImplementationException("resultAt(int) not available for QFuture<void>.");
    	}
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#resultCount">QFuture::resultCount()const</a></p>
     */
    @io.qt.QtUninvokable
    public final int resultCount(){
        return d.resultCount();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#results">QFuture::results()const</a></p>
     */
	@io.qt.QtUninvokable
    public final io.qt.core.QList<T> results(){
    	if(d instanceof QFutureInterface) {
	    	try{
                return QFutureInterface.results(QtJambi_LibraryUtilities.internal.nativeId(d));
            }catch(QNoImplementationException e) {
	    		throw new QNoImplementationException("results() not available for QFuture<void>.");
	    	}
    	}else {
    		throw new QNoImplementationException("results() not available for QFuture<void>.");
    	}
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#resume">QFuture::resume()</a></p>
     */
    @io.qt.QtUninvokable
    public final void resume(){
        setPaused(false);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#setPaused">QFuture::setPaused(bool)</a></p>
     */
    @io.qt.QtUninvokable
    public final void setPaused(boolean paused){
        d.setPaused(paused);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#togglePaused">QFuture::togglePaused()</a></p>
     */
    @io.qt.QtUninvokable
    public final void togglePaused(){
        d.togglePaused();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt-5/qfuture.html#waitForFinished">QFuture::waitForFinished()</a></p>
     */
    @io.qt.QtUninvokable
    public final void waitForFinished(){
        d.waitForFinished();
    }
    
    @Override
    public java.util.Iterator<T> iterator() {
    	if(d instanceof QFutureInterface) {
	    	if(d.isFinished())
	    		return results().iterator();
	        return new java.util.Iterator<T>(){
	        	int i=0;
				@Override
				public boolean hasNext() {
					d.waitForResult(i);
					return d.isResultReadyAt(i);
				}
	
				@Override
				public T next() {
					return QFutureInterface.result(QtJambi_LibraryUtilities.internal.nativeId(d), i++);
				}
	        };
		}else {
			throw new QNoImplementationException("iterator() not available for QFutureInterfaceBase.");
		}
    }
    
    @Override
    @io.qt.QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof io.qt.core.QFuture<?>) {
            return d.equals(((io.qt.core.QFuture<?>) other).d);
        }
        return false;
    }


    @io.qt.QtUninvokable
    @Override
    public int hashCode() {
        return d.hashCode();
    }
    
    @Override
    public QFuture<T> clone() {
        return new QFuture<T>(d.clone(), true);
    }
    
    @io.qt.QtUninvokable
    io.qt.core.QFutureInterfaceBase futureInterface() { return d; }
}
