/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
**
** This file is part of Qt Jambi.
**
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** In addition, as a special exception, the copyright holders grant you
** certain additional rights. These rights are described in the Nokia Qt
** LGPL Exception version 1.0, included in the file LGPL_EXCEPTION.txt in
** this package.
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package generator;

import io.qt.*;
import io.qt.internal.QtJambiObject.QPrivateConstructor;
import io.qt.core.*;

class QObjectType___ extends QObjectType {
        if(java.util.logging.Logger.getLogger("io.qt.autotests").isLoggable(java.util.logging.Level.FINEST)) {
            Class<?> cls = this.getClass();
            int hash = System.identityHashCode(this);
            destroyed.connect(()->{
                String className = cls.getName();
                String shortClassName = className;
                int i = shortClassName.lastIndexOf('.');
                if(i > 0)
                    shortClassName = shortClassName.substring(i + 1);
                java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINEST, shortClassName + ".dtor(" + className + "@" + hash + ") thread=" + Thread.currentThread().getId());
            });
        }
}// class

class SharedPointerTest___ extends SharedPointerTest {
	@QtBlockedSlot
	public final io.qt.widgets.QGraphicsItemInterface createSharedObject2()    {
		io.qt.widgets.QGraphicsItemInterface item = privateCreateSharedObject2();
		if(item.toGraphicsObject()!=null){
			item = item.toGraphicsObject();
		}
		return item;
	}
}// class

class General___{
    public final static io.qt.InternalAccess internalAccess = QtJambi_LibraryUtilities.internal;
}// class
