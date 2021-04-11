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

import io.qt.QtObject;

/**
 * <p>Supertype for typed and untyped QFuture</p>
 */
public abstract class QAbstractFuture extends QtObject {
	QAbstractFuture(QPrivateConstructor p) {
		super(p);
	}
	
	@io.qt.QtUninvokable
    public abstract void cancel();
	
	@io.qt.QtUninvokable
    public abstract boolean isCanceled();
	
	@io.qt.QtUninvokable
    public abstract boolean isFinished();
	
	@io.qt.QtUninvokable
    public abstract boolean isPaused();
	
	@io.qt.QtUninvokable
    public abstract boolean isRunning();
	
	@io.qt.QtUninvokable
    public abstract boolean isStarted();
	
	@io.qt.QtUninvokable
    public abstract void pause();
	
	@io.qt.QtUninvokable
    public abstract int progressMaximum();
	
	@io.qt.QtUninvokable
    public abstract int progressMinimum();
	
	@io.qt.QtUninvokable
    public abstract java.lang.String progressText();
	
	@io.qt.QtUninvokable
    public abstract int progressValue();
	
	@io.qt.QtUninvokable
    public abstract int resultCount();
	
	@io.qt.QtUninvokable
    public abstract void resume();
	
	@io.qt.QtUninvokable
    public abstract void setPaused(boolean paused);
	
	@io.qt.QtUninvokable
    public abstract void togglePaused();
	
	@io.qt.QtUninvokable
    public abstract void waitForFinished();
	
	@Override
    public abstract QAbstractFuture clone();
	
	@io.qt.QtUninvokable
	abstract io.qt.core.QFutureInterfaceBase futureInterface();
}
