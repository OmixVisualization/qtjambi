/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.util.Collection;
import java.util.NoSuchElementException;
import java.util.Objects;

import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;
import io.qt.NativeAccess;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qvector.html">QVector</a></p>
 */
public class QVector<T> extends io.qt.internal.AbstractList<T> implements Cloneable
{

    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    @NativeAccess
    protected QVector(QPrivateConstructor p) {
        super(p);
    }
    
    public QVector(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}
    
    public QVector(Class<T> elementType) {
		super(null);
		QMetaType metaType = QMetaType.fromType(elementType);
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QVector.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QVector.");
		initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
	public QVector(QMetaType metaType) {
		super(null);
		if(metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QVector.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QVector.");
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
	}
    
    public QVector(Collection<T> other) {
		super(null);
		QMetaType metaType = QList.findElementMetaType(Objects.requireNonNull(other));
		if(metaType==null || metaType.id()==0)
			throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QVector.");
		if(metaType.id()==QMetaType.Type.Void.value())
			throw new IllegalArgumentException("void cannot be type of QVector.");
		initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    public static QVector<Object> createVariantVector(){
        return new QVector<>(new QMetaType(QMetaType.Type.QVariant));
    }

    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    @Override
    public QVector<T> clone(){
        return new QVector<>(this);
    }
    
    @QtUninvokable
    private static native void onDispose(long listAccess);

    @QtUninvokable
    public final void append(java.util.Collection<T> t)    {
        appendVector(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    }
    @QtUninvokable
    private static native <T> void appendVector(long __this__nativeId, java.util.Collection<T> t);

    @QtUninvokable
    public final void append(T t)    {
        try {
        	final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
        	int size = size(nativeId);
            insert(nativeId, size, 1, t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }

    @QtUninvokable
    public final T at(int i)    {
        if (i >= size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        return at(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> T at(long __this__nativeId, int i);

    @QtUninvokable
    protected final QSequentialIterator<T> begin()    {
        return begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialIterator<T> begin(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialIterator<T> end()    {
        return end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialIterator<T> end(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialConstIterator<T> constBegin()    {
        return constBegin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constBegin(long __this__nativeId);

    @QtUninvokable
    protected final QSequentialConstIterator<T> constEnd()    {
        return constEnd(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSequentialConstIterator<T> constEnd(long __this__nativeId);

    @QtUninvokable
    public final int capacity()    {
        return capacity(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int capacity(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> void clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(Object t)    {
        try{
        	return contains(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return false;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return false;
    	}
    }
    @QtUninvokable
    private static native <T> boolean contains(long __this__nativeId, T t);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(T t)    {
        try{
        	return count(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> int count(long __this__nativeId, T t);

    @QtUninvokable
    public final boolean endsWith(T t)    {
        try{
        	return endsWith(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> boolean endsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final void fill(T t) {
        fill(t, (int)-1);
    }
    @QtUninvokable
    public final void fill(T t, int size)    {
    	try {
    		fill(QtJambi_LibraryUtilities.internal.nativeId(this), t, size);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> void fill(long __this__nativeId, T t, int size);

    @QtUninvokable
    public final T first()    {
        return at(0);
    }

    @QtUninvokable
    public final int indexOf(T t, int from)    {
    	try {
    		return indexOf(QtJambi_LibraryUtilities.internal.nativeId(this), t, from);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return -1;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return -1;
    	}
    }
    @QtUninvokable
    private static native <T> int indexOf(long __this__nativeId, T t, int from);

    @QtUninvokable
    public final void insert(int i, T t)    {
        insert(i, 1, t);
    }

    @QtUninvokable
    public final void insert(int i, int n, T t)    {
        if (i > size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        try {
        	insert(QtJambi_LibraryUtilities.internal.nativeId(this), i, n, t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> void insert(long __this__nativeId, int i, int n, T t);

    @QtUninvokable
    public final boolean isEmpty()    {
        return size()==0;
    }

    @QtUninvokable
    public final T last()    {
        return at(size()-1);
    }

    @SuppressWarnings("unchecked")
	@QtUninvokable
    @Override
    public final int lastIndexOf(Object t) {
    	try {
    		return lastIndexOf((T)t, (int)-1);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return -1;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return -1;
    	}
    }
    @QtUninvokable
    public final int lastIndexOf(T t, int from)    {
    	try {
    		return lastIndexOf(QtJambi_LibraryUtilities.internal.nativeId(this), t, from);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return -1;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return -1;
    	}
    }
    @QtUninvokable
    private static native <T> int lastIndexOf(long __this__nativeId, T t, int from);

    @QtUninvokable
    public final int length()    {
        return size();
    }

    @QtUninvokable
    public final QVector<T> mid(int pos) {
        return mid(pos, (int)-1);
    }
    @QtUninvokable
    public final QVector<T> mid(int pos, int length)    {
        if (pos >= size() || pos < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), pos));
        }
        if (length>=0 && pos+length > size()) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s from %2$s to %3$s", size(), pos, pos+length));
        }
        return mid(QtJambi_LibraryUtilities.internal.nativeId(this), pos, length);
    }
    @QtUninvokable
    private static native <T> QVector<T> mid(long __this__nativeId, int pos, int length);

    @QtUninvokable
    public final void move(int from, int to)    {
        if (from >= size() || from < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), from));
        }
        if (to >= size() || to < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), to));
        }
        move(QtJambi_LibraryUtilities.internal.nativeId(this), from, to);
    }
    @QtUninvokable
    private static native <T> void move(long __this__nativeId, int from, int to);

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        return operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), l);
    }
    @QtUninvokable
    private static native <T> boolean operator_equal(long __this__nativeId, java.util.Collection<T> l);

    @QtUninvokable
    public final void prepend(T t)    {
    	try {
        	insert(QtJambi_LibraryUtilities.internal.nativeId(this), 0, 1, t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }

    @QtUninvokable
    public final void remove(int i, int n)    {
        if (i+n > size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i+n));
        }
        remove(QtJambi_LibraryUtilities.internal.nativeId(this), i, n);
    }
    @QtUninvokable
    private static native void remove(long __this__nativeId, int i, int n);

    @QtUninvokable
    public final int removeAll(T t)    {
        try {
        	return removeAll(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(IllegalArgumentException e) {
    		return 0;
    	}catch(RuntimeException e) {
    		RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
    		if(e1==e)
    			throw e;
    		return 0;
    	}
    }
    @QtUninvokable
    private static native <T> int removeAll(long __this__nativeId, T t);

    @QtUninvokable
    public final void removeAt(int i)    {
    	final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
    	int size = size(nativeId);
        if (i >= size || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size, i));
        }
        remove(QtJambi_LibraryUtilities.internal.nativeId(this), i, 1);
    }

    @QtUninvokable
    public final T removeFirst()    {
        if(size()==0)
            throw new NoSuchElementException();
        T e = at(0);
        removeAt(0);
        return e;
    }

    @QtUninvokable
    public final T removeLast()    {
        if(size()==0)
            throw new NoSuchElementException();
        T e = at(size()-1);
        removeAt(size()-1);
        return e;
    }

    @QtUninvokable
    public final boolean removeOne(T t)    {
        try {
        	final long nativeId = QtJambi_LibraryUtilities.internal.nativeId(this);
        	int idx = indexOf(nativeId, t, 0);
        	if(idx>=0) {
        		remove(nativeId, idx, 1);
        		return true;
        	}
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(IllegalArgumentException e) {
        }catch(RuntimeException e) {
            RuntimeException e1 = QSet.handleException(e, elementMetaType(), t);
            if(e1==e)
                throw e;
        }
        return false;
    }

    @QtUninvokable
    public final void replace(int i, T t)    {
        if (i >= size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        try {
        	replace(QtJambi_LibraryUtilities.internal.nativeId(this), i, t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
    	}
    }
    @QtUninvokable
    private static native <T> void replace(long __this__nativeId, int i, T t);

    @QtUninvokable
    public final void reserve(int size)    {
        reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private static native <T> void reserve(long __this__nativeId, int size);

    @QtUninvokable
    public final void resize(int size)    {
        resize(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private static native void resize(long __this__nativeId, int size);

    @QtUninvokable
    public final void squeeze()    {
        squeeze(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native void squeeze(long __this__nativeId);

    @QtUninvokable
    public final int size()    {
        return size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int size(long __this__nativeId);

    @QtUninvokable
    public final boolean startsWith(T t)    {
        try{
        	return startsWith(QtJambi_LibraryUtilities.internal.nativeId(this), t);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> boolean startsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final T takeAt(int i)    {
    	T result = at(i);
    	removeAt(i);
        return result;
    }

    @QtUninvokable
    public final T takeFirst()    {
        return takeAt(0);
    }
    
    @QtUninvokable
    public final T takeLast()    {
        return takeAt(size()-1);
    }

    @QtUninvokable
    public final QSet<T> toSet()    {
		QSet<T> set = new QSet<>(elementMetaType());
		set.unite(this);
        return set;
    }

    @QtUninvokable
    public final T value(int i)    {
        return value(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> T value(long __this__nativeId, int i);

    @QtUninvokable
    public final T value(int i, T defaultValue)    {
    	try {
    		return valueDefault(QtJambi_LibraryUtilities.internal.nativeId(this), i, defaultValue);
    	}catch(QNoNativeResourcesException e) {
    		throw e;
    	}catch(RuntimeException e) {
    		throw QSet.handleException(e, elementMetaType(), defaultValue);
    	}
    }
    @QtUninvokable
    private static native <T> T valueDefault(long __this__nativeId, int i, T defaultValue);

    @SuppressWarnings({ "rawtypes", "unchecked" })
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Collection && QList.checkContainerType(this.elementMetaType(), (java.util.Collection<?>) other)) {
            return operator_equal((java.util.Collection) other);
        }
        return false;
    }


    @Override
    @QtUninvokable
    public int hashCode() { 
    	try {
    		return hashCode(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoNativeResourcesException e) {
			return 0;
		}
    }
    @QtUninvokable
    private static native int hashCode(long __this__nativeId);
    
    @Override
    @QtUninvokable
    public String toString() {
    	try {
			return toString(QtJambi_LibraryUtilities.internal.nativeId(this));
		} catch (QNoNativeResourcesException e) {
			return "null";
		}
    }
    @QtUninvokable
    private static native String toString(long __this__nativeId);
    
    @Override
    @QtUninvokable
    public boolean add(T e){
        append(e);
        return true;
    }

    @Override
    @QtUninvokable
    public void add(int index, T e){
            insert(index, e);
    }

    @Override
    @QtUninvokable
    public T set(int index, T e){
        if(e!=null){
            T el = get(index);
            replace(index, e);
            return el;
        } else return null;
    }
    
    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public boolean remove(Object e){
        return removeOne((T)e);
    }

    @SuppressWarnings("unchecked")
	@Override
    @QtUninvokable
    public int indexOf(Object e){
        return indexOf((T)e, 0);
    }

    @Override
    @QtUninvokable
    public T get(int index) {
        return at(index);
    }
    
    @Override
    @QtUninvokable
    protected final void removeAtIndex(int cursor) {
        removeAt(cursor);
    }
    
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void writeTo(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.QDataStream stream){
        readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    QMetaType elementMetaType() {
    	return elementMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private static native QMetaType elementMetaType(long containerAccess);
    
    /**
     * Returns a QVector containing given elements.
     *
     * @param <E> the {@code QVector}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QVector} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     */
    @SafeVarargs
    public static <T> QVector<T> of(T element0, T...elements) {
        QMetaType metaType = QList.findElementMetaType(element0, elements);
        if(metaType==null || metaType.id()==0)
            throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QVector.");
        if(metaType.id()==QMetaType.Type.Void.value())
            throw new IllegalArgumentException("void cannot be type of QVector.");
        QVector<T> result = new QVector<>(metaType);
        result.add(element0);
        for (T t : elements) {
            result.add(t);
        }
        return result;
    }
}
