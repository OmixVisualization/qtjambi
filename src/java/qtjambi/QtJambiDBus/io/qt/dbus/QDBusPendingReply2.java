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
