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
package io.qt.core;

import io.qt.QtUninvokable;

/**
 * The QPropertyData class is a helper class for properties with automatic property bindings.
 * <p>Java wrapper for Qt class <a href="https://doc.qt.io/qt/qpropertydata.html">QPropertyData</a></p>
 * <p>For primitive-typed implementations see:</p>
 * <ul>
 * <li>{@link QBooleanPropertyData}</li>
 * <li>{@link QBytePropertyData}</li>
 * <li>{@link QShortPropertyData}</li>
 * <li>{@link QIntPropertyData}</li>
 * <li>{@link QLongPropertyData}</li>
 * <li>{@link QFloatPropertyData}</li>
 * <li>{@link QDoublePropertyData}</li>
 * <li>{@link QCharPropertyData}</li>
 * </ul>
 */
public abstract class QPropertyData<T> extends QUntypedPropertyData {

	public QPropertyData() {
		super();
	}

	QPropertyData(QPrivateConstructor p) {
		super(p);
	}

	abstract QMetaType valueMetaType();
	
    /**
     * <p>Returns the data stored in this property.</p>
     * <p><b>Note:</b> As this will bypass any binding evaluation it might return an outdated value if a binding is set on this property. 
     * Using this method will also not register the property access with any currently executing binding.</p>
     * @return value bypassing bindings
     */
	@QtUninvokable
	public abstract T getValueBypassingBindings();
	
    /**
     * <p>Sets the data value stored in this property to val.</p>
     * <p><b>Note:</b> Using this method will bypass any potential binding registered for this property.</p>
     * @param val
     */
	@QtUninvokable
	public abstract boolean setValueBypassingBindings(T val);
}
