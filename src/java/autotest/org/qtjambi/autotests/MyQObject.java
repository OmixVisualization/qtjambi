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

import java.util.List;

import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.widgets.QWidget;

// ### Test overloads
// ### Test mixed types
// ### Test widening

class MyQObject extends QObject
{
    public Object slotResult = null;
    public Object slotResult2 = null;
    public static int finalizedCount = 0;

    public Signal1<String> signalString = new Signal1<String>();
    public Signal1<Long> signalLong = new Signal1<Long>();
    public Signal1<Integer> signalInteger = new Signal1<Integer>();
    public Signal1<Integer> signalInteger2 = new Signal1<Integer>();
    public Signal1<Boolean> signalBoolean = new Signal1<Boolean>();
    public Signal1<Short> signalShort = new Signal1<Short>();
    public Signal1<Character> signalCharacter = new Signal1<Character>();
    public Signal1<Float> signalFloat = new Signal1<Float>();
    public Signal1<Double> signalDouble = new Signal1<Double>();
    public Signal1<MyQObject> signalMyQObject = new Signal1<MyQObject>();
    public Signal2<Double, Integer> signalDoubleInteger = new Signal2<Double, Integer>();
    public Signal0 signalNoParams = new Signal0();
    public Signal4<String, Double, Integer, Integer> signalMixed1 = new Signal4<String, Double, Integer, Integer>();
    public Signal4<String, MyQObject, Integer, Double> signalMixed2 = new Signal4<String, MyQObject, Integer, Double>();
    public Signal1<QSize> signalQSize = new Signal1<QSize>();
    public Signal1<QObject[]> signalQObjectArray = new Signal1<QObject[]>();
    public Signal3<Integer[], Integer[][], Integer[][][]> signalManyArrays = new Signal3<Integer[], Integer[][], Integer[][][]>();
    public Signal1<List<String>> signalStringList = new Signal1<List<String>>();
    public Signal1<List<List<String>>> signalStringListList = new Signal1<List<List<String>>>();
    public Signal0 destroyed = null;


    public MyQObject()
    {
        this(null);
    }


    public MyQObject(QObject parent)
    {
        super(parent);
    }

    @Override
    protected void disposed()
    {
        if (destroyed != null)
            this.destroyed.emit();
        super.disposed();
    }

    public void increaseFinalized()
    {
        ++finalizedCount;
    }

    public void javaSignalString(String param)
    {
        signalString.emit(param);
    }

    public String javaSlotString(String param)
    {
        slotResult = param;
        return param;
    }

    // java.lang.Long
    public void javaSignalLong(Long param)
    {
        signalLong.emit(param);
    }
    public Long javaSlotLong(Long param)
    {
        slotResult = param;
        return param;
    }

    // java.lang.Integer
    public void javaSignalInteger(Integer param)
    {
        signalInteger.emit(param);
    }

    public Integer javaSlotInteger(Integer param)
    {
        slotResult = param;
        return param;
    }

    // List<List<String>>
    public void javaSignalStringListList(List<List<String>> param)
    {
        signalStringListList.emit(param);
    }

    public void javaSlotStringListList(List<List<String>> param)
    {
        slotResult = param.get(0).get(1);
    }

    // List<String>
    public void javaSignalStringList(List<String> param)
    {
        signalStringList.emit(param);
    }

    public void javaSlotStringList(List<String> param)
    {
        slotResult = param.get(0);
    }

    // Some arrays
    public void javaSignalManyArrays(Integer[] param1, Integer[][] param2, Integer[][][] param3)
    {
        signalManyArrays.emit(param1, param2, param3);
    }

    public void javaSlotManyArrays(Integer[] param1, Integer[][] param2, Integer[][][] param3)
    {
        slotResult = param3;
    }

    // java.lang.Boolean
    public void javaSignalBoolean(Boolean param)
    {
        signalBoolean.emit(param);
    }
    public Boolean javaSlotBoolean(Boolean param)
    {
        slotResult = param;
        return param;
    }

