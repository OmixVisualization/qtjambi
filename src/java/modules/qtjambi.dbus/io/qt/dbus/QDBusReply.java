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

package io.qt.dbus;

import java.util.Objects;

/**
 * The QDBusReply class stores the reply for a method call to a remote object.
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qdbusreply.html">QDBusReply</a></p>
 */
public final class QDBusReply<T> extends io.qt.QtObject implements java.lang.Cloneable
{
    static {
        QtJambi_LibraryUtilities.initialize();
    }

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
    }

    private native static <T> void initialize_native(QDBusReply<T> instance, io.qt.dbus.QDBusError dbusError);
    
    public QDBusReply(io.qt.dbus.QDBusMessage reply, Class<T> valueType, io.qt.core.QMetaType... instantiation){
        super((QPrivateConstructor)null);
        Objects.requireNonNull(valueType);
        initialize_native(this, reply, io.qt.core.QMetaType.fromType(valueType, instantiation));
    }
    
    private native static <T> void initialize_native(QDBusReply<T> instance, io.qt.dbus.QDBusMessage reply, io.qt.core.QMetaType metaType);

    public QDBusReply(io.qt.dbus.QDBusPendingCall pcall, Class<T> valueType, io.qt.core.QMetaType... instantiation){
        super((QPrivateConstructor)null);
        Objects.requireNonNull(valueType);
        initialize_native(this, pcall, io.qt.core.QMetaType.fromType(valueType, instantiation));
    }
    
    private native static <T> void initialize_native(QDBusReply<T> instance, io.qt.dbus.QDBusPendingCall pcall, io.qt.core.QMetaType metaType);

    @io.qt.QtUninvokable
    private native final void set(io.qt.dbus.QDBusReply<T> other);
    
    @io.qt.QtUninvokable
    public native final io.qt.dbus.QDBusError error();
    
    @io.qt.QtUninvokable
    public native final boolean isValid();

    @io.qt.QtUninvokable
    public native T value();
    
    protected QDBusReply(QPrivateConstructor p) { super(p); } 
}
