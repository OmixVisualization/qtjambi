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

import java.util.Objects;

import io.qt.NativeAccess;
import io.qt.NonNull;
import io.qt.QNoImplementationException;
import io.qt.QtObject;
import io.qt.StrictNonNull;

/**
 * <p>Way to store computation results to be accessed by QFuture</p>
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qpromise.html">QPromise</a></p>
 */
public final class QPromise<T>
{
    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    private static class NativeInstance extends QtObject{
        @NativeAccess
		private NativeInstance(QPrivateConstructor p) {
			super(p);
		}
        
        @NativeAccess
        private QPromise<?> promise;
    }
    
    private final io.qt.core.QFutureInterfaceBase d;
    
    @NativeAccess
    private NativeInstance nativeInstance;
    
    public static QPromise<Void> createVoidPromise() {
    	return new QPromise<>(true);
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#QPromise">QPromise::QPromise()</a></p>
     */
    public QPromise(){
        super();
        d = new QFutureInterface<>();
        registerPromise(this);
    }
    
    private void registerPromise(QPromise<?> promise) {
    	QFutureInterfaceBase d = promise.d;
        QtJambi_LibraryUtilities.internal.registerCleaner(promise, ()->{if(!d.isDisposed()){
        	d.reportFinished();
        }});
    }
    
	private QPromise(boolean isVoid){
        super();
        d = QFutureInterface.createVoidFutureInterface();
        registerPromise(this);
    }
    
    public QPromise(io.qt.core.@StrictNonNull QFutureInterface<T> other){
    	d = other.clone();
    	registerPromise(this);
    }
    
    @NativeAccess
    private QPromise(io.qt.core.QFutureInterfaceBase other, boolean internal){
    	d = Objects.requireNonNull(other);
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#finish">QPromise::finish()</a></p>
     */
    @io.qt.QtUninvokable
    public final void finish(){
    	d.reportFinished();
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#future">QPromise::future()const</a></p>
     */
    @io.qt.QtUninvokable
    public final io.qt.core.@NonNull QFuture<T> future(){
    	return new QFuture<>(d.clone(), true);
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#isCanceled">QPromise::isCanceled()const</a></p>
     */
    @io.qt.QtUninvokable
    public final boolean isCanceled(){
    	return d.isCanceled();
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#setProgressRange">QPromise::setProgressRange(int,int)</a></p>
     */
    @io.qt.QtUninvokable
    public final void setProgressRange(int minimum, int maximum){
    	d.setProgressRange(minimum, maximum);
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#setProgressValue">QPromise::setProgressValue(int)</a></p>
     */
    @io.qt.QtUninvokable
    public final void setProgressValue(int progressValue){
    	d.setProgressValue(progressValue);
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#setProgressValueAndText">QPromise::setProgressValueAndText(int,QString)</a></p>
     */
    @io.qt.QtUninvokable
    public final void setProgressValueAndText(int progressValue, java.lang.String progressText){
    	d.setProgressValueAndText(progressValue, progressText);
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#start">QPromise::start()</a></p>
     */
    @io.qt.QtUninvokable
    public final void start(){
    	d.reportStarted();
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#suspendIfRequested">QPromise::suspendIfRequested()</a></p>
     */
    @io.qt.QtUninvokable
    public final void suspendIfRequested(){
    	d.suspendIfRequested();
    }
    
    /**
     * <p>See <a href="https://doc.qt.io/qt/qpromise.html#swap">QPromise::swap(QPromise&lt;T&gt;&amp;)</a></p>
     */
    @io.qt.QtUninvokable
    public final void swap(io.qt.core.@StrictNonNull QPromise<T> other){
        java.util.Objects.requireNonNull(other, "Argument 'other': null not expected.");
        d.swap(other.d);
    }
    
    @io.qt.QtUninvokable
    public final void setException(@StrictNonNull Throwable e) {
        d.reportException(e);
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(T result) {
        try {
			return QFutureInterface.reportResult(QtJambi_LibraryUtilities.internal.nativeId(d), result, -1);
		} catch (QNoImplementationException e) {
			throw new QNoImplementationException("addResult(T) not available for QPromise<void>.");
		}
    }
    
    @io.qt.QtUninvokable
    public final boolean addResult(T result, int index) {
    	try{
    		return QFutureInterface.reportResult(QtJambi_LibraryUtilities.internal.nativeId(d), result, index);
    	} catch (QNoImplementationException e) {
			throw new QNoImplementationException("addResult(T, int) not available for QPromise<void>.");
		}
    }
}
