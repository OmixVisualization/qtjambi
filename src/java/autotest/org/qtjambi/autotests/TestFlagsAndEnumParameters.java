package org.qtjambi.autotests;

import org.qtjambi.autotests.generated.FlagsAndEnumTest;
import org.junit.Test;
import static org.junit.Assert.assertEquals;

import org.qtjambi.qt.QFlags;
import org.qtjambi.qt.QSignalEmitter.Signal1;
import org.qtjambi.qt.QtEnumerator;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.internal.MetaObjectTools;

public class TestFlagsAndEnumParameters extends FlagsAndEnumTest{
	
	public final Signal1<TestEnum> testSignal4 = new Signal1<TestEnum>();
	public final Signal1<TestEnum2> testSignal5 = new Signal1<TestEnum2>();
	public final Signal1<MyFlags> testSignal6 = new Signal1<MyFlags>();
	
	private int result;
	
	@Test
	public void testFlagsAndEnumParameters(){
		this.__qt_signalInitialization(testSignal1.name());
		this.__qt_signalInitialization(testSignal2.name());
		this.__qt_signalInitialization(testSignal3.name());
		this.__qt_signalInitialization(testSignal4.name());
		this.__qt_signalInitialization(testSignal5.name());
		this.__qt_signalInitialization(testSignal6.name());
		
		MetaObjectTools.emitNativeSignal(this, "testSignal1(org.qtjambi.qt.core.Qt$AlignmentFlag)", "testSignal1(Qt::AlignmentFlag)", new Object[]{Qt.AlignmentFlag.AlignJustify});
		try{
			assertEquals(Qt.AlignmentFlag.AlignJustify, Qt.AlignmentFlag.resolve(result));
			result = 0;
			
			testSignal1.emit(Qt.AlignmentFlag.AlignJustify);
			assertEquals(Qt.AlignmentFlag.AlignJustify, Qt.AlignmentFlag.resolve(result));
			result = 0;
			
			MetaObjectTools.emitNativeSignal(this, "testSignal2(org.qtjambi.qt.core.Qt$Orientation)", "testSignal2(Qt::Orientation)", new Object[]{Qt.Orientation.Horizontal});
			assertEquals(Qt.Orientation.Horizontal, Qt.Orientation.resolve(result));
			result = 0;
			
			testSignal2.emit(Qt.Orientation.Horizontal);
			assertEquals(Qt.Orientation.Horizontal, Qt.Orientation.resolve(result));
			result = 0;
			
			Qt.Alignment a = new Qt.Alignment(Qt.AlignmentFlag.AlignTop, Qt.AlignmentFlag.AlignJustify);
			MetaObjectTools.emitNativeSignal(this, "testSignal3(org.qtjambi.qt.core.Qt$Alignment)", "testSignal3(Qt::Alignment)", new Object[]{a});
			assertEquals(a, new Qt.Alignment(result));
			result = 0;
			
			testSignal3.emit(a);
			assertEquals(a, new Qt.Alignment(result));
			result = 0;
		}catch(org.qtjambi.qt.QNoSuchEnumValueException e){
			org.junit.Assert.fail("No Such Enum Value: Qt.AlignmentFlag " + result);
		}

		// must not crash internally inside MetaObjectTools.emitNativeSignal()
		testSignal4.emit(TestEnum.ENTRY1);
		testSignal5.emit(TestEnum2.ENTRY2);
		testSignal6.emit(new MyFlags(TestEnum2.ENTRY3, TestEnum2.ENTRY2));
	}
	
	public void processResult(int result) {
		this.result = result;
	}
	
	public enum TestEnum{
		ENTRY1, ENTRY2, ENTRY3
	}
	
	public enum TestEnum2 implements QtEnumerator{
		ENTRY1, ENTRY2, ENTRY3;

		@Override
		public int value() {
			return ordinal();
		}
		
		public static TestEnum2 resolve(int i){
			return TestEnum2.values()[i];
		}
	}
	
	public static class MyFlags extends QFlags<TestEnum2>{

		public MyFlags(TestEnum2... args) {
			super(args);
		}
		
	}
}
