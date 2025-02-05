/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt.core;

import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.DoubleBuffer;
import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;
import java.util.function.Supplier;

import io.qt.QtObject;
import io.qt.QtUninvokable;

abstract class AbstractSpan<T> extends AbstractContainer<T> implements java.lang.Iterable<T>{
	
	final SpanData<T> d;
	
	static interface SpanData<T>{
    	default void commit(AbstractSpan<T> span) {}
    	boolean isManagedSpan();
    	default boolean isBuffered() {return false;}
    	default QList<T> toList(AbstractSpan<T> span) {
    		long _this = QtJambi_LibraryUtilities.internal.nativeId(span);
    		QList<T> list = new QList<>(AbstractSpan.elementType(_this), AbstractSpan.size(_this));
    		AbstractSpan.toList(_this, QtJambi_LibraryUtilities.internal.nativeId(list));
        	return list;
    	}
    	default QtObject owner() {return null;}
    }
	private static <T> SpanData<T> asManagedSpanData() {return ()->true;}
	private static <T> SpanData<T> asUnmanagedSpanData() {return ()->false;}
	
	private static interface Owner<T> extends SpanData<T>{
		QtObject owner();
		default boolean isManagedSpan() {return false;}
	}
	private static <T> Owner<T> ofOwner(QtObject owner) {
		return ()->owner;
	}
	
	private static interface BufferData<T> extends SpanData<T>{
		QtObject storage();
		default boolean isManagedSpan() {return true;}
	}
	private static <T> BufferData<T> ofStorage(QtObject owner) {
		return ()->owner;
	}
	
	private static class ArrayCommitter<T> implements SpanData<T>{
		private ArrayCommitter(QList<T> list, T[] array) {
			this.list = list;
			this.array = array;
		}
		ArrayCommitter(Class<? super T> type, T[] array){
			this(QList.ofTyped(type, array), array);
		}
		
		ArrayCommitter(QMetaType metaType, T[] array){
	    	this(QList.ofTyped(metaType, array), array);
		}
		private final QList<T> list;
		final T[] array;
		public boolean isManagedSpan() {return false;}
    	public void commit(AbstractSpan<T> span){
			for (int i = 0, l = Math.min(array.length, list.size()); i < l; i++) {
				array[i] = list.at(i);
			}
		}
    	public QList<T> toList(AbstractSpan<T> span){
    		return list.clone();
    	}
    }
	
	private static interface ListSupplier<T> extends SpanData<T>{
		QList<T> storage();
		default QList<T> toList(AbstractSpan<T> span) {return storage().clone();}
		default boolean isManagedSpan() {return false;}
	}
	private static <T> ListSupplier<T> ofListStorage(QList<T> list) {
		return ()->list;
	}
	
	private static interface BufferedSpanData<T> extends SpanData<T>{
    	void commit(AbstractSpan<T> span);
    	default boolean isManagedSpan() {return true;}
    	default boolean isBuffered() {return true;}
    }
	private static <T> BufferedSpanData<T> newBufferedSpanData(){ return span->AbstractSpan.commit(QtJambi_LibraryUtilities.internal.nativeId(span)); }
	
	AbstractSpan(QPrivateConstructor c, QtObject owner) { 
		this(c, owner!=null ? ofOwner(owner) : asUnmanagedSpanData());
	}
	
    private AbstractSpan(QPrivateConstructor c, SpanData<T> d) { 
		super(c);
		this.d = d;
	}
	
    AbstractSpan(QByteArray byteArray){
		super((QPrivateConstructor)null);
		ByteBuffer buffer = isConstSpan() ? byteArray.data() : QtJambi_LibraryUtilities.internal.mutableData(byteArray);
		this.d = ofStorage(byteArray.clone());
		initializeFromBuffer(this, buffer, 'B', 0, buffer.isReadOnly());
	}
    
    AbstractSpan(ByteBuffer buffer, Supplier<QtObject> ownerSupplier){
		super((QPrivateConstructor)null);
		this.d = ofStorage(ownerSupplier.get());
		initializeFromBuffer(this, buffer, 'B', 0, buffer.isReadOnly());
	}
    
    AbstractSpan(QString string){
		super((QPrivateConstructor)null);
		CharBuffer buffer = isConstSpan() ? string.data() : QtJambi_LibraryUtilities.internal.mutableData(string);
		this.d = ofStorage(string.clone());
		initializeFromBuffer(this, buffer, 'C', 0, buffer.isReadOnly());
	}
    
	AbstractSpan(java.nio.Buffer buffer, char type) {
		super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromBuffer(this, buffer, type, 0, buffer.isReadOnly());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}
	
    AbstractSpan(QList<T> list){
		super((QPrivateConstructor)null);
		int size = list.size();
		AbstractIterator<T> iter = isConstSpan() ? list.constBegin() : list.begin();
		this.d = ofListStorage(list.clone());
		list = ((ListSupplier<T>)d).storage();
    	initializeFromBegin(this, QtJambi_LibraryUtilities.internal.nativeId(list), QtJambi_LibraryUtilities.internal.nativeId(iter), size, QtJambi_LibraryUtilities.internal.nativeId(d.owner()));
		this.__rcContainer = list.__rcContainer;
	}
    
