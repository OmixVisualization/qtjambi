/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.dbus;

import io.qt.core.QMetaType;

public class QDBusPendingReply5<A,B,C,D,E> extends QDBusPendingReply4<A,B,C,D> {

	private final QMetaType typeE;
	
	public QDBusPendingReply5() {
		typeE = new QMetaType();
	}

	public QDBusPendingReply5(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE) {
		super(message, typeA, typeB, typeC, typeD);
		this.typeE = QMetaType.fromType(typeE);
	}

	public QDBusPendingReply5(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE) {
		super(call, typeA, typeB, typeC, typeD);
		this.typeE = QMetaType.fromType(typeE);
	}

	public QDBusPendingReply5(QDBusPendingReply5<A,B,C,D,E> other) {
		super(other);
		this.typeE = other.typeE;
	}
	
	public QDBusPendingReply5(QDBusPendingReply4<A,B,C,D> other, Class<E> typeE, QMetaType... instantiations) {
		super(other);
		this.typeE = QMetaType.fromType(typeE, instantiations);
	}

	@Override
	public QDBusPendingReply5<A,B,C,D,E> clone() {
		return new QDBusPendingReply5<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeE==null || !typeE.isValid();
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final E argumentAt4(){
		return argumentAt(typeE, 4);
	}
	
	void fillMetaTypes(QMetaType[] metaTypes) {
		metaTypes[4] = typeE;
		super.fillMetaTypes(metaTypes);
	}
}
