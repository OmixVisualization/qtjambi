package io.qt.dbus;

import io.qt.core.QVariant;

public class QDBusPendingReply5<A,B,C,D,E> extends QDBusPendingReply4<A,B,C,D> {

	private final Class<E> typeE;
	
	public QDBusPendingReply5() {
		typeE = null;
	}

	public QDBusPendingReply5(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE) {
		super(message, typeA, typeB, typeC, typeD);
		this.typeE = typeE;
	}

	public QDBusPendingReply5(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE) {
		super(call, typeA, typeB, typeC, typeD);
		this.typeE = typeE;
	}

	public QDBusPendingReply5(QDBusPendingReply5<A,B,C,D,E> other) {
		super(other);
		this.typeE = other.typeE;
	}

	@Override
	public QDBusPendingReply5<A,B,C,D,E> clone() {
		return new QDBusPendingReply5<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeE==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final E argumentAt4(){
		return QVariant.convert(argumentAt(4), typeE);
	}
}
