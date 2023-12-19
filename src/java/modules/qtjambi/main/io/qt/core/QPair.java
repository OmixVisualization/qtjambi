/****************************************************************************
**
** Copyright (C) 2009-2023 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
**
** This file is part of Qt Jambi.
**
** $BEGIN_LICENSE$
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
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
** 
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.core;

import java.util.Map;
import java.util.AbstractMap.SimpleEntry;

import io.qt.NativeAccess;
import io.qt.QtUninvokable;

/**
  * QPair keeps two generic values accessible by <code>first</code> and <code>second</code>.
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
    
    /**
     * Convenient factory method for new pair.
     * @param t
     * @param s
     * @return new pair
     */
    @QtUninvokable
    public static <T,S> QPair<T, S> pair(T t, S s){
    	return new QPair<>(t, s);
    }
    
    /**
     * Convenient entry-pair converter method for new pair.
     * @param entry
     * @return new pair
     */
    @QtUninvokable
    public static <T,S> QPair<T, S> fromEntry(Map.Entry<T,S> entry){
    	return new QPair<>(entry.getKey(), entry.getValue());
    }
    
    /**
     * Convenient pair-entry converter method.
     * @param pair
     * @return new entry
     */
    @QtUninvokable
    public static <T,S> Map.Entry<T, S> toEntry(QPair<T,S> pair){
    	return new SimpleEntry<>(pair.first, pair.second);
    }
}
