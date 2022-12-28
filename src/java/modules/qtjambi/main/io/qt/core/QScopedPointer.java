/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
import java.util.function.Consumer;
import java.util.function.Function;
import java.util.function.ToDoubleFunction;
import java.util.function.ToIntFunction;
import java.util.function.ToLongFunction;

import io.qt.QtUninvokable;
import io.qt.InternalAccess.Cleanable;
import io.qt.QtObjectInterface;

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
	
	private static class Data<O>{
		private O data;
		private Consumer<O> cleanup;
		private Cleanable cleanable;
		
	    @QtUninvokable
		void close(){
			if(cleanup!=null) {
				try{
					if(data!=null)
						cleanup.accept(data);
				}finally{
					cleanup = null;
				}
			}
			data = null;
		}
	}

	private final Data<O> data = new Data<>();
	
	private QScopedPointer(O data, Consumer<O> cleanup) {
		this.data.data = data;
		this.data.cleanup = Objects.requireNonNull(cleanup);
		this.data.cleanable = QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::close);
	}

    @QtUninvokable
	public O data() {
		return data.data;
	}

    @QtUninvokable
	public O get() {
		return data.data;
	}

    @QtUninvokable
	public O take() {
		O oldData = data.data;
		data.data = null;
		return oldData;
	}

    @QtUninvokable
	public void reset(O other) {
		if(data.cleanup!=null) {
			O oldData = data.data;
			data.data = other;
			data.cleanup.accept(oldData);
		}
	}
	
    @QtUninvokable
	public boolean isNull(){
		return data.data==null;
	}
	
    @QtUninvokable
	public void swap(QScopedPointer<O> other){
		O oldData = data.data;
		Consumer<O> oldCleanup = data.cleanup;
		this.data.data = other.data.data;
		this.data.cleanup = other.data.cleanup;
		other.data.data = oldData;
		other.data.cleanup = oldCleanup;
	}

	@Override
    @QtUninvokable
	public void close(){
		data.close();
		if(this.data.cleanable!=null)
			this.data.cleanable.clean();
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
	
    @QtUninvokable
	public static <O extends QtObjectInterface> QScopedPointer<O> disposing(O data){
		return new QScopedPointer<>(data, QScopedPointer::dispose);
	}
	
    @QtUninvokable
	public static <O extends QObject> QScopedPointer<O> disposingLater(O data){
		return new QScopedPointer<>(data, QScopedPointer::disposeLater);
	}
	
    @QtUninvokable
    @Deprecated
	public static <O> QScopedPointer<O> cleanup(O data, Consumer<O> cleanup){
		return new QScopedPointer<>(data, cleanup);
	}
    
    @QtUninvokable
	public static <O> QScopedPointer<O> cleanup(Consumer<O> cleanup, O data){
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
