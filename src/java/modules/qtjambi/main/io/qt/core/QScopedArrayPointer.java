/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.Arrays;
import java.util.Objects;
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.ToDoubleFunction;
import java.util.function.ToIntFunction;
import java.util.function.ToLongFunction;
import java.util.logging.Level;

import io.qt.QtUninvokable;
import io.qt.StrictNonNull;
import io.qt.InternalAccess.Cleanable;
import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.QtObjectInterface;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qscopedarraypointer.html">QScopedArrayPointer</a>
 * to be used inside a try-with-resource block.</p>
 * <p>Example:</p>
 * <code>
 * try(var pointer = QScopedArrayPointer.disposing(new QDialog(), new QColor())){<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;QDialog dialog = pointer.get(0);<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;dialog.exec();<br>
 * }<br>
 * &#47;&#47; dialog is disposed
 * </code>
 * 
 */
public final class QScopedArrayPointer<O> implements AutoCloseable {
	
	private static class Data<O>{
		private O[] data;
		private Consumer<O> cleanup;
		private Cleanable cleanable;
		
	    @QtUninvokable
		void close(){
			if(cleanup!=null) {
				try{
					if(data!=null) {
						for(O o : data) {
							try{
								cleanup.accept(o);
							} catch (Throwable e) {
								QScopedPointer.logger.log(Level.WARNING, "QScopedArrayPointer cleanup", e);
							}
						}
					}
				}finally{
					cleanup = null;
				}
			}
			data = null;
		}
	}

	private final Data<O> data = new Data<>();
	
