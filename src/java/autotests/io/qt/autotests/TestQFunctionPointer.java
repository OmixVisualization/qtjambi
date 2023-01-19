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
package io.qt.autotests;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

import org.junit.Assert;
import org.junit.Assume;
import org.junit.Test;

import io.qt.QNativePointer;
import io.qt.QUnsuccessfulInvocationException;
import io.qt.QtMetaType;
import io.qt.QtObjectInterface;
import io.qt.QtReferenceType;
import io.qt.autotests.generated.FunctionalTest;
import io.qt.core.QDir;
import io.qt.core.QEasingCurve;
import io.qt.core.QFunctionPointer;
import io.qt.core.QGenericArgument;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;
import io.qt.core.QOperatingSystemVersion.OSType;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QStringList;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.gui.QFont;
import io.qt.internal.ClassAnalyzerUtility;

public class TestQFunctionPointer extends ApplicationInitializer{
	
	@Test
    public void testFunctionExtraction() {
		List<QEasingCurve.EasingFunction> list = new ArrayList<>();
		
		QEasingCurve c = new QEasingCurve();
		c.setType(QEasingCurve.Type.Custom);
		for (int i = 0; i < 5; ++i) {
			int _i = i;
			QEasingCurve.EasingFunction f = d->d/(_i+1);
			c.setCustomType(f);
			Assert.assertEquals(0.2/(_i+1), c.valueForProgress(0.2), 0.001);
			list.add(f);
		}
		c.dispose();
		
		for (int i = list.size()-1; i >= 0; --i) {
			list.get(i).dispose();
		}
		list.clear();
	}
	
	@FunctionalInterface
	interface Fun30 extends QtObjectInterface{
		@QtMetaType(type=QMetaType.Type.QVariantList)
		QList<Object> invoke(String s, QObject o, long j, short i16, double d, @QtReferenceType(isConst=true) QColor color, @QtReferenceType(isConst=true) QStringList list, QRectF rect, QFont font);
	}
	
	@FunctionalInterface
	interface UnknownFunction1 extends QtObjectInterface{
		void invoke(int i);
	}
	
	@FunctionalInterface
	interface Fun53 extends QtObjectInterface{
		void invoke(UnknownFunction1 fun);
		default void defaultMethod(){
			System.out.println("defaultMethod()");
		}
	}
	
	@FunctionalInterface
	interface UnknownFunction2 extends QtObjectInterface{
		long invoke(double d, int i, @QtReferenceType(isConst=true) QColor color, Qt.Alignment a);
	}
	
	@FunctionalInterface
	interface Fun54 extends QtObjectInterface{
		long invoke(UnknownFunction2 fun);
	}
	
	@FunctionalInterface
	interface UnknownFunction3 extends QtObjectInterface{
		UnknownFunction1 invoke(UnknownFunction2 fun2);
	}
	
	@FunctionalInterface
	interface Fun55 extends QtObjectInterface{
		UnknownFunction1 invoke(UnknownFunction3 fun);
	}
	
	@FunctionalInterface
	interface PrintF extends QtObjectInterface{
		void printf(byte[] format, Object...values);
		
		default void printf(String format, Object...values) {
			printf((format+'\0').getBytes(), values);
		}
	}
	
