/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.core;

import io.qt.NativeAccess;
import io.qt.NonNull;
import io.qt.QNoImplementationException;
import io.qt.StrictNonNull;

/**
 * <p>Represents the result of an asynchronous computation</p>
 * <p>Java wrapper for Qt class <a href="http://doc.qt.io/qt/qfuture.html">QFuture</a></p>
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
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#QFuture">QFuture::QFuture()</a></p>
     */
    public QFuture(){
    	d = QFutureInterface.canceledResult();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#QFuture-2">QFuture::QFuture(QFutureInterface&lt;T&gt;*)</a></p>
     */
    public QFuture(io.qt.core.@StrictNonNull QFutureInterface<T> d){
        super();
        this.d = d.clone();
    }
    
    @NativeAccess
    QFuture(io.qt.core.QFutureInterfaceBase d, boolean internal){
        super();
        java.util.Objects.requireNonNull(d, "Argument 'd': null not expected.");
        this.d = d;
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#cancel">QFuture::cancel()</a></p>
     */
    @io.qt.QtUninvokable
    public final void cancel(){
		d.cancel();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#isCanceled">QFuture::isCanceled()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isCanceled(){
        return d.isCanceled();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#isFinished">QFuture::isFinished()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isFinished(){
        return d.isFinished();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#isPaused">QFuture::isPaused()const</a></p>
     * @deprecated Use isSuspending() or isSuspended() instead.
     */
    @Deprecated
    @io.qt.QtUninvokable
    public final boolean isPaused(){
        return d.isPaused();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#isSuspended">QFuture::isSuspended()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isSuspended(){
        return d.isSuspended();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#isSuspending">QFuture::isSuspending()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isSuspending(){
        return d.isSuspending();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#isResultReadyAt">QFuture::isResultReadyAt(int)const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isResultReadyAt(int resultIndex){
        return d.isResultReadyAt(resultIndex);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#isRunning">QFuture::isRunning()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isRunning(){
        return d.isRunning();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#isStarted">QFuture::isStarted()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isStarted(){
        return d.isStarted();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#pause">QFuture::pause()</a></p>
     * @deprecated Use suspend() instead.
     */
    @Deprecated
    @io.qt.QtUninvokable
    public final void pause(){
        setPaused(true);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#pause">QFuture::suspend()</a></p>
     */
    @io.qt.QtUninvokable
    public final void suspend(){
        setSuspended(true);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#progressMaximum">QFuture::progressMaximum()const</a></p>
     */
    @io.qt.QtUninvokable
    public final int progressMaximum(){
        return d.progressMaximum();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#progressMinimum">QFuture::progressMinimum()const</a></p>
     */
    @io.qt.QtUninvokable
    public final int progressMinimum(){
        return d.progressMinimum();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#progressText">QFuture::progressText()const</a></p>
     */
    @io.qt.QtUninvokable
    public final java.lang.String progressText(){
        return d.progressText();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#progressValue">QFuture::progressValue()const</a></p>
     */
    @io.qt.QtUninvokable
    public final int progressValue(){
        return d.progressValue();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#result">QFuture::result()const</a></p>
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
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#resultAt">QFuture::resultAt(int)const</a></p>
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
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#resultCount">QFuture::resultCount()const</a></p>
     */
    @io.qt.QtUninvokable
    public final int resultCount(){
        return d.resultCount();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#results">QFuture::results()const</a></p>
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
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#resume">QFuture::resume()</a></p>
     */
    @io.qt.QtUninvokable
    public final void resume(){
    	setSuspended(false);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#setPaused">QFuture::setPaused(bool)</a></p>
     * @deprecated Use setSuspended() instead.
     */
    @Deprecated
        @io.qt.QtUninvokable
    public final void setPaused(boolean paused){
        d.setPaused(paused);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#setSuspended">QFuture::setSuspended(bool)</a></p>
     */
    @io.qt.QtUninvokable
    public final void setSuspended(boolean suspended){
        d.setSuspended(suspended);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#togglePaused">QFuture::togglePaused()</a></p>
     * @deprecated Use toggleSuspended() instead.
     */
    @Deprecated
    @io.qt.QtUninvokable
    public final void togglePaused(){
        d.togglePaused();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#toggleSuspended">QFuture::toggleSuspended()</a></p>
     */
    @io.qt.QtUninvokable
    public final void toggleSuspended(){
        d.toggleSuspended();
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#waitForFinished">QFuture::waitForFinished()</a></p>
     */
    @io.qt.QtUninvokable
    public final void waitForFinished(){
        d.waitForFinished();
    }
    
    @Override
    public java.util.@NonNull Iterator<T> iterator() {
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
    public @NonNull QFuture<T> clone() {
        return new QFuture<T>(d.clone(), true);
    }
    
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(java.util.function.Function<QFuture<T>, R> function){
        return then(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> then(long __this_nativeId, java.util.function.Function<QFuture<T>, R> function);
    
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(QtFuture.Launch policy, java.util.function.Function<QFuture<T>, R> function){
        return thenLaunch(QtJambi_LibraryUtilities.internal.nativeId(d), policy.value(), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenLaunch(long __this_nativeId, int policy, java.util.function.Function<QFuture<T>, R> function);
    
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(QThreadPool pool, java.util.function.Function<QFuture<T>, R> function){
        return thenPool(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.nativeId(pool), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenPool(long __this_nativeId, long pool, java.util.function.Function<QFuture<T>, R> function);
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(java.util.function.Consumer<QFuture<T>> function){
        return thenVoid(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenVoid(long __this_nativeId, java.util.function.Consumer<QFuture<T>> function);
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(QtFuture.Launch policy, java.util.function.Consumer<QFuture<T>> function){
        return thenLaunchVoid(QtJambi_LibraryUtilities.internal.nativeId(d), policy.value(), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenLaunchVoid(long __this_nativeId, int policy, java.util.function.Consumer<QFuture<T>> function);
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(QThreadPool pool, java.util.function.Consumer<QFuture<T>> function){
        return thenPoolVoid(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.nativeId(pool), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenPoolVoid(long __this_nativeId, long pool, java.util.function.Consumer<QFuture<T>> function);
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onFailed(java.util.function.Function<Throwable, T> function){
        return onFailed(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onCanceled(java.util.function.Supplier<T> function){
        return onCanceled(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onFailed(java.util.function.Consumer<Throwable> function){
        return onFailed(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onFailed(long __this_nativeId, Object function);
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onCanceled(java.lang.Runnable function){
        return onCanceled(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onCanceled(long __this_nativeId, java.lang.Object function);

    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(QObject context, java.util.function.Function<QFuture<T>, R> function){
        return thenPool(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenContext(long __this_nativeId, long contextId, java.util.function.Function<QFuture<T>, R> function);
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(QObject context, java.util.function.Consumer<QFuture<T>> function){
        return thenPoolVoid(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenContextVoid(long __this_nativeId, long contextId, java.util.function.Consumer<QFuture<T>> function);
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onFailed(QObject context, java.util.function.Function<Throwable, T> function){
        return onFailedContext(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onFailedContext(long __this_nativeId, long contextId, Object function);
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onCanceled(QObject context, java.util.function.Supplier<T> function){
        return onCanceledContext(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onFailed(QObject context, java.util.function.Consumer<Throwable> function){
        return onFailedContext(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onCanceled(QObject context, java.lang.Runnable function){
        return onCanceledContext(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(java.util.Objects.requireNonNull(context)), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onCanceledContext(long __this_nativeId, long contextId, java.lang.Object function);
    
    public <U> @NonNull QFuture<U> unwrap(@StrictNonNull Class<U> flatType){
		QFutureInterface<U> promise = new QFutureInterface<>(QFutureInterfaceBase.State.Pending);
		this.then(nested->{
			promise.reportStarted();
			try {
            	for(T result : nested.results()) {
            		if(result instanceof QFuture) {
            			QFuture<U> future = ((QFuture<?>) result).unwrap(flatType);
            			promise.reportResults(future.results());
            		}else if(result==null || flatType.isInstance(result)){
            			promise.reportResult(flatType.cast(result));
            		}
            	}
			}catch(Throwable e) {
				promise.reportException(e);
            }
			promise.reportFinished();
		}).onCanceled(() -> {
            promise.reportCanceled();
            promise.reportFinished();
        });
        return promise.future();
    }
}
