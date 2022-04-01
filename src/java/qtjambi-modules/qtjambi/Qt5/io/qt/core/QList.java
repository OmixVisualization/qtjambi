/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
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
import java.util.Objects;

import io.qt.QNoNativeResourcesException;
import io.qt.QtUninvokable;
import io.qt.NativeAccess;

/**
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qlist.html">QList</a></p>
 */
public class QList<T> extends io.qt.internal.QtJambiListObject<T> implements Cloneable
{
    static {
        QtJambi_LibraryUtilities.initialize();
    }
    
    @NativeAccess
    protected QList(QPrivateConstructor p) {
        super(p);
    }
    
    public QList(QMetaType.Type metaType) {
		this(new QMetaType(metaType));
	}
    
    public QList(Class<T> elementType) {
        super(null);
        QMetaType metaType = QMetaType.fromType(elementType);
        if(metaType.id()==0)
            throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QList.");
        if(metaType.id()==QMetaType.Type.Void.value())
            throw new IllegalArgumentException("void cannot be type of QList.");
        initialize(elementType, QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
    }
    
    public QList(QMetaType metaType) {
        super(null);
        if(metaType.id()==0)
            throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QList.");
        if(metaType.id()==QMetaType.Type.Void.value())
            throw new IllegalArgumentException("void cannot be type of QList.");
        initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), null);
    }
    
    public QList(Collection<T> other) {
        super(null);
        QMetaType metaType = findElementMetaType(other);
        if(metaType==null || metaType.id()==0)
            throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QList.");
        if(metaType.id()==QMetaType.Type.Void.value())
            throw new IllegalArgumentException("void cannot be type of QList.");
        initialize(metaType.javaType(), QtJambi_LibraryUtilities.internal.nativeId(metaType), other);
    }
    
    public static QList<Object> createVariantList(){
        return new QList<>(new QMetaType(QMetaType.Type.QVariant));
    }

    @QtUninvokable
    private native void initialize(Class<?> elementType, long elementMetaType, Collection<T> other);
    
    @Override
    public QList<T> clone(){
        return new QList<>(this);
    }
    
    @QtUninvokable
    public final void append(java.util.Collection<T> t) {
        __qt_QList_appendList(QtJambi_LibraryUtilities.internal.nativeId(this), Objects.requireNonNull(t));
    }
    @QtUninvokable
    private static native <T> void __qt_QList_appendList(long __this__nativeId, java.util.Collection<T> t);

    @QtUninvokable
    public final void append(T t)    {
        try {
            __qt_QList_append(QtJambi_LibraryUtilities.internal.nativeId(this), t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> void __qt_QList_append(long __this__nativeId, T t);

    @QtUninvokable
    public final T at(int i)    {
        if (i >= size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        return __qt_QList_at(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> T __qt_QList_at(long __this__nativeId, int i);

    @QtUninvokable
    protected final QIterator<T> begin()    {
        return __qt_QList_begin(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QIterator<T> __qt_QList_begin(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        __qt_QList_clear(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> void __qt_QList_clear(long __this__nativeId);

    @QtUninvokable
    public final boolean contains(Object t)    {
        try {
            return __qt_QList_contains(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> boolean __qt_QList_contains(long __this__nativeId, T t);

    @QtUninvokable
    public final int count()    {
        return size();
    }

    @QtUninvokable
    public final int count(T t)    {
        try{
            return __qt_QList_count(QtJambi_LibraryUtilities.internal.nativeId(this), t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> int __qt_QList_count(long __this__nativeId, T t);

    @QtUninvokable
    protected final QIterator<T> end()    {
        return __qt_QList_end(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QIterator<T> __qt_QList_end(long __this__nativeId);

    @QtUninvokable
    public final boolean endsWith(T t)    {
        try{
            return __qt_QList_endsWith(QtJambi_LibraryUtilities.internal.nativeId(this), t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> boolean __qt_QList_endsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final T first()    {
        return at(0);
    }
    
    @QtUninvokable
    public final int indexOf(T t, int from)    {
        try {
            return __qt_QList_indexOf(QtJambi_LibraryUtilities.internal.nativeId(this), t, from);
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
    private static native <T> int __qt_QList_indexOf(long __this__nativeId, T t, int from);

    @QtUninvokable
    public final void insert(int i, T t)    {
        if (i > size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        try {
            __qt_QList_insert(QtJambi_LibraryUtilities.internal.nativeId(this), i, t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> void __qt_QList_insert(long __this__nativeId, int i, T t);

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
        return lastIndexOf((T)t, (int)-1);
    }
    @QtUninvokable
    public final int lastIndexOf(T t, int from)    {
        try {
            return __qt_QList_lastIndexOf(QtJambi_LibraryUtilities.internal.nativeId(this), t, from);
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
    private static native <T> int __qt_QList_lastIndexOf(long __this__nativeId, T t, int from);

    @QtUninvokable
    public final int length()    {
        return size();
    }

    @QtUninvokable
    public final QList<T> mid(int pos) {
        return mid(pos, (int)-1);
    }
    @QtUninvokable
    public final QList<T> mid(int pos, int length)    {
        if (pos >= size() || pos < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), pos));
        }
        if (length>=0 && pos+length > size()) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s from %2$s to %3$s", size(), pos, pos+length));
        }
        return __qt_QList_mid(QtJambi_LibraryUtilities.internal.nativeId(this), pos, length);
    }
    @QtUninvokable
    private static native <T> QList<T> __qt_QList_mid(long __this__nativeId, int pos, int length);

    @QtUninvokable
    public final void move(int from, int to)    {
        if (from >= size() || from < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), from));
        }
        if (to >= size() || to < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), to));
        }
        __qt_QList_move(QtJambi_LibraryUtilities.internal.nativeId(this), from, to);
    }
    @QtUninvokable
    private static native <T> void __qt_QList_move(long __this__nativeId, int from, int to);

    @QtUninvokable
    private final boolean operator_equal(java.util.Collection<T> l)    {
        return __qt_QList_operator_equal(QtJambi_LibraryUtilities.internal.nativeId(this), l);
    }
    @QtUninvokable
    private static native <T> boolean __qt_QList_operator_equal(long __this__nativeId, java.util.Collection<T> l);

    @QtUninvokable
    public final void prepend(T t)    {
        try {
            __qt_QList_prepend(QtJambi_LibraryUtilities.internal.nativeId(this), t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> void __qt_QList_prepend(long __this__nativeId, T t);

    @QtUninvokable
    public final int removeAll(T t)    {
        try {
            return __qt_QList_removeAll(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> int __qt_QList_removeAll(long __this__nativeId, T t);

    @QtUninvokable
    public final void removeAt(int i)    {
        if (i >= size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        __qt_QList_removeAt(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> void __qt_QList_removeAt(long __this__nativeId, int i);

    @QtUninvokable
    public final void removeFirst()    {
        removeAt(0);
    }

    @QtUninvokable
    public final void removeLast()    {
        removeAt(size()-1);
    }

    @QtUninvokable
    public final boolean removeOne(T t)    {
        try {
            return __qt_QList_removeOne(QtJambi_LibraryUtilities.internal.nativeId(this), t);
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
    private static native <T> boolean __qt_QList_removeOne(long __this__nativeId, T t);

    @QtUninvokable
    public final void replace(int i, T t)    {
        if (i >= size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        try {
            __qt_QList_replace(QtJambi_LibraryUtilities.internal.nativeId(this), i, t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> void __qt_QList_replace(long __this__nativeId, int i, T t);

    @QtUninvokable
    public final void reserve(int size)    {
        __qt_QList_reserve(QtJambi_LibraryUtilities.internal.nativeId(this), size);
    }
    @QtUninvokable
    private static native <T> void __qt_QList_reserve(long __this__nativeId, int size);

    @QtUninvokable
    public final int size()    {
        return __qt_QList_size(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> int __qt_QList_size(long __this__nativeId);

    @QtUninvokable
    public final boolean startsWith(T t)    {
        try{
            return __qt_QList_startsWith(QtJambi_LibraryUtilities.internal.nativeId(this), t);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), t);
        }
    }
    @QtUninvokable
    private static native <T> boolean __qt_QList_startsWith(long __this__nativeId, T t);

    @QtUninvokable
    public final T takeAt(int i)    {
        if (i >= size() || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size(), i));
        }
        return __qt_QList_takeAt(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> T __qt_QList_takeAt(long __this__nativeId, int i);

    @QtUninvokable
    public final T takeFirst()    {
        return takeAt(0);
    }
    
    @QtUninvokable
    public final T takeLast()    {
        return takeAt(size()-1);
    }

    @QtUninvokable
    public final T value(int i)    {
        return __qt_QList_value(QtJambi_LibraryUtilities.internal.nativeId(this), i);
    }
    @QtUninvokable
    private static native <T> T __qt_QList_value(long __this__nativeId, int i);

    @QtUninvokable
    public final T value(int i, T defaultValue)    {
        try {
            return __qt_QList_valueDefault(QtJambi_LibraryUtilities.internal.nativeId(this), i, defaultValue);
        }catch(QNoNativeResourcesException e) {
            throw e;
        }catch(RuntimeException e) {
            throw QSet.handleException(e, elementMetaType(), defaultValue);
        }
    }
    @QtUninvokable
    private static native <T> T __qt_QList_valueDefault(long __this__nativeId, int i, T defaultValue);

    @SuppressWarnings({ "rawtypes", "unchecked" })
    @Override
    @QtUninvokable
    public boolean equals(Object other) {
        if (other instanceof java.util.Collection && checkContainerType(this.elementMetaType(), (java.util.Collection<?>) other)) {
            return operator_equal((java.util.Collection) other);
        }
        return false;
    }

    @Override
    @QtUninvokable
    public int hashCode() {
        int hashCode = getClass().hashCode();
        for (T e : this)
            hashCode = 31*hashCode + (e==null ? 0 : e.hashCode());
        return hashCode;
    }
    
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
        return indexOf((T)e, (int)0);
    }

    @Override
    @QtUninvokable
    public T get(int index) {
        return at(index);
    }
    
    @QtUninvokable
    public final void swapItemsAt(int i, int j)    {
        int size = size();
        if (i >= size || i < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size, i));
        }
        if (j >= size || j < 0) {
            throw new IndexOutOfBoundsException(String.format("Accessing container of size %1$s at %2$s", size, j));
        }
        __qt_QList_swapItemsAt(QtJambi_LibraryUtilities.internal.nativeId(this), i, j);
    }
    
    @QtUninvokable
    private static native <T> void __qt_QList_swapItemsAt(long __this__nativeId, int i, int j);
    
    @Override
    @QtUninvokable
    protected final void removeAtIndex(int cursor) {
        removeAt(cursor);
    }
    
    @QtUninvokable
    public final QSet<T> toSet()    {
        return __qt_QList_toSet(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @QtUninvokable
    private static native <T> QSet<T> __qt_QList_toSet(long __this__nativeId);
    
    @io.qt.QtUninvokable
    public void writeTo(io.qt.core.QDataStream stream){
        __qt_QList_writeTo(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QList_writeTo(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    public void readFrom(io.qt.core.QDataStream stream){
        __qt_QList_readFrom(QtJambi_LibraryUtilities.internal.nativeId(this), QtJambi_LibraryUtilities.internal.nativeId(stream));
    }
    
    @io.qt.QtUninvokable
    private native void __qt_QList_readFrom(long __this__nativeId, long stream);
    
    @io.qt.QtUninvokable
    QMetaType elementMetaType() {
        return elementMetaType(QtJambi_LibraryUtilities.internal.nativeId(this));
    }
    @io.qt.QtUninvokable
    private static native QMetaType elementMetaType(long containerAccess);
    
    /**
     * Returns a QList containing given elements.
     *
     * @param <E> the {@code QList}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QList} containing the specified element
     * @throws NullPointerException if elements are {@code null}
     *
     */
    @SuppressWarnings({ "unchecked" })
    @SafeVarargs
    public static <T> QList<T> of(T element0, T...elements) {
        QMetaType metaType = findElementMetaType(element0, elements);
        if(metaType==null || metaType.id()==0)
            throw new IllegalArgumentException("QMetaType::UnknownType cannot be type of QList.");
        if(metaType.id()==QMetaType.Type.Void.value())
            throw new IllegalArgumentException("void cannot be type of QList.");
        QList<T> result;
        if(metaType.id()==QMetaType.Type.QString.value())
            result = (QList<T>)(QList<?>)new QStringList();
        else
            result = new QList<>(metaType);
        result.reserve(elements.length+1);
        result.append(element0);
        for (T t : elements) {
            result.append(t);
        }
        return result;
    }
    
    /**
     * Returns a QStringList containing given elements.
     *
     * @param <E> the {@code QStringList}'s element type
     * @param element0 the first element
     * @param elements subsequent elements
     * @return a {@code QStringList} containing the specified element
     */
    @SafeVarargs
    public static QStringList of(String element0, String...elements) {
        QStringList result = new QStringList();
        result.reserve(elements.length+1);
        result.append(element0);
        for (String element : elements) {
            result.append(element);
        }
        return result;
    }

    static boolean checkContainerType(QMetaType elementType, java.util.Collection<?> container) {
        if (container instanceof QList) {
            return elementType.equals(((QList<?>) container).elementMetaType());
        } else if (container instanceof QSet) {
            return elementType.equals(((QSet<?>) container).elementMetaType());
        } else if (container instanceof QVector) {
            return elementType.equals(((QVector<?>) container).elementMetaType());
        } else if (container instanceof QLinkedList) {
            return elementType.equals(((QLinkedList<?>) container).elementMetaType());
        } else {
            Class<?> type = elementType.javaType();
            if (type.isPrimitive()) {
                if (type == int.class)
                    type = Integer.class;
                if (type == short.class)
                    type = Short.class;
                if (type == byte.class)
                    type = Byte.class;
                if (type == long.class)
                    type = Long.class;
                if (type == float.class)
                    type = Float.class;
                if (type == double.class)
                    type = Double.class;
                if (type == boolean.class)
                    type = Boolean.class;
                if (type == char.class)
                    type = Character.class;
            }
            for (Object entry : container) {
                if (entry != null && !type.isInstance(entry))
                    return false;
            }
        }
        return true;
    }
    
    @SafeVarargs
    static QMetaType findElementMetaType(Object element0, Object... elements){
        QMetaType result = getMetaType(element0);
        Class<?> type = element0==null ? null : element0.getClass();
        for(Object e : elements) {
            if(result!=null) {
                QMetaType _result = getMetaType(e);
                if(!result.isValid() || result.id()==QMetaType.Type.Nullptr.value())
                    result = _result;
                if(!_result.equals(result)) {
                    result = null;
                }
            }
            if(e!=null) {
                if(type==null) {
                    type = e.getClass();
                }else {
                    Class<?> type2 = e.getClass();
                    if(type2!=type) {
                        if(type2.isAssignableFrom(type)) {
                            type = type2;
                        }else if(!type.isAssignableFrom(type2)) {
                            do{
                                type = type.getSuperclass();
                            }while(!type.isAssignableFrom(type2));
                        }
                    }
                }
            }
        }
        if(result==null)
            result = QMetaType.fromType(type);
        return result;
    }
    
    static QMetaType findElementMetaType(Collection<?> elements){
        if(elements.getClass()==QList.class) {
            return ((QList<?>)elements).elementMetaType();
        }else if(elements.getClass()==QQueue.class) {
            return ((QQueue<?>)elements).elementMetaType();
        }else if(elements.getClass()==QStack.class) {
            return ((QStack<?>)elements).elementMetaType();
        }else if(elements.getClass()==QSet.class) {
            return ((QSet<?>)elements).elementMetaType();
        }else {
            QMetaType result = new QMetaType();
            Class<?> type = null;
            for(Object e : elements) {
                if(result!=null) {
                    QMetaType _result = getMetaType(e);
                    if(!result.isValid() || result.id()==QMetaType.Type.Nullptr.value())
                        result = _result;
                    if(!_result.equals(result)) {
                        result = null;
                    }
                }
                if(e!=null) {
                    if(type==null) {
                        type = e.getClass();
                    }else {
                        Class<?> type2 = e.getClass();
                        if(type2!=type) {
                            if(type2.isAssignableFrom(type)) {
                                type = type2;
                            }else if(!type.isAssignableFrom(type2)) {
                                do{
                                    type = type.getSuperclass();
                                }while(!type.isAssignableFrom(type2));
                            }
                        }
                    }
                }
            }
            if(result==null)
                result = QMetaType.fromType(type);
            return result;
        }
    }
    
    static QMetaType getMetaType(Object obj){
        if(obj==null)
            return new QMetaType(QMetaType.Type.Nullptr);
        if(obj.getClass()==QList.class) {
            return QMetaType.fromType(QList.class, ((QList<?>)obj).elementMetaType());
        }else if(obj.getClass()==QQueue.class) {
            return QMetaType.fromType(QQueue.class, ((QQueue<?>)obj).elementMetaType());
        }else if(obj.getClass()==QStack.class) {
            return QMetaType.fromType(QStack.class, ((QStack<?>)obj).elementMetaType());
        }else if(obj.getClass()==QSet.class) {
            return QMetaType.fromType(QSet.class, ((QSet<?>)obj).elementMetaType());
        }else if(obj.getClass()==QMultiMap.class) {
            return QMetaType.fromType(QMultiMap.class, ((QMultiMap<?,?>)obj).keyMetaType(), ((QMultiMap<?,?>)obj).valueMetaType());
        }else if(obj.getClass()==QMap.class) {
            return QMetaType.fromType(QMap.class, ((QMap<?,?>)obj).keyMetaType(), ((QMap<?,?>)obj).valueMetaType());
        }else if(obj.getClass()==QMultiHash.class) {
            return QMetaType.fromType(QMultiHash.class, ((QMultiHash<?,?>)obj).keyMetaType(), ((QMultiHash<?,?>)obj).valueMetaType());
        }else if(obj.getClass()==QHash.class) {
            return QMetaType.fromType(QHash.class, ((QHash<?,?>)obj).keyMetaType(), ((QHash<?,?>)obj).valueMetaType());
        }else if(obj.getClass()==QPair.class) {
            return QMetaType.fromType(QPair.class, getMetaType(((QPair<?,?>)obj).first), getMetaType(((QPair<?,?>)obj).second));
        }else if(obj instanceof java.util.List) {
            java.util.List<?> list = (java.util.List<?>)obj;
            QMetaType elementMetaType = findElementMetaType(list);
            if(elementMetaType.isValid()) {
                return QMetaType.fromType(QList.class, elementMetaType);
            }
        }else if(obj instanceof java.util.Set) {
            java.util.Set<?> list = (java.util.Set<?>)obj;
            QMetaType elementMetaType = findElementMetaType(list);
            if(elementMetaType.isValid()) {
                return QMetaType.fromType(QSet.class, elementMetaType);
            }
        }else if(obj instanceof java.util.Map) {
            java.util.Map<?,?> map = (java.util.Map<?,?>)obj;
            QPair<QMetaType, QMetaType> metaTypes = QMap.findMapMetaType(map);
            if(metaTypes.first.isValid() && metaTypes.second.isValid()) {
                if(obj instanceof java.util.NavigableMap)
                    return QMetaType.fromType(QMap.class, metaTypes.first, metaTypes.second);
                else
                    return QMetaType.fromType(QHash.class, metaTypes.first, metaTypes.second);
            }
        }
        QMetaType metaType = QMetaType.fromType(obj.getClass());
        return metaType;
    }
}
