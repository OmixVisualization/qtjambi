package io.qt.dbus;

import io.qt.core.QVariant;

public class QDBusPendingReply3<A,B,C> extends QDBusPendingReply2<A,B> {

	private final Class<C> typeC;
	
	public QDBusPendingReply3() {
		typeC = null;
	}

	public QDBusPendingReply3(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC) {
		super(message, typeA, typeB);
		this.typeC = typeC;
	}

	public QDBusPendingReply3(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC) {
		super(call, typeA, typeB);
		this.typeC = typeC;
	}

	public QDBusPendingReply3(QDBusPendingReply3<A,B,C> other) {
		super(other);
		this.typeC = other.typeC;
	}

	@Override
	public QDBusPendingReply3<A,B,C> clone() {
		return new QDBusPendingReply3<A,B,C>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeC==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final C argumentAt2(){
		return QVariant.convert(argumentAt(2), typeC);
	}
}
