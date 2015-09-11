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

package org.qtjambi.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.junit.Test;

import org.qtjambi.autotests.generated.Variants;

import org.qtjambi.qt.QVariant;
import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.core.QBitArray;
import org.qtjambi.qt.core.QByteArray;
import org.qtjambi.qt.core.QDate;
import org.qtjambi.qt.core.QDateTime;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QPointF;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.QRegExp;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.QSizeF;
import org.qtjambi.qt.core.QTime;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.widgets.QGraphicsItem;
import org.qtjambi.qt.widgets.QGraphicsPixmapItem;

public class TestQVariant extends QApplicationTest {
    @Test
    public void run_QVariantString() {
        Object variant = "this is my string";
        String expectedString = "this is my string";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("Normal string", variant, expectedDouble, true, expectedString, true, expectedByteArray, false, expectedInt, true, expectedBool, true, expectedBitArray, false, expectedChar,
                false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false, expectedRegExp, false,
                expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantByteArray() {
        Object variant = new QByteArray("this is my string");
        String expectedString = "this is my string";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("this is my string");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("QByteArray", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantObject() {
        Object variant = new QObject();
        String expectedString = "";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 0;
        boolean expectedBool = false;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("java.lang.Object", variant, expectedDouble, false, expectedString, true, expectedByteArray, false, expectedInt, false, expectedBool, false,
                expectedBitArray, false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantStringDouble() {
        Object variant = "123.456";
        String expectedString = "123.456";
        double expectedDouble = 123.456;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("String double", variant, expectedDouble, true, expectedString, true, expectedByteArray, false, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantByteArrayDouble() {
        Object variant = new QByteArray("456.789");;
        String expectedString = "456.789";
        double expectedDouble = 456.789;
        QByteArray expectedByteArray = new QByteArray("456.789");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("Byte array double", variant, expectedDouble, true, expectedString, true, expectedByteArray, true, expectedInt, true, expectedBool, true,
                expectedBitArray, false, expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantStringInteger() {
        Object variant = "321";
        String expectedString = "321";
        double expectedDouble = 321.0;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 321;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("String integer", variant, expectedDouble, true, expectedString, true, expectedByteArray, false, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantStringBooleanFalse() {
        Object variant = "FaLsE";
        String expectedString = "FaLsE";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 0;
        boolean expectedBool = false;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("String boolean", variant, expectedDouble, true, expectedString, true, expectedByteArray, false, expectedInt, true, expectedBool, true, expectedBitArray,
                false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantBooleanTrue() {
        Object variant = "tRUe";;
        String expectedString = "tRUe";;
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 0;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("String boolean (true)", variant, expectedDouble, true, expectedString, true, expectedByteArray, false, expectedInt, true, expectedBool, true,
                expectedBitArray, false, expectedChar, false, expectedDate, true, expectedTime, true, expectedDateTime, true, expectedPoint, false, expectedPointF, false, expectedRect, false,
                expectedRectF, false, expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantDouble() {
        Object variant = 123.567;
        String expectedString = "123.567";
        double expectedDouble = 123.567;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 123;
        boolean expectedBool = true;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime(expectedDate, expectedTime);
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("Double", variant, expectedDouble, true, expectedString, true, expectedByteArray, false, expectedInt, true, expectedBool, true, expectedBitArray, false,
                expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);
    }

    @Test
    public void run_QVariantNull() {
        Object variant = null;
        String expectedString = "";
        double expectedDouble = 0.0;
        QByteArray expectedByteArray = new QByteArray("");
        int expectedInt = 0;
        boolean expectedBool = false;
        QBitArray expectedBitArray = new QBitArray();
        char expectedChar = 0;
        QDate expectedDate = new QDate();
        QTime expectedTime = new QTime();
        QDateTime expectedDateTime = new QDateTime();
        QPoint expectedPoint = new QPoint();
        QPointF expectedPointF = new QPointF();
        QRect expectedRect = new QRect();
        QRectF expectedRectF = new QRectF();
        QRegExp expectedRegExp = new QRegExp();
        QSize expectedSize = new QSize();
        QSizeF expectedSizeF = new QSizeF();
        List<Object> expectedList = new ArrayList<Object>();
        Map<String, Object> expectedMap = new HashMap<String, Object>();

        testQVariant("Null", variant, expectedDouble, false, expectedString, false, expectedByteArray, false, expectedInt, false, expectedBool, false, expectedBitArray, false,
                expectedChar, false, expectedDate, false, expectedTime, false, expectedDateTime, false, expectedPoint, false, expectedPointF, false, expectedRect, false, expectedRectF, false,
                expectedRegExp, false, expectedSize, false, expectedSizeF, false, expectedList, false, expectedMap, false);

    }

    public void testQVariant(String name, Object object, Double expectedDouble, Boolean canConvertDouble, String expectedString, Boolean canConvertString, QByteArray expectedByteArray,
            Boolean canConvertByteArray, Integer expectedInt, Boolean canConvertInt, Boolean expectedBool, Boolean canConvertBool, QBitArray expectedBitArray, Boolean canConvertBitArray,
            Character expectedChar, Boolean canConvertChar, QDate expectedDate, Boolean canConvertDate, QTime expectedTime, Boolean canConvertTime, QDateTime expectedDateTime,
            Boolean canConvertDateTime, QPoint expectedPoint, Boolean canConvertPoint, QPointF expectedPointF, Boolean canConvertPointF, QRect expectedRect, Boolean canConvertRect,
            QRectF expectedRectF, Boolean canConvertRectF, QRegExp expectedRegExp, Boolean canConvertRegExp, QSize expectedSize, Boolean canConvertSize, QSizeF expectedSizeF, Boolean canConvertSizeF,
            List<?> expectedList, Boolean canConvertList, Map<?, ?> expectedMap, Boolean canConvertMap) {

        assertEquals(QVariant.toString(object), expectedString);
        assertEquals((double)QVariant.toDouble(object), (double)expectedDouble, 0.0);
        assertEquals((int)QVariant.toInt(object), (double)expectedInt, 0.0);
        assertEquals(QVariant.toBoolean(object), expectedBool);
        assertTrue(QVariant.toBitArray(object).equals(expectedBitArray));
        assertEquals((char)QVariant.toChar(object), (char)expectedChar);
        assertTrue(QVariant.toDate(object).equals(expectedDate));
        assertTrue(QVariant.toTime(object).equals(expectedTime));
        assertTrue(QVariant.toDateTime(object).equals(expectedDateTime));
        QVariant.toDateTime(object);
        assertEquals(QVariant.toPoint(object).x(), expectedPoint.x());
        assertEquals(QVariant.toPoint(object).y(), expectedPoint.y());
        assertEquals(QVariant.toPointF(object).x(), expectedPointF.x(), 0.0);
        assertEquals(QVariant.toPointF(object).y(), expectedPointF.y(), 0.0);
        assertEquals(QVariant.toRect(object).left(), expectedRect.left());
        assertEquals(QVariant.toRect(object).top(), expectedRect.top());
        assertEquals(QVariant.toRect(object).right(), expectedRect.right());
        assertEquals(QVariant.toRect(object).bottom(), expectedRect.bottom());
        assertEquals(QVariant.toRectF(object).left(), expectedRectF.left(), 0.0);
        assertEquals(QVariant.toRectF(object).top(), expectedRectF.top(), 0.0);
        assertEquals(QVariant.toRectF(object).bottom(), expectedRectF.bottom(), 0.0);
        assertEquals(QVariant.toRectF(object).right(), expectedRectF.right(), 0.0);
        assertTrue(QVariant.toRegExp(object).equals(expectedRegExp));
        assertEquals(QVariant.toSize(object).width(), expectedSize.width());
        assertEquals(QVariant.toSize(object).height(), expectedSize.height());
        assertEquals(QVariant.toSizeF(object).width(), expectedSizeF.width(), 0.0);
        assertEquals(QVariant.toSizeF(object).height(), expectedSizeF.height(), 0.0);
        assertEquals(QVariant.toList(object), expectedList);
        assertEquals(QVariant.toMap(object), expectedMap);
        {
            QByteArray ba = QVariant.toByteArray(object);
            assertEquals(ba.size(), expectedByteArray.size());
            for (int i = 0; i < ba.size(); ++i) {
                assertEquals(ba.at(i), expectedByteArray.at(i));
            }
        }

        assertEquals(QVariant.canConvertToString(object), canConvertString);
        assertEquals(QVariant.canConvertToDouble(object), canConvertDouble);
        assertEquals(QVariant.canConvertToByteArray(object), canConvertByteArray);
        assertEquals(QVariant.canConvertToInt(object), canConvertInt);
        assertEquals(QVariant.canConvertToBoolean(object), canConvertBool);
        assertEquals(QVariant.canConvertToBitArray(object), canConvertBitArray);
        assertEquals(QVariant.canConvertToChar(object), canConvertChar);
        assertEquals(QVariant.canConvertToDate(object), canConvertDate);
        assertEquals(QVariant.canConvertToTime(object), canConvertTime);
        assertEquals(QVariant.canConvertToDateTime(object), canConvertDateTime);
        assertEquals(QVariant.canConvertToPoint(object), canConvertPoint);
        assertEquals(QVariant.canConvertToPointF(object), canConvertPointF);
        assertEquals(QVariant.canConvertToRect(object), canConvertRect);
        assertEquals(QVariant.canConvertToRectF(object), canConvertRectF);
        assertEquals(QVariant.canConvertToRegExp(object), canConvertRegExp);
        assertEquals(QVariant.canConvertToSize(object), canConvertSize);
        assertEquals(QVariant.canConvertToSizeF(object), canConvertSizeF);
        assertEquals(QVariant.canConvertToList(object), canConvertList);
        assertEquals(QVariant.canConvertToMap(object), canConvertMap);
    }

    @Test
    public void run_QByteArray_toString() {
        byte data[] = {'a', 'e', 'r', 'y', 'n'};

        // Normal construction
        assertEquals(new QByteArray(data).toString(), "aeryn");

        // String construction
        assertEquals(new QByteArray("aeryn").toString(), "aeryn");

        // Using int, char constructor
        assertEquals(new QByteArray(5, (byte) 'a').toString(), "aaaaa");
    }

    private class DataPrimitive {
        public Object value;
        public String methodName;

        public DataPrimitive(Object value, String methodName) {
            this.value = value;
            this.methodName = methodName;
        }
    }

    @Test
    public void run_primitives() {
        DataPrimitive[] data = { new DataPrimitive(new Long(3), "currentQInt64"),
                new DataPrimitive(new Long(4), "currentQUInt64"), new DataPrimitive(new Integer(5), "currentQInt32"),
                new DataPrimitive(new Integer(6), "currentQUInt32"),
                new DataPrimitive(new Short((short) 7), "currentQInt16"),
                new DataPrimitive(new Short((short) 8), "currentQUInt16"),
                new DataPrimitive(new Byte((byte) 9), "currentQInt8"),
                new DataPrimitive(new Byte((byte) 10), "currentQUInt8"),
                new DataPrimitive(new Float(11), "currentFloat"),
                new DataPrimitive(new Double(12), "currentDouble") };

        for (int i = 0; i < data.length; i++) {
            Object value = data[i].value;
            String methodName = data[i].methodName;
            Variants v = new Variants();
            Object returned = v.pushThrough(value);
            assertTrue(v.isValid());
            Object nativlySet = null;

            try {
                Method method = Variants.class.getMethod(methodName);
                nativlySet = method.invoke(v);
            } catch (Exception e) {
                System.err.println("Failed to call: " + methodName);
                e.printStackTrace();
            }
            assertEquals(nativlySet, value);
            assertEquals(returned, value);
        }
    }

    @Test
    public void run_primitives2() {
        Variants v = new Variants();

        int value = 1;

        long the_long = ++value;
        v.pushThrough(the_long);
        assertEquals(v.currentQInt64(), the_long);

        long the_ulong = ++value;
        v.pushThrough(the_ulong);
        assertEquals(v.currentQUInt64(), the_ulong);

        int the_int = ++value;
        v.pushThrough(the_int);
        assertEquals(v.currentQInt32(), the_int);

        int the_uint = ++value;
        v.pushThrough(the_uint);
        assertEquals(v.currentQUInt32(), the_uint);

        char the_short = (char) ++value;
        v.pushThrough(the_short);

        assertEquals(v.currentQInt16(), the_short);

        char the_ushort = (char) ++value;
        v.pushThrough(the_ushort);
        assertEquals(v.currentQUInt16(), the_ushort);

        byte the_byte = (byte) ++value;
        v.pushThrough(the_byte);
        assertEquals(v.currentQInt8(), the_byte);

        byte the_ubyte = (byte) ++value;
        v.pushThrough(the_ubyte);
        assertEquals(v.currentQUInt8(), the_ubyte);
    }

    @Test
    public void run_JObjectWrapper_test() {
        Variants v = new Variants();

        v.pushThrough(new Object() {
            @Override
            public String toString() {
                return "abcd";
            }
        });
        assertEquals("abcd", v.currentToString());

        v.pushThrough(Qt.ArrowType.UpArrow);
        assertEquals(Qt.ArrowType.UpArrow.value(), v.currentToInt());
    }


    private boolean itemChangeCalled = false;
    @Test
    public void run_interfaces_passed_as_QVariant() {
        QGraphicsPixmapItem it = new QGraphicsPixmapItem() {

            @QtBlockedSlot
            public Object itemChange(QGraphicsItem.GraphicsItemChange change, Object value) {
                itemChangeCalled = change == QGraphicsItem.GraphicsItemChange.ItemChildRemovedChange
                                          && value instanceof QGraphicsPixmapItem;
                return super.itemChange(change, value);
            }
        };

        itemChangeCalled = false;
        // Note that ChildAddedChange will send an incomplete Object to itemChange. This is documented
        // to happen in C++ as well, since the change event is posted from the child's constructor.
        QGraphicsPixmapItem it2 = new QGraphicsPixmapItem(it);
        it2.setParentItem(null);
        assertTrue(itemChangeCalled);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQVariant.class.getName());
    }
}
