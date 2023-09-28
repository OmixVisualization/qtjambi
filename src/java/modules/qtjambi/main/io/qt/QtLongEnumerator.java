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
package io.qt;

import java.util.Comparator;

/**
 * The QtEnumerator interface servers as a base for all 64 Bit enum classes.
 * Its sole purpose is to unify the access to the <code>long</code>
 * value of enumerators and flags using the {@link #value()} method.
 */
public interface QtLongEnumerator extends QtAbstractEnumerator {
    /**
     * This function should return a long value for the enum
     * values of the enumeration that implements this interface.
     */
	@NativeAccess
    public default long value() {
		return ordinal();
	}
	
	/**
	 * Creates a comparator for enumeration values
	 * @param <E>
	 * @return comparator
	 */
	public static <E extends QtLongEnumerator> @NonNull Comparator<@StrictNonNull E> comparator() {
		return (@StrictNonNull E e1, @StrictNonNull E e2)->{
			return Long.compare(e1.value(), e2.value());
		};
	}
}
