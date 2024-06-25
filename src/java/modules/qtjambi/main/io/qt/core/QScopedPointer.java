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

import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.ToDoubleFunction;
import java.util.function.ToIntFunction;
import java.util.function.ToLongFunction;
import java.util.logging.Logger;

import io.qt.InternalAccess.Cleanable;
import io.qt.NonNull;
import io.qt.QtObjectInterface;
import io.qt.QtUninvokable;
import io.qt.StrictNonNull;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qscopedpointer.html">QScopedPointer</a>
 * to be used inside a try-with-resource block.</p>
 * <p>Example:</p>
 * <code>
 * try(var pointer = QScopedPointer.disposing(new QDialog())){<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;QDialog dialog = pointer.get();<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;dialog.exec();<br>
 * }<br>
 * &#47;&#47; dialog is disposed
 * </code>
 * <p>Alternatively, <i>performAndDispose</i> disposes a recource after calling a lambda expression:</p>
 * <code>
 * QScopedPointer.performAndDispose(QDialog::exec, new QDialog());
 * </code>
 * 
 */
public final class QScopedPointer<O> implements AutoCloseable {
	
	static final Logger logger = Logger.getLogger("io.qt.core");
	
	static class Data<O>{
		QScope.AbstractEntry<O> entry;
		Cleanable cleanable;
		void cleanup() {
			if(entry!=null) {
				entry.cleanup();
				entry = null;
			}
		}
	}

	private final Data<O> data = new Data<>();
	
