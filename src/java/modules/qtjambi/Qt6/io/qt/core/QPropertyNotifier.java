/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.NativeAccess;
import io.qt.StrictNonNull;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qpropertynotifier.html">QPropertyNotifier</a></p>
 * The QPropertyNotifier class controls the lifecycle of change callback installed on a {@link QProperty}.
 * 
 * QPropertyNotifier is created when registering a callback on a QProperty to listen to changes to the property's value,
 * using {@link QProperty#addNotifier(Runnable)}.
 * As long as the notifier is alive i.e. as long as a reference to the QPropertyNotifier instance exists, the callback remains installed.
 */
public final class QPropertyNotifier extends QPropertyObserver {
	private final Runnable functor;

	public QPropertyNotifier(@StrictNonNull Runnable functor) {
		super((QPrivateConstructor)null);
		initialize(this);
		this.functor = Objects.requireNonNull(functor, "Argument 'functor': null not expected.");
	}
	
	QPropertyNotifier(QPropertyBindingData bindingData, Runnable functor) {
		super((QPrivateConstructor)null);
		initialize(this);
		this.functor = Objects.requireNonNull(functor, "Argument 'functor': null not expected.");
		setSource(bindingData);
	}
	
	@NativeAccess
	private void invoke() {
		functor.run();
	}
    private native static void initialize(Object instance);
}
