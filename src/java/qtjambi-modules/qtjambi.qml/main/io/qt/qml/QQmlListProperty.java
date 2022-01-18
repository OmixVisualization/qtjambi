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
        io.qt.qml.QtJambi_LibraryUtilities.initialize();
    }
    
    private final java.util.List<T> list;

    public QQmlListProperty(){
        super((QPrivateConstructor)null);
        this.list = null;
        initialize_native(this);
    }

    private static native <T extends QObject> void initialize_native(QQmlListProperty<T> instance);

    public QQmlListProperty(io.qt.core.QObject o, java.util.List<T> list){
        super((QPrivateConstructor)null);
        this.list = list;
        initialize_native(this, Objects.requireNonNull(o), list);
    }

    private static native <T extends QObject> void initialize_native(QQmlListProperty<T> instance, io.qt.core.QObject o, java.util.List<T> list);

    @QtUninvokable
    public native final void append(T object);

    @QtUninvokable
    public native final T at(long index);

    @QtUninvokable
    public native final boolean canAppend();

    @QtUninvokable
    public native final boolean canAt();

    @QtUninvokable
    public native final boolean canClear();

    @QtUninvokable
    public native final boolean canCount();

    @QtUninvokable
    native public final void clear();

    @QtUninvokable
    public native final long count();

    @QtUninvokable
    public native final io.qt.core.QObject object();

    private QQmlListProperty(QPrivateConstructor p) { 
    	super(p); 
        this.list = null;
	}

    @Override
    public QQmlListProperty<T> clone() {
        if(list!=null) {
        	return new QQmlListProperty<>(object(), list);
        }else {
        	return clone_native();
        }
    }
    private native QQmlListProperty<T> clone_native();
}