	private QScopedPointer(O data, Consumer<O> cleanup) {
		this(new QScope.CleanupEntry<>(data, cleanup));
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	private QScopedPointer(QtObjectInterface data) {
		this((QScope.AbstractEntry)new QScope.DisposingEntry(data));
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	private QScopedPointer(QObject data) {
		this((QScope.AbstractEntry)new QScope.DisposingLaterEntry(data));
	}
	
	private QScopedPointer(QScope.AbstractEntry<O> entry) {
		this.data.entry = entry;
		this.data.cleanable = QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::cleanup);
	}
	
	/**
	 * <p>Returns the object stored by this scoped pointer</p>
	 * <p>See <a href="https://doc.qt.io/qt/qscopedpointer.html#data">QScopedPointer::data()const</a>.</p>
	 * @return data
	 */
    @QtUninvokable
	public O data() {
		return data.entry.data;
	}

	/**
	 * @see #data()
	 */
    @QtUninvokable
	public O get() {
		return data.entry.data;
	}

	/**
	 * <p>Removes and returns the object stored by this scoped pointer without cleanup</p>
	 * @return data
	 */
    @QtUninvokable
	public O take() {
		O oldData = data.entry.data;
		data.entry.data = null;
		return oldData;
	}

	/**
	 * <p>performs cleanup operation on the contained object and sets new reference.</p>
	 * <p>See <a href="https://doc.qt.io/qt/qscopedpointer.html#reset">QScopedPointer::reset(T*)</a>.</p>
	 */
    @SuppressWarnings({ "unchecked", "rawtypes" })
	@QtUninvokable
	public void reset(O other) {
		if(data.entry!=null) {
			QScope.AbstractEntry<O> oldEntry = data.entry;
			if(oldEntry instanceof QScope.DisposingEntry) {
				this.data.entry = (QScope.AbstractEntry)new QScope.DisposingEntry((QtObjectInterface)other);
			}else if(oldEntry instanceof QScope.RunningEntry) {
				this.data.entry = (QScope.AbstractEntry)new QScope.RunningEntry((Runnable)other);
			}else if(oldEntry instanceof QScope.CleanupEntry) {
				this.data.entry = (QScope.AbstractEntry)new QScope.CleanupEntry(other, ((QScope.CleanupEntry)oldEntry).getCleanup());
			}else {
				throw new IllegalStateException("Unable to reset scoped pointer.");
			}
			oldEntry.cleanup();
		}
	}
    
    /**
     * Overloaded function for {@link #reset(Object)} with <code>other=null</code>.
     */
    @QtUninvokable
	public void reset() {
    	reset(null);
    }
	
	/**
	 * <p>See <a href="https://doc.qt.io/qt/qscopedpointer.html#isNull">QScopedPointer::isNull()const</a>.</p>
	 */
    @QtUninvokable
	public boolean isNull(){
		return data.entry==null || data.entry.data==null;
	}
	
    @QtUninvokable
	public void swap(@StrictNonNull QScopedPointer<O> other){
    	QScope.AbstractEntry<O> oldEntry = data.entry;
		this.data.entry = other.data.entry;
		other.data.entry = oldEntry;
	}

    /**
     * performs the cleanup operation
     */
	@Override
    @QtUninvokable
	public void close(){
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
     * Creates a scoped pointer for the given object performing {@link QtObjectInterface#dispose()} at cleanup.
     * @param data the contained object
     * @return scoped pointer
     */
    @QtUninvokable
	public static <O extends QtObjectInterface> @NonNull QScopedPointer<O> disposing(O data){
		return new QScopedPointer<>(data);
	}
	
    /**
     * Creates a scoped pointer for the given object performing {@link QObject#disposeLater()} at cleanup.
     * @param data the contained object
     * @return scoped pointer
     */
    @QtUninvokable
	public static <O extends QObject> @NonNull QScopedPointer<O> disposingLater(O data){
		return new QScopedPointer<>(data);
	}
	
    @QtUninvokable
    @Deprecated(forRemoval=true)
	public static <O> @NonNull QScopedPointer<O> cleanup(O data, @StrictNonNull Consumer<O> cleanup){
		return new QScopedPointer<>(data, cleanup);
	}
    
    /**
     * Creates a scoped pointer for the given object performing the given operation at cleanup.
     * @param cleanup operation performed at cleanup
     * @param data the contained object
     * @return scoped pointer
     */
    @QtUninvokable
	public static <O> @NonNull QScopedPointer<O> cleanup(@StrictNonNull Consumer<O> cleanup, O data){
		return new QScopedPointer<>(data, cleanup);
	}
	
	/**
	 * Performs an action on a resource which will be disposes subsequently.
	 * <code>
	 * QScopedPointer.performAndDispose(QDialog::exec, new QDialog());
	 * </code>
	 * 
	 * @param data the resource
	 * @param action the action
	 */
    @QtUninvokable
	public static <O extends QtObjectInterface> void performAndDispose(Consumer<O> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposing(data)){
			action.accept(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * <code>
	 * QScopedPointer.performAndDispose(QDialog::exec, new QDialog());
	 * </code>
	 * 
	 * @param data the resource
	 * @param action the action
	 */
    @QtUninvokable
	public static <O extends QObject> void performAndDisposeLater(Consumer<O> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposingLater(data)){
			action.accept(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a resource which will be cleaned up subsequently.
	 * <code>
	 * QScopedPointer.performAndCleanup(object-> { 
	 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;...//action
	 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;},
	 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;object -> { object.cleanup(); }, new CustomObject() );
	 * </code>
	 * 
	 * @param data the resource
	 * @param action the action
	 */
    @QtUninvokable
	public static <O> void performAndCleanup(Consumer<O> action, Consumer<O> cleanup, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.cleanup(cleanup, data)){
			action.accept(ptr.get());
		}
	}
    
	/**
	 * Performs an action on a resource which will be disposes subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O extends QtObjectInterface,R> R performAndDispose(Function<O,R> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposing(data)){
			return action.apply(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O extends QObject,R> R performAndDisposeLater(Function<O,R> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposingLater(data)){
			return action.apply(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a resource which will be cleaned up subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O,R> R performAndCleanup(Function<O,R> action, Consumer<O> cleanup, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.cleanup(cleanup, data)){
			return action.apply(ptr.get());
		}
	}
    
	/**
	 * Performs an action on a resource which will be disposes subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O extends QtObjectInterface> int performAndDispose(ToIntFunction<O> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposing(data)){
			return action.applyAsInt(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O extends QObject> int performAndDisposeLater(ToIntFunction<O> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposingLater(data)){
			return action.applyAsInt(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a resource which will be cleaned up subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O> int performAndCleanup(ToIntFunction<O> action, Consumer<O> cleanup, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.cleanup(cleanup, data)){
			return action.applyAsInt(ptr.get());
		}
	}
    
	/**
	 * Performs an action on a resource which will be disposes subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O extends QtObjectInterface> double performAndDispose(ToDoubleFunction<O> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposing(data)){
			return action.applyAsDouble(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O extends QObject> double performAndDisposeLater(ToDoubleFunction<O> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposingLater(data)){
			return action.applyAsDouble(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a resource which will be cleaned up subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O> double performAndCleanup(ToDoubleFunction<O> action, Consumer<O> cleanup, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.cleanup(cleanup, data)){
			return action.applyAsDouble(ptr.get());
		}
	}
    
	/**
	 * Performs an action on a resource which will be disposes subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O extends QtObjectInterface> long performAndDispose(ToLongFunction<O> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposing(data)){
			return action.applyAsLong(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a {@link QObject} resource which will be called {@link QObject#disposeLater()} subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O extends QObject> long performAndDisposeLater(ToLongFunction<O> action, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.disposingLater(data)){
			return action.applyAsLong(ptr.get());
		}
	}
	
	/**
	 * Performs an action on a resource which will be cleaned up subsequently.
	 * 
	 * @param data the resource
	 * @param action the action
	 * @return result
	 */
    @QtUninvokable
	public static <O> long performAndCleanup(ToLongFunction<O> action, Consumer<O> cleanup, O data){
		try(QScopedPointer<O> ptr = QScopedPointer.cleanup(cleanup, data)){
			return action.applyAsLong(ptr.get());
		}
	}
}
