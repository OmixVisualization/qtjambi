package io.qt.dbus;

import io.qt.core.QVariant;

public class QDBusPendingReply8<A,B,C,D,E,F,G,H> extends QDBusPendingReply7<A,B,C,D,E,F,G> {

	private final Class<H> typeH;
	
	public QDBusPendingReply8() {
		typeH = null;
	}

	public QDBusPendingReply8(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH) {
		super(message, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		this.typeH = typeH;
	}

	public QDBusPendingReply8(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG, Class<H> typeH) {
		super(call, typeA, typeB, typeC, typeD, typeE, typeF, typeG);
		this.typeH = typeH;
	}

	public QDBusPendingReply8(QDBusPendingReply8<A,B,C,D,E,F,G,H> other) {
		super(other);
		this.typeH = other.typeH;
	}

	@Override
	public QDBusPendingReply8<A,B,C,D,E,F,G,H> clone() {
		return new QDBusPendingReply8<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeH==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final H argumentAt7(){
		return QVariant.convert(argumentAt(7), typeH);
	}
}