	@SuppressWarnings("deprecation")
	@Test
    public void testSingleParamFunctions() throws Throwable {
    	QObject obj = new QObject();
    	obj.setObjectName("QObject Test");
    	QFunctionPointer fp = FunctionalTest.getFunction(0);
    	fp.invoke();
    	fp = FunctionalTest.getFunction(1);
    	fp.invoke(12345);
    	fp = FunctionalTest.getFunction(2);
    	fp.invoke(false);
    	fp = FunctionalTest.getFunction(3);
    	fp.invoke(2.5);
    	fp = FunctionalTest.getFunction(4);
    	fp.invoke(3.5f);
    	fp = FunctionalTest.getFunction(5);
    	fp.invoke('F');
    	fp = FunctionalTest.getFunction(6);
    	fp.invoke("String Test");
    	fp = FunctionalTest.getFunction(7);
    	fp.invoke(QGenericArgument.of("String& Test").asConstRef());
    	fp = FunctionalTest.getFunction(8);
    	fp.invoke(obj);
    	fp = FunctionalTest.getFunction(9);
    	fp.invoke((Object)null);
    	fp = FunctionalTest.getFunction(10);
    	fp.invoke(QGenericArgument.defaultOf(new QMetaType(QMetaType.type("JNIEnv*"))));
    	fp = FunctionalTest.getFunction(46);
    	fp.invoke(QMap.of("Rect1", new QRect(6,6,2,1)));
    	{
    		Map<String,QRect> arg = new TreeMap<>();
    		arg.put("Rect1", new QRect(6,6,2,1));
    		fp.invoke(QGenericArgument.of(arg).as(QMetaType.fromType(QMap.class, QMetaType.fromType(String.class), QMetaType.fromType(QRect.class))));
    	}
    	fp = FunctionalTest.getFunction(47);
    	fp.invoke(QGenericArgument.of(QMap.of("Rect1", new QRect(6,6,2,1))).asConstRef());
    	
	}
	
	@SuppressWarnings("deprecation")
	@Test
    public void testMultipleParamsFunctions() throws Throwable {
    	QObject obj = new QObject();
    	obj.setObjectName("QObject Test");
		QFunctionPointer fp = FunctionalTest.getFunction(11);
    	fp.invoke((byte)0, 12345);
    	fp = FunctionalTest.getFunction(12);
    	fp.invoke((byte)0, true);
    	fp = FunctionalTest.getFunction(13);
    	fp.invoke(0, Math.PI);
    	fp = FunctionalTest.getFunction(14);
    	fp.invoke(25L, (float)Math.PI);
    	fp = FunctionalTest.getFunction(15);
    	fp.invoke((byte)0, 'P');
    	fp = FunctionalTest.getFunction(16);
    	fp.invoke((byte)0, "String Test 2");
    	fp = FunctionalTest.getFunction(17);
    	fp.invoke((byte)0, QGenericArgument.of("String& Test 2").asConstRef());
    	fp = FunctionalTest.getFunction(18);
    	obj.setObjectName("QObject Test 2");
    	fp.invoke((byte)0, obj);
    	fp = FunctionalTest.getFunction(19);
    	fp.invoke((byte)0, null);
    	fp = FunctionalTest.getFunction(20);
    	fp.invoke((byte)0, QGenericArgument.defaultOf(new QMetaType(QMetaType.type("JNIEnv*"))));
    	fp = FunctionalTest.getFunction(37);
    	fp.invoke('A', 1, 2, 3, Qt.GlobalColor.blue, Qt.AlignmentFlag.AlignTop.combined(Qt.AlignmentFlag.AlignLeading));
    	ApplicationInitializer.runGC();
	}
	
	@Test
    public void testReturningFunctions_int() throws Throwable {
    	QFunctionPointer fp = FunctionalTest.getFunction(21);
    	Assert.assertEquals((Object)12345, fp.invoke(int.class, 12345));
	}
	
	@Test
    public void testReturningFunctions_boolean() throws Throwable {
    	QFunctionPointer fp = FunctionalTest.getFunction(22);
    	Assert.assertEquals((Object)false, fp.invoke(boolean.class, false));
	}
	
	@Test
    public void testReturningFunctions_double() throws Throwable {
    	QFunctionPointer fp = FunctionalTest.getFunction(23);
    	Assert.assertEquals((Object)7.5, fp.invoke(double.class, 7.5));
	}
	
	@Test
    public void testReturningFunctions_float() throws Throwable {
    	QFunctionPointer fp = FunctionalTest.getFunction(24);
    	Assert.assertEquals((Object)9.5f, fp.invoke(float.class, 9.5f));
	}
	
