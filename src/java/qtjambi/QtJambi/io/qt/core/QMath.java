/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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
** $END_LICENSE$

**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.core;

import static java.lang.Math.*;

/**
 * Provides additional math functions.
 */
public final class QMath {
	
	private QMath() {}
	
    /**
     * Compares the floating point value p1 and p2 and returns true if they are considered equal, otherwise false.
     * @see https://doc.qt.io/qt/qtglobal.html#qFuzzyCompare
     * @param p1
     * @param p2
     * @return compare
     */
	public static boolean fuzzyCompare(double p1, double p2)
	{
	    return (abs(p1 - p2) * 1000000000000. <= min(abs(p1), abs(p2)));
	}
	
    /**
     * Compares the floating point value p1 and p2 and returns true if they are considered equal, otherwise false.
     * @see https://doc.qt.io/qt/qtglobal.html#qFuzzyCompare-1
     * @param p1
     * @param p2
     * @return compare
     */
	public static boolean fuzzyCompare(float p1, float p2)
	{
	    return (abs(p1 - p2) * 100000.f <= min(abs(p1), abs(p2)));
	}
	
    /**
     * Returns true if the absolute value of d is within 0.000000000001 of 0.0.
     * @see https://doc.qt.io/qt/qtglobal.html#qFuzzyIsNull
     * @param d
     * @return is null
     */
	public static boolean fuzzyIsNull(double d)
	{
	    return abs(d) <= 0.000000000001;
	}
	
    /**
     * Returns true if the absolute value of f is within 0.00001f of 0.0.
     * @see https://doc.qt.io/qt/qtglobal.html#qFuzzyIsNull-1
     * @param d
     * @return is null
     */
	public static boolean fuzzyIsNull(float f)
	{
	    return abs(f) <= 0.00001f;
	}
	
    /**
     * Returns val bounded by min and max.
     * @see https://doc.qt.io/qt/qtglobal.html#qBound
     * @param min
     * @param val
     * @param max
     * @return bounded value
     */
	public static byte bound(byte min, byte val, byte max){ return (byte)max(min, min(max, val)); }
	
    /**
     * Returns val bounded by min and max.
     * @see https://doc.qt.io/qt/qtglobal.html#qBound
     * @param min
     * @param val
     * @param max
     * @return bounded value
     */
	public static short bound(short min, short val, short max){ return (short)max(min, min(max, val)); }
	
    /**
     * Returns val bounded by min and max.
     * @see https://doc.qt.io/qt/qtglobal.html#qBound
     * @param min
     * @param val
     * @param max
     * @return bounded value
     */
	public static int bound(int min, int val, int max){ return max(min, min(max, val)); }
	
    /**
     * Returns val bounded by min and max.
     * @see https://doc.qt.io/qt/qtglobal.html#qBound
     * @param min
     * @param val
     * @param max
     * @return bounded value
     */
	public static long bound(long min, long val, long max){ return max(min, min(max, val)); }
	
    /**
     * Returns val bounded by min and max.
     * @see https://doc.qt.io/qt/qtglobal.html#qBound
     * @param min
     * @param val
     * @param max
     * @return bounded value
     */
	public static float bound(float min, float val, float max){ return max(min, min(max, val)); }
	
    /**
     * Returns val bounded by min and max.
     * @see https://doc.qt.io/qt/qtglobal.html#qBound
     * @param min
     * @param val
     * @param max
     * @return bounded value
     */
	public static double bound(double min, double val, double max){ return max(min, min(max, val)); }
}

