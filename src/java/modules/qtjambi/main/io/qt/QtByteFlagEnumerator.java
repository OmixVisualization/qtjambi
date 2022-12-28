/****************************************************************************
**
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt;

import io.qt.internal.EnumUtility;

/**
 * The QtByteFlagEnumerator interface servers as a base for all 8 Bit enums available as QFlags.
 * @see QFlags
 */
public interface QtByteFlagEnumerator extends QtByteEnumerator, QtAbstractFlagEnumerator {
    /**
     * This function should return an integer value for the enum
     * values of the enumeration that implements this interface.
     */
	public default byte value() {
		int o = ordinal();
		if(!EnumUtility.isSmallEnum(this))
			return (byte)o;
		return (byte)(o==0 ? 0 : 0x01 << (o-1));
	}
}