	@Test
    public void testReturningFunctions_char() throws Throwable {
		QFunctionPointer fp = FunctionalTest.getFunction(35);
    	try {
			QMetaType.Type char16 = QMetaType.Type.valueOf("Char16");
			Assert.assertEquals((Object)'Z', fp.invoke(QGenericArgument.returning(new QMetaType(char16)), 'Z'));
			fp = FunctionalTest.getFunction(36);
			Assert.assertEquals((Object)'Q', fp.invoke(QGenericArgument.returning(new QMetaType(char16)), 'Q'));
		} catch (AssertionError e) {
			throw e;
		} catch (Throwable e) {
			e.printStackTrace();
		}
	}
	
	@Test
    public void testReturningFunctions_QChar() throws Throwable {
		Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
		QFunctionPointer fp = FunctionalTest.getFunction(25);
    	Assert.assertEquals((Object)'W', fp.invoke(char.class, 'W'));    	
	}
	
	@Test
    public void testReturningFunctions_flags() throws Throwable {
		Assume.assumeFalse(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android));
    	QFunctionPointer fp = FunctionalTest.getFunction(45);
    	Assert.assertEquals(Qt.AlignmentFlag.AlignVCenter.combined(Qt.AlignmentFlag.AlignJustify), fp.invoke(Qt.Alignment.class));
    	fp = FunctionalTest.getFunction(32);
    	Assert.assertEquals(Qt.AlignmentFlag.AlignTop.combined(Qt.AlignmentFlag.AlignBottom), fp.invoke(Qt.Alignment.class, Qt.AlignmentFlag.AlignTop.combined(Qt.AlignmentFlag.AlignBottom)));
	}
	
	@Test
    public void testReturningFunctions_enum() throws Throwable {
		QFunctionPointer fp = FunctionalTest.getFunction(31);
    	Assert.assertEquals(Qt.AlignmentFlag.AlignBottom, fp.invoke(Qt.AlignmentFlag.class, Qt.AlignmentFlag.AlignBottom));
	}
	
	@Test
    public void testReturningFunctions_functionpointer() throws Throwable {
    	QFunctionPointer fp = FunctionalTest.getFunction(52);
    	{
    		QFunctionPointer arg = FunctionalTest.getFunction(0);
    		QFunctionPointer result = fp.invoke(QFunctionPointer.class, arg);
    		result.invoke();
    		QNativePointer argNP = QNativePointer.fromObject(arg);
    		QNativePointer resultNP = QNativePointer.fromObject(result);
    		Assert.assertEquals(argNP.pointerValue().pointer(), resultNP.pointerValue().pointer());
    	}
	}
	
	@Test
    public void testReturningFunctions_pointer() throws Throwable {
		QObject obj = new QObject();
    	obj.setObjectName("QObject Test");
    	QFunctionPointer fp;
    	fp = FunctionalTest.getFunction(28);
    	Assert.assertEquals(obj, fp.invoke(QObject.class, obj));
	}
	
	@Test
    public void testStructReturningFunctions() throws Throwable {
		Assume.assumeTrue("Windows only", QOperatingSystemVersion.current().isAnyOfType(OSType.Windows));
		QFunctionPointer fp = FunctionalTest.getFunction(34);
    	Object variant = new Object();
    	Assert.assertEquals(variant, fp.invoke(QGenericArgument.<Object>returning(new QMetaType(QMetaType.Type.QVariant)), QGenericArgument.of(variant).as(new QMetaType(QMetaType.Type.QVariant)).asConstRef()));
		QObject obj = new QObject();
    	obj.setObjectName("QObject Test");
    	fp = FunctionalTest.getFunction(26);
    	Assert.assertEquals("String Test 3", fp.invoke(String.class, "String Test 3"));
    	fp = FunctionalTest.getFunction(27);
    	Assert.assertEquals("String& Test 3", fp.invoke(String.class, QGenericArgument.of("String& Test 3").asConstRef()));
    	fp = FunctionalTest.getFunction(29);
    	Assert.assertEquals(new QColor(Qt.GlobalColor.darkBlue), fp.invoke(QColor.class));
    	fp = FunctionalTest.getFunction(44);
    	Assert.assertEquals(new QRect(8,2,4,9), fp.invoke(QRect.class));
    	fp = FunctionalTest.getFunction(33);
    	Assert.assertEquals(QDir.current(), fp.invoke(QDir.class, QGenericArgument.of(QDir.current()).asConstRef()));
		fp = FunctionalTest.getFunction(30);
    	{
    		QFont font = new QFont("Arial", 15);
	    	@SuppressWarnings("rawtypes")
			QList result = fp.invoke(QGenericArgument.returning(QList.class, new QMetaType(QMetaType.Type.QVariant)),
	    			"MultiString",
	    			obj,
	    			90000L,
	    			(short)897,
	    			6.666,
	    			QGenericArgument.of(new QColor(Qt.GlobalColor.blue)).asConstRef(),
	    			QGenericArgument.of(Arrays.asList("A", "B", "C")).as(new QMetaType(QMetaType.Type.QStringList)).asConstRef(),
	    			new QRectF(5,5,2,2),
	    			font
	    			);
	    	Assert.assertEquals(9, result.size());
	    	Assert.assertEquals("MultiString", result.get(0));
	    	Assert.assertEquals(obj, result.get(1));
			Assert.assertEquals(90000L, result.get(2));
			Assert.assertEquals((short)897, result.get(3));
			Assert.assertEquals(6.666, result.get(4));
			Assert.assertEquals(new QColor(Qt.GlobalColor.blue), result.get(5));
			Assert.assertEquals(new QStringList("A", "B", "C"), result.get(6));
			Assert.assertEquals(new QRectF(5,5,2,2), result.get(7));
			Assert.assertEquals(font, result.get(8));
    	}
	}
	
	@Test
    public void testArrayFunctions() throws Throwable {
		QFunctionPointer fp;
		{
        	fp = FunctionalTest.getFunction(48);
        	String arg[] = {"Argument"};
        	fp.invoke((Object)arg);
        	Assert.assertEquals("Test", arg[0]);
    	}
    	{
        	fp = FunctionalTest.getFunction(49);
        	int arg[] = {9,8,7,6,5,4,3,2,1};
        	fp.invoke(arg, arg.length);
        	for (int i = 0; i < arg.length; i++) {
        		Assert.assertEquals(i, arg[i]);
			}
    	}
    	{
        	fp = FunctionalTest.getFunction(50);
        	QRectF arg[] = {new QRectF(5,5,5,5), new QRectF(5,5,5,5)};
        	fp.invoke(arg, arg.length);
        	for (int i = 0; i < arg.length; i++) {
        		Assert.assertEquals(new QRectF(0,0,i+1,i*2+1), arg[i]);
			}
    	}
	}
	
	@Test
    public void testVariadicFunction() throws Throwable {
    	Assume.assumeTrue("Cannot run variadic function on macOS arm64.", 
				!QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.MacOS, 
																QOperatingSystemVersion.OSType.IOS, 
																QOperatingSystemVersion.OSType.WatchOS)
				|| "x86_64".equals(System.getProperty("os.arch"))
				|| "x64".equals(System.getProperty("os.arch"))
				|| "amd64".equals(System.getProperty("os.arch"))
			);
    	QFunctionPointer fp = FunctionalTest.getFunction(51);
    	fp.invoke(("print text: %s, integer: %d, double: %f ...\n\0").getBytes(), "string\0".getBytes(), 555, 9.8163);
	}
	
