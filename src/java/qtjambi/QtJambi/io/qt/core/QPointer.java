/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.QtObjectInterface;
import io.qt.QtUninvokable;
import io.qt.QtUtilities;
import io.qt.core.QMetaObject.*;

public final class QPointer<O extends QtObjectInterface> {
	
	public QPointer(O object) {
		super();
		set(object);
	}
	
	public QPointer() {
		super();
	}

	private O object;
	
    @QtUninvokable
	public void clear(){
		set(null);
	}
	
    @QtUninvokable
	public O data() {
		return this.object;
	}
    
    @QtUninvokable
	public O get() {
		return this.object;
	}
	
    @QtUninvokable
	public boolean isNull() {
		return this.object==null;
	}
	
    @QtUninvokable
	public QPointer<O> set(O object) {
		if(this.object != object) {
			if(this.object!=null) {
				if(this.object instanceof QObject) {
					((QObject)this.object).destroyed.disconnect(this::unset);
				}else {
					DisposedSignal disposed = QtUtilities.getSignalOnDispose(this.object);
					if(disposed!=null) {
						disposed.disconnect(this::unset);
					}
				}
			}
			this.object = object;
			if(this.object!=null) {
				if(this.object instanceof QObject) {
					((QObject)this.object).destroyed.connect(this::unset);
				}else {
					DisposedSignal disposed = QtUtilities.getSignalOnDispose(this.object);
					if(disposed!=null) {
						disposed.connect(this::unset);
					}
				}
			}
		}
		return this;
	}
	
	private void unset() {
		this.object = null;
	}
}
