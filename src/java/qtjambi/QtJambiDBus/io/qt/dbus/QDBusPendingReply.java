package io.qt.dbus;

import java.util.List;
import java.util.Map;
import java.util.NavigableMap;
import java.util.Objects;

import io.qt.core.QVariant;

public class QDBusPendingReply<A> extends io.qt.dbus.QDBusPendingCall.Impl implements java.lang.Cloneable {
	
	static {
        QtJambi_LibraryInitializer.init();
    }
	
	private final Class<A> typeA;
	private final QVariant.Type variantTypeA;
	
	public static QDBusPendingReply<Object> newInstance(QDBusMessage message){
		return new QDBusPendingReply<>(message, Object.class);
	}
	
	public static QDBusPendingReply<Object> newInstance(QDBusPendingCall call){
		return new QDBusPendingReply<>(call, Object.class);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<?>> newListInstance(QDBusMessage message){
		return new QDBusPendingReply(message, List.class, QVariant.Type.List);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<?>> newListInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, List.class, QVariant.Type.List);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<String>> newStringListInstance(QDBusMessage message){
		return new QDBusPendingReply(message, List.class, QVariant.Type.StringList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<List<String>> newStringListInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, List.class, QVariant.Type.StringList);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<NavigableMap<String,?>> newMapInstance(QDBusMessage message){
		return new QDBusPendingReply(message, NavigableMap.class, QVariant.Type.Map);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<NavigableMap<String,?>> newMapInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, NavigableMap.class, QVariant.Type.Map);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<Map<String,?>> newHashInstance(QDBusMessage message){
		return new QDBusPendingReply(message, Map.class, QVariant.Type.Hash);
	}
	
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static QDBusPendingReply<Map<String,?>> newHashInstance(QDBusPendingCall call){
		return new QDBusPendingReply(call, Map.class, QVariant.Type.Hash);
	}
	
	public QDBusPendingReply() {
        super(io.qt.dbus.QDBusPendingCall.fromError(new QDBusError()));
		typeA = null;
		variantTypeA = null;
	}

	private QDBusPendingReply(QDBusMessage message, Class<A> typeA, QVariant.Type variantTypeA) {
		super(io.qt.dbus.QDBusPendingCall.fromCompletedCall(message));
		this.typeA = Objects.requireNonNull(typeA);
		this.variantTypeA = variantTypeA;
	}

	private QDBusPendingReply(QDBusPendingCall call, Class<A> typeA, QVariant.Type variantTypeA) {
		super(call);
		this.typeA = Objects.requireNonNull(typeA);
		this.variantTypeA = variantTypeA;
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
			return QVariant.convert(QVariant.convert(argumentAt(0), variantTypeA), typeA);
		}
	}
	
	@io.qt.QtUninvokable
	public final A value(){
		return argumentAt0();
	}
}
