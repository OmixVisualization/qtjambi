/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
 * QDoublePropertyData is primitive-typed version of QPropertyData&lt;Double&gt;.
 * @see QPropertyData
 */
public class QDoublePropertyData extends QUntypedPropertyData {
	
	QDoublePropertyData(double val) {
		super((QPrivateConstructor)null);
		initialize_native(this, val);
	}
	
	QDoublePropertyData() {
		super((QPrivateConstructor)null);
		initialize_native(this, 0.);
	}
	
	private native static void initialize_native(QDoublePropertyData instance, double val);

	@QtUninvokable
	public final double getValueBypassingBindings() {
		return getValueBypassingBindings(QtJambi_LibraryUtilities.internal.nativeId(this));
	}
	
	@QtUninvokable
	private static native double getValueBypassingBindings(long this_id);
	
	@QtUninvokable
	public final boolean setValueBypassingBindings(double val) {
		return setValueBypassingBindings(QtJambi_LibraryUtilities.internal.nativeId(this), val);
	}
	
	@QtUninvokable
	private static native boolean setValueBypassingBindings(long this_id, double val);
}
