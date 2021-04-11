/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
** Copyright (C) 2009-2021 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static io.qt.internal.QtJambiInternal.resolveEnum;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import io.qt.QFlags;
import io.qt.QNoNativeResourcesException;
import io.qt.QtByteEnumerator;
import io.qt.QtEnumerator;
import io.qt.QtExtensibleEnum;
import io.qt.QtLongEnumerator;
import io.qt.QtObjectInterface;
import io.qt.QtShortEnumerator;
import io.qt.internal.QtJambiInternal;

/**
 * The QVariant class contains a set of static methods to convert between datatypes.
 * <p>
 * You can check with this class whether a datatype can be converted to another.
 * This is datatypes which cannot simply be cast to each other, but requires
 * processing to convert, i.e., you cannot use the instanceof operator as you would
 * for classes with the same super class.
*/
public final class QVariant {
	
	private QVariant(){}

	@QtExtensibleEnum
    public enum Type implements io.qt.QtEnumerator{
        Invalid(QMetaType.Type.UnknownType.value()),
        Boolean(QMetaType.Type.Bool.value()),
        Int(QMetaType.Type.Int.value()),
        Long(QMetaType.Type.LongLong.value()),
        Double(QMetaType.Type.Double.value()),
        Char(QMetaType.Type.QChar.value()),
        Map(QMetaType.Type.QVariantMap.value()),
        List(QMetaType.Type.QVariantList.value()),
        String(QMetaType.Type.QString.value()),
        StringList(QMetaType.Type.QStringList.value()),
        ByteArray(QMetaType.Type.QByteArray.value()),
        BitArray(QMetaType.Type.QBitArray.value()),
        Date(QMetaType.Type.QDate.value()),
        Time(QMetaType.Type.QTime.value()),
        DateTime(QMetaType.Type.QDateTime.value()),
        Url(QMetaType.Type.QUrl.value()),
        Locale(QMetaType.Type.QLocale.value()),
        Rect(QMetaType.Type.QRect.value()),
        RectF(QMetaType.Type.QRectF.value()),
        Size(QMetaType.Type.QSize.value()),
        SizeF(QMetaType.Type.QSizeF.value()),
        Line(QMetaType.Type.QLine.value()),
        LineF(QMetaType.Type.QLineF.value()),
        Point(QMetaType.Type.QPoint.value()),
        PointF(QMetaType.Type.QPointF.value()),
        RegExp(QMetaType.Type.QRegExp.value()),
        RegularExpression(QMetaType.Type.QRegularExpression.value()),
        Hash(QMetaType.Type.QVariantHash.value()),
        EasingCurve(QMetaType.Type.QEasingCurve.value()),
        Uuid(QMetaType.Type.QUuid.value()),
        ModelIndex(QMetaType.Type.QModelIndex.value()),
        PersistentModelIndex(QMetaType.Type.QPersistentModelIndex.value()),
        LastCoreType(QMetaType.Type.LastCoreType.value()),
        Font(QMetaType.Type.QFont.value()),
        Pixmap(QMetaType.Type.QPixmap.value()),
        Brush(QMetaType.Type.QBrush.value()),
        Color(QMetaType.Type.QColor.value()),
        Palette(QMetaType.Type.QPalette.value()),
        Image(QMetaType.Type.QImage.value()),
        Polygon(QMetaType.Type.QPolygon.value()),
        Region(QMetaType.Type.QRegion.value()),
        Bitmap(QMetaType.Type.QBitmap.value()),
        Cursor(QMetaType.Type.QCursor.value()),
        KeySequence(QMetaType.Type.QKeySequence.value()),
        Pen(QMetaType.Type.QPen.value()),
        TextLength(QMetaType.Type.QTextLength.value()),
        TextFormat(QMetaType.Type.QTextFormat.value()),
        Matrix(QMetaType.Type.QMatrix.value()),
        Transform(QMetaType.Type.QTransform.value()),
        Matrix4x4(QMetaType.Type.QMatrix4x4.value()),
        Vector2D(QMetaType.Type.QVector2D.value()),
        Vector3D(QMetaType.Type.QVector3D.value()),
        Vector4D(QMetaType.Type.QVector4D.value()),
        Quaternion(QMetaType.Type.QQuaternion.value()),
        PolygonF(QMetaType.Type.QPolygonF.value()),
        Icon(QMetaType.Type.QIcon.value()),
        LastGuiType(QMetaType.Type.LastGuiType.value()),
        SizePolicy(QMetaType.Type.QSizePolicy.value()),
        UserType(QMetaType.Type.User.value()),
        ;
        private int val;
        private Type(int val) { this.val = val; }
        public int value() { return val; }
        
