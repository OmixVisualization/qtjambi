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

import java.util.List;
import java.util.Map;
import java.util.NavigableMap;

import io.qt.core.QMetaType;

/**
 * The QDBusPendingReply class contains the reply to an asynchronous method call.
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qdbuspendingreply.html">QDBusPendingReply</a></p>
 */
public class QDBusPendingReply<A> extends io.qt.dbus.QDBusPendingReplyBase implements java.lang.Cloneable {
	
	static {
        QtJambi_LibraryUtilities.initialize();
    }
	
	private final QMetaType typeA;
	
	public static QDBusPendingReply<Object> newInstance(QDBusMessage message){
		return new QDBusPendingReply<>(message, QMetaType.Type.QVariant);
	}
	
	public static QDBusPendingReply<Object> newInstance(QDBusPendingCall call){
		return new QDBusPendingReply<>(call, QMetaType.Type.QVariant);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<?>> newListInstance(QDBusMessage message){
		return new QDBusPendingReply(message, QMetaType.Type.QVariantList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<?>> newListInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, QMetaType.Type.QVariantList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<String>> newStringListInstance(QDBusMessage message){
		return new QDBusPendingReply(message, QMetaType.Type.QStringList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<String>> newStringListInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, QMetaType.Type.QStringList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<NavigableMap<String,?>> newMapInstance(QDBusMessage message){
		return new QDBusPendingReply(message, QMetaType.Type.QVariantMap);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<NavigableMap<String,?>> newMapInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, QMetaType.Type.QVariantMap);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<Map<String,?>> newHashInstance(QDBusMessage message){
		return new QDBusPendingReply(message, QMetaType.Type.QVariantHash);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<Map<String,?>> newHashInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, QMetaType.Type.QVariantHash);
	}
	
	public QDBusPendingReply() {
        super();
		typeA = new QMetaType();
        assign(io.qt.dbus.QDBusPendingCall.fromError(new QDBusError()));
	}

	private QDBusPendingReply(QDBusMessage message, QMetaType.Type variantTypeA) {
		super();
		this.typeA = new QMetaType(variantTypeA);
        assign(io.qt.dbus.QDBusPendingCall.fromCompletedCall(message));
	}

	private QDBusPendingReply(QDBusPendingCall call, QMetaType.Type variantTypeA) {
		super();
		this.typeA = new QMetaType(variantTypeA);
        assign(call);
	}
	
	public QDBusPendingReply(QDBusMessage message, Class<A> typeA, QMetaType... instantiations) {
		super();
		this.typeA = QMetaType.fromType(typeA, instantiations);
        assign(io.qt.dbus.QDBusPendingCall.fromCompletedCall(message));
	}

	public QDBusPendingReply(QDBusPendingCall call, Class<A> typeA, QMetaType... instantiations) {
		super();
        this.typeA = QMetaType.fromType(typeA, instantiations);
        assign(call);
	}

	public QDBusPendingReply(QDBusPendingReply<A> other) {
		super();
		typeA = other.typeA;
        assign(other);
	}
	
	private boolean hasCalculatedMetaTypes = false;
	
	private void calculateMetaTypes() {
		if(!hasCalculatedMetaTypes) {
			hasCalculatedMetaTypes = true;
			QMetaType[] metaTypes = new QMetaType[numberOfArguments()];
			fillMetaTypes(metaTypes);
			for (int i=0; i<metaTypes.length; ++i) {
				if(metaTypes[i]==null)
					throw new NullPointerException("QDBusPendingReply: meta type is null.");
				if(!metaTypes[i].isValid())
					throw new RuntimeException("QDBusPendingReply: meta type is unknown.");
				String signature = QDBusMetaType.typeToSignature(metaTypes[i]);
				if(signature==null || signature.isEmpty())
					throw new RuntimeException("QDBusPendingReply: type "+ metaTypes[i].name() +" is not registered with QtDBus");
			}
			setMetaTypes(metaTypes);
		}
	}
	
	void fillMetaTypes(QMetaType[] metaTypes) {
		metaTypes[0] = typeA;
	}

	public QDBusPendingReply<A> clone(){
		return new QDBusPendingReply<>(this);
	}
	
	@io.qt.QtUninvokable
    public final int count(){
        return isInvalid() ? 0 : numberOfArguments();
    }
	
	boolean isInvalid() {
		return typeA==null || !typeA.isValid();
	}

	int numberOfArguments() {
		return 1;
	}

	@SuppressWarnings("unchecked")
	@io.qt.QtUninvokable
    final <R> R argumentAt(QMetaType type, int index){
        if(index<0 || index>=numberOfArguments())
            throw new IndexOutOfBoundsException("Index out of range: " + index);
        if(isInvalid())
        	throw new RuntimeException("Invalid QDBusPendingReply");
        calculateMetaTypes();
        Object result = argumentAt(index);
        if(result instanceof QDBusArgument) {
        	QDBusArgument argument = (QDBusArgument)result;
        	result = QDBusMetaType.demarshall(argument, type);
        }
        return (R)result;
    }
	
	@io.qt.QtUninvokable
	public final A argumentAt0(){
		return argumentAt(typeA, 0);
	}
	
	@io.qt.QtUninvokable
	public final A value(){
		return argumentAt0();
	}
}