	private QScopedArrayPointer(O[] data, Consumer<O> cleanup) {
		this.data.data = data;
		this.data.cleanup = Objects.requireNonNull(cleanup, "Argument 'cleanup': null not expected.");
		this.data.cleanable = QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::close);
	}

	/**
	 * <p>Returns the array stored by this scoped pointer</p>
	 * @return data
	 */
    @QtUninvokable
	public O @Nullable[] data() {
		return data.data==null ? null : Arrays.copyOf(data.data, data.data.length);
	}

	/**
	 * @see #data()
	 */
    @QtUninvokable
	public O @Nullable[] get() {
		return data();
	}
    
	/**
	 * <p>Returns the object at index i in the array stored by this scoped pointer</p>
	 * <p>See <a href="https://doc.qt.io/qt/qscopedpointer.html#operator-5b-5d-1">QScopedPointer::operator[](qsizetype i)const</a>.</p>
	 * @param i index
	 * @return data
	 */
    @QtUninvokable
	public O data(int i) {
		return data.data==null ? null : data.data[i];
	}

	/**
	 * @see #data(int)
	 */
    @QtUninvokable
	public O get(int i) {
		return data(i);
	}

    @QtUninvokable
	public O @Nullable[] take() {
    	if(data.data==null)
    		return null;
		O[] oldData = data.data;
		data.data = null;
		return oldData;
	}
    
	/**
	 * <p>Removes and returns the array stored by this scoped pointer without cleanup</p>
	 * @return data
	 */
    @QtUninvokable
	public O take(int i) {
    	if(data.data==null)
    		return null;
		O oldData = data.data[i];
		data.data[i] = null;
		return oldData;
	}

	/**
	 * <p>performs cleanup operation on all objects of the contained array and sets new array.</p>
	 */
    @QtUninvokable
    @SafeVarargs
	public final void reset(O... other) {
		if(data.cleanup!=null) {
			O[] oldData = data.data;
			data.data = other;
			if(oldData!=null) {
				for(O o : oldData) {
					data.cleanup.accept(o);
				}
			}
		}
	}
	
    /**
     * checks if the array is null
     */
    @QtUninvokable
	public boolean isNull(){
		return data.data==null;
	}
	
    @QtUninvokable
	public void swap(@StrictNonNull QScopedArrayPointer<O> other){
		O[] oldData = data.data;
		Consumer<O> oldCleanup = data.cleanup;
		this.data.data = other.data.data;
		this.data.cleanup = other.data.cleanup;
		other.data.data = oldData;
		other.data.cleanup = oldCleanup;
	}

    /**
     * performs the cleanup operation
     */
	@Override
    @QtUninvokable
	public void close(){
		data.close();
		if(this.data.cleanable!=null) {
			this.data.cleanable.clean();
			this.data.cleanable = null;
		}
	}
	
    @QtUninvokable
	private static void dispose(QtObjectInterface data) {
		if(QtJambi_LibraryUtilities.internal.nativeId(data)!=0)
			data.dispose();
	}
	
    @QtUninvokable
	private static void disposeLater(QObject data) {
		if(QtJambi_LibraryUtilities.internal.nativeId(data)!=0)
			data.disposeLater();
	}
	
    /**
     * Creates a scoped pointer for the given objects performing {@link QtObjectInterface#dispose()} on each object at cleanup.
     * @param data the contained objects
     * @return scoped pointer
     */
    @SafeVarargs
	@QtUninvokable
	public static <O extends QtObjectInterface> @NonNull QScopedArrayPointer<O> disposing(O @Nullable... data){
		return new QScopedArrayPointer<>(data, QScopedArrayPointer::dispose);
	}
	
    /**
     * Creates a scoped pointer for the given objects performing {@link QObject#disposeLater()} on each object at cleanup.
     * @param data the contained objects
     * @return scoped pointer
     */
    @SafeVarargs
    @QtUninvokable
	public static <O extends QObject> @NonNull QScopedArrayPointer<O> disposingLater(O @Nullable... data){
		return new QScopedArrayPointer<>(data, QScopedArrayPointer::disposeLater);
	}
	
    /**
     * Creates a scoped array pointer for the given objects performing the given operation on each object at cleanup.
     * @param cleanup operation performed at cleanup
     * @param data the contained objects
     * @return scoped pointer
     */
    @SafeVarargs
    @QtUninvokable
	public static <O> @NonNull QScopedArrayPointer<O> cleanup(@StrictNonNull Consumer<O> cleanup, O @Nullable... data){
		return new QScopedArrayPointer<>(data, cleanup);
	}
	
	/**
	 * Performs an action on resources which will be disposes subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 */
    @SafeVarargs
    @QtUninvokable
	public static <O extends QtObjectInterface> void performAndDispose(Consumer<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposing(data)){
			action.accept(ptr.get());
		}
	}
	
	/**
	 * Performs an action on multiple {@link QObject} resources which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 */
    @SafeVarargs
    @QtUninvokable
	public static <O extends QObject> void performAndDisposeLater(Consumer<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposingLater(data)){
			action.accept(ptr.get());
		}
	}
	
	/**
	 * Performs an action on resources which will be cleaned up subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 */
    @SafeVarargs
    @QtUninvokable
	public static <O> void performAndCleanup(Consumer<O> cleanup, Consumer<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.cleanup(cleanup, data)){
			action.accept(ptr.get());
		}
	}
    
	/**
	 * Performs an action on resources which will be disposes subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @SafeVarargs
    @QtUninvokable
	public static <O extends QtObjectInterface,R> R performAndDispose(Function<O[],R> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposing(data)){
			return action.apply(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O extends QObject,R> R performAndDisposeLater(Function<O[],R> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposingLater(data)){
			return action.apply(ptr.get());
		}
	}
	
	/**
	 * Performs an action on resources which will be cleaned up subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O,R> R performAndCleanup(Consumer<O> cleanup, Function<O[],R> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.cleanup(cleanup, data)){
			return action.apply(ptr.get());
		}
	}
    
	/**
	 * Performs an action on resources which will be disposes subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O extends QtObjectInterface> int performAndDispose(ToIntFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposing(data)){
			return action.applyAsInt(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O extends QObject> int performAndDisposeLater(ToIntFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposingLater(data)){
			return action.applyAsInt(ptr.get());
		}
	}
	
	/**
	 * Performs an action on resources which will be cleaned up subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O> int performAndCleanup(Consumer<O> cleanup, ToIntFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.cleanup(cleanup, data)){
			return action.applyAsInt(ptr.get());
		}
	}
    
	/**
	 * Performs an action on resources which will be disposes subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O extends QtObjectInterface> double performAndDispose(ToDoubleFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposing(data)){
			return action.applyAsDouble(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O extends QObject> double performAndDisposeLater(ToDoubleFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposingLater(data)){
			return action.applyAsDouble(ptr.get());
		}
	}
	
	/**
	 * Performs an action on resources which will be cleaned up subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O> double performAndCleanup(Consumer<O> cleanup, ToDoubleFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.cleanup(cleanup, data)){
			return action.applyAsDouble(ptr.get());
		}
	}
    
	/**
	 * Performs an action on resources which will be disposes subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O extends QtObjectInterface> long performAndDispose(ToLongFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposing(data)){
			return action.applyAsLong(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O extends QObject> long performAndDisposeLater(ToLongFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.disposingLater(data)){
			return action.applyAsLong(ptr.get());
		}
	}
	
	/**
	 * Performs an action on resources which will be cleaned up subsequently.
	 * 
	 * @param data multiple resources
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
    @SafeVarargs
	public static <O> long performAndCleanup(Consumer<O> cleanup, ToLongFunction<O[]> action, O... data){
		try(QScopedArrayPointer<O> ptr = QScopedArrayPointer.cleanup(cleanup, data)){
			return action.applyAsLong(ptr.get());
		}
	}
}
