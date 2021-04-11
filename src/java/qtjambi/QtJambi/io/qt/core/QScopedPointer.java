/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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

import io.qt.QtUninvokable;
import io.qt.internal.QtJambiInternal;
import io.qt.internal.QtJambiObject;

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
 */
public final class QScopedPointer<O> implements AutoCloseable {
	
	private static class Data<O>{
		private O data;
		private Consumer<O> cleanup;
		
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
		QtJambiInternal.registerCleaner(this, this.data::close);
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
	}
	
    @QtUninvokable
	private static void dispose(QtJambiObject data) {
		if(QtJambiInternal.nativeId(data)!=0)
			data.dispose();
	}
	
    @QtUninvokable
	private static void disposeLater(QObject data) {
		if(QtJambiInternal.nativeId(data)!=0)
			data.disposeLater();
	}
	
    @QtUninvokable
	public static <O extends QtJambiObject> QScopedPointer<O> disposing(O data){
		return new QScopedPointer<>(data, QScopedPointer::dispose);
	}
	
    @QtUninvokable
	public static <O extends QObject> QScopedPointer<O> disposingLater(O data){
		return new QScopedPointer<>(data, QScopedPointer::disposeLater);
	}
	
    @QtUninvokable
	public static <O> QScopedPointer<O> cleanup(O data, Consumer<O> cleanup){
		return new QScopedPointer<>(data, cleanup);
	}
}
