/****************************************************************************
**
** Copyright (C) 2009-2015 Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
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

package org.qtjambi.qt.qml;

import org.qtjambi.qt.*;
import org.qtjambi.qt.core.QObject;

/**
 * This class is a wrapper class of the C++ template type QQmlListProperty.
 * The generator takes care about mapping the native template parameter
 * to the generic java parameter.
 *
 * @author Peter Droste <info@omix-visualization.com>
 */
@QtJambiGeneratedClass
public class QQmlListProperty<T extends QObject> extends org.qtjambi.qt.internal.QtJambiObject
    implements java.lang.Cloneable
{

    static {
        org.qtjambi.qt.qml.QtJambi_LibraryInitializer.init();
    }

    public QQmlListProperty(){
        super((QPrivateConstructor)null);
        __qt_QQmlListProperty();
    }

    native void __qt_QQmlListProperty();

    public QQmlListProperty(org.qtjambi.qt.core.QObject o, java.util.List<T> list){
        super((QPrivateConstructor)null);
        __qt_QQmlListProperty_QObject_List(o == null ? 0 : o.nativeId(), list);
    }

    native void __qt_QQmlListProperty_QObject_List(long o, java.util.List<T> list);

    @QtBlockedSlot
    public final void append(T object)    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        __qt_append_QObject(nativeId(), object);
    }
    @QtBlockedSlot
    native void __qt_append_QObject(long __this__nativeId, T object);

    @QtBlockedSlot
    public final T at(int index)    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        return __qt_at_int(nativeId(), index);
    }
    @QtBlockedSlot
    native T __qt_at_int(long __this__nativeId, int index);

    @QtBlockedSlot
    public final boolean canAppend()    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        return __qt_canAppend(nativeId());
    }
    @QtBlockedSlot
    native boolean __qt_canAppend(long __this__nativeId);

    @QtBlockedSlot
    public final boolean canAt()    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        return __qt_canAt(nativeId());
    }
    @QtBlockedSlot
    native boolean __qt_canAt(long __this__nativeId);

    @QtBlockedSlot
    public final boolean canClear()    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        return __qt_canClear(nativeId());
    }
    @QtBlockedSlot
    native boolean __qt_canClear(long __this__nativeId);

    @QtBlockedSlot
    public final boolean canCount()    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        return __qt_canCount(nativeId());
    }
    @QtBlockedSlot
    native boolean __qt_canCount(long __this__nativeId);

    @QtBlockedSlot
    public final void clear()    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        __qt_clear(nativeId());
    }
    @QtBlockedSlot
    native void __qt_clear(long __this__nativeId);

    @QtBlockedSlot
    public final int count()    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        return __qt_count(nativeId());
    }
    @QtBlockedSlot
    native int __qt_count(long __this__nativeId);

    @QtBlockedSlot
    public final org.qtjambi.qt.core.QObject object()    {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        return __qt_object(nativeId());
    }
    @QtBlockedSlot
    native org.qtjambi.qt.core.QObject __qt_object(long __this__nativeId);

    public static native <T extends QObject> QQmlListProperty<T> fromNativePointer(QNativePointer nativePointer);

    protected QQmlListProperty(QPrivateConstructor p) { super(p); } 

    public static native QNativePointer nativePointerArray(QQmlListProperty<?> array[]);

    @Override
    public QQmlListProperty<T> clone() {
        if (nativeId() == 0)
            throw new QNoNativeResourcesException("Function call on incomplete object of type: " +getClass().getName());
        return __qt_clone(nativeId());
    }
    native QQmlListProperty<T> __qt_clone(long __this_nativeId);
}
