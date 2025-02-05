/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.remoteobjects;

import java.util.List;
import java.util.Map;
import java.util.NavigableMap;

import io.qt.*;
import io.qt.core.QMetaType;
import io.qt.core.QVariant;

/**
 * A generic version of QRemoteObjectPendingCall.
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qremoteobjectpendingreply.html">QRemoteObjectPendingReply</a></p>
 */
public final class QRemoteObjectPendingReply<A> extends io.qt.QtObject implements QRemoteObjectPendingCall, java.lang.Cloneable {
	
	static {
        QtJambi_LibraryUtilities.initialize();
    }
	
	private final QMetaType metaType;
	
	public static @NonNull QRemoteObjectPendingReply<Object> newInstance(@NonNull QRemoteObjectPendingCall call){
		return new QRemoteObjectPendingReply<>(call, QMetaType.Type.QVariant);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static @NonNull QRemoteObjectPendingReply<List<?>> newListInstance(@NonNull QRemoteObjectPendingCall call){
		return new QRemoteObjectPendingReply(call, QMetaType.Type.QVariantList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static @NonNull QRemoteObjectPendingReply<List<String>> newStringListInstance(@NonNull QRemoteObjectPendingCall call){
		return new QRemoteObjectPendingReply(call, QMetaType.Type.QStringList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static @NonNull QRemoteObjectPendingReply<NavigableMap<String,?>> newMapInstance(@NonNull QRemoteObjectPendingCall call){
		return new QRemoteObjectPendingReply(call, QMetaType.Type.QVariantMap);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static @NonNull QRemoteObjectPendingReply<Map<String,?>> newHashInstance(@NonNull QRemoteObjectPendingCall call){
		return new QRemoteObjectPendingReply(call, QMetaType.Type.QVariantHash);
	}
	
	public QRemoteObjectPendingReply() {
		super((QPrivateConstructor)null);
		initialize_native(this);
		metaType = new QMetaType();
	}

	private QRemoteObjectPendingReply(QRemoteObjectPendingCall call, QMetaType.Type variantTypeA) {
		super((QPrivateConstructor)null);
		initialize_native(this, call);
		this.metaType = new QMetaType(variantTypeA);
	}
	
	public QRemoteObjectPendingReply(@NonNull QRemoteObjectPendingCall call, @Nullable Class<A> typeA, @NonNull QMetaType @NonNull... instantiations) {
		super((QPrivateConstructor)null);
        initialize_native(this, call);
        this.metaType = QMetaType.fromType(typeA, instantiations);
	}
	
	/**
	 * Copy constructor
	 */
	public QRemoteObjectPendingReply(@StrictNonNull QRemoteObjectPendingReply<A> other) {
		super((QPrivateConstructor)null);
		initialize_native(this, other);
		metaType = other.metaType;
	}
	
	/**
	 * {@inheritDoc}
	 */
	public @NonNull QRemoteObjectPendingReply<A> clone(){
		return new QRemoteObjectPendingReply<>(this);
	}
	
    /**
     * <p>See <a href="https://doc.qt.io/qt/qremoteobjectpendingreply.html#returnValue">QRemoteObjectPendingReply::returnValue() const</a></p>
     */
	@SuppressWarnings("unchecked")
	@io.qt.QtUninvokable
	public final A returnValue(){
		return (A)QVariant.convert(QRemoteObjectPendingCall.super.returnValue(), metaType);
	}
	
	@io.qt.QtUninvokable
	private native static void initialize_native(QRemoteObjectPendingReply<?> instance);
	
	@io.qt.QtUninvokable
	private native static void initialize_native(QRemoteObjectPendingReply<?> instance, io.qt.remoteobjects.QRemoteObjectPendingCall other);
}
