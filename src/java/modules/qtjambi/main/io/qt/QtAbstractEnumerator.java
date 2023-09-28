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

/**
 * The QtAbstractEnumerator interface servers as a base for all Qt Jambi enums.
 */
public interface QtAbstractEnumerator {
	/**
	 * @see Enum#ordinal()
	 * @return the ordinal of this enumeration constant
	 */
	public int ordinal();
	
	/**
	 * @see Enum#name()
	 * @return the name of this enum constant
	 */
	public @NonNull String name();
	
	/**
	 * @see Enum#getDeclaringClass()
	 * @return the Class object corresponding to this enum constant's enum type
	 */
	public @NonNull Class<? extends QtAbstractEnumerator> getDeclaringClass();
	
    /**
     * Converts the enumerator e to the equivalent value expressed in its enumeration's underlying type.
     * <p>See <a href="https://doc.qt.io/qt/qttypetraits-proxy.html#qToUnderlying"><code>qToUnderlying&lt;Enum>(Enum)</code></a></p>
     */
	public static int qToUnderlying(@StrictNonNull QtEnumerator e) {
		return e.value();
	}
	
    /**
     * Converts the enumerator e to the equivalent value expressed in its enumeration's underlying type.
     * <p>See <a href="https://doc.qt.io/qt/qttypetraits-proxy.html#qToUnderlying"><code>qToUnderlying&lt;Enum>(Enum)</code></a></p>
     */
	public static short qToUnderlying(@StrictNonNull QtShortEnumerator e) {
		return e.value();
	}
	
    /**
     * Converts the enumerator e to the equivalent value expressed in its enumeration's underlying type.
     * <p>See <a href="https://doc.qt.io/qt/qttypetraits-proxy.html#qToUnderlying"><code>qToUnderlying&lt;Enum>(Enum)</code></a></p>
     */
	public static byte qToUnderlying(@StrictNonNull QtByteEnumerator e) {
		return e.value();
	}
	
    /**
     * Converts the enumerator e to the equivalent value expressed in its enumeration's underlying type.
     * <p>See <a href="https://doc.qt.io/qt/qttypetraits-proxy.html#qToUnderlying"><code>qToUnderlying&lt;Enum>(Enum)</code></a></p>
     */
	public static long qToUnderlying(@StrictNonNull QtLongEnumerator e) {
		return e.value();
	}
}
