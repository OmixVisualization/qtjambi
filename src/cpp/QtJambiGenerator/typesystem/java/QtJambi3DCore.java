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

package generator;

import io.qt.*;
import io.qt.qt3d.*;

class QAbstractAspect___java extends QAbstractAspect {
    
    /**
     * <p>See <a href="@docRoot/qt3dcore-qabstractaspect.html#registerBackendType">Qt3DCore::QAbstractAspect::registerBackendType(Qt3DCore::QBackendNodeMapperPtr)</a></p>
     */
    @io.qt.QtUninvokable
    protected final void registerBackendType(Class<? extends io.qt.core.QObject> type, io.qt.qt3d.core.QBackendNodeMapper functor)    {
        registerBackendType(io.qt.core.QMetaObject.forType(type), functor);
    }
    
    @io.qt.QtUninvokable
    protected final void unregisterBackendType(Class<? extends io.qt.core.QObject> type)    {
        unregisterBackendType(io.qt.core.QMetaObject.forType(type));
    }
    
}// class

class QEntity_6_ {

    /**
     * <p>See <a href="@docRoot/qt3dcore-qentity.html#componentsOfType">Qt3DCore::QEntity::componentsOfType&lt;T>()const</a></p>
     */
    @io.qt.QtUninvokable
    public final <T extends io.qt.qt3d.core.QComponent> io.qt.core.QList<T> componentsOfType(Class<T> type)
    {
        io.qt.core.QList<T> matchComponents = new io.qt.core.QList<T>(type);
        for (io.qt.qt3d.core.QComponent component : components()) {
            if (type.isInstance(component))
                matchComponents.append(type.cast(component));
        }
        return matchComponents;
    }
    
}// class

class QEntity_5_ {

    /**
     * <p>See <a href="@docRoot/qt3dcore-qentity.html#componentsOfType">Qt3DCore::QEntity::componentsOfType&lt;T>()const</a></p>
     */
    @io.qt.QtUninvokable
    public final <T extends io.qt.qt3d.core.QComponent> io.qt.core.QVector<T> componentsOfType(Class<T> type)
    {
        io.qt.core.QVector<T> matchComponents = new io.qt.core.QVector<T>(type);
        for (io.qt.qt3d.core.QComponent component : components()) {
            if (type.isInstance(component))
                matchComponents.append(type.cast(component));
        }
        return matchComponents;
    }
    
}// class
