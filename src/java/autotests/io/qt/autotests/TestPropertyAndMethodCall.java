/****************************************************************************
**
** Copyright (C) 1992-2009 Nokia. All rights reserved.
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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.math.BigInteger;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;

import org.junit.AfterClass;
import org.junit.Assume;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QFlags;
import io.qt.QNoSuchEnumValueException;
import io.qt.QtFlagEnumerator;
import io.qt.QtPointerType;
import io.qt.QtPropertyMember;
import io.qt.QtPropertyNotify;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.QtUninvokable;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.PropertyAndMethodCallTest;
import io.qt.core.QDataStream;
import io.qt.core.QLibraryInfo;
import io.qt.core.QMetaType;
import io.qt.core.QObject;
import io.qt.core.QRectF;
import io.qt.core.QStringListModel;
import io.qt.core.Qt;
import io.qt.gui.QColor;
import io.qt.gui.QDrag;
import io.qt.gui.QPainter;
import io.qt.gui.QRgba64;
import io.qt.internal.ClassAnalyzerUtility;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QStyleOptionGraphicsItem;
import io.qt.widgets.QWidget;

public class TestPropertyAndMethodCall extends ApplicationInitializer {

	private static PropertyAndMethodCallTest object;
	private static TestQObject javaObject;
//	private static TestPtrQObject javaObject2;

	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitialize();
		QMetaType.qRegisterMetaType(TestQObject.ExtendedColor.class);
		QMetaType.qRegisterMetaType(TestQObject.DerivedQObject.class);
		QMetaType.qRegisterMetaType(TestQObject.CustomQtValue.class);
		QMetaType.qRegisterMetaType(TestQObject.CustomQtInterfaceValue.class);
		QMetaType.qRegisterMetaType(TestQObject.CustomJavaType.class);
		QMetaType.qRegisterMetaType(TestQObject.CustomJavaType.class);
		QMetaType.qRegisterMetaType(TestQObject.CustomQtEnum.class);
		QMetaType.qRegisterMetaType(TestQObject.CustomEnum.class);
		QMetaType.qRegisterMetaType(TestQObject.CustomQtFlags.class);
		QMetaType.qRegisterMetaType(QGraphicsItem.class);
		QMetaType.qRegisterMetaType(QStringListModel.class);
		QMetaType.qRegisterMetaType(QDrag.class);
		QMetaType.qRegisterMetaType(Qt.AspectRatioMode.class);
		QMetaType.qRegisterMetaType(Qt.Orientations.class);
		object = new PropertyAndMethodCallTest();
		javaObject = new TestQObject(null);
		javaObject.metaObject().methods().forEach(m->System.out.println(m.typeName() + " " + m.cppMethodSignature()));
		javaObject.metaObject().properties().forEach(p->System.out.println(p.typeName()+" "+p.name()));
	}
	
	@AfterClass
    public static void testDispose() throws Exception {
		object = null;
		javaObject = null;
		ApplicationInitializer.testDispose();
	}

	@Test
	public void testMethodCallColor() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallColor(javaObject));
	}
	
	@Test
	public void testMethodCallColorPtr() {
		Assume.assumeTrue(ClassAnalyzerUtility.useAnnotatedType);
		assertTrue(PropertyAndMethodCallTest.testMethodCallColorPtr(javaObject));
	}

	@Test
	public void testMethodCallCustomJavaType() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallCustomJavaType(javaObject));
	}

	@Test
	public void testMethodCallCustomQtEnum() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallCustomQtEnum(javaObject));
	}

	@Test
	public void testMethodCallCustomQtEnum2() {
		Assume.assumeTrue("Checking enum null in Qt6 makes no sense", QLibraryInfo.version().majorVersion()==5);
		assertTrue(PropertyAndMethodCallTest.testMethodCallCustomQtEnum2(javaObject));
	}

	@Test
	public void testMethodCallCustomQtValue() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallCustomQtValue(javaObject));
	}
	
	@Test
	public void testMethodCallExtendedColor() {
		Assume.assumeTrue("Extended value types are available since Qt6", QLibraryInfo.version().majorVersion()>5);
		assertTrue(PropertyAndMethodCallTest.testMethodCallExtendedColor(javaObject));
	}
	
	@Test
	public void testMethodCallCustomQtInterfaceValue() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallCustomQtInterfaceValue(javaObject));
	}

	@Test
	public void testMethodCallDerivedQObject() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallDerivedQObject(javaObject));
	}

	@Test
	public void testMethodCallEnum() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallEnum(javaObject));
	}

	@Test
	public void testMethodCallNumber() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallNumber(javaObject));
	}

	@Test
	public void testMethodCallQtEnum() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallQtEnum(javaObject));
	}

	@Test
	public void testMethodCallQtFlags() {
		assertTrue(PropertyAndMethodCallTest.testMethodCallQtFlags(javaObject));
	}

	@Test
	public void testFetchPropertyCustomJavaTypeCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyCustomJavaType(javaObject));
	}

	@Test
	public void testFetchPropertyColorCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyColor(javaObject));
	}
	
	@Test
	public void testFetchPropertyColorPtrCPP() {
		Assume.assumeTrue(ClassAnalyzerUtility.useAnnotatedType);
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyColorPtr(javaObject));
	}

	@Test
	public void testFetchPropertyCustomQtEnumCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyCustomQtEnum(javaObject));
	}

	@Test
	public void testFetchPropertyCustomQtEnum2CPP() {
		Assume.assumeTrue("Checking enum null in Qt6 makes no sense", QLibraryInfo.version().majorVersion()==5);
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyCustomQtEnum2(javaObject));
	}

	@Test
	public void testFetchPropertyCustomQtValueCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyCustomQtValue(javaObject));
	}
	
	@Test
	public void testFetchPropertyExtendedColorCPP() {
		Assume.assumeTrue("Extended value types are available since Qt6", QLibraryInfo.version().majorVersion()>5);
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyExtendedColor(javaObject));
	}
	
	@Test
	public void testFetchPropertyCustomQtInterfaceValueCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyCustomQtInterfaceValue(javaObject));
	}

	@Test
	public void testFetchPropertyDerivedQObjectCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyDerivedQObject(javaObject));
	}

	@Test
	public void testFetchPropertyCustomEnumCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyEnum(javaObject));
	}

	@Test
	public void testFetchPropertyNumberCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyNumber(javaObject));
	}
	
	@Test
	public void testFetchPropertyQtEnumCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyQtEnum(javaObject));
	}

	@Test
	public void testFetchPropertyQtFlagsCPP() {
		assertTrue(PropertyAndMethodCallTest.testFetchPropertyQtFlags(javaObject));
	}

	@Test
	public void testFetchPropertyCustomJavaTypeJAVA() {
		assertEquals(javaObject.getCustomJavaType(), javaObject.property("customJavaType"));
	}

	@Test
	public void testFetchPropertyColorJAVA() {
		assertEquals(javaObject.getColor(), javaObject.property("color"));
	}
	
	@Test
	public void testMemberProperty() {
		assertEquals(javaObject.memberProperty, javaObject.property("member"));
		assertTrue(javaObject.setProperty("member", "null"));
		assertEquals("null", javaObject.memberProperty);
	}
	
	@Test
	public void testFetchPropertyColorPtrJAVA() {
		assertEquals(javaObject.getColorPtr(), javaObject.property("colorPtr"));
	}

	@Test
	public void testFetchPropertyCustomQtEnumJAVA() {
		assertEquals(javaObject.getCustomQtEnum(), javaObject.property("customQtEnum"));
	}

	@Test
	public void testFetchPropertyCustomQtEnum2JAVA() {
		assertEquals(null, javaObject.property("customQtEnum2"));
	}

	@Test
	public void testFetchPropertyCustomQtValueJAVA() {
		assertEquals(javaObject.getCustomQtValue(), javaObject.property("customQtValue"));
	}
	
	@Test
	public void testFetchPropertyExtendedColorJAVA() {
		Assume.assumeTrue("Extended value types are available since Qt6", QLibraryInfo.version().majorVersion()>5);
		assertEquals(javaObject.getExtendedColor(), javaObject.property("extendedColor"));
	}
	
	@Test
	public void testFetchPropertyCustomQtInterfaceValueJAVA() {
		assertEquals(javaObject.getCustomQtInterfaceValue(), javaObject.property("customQtInterfaceValue"));
	}

	@Test
	public void testFetchPropertyDerivedQObjectJAVA() {
		assertEquals(javaObject.getDerivedQObject(), javaObject.property("derivedQObject"));
	}

	@Test
	public void testFetchPropertyCustomEnumJAVA() {
		Object prop = javaObject.property("customEnum");
		assertNotNull(prop);
		assertEquals(javaObject.getEnum(), javaObject.property("customEnum"));
	}

	@Test
	public void testFetchPropertyNumberJAVA() {
		assertEquals(javaObject.getNumber(), javaObject.property("number"));
	}

	@Test
	public void testFetchPropertyQtEnumJAVA() {
		Object prop = javaObject.property("qtEnum");
		assertNotNull(prop);
		assertEquals(Qt.AspectRatioMode.class, prop.getClass());
		assertEquals(javaObject.getQtEnum(), prop);
	}
	
	@Test
	public void testFetchPropertyIntConstJAVA() {
		Object prop = javaObject.property("intConst");
		assertNotNull(prop);
		assertEquals(42, prop);
	}
	
	@Test
	public void testFetchPropertyStaticIntConstJAVA() {
		Object prop = javaObject.property("staticIntConst");
		assertNotNull(prop);
		assertEquals(142, prop);
	}

	@Test
	public void testFetchPropertyQtFlagsJAVA() {
		Object prop = javaObject.property("qtFlags");
		assertNotNull(prop);
		assertEquals(Qt.Orientations.class, prop.getClass());
		assertEquals(javaObject.getQtFlags(), prop);
	}

	@Test
	public void testConnectSignalsAndSlots() {
		assertTrue(object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType));
	}

	@Test
	public void testSignalCustomEnumNULL() {
		Assume.assumeTrue("Checking enum null in Qt6 makes no sense", QLibraryInfo.version().majorVersion()==5);
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		javaObject.customEnumChanged.emit(null);
		assertEquals(null, object.receivedCustomEnum());
	}

	@Test
	public void testSignalCustomEnum() {
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		javaObject.customEnumChanged.emit(TestQObject.CustomEnum.Entry3);
		assertEquals(TestQObject.CustomEnum.Entry3, object.receivedCustomEnum());
	}

	@Test
	public void testSignalCustomQtEnumNULL() {
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		javaObject.customQtEnumChanged.emit(null);
		assertEquals(null, object.receivedCustomQtEnum());
	}

	@Test
	public void testSignalCustomQtEnum() {
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		javaObject.customQtEnumChanged.emit(TestQObject.CustomQtEnum.Entry3);
		assertEquals(TestQObject.CustomQtEnum.Entry3, object.receivedCustomQtEnum());
	}

	@Test
	public void testSignalQtFlags() {
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		TestQObject.CustomQtFlags flags = new TestQObject.CustomQtFlags(TestQObject.CustomQtEnum.Entry1,
				TestQObject.CustomQtEnum.Entry2);
		javaObject.customQtFlagsChanged.emit(flags);
		assertEquals(flags, object.receivedQtFlags());
	}

	@Test
	public void testSignalCustomQtValue() {
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		TestQObject.CustomQtValue customQtValue = new TestQObject.CustomQtValue();
		javaObject.customQtValueChanged.emit(customQtValue);
		assertEquals(customQtValue, object.receivedCustomQtValue());
	}
	
	@Test
	public void testSignalCustomQtInterfaceValue() {
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		TestQObject.CustomQtInterfaceValue customQtInterfaceValue = new TestQObject.CustomQtInterfaceValue();
		javaObject.customQtInterfaceValueChanged.emit(customQtInterfaceValue);
		assertEquals(customQtInterfaceValue, object.receivedCustomQtInterfaceValue());
	}

	@Test
	public void testSignalCustomJavaType() {
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		TestQObject.CustomJavaType customJavaType = new TestQObject.CustomJavaType();
		javaObject.customJavaTypeChanged.emit(customJavaType);
		assertEquals(customJavaType, object.receivedCustomJavaType());
	}

	@Test
	public void testSignalDerivedQObject() {
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		TestQObject.DerivedQObject derivedObject = new TestQObject.DerivedQObject();
		javaObject.derivedQObjectChanged.emit(derivedObject);
		assertEquals(derivedObject, object.receivedDerivedQObject());
	}
	
	@Test
	public void testSignalExtendedColor() {
		Assume.assumeTrue("Extended value types are available since Qt6", QLibraryInfo.version().majorVersion()>5);
		object.connectSignals(javaObject, ClassAnalyzerUtility.useAnnotatedType);
		TestQObject.ExtendedColor extendedColor = new TestQObject.ExtendedColor(QRgba64.fromRgba64(0x014691ab6));
		javaObject.extendedColorChanged.emit(extendedColor);
		assertEquals(extendedColor, object.receivedExtendedColor());
	}

	public static class TestQObject extends QObject {

		public enum CustomEnum {
			Entry1, Entry2, Entry3
		};

		public enum CustomQtEnum implements QtFlagEnumerator {
			Entry1(0x0010), Entry2(0x0100), Entry3(0x1000);

			CustomQtEnum(int value) {
				this.value = value;
			}

			private final int value;

			@Override
			public int value() {
				return value;
			}

			public static CustomQtEnum resolve(int i) {
				switch (i) {
				case 0:
					return null;
				case 0x0010:
					return Entry1;
				case 0x0100:
					return Entry2;
				case 0x1000:
					return Entry3;
				}
				throw new QNoSuchEnumValueException(i);
			}

			@Override
			public CustomQtFlags asFlags() {
				return new CustomQtFlags(value);
			}
		};

		public static class CustomQtFlags extends QFlags<CustomQtEnum> {
			public CustomQtFlags(int value) {
				super(value);
			}

			public CustomQtFlags(CustomQtEnum... args) {
				super(args);
			}

			private static final long serialVersionUID = -8392034916129435499L;
			
			@Override
			public CustomQtFlags clone() {
				return new CustomQtFlags(value());
			}
			@Override
			public CustomQtFlags combined(CustomQtEnum flag) {
				return new CustomQtFlags(value() | flag.value());
			}
			@Override
			public CustomQtEnum[] flags() {
				return flags(CustomQtEnum.values());
			}
		}

		public static class DerivedQObject extends QObject {

			public DerivedQObject() {
				super((QObject) null);
				this.setObjectName("DerivedQObject#" + Long.toHexString(General.internalAccess.nativeId(this)));
			}

		}

		public static class CustomQtValue extends QGraphicsItem.Impl {
			public CustomQtValue() {
				super();
			}

			@Override
			@QtUninvokable
			public QRectF boundingRect() {
				return null;
			}

			@Override
			@QtUninvokable
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
		}

		public static class CustomQtInterfaceValue implements QGraphicsItem {
			public CustomQtInterfaceValue() {
				super();
			}

			@Override
			@QtUninvokable
			public QRectF boundingRect() {
				return null;
			}

			@Override
			@QtUninvokable
			public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {
			}
		}

		public static class CustomJavaType {

		}
		
		public static class ExtendedColor extends QColor{
			
			ExtendedColor(int rgb) {
				super(rgb);
				text = this.name();
			}

			ExtendedColor(String name) {
				super(name);
				text = this.name();
			}

			ExtendedColor(QRgba64 rgba64) {
				super(rgba64);
				text = this.name();
			}

			ExtendedColor() {
				super();
				text = this.name();
			}

			private String text;

			@QtPropertyReader
			public String text() {
				return text;
			}

			@Override
			public void writeTo(QDataStream s) {
				super.writeTo(s);
				s.append(text);
			}

			@Override
			public void readFrom(QDataStream s) {
				super.readFrom(s);
				text = s.readString();
			}

			@Override
			public boolean equals(Object other) {
				return super.equals(other) && other instanceof ExtendedColor && Objects.equals(text, ((ExtendedColor)other).text);
			}

			@Override
			public int hashCode() {
				int result = 1;
	            result = 31 * result + super.hashCode();
	            result = 31 * result + (text == null ? 0 : text.hashCode());
				return result;
			}

			@Override
			public String toString() {
				return super.toString();
			}

			@Override
			public ExtendedColor clone() {
				return new ExtendedColor(this.rgba64());
			}
		}

		public TestQObject(QObject obj) {
			super(obj);
		}

		private QStringListModel myModel;
		private List<String> myList;
		private DerivedQObject derivedQObject = new DerivedQObject();
		private CustomQtValue customQtValue = new CustomQtValue();
		private CustomQtInterfaceValue customQtInterfaceValue = new CustomQtInterfaceValue();
		private CustomJavaType customJavaType = new CustomJavaType();
		private CustomQtEnum customQtEnum = CustomQtEnum.Entry1;
		private CustomQtFlags customQtFlags = new CustomQtFlags();
		private Qt.AspectRatioMode qtEnum = Qt.AspectRatioMode.KeepAspectRatio;
		private Qt.Orientations qtFlags = new Qt.Orientations(Qt.Orientation.Horizontal);
		private final QColor colorPtr = new QColor(0x01abcdef);
		private ExtendedColor extendedColor = new ExtendedColor(QRgba64.fromRgba64(0x01abcdef));
		public final int intConst = 42;
		public static final int staticIntConst = 142;
		
		@QtPropertyMember(name="member")
		private String memberProperty = "QtPropertyMember";

		{
			setObjectName("MyDataSet");
			myList = new ArrayList<String>();
			myList.add("test item 1");
			myList.add("test item 2");
			myList.add("test item 3");
			myList.add("test item 4");
			myModel = new QStringListModel(myList);
			myModel.setObjectName("model#" + Long.toHexString(General.internalAccess.nativeId(myModel)));
		}

		@QtPropertyReader(name = "qobject")
		public QObject getObject() {
			QObject o = new QObject();
			o.setObjectName("custom property");
			return o;
		}
		
		@QtPropertyReader(name = "model")
		public QStringListModel getModel() {
			return myModel;
		}

		@QtPropertyWriter(name = "model")
		public void setModel(QStringListModel myModel) {
			this.myModel = myModel;
		}

		public boolean testModel(QStringListModel myModel) {
			return myModel == this.myModel;
		}

		@QtPropertyReader(name = "intArray")
		public int[] getIntArray() {
			return new int[]{1,2,3,4,5};
		}

		public boolean testIntArray(int[] array) {
			return Arrays.equals(array, new int[]{1,2,3,4,5});
		}

		@QtPropertyReader(name = "color")
		public QColor getColor() {
			return new QColor("red");
		}

		public boolean testColor(QColor c) {
			return new QColor("red").equals(c);
		}
		
		@QtPropertyReader(name = "colorPtr")
		public @QtPointerType QColor getColorPtr() {
			return colorPtr;
		}

		public boolean testColorPtr(@QtPointerType QColor c) {
			return colorPtr==c;
		}

		@QtPropertyReader(name = "customEnum")
		public CustomEnum getEnum() {
			return CustomEnum.Entry1;
		}

		public boolean testCustomEnum(CustomEnum e) {
			return System.identityHashCode(e) == System.identityHashCode(CustomEnum.Entry1);
		}

		public boolean testEnum(CustomEnum e) {
			return System.identityHashCode(e) == System.identityHashCode(CustomEnum.Entry1);
		}

		@QtPropertyReader(name = "number")
		public Number getNumber() {
			return BigInteger.valueOf(12345);
		}

		public boolean testNumber(Number e) {
			return BigInteger.valueOf(12345).equals(e);
		}

		@QtPropertyNotify(name = "model")
		public final Signal1<QStringListModel> modelChanged = new Signal1<QStringListModel>();

		@QtPropertyNotify(name = "customEnum")
		public final Signal1<CustomEnum> customEnumChanged = new Signal1<CustomEnum>();

		@QtPropertyNotify(name = "customQtEnum")
		public final Signal1<CustomQtEnum> customQtEnumChanged = new Signal1<CustomQtEnum>();

		@QtPropertyNotify(name = "customQtFlags")
		public final Signal1<CustomQtFlags> customQtFlagsChanged = new Signal1<CustomQtFlags>();

		@QtPropertyNotify(name = "customQtValue")
		public final Signal1<CustomQtValue> customQtValueChanged = new Signal1<CustomQtValue>();

		@QtPropertyNotify(name = "customQtInterfaceValue")
		public final Signal1<CustomQtInterfaceValue> customQtInterfaceValueChanged = new Signal1<CustomQtInterfaceValue>();

		@QtPropertyNotify(name = "customJavaType")
		public final Signal1<CustomJavaType> customJavaTypeChanged = new Signal1<CustomJavaType>();

		@QtPropertyNotify(name = "customColor")
		public final Signal1<QColor> customColorChanged = new Signal1<QColor>();

		@QtPropertyNotify(name = "customColorPtr")
		public final Signal1<@QtPointerType QColor> customColorPtrChanged = new Signal1<>();

		@QtPropertyNotify(name = "intArray")
		public final Signal1<int[]> customIntArrayChanged = new Signal1<>();

		@QtPropertyNotify(name = "derivedQObject")
		public final Signal1<DerivedQObject> derivedQObjectChanged = new Signal1<>();

		@QtPropertyNotify(name = "extendedColor")
		public final Signal1<ExtendedColor> extendedColorChanged = new Signal1<>();

		@QtPropertyReader(name = "list")
		public List<String> getList() {
			return myList;
		}

		@QtPropertyWriter(name = "list")
		public void setList(List<String> myList) {
			this.myList = myList;
		}

		public boolean testList(List<String> myList) {
			return this.myList.equals(myList);
		}

		@QtPropertyReader(name = "derivedQObject")
		public DerivedQObject getDerivedQObject() {
			return derivedQObject;
		}

		public boolean testDerivedQObject(DerivedQObject derivedQObject) {
			return this.derivedQObject == derivedQObject;
		}

		@QtPropertyReader(name = "customQtValue")
		public CustomQtValue getCustomQtValue() {
			return customQtValue;
		}

		public boolean testCustomQtValue(CustomQtValue customQtValue) {
			return this.customQtValue.equals(customQtValue);
		}

		@QtPropertyReader(name = "customQtInterfaceValue")
		public CustomQtInterfaceValue getCustomQtInterfaceValue() {
			return customQtInterfaceValue;
		}

		public boolean testCustomQtInterfaceValue(CustomQtInterfaceValue customQtInterfaceValue) {
			return this.customQtInterfaceValue.equals(customQtInterfaceValue);
		}

		@QtPropertyReader(name = "customJavaType")
		public CustomJavaType getCustomJavaType() {
			return customJavaType;
		}

		public boolean testCustomJavaType(CustomJavaType customJavaType) {
			return this.customJavaType == customJavaType;
		}

		@QtPropertyReader(name = "customQtEnum")
		public CustomQtEnum getCustomQtEnum() {
			return customQtEnum;
		}

		@QtPropertyReader(name = "customQtEnum2")
		public CustomQtEnum getCustomQtEnum2() {
			return null;
		}

		public boolean testCustomQtEnum(CustomQtEnum customQtEnum) {
			return this.customQtEnum == customQtEnum;
		}

		public boolean testCustomQtEnum2(CustomQtEnum customQtEnum) {
			return customQtEnum == null;
		}

		@QtPropertyReader(name = "customQtFlags")
		public CustomQtFlags getCustomQtFlags() {
			return customQtFlags;
		}

		public boolean testCustomQtFlags(CustomQtFlags customQtFlags) {
			return this.customQtFlags == customQtFlags;
		}

		@QtPropertyReader(name = "qtEnum")
		public Qt.AspectRatioMode getQtEnum() {
			return qtEnum;
		}

		public boolean testQtEnum(Qt.AspectRatioMode customQtEnum) {
			return this.qtEnum == customQtEnum;
		}

		@QtPropertyReader(name = "qtFlags")
		public Qt.Orientations getQtFlags() {
			return qtFlags;
		}

		public boolean testQtFlags(Qt.Orientations customQtFlags) {
			return this.qtFlags.equals(customQtFlags);
		}

		public ExtendedColor getExtendedColor() {
			return extendedColor;
		}

		public void setExtendedColor(ExtendedColor extendedColor) {
			this.extendedColor = extendedColor;
		}
		
		public boolean testExtendedColor(ExtendedColor extendedColor) {
			return this.extendedColor.equals(extendedColor);
		}
	}
	
	public static void main(String args[]) {
		org.junit.runner.JUnitCore.main(TestPropertyAndMethodCall.class.getName());
	}
}
