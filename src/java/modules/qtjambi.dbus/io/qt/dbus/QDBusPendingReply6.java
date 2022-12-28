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

package io.qt.dbus;

import io.qt.core.QMetaType;

/**
 * The QDBusPendingReply class contains the reply to an asynchronous method call.
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qdbuspendingreply.html">QDBusPendingReply</a></p>
 */
public class QDBusPendingReply6<A,B,C,D,E,F> extends QDBusPendingReply5<A,B,C,D,E> {

	private final QMetaType typeF;
	
	public QDBusPendingReply6() {
		typeF = new QMetaType();
	}

	public QDBusPendingReply6(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF) {
		super(message, typeA, typeB, typeC, typeD, typeE);
		this.typeF = QMetaType.fromType(typeF);
	}

	public QDBusPendingReply6(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF) {
		super(call, typeA, typeB, typeC, typeD, typeE);
		this.typeF = QMetaType.fromType(typeF);
	}

	public QDBusPendingReply6(QDBusPendingReply6<A,B,C,D,E,F> other) {
		super(other);
		this.typeF = other.typeF;
	}
	
	public QDBusPendingReply6(QDBusPendingReply5<A,B,C,D,E> other, Class<F> typeF, QMetaType... instantiations) {
		super(other);
		this.typeF = QMetaType.fromType(typeF, instantiations);
	}

	@Override
	public QDBusPendingReply6<A,B,C,D,E,F> clone() {
		return new QDBusPendingReply6<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeF==null || !typeF.isValid();
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final F argumentAt5(){
		return argumentAt(typeF, 5);
	}

	void fillMetaTypes(QMetaType[] metaTypes) {
		metaTypes[5] = typeF;
		super.fillMetaTypes(metaTypes);
	}
}