//	@Test
    public void testVariadicFunctionCast() throws Throwable {
		QFunctionPointer fp = FunctionalTest.getFunction(51);
		PrintF printf = fp.cast(PrintF.class);
		printf.printf("print text: %s, integer: %d, double: %f ...\n", "string\0".getBytes(), 555, 9.8163);
	}
    	
	@Test
    public void testFunctionPointerCast() throws Throwable {
		QFunctionPointer fp = FunctionalTest.getFunction(53);
    	{
    		Fun53 fun53 = fp.cast(Fun53.class);
    		fun53.hashCode();
    		fun53.defaultMethod();
    		int[] result = {0};
    		fun53.invoke(i->{
    			result[0] = i;
			});
    		Assert.assertEquals(987654321, result[0]);
    	}
    	
    	fp = FunctionalTest.getFunction(54);
    	try{
    		Fun54 fun54 = fp.cast(Fun54.class);
    		fun54.hashCode();
    		int[] iresult = {0};
    		double[] dresult = {0};
    		QColor[] cresult = {null};
    		Qt.Alignment[] aresult = {null};
    		long l = fun54.invoke((d,i,c,a)->{
    			iresult[0] = i;
    			dresult[0] = d;
    			cresult[0] = c;
    			aresult[0] = a;
    			return (long)21;
			});
    		Assert.assertEquals(21l, l);
    		Assert.assertEquals(10, iresult[0]);
    		Assert.assertEquals(0.0001, 9.12345, dresult[0]);
    		Assert.assertEquals(new QColor(Qt.GlobalColor.blue), cresult[0]);
    		Assert.assertEquals(Qt.AlignmentFlag.AlignLeft.combined(Qt.AlignmentFlag.AlignTop), aresult[0]);
    	}catch(AssertionError e) {
    		throw e;
    	}catch(QUnsuccessfulInvocationException e) {
    		if(ClassAnalyzerUtility.useAnnotatedType || !String.format("Type %1$s not supported without pointer or reference arithmetic.", QColor.class.getName()).equals(e.getMessage())) {
    			throw e;
    		}
    	}
    	fp = FunctionalTest.getFunction(55);
    	{
    		Fun55 fun55 = fp.cast(Fun55.class);
    		int[] uf1Arg = {0};
    		UnknownFunction1 uf1 = i->{
    			uf1Arg[0] = i;
    		};
    		long[] uf2Result = {0};
    		UnknownFunction3 uf3 = uf2->{ 
    			uf2Result[0] = uf2.invoke(6.5, 2, new QColor(Qt.GlobalColor.darkGray), Qt.AlignmentFlag.AlignTrailing.asFlags());
    			return uf1; 
    		};
    		UnknownFunction1 result = fun55.invoke(uf3);
    		Assert.assertEquals(uf1, result);
    		Assert.assertEquals(uf1Arg[0], 0);
    		Assert.assertEquals((long)((2 + new QColor(Qt.GlobalColor.darkGray).rgba() + Qt.AlignmentFlag.AlignTrailing.value())*6.5), uf2Result[0]);
    		result.invoke(5);
    		Assert.assertEquals(uf1Arg[0], 5);
    	}
    }
	
	@Test
    public void testStructReturningFunctionPointerCast() throws Throwable {
		Assume.assumeTrue("Windows only", QOperatingSystemVersion.current().isAnyOfType(OSType.Windows));
    	QObject obj = new QObject();
    	obj.setObjectName("QObject Test");
    	
		QFunctionPointer fp = FunctionalTest.getFunction(30);
    	{
    		QFont font = new QFont("Arial", 15);
    		Fun30 fun30 = fp.cast(Fun30.class);
    		fun30.getClass();
    		fun30.hashCode();
    		fun30.toString();
    		fun30.equals(null);
    		fun30.equals(fun30);
    		fun30.equals((Object)fp);
    		fun30.equals((Object)obj);
    		QList<Object> result = fun30.invoke(
	    			"MultiString",
	    			obj,
	    			90000L,
	    			(short)897,
	    			6.666,
	    			new QColor(Qt.GlobalColor.blue),
	    			new QStringList("A", "B", "C"),
	    			new QRectF(5,5,2,2),
	    			font
				);
	    	Assert.assertEquals(9, result.size());
	    	Assert.assertEquals("MultiString", result.get(0));
	    	Assert.assertEquals(obj, result.get(1));
			Assert.assertEquals(90000L, result.get(2));
			Assert.assertEquals((short)897, result.get(3));
			Assert.assertEquals(6.666, result.get(4));
			Assert.assertEquals(new QColor(Qt.GlobalColor.blue), result.get(5));
			Assert.assertEquals(new QStringList("A", "B", "C"), result.get(6));
			Assert.assertEquals(new QRectF(5,5,2,2), result.get(7));
			Assert.assertEquals(font, result.get(8));
    	}
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestQFunctionPointer.class.getName());
    }
}
