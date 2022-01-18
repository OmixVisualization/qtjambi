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

public class QDBusPendingReply4<A,B,C,D> extends QDBusPendingReply3<A,B,C> {

	private final QMetaType typeD;
	
	public QDBusPendingReply4() {
		typeD = new QMetaType();
	}

	public QDBusPendingReply4(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD) {
		super(message, typeA, typeB, typeC);
		this.typeD = QMetaType.fromType(typeD);
	}

	public QDBusPendingReply4(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD) {
		super(call, typeA, typeB, typeC);
		this.typeD = QMetaType.fromType(typeD);
	}

	public QDBusPendingReply4(QDBusPendingReply4<A,B,C,D> other) {
		super(other);
		this.typeD = other.typeD;
	}
	
	public QDBusPendingReply4(QDBusPendingReply3<A,B,C> other, Class<D> typeD, QMetaType... instantiations) {
		super(other);
		this.typeD = QMetaType.fromType(typeD, instantiations);
	}

	@Override
	public QDBusPendingReply4<A,B,C,D> clone() {
		return new QDBusPendingReply4<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeD==null || !typeD.isValid();
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final D argumentAt3(){
		return argumentAt(typeD, 3);
	}
	
	void fillMetaTypes(QMetaType[] metaTypes) {
		metaTypes[3] = typeD;
		super.fillMetaTypes(metaTypes);
	}
}