        public static Type resolve(int value) {
            {
                return resolveEnum(Type.class, value, null);
            }
        }

        public static Type resolve(int value, String name) {
            if(name==null || name.isEmpty())
                return resolve(value);
            else
                return resolveEnum(Type.class, value, name);
        }
    };
    
    private static Class<?> dbusVariant;
    private static boolean dbusVariantResolved;
    
    private static boolean isDBusVariant(Object value, Class <?> cl) {
    	if(!dbusVariantResolved) {
    		dbusVariantResolved = true;
    		Class<?> _dbusVariant = null;
        	try {
        		_dbusVariant = Class.forName("io.qt.dbus.QDBusVariant");
        	} catch (Exception e) {
        	}
        	dbusVariant = _dbusVariant;
    	}
		return dbusVariant!=null && cl!=dbusVariant && dbusVariant.isInstance(value);
	}

    public static Object convert(Object obj, QMetaType.Type type) {
    	return convert(obj, type.value(), null);
    }
    public static Object convert(Object obj, QMetaType.Type type, boolean ok[]) {
    	return convert(obj, type.value(), ok);
    }
    public static Object convert(Object obj, Type type) {
    	return convert(obj, type.value(), null);
    }
    public static Object convert(Object obj, Type type, boolean ok[]) {
    	return convert(obj, type.value(), ok);
    }
    public static Object convert(Object obj, int userType) {
    	return convert(obj, userType, null);
    }
    
    private static native Object convert(Object obj, int userType, boolean ok[]);
    
    public static boolean canConvert(Object obj, QMetaType.Type type) {
    	return canConvert(obj, type.value());
    }
    public static boolean canConvert(Object obj, Type type) {
    	return canConvert(obj, type.value());
    }
    
    public static boolean canConvert(Object obj, int targetType) {
    	return QMetaType.javaType(targetType).isInstance(obj) || canConvertByType(type(obj), targetType);
    }
    
    private static native boolean canConvertByType(int sourceType, int targetType);
    
    public static int type(Object obj) {
    	if(obj==null)
    		return QMetaType.Type.Nullptr.value();
    	if(obj.getClass()==QList.class) {
    		return QMetaType.fromType(QList.class, ((QList<?>)obj).elementMetaType()).id();
    	}else if(obj.getClass()==QQueue.class) {
    		return QMetaType.fromType(QQueue.class, ((QQueue<?>)obj).elementMetaType()).id();
    	}else if(obj.getClass()==QStack.class) {
    		return QMetaType.fromType(QStack.class, ((QStack<?>)obj).elementMetaType()).id();
    	}else if(obj.getClass()==QVector.class) {
    		return QMetaType.fromType(QVector.class, ((QVector<?>)obj).elementMetaType()).id();
    	}else if(obj.getClass()==QSet.class) {
    		return QMetaType.fromType(QSet.class, ((QSet<?>)obj).elementMetaType()).id();
    	}else if(obj.getClass()==QLinkedList.class) {
    		return QMetaType.fromType(QLinkedList.class, ((QLinkedList<?>)obj).elementMetaType()).id();
    	}else if(obj.getClass()==QMultiMap.class) {
    		return QMetaType.fromType(QMultiMap.class, ((QMultiMap<?,?>)obj).keyMetaType(), ((QMultiMap<?,?>)obj).valueMetaType()).id();
    	}else if(obj.getClass()==QMap.class) {
    		return QMetaType.fromType(QMap.class, ((QMap<?,?>)obj).keyMetaType(), ((QMap<?,?>)obj).valueMetaType()).id();
    	}else if(obj.getClass()==QMultiHash.class) {
    		return QMetaType.fromType(QMultiHash.class, ((QMultiHash<?,?>)obj).keyMetaType(), ((QMultiHash<?,?>)obj).valueMetaType()).id();
    	}else if(obj.getClass()==QHash.class) {
    		return QMetaType.fromType(QHash.class, ((QHash<?,?>)obj).keyMetaType(), ((QHash<?,?>)obj).valueMetaType()).id();
    	}
    	return QMetaType.fromType(obj.getClass()).id();
    }

