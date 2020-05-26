package io.qt.dbus;

import io.qt.core.QVariant;

public class QDBusPendingReply6<A,B,C,D,E,F> extends QDBusPendingReply5<A,B,C,D,E> {

	private final Class<F> typeF;
	
	public QDBusPendingReply6() {
		typeF = null;
	}

	public QDBusPendingReply6(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF) {
		super(message, typeA, typeB, typeC, typeD, typeE);
		this.typeF = typeF;
	}

	public QDBusPendingReply6(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF) {
		super(call, typeA, typeB, typeC, typeD, typeE);
		this.typeF = typeF;
	}

	public QDBusPendingReply6(QDBusPendingReply6<A,B,C,D,E,F> other) {
		super(other);
		this.typeF = other.typeF;
	}

	@Override
	public QDBusPendingReply6<A,B,C,D,E,F> clone() {
		return new QDBusPendingReply6<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeF==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final F argumentAt5(){
		return QVariant.convert(argumentAt(5), typeF);
	}
}
