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

import java.util.List;
import java.util.Map;
import java.util.NavigableMap;
import java.util.Objects;

import io.qt.core.QMetaType;
import io.qt.core.QVariant;

public class QDBusPendingReply<A> extends io.qt.dbus.QDBusPendingCall.Impl implements java.lang.Cloneable {
	
	static {
        QtJambi_LibraryInitializer.init();
    }
	
	private final Class<A> typeA;
	private final QMetaType variantTypeA;
	
	public static QDBusPendingReply<Object> newInstance(QDBusMessage message){
		return new QDBusPendingReply<>(message, Object.class, QMetaType.Type.QVariant);
	}
	
	public static QDBusPendingReply<Object> newInstance(QDBusPendingCall call){
		return new QDBusPendingReply<>(call, Object.class, QMetaType.Type.QVariant);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<?>> newListInstance(QDBusMessage message){
		return new QDBusPendingReply(message, List.class, QMetaType.Type.QVariantList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<?>> newListInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, List.class, QMetaType.Type.QVariantList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<String>> newStringListInstance(QDBusMessage message){
		return new QDBusPendingReply(message, List.class, QMetaType.Type.QStringList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<String>> newStringListInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, List.class, QMetaType.Type.QStringList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<NavigableMap<String,?>> newMapInstance(QDBusMessage message){
		return new QDBusPendingReply(message, NavigableMap.class, QMetaType.Type.QVariantMap);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<NavigableMap<String,?>> newMapInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, NavigableMap.class, QMetaType.Type.QVariantMap);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<Map<String,?>> newHashInstance(QDBusMessage message){
		return new QDBusPendingReply(message, Map.class, QMetaType.Type.QVariantHash);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<Map<String,?>> newHashInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, Map.class, QMetaType.Type.QVariantHash);
	}
	
	public QDBusPendingReply() {
        super(io.qt.dbus.QDBusPendingCall.fromError(new QDBusError()));
		typeA = null;
		variantTypeA = null;
	}

	private QDBusPendingReply(QDBusMessage message, Class<A> typeA, QMetaType.Type variantTypeA) {
		super(io.qt.dbus.QDBusPendingCall.fromCompletedCall(message));
		this.typeA = Objects.requireNonNull(typeA);
		this.variantTypeA = variantTypeA!=null ? new QMetaType(variantTypeA) : null;
	}

	private QDBusPendingReply(QDBusPendingCall call, Class<A> typeA, QMetaType.Type variantTypeA) {
		super(call);
		this.typeA = Objects.requireNonNull(typeA);
		this.variantTypeA = variantTypeA!=null ? new QMetaType(variantTypeA) : null;
	}
	
	public QDBusPendingReply(QDBusMessage message, Class<A> typeA) {
		super(io.qt.dbus.QDBusPendingCall.fromCompletedCall(message));
		this.typeA = Objects.requireNonNull(typeA);
		variantTypeA = null;
	}

	public QDBusPendingReply(QDBusPendingCall call, Class<A> typeA) {
		super(call);
		this.typeA = Objects.requireNonNull(typeA);
		variantTypeA = null;
	}

	public QDBusPendingReply(QDBusMessage message, Class<A> typeA, QMetaType... instantiations) {
		super(io.qt.dbus.QDBusPendingCall.fromCompletedCall(message));
		this.typeA = Objects.requireNonNull(typeA);
		variantTypeA = instantiations!=null ? QMetaType.fromType(typeA, instantiations) : null;
	}

	public QDBusPendingReply(QDBusPendingCall call, Class<A> typeA, QMetaType... instantiations) {
		super(call);
		this.typeA = Objects.requireNonNull(typeA);
		variantTypeA = instantiations!=null ? QMetaType.fromType(typeA, instantiations) : null;
	}

	public QDBusPendingReply(QDBusPendingReply<A> other) {
		super(other);
		this.typeA = other.typeA;
		variantTypeA = other.variantTypeA;
	}

	public QDBusPendingReply<A> clone(){
		return new QDBusPendingReply<>(this);
	}
	
	@io.qt.QtUninvokable
    public final int count(){
        return isInvalid() ? 0 : numberOfArguments();
    }
	
	boolean isInvalid() {
		return typeA==null;
	}

	int numberOfArguments() {
		return 1;
	}

	@io.qt.QtUninvokable
    final Object argumentAt(int index){
        if(index<0 || index>=count())
            throw new IndexOutOfBoundsException("Index out of range: " + index);
        if(isInvalid())
        	throw new RuntimeException("Invalid QDBusPendingReply");
        waitForFinished();
        return reply().arguments().get(index);
    }
	
	@io.qt.QtUninvokable
	public final A argumentAt0(){
		if(variantTypeA==null) {
			return QVariant.convert(argumentAt(0), typeA);
		}else{
			return QVariant.convert(QVariant.convert(argumentAt(0), variantTypeA.id()), typeA);
		}
	}
	
	@io.qt.QtUninvokable
	public final A value(){
		return argumentAt0();
	}
}