    private static void setOk(boolean ok[], boolean isOk)
    {
        if (ok != null && ok.length > 0)
            ok[0] = isOk;
    }

    public static boolean canConvertToDouble(Object obj)
    {
        return obj instanceof Number || canConvert(obj, Type.Double);
    }
    public static double toDouble(Object obj) { return toDouble(obj, null); }
    public static double toDouble(Object obj, boolean ok[])
    {
    	if (obj==null) {
            setOk(ok, true);
            return 0.0;
        } else if (obj instanceof Number) {
            setOk(ok, true);
            return ((Number) obj).doubleValue();
        } else {
            return __qt_toDouble(obj, ok);
        }
    }
    private static native double __qt_toDouble(Object variant, boolean ok[]);
    
    public static boolean canConvertToFloat(Object obj)
    {
        return obj instanceof Number || canConvert(obj, QMetaType.Type.Float);
    }
    public static float toFloat(Object obj) { return toFloat(obj, null); }
    public static float toFloat(Object obj, boolean ok[])
    {
    	if (obj==null) {
            setOk(ok, true);
            return 0.f;
        } else if (obj instanceof Number) {
            setOk(ok, true);
            return ((Number) obj).floatValue();
        } else {
            return __qt_toFloat(obj, ok);
        }
    }
    private static native float __qt_toFloat(Object variant, boolean ok[]);

    public static boolean canConvertToString(Object obj)
    {
        return true;
    }

    /**
     * {@inheritDoc}
     */
    public static String toString(Object obj)
    {
        return obj==null ? "" : __qt_toString(obj);
    }
    private static native String __qt_toString(Object variant);

    public static boolean canConvertToBoolean(Object obj)
    {
        return obj instanceof Boolean || obj instanceof Number || canConvert(obj, Type.Boolean);
    }
    public static boolean toBoolean(Object obj)
    {
    	if (obj==null) {
            return false;
        } else if (obj instanceof Boolean) {
            return (Boolean) obj;
        } else if (obj instanceof Number) {
            return ((Number) obj).intValue() != 0;
        } else {
            return __qt_toBoolean(obj);
        }
    }
    private static native boolean __qt_toBoolean(Object obj);

