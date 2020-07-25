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

public class QDBusPendingReply7<A,B,C,D,E,F,G> extends QDBusPendingReply6<A,B,C,D,E,F> {

	private final Class<G> typeG;
	
	public QDBusPendingReply7() {
		typeG = null;
	}

	public QDBusPendingReply7(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG) {
		super(message, typeA, typeB, typeC, typeD, typeE, typeF);
		this.typeG = typeG;
	}

	public QDBusPendingReply7(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG) {
		super(call, typeA, typeB, typeC, typeD, typeE, typeF);
		this.typeG = typeG;
	}

	public QDBusPendingReply7(QDBusPendingReply7<A,B,C,D,E,F,G> other) {
		super(other);
		this.typeG = other.typeG;
	}

	@Override
	public QDBusPendingReply7<A,B,C,D,E,F,G> clone() {
		return new QDBusPendingReply7<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeG==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final G argumentAt6(){
		return QVariant.convert(argumentAt(6), typeG);
	}
}
