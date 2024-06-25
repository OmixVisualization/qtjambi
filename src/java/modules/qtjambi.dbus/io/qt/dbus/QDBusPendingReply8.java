/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.StrictNonNull;
import io.qt.core.QMetaType;

/**
 * The QDBusPendingReply class contains the reply to an asynchronous method call.
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qdbuspendingreply.html">QDBusPendingReply</a></p>
 */
public class QDBusPendingReply8<A,B,C,D,E,F,G,H> extends QDBusPendingReply7<A,B,C,D,E,F,G> {

	private final QMetaType typeH;
	
	public QDBusPendingReply8() {
		typeH = new QMetaType();
	}

	public QDBusPendingReply8(@NonNull QDBusMessage message, @Nullable Class<A> typeA, @Nullable Class<B> typeB, @Nullable Class<C> typeC, @Nullable Class<D> typeD, @Nullable Class<E> typeE, @Nullable Class<F> typeF, @Nullable Class<G> typeG, @Nullable Class<H> typeH) {
		super(message, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		this.typeH = QMetaType.fromType(typeH);
	}

	public QDBusPendingReply8(@NonNull QDBusPendingCall call, @Nullable Class<A> typeA, @Nullable Class<B> typeB, @Nullable Class<C> typeC, @Nullable Class<D> typeD, @Nullable Class<E> typeE, @Nullable Class<F> typeF, @Nullable Class<G> typeG, @Nullable Class<H> typeH) {
		super(call, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		this.typeH = QMetaType.fromType(typeH);
	}

	public QDBusPendingReply8(@StrictNonNull QDBusPendingReply8<A,B,C,D,E,F,G,H> other) {
		super(other);
		this.typeH = other.typeH;
	}
	
	public QDBusPendingReply8(@NonNull QDBusPendingReply7<A,B,C,D,E,F,G> other, @Nullable Class<H> typeH, @NonNull QMetaType @NonNull... instantiations) {
		super(other);
		this.typeH = QMetaType.fromType(typeH, instantiations);
	}

	@Override
	public @NonNull QDBusPendingReply8<A,B,C,D,E,F,G,H> clone() {
		return new QDBusPendingReply8<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeH==null || !typeH.isValid();
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final H argumentAt7(){
		return argumentAt(typeH, 7);
	}
	
	void fillMetaTypes(QMetaType[] metaTypes) {
		metaTypes[7] = typeH;
		super.fillMetaTypes(metaTypes);
	}
}
