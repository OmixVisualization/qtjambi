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

package io.qt.dbus;

import io.qt.core.QVariant;

public class QDBusPendingReply8<A,B,C,D,E,F,G,H> extends QDBusPendingReply7<A,B,C,D,E,F,G> {

	private final Class<H> typeH;
	
	public QDBusPendingReply8() {
		typeH = null;
	}

	public QDBusPendingReply8(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH) {
		super(message, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		this.typeH = typeH;
	}

	public QDBusPendingReply8(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH) {
		super(call, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		this.typeH = typeH;
	}

	public QDBusPendingReply8(QDBusPendingReply8<A,B,C,D,E,F,G,H> other) {
		super(other);
		this.typeH = other.typeH;
	}

	@Override
	public QDBusPendingReply8<A,B,C,D,E,F,G,H> clone() {
		return new QDBusPendingReply8<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeH==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final H argumentAt7(){
		return QVariant.convert(argumentAt(7), typeH);
	}
}