    AbstractSpan(QPrivateConstructor p, QList<T> list){
		super(p);
		int size = list.size();
		AbstractIterator<T> iter = isConstSpan() ? list.constBegin() : list.begin();
		this.d = ofListStorage(list);
    	initializeFromBegin(this, QtJambi_LibraryUtilities.internal.nativeId(list), QtJambi_LibraryUtilities.internal.nativeId(iter), size, QtJambi_LibraryUtilities.internal.nativeId(d.owner()));
		this.__rcContainer = list.__rcContainer;
	}
    
    AbstractSpan(Class<? super T> type, T[] array){
		super((QPrivateConstructor)null);
		ArrayCommitter<T> d = new ArrayCommitter<>(type, array);
		this.d = d;
    	initializeFromBegin(this, QtJambi_LibraryUtilities.internal.nativeId(d.list), QtJambi_LibraryUtilities.internal.nativeId(d.list.begin()), d.list.size(), QtJambi_LibraryUtilities.internal.nativeId(d.owner()));
		this.__rcContainer = d.list.__rcContainer;
	}
	
    AbstractSpan(QMetaType metaType, T[] array){
		super((QPrivateConstructor)null);
		ArrayCommitter<T> d = new ArrayCommitter<>(metaType, array);
		this.d = d;
    	initializeFromBegin(this, QtJambi_LibraryUtilities.internal.nativeId(d.list), QtJambi_LibraryUtilities.internal.nativeId(d.list.begin()), d.list.size(), QtJambi_LibraryUtilities.internal.nativeId(d.owner()));
		this.__rcContainer = d.list.__rcContainer;
	}
    
    AbstractSpan(boolean[] array) {
		super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromBooleanArray(this, array, isConstSpan());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}
    
    AbstractSpan(byte[] array) {
    	super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromByteArray(this, array, isConstSpan());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}
    
    AbstractSpan(short[] array) {
    	super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromShortArray(this, array, isConstSpan());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}
    
    AbstractSpan(int[] array) {
    	super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromIntArray(this, array, isConstSpan());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}

    AbstractSpan(long[] array) {
    	super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromLongArray(this, array, isConstSpan());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}
    
    AbstractSpan(float[] array) {
    	super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromFloatArray(this, array, isConstSpan());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}
    
    AbstractSpan(double[] array) {
    	super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromDoubleArray(this, array, isConstSpan());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}
    
    AbstractSpan(char[] array) {
    	super((QPrivateConstructor)null);
		boolean isBuffered = initializeFromCharArray(this, array, isConstSpan());
		this.d = isBuffered ? newBufferedSpanData() : asManagedSpanData();
	}
    
    AbstractSpan() { 
		this((QPrivateConstructor)null, asUnmanagedSpanData());
		initializeFromBegin(this, 0, 0, 0, 0);
	}
    
    AbstractSpan(AbstractSpan<T> other, int offset, int n) { 
		this((QPrivateConstructor)null, other.d);
		initializeFromClone(this, QtJambi_LibraryUtilities.internal.nativeId(other), QtJambi_LibraryUtilities.internal.nativeId(d.owner()), d.isManagedSpan(), offset, n);
	}
    
    static <B extends java.nio.Buffer> B asBuffer(Class<B> bufferClass, AbstractSpan<?> span) {
    	boolean isConst = isConst(QtJambi_LibraryUtilities.internal.nativeId(span));
    	QtObject owner;
    	if(span.d instanceof BufferData) {
    		owner = ((BufferData<?>)span.d).storage();
    	}else if(span.d instanceof ListSupplier) {
    		owner = ((ListSupplier<?>)span.d).storage();
    	}else if(span.d instanceof ArrayCommitter) {
    		owner = ((ArrayCommitter<?>)span.d).list;
    	}else if(span.d.owner()!=null){
    		owner = span.d.owner();
    	}else {
    		owner = span;
    	}
    	if(owner instanceof QList) {
    		B buffer = QList.asBuffer(bufferClass, (QList<?>)owner);
    		if(isConst && !buffer.isReadOnly()) {
    			if(buffer instanceof ByteBuffer)
    				buffer = bufferClass.cast(((ByteBuffer) buffer).asReadOnlyBuffer());
    			else if(buffer instanceof ShortBuffer)
    				buffer = bufferClass.cast(((ShortBuffer) buffer).asReadOnlyBuffer());
    			else if(buffer instanceof IntBuffer)
    				buffer = bufferClass.cast(((IntBuffer) buffer).asReadOnlyBuffer());
    			else if(buffer instanceof LongBuffer)
    				buffer = bufferClass.cast(((LongBuffer) buffer).asReadOnlyBuffer());
    			else if(buffer instanceof CharBuffer)
    				buffer = bufferClass.cast(((CharBuffer) buffer).asReadOnlyBuffer());
    			else if(buffer instanceof FloatBuffer)
    				buffer = bufferClass.cast(((FloatBuffer) buffer).asReadOnlyBuffer());
    			else if(buffer instanceof DoubleBuffer)
    				buffer = bufferClass.cast(((DoubleBuffer) buffer).asReadOnlyBuffer());
    			else {
	    			try {
						Method asReadOnlyBuffer = bufferClass.getMethod("asReadOnlyBuffer");
						buffer = bufferClass.cast(asReadOnlyBuffer.invoke(buffer));
					} catch (Exception e) {
					}
    			}
    		}
    		return buffer;
    	}else if(owner instanceof QByteArray && bufferClass==ByteBuffer.class) {
    		if(isConst) {
    			return bufferClass.cast(((QByteArray)owner).data());
    		}else {
    			return bufferClass.cast(QtJambi_LibraryUtilities.internal.mutableData((QByteArray)owner));
    		}
    	}else if(owner instanceof QString && bufferClass==CharBuffer.class) {
    		if(isConst) {
    			return bufferClass.cast(((QString)owner).data());
    		}else {
    			return bufferClass.cast(QtJambi_LibraryUtilities.internal.mutableData((QString)owner));
    		}
    	}
    	return asBuffer(bufferClass, QtJambi_LibraryUtilities.internal.nativeId(span), QtJambi_LibraryUtilities.internal.nativeId(owner));
    }
    
