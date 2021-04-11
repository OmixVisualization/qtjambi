/****************************************************************************
**
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.dbus;

import static io.qt.internal.QtJambiInternal.nativeId;

import java.util.Objects;

import io.qt.core.QVariant;

public final class QDBusReply<T> extends io.qt.QtObject implements java.lang.Cloneable
{
    static {
        QtJambi_LibraryInitializer.init();
    }

    private final Class<T> valueType;
    
    public QDBusReply() {
    	this(new io.qt.dbus.QDBusError());
    }
    
    @Override
    public QDBusReply<T> clone() {
    	QDBusReply<T> newReply = new QDBusReply<>();
    	newReply.set(this);
        return newReply;
    }

    public QDBusReply(io.qt.dbus.QDBusError dbusError){
        super((QPrivateConstructor)null);
        __qt_QDBusReply_new_cref_QDBusError(this, dbusError);
        this.valueType = null;
    }

    private native static <T> void __qt_QDBusReply_new_cref_QDBusError(Object instance, io.qt.dbus.QDBusError dbusError);
    
    public QDBusReply(io.qt.dbus.QDBusMessage reply, Class<T> valueType){
        super((QPrivateConstructor)null);
        __qt_QDBusReply_new_cref_QDBusMessage(this, reply);
        this.valueType = Objects.requireNonNull(valueType);
    }
    
    private native static <T> void __qt_QDBusReply_new_cref_QDBusMessage(Object instance, io.qt.dbus.QDBusMessage reply);

    public QDBusReply(io.qt.dbus.QDBusPendingCall pcall, Class<T> valueType){
        super((QPrivateConstructor)null);
        __qt_QDBusReply_new_cref_QDBusPendingCall(this, pcall);
        this.valueType = Objects.requireNonNull(valueType);
    }
    
    private native static <T> void __qt_QDBusReply_new_cref_QDBusPendingCall(Object instance, io.qt.dbus.QDBusPendingCall pcall);

    @io.qt.QtUninvokable
    private final void set(io.qt.dbus.QDBusReply<T> other){
        __qt_QDBusReply_set_cref_QDBusReply(nativeId(this), other);
    }
    
    @io.qt.QtUninvokable
    private native <T2> io.qt.dbus.QDBusReply<java.lang.Object> __qt_QDBusReply_set_cref_QDBusReply(long __this__nativeId, io.qt.dbus.QDBusReply<T2> other);
    
    @io.qt.QtUninvokable
    public final io.qt.dbus.QDBusError error(){
        return __qt_QDBusReply_error_constfct(nativeId(this));
    }
    
    @io.qt.QtUninvokable
    private native io.qt.dbus.QDBusError __qt_QDBusReply_error_constfct(long __this__nativeId);
    
    @io.qt.QtUninvokable
    public final boolean isValid(){
        return __qt_QDBusReply_isValid_constfct(nativeId(this));
    }
    
    @io.qt.QtUninvokable
    private native boolean __qt_QDBusReply_isValid_constfct(long __this__nativeId);

    @SuppressWarnings("unchecked")
	public final T value(){
        return valueType==null ? (T)__qt_QDBusReply__value_constfct(nativeId(this)) : QVariant.convert(__qt_QDBusReply__value_constfct(nativeId(this)), valueType);
    }

    @io.qt.QtUninvokable
    private native java.lang.Object __qt_QDBusReply__value_constfct(long __this__nativeId);
    
    protected QDBusReply(QPrivateConstructor p) { super(p); valueType = null; } 
}
