/****************************************************************************
**
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.qml;

import java.util.Objects;

import io.qt.QtUninvokable;
import io.qt.core.QObject;

/**
 * This class is a wrapper class of the C++ template type QQmlListProperty.
 * The generator takes care about mapping the native template parameter
 * to the generic java parameter.
 *
 */
public final class QQmlListProperty<T extends QObject> extends io.qt.QtObject
    implements java.lang.Cloneable
{

    static {
        io.qt.qml.QtJambi_LibraryInitializer.init();
    }
    
    private final java.util.List<T> list;

    public QQmlListProperty(){
        super((QPrivateConstructor)null);
        this.list = null;
        __qt_QQmlListProperty_new(this);
    }

    private static native void __qt_QQmlListProperty_new(Object instance);

    public QQmlListProperty(io.qt.core.QObject o, java.util.List<T> list){
        super((QPrivateConstructor)null);
        this.list = list;
        __qt_QQmlListProperty_new(this, Objects.requireNonNull(o), list);
    }

    private static native <T> void __qt_QQmlListProperty_new(Object instance, io.qt.core.QObject o, java.util.List<T> list);

    @QtUninvokable
    public final void append(T object)    {
        __qt_append_QObject(io.qt.internal.QtJambiInternal.checkedNativeId(this), object);
    }
    @QtUninvokable
    native void __qt_append_QObject(long __this__nativeId, T object);

    @QtUninvokable
    public final T at(int index)    {
        return __qt_at_int(io.qt.internal.QtJambiInternal.checkedNativeId(this), index);
    }
    @QtUninvokable
    native T __qt_at_int(long __this__nativeId, int index);

    @QtUninvokable
    public final boolean canAppend()    {
        return __qt_canAppend(io.qt.internal.QtJambiInternal.checkedNativeId(this));
    }
    @QtUninvokable
    native boolean __qt_canAppend(long __this__nativeId);

    @QtUninvokable
    public final boolean canAt()    {
        return __qt_canAt(io.qt.internal.QtJambiInternal.checkedNativeId(this));
    }
    @QtUninvokable
    native boolean __qt_canAt(long __this__nativeId);

    @QtUninvokable
    public final boolean canClear()    {
        return __qt_canClear(io.qt.internal.QtJambiInternal.checkedNativeId(this));
    }
    @QtUninvokable
    native boolean __qt_canClear(long __this__nativeId);

    @QtUninvokable
    public final boolean canCount()    {
        return __qt_canCount(io.qt.internal.QtJambiInternal.checkedNativeId(this));
    }
    @QtUninvokable
    native boolean __qt_canCount(long __this__nativeId);

    @QtUninvokable
    public final void clear()    {
        __qt_clear(io.qt.internal.QtJambiInternal.checkedNativeId(this));
    }
    @QtUninvokable
    native void __qt_clear(long __this__nativeId);

    @QtUninvokable
    public final int count()    {
        return __qt_count(io.qt.internal.QtJambiInternal.checkedNativeId(this));
    }
    @QtUninvokable
    native int __qt_count(long __this__nativeId);

    @QtUninvokable
    public final io.qt.core.QObject object()    {
        return __qt_object(io.qt.internal.QtJambiInternal.checkedNativeId(this));
    }
    @QtUninvokable
    native io.qt.core.QObject __qt_object(long __this__nativeId);

    private QQmlListProperty(QPrivateConstructor p) { 
    	super(p); 
        this.list = null;
	}

    @Override
    public QQmlListProperty<T> clone() {
        if(list!=null) {
        	return new QQmlListProperty<>(object(), list);
        }else {
        	return __qt_clone(io.qt.internal.QtJambiInternal.checkedNativeId(this));
        }
    }
    private native QQmlListProperty<T> __qt_clone(long __this_nativeId);

    @io.qt.internal.NativeAccess
    private static native int __qt_sizeOfType();
}
