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

import java.lang.ref.WeakReference;

import io.qt.QtObjectInterface;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;
import io.qt.core.QMetaObject.*;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qweakpointer.html">QWeakPointer</a>
 * similar to {@link java.lang.ref.WeakReference} but also supplying <code>null</code> when resource has been disposed.</p>
 */
public final class QWeakPointer<O extends QtObjectInterface> {
	
	public QWeakPointer(O object) {
		super();
		set(object);
	}
	
	public QWeakPointer() {
		super();
	}

	private WeakReference<O> reference;
	
    @QtUninvokable
	public void clear(){
		set(null);
	}
	
    @QtUninvokable
	public O data() {
		return this.reference==null ? null : this.reference.get();
	}
    
    @QtUninvokable
	public O get() {
		return this.reference==null ? null : this.reference.get();
	}
	
    @QtUninvokable
	public boolean isNull() {
		return data()==null;
	}
	
    @QtUninvokable
	public QWeakPointer<O> set(O object) {
    	if(object!=null && object.isDisposed())
    		object = null;
		O _object = data();
		if(_object != object) {
			if(_object!=null) {
				if(_object instanceof QObject) {
					((QObject)_object).destroyed.disconnect(this::unset);
				}else {
					DisposedSignal disposed = QtUtilities.getSignalOnDispose(_object);
					if(disposed!=null) {
						disposed.disconnect(this::unset);
					}
				}
			}
			this.reference = object==null ? null : new WeakReference<>(object);
			_object = data();
			if(_object!=null) {
				if(_object instanceof QObject) {
					((QObject)_object).destroyed.connect(this::unset);
				}else {
					DisposedSignal disposed = QtUtilities.getSignalOnDispose(_object);
					if(disposed!=null) {
						disposed.connect(this::unset);
					}
				}
			}
		}
		return this;
	}
	
	private void unset() {
		this.reference = null;
	}
}
