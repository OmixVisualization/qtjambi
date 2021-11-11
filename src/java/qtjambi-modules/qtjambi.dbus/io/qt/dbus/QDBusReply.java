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

import java.util.Objects;

import io.qt.core.QVariant;

public final class QDBusReply<T> extends io.qt.QtObject implements java.lang.Cloneable
{
    static {
        QtJambi_LibraryUtilities.initialize();
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
        initialize_native(this, dbusError);
        this.valueType = null;
    }

    private native static <T> void initialize_native(QDBusReply<T> instance, io.qt.dbus.QDBusError dbusError);
    
    public QDBusReply(io.qt.dbus.QDBusMessage reply, Class<T> valueType){
        super((QPrivateConstructor)null);
        initialize_native(this, reply);
        this.valueType = Objects.requireNonNull(valueType);
    }
    
    private native static <T> void initialize_native(QDBusReply<T> instance, io.qt.dbus.QDBusMessage reply);

    public QDBusReply(io.qt.dbus.QDBusPendingCall pcall, Class<T> valueType){
        super((QPrivateConstructor)null);
        initialize_native(this, pcall);
        this.valueType = Objects.requireNonNull(valueType);
    }
    
    private native static <T> void initialize_native(QDBusReply<T> instance, io.qt.dbus.QDBusPendingCall pcall);

    @io.qt.QtUninvokable
    private native final void set(io.qt.dbus.QDBusReply<T> other);
    
    @io.qt.QtUninvokable
    public native final io.qt.dbus.QDBusError error();
    
    @io.qt.QtUninvokable
    public native final boolean isValid();

    @SuppressWarnings("unchecked")
	public final T value(){
        return valueType==null ? (T)value_native() : QVariant.convert(value_native(), valueType);
    }

    @io.qt.QtUninvokable
    private native java.lang.Object value_native();
    
    protected QDBusReply(QPrivateConstructor p) { super(p); valueType = null; } 
}
