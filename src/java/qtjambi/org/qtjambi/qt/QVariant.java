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

import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QBitArray;
import org.qtjambi.qt.core.QDate;
import org.qtjambi.qt.core.QDateTime;
import org.qtjambi.qt.core.QLocale;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QRegExp;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QSizeF;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.core.QLine;
import org.qtjambi.qt.core.QLineF;
import org.qtjambi.qt.internal.QtJambiObject;

import java.util.List;
import java.util.Collection;
import java.util.ArrayList;
import java.util.Map;
import java.util.HashMap;

/**
 * The QVariant class contains a set of static methods to convert between datatypes.
 * <p>
 * You can check with this class whether a datatype can be converted to another.
 * This is datatypes which cannot simply be cast to each other, but requires
 * processing to convert, i.e., you cannot use the instanceof operator as you would
 * for classes with the same super class.
*/
public class QVariant extends QtJambiObject {

    // Java types
    /** */
    public static final int Invalid = 0;
    /** */
    public static final int Double = 6;
    /** */
    public static final int String = 10;
    /** */
    public static final int Boolean = 1;
    /** */
    public static final int ByteArray = 12;
    /** */
    public static final int BitArray = 13;
    /** */
    public static final int Char = 7;
    /** */
    public static final int Date = 14;
    /** */
    public static final int DateTime = 16;
    /** */
    public static final int Int = 2;
    /** */
    public static final int Line = 23;
    /** */
    public static final int LineF = 24;
    /** */
    public static final int Locale = 18;
    /** */
    public static final int Long = 4;
    /** */
    public static final int Point = 25;
    /** */
    public static final int PointF = 26;
    /** */
    public static final int Rect = 19;
    /** */
    public static final int RectF = 20;
    /** */
    public static final int RegExp = 27;
    /** */
    public static final int Size = 21;
    /** */
    public static final int SizeF = 22;
    /** */
    public static final int StringList = 11;
    /** */
    public static final int Time = 15;

    // Gui types
    /** */
    public static final int Font = 64;
    /** */
    public static final int Pixmap = 65;
    /** */
    public static final int Brush = 66;
    /** */
    public static final int Color = 67;
    /** */
    public static final int Palette = 68;
    /** */
    public static final int Icon = 69;
    /** */
    public static final int Image = 70;
    /** */
    public static final int Polygon = 71;
    /** */
    public static final int Region = 72;
    /** */
    public static final int Bitmap = 73;
    /** */
    public static final int Cursor = 74;
    /** */
    public static final int SizePolicy = 75;
    /** */
    public static final int KeySequence = 76;
    /** */
    public static final int Pen = 77;
    /** */
    public static final int TextLength = 78;
    /** */
    public static final int TextFormat = 79;
    /** */
    public static final int UserType = 127;


    /** @exclude */
    enum Types {
        Invalid(0),
        Double(6),
        String(10),
        Boolean(1),
        ByteArray(12),
        BitArray(13),
        Char(7),
        Date(14),
        DateTime(16),
        Int(2),
        Line(23),
        LineF(24),
        Locale(18),
        Long(4),
        Point(25),
        PointF(26),
        Rect(19),
        RectF(20),
        RegExp(27),
        Size(21),
        SizeF(22),
        StringList(11),
        Time(15);

        private int val;
        private Types(int val) { this.val = val; }
        public int value() { return val; }
    };

    private static native Object __qt_convert(int type, Object obj);
    private static native boolean __qt_canConvert(Object obj, int type);

    private static void setOk(Boolean ok[], boolean isOk)
    {
        if (ok != null && ok.length > 0)
            ok[0] = isOk;
    }

