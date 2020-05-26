package io.qt.dbus;

import io.qt.core.QVariant;

public class QDBusPendingReply7<A,B,C,D,E,F,G> extends QDBusPendingReply6<A,B,C,D,E,F> {

	private final Class<G> typeG;
	
	public QDBusPendingReply7() {
		typeG = null;
	}

	public QDBusPendingReply7(QDBusMessage message, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG) {
		super(message, typeA, typeB, typeC, typeD, typeE, typeF);
		this.typeG = typeG;
	}

	public QDBusPendingReply7(QDBusPendingCall call, Class<A> typeA, Class<B> typeB, Class<C> typeC, Class<D> typeD, Class<E> typeE, Class<F> typeF, Class<G> typeG) {
		super(call, typeA, typeB, typeC, typeD, typeE, typeF);
		this.typeG = typeG;
	}

	public QDBusPendingReply7(QDBusPendingReply7<A,B,C,D,E,F,G> other) {
		super(other);
		this.typeG = other.typeG;
	}

	@Override
	public QDBusPendingReply7<A,B,C,D,E,F,G> clone() {
		return new QDBusPendingReply7<>(this);
	}

	@Override
	boolean isInvalid() {
		return super.isInvalid() || typeG==null;
	}

	@Override
	int numberOfArguments() {
		return super.numberOfArguments()+1;
	}

	@io.qt.QtUninvokable
	public final G argumentAt6(){
		return QVariant.convert(argumentAt(6), typeG);
	}
}
