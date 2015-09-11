/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
**
** This file is part of Qt Jambi.
**
** ** $BEGIN_LICENSE$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
** 
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** 
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package org.qtjambi.qt;

/**
 * This class manages a set of QtEnumerator values. Each enum value
 * is treated as a flag that is either set or unset. You can set and
 * clear flags, and query which flags are set.
 */
@SuppressWarnings("serial")
public abstract class QFlags<T extends QtEnumerator>
    implements QtEnumerator,
               java.io.Serializable,
               Cloneable
{
     /**
      * Creates a new QFlags where the flags in <tt>args</tt> are set.
      */
     protected QFlags(T ... args) {
         set(args);
     }

     /**
      * Sets the flag <tt>other</tt>
      */
     public final void set(QFlags<T> other) {
         value |= other.value();
     }

     /**
      * Sets the flags in <tt>ts</tt>.
      */
     public final void set(T ... ts) {
         for (T t : ts)
             value |= t.value();
     }

    /**
     * Returns true if flag <tt>other</tt> is set; otherwise, returns
     * false.
     */
     public final boolean isSet(QFlags<T> other) {
         return (value & other.value()) == other.value();
     }

     /**
      * Returns true if all <tt>ts</tt> flags are set; otherwise,
      * returns false.
      */
     public final boolean isSet(T ... ts) {
         for (T t : ts) {
             if ((t.value() & value) != t.value())
                  return false;
         }
         return true;
     }

    /**
     * Clears the flag <tt>other</tt>.
     */
     public final void clear(QFlags<T> other) {
         value &= ~other.value();
     }

     /**
      * Clears all flags in <tt>ts</tt>.
      *
      */
     public final void clear(T ... ts) {
         for (T t : ts)
             value &= ~t.value();
     }

    /**
     * Clears all flags.
     */
     public final void clearAll() {
         value = 0;
     }

    /**
     * Sets the value of this QFlags.
     */
     public final void setValue(int value) {
         this.value = value;
     }

    /**
     * Returns the value of this QFlags.
     */
     public final int value() {
         return value;
     }

    /**
     * {@inheritDoc}
     */
     @Override
    public final boolean equals(Object object) {
         return object != null
             && object.getClass() == getClass()
             && ((QFlags<?>) object).value() == value();
     }

    /**
     * {@inheritDoc}
     */
     @Override
    public final String toString() {
         String hexString = Integer.toHexString(value);
         return "0x" + ("00000000".substring(hexString.length())) + hexString;
     }

     private int value;
}

