/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2020 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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
package io.qt;

import java.util.Comparator;

import io.qt.internal.NativeAccess;

/**
 * The QtEnumerator interface servers as a base for all qint64 enum classes.
 * Its sole purpose is to unify the access to the integer
 * value of enumerators and flags using the value() method.
 *
 * If you manually implement this class, your implementation must contain a method
 * with the following signature:
 *
 *      public static T resolve(long value);
 *
 * where T is your subclass. This should return the enum value corresponding to the
 * specified int value.
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
	 * @see Enum#ordinal()
	 * @return the ordinal of this enumeration constant
	 */
	public int ordinal();
	
	/**
	 * @see Enum#name()
	 * @return the name of this enum constant
	 */
	public String name();
	
	/**
	 * @see Enum#getDeclaringClass()
	 * @return the Class object corresponding to this enum constant's enum type
	 */
	public Class<? extends QtLongEnumerator> getDeclaringClass();
	
	/**
	 * Creates a comparator for enumeration values
	 * @param <E>
	 * @return comparator
	 */
	public static <E extends QtLongEnumerator> Comparator<E> comparator() {
		return (E e1, E e2)->{
			return Long.compare(e1.value(), e2.value());
		};
	}
}
