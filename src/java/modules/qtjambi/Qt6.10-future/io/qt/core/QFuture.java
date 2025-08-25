/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static java.util.Objects.requireNonNull;

import io.qt.NativeAccess;
import io.qt.NonNull;
import io.qt.Nullable;
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
        requireNonNull(d, "Argument 'd': null not expected.");
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
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#cancelChain">QFuture::cancelChain()</a></p>
     */
    @io.qt.QtUninvokable
    public final void cancelChain(){
		d.cancelChain();
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
    @Deprecated(forRemoval=true, since="Qt 6.0")
    @io.qt.QtUninvokable
    public final boolean isPaused(){
        return d.isSuspended() || d.isSuspending();
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
    @Deprecated(forRemoval=true, since="Qt 6.0")
    @io.qt.QtUninvokable
    public final void pause(){
    	setSuspended(true);
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
    public final io.qt.core.@NonNull QList<T> results(){
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
    @Deprecated(forRemoval=true, since="Qt 6.0")
    @io.qt.QtUninvokable
    public final void setPaused(boolean paused){
        d.setSuspended(paused);
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
    @Deprecated(forRemoval=true, since="Qt 6.0")
    @io.qt.QtUninvokable
    public final void togglePaused(){
        d.toggleSuspended();
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
            return equals((io.qt.core.QFuture<?>) other);
        }
        return false;
    }
    
    @io.qt.QtUninvokable
    public boolean equals(io.qt.core.@StrictNonNull QFuture<?> other) {
    	return d.equals(other.d);
    }


    @io.qt.QtUninvokable
    @Override
    public int hashCode() {
        return d.hashCode();
    }
    
    @Override
    public @NonNull QFuture<T> clone() {
        return new QFuture<T>(d.clone(), true);
    }/**
     * <p>Similar to <code>Runnable</code> but potentially throwing exception.</p>
     * @see java.lang.Runnable
     */
    @FunctionalInterface
    public interface Runnable {
        /**
         * @see java.lang.Runnable#run()
         * @throws Throwable
         */
        void run() throws Throwable;
    }

    /**
     * <p>Similar to <code>Supplier</code> but potentially throwing exception.</p>
     * @see java.util.function.Supplier
     */
    @FunctionalInterface
    public interface Supplier<T> {
        /**
         * @see java.util.function.Supplier#get()
         * @throws Throwable
         */
        T get() throws Throwable;
    }

    /**
     * <p>Similar to <code>Consumer</code> but potentially throwing exception.</p>
     * @see java.util.function.Consumer
     */
    @FunctionalInterface
    public interface Consumer<T> {
        /**
         * @see java.util.function.Consumer#accept(Object)
         * @throws Throwable
         */
        void accept(T t) throws Throwable;
    }

    /**
     * <p>Similar to <code>Function</code> but potentially throwing exception.</p>
     * @see java.util.function.Function
     */
    @FunctionalInterface
    public interface Function<T, R> {
        /**
         * @see java.util.function.Function#apply(Object)
         * @throws Throwable
         */
        R apply(T t) throws Throwable;
    }

    /**
     * <p>Similar to <code>Consumer</code> but potentially throwing exception.</p>
     * @see java.util.function.Consumer
     */
    @FunctionalInterface
    public interface FutureConsumer<T> {
        /**
         * @see java.util.function.Consumer#accept(Object)
         * @throws Throwable
         */
        void accept(QFuture<T> t) throws Throwable;
    }

    /**
     * <p>Similar to <code>Function</code> but potentially throwing exception.</p>
     * @see java.util.function.Function
     */
    @FunctionalInterface
    public interface FutureFunction<T, R> {
        /**
         * @see java.util.function.Function#apply(Object)
         * @throws Throwable
         */
        R apply(QFuture<T> t) throws Throwable;
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then">QFuture::then(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@StrictNonNull Supplier<R> function){
        return then(QtFuture.Launch.Sync, function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then">QFuture::then(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@StrictNonNull Function<T, R> function){
        return then(QtFuture.Launch.Sync, function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then">QFuture::then(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@StrictNonNull FutureFunction<T, R> function){
        return then(QtFuture.Launch.Sync, function);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-1">QFuture::then(QtFuture::Launch, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(QtFuture.@NonNull Launch policy, @StrictNonNull Supplier<R> function){
        return thenLaunchSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), policy.value(), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-1">QFuture::then(QtFuture::Launch, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(QtFuture.@NonNull Launch policy, @StrictNonNull Function<T, R> function){
        return thenLaunchValueFunction(QtJambi_LibraryUtilities.internal.nativeId(d), policy.value(), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-1">QFuture::then(QtFuture::Launch, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(QtFuture.@NonNull Launch policy, @StrictNonNull FutureFunction<T, R> function){
        return thenLaunchFutureFunction(QtJambi_LibraryUtilities.internal.nativeId(d), policy.value(), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenLaunchSupplier(long __this_nativeId, int policy, Supplier<R> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenLaunchValueFunction(long __this_nativeId, int policy, Function<T, R> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenLaunchFutureFunction(long __this_nativeId, int policy, FutureFunction<T, R> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-2">QFuture::then(QThreadPool*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@NonNull QThreadPool pool, @StrictNonNull Supplier<R> function){
        return thenPoolSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.nativeId(pool), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-2">QFuture::then(QThreadPool*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@NonNull QThreadPool pool, @StrictNonNull Function<T, R> function){
        return thenPoolValueFunction(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.nativeId(pool), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-2">QFuture::then(QThreadPool*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@NonNull QThreadPool pool, @StrictNonNull FutureFunction<T, R> function){
        return thenPoolFutureFunction(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.nativeId(pool), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenPoolValueFunction(long __this_nativeId, long pool, Function<T, R> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenPoolSupplier(long __this_nativeId, long pool, Supplier<R> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenPoolFutureFunction(long __this_nativeId, long pool, FutureFunction<T, R> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then">QFuture::then(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@StrictNonNull Runnable function){
        return then(QtFuture.Launch.Sync, function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then">QFuture::then(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@StrictNonNull Consumer<T> function){
        return then(QtFuture.Launch.Sync, function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then">QFuture::then(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@StrictNonNull FutureConsumer<T> function){
        return then(QtFuture.Launch.Sync, function);
    }    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-1">QFuture::then(QtFuture::Launch, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(QtFuture.@NonNull Launch policy, @StrictNonNull Runnable function){
        return thenLaunchRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), policy.value(), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-1">QFuture::then(QtFuture::Launch, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(QtFuture.@NonNull Launch policy, @StrictNonNull Consumer<T> function){
        return thenLaunchValueConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), policy.value(), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-1">QFuture::then(QtFuture::Launch, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(QtFuture.@NonNull Launch policy, @StrictNonNull FutureConsumer<T> function){
        return thenLaunchFutureConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), policy.value(), function);
    }
    @io.qt.QtUninvokable
    private native static <R> QFuture<Void> thenLaunchRunnable(long __this_nativeId, int policy, Runnable function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenLaunchValueConsumer(long __this_nativeId, int policy, Consumer<T> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenLaunchFutureConsumer(long __this_nativeId, int policy, FutureConsumer<T> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-2">QFuture::then(QThreadPool*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@NonNull QThreadPool pool, @StrictNonNull Runnable function){
        return thenPoolRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.nativeId(pool), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-2">QFuture::then(QThreadPool*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@NonNull QThreadPool pool, @StrictNonNull Consumer<T> function){
        return thenPoolValueConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.nativeId(pool), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-2">QFuture::then(QThreadPool*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@NonNull QThreadPool pool, @StrictNonNull FutureConsumer<T> function){
        return thenPoolFutureConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.nativeId(pool), function);
    }
    @io.qt.QtUninvokable
    private native static <R> QFuture<Void> thenPoolRunnable(long __this_nativeId, long pool, Runnable function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenPoolValueConsumer(long __this_nativeId, long pool, Consumer<T> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenPoolFutureConsumer(long __this_nativeId, long pool, FutureConsumer<T> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed">QFuture::onFailed(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onFailed(@StrictNonNull Function<@NonNull Throwable, T> function){
        return onFailedFunction(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed">QFuture::onFailed(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onFailed(@StrictNonNull Supplier<T> function){
        return onFailedSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed">QFuture::onFailed(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <Failure extends Throwable> @NonNull QFuture<T> onFailed(@StrictNonNull Class<Failure> exceptionType, @StrictNonNull Function<@NonNull Failure, T> function){
        return onFailedClassFunction(QtJambi_LibraryUtilities.internal.nativeId(d), requireNonNull(exceptionType, "Argument 'exceptionType': null not expected."), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed">QFuture::onFailed(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <Failure extends Throwable> @NonNull QFuture<Void> onFailed(@StrictNonNull Class<Failure> exceptionType, @StrictNonNull Runnable function){
        return onFailedClassRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), requireNonNull(exceptionType, "Argument 'exceptionType': null not expected."), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed">QFuture::onFailed(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <Failure extends Throwable> @NonNull QFuture<T> onFailed(@StrictNonNull Class<Failure> exceptionType, @StrictNonNull Supplier<T> function){
        return onFailedClassSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), requireNonNull(exceptionType, "Argument 'exceptionType': null not expected."), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed">QFuture::onFailed(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <Failure extends Throwable> @NonNull QFuture<Void> onFailed(@StrictNonNull Class<Failure> exceptionType, @StrictNonNull Consumer<@NonNull Failure> function){
        return onFailedClassConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), requireNonNull(exceptionType, "Argument 'exceptionType': null not expected."), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed">QFuture::onFailed(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onFailed(@StrictNonNull Runnable function){
        return onFailedRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed">QFuture::onFailed(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onFailed(@StrictNonNull Consumer<@NonNull Throwable> function){
        return onFailedConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onFailedFunction(long __this_nativeId, Function<Throwable, T> function);
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onFailedSupplier(long __this_nativeId, Supplier<T> function);
    @io.qt.QtUninvokable
    private native static QFuture<Void> onFailedRunnable(long __this_nativeId, Runnable function);
    @io.qt.QtUninvokable
    private native static QFuture<Void> onFailedConsumer(long __this_nativeId, Consumer<Throwable> function);
    @io.qt.QtUninvokable
    private native static <Failure extends Throwable> QFuture<Void> onFailedClassRunnable(long __this_nativeId, Class<Failure> exceptionType, Runnable function);
    @io.qt.QtUninvokable
    private native static <T, Failure extends Throwable> QFuture<T> onFailedClassSupplier(long __this_nativeId, Class<Failure> exceptionType, Supplier<T> function);
    @io.qt.QtUninvokable
    private native static <T, Failure extends Throwable> QFuture<T> onFailedClassFunction(long __this_nativeId, Class<Failure> exceptionType, Function<Failure, T> function);
    @io.qt.QtUninvokable
    private native static <Failure extends Throwable> QFuture<Void> onFailedClassConsumer(long __this_nativeId, Class<Failure> exceptionType, Consumer<Failure> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onCanceled">QFuture::onCanceled(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onCanceled(@StrictNonNull Runnable function){
        return onCanceledRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onCanceled">QFuture::onCanceled(Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onCanceled(@StrictNonNull Supplier<T> function){
        return onCanceledSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onCanceledSupplier(long __this_nativeId, Supplier<T> function);
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onCanceledRunnable(long __this_nativeId, Runnable function);

    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-3">QFuture::then(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@Nullable QObject context, @StrictNonNull Supplier<R> function){
        return thenContextSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-3">QFuture::then(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@Nullable QObject context, @StrictNonNull Function<T, R> function){
        return thenContextValueFunction(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-3">QFuture::then(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <R> @NonNull QFuture<R> then(@Nullable QObject context, @StrictNonNull FutureFunction<T, R> function){
        return thenContextFutureFunction(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenContextSupplier(long __this_nativeId, long contextId, Supplier<R> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenContextValueFunction(long __this_nativeId, long contextId, Function<T, R> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<R> thenContextFutureFunction(long __this_nativeId, long contextId, FutureFunction<T, R> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-3">QFuture::then(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@Nullable QObject context, @StrictNonNull Runnable function){
        return thenContextRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-3">QFuture::then(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@Nullable QObject context, @StrictNonNull Consumer<T> function){
        return thenContextValueConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#then-3">QFuture::then(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> then(@Nullable QObject context, @StrictNonNull FutureConsumer<T> function){
        return thenContextFutureConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    @io.qt.QtUninvokable
    private native static <R> QFuture<Void> thenContextRunnable(long __this_nativeId, long contextId, Runnable function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenContextValueConsumer(long __this_nativeId, long contextId, Consumer<T> function);
    @io.qt.QtUninvokable
    private native static <T,R> QFuture<Void> thenContextFutureConsumer(long __this_nativeId, long contextId, FutureConsumer<T> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed-1">QFuture::onFailed(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onFailed(@Nullable QObject context, @StrictNonNull Function<@NonNull Throwable, T> function){
        return onFailedContextFunction(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed-1">QFuture::onFailed(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onFailed(@Nullable QObject context, @StrictNonNull Supplier<T> function){
        return onFailedContextSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed-1">QFuture::onFailed(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <Failure extends Throwable> @NonNull QFuture<Void> onFailed(@Nullable QObject context, @StrictNonNull Class<Failure> exceptionType, @StrictNonNull Runnable function){
        return onFailedContextClassRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), requireNonNull(exceptionType, "Argument 'exceptionType': null not expected."), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed-1">QFuture::onFailed(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <Failure extends Throwable> @NonNull QFuture<T> onFailed(@Nullable QObject context, @StrictNonNull Class<Failure> exceptionType, @StrictNonNull Supplier<T> function){
        return onFailedContextClassSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), requireNonNull(exceptionType, "Argument 'exceptionType': null not expected."), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed-1">QFuture::onFailed(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <Failure extends Throwable> @NonNull QFuture<T> onFailed(@Nullable QObject context, @StrictNonNull Class<Failure> exceptionType, @StrictNonNull Function<@NonNull Failure, T> function){
        return onFailedContextClassFunction(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), requireNonNull(exceptionType, "Argument 'exceptionType': null not expected."), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed-1">QFuture::onFailed(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onFailed(@Nullable QObject context, @StrictNonNull Consumer<@NonNull Throwable> function){
        return onFailedContextConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed-1">QFuture::onFailed(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onFailed(@Nullable QObject context, @StrictNonNull Runnable function){
        return onFailedContextRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onFailed-1">QFuture::onFailed(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public <Failure extends Throwable> @NonNull QFuture<Void> onFailed(@Nullable QObject context, @StrictNonNull Class<Failure> exceptionType, @StrictNonNull Consumer<@NonNull Failure> function){
        return onFailedContextClassConsumer(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), requireNonNull(exceptionType, "Argument 'exceptionType': null not expected."), function);
    }
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onFailedContextFunction(long __this_nativeId, long contextId, Function<Throwable, T> function);
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onFailedContextSupplier(long __this_nativeId, long contextId, Supplier<T> function);
    @io.qt.QtUninvokable
    private native static QFuture<Void> onFailedContextRunnable(long __this_nativeId, long contextId, Runnable function);
    @io.qt.QtUninvokable
    private native static QFuture<Void> onFailedContextConsumer(long __this_nativeId, long contextId, Consumer<Throwable> function);
    @io.qt.QtUninvokable
    private native static <Failure extends Throwable> QFuture<Void> onFailedContextClassRunnable(long __this_nativeId, long contextId, Class<Failure> exceptionType, Runnable function);
    @io.qt.QtUninvokable
    private native static <T, Failure extends Throwable> QFuture<T> onFailedContextClassSupplier(long __this_nativeId, long contextId, Class<Failure> exceptionType, Supplier<T> function);
    @io.qt.QtUninvokable
    private native static <T, Failure extends Throwable> QFuture<T> onFailedContextClassFunction(long __this_nativeId, long contextId, Class<Failure> exceptionType, Function<Failure, T> function);
    @io.qt.QtUninvokable
    private native static <Failure extends Throwable> QFuture<Void> onFailedContextClassConsumer(long __this_nativeId, long contextId, Class<Failure> exceptionType, Consumer<Failure> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onCanceled-1">QFuture::onCanceled(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<T> onCanceled(@Nullable QObject context, @StrictNonNull Supplier<T> function){
        return onCanceledContextSupplier(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#onCanceled-1">QFuture::onCanceled(QObject*, Function&amp;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public @NonNull QFuture<Void> onCanceled(@Nullable QObject context, @StrictNonNull Runnable function){
        return onCanceledContextRunnable(QtJambi_LibraryUtilities.internal.nativeId(d), QtJambi_LibraryUtilities.internal.checkedNativeId(requireNonNull(context, "Argument 'context': null not expected.")), function);
    }
    @io.qt.QtUninvokable
    private native static QFuture<Void> onCanceledContextRunnable(long __this_nativeId, long contextId, Runnable function);
    @io.qt.QtUninvokable
    private native static <T> QFuture<T> onCanceledContextSupplier(long __this_nativeId, long contextId, Supplier<T> function);
    
    /**
     * <p>See <a href="http://doc.qt.io/qt/qfuture.html#unwrap">QFuture::unwrap()</a></p>
     */
    public <U> @NonNull QFuture<U> unwrap(@StrictNonNull Class<U> flatType){
		QFutureInterface<U> promise = new QFutureInterface<>(QFutureInterfaceBase.State.Pending);
		this.then((QFuture<T> nested)->{
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
