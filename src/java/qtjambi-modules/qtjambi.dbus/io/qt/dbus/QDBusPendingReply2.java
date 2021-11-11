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

package io.qt.dbus;

import io.qt.core.QVariant;

public class QDBusPendingReply2<A,B> extends QDBusPendingReply<A> {

	private final Class<B> typeB;
	
	public QDBusPendingReply2() {
		typeB = null;
	}

	public QDBusPendingReply2(QDBusMessage message, Class<A> typeA, Class<B> typeB) {
		super(message, typeA);
		this.typeB = typeB;
	}

	public QDBusPendingReply2(QDBusPendingCall call, Class<A> typeA, Class<B> typeB) {
		super(call, typeA);
		this.typeB = typeB;
	}

	public QDBusPendingReply2(QDBusPendingReply2<A,B> other) {
		super(other);
		this.typeB = other.typeB;
	}

	@Override
	public QDBusPendingReply2<A,B> clone() {
		return new QDBusPendingReply2<A,B>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeB==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final B argumentAt1(){
		return QVariant.convert(argumentAt(1), typeB);
	}
}
