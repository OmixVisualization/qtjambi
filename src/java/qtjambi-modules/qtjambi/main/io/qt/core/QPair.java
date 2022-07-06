/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2022 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

package io.qt.core;

import io.qt.NativeAccess;
import io.qt.QtUninvokable;

/**
  * QPair keeps two generic values. They are public with the names
  * <code>first</code> and <code>second</code>.
  *
  */
public class QPair <T, S> implements Cloneable {

    /** First value of the pair. */
	@NativeAccess public T first;
    /** Second value of the pair. */
    @NativeAccess public S second;

    /**
     * Constructs a pair.
     * @param t The first parameter.
     * @param s The second parameter.
     */
    @NativeAccess
    public QPair(T t, S s) {
        first = t;
        second = s;
    }


    /**
     * Returns true if this pair is the same as the other pair. If any
     * of the first or second members are null the result is false regardless.
     * @param o The other parameter
     * @return True if they are equal.
     */
    @Override
    @QtUninvokable
    public boolean equals(Object o) {
        QPair<?, ?> other = o instanceof QPair ? (QPair<?, ?>) o : null;
        if (other == null || first == null || second == null || other.first == null || other.second == null)
            return false;
        return first.equals(other.first) && second.equals(other.second);
    }
    
    @Override
    @QtUninvokable
    public int hashCode() { 
    	int hashCode = getClass().hashCode();
        hashCode = 31*hashCode + (first==null ? 0 : first.hashCode());
        hashCode = 31*hashCode + (second==null ? 0 : second.hashCode());
        return hashCode;
    }


    /**
     * Returns a string representation of this pair.
     */
    @Override
    @QtUninvokable
    public String toString() {
        return QList.pairPrefix() + "(" + (first != null ? first.toString() : "null")
         + "," + (second != null ? second.toString() : "null") + ")";
    }


    /**
     * Returns a copy of this object.
     */
    @Override
    @QtUninvokable
    public QPair<T, S> clone() {
        return new QPair<T, S>(first, second);
    }
}
