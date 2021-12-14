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

import io.qt.InternalAccess.Cleanable;
import io.qt.QtUninvokable;

/**
 * <p>QScopeGuard is a class of which the sole purpose is to run the function <code>f</code> at the end of a try-with-resource block.
 * This is useful for guaranteeing your cleanup code is executed, whether the function is exited normally, 
 * exited early by a return statement, or exited by an exception.
 * </p>
 * <p>See <a href="https://doc.qt.io/qt/qscopeguard.html">QScopeGuard</a>.</p>
 * <p>Example:</p>
 * <code>
 * QIODevice device = ...<br>
 * try(var guard = new QScopeGuard(device::close)){<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;if(!device.open())<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;guard.dismiss();<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;else<br>
 * &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;device.write(...);<br>
 * }
 * </code>
 */
public final class QScopeGuard implements AutoCloseable {
	
	private static class Data{
		private Runnable data;
		private Cleanable cleanable;
		
	    @QtUninvokable
		void close(){
			if(data!=null) {
				try{
					data.run();
				}finally{
					data = null;
				}
			}
		}
	}

	private final Data data = new Data();
	
	/**
	 * Create a scope guard that will execute <code>function</code> at the end of the scope.
	 * @param function runnable to be executed
	 */
	public QScopeGuard(Runnable runnable) {
		this.data.data = runnable;
		this.data.cleanable = QtJambi_LibraryUtilities.internal.registerCleaner(this, this.data::close);
	}

	@Override
    @QtUninvokable
	public void close(){
		data.close();
		if(this.data.cleanable!=null)
			this.data.cleanable.clean();
	}
	
	/**
	 * Disarms the scope guard, so that the function F will not be called at the end of the scope.
	 */
	@QtUninvokable
	public void dismiss() {
		data.data = null;
		if(this.data.cleanable!=null)
			this.data.cleanable.clean();
	}
}