    // java.lang.Short
    public void javaSignalShort(Short param)
    {
        signalShort.emit(param);
    }
    public Short javaSlotShort(Short param)
    {
        slotResult = param;
        return param;
    }

    // java.lang.Character
    public void javaSignalCharacter(Character param)
    {
        signalCharacter.emit(param);
    }
    public Character javaSlotCharacter(Character param)
    {
        slotResult = param;
        return param;
    }

    // java.lang.Float
    public void javaSignalFloat(Float param)
    {
        signalFloat.emit(param);
    }
    public Float javaSlotFloat(Float param)
    {
        slotResult = param;
        return param;
    }

    // java.lang.Double
    public void javaSignalDouble(Double param)
    {
        signalDouble.emit(param);
    }

    public Double javaSlotDouble(Double param)
    {
        slotResult = param;
        return param;
    }

    // MyQObject
    public void javaSignalMyQObject(MyQObject param)
    {
        signalMyQObject.emit(param);
    }

    public MyQObject javaSlotMyQObject(MyQObject param)
    {
        slotResult = param;
        return param;
    }

    public QObject javaSlotQObject(QObject param)
    {
        slotResult = param;
        return param;
    }

    public void javaSignalQObjectArray(QObject array[])
    {
        signalQObjectArray.emit(array);
    }

    public void javaSlotQObjectArray(QObject array[])
    {
        slotResult = array;
    }

    // long
    public long javaSlotlong(long param)
    {
        slotResult = param;

        return param;
    }

    // int
    public void javaSignalint(int param)
    {
        signalInteger.emit(param);
    }
    public int javaSlotint(int param)
    {
        slotResult = param;
        return param;
    }

    // boolean
    public void javaSignalboolean(boolean param)
    {
        signalBoolean.emit(param);
    }
    public boolean javaSlotboolean(boolean param)
    {
        slotResult = param;
        return param;
    }

    // short
    public void javaSignalshort(short param)
    {
        signalShort.emit(param);
    }
    public short javaSlotshort(short param)
    {
        slotResult = param;
        return param;
    }

    // char
    public void javaSignalchar(char param)
    {
        signalCharacter.emit(param);
    }
    public char javaSlotchar(char param)
    {
        slotResult = param;
        return param;
    }

    // float
    public void javaSignalfloat(float param)
    {
        signalFloat.emit(param);
    }
    public float javaSlotfloat(float param)
    {
        slotResult = param;
        return param;
    }

    // double
    public void javaSignaldouble(double param)
    {
        signalDouble.emit(param);
    }
    public double javaSlotdouble(double param)
    {
        slotResult = param;
        return param;
    }

    public void javaSignalTwoParameters(double param1, int param2)
    {
        signalDoubleInteger.emit(param1, param2);
    }

    public void javaSignalNoParams()
    {
        signalNoParams.emit();
    }

    public void javaSlotNoParams()
    {
        slotResult = "Slot no params";
    }

    public String javaSlotNoParamsReturn()
    {
        slotResult = "Slot no params return";
        return (String)slotResult;
    }


    public void javaSignalMixed1(String s, double d, int i, Integer j)
    {
        signalMixed1.emit(s, d, i, j);
    }

    public Integer javaSlotMixed1(String s, double d, int i, Integer j)
    {
        slotResult = s;
        return j;
    }


    public void javaSignalMixed2(String a, MyQObject b, int c, Double d)
    {
        signalMixed2.emit(a, b, c, d);
    }

    public int javaSlotMixed2(String a, MyQObject b, int c)
    {
        slotResult = b;
        return c;
    }

    public void javaSlotResultTextTrue()
    {
        slotResult = "true";
    }

    public void javaSlotFocusChanged(QWidget old, QWidget now)
    {
        slotResult = old;
        slotResult2 = now;
    }

    public void javaSignalQSize(QSize sz)
    {
        signalQSize.emit(sz);
    }
}
