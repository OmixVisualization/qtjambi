package io.qt.dbus;

import io.qt.core.QVariant;

public class QDBusPendingReply4<A,B,C,D> extends QDBusPendingReply3<A,B,C> {

	private final Class<D> typeD;
	
	public QDBusPendingReply4() {
		typeD = null;
	}

	public QDBusPendingReply4(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD) {
		super(message, typeA, typeB, typeC);
		this.typeD = typeD;
	}

	public QDBusPendingReply4(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD) {
		super(call, typeA, typeB, typeC);
		this.typeD = typeD;
	}

	public QDBusPendingReply4(QDBusPendingReply4<A,B,C,D> other) {
		super(other);
		this.typeD = other.typeD;
	}

	@Override
	public QDBusPendingReply4<A,B,C,D> clone() {
		return new QDBusPendingReply4<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeD==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final D argumentAt3(){
		return QVariant.convert(argumentAt(3), typeD);
	}
}
