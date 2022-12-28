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

package io.qt.core;

import static java.lang.Math.*;

/**
 * Use {@link QtGlobal} instead.
 */
@Deprecated
public final class QMath {
	
	private QMath() {}
	
    /**
     * Use {@link QtGlobal#qFuzzyCompare(double, double)} instead.
     */
	@Deprecated
	public static boolean fuzzyCompare(double p1, double p2)
	{
	    return (abs(p1 - p2) * 1000000000000. <= min(abs(p1), abs(p2)));
	}
	
    /**
     * Use {@link QtGlobal#qFuzzyCompare(float, float)} instead.
     */
	@Deprecated
	public static boolean fuzzyCompare(float p1, float p2)
	{
	    return (abs(p1 - p2) * 100000.f <= min(abs(p1), abs(p2)));
	}
	
    /**
     * Use {@link QtGlobal#qFuzzyIsNull(double)} instead.
     */
	@Deprecated
	public static boolean fuzzyIsNull(double d)
	{
	    return abs(d) <= 0.000000000001;
	}
	
    /**
     * Use {@link QtGlobal#qFuzzyIsNull(float)} instead.
     */
	@Deprecated
	public static boolean fuzzyIsNull(float f)
	{
	    return abs(f) <= 0.00001f;
	}
	
    /**
     * Use {@link QtGlobal#qBound(byte,byte,byte)} instead.
     */
	@Deprecated
	public static byte bound(byte min, byte val, byte max){ return (byte)max(min, min(max, val)); }
	
    /**
     * Use {@link QtGlobal#qBound(short,short,short)} instead.
     */
	@Deprecated
	public static short bound(short min, short val, short max){ return (short)max(min, min(max, val)); }
	
    /**
     * Use {@link QtGlobal#qBound(int,int,int)} instead.
     */
	@Deprecated
	public static int bound(int min, int val, int max){ return max(min, min(max, val)); }
	
    /**
     * Use {@link QtGlobal#qBound(long,long,long)} instead.
     */
	@Deprecated
	public static long bound(long min, long val, long max){ return max(min, min(max, val)); }
	
    /**
     * Use {@link QtGlobal#qBound(float,float,float)} instead.
     */
	@Deprecated
	public static float bound(float min, float val, float max){ return max(min, min(max, val)); }
	
    /**
     * Use {@link QtGlobal#qBound(double,double,double)} instead.
     */
	@Deprecated
	public static double bound(double min, double val, double max){ return max(min, min(max, val)); }
}

