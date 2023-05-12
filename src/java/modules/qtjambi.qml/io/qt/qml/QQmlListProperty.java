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

package io.qt.qml;

import java.util.Objects;
import java.util.function.*;

import io.qt.*;
import io.qt.core.*;

/**
 * <p>The QQmlListProperty class allows applications to expose list-like properties of QObject-derived classes to QML.</p>
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qqmllistproperty.html">QQmlListProperty</a></p>
 */
public final class QQmlListProperty<T extends QtObjectInterface> extends QtObject
    implements java.lang.Cloneable
{

    static {
        io.qt.qml.QtJambi_LibraryUtilities.initialize();
    }
    
    @NativeAccess
    private QMetaType elementType = null;
    
    public QQmlListProperty(){
        super((QPrivateConstructor)null);
        initialize_native_plain(this);
    }

    private static native void initialize_native_plain(QQmlListProperty<?> instance);

    /**
     * <p>See <a href="https://doc.qt.io/qt/qqmllistproperty.html#QQmlListProperty-1">QQmlListProperty::QQmlListProperty(QObject*, QList&lt;T *>*)</a></p>
     */
    public QQmlListProperty(QObject o, QList<T> list){
        this(o, list, false);
    }
    
    /**
     * This constructor allows you to create a read-only property from a QList.
     * <p>See <a href="https://doc.qt.io/qt/qqmllistproperty.html#QQmlListProperty-1">QQmlListProperty::QQmlListProperty(QObject*, QList&lt;T *>*)</a></p>
     */
    public QQmlListProperty(QObject o, QList<T> list, boolean readonly){
        super((QPrivateConstructor)null);
        long listNativeId = QtJambi_LibraryUtilities.internal.checkedNativeId(Objects.requireNonNull(list));
        initialize_native_list(this, Objects.requireNonNull(o), list, readonly);
        elementType = getListElementType(listNativeId);
    }
    
    private static native QMetaType getListElementType(long listNativeId);

    private static native void initialize_native_list(QQmlListProperty<?> instance, QObject o, QList<?> list, boolean readonly);
    
    /**
     * Replace the element at position index in the list property with value.
     */
    public interface ReplaceFunction<O,T>{
    	void accept(O object, long index, T t);
    }
    
    /**
     * Return the element at position index in the list property.
     */
    public interface AtFunction<O,T>{
    	T apply(O object, long index);
    }
    
    /**
     * This constructor creates a read-only QQmlListProperty with given access methods. The access functions should operate on the same data source (e.g. list).
     * <p>See <a href="https://doc.qt.io/qt/qqmllistproperty.html#QQmlListProperty-4">QQmlListProperty::QQmlListProperty(QObject *, void *, CountFunction, AtFunction)</a></p>
     */
    public <O extends QObject> QQmlListProperty(Class<T> elementType, 
    		O o, 
    		ToLongFunction<O> countFunction, 
    		AtFunction<O,T> atFunction) {
    	this(elementType, o, null, countFunction, atFunction, null, null, null);
    }
    
    /**
     * This constructor creates a writable QQmlListProperty with given access methods. The access functions should operate on the same data source (e.g. list).
     * <p>See <a href="https://doc.qt.io/qt/qqmllistproperty.html#QQmlListProperty-2">QQmlListProperty::QQmlListProperty(QObject *, void *, AppendFunction, CountFunction, AtFunction, ClearFunction)</a></p>
     */
    public <O extends QObject> QQmlListProperty(Class<T> elementType, 
    		O o, 
    		BiConsumer<O,T> appendFunction, 
    		ToLongFunction<O> countFunction, 
    		AtFunction<O,T> atFunction,
    		Consumer<O> clearFunction) {
    	this(elementType, o, appendFunction, countFunction, atFunction, clearFunction, null, null);
    }
    
    /**
     * This constructor creates a writable QQmlListProperty with given access methods. The access functions should operate on the same data source (e.g. list).
     * <p>See <a href="https://doc.qt.io/qt/qqmllistproperty.html#QQmlListProperty-3">QQmlListProperty::QQmlListProperty(QObject *, void *, AppendFunction, CountFunction, AtFunction, ClearFunction, ReplaceFunction, RemoveLastFunction)</a></p>
     */
    public <O extends QObject> QQmlListProperty(Class<T> elementType, 
									    		O o, 
									    		BiConsumer<O,T> appendFunction, 
									    		ToLongFunction<O> countFunction, 
									    		AtFunction<O,T> atFunction,
									    		Consumer<O> clearFunction,
									    		ReplaceFunction<O,T> replaceFunction,
									    		Consumer<O> removeLastFunction) {
        super((QPrivateConstructor)null);
        QMetaType metaType = QMetaType.fromType(elementType);
        initialize_native_functions(this, metaType, o, 
        		appendFunction, 
        		countFunction, 
        		atFunction,
        		clearFunction,
        		replaceFunction,
        		removeLastFunction, Objects.hash(metaType, 
						        				appendFunction, 
						                		countFunction, 
						                		atFunction,
						                		clearFunction,
						                		replaceFunction,
						                		removeLastFunction));
    }
    
    private static native void initialize_native_functions(QQmlListProperty<?> instance, QMetaType metaType, QObject o, 
    		BiConsumer<?,?> appendFunction, 
    		ToLongFunction<?> countFunction, 
    		AtFunction<?,?> atFunction,
    		Consumer<?> clearFunction,
    		ReplaceFunction<?,?> replaceFunction,
    		Consumer<?> removeLastFunction, int hash);

    @QtUninvokable
    public final void append(T object) {
    	append(object, QtJambi_LibraryUtilities.internal.checkedNativeId(elementType));
    }

    private native final void append(T object, long elementType);
    
    @io.qt.QtUninvokable
    public final void replace(long index, T object) {
    	replace(index, object, QtJambi_LibraryUtilities.internal.checkedNativeId(elementType));
    }
    
    private native final void replace(long index, T object, long elementType);

    @QtUninvokable
    public final T at(long index) {
    	return at(index, QtJambi_LibraryUtilities.internal.checkedNativeId(elementType));
    }

    private native final T at(long index, long elementType);

    @QtUninvokable
    public native final boolean canAppend();

    @QtUninvokable
    public native final boolean canAt();

    @QtUninvokable
    public native final boolean canClear();

    @QtUninvokable
    public native final boolean canCount();

    @QtUninvokable
    public native final boolean canRemoveLast();

    @QtUninvokable
    public native final boolean canReplace();

    @QtUninvokable
    native public final void clear();

    @QtUninvokable
    public native final long count();
    
    @io.qt.QtUninvokable
    public native final void removeLast();
    
    @QtUninvokable
    public native final QObject object();

    private QQmlListProperty(QPrivateConstructor p) { 
    	super(p);
	}

    @Override
    public QQmlListProperty<T> clone(){
    	QQmlListProperty<T> clone = clone_native();
    	clone.elementType = this.elementType;
    	return clone;
    }
    
    private native QQmlListProperty<T> clone_native();
}