    public static boolean canConvertToByteArray(Object obj)
    {
        return obj instanceof QByteArray || canConvert(obj, Type.ByteArray);
    }
    public static QByteArray toByteArray(Object obj)
    {
    	if(obj instanceof QByteArray) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QByteArray)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.ByteArray);
	        if (returned instanceof QByteArray) {
	            return (QByteArray)returned;
	        }
    	}
        return new QByteArray();
    }

    public static boolean canConvertToBitArray(Object obj)
    {
        return obj instanceof QBitArray || canConvert(obj, Type.BitArray);
    }
    public static QBitArray toBitArray(Object obj)
    {
    	if(obj instanceof QBitArray) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QBitArray)obj;
    	}else if(obj!=null){
            Object returned = convert(obj, Type.BitArray);
            if (returned instanceof QBitArray) {
                return (QBitArray)returned;
            }    		
    	}
        return new QBitArray();
    }

    public static boolean canConvertToChar(Object obj)
    {
        return obj instanceof Character || canConvert(obj, Type.Char);
    }
    public static char toChar(Object obj)
    {
    	if(obj==null)
    		return 0;
    	else if (obj instanceof Character)
            return (Character) obj;
        else
            return __qt_toChar(obj);
    }
    private static native char __qt_toChar(Object obj);

    public static boolean canConvertToDate(Object obj)
    {
        return obj instanceof QDate || canConvert(obj, Type.Date);
    }
    public static QDate toDate(Object obj)
    {
    	if(obj instanceof QDate) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QDate)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.Date);
	        if (returned instanceof QDate) {
	            return (QDate)returned;
	        }
    	}
        return new QDate();
    }

    public static boolean canConvertToDateTime(Object obj)
    {
        return obj instanceof QDateTime || canConvert(obj, Type.DateTime);
    }
    public static QDateTime toDateTime(Object obj)
    {
    	if(obj instanceof QDateTime) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QDateTime)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.DateTime);
	        if (returned instanceof QDateTime) {
	            return (QDateTime)returned;
	        }
    	}
        return new QDateTime();
    }

    public static boolean canConvertToInt(Object obj)
    {
        return obj instanceof Number || canConvert(obj, Type.Int);
    }
    public static int toInt(Object obj) { return toInt(obj, null); }
    public static int toInt(Object obj, boolean ok[])
    {
        if(obj==null) {
        	return 0;
        }else if (obj instanceof Number) {
            setOk(ok, true);
            return ((Number) obj).intValue();
        } else {
            return __qt_toInt(obj, ok);
        }
    }
    private static native int __qt_toInt(Object obj, boolean ok[]);

    public static boolean canConvertToLine(Object obj)
    {
        return obj instanceof QLine || canConvert(obj, Type.Line);
    }
    public static QLine toLine(Object obj)
    {
    	if(obj instanceof QLine) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QLine)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.Line);
	        if (returned instanceof QLine) {
	            return (QLine) returned;
	        }
    	}
        return new QLine();
    }
    public static boolean canConvertToLineF(Object obj)
    {
        return obj instanceof QLineF || canConvert(obj, Type.LineF);
    }
    public static QLineF toLineF(Object obj)
    {
    	if(obj instanceof QLineF) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QLineF)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.LineF);
	        if (returned instanceof QLineF) {
	            return (QLineF)returned;
	        }
    	}
        return new QLineF();
    }

    public static boolean canConvertToLocale(Object obj)
    {
        return obj instanceof QLocale || canConvert(obj, Type.Locale);
    }
    public static QLocale toLocale(Object obj)
    {
    	if(obj instanceof QLocale) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QLocale)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.Locale);
	        if (returned instanceof QLocale) {
	            return (QLocale)returned;
	        }
    	}
        return new QLocale();
    }

    public static boolean canConvertToPoint(Object obj)
    {
        return obj instanceof QPoint || canConvert(obj, Type.Point);
    }
    public static QPoint toPoint(Object obj)
    {
    	if(obj instanceof QPoint) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QPoint)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.Point);
	        if (returned instanceof QPoint) {
	            return (QPoint) returned;
	        }
    	}
        return new QPoint();
    }

    public static boolean canConvertToPointF(Object obj)
    {
        return obj instanceof QPointF || canConvert(obj, Type.PointF);
    }
    public static QPointF toPointF(Object obj)
    {
    	if(obj instanceof QPointF) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QPointF)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.PointF);
	        if (returned instanceof QPointF) {
	            return (QPointF)returned;
	        }
    	}
        return new QPointF();
    }

    public static boolean canConvertToRect(Object obj)
    {
        return obj instanceof QRect || canConvert(obj, Type.Rect);
    }
    public static QRect toRect(Object obj)
    {
    	if(obj instanceof QRect) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QRect)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.Rect);
	        if (returned instanceof QRect) {
	            return (QRect)returned;
	        }
    	}
        return new QRect();
    }

    public static boolean canConvertToRectF(Object obj)
    {
        return obj instanceof QRectF || canConvert(obj, Type.RectF);
    }
    public static QRectF toRectF(Object obj)
    {
    	if(obj instanceof QRectF) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QRectF)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.RectF);
	        if (returned instanceof QRectF) {
	            return (QRectF)returned;
	        }
    	}
        return new QRectF();
    }

    public static boolean canConvertToRegExp(Object obj)
    {
        return obj instanceof QRegExp || canConvert(obj, Type.RegExp);
    }
    public static QRegExp toRegExp(Object obj)
    {
    	if(obj instanceof QRegExp) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QRegExp)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.RegExp);
	        if (returned instanceof QRegExp) {
	            return (QRegExp)returned;
	        }
    	}
        return new QRegExp();
    }

    public static boolean canConvertToRegularExpression(Object obj)
    {
        return obj instanceof QRegularExpression || canConvert(obj, Type.RegularExpression);
    }
    public static QRegularExpression toRegularExpression(Object obj)
    {
    	if(obj instanceof QRegularExpression) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QRegularExpression)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.RegularExpression);
	        if (returned instanceof QRegularExpression) {
	            return (QRegularExpression)returned;
	        }
    	}
        return new QRegularExpression();
    }

    public static boolean canConvertToSize(Object obj)
    {
        return obj instanceof QSize || canConvert(obj, Type.Size);
    }
    public static QSize toSize(Object obj)
    {
    	if(obj instanceof QSize) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QSize)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.Size);
	        if (returned instanceof QSize) {
	            return (QSize)returned;
	        }
    	}
        return new QSize();
    }

    public static boolean canConvertToSizeF(Object obj)
    {
        return obj instanceof QSizeF || canConvert(obj, Type.SizeF);
    }
    public static QSizeF toSizeF(Object obj)
    {
    	if(obj instanceof QSizeF) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QSizeF)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.SizeF);
	        if (returned instanceof QSizeF) {
	            return (QSizeF)returned;
	        }
    	}
        return new QSizeF();
    }

    public static boolean canConvertToTime(Object obj)
    {
        return obj instanceof QTime || canConvert(obj, Type.Time);
    }
    public static QTime toTime(Object obj)
    {
    	if(obj instanceof QTime) {
    		if(!((QtObjectInterface)obj).isDisposed())
    			return (QTime)obj;
    	}else if(obj!=null){
	        Object returned = convert(obj, Type.Time);
	        if (returned instanceof QTime) {
	            return (QTime)returned;
	        }
    	}
        return new QTime();
    }

    public static boolean canConvertToLong(Object obj)
    {
        return obj instanceof Number || canConvert(obj, Type.Long);
    }
    public static long toLong(Object obj) { return toLong(obj, null); }
    public static long toLong(Object obj, boolean ok[])
    {
    	if(obj==null) {
    		setOk(ok, true);
    		return 0;
    	}else if (obj instanceof Number) {
            setOk(ok, true);
            return ((Number) obj).longValue();
        } else {
            return __qt_toLong(obj, ok);
        }
    }
    private static native long __qt_toLong(Object obj, boolean ok[]);

    public static boolean canConvertToList(Object obj)
    {
    	return obj instanceof Collection || canConvert(obj, Type.List);
    }
    public static List<?> toList(Object obj)
    {
        if (obj instanceof Collection){
            if (obj instanceof List)
                return (List<?>)obj;
            List<Object> list = new ArrayList<Object>();
                list.addAll((Collection<?>) obj);
            return list;
        }else if(obj!=null) {
        	Object returned = convert(obj, Type.List);
	        if (returned instanceof List) {
	            return (List<?>)returned;
	        }
        }
        return Collections.emptyList();
    }

    public static boolean canConvertToStringList(Object obj)
    {
    	return obj instanceof Collection || canConvert(obj, Type.StringList);
    }
    @SuppressWarnings("unchecked")
	public static List<String> toStringList(Object obj)
    {
        if (obj instanceof Collection){
            if (obj instanceof List) {
            	boolean fail = false;
            	for(Object entry : (List<?>)obj) {
            		if(!(entry instanceof String)) {
            			fail = true;
            			break;
            		}
            	}
            	if(fail)
            		return (List<String>)obj;
            }
            List<String> list = new ArrayList<String>();
            for(Object entry : (Collection<?>)obj) {
            	list.add(entry==null ? null : entry.toString());
            }
            return list;
        }else if(obj!=null) {
        	Object returned = convert(obj, Type.StringList);
	        if (returned instanceof List) {
	            return (List<String>)returned;
	        }
        }
        return Collections.emptyList();
    }

    public static boolean canConvertToCollection(Object obj)
    {
        return obj instanceof Collection || canConvert(obj, Type.List);
    }
    public static Collection<?> toCollection(Object obj)
    {
        if (obj instanceof Collection){
            return (Collection<?>)obj;
        }else if(obj!=null) {
        	Object returned = convert(obj, Type.List);
	        if (returned instanceof Collection) {
	            return (Collection<?>)returned;
	        }
        }
        return Collections.emptyList();
    }

    public static boolean canConvertToMap(Object obj)
    {
    	return obj instanceof Map || canConvert(obj, Type.Hash);
    }

    @SuppressWarnings("unchecked")
    public static Map<String,?> toMap(Object obj)
    {
        if (obj instanceof Map) {
            Map<?, ?> otherMap = (Map<?, ?>) obj;
            boolean convert = false;
            for (Map.Entry<?, ?> e : otherMap.entrySet()){
                if(!(e.getKey() instanceof String)){
                    convert = true;
                    break;
                }
            }
            if(convert){
                Map<String, Object> map = new TreeMap<String, Object>();
                for (Map.Entry<?, ?> e : otherMap.entrySet())
                    map.put(e.getKey().toString(), e.getValue());
                return map;
            }else{
                return (Map<String,?>) obj;
            }
        }else if(obj!=null) {
        	Object returned = convert(obj, Type.Hash);
        	if(returned instanceof Map) {
	        	Map<?, ?> otherMap = (Map<?, ?>) returned;
	            boolean convert = false;
	            for (Map.Entry<?, ?> e : otherMap.entrySet()){
	                if(!(e.getKey() instanceof String)){
	                    convert = true;
	                    break;
	                }
	            }
	            if(convert){
	                Map<String, Object> map = new TreeMap<String, Object>();
	                for (Map.Entry<?, ?> e : otherMap.entrySet())
	                    map.put(e.getKey().toString(), e.getValue());
	                return map;
	            }else{
	                return (Map<String,?>) returned;
	            }
        	}
        }
        return Collections.emptyMap();
    }
    
    @SuppressWarnings("unchecked")
    public static <T> T convert(Object value, Class < T > cl) {
        if(cl.isInstance(value)) {
        	if(value instanceof QtObjectInterface) {
        		if(!((QtObjectInterface)value).isDisposed()) {
        			return cl.cast(value);
        		}
        	}else {
        		return cl.cast(value);
        	}
        }
        if(isDBusVariant(value, cl)){
        	int type = QMetaType.metaTypeId(cl);
        	return convert(convert(value, type), cl);
        }
        
        if(cl==int.class){
            if(value instanceof Integer)
                return (T)value;
            boolean[] ok = {false};
            value = QVariant.toInt(value, ok);
            if(!ok[0]){
                throw new NumberFormatException("Cannot convert to "+cl.getName());
            }
            return (T)value;
        }else if(cl==byte.class){
            if(value instanceof Byte)
                return (T)value;
            boolean[] ok = {false};
            value = (byte)QVariant.toInt(value, ok);
            if(!ok[0]){
                throw new NumberFormatException("Cannot convert to "+cl.getName());
            }
            return (T)value;
        }else if(cl==short.class){
            if(value instanceof Short)
                return (T)value;
            boolean[] ok = {false};
            value = (short)QVariant.toInt(value, ok);
            if(!ok[0]){
                throw new NumberFormatException("Cannot convert to "+cl.getName());
            }
            return (T)value;
        }else if(cl==long.class){
            if(value instanceof Long)
                return (T)value;
            boolean[] ok = {false};
            value = QVariant.toLong(value, ok);
            if(!ok[0]){
                throw new NumberFormatException("Cannot convert to "+cl.getName());
            }
            return (T)value;
        }else if(cl==double.class){
            if(value instanceof Double)
                return (T)value;
            boolean[] ok = {false};
            value = QVariant.toDouble(value, ok);
            if(!ok[0]){
                throw new NumberFormatException("Cannot convert to "+cl.getName());
            }
            return (T)value;
        }else if(cl==float.class){
            if(value instanceof Float)
                return (T)value;
            boolean[] ok = {false};
            value = (float)QVariant.toDouble(value, ok);
            if(!ok[0]){
                throw new NumberFormatException("Cannot convert to "+cl.getName());
            }
            return (T)value;
        }else if(cl==char.class){
            if(value instanceof Character)
                return (T)value;
            if(!QVariant.canConvertToChar(value)){
                throw new NumberFormatException("Cannot convert to "+cl.getName());
            }
            value = QVariant.toChar(value);
            return (T)value;
        }else if(cl==boolean.class){
            if(value instanceof Boolean)
                return (T)value;
            if(!QVariant.canConvertToBoolean(value)){
                throw new NumberFormatException("Cannot convert to "+cl.getName());
            }
            value = QVariant.toBoolean(value);
            return (T)value;
        }else if(cl==QBitArray.class){
            if(value instanceof QBitArray)
                return cl.cast(value);
            if(QVariant.canConvertToBitArray(value)){
            	value = QVariant.toBitArray(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QByteArray.class){
            if(value instanceof QByteArray)
                return cl.cast(value);
            if(QVariant.canConvertToByteArray(value)){
            	value = QVariant.toByteArray(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QDate.class){
            if(value instanceof QDate)
                return cl.cast(value);
            if(QVariant.canConvertToDate(value)){
            	value = QVariant.toDate(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QDateTime.class){
            if(value instanceof QDateTime)
                return cl.cast(value);
            if(QVariant.canConvertToDateTime(value)){
            	value = QVariant.toDateTime(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QTime.class){
            if(value instanceof QTime)
                return cl.cast(value);
            if(QVariant.canConvertToTime(value)){
            	value = QVariant.toTime(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QLine.class){
            if(value instanceof QLine)
                return cl.cast(value);
            if(QVariant.canConvertToLine(value)){
            	value = QVariant.toLine(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==List.class){
            if(value instanceof List)
                return cl.cast(value);
            if(QVariant.canConvertToList(value)){
            	value = QVariant.toList(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QLocale.class){
            if(value instanceof QLocale)
                return cl.cast(value);
            if(QVariant.canConvertToLocale(value)){
            	value = QVariant.toLocale(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QPoint.class){
            if(value instanceof QPoint)
                return cl.cast(value);
            if(QVariant.canConvertToPoint(value)){
            	value = QVariant.toPoint(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==Map.class){
            if(value instanceof Map)
                return cl.cast(value);
            if(QVariant.canConvertToMap(value)){
            	value = QVariant.toMap(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QRect.class){
            if(value instanceof QRect)
                return cl.cast(value);
            if(QVariant.canConvertToRect(value)){
            	value = QVariant.toRect(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QRectF.class){
            if(value instanceof QRectF)
                return cl.cast(value);
            if(QVariant.canConvertToRectF(value)){
            	value = QVariant.toRectF(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QSize.class){
            if(value instanceof QSize)
                return cl.cast(value);
            if(QVariant.canConvertToSize(value)){
            	value = QVariant.toSize(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QSizeF.class){
            if(value instanceof QSizeF)
                return cl.cast(value);
            if(QVariant.canConvertToSizeF(value)){
            	value = QVariant.toSizeF(value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==QRegExp.class){
            if(value instanceof QRegExp)
                return cl.cast(value);
            if(QVariant.canConvertToRegExp(value)){
            	value = QVariant.toRegExp(value);
                return cl.cast(value);
            }
            return null;
        }else if(Enum.class.isAssignableFrom(cl)){
            if(QVariant.canConvertToEnum((Class<? extends Enum<?>>)cl, value)){
            	value = QVariant.toEnum((Class<? extends Enum<?>>)cl, value);
                return cl.cast(value);
            }
            return null;
        }else if(cl==String.class){
            return value!=null ? cl.cast(value.toString()) : cl.cast("");
        }
        int type = QMetaType.metaTypeId(cl);
        if(canConvert(value, type)) {
        	return cl.cast(convert(value, type));
        }
        return cl.cast(value);
    }

	public static <E extends Enum<?>> boolean canConvertToEnum(Class<E> enumClass, Object obj)
    {
        if(enumClass.isInstance(obj)) {
            return true;
        }else if(QtEnumerator.class.isAssignableFrom(enumClass) 
                || QtByteEnumerator.class.isAssignableFrom(enumClass) 
                || QtShortEnumerator.class.isAssignableFrom(enumClass)) {
            return canConvertToInt(obj);
        }else if(QtLongEnumerator.class.isAssignableFrom(enumClass)) {
            return canConvertToLong(obj);
        }else if (canConvertToInt(obj)) {
            boolean[] ok = {false};
            int value = toInt(obj, ok);
            E[] constants = enumClass.getEnumConstants();
            return constants!=null && ok[0] && value<constants.length;
        }else return false;
    }
    
	public static boolean canConvertToFlags(Object obj)
    {
		return obj instanceof QFlags || canConvertToInt(obj);
    }
	
	public static <E extends Enum<?>> E toEnum(Class<E> enumClass, Object obj) {
		return toEnum(enumClass, obj, (boolean[])null);
	}
	
	public static <E extends Enum<?>> E toEnum(Class<E> enumClass, Object obj, boolean ok[]) {
		if(enumClass.isInstance(obj)) {
			try {
				return enumClass.cast(obj);
			} catch (Exception e) {
			}
		}else if(QtEnumerator.class.isAssignableFrom(enumClass)) {
    		try {
    			Method resolveMethod = enumClass.getMethod("resolve", int.class);
    			int i = toInt(obj, ok);
    			if(ok==null || ok[0])
    				return enumClass.cast(resolveMethod.invoke(null, java.lang.Integer.valueOf(i)));
			} catch (Exception e) {
			}
    	}else if(QtByteEnumerator.class.isAssignableFrom(enumClass)) {
    		try {
    			Method resolveMethod = enumClass.getMethod("resolve", byte.class);
    			byte i = (byte)toInt(obj, ok);
    			if(ok==null || ok[0])
    				return enumClass.cast(resolveMethod.invoke(null, java.lang.Byte.valueOf(i)));
			} catch (Exception e) {
			}
    	}else if(QtShortEnumerator.class.isAssignableFrom(enumClass)) {
    		try {
    			Method resolveMethod = enumClass.getMethod("resolve", short.class);
    			short i = (short)toInt(obj, ok);
    			if(ok==null || ok[0])
    				return enumClass.cast(resolveMethod.invoke(null, java.lang.Short.valueOf(i)));
			} catch (Exception e) {
			}
    	}else if(QtLongEnumerator.class.isAssignableFrom(enumClass)) {
    		try {
    			Method resolveMethod = enumClass.getMethod("resolve", long.class);
    			long l = toLong(obj, ok);
    			if(ok==null || ok[0])
    				return enumClass.cast(resolveMethod.invoke(null, java.lang.Long.valueOf(l)));
			} catch (Exception e) {
			}
    	}else if (canConvertToInt(obj)) {
        	int value = toInt(obj, ok);
        	E[] constants = enumClass.getEnumConstants();
        	if(constants!=null && (ok==null || ok[0]  && value<constants.length)) {
        		return constants[value];
        	}
    		
    	}
		if(ok!=null && ok.length>0)
			ok[0] = false;
		return null;
	}
	
	public static <F extends QFlags<?>> F toFlags(Class<F> flagsClass, Object obj) {
		return toFlags(flagsClass, obj, null);
	}
	
	public static <F extends QFlags<?>> F toFlags(Class<F> flagsClass, Object obj, boolean ok[]) {
		if(flagsClass.isInstance(obj)) {
        	try{
				return flagsClass.cast(obj);
			} catch (Exception e) {
			}
		}else if (canConvertToInt(obj)) {
        	int value = toInt(obj, ok);
        	try{
        		return flagsClass.getConstructor(int.class).newInstance(value);
        	} catch (Exception e) {
			}
    	}
		if(ok!=null && ok.length>0)
			ok[0] = false;
		return null;
	}
	
	public static void saveObject(QDataStream stream, Object variant){
		saveObject(stream, variant, null);
	}
	
	public static void saveObject(QDataStream stream, Object variant, Boolean[] ok){
		long nativeId = QtJambiInternal.nativeId(stream);
		if (nativeId == 0)
			throw new QNoNativeResourcesException("Function call on incomplete object of type: " +stream.getClass().getName());
		__qt_saveObject(nativeId, variant, ok);
	}
	
	public static Object loadObject(QDataStream stream){
		return loadObject(stream, null);
	}
	
	public static Object loadObject(QDataStream stream, Boolean[] ok){
		long nativeId = QtJambiInternal.nativeId(stream);
		if (nativeId == 0)
			throw new QNoNativeResourcesException("Function call on incomplete object of type: " +stream.getClass().getName());
		return __qt_loadObject(nativeId, ok);
	}
	
	private static native void __qt_saveObject(long stream_nativeId, Object variant, Boolean[] ok);
	
	private static native Object __qt_loadObject(long stream_nativeId, Boolean[] ok);
}
