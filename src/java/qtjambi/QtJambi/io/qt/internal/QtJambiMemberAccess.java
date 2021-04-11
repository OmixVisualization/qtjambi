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
package io.qt.internal;

import io.qt.QtObjectInterface;

/**
 * This class manages member access for interface default functions.
 * @param <T> interface type
 */
public class QtJambiMemberAccess<T extends QtObjectInterface> {
	
	private final java.lang.ref.WeakReference<T> instanceRef;
	
	protected QtJambiMemberAccess(T instance) {
		instanceRef = new java.lang.ref.WeakReference<>(instance);
	}
	
	protected T instance() {
		return instanceRef.get();
	}
	
	protected static <Q extends QtObjectInterface,M extends QtJambiMemberAccess<Q>> M findMemberAccess(Q ifc, Class<? extends QtObjectInterface> interfaceClass, Class<M> accessClass){
		if(ifc instanceof QtJambiObject) {
			QtJambiObject object = (QtJambiObject)ifc;
			return accessClass.cast(object.nativeLink.getMemberAccess(interfaceClass));
		}else {
			QtJambiInternal.NativeLink link = QtJambiInternal.findInterfaceLink(ifc, true);
			M result = accessClass.cast(link.getMemberAccess(interfaceClass));
			return result;
		}
	}
}