    public static boolean canConvertToDouble(Object obj)
    {
        if (obj instanceof Number)
            return true;
        else if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Double.value());
        else
            return false;
    }
    public static double toDouble(Object obj) { return toDouble(obj, (Boolean[]) null); }
    public static double toDouble(Object obj, Boolean ok[])
    {
        if (obj instanceof Number) {
            setOk(ok, true);
            return ((Number) obj).doubleValue();
        } else if (obj instanceof String || obj instanceof QtJambiObject) {
            return __qt_toDouble(obj, ok);
        } else {
            setOk(ok, false);
            return 0.0;
        }
    }
    private static native double __qt_toDouble(Object variant, Boolean ok[]);

    public static boolean canConvertToString(Object obj)
    {
        return obj != null;
    }

    /**
     * {@inheritDoc}
     */
    public static String toString(Object obj)
    {
        if (obj instanceof QtJambiObject) {
            return __qt_toString(obj);
        }
        return obj != null ? obj.toString() : "";
    }
    private static native String __qt_toString(Object variant);

    public static boolean canConvertToBoolean(Object obj)
    {
        if (obj instanceof Boolean || obj instanceof Number)
            return true;
        else if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Boolean.value());
        else
            return false;
    }
    public static boolean toBoolean(Object obj)
    {
        if (obj instanceof Boolean) {
            return (Boolean) obj;
        } else if (obj instanceof Number) {
            return ((Number) obj).intValue() != 0;
        } else if (obj instanceof QtJambiObject || obj instanceof String) {
            return __qt_toBoolean(obj);
        } else {
            return false;
        }
    }
    private static native boolean __qt_toBoolean(Object obj);

    public static boolean canConvertToByteArray(Object obj)
    {
        if (obj instanceof QtJambiObject)
            return __qt_canConvert(obj, Types.ByteArray.value());
        else
            return false;
    }
    public static QByteArray toByteArray(Object obj)
    {
        if (obj instanceof QtJambiObject) {
            QByteArray returned = (QByteArray) __qt_convert(Types.ByteArray.value(), obj);
            if (returned != null) {
                return returned;
            }
        }

        return new QByteArray();
    }

    public static boolean canConvertToBitArray(Object obj)
    {
        if (obj instanceof QtJambiObject)
            return __qt_canConvert(obj, Types.BitArray.value());
        else
            return false;
    }
    public static QBitArray toBitArray(Object obj)
    {
        if (obj instanceof QtJambiObject) {
            QBitArray returned = (QBitArray) __qt_convert(Types.BitArray.value(), obj);
            if (returned != null) {
                return returned;
            }
        }

        return new QBitArray();
    }

    public static boolean canConvertToChar(Object obj)
    {
        if (obj instanceof Character)
            return true;
        else if (obj instanceof QtJambiObject)
            return __qt_canConvert(obj, Types.Char.value());
        else
            return false;
    }
    public static char toChar(Object obj)
    {
        if (obj instanceof Character)
            return (Character) obj;
        else if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_toChar(obj);
        else
            return 0;
    }
    private static native char __qt_toChar(Object obj);

    public static boolean canConvertToDate(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Date.value());
        else
            return false;
    }
    public static QDate toDate(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QDate returned = (QDate) __qt_convert(Types.Date.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QDate();
    }

    public static boolean canConvertToDateTime(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.DateTime.value());
        else
            return false;
    }
    public static QDateTime toDateTime(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QDateTime returned = (QDateTime) __qt_convert(Types.DateTime.value(), obj);
            if (returned != null) {
                return returned;
            }

        }
        return new QDateTime();
    }

    public static boolean canConvertToInt(Object obj)
    {
        if (obj instanceof Number)
            return true;
        else if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Int.value());
        else
            return false;

    }
    public static int toInt(Object obj) { return toInt(obj, null); }
    public static int toInt(Object obj, Boolean ok[])
    {
        if (obj instanceof Number) {
            setOk(ok, true);
            return ((Number) obj).intValue();
        } else if (obj instanceof QtJambiObject || obj instanceof String) {
            return __qt_toInt(obj, ok);
        } else {
            setOk(ok, false);
            return 0;
        }
    }
    private static native int __qt_toInt(Object obj, Boolean ok[]);

    public static boolean canConvertToLine(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Line.value());
        else
            return false;
    }
    public static QLine toLine(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QLine returned = (QLine) __qt_convert(Types.Line.value(), obj);
            if (returned != null) {
                return returned;
            }

        }
        return new QLine();
    }
    public static boolean canConvertToLineF(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.LineF.value());
        else
            return false;
    }
    public static QLineF toLineF(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QLineF returned = (QLineF) __qt_convert(Types.LineF.value(), obj);
            if (returned != null) {
                return returned;
            }

        }
        return new QLineF();
    }

    public static boolean canConvertToLocale(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Locale.value());
        else
            return false;
    }
    public static QLocale toLocale(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QLocale returned = (QLocale) __qt_convert(Types.Locale.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QLocale();
    }

    public static boolean canConvertToPoint(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Point.value());
        else
            return false;
    }
    public static QPoint toPoint(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QPoint returned = (QPoint) __qt_convert(Types.Point.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QPoint();
    }

    public static boolean canConvertToPointF(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.PointF.value());
        else
            return false;
    }
    public static QPointF toPointF(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QPointF returned = (QPointF) __qt_convert(Types.PointF.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QPointF();
    }

    public static boolean canConvertToRect(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Rect.value());
        else
            return false;
    }
    public static QRect toRect(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QRect returned = (QRect) __qt_convert(Types.Rect.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QRect();
    }

    public static boolean canConvertToRectF(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.RectF.value());
        else
            return false;
    }
    public static QRectF toRectF(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QRectF returned = (QRectF) __qt_convert(Types.RectF.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QRectF();
    }

    public static boolean canConvertToRegExp(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.RegExp.value());
        else
            return false;
    }
    public static QRegExp toRegExp(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QRegExp returned = (QRegExp) __qt_convert(Types.RegExp.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QRegExp();
    }

    public static boolean canConvertToSize(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Size.value());
        else
            return false;
    }
    public static QSize toSize(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QSize returned = (QSize) __qt_convert(Types.Size.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QSize();
    }

    public static boolean canConvertToSizeF(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.SizeF.value());
        else
            return false;
    }
    public static QSizeF toSizeF(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QSizeF returned = (QSizeF) __qt_convert(Types.SizeF.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QSizeF();
    }

    public static boolean canConvertToTime(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Time.value());
        else
            return false;
    }
    public static QTime toTime(Object obj)
    {
        if (obj instanceof QtJambiObject || obj instanceof String) {
            QTime returned = (QTime) __qt_convert(Types.Time.value(), obj);
            if (returned != null) {
                return returned;
            }
        }
        return new QTime();
    }

    public static boolean canConvertToLong(Object obj)
    {
        if (obj instanceof Number)
            return true;
        else if (obj instanceof QtJambiObject || obj instanceof String)
            return __qt_canConvert(obj, Types.Long.value());
        else
            return false;
    }
    public static long toLong(Object obj) { return toLong(obj, null); }
    public static long toLong(Object obj, Boolean ok[])
    {
        if (obj instanceof Number) {
            setOk(ok, true);
            return ((Number) obj).longValue();
        } else if (obj instanceof QtJambiObject || obj instanceof String) {
            return __qt_toLong(obj, ok);
        } else {
            setOk(ok, false);
            return 0;
        }
    }
    private static native long __qt_toLong(Object obj, Boolean ok[]);

    public static boolean canConvertToList(Object obj)
    {
        return (obj instanceof Collection);
    }
    public static List<Object> toList(Object obj)
    {
        List<Object> list = new ArrayList<Object>();
        if (obj instanceof Collection)
            list.addAll((List<?>) obj);
        return list;
    }

    public static boolean canConvertToMap(Object obj)
    {
        return (obj instanceof Map);
    }
    public static Map<String, Object> toMap(Object obj)
    {
        Map<String, Object> map = new HashMap<String, Object>();
        if (obj instanceof Map) {
            Map<?, ?> otherMap = (Map<?, ?>) obj;
            for (Map.Entry<?, ?> e : otherMap.entrySet())
                map.put(e.getKey().toString(), e.getValue());
        }
        return map;
    }

}