    static <A> A asArray(Class<A> arrayClass, AbstractSpan<?> begin) {
    	Class<?> expectedClass = arrayClass.getComponentType();
    	char c;
    	if(expectedClass==long.class)
    		c = 'J';
    	else if(expectedClass==boolean.class)
    		c = 'Z';
    	else
    		c = expectedClass.getSimpleName().toUpperCase().charAt(0);
    	QMetaType valueType = elementType(QtJambi_LibraryUtilities.internal.nativeId(begin));
    	if(valueType.javaType()!=expectedClass)
    		throw new IllegalArgumentException(String.format("Cannot convert QSpan<%1$s> to %2$s[]", valueType.name(), expectedClass.getTypeName()));
    	return arrayClass.cast(asArray(QtJambi_LibraryUtilities.internal.nativeId(begin), c));
    }
    
    native static <A> A asArray(long begin, char c);
    
    native static <B extends java.nio.Buffer> B asBuffer(Class<B> bufferClass, long begin, long owner);
	
    private static native void initializeFromBegin(AbstractSpan<?> span, long list, long begin, long size, long owner);
    
    private static native boolean initializeFromBuffer(AbstractSpan<?> span, java.nio.Buffer buffer, char type, long owner, boolean isConst);
    
    private static native boolean initializeFromBooleanArray(AbstractSpan<?> span, boolean[] array, boolean isConst);
    
    private static native boolean initializeFromByteArray(AbstractSpan<?> span, byte[] array, boolean isConst);
    
    private static native boolean initializeFromShortArray(AbstractSpan<?> span, short[] array, boolean isConst);
    
    private static native boolean initializeFromIntArray(AbstractSpan<?> span, int[] array, boolean isConst);
    
    private static native boolean initializeFromLongArray(AbstractSpan<?> span, long[] array, boolean isConst);
    
    private static native boolean initializeFromFloatArray(AbstractSpan<?> span, float[] array, boolean isConst);
    
    private static native boolean initializeFromDoubleArray(AbstractSpan<?> span, double[] array, boolean isConst);
    
    private static native boolean initializeFromCharArray(AbstractSpan<?> span, char[] array, boolean isConst);
    
    static native boolean isConst(long id);
    
    static native int hashCode(long id);
    
    static native String toString(long id);
    
    static native int size(long id);
    
    static native void toList(long id, long begin);
    
    private final void commit() {
    	d.commit(this);
	}
    
    static native void commit(long id);
    
    private static native void initializeFromClone(AbstractSpan<?> span, long other, long owner, boolean isArrayOrNondirectBuffer, int offset, int n);
	
    /**
     * {@inheritDoc}
     */
	@Override
	protected final QSequentialConstIterator<T> constBegin(){
		return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	final QSequentialIterator<T> begin(){
		if(isConstSpan())
			throw new RuntimeException(String.format("Cannot get non-const iterator for QSpan<const %1%s>", _elementType().name()));
		return begin(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	
	/**
     * {@inheritDoc}
     */
	@Override
	protected final QSequentialConstIterator<T> constEnd(){
		return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	final QSequentialIterator<T> end(){
		if(isConstSpan())
			throw new RuntimeException(String.format("Cannot get non-const iterator for QSpan<const %1%s>", _elementType().name()));
		return end(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
    
    native T get(long nativeId, int index);
    
    native boolean set(long nativeId, int index, T value);

    native QSequentialConstIterator<T> constBegin(long nativeId);
    
    native QSequentialConstIterator<T> constEnd(long nativeId);
    
    native QSequentialIterator<T> begin(long nativeId);
    
    native QSequentialIterator<T> end(long nativeId);
    
    abstract boolean isConstSpan();
    
    @QtUninvokable
	final QMetaType _elementType() {
    	return elementType(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
    
    @QtUninvokable
    static native QMetaType elementType(long __this__nativeId);
    
    /**
     * {@inheritDoc}
     */
    @Override
    public abstract AbstractSpan<T> clone();
}
