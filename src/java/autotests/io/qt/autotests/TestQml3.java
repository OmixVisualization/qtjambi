/****************************************************************************
**
** Copyright (C) 2009-2026 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.OptionalInt;
import java.util.concurrent.atomic.AtomicInteger;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.QtInvokable;
import io.qt.QtPropertyReader;
import io.qt.autotests.generated.General;
import io.qt.core.QAbstractListModel;
import io.qt.core.QAbstractTableModel;
import io.qt.core.QEventLoop;
import io.qt.core.QList;
import io.qt.core.QLogging;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QRandomGenerator;
import io.qt.core.QStringListModel;
import io.qt.core.QTimer;
import io.qt.core.QVariant;
import io.qt.core.Qt;
import io.qt.core.Qt.Orientation;
import io.qt.gui.QFontDatabase;
import io.qt.qml.QJSValue;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;
import io.qt.qml.QtQml;
import io.qt.quick.QQuickItem;
import io.qt.quick.QQuickWindow;
import io.qt.widgets.QApplication;

public class TestQml3 extends ApplicationInitializer{
	
	@BeforeClass
	public static void testInitialize() throws Exception {
		ApplicationInitializer.testInitializeWithGui();
	}
	
	public static class RootModel extends QObject{
		protected static class ItemModel extends QAbstractListModel{
			public ItemModel() {
			}

			@Override
			public Object data(@NonNull QModelIndex index, int role) {
				return null;
			}

			@Override
			public @NonNull QModelIndex index(int row, int column, @NonNull QModelIndex parent) {
				return createIndex(row, column);
			}

			@Override
			public int rowCount(@NonNull QModelIndex parent) {
				return 2;
			}
		}
		
		private final ItemModel itemModel = new ItemModel();
		
		public ItemModel getItemModel() {
			return itemModel;
		}
	}
	
	public static class PropertyAndMethodProvider extends QObject{
		private String text = "PropertyAndMethodProviderText";

		@QtInvokable
		@QtPropertyReader
		public String text() {
			return text;
		}

		public void setText(String text) {
			this.text = text;
		}
	}
	
	public static class PropertyAndMethodProvider2 extends PropertyAndMethodProvider{
		public String text() {
			return "PropertyAndMethodProviderText2";
		}
	}
	
	public static class Context extends QObject{
		public final Signal1<Object> rootModelChanged = new Signal1<>();
		public final Signal1<PropertyAndMethodProvider> propertyAndMethodProviderChanged = new Signal1<>();
	}
	
	@Test
    public void test1() {
		QQmlEngine engine = new QQmlEngine();
		Context context = new Context();
		RootModel rm = new RootModel();
		engine.rootContext().setContextProperty("Context", context);
		QQmlComponent component = new QQmlComponent(engine);
		component.setData("import QtQuick 2.0\n"
				+ "Item{\n"
				+ "id: _this\n"
				+ "property var root: null\n"
				+ "property var itemModel: root ? root.itemModel : null\n"
				+ "property var index: root && root.itemModel ? root.itemModel.index(0,0) : null\n"
				+ "Connections{\n"
				+ "target: Context\n"
				+ "function onRootModelChanged(r){\n"
				+ "_this.root = r;\n"
				+ "}\n"
				+ "}\n"
				+ "}", "qrc:");
//		System.out.println(component.errorString());
		QObject root = component.create();
		Assert.assertTrue(root!=null);
//		for(QMetaProperty p : root.metaObject().properties()) {
//			System.out.println(p.metaType().name()+" "+p.name());
//		}
		System.out.println("root="+root.property("root"));
		System.out.println("itemModel="+root.property("itemModel"));
		System.out.println("index="+root.property("index"));
		context.rootModelChanged.emit(rm);
		System.out.println("root="+root.property("root"));
		System.out.println("itemModel="+root.property("itemModel"));
		System.out.println("index="+root.property("index"));
		engine.dispose();
		rm.dispose();
	}
	
	@Test
    public void test2() {
		QMetaObject mo = QMetaObject.forType(PropertyAndMethodProvider.class);
		for(QMetaProperty p : mo.properties()) {
			if(p.enclosingMetaObject()==mo)
				System.out.println(p.typeName()+" "+p.name());
		}
		for(QMetaMethod m : mo.methods()) {
			if(m.enclosingMetaObject()==mo)
				System.out.println(m.typeName()+" "+m.cppMethodSignature());
		}
		QQmlEngine engine = new QQmlEngine();
		Context context = new Context();
		engine.rootContext().setContextProperty("Context", context);
		QQmlComponent component = new QQmlComponent(engine);
		component.setData("import QtQuick 2.0\n"
				+ "Item{\n"
				+ "id: _this\n"
				+ "property var text\n"
				+ "Connections{\n"
				+ "target: Context\n"
				+ "function onPropertyAndMethodProviderChanged(p){\n"
				+ "var text = p.text;\n"
				+ "_this.text = p.text;\n"
//				+ "var text2 = p.text();\n"
				+ "}\n"
				+ "}\n"
				+ "}", "qrc:");
		QObject root = component.create();
		Assert.assertTrue(root!=null);
		PropertyAndMethodProvider prov = new PropertyAndMethodProvider2();
		context.propertyAndMethodProviderChanged.emit(prov);
		Assert.assertEquals("PropertyAndMethodProviderText2", root.property("text"));
		engine.dispose();
	}
	
	public static class ExceptionPropertyProvider extends QObject{
		ExceptionPropertyProvider() {
			super();
		}

		ExceptionPropertyProvider(QPrivateConstructor constructor) {
			super(constructor);
		}

		ExceptionPropertyProvider(@Nullable QObject parent) {
			super(parent);
		}

		public final Signal0 numberChanged = new Signal0();
		public final Signal0 countChanged = new Signal0();
		public final Signal0 throwingChanged = new Signal0();
		private int number;
		private int count;
		private boolean throwing;

		public int getNumber() {
			if(throwing)
				throw new RuntimeException("Cannot get property");
			return number;
		}

		public void setNumber(int number) {
			if(this.number != number) {
				this.number = number;
				numberChanged.emit();
			}
		}

		public boolean isThrowing() {
			return throwing;
		}

		public void setThrowing(boolean throwing) {
			if(this.throwing != throwing) {
				this.throwing = throwing;
				throwingChanged.emit();
			}
		}
		
		public void call(String text) {
			if(throwing)
				throw new RuntimeException(text);
		}

		public int getCount() {
			return count;
		}

		public void setCount(int count) {
			if(this.count != count) {
				if(throwing)
					throw new RuntimeException("Cannot set property");
				this.count = count;
				countChanged.emit();
			}
		}
	}
	
	public static class ExceptionProvoker extends QObject{
		ExceptionProvoker() {
			super();
		}

		ExceptionProvoker(QPrivateConstructor constructor) {
			super(constructor);
		}

		ExceptionProvoker(@Nullable QObject parent) {
			super(parent);
		}
		public final Signal1<Integer> numberChanged = new Signal1<>();
		public final Signal1<Integer> countChanged = new Signal1<>();
		public final Signal0 performClick = new Signal0();
		public final void sendRandomNumber() {
			numberChanged.emit(QRandomGenerator.global().bounded(0,99));
		}
		public final void sendRandomCount() {
			countChanged.emit(QRandomGenerator.global().bounded(0, 99));
		}
		public final void sendClick() {
			performClick.emit();
		}
	}
	
	@Test
    public void testMetaCallExceptions() {
		QLogging.qInstallMessageHandler((t,c,msg)->System.out.println(msg));
		QQmlEngine engine = new QQmlEngine();
		QtQml.qmlRegisterType(ExceptionPropertyProvider.class, "test", 1, 0, "ExceptionPropertyProvider");
		QtQml.qmlRegisterType(ExceptionProvoker.class, "test", 1, 0, "ExceptionProvoker");
		QQmlComponent component = new QQmlComponent(engine);
		component.setData("import QtQuick 2.15\n" + 
				"import QtQuick.Controls 2.15\n" + 
				"import test 1.0\n" + 
				"\n" + 
				"ApplicationWindow {\n" + 
				"    visible: true\n" + 
				"    width: 400\n" + 
				"    height: 300\n" +  
				"\n" + 
				"    Grid  {\n" + 
				"        anchors.centerIn: parent\n" + 
				"        rows: 3\n" + 
				"        columns: 2\n" + 
				"\n" + 
				"            Text {\n" + 
				"                text: \"Number: \"+prv.number\n" + 
				"                font.pixelSize: 18\n" + 
				"                horizontalAlignment: Text.AlignHCenter\n" + 
				"            }\n" + 
				"            SpinBox {\n" + 
				"                id: numberSpinBox\n" + 
				"                objectName: \"numberSpinBox\"\n" + 
				"                //displayText: \"Number:\"\n" + 
				"            }\n" + 
				"            Text {\n" + 
				"                text: \"Count: \"+prv.count\n" + 
				"                font.pixelSize: 18\n" + 
				"                horizontalAlignment: Text.AlignHCenter\n" + 
				"            }\n" + 
				"            SpinBox {\n" + 
				"                id: countSpinBox\n" + 
				"                objectName: \"countSpinBox\"\n" + 
				"                //displayText: \"Count:\"\n" + 
				"            }\n" + 
				"            Button {\n" + 
				"                id: caller\n" + 
				"                text: \"Call throwing java method\"\n" + 
				"                objectName: \"caller\"\n" + 
				"                onClicked: prv.call(\"Test Exception\")\n" + 
				"            }\n" + 
				"            Button {\n" + 
				"                id: enableExceptionButton\n" + 
				"                objectName: \"checker\"\n" + 
				"                text: \"Enable Exceptions\"\n" + 
				"                checkable: true\n" + 
				"            }\n" + 
				"    }\n" + 
				"    ExceptionPropertyProvider { id: prv; throwing: enableExceptionButton.checked; count: countSpinBox.value; number: numberSpinBox.value }\n" + 
				"    ExceptionProvoker { id: pvk; onNumberChanged: value => {numberSpinBox.value = value;}; onCountChanged: value => {countSpinBox.value = value;}; onPerformClick: {caller.clicked();} }\n" + 
				"}", "qrc:");
		Assert.assertFalse(component.errorString(), component.isError());
		QQuickWindow root = component.create(QQuickWindow.class);
		Assert.assertTrue(root!=null);
		ExceptionProvoker provoker = root.findChild(ExceptionProvoker.class);
		QQuickItem caller = root.findChild(QQuickItem.class, "caller");
		Assert.assertTrue(caller!=null);
		QQuickItem checker = root.findChild(QQuickItem.class, "checker");
		Assert.assertTrue(checker!=null);
		QQuickItem numberSpinBox = root.findChild(QQuickItem.class, "numberSpinBox");
		Assert.assertTrue(numberSpinBox!=null);
		QQuickItem countSpinBox = root.findChild(QQuickItem.class, "countSpinBox");
		Assert.assertTrue(countSpinBox!=null);
		QEventLoop eventLoop = new QEventLoop();
		root.closing.connect(eventLoop, QEventLoop::quit);
		QTimer.singleShot(600, provoker, ExceptionProvoker::sendRandomCount);
		QTimer.singleShot(700, provoker, ExceptionProvoker::sendRandomNumber);
		QTimer.singleShot(1000, caller, "click()");
		QTimer.singleShot(1100, numberSpinBox, "increase()");
		QTimer.singleShot(1200, countSpinBox, "increase()");
		QTimer.singleShot(1300, numberSpinBox, "increase()");
		QTimer.singleShot(1400, countSpinBox, "increase()");
		QTimer.singleShot(1500, numberSpinBox, "increase()");
		QTimer.singleShot(1600, countSpinBox, "increase()");
		QTimer.singleShot(2000, checker, "click()");
		QTimer.singleShot(3000, caller, "click()");
		QTimer.singleShot(4000, numberSpinBox, "increase()");
		QTimer.singleShot(5000, countSpinBox, "increase()");
		QTimer.singleShot(5500, provoker, ExceptionProvoker::sendClick);
		QTimer.singleShot(6000, provoker, ExceptionProvoker::sendRandomCount);
		QTimer.singleShot(7000, provoker, ExceptionProvoker::sendRandomNumber);
		QTimer.singleShot(15000, eventLoop, QEventLoop::quit);
		eventLoop.exec();
		root.dispose();
		engine.dispose();
	}
	
	@Test
    public void testExceptionModel() {
		QLogging.qInstallMessageHandler((t,c,msg)->System.out.println(msg));
		QAbstractTableModel model = new QAbstractTableModel() {
			boolean throwing = false;
			@Override
			public int rowCount(QModelIndex parent) {
				return 3;
			}
			
			@Override
			public int columnCount(QModelIndex parent) {
				return 2;
			}
			
			@Override
			public Object data(QModelIndex index, int role) {
				if(throwing)
					throw new RuntimeException("Table Exception");
				return String.format("[%1$s,%2$s]", index.row(), index.clone());
			}
			@SuppressWarnings("unused")
			public void increase() {
				throwing = true;
				dataChanged.emit(index(0, 0), index(2, 1), QList.ofInt(Qt.ItemDataRole.DisplayRole));
			}

			@Override
			public Object headerData(int section, @NonNull Orientation orientation, int role) {
				if(throwing)
					throw new RuntimeException("Table Exception");
				return super.headerData(section, orientation, role);
			}
		};
		QLogging.qInstallMessageHandler((t,c,msg)->System.out.println(msg));
		QQmlEngine engine = new QQmlEngine();
		engine.rootContext().setContextProperty("Model", model);
		QQmlComponent component = new QQmlComponent(engine);
		component.setData("import QtQuick 2.15\n"
				+ "import QtQuick.Controls 2.15\n"
				+ "ApplicationWindow{\n"
				+ "    visible: true\n" 
				+ "    width: 400\n" 
				+ "    height: 300\n"
				+ "    property bool active: false\n"
				+ "    TableView{\n"
				+ "        anchors.fill: parent\n"
				+ "        model: Model\n"
				+ "    delegate: Rectangle {\n" + 
				"        implicitWidth: 100\n" + 
				"        implicitHeight: 50\n" + 
				"        Text {\n" + 
				"            text: display\n" + 
				"        }\n"
				+ "      }\n" + 
				"    }\n"
				+ "}", "qrc:");
		Assert.assertFalse(component.errorString(), component.isError());
		QQuickWindow view = component.create(QQuickWindow.class);
		view.show();
		QTimer.singleShot(1000, model, ()->{QMetaObject.invokeMethod(model, "increase()");});
		QTimer.singleShot(2000, QApplication::quit);
		QApplication.exec();
		view.close();
		view.dispose();
		component.dispose();
		engine.dispose();
	}
    
    static class MyObject extends QStringListModel{
    	static final AtomicInteger created = new AtomicInteger();
    	static final AtomicInteger deleted = new AtomicInteger();
    	MyObject(){
    		super(QFontDatabase.families());
//    		QJSEngine.setObjectOwnership(this, QJSEngine.ObjectOwnership.JavaScriptOwnership);
    		created.incrementAndGet();
    		General.internalAccess.registerCleaner(this, ()->{
    			System.out.println("Java-deleting MyObject");
    			deleted.incrementAndGet();
    		});
    		destroyed.connect(()->{
    			System.out.println("Native-deleting MyObject");
    		});
    	}
    	private String text = "TEST";

		public String getText() {
			return text;
		}

		public void setText(String text) {
			this.text = text;
		}
    }
    
    static class MyValue{
    }
    
    static class MyRoot extends QObject{
    	final ArrayList<Object> receivedObjects = new ArrayList<>();
    	//@QtMetaType(name = "_jobject*") 
    	final Signal1<MyValue> myValueChanged = new Signal1<>();
    	final Signal1<Optional<MyValue>> myOptionalValueChanged = new Signal1<>();
    	final Signal1<OptionalInt> intOptChanged = new Signal1<>();
    	private final QQmlEngine engine;
    	public MyRoot(QQmlEngine engine) {
			this.engine = engine;
//			System.out.println(QMetaMethod.fromSignal(myValueChanged).cppMethodSignature());
//			System.out.println(QMetaMethod.fromSignal(intOptChanged).cppMethodSignature());
		}
		@QtPropertyReader(name="object")
    	public QJSValue object() {
    		return engine.newQObject(new MyObject());
    	}
    	@QtPropertyReader(name="list")
    	public List<MyObject> list() {
    		return List.of(new MyObject());
    	}
//    	@QtPropertyReader(name="variant")
//    	public Object variant() {
//    		return List.of(new MyObject());
//    	}
    	@QtInvokable
    	public void runGC() {
    		var v = MyObject.created.get();
    		assertTrue(v>0);
    		for (int i = 0; i < 200 && MyObject.deleted.get()!=v; i++) {
        		ApplicationInitializer.runGC();				
        		Thread.yield();
			}
//    		assertEquals(v, MyObject.deleted.get());
    	}
    	@QtInvokable
    	public void deleteObject(QObject o) {
    		o.dispose();
    	}
    	@QtInvokable
    	public void testObject(Object o) {
    		receivedObjects.add(o);
    	}
    }
    
	@Test
	public void testOptional() {
//		QMetaType.fromType(MyValue.class);
//		System.out.println(QMetaType.fromType(java.util.OptionalInt.class).name());
//		System.out.println(QMetaType.fromType(java.util.OptionalLong.class).name());
//		System.out.println(QMetaType.fromType(java.util.OptionalDouble.class).name());
//		System.out.println(QMetaType.fromType(java.util.Optional.class, QMetaType.fromType(MyValue.class)).name());
//		System.out.println(QMetaType.fromName("QSharedPointer<JObjectWrapper<io::qt::autotests::TestQmlException::MyValue>>").name());
		QLogging.qInstallMessageHandler((a,b,c)->{
			System.out.println(c);
		});
		QQmlEngine engine = new QQmlEngine();
		MyRoot root = new MyRoot(engine);
		engine.newQObject(root);
		engine.rootContext().setContextProperty("myroot", root);
		engine.rootContext().setContextProperty("jnull", new QVariant(QMetaType.fromType(MyValue.class), null));
		QQmlComponent component = new QQmlComponent(engine);
		component.setData("import QtQml\n"
				+ "import QtQuick\n"
				+ "import QtQuick.Controls\n"
				+ "Window{\n"
				+ "  visible: true\n"
				+ "  ListView{\n"
				+ "    id: list\n"
				+ "    anchors.fill: parent\n"
				+ "    property var text\n"
				+ "    model: myroot.object\n"
				+ "    visible: !model.empty\n"
				+ "    delegate: ItemDelegate {\n"
				+ "      text: model.display\n"
				+ "      width: list.width\n"
				+ "    }\n"
				+ "  }\n"
				+ "  Connections{\n"
				+ "    target: myroot\n"
				+ "    function onMyValueChanged(m){\n"
				+ "        if(m==jnull){console.log('null');}\n"
				+ "        myroot.testObject(m);\n"
//				+ "        console.log(m);\n"
				+ "    }\n"
				+ "  }\n"
				+ "  Connections{\n"
				+ "    target: myroot\n"
				+ "    function onMyOptionalValueChanged(m){\n"
				+ "        if(m==null){console.log('null');}\n"
				+ "        myroot.testObject(m);\n"
//				+ "        console.log(m);\n"
				+ "    }\n"
				+ "  }\n"
				+ "  Connections{\n"
				+ "    target: myroot\n"
				+ "    function onIntOptChanged(i){\n"
				+ "        if(i==null){console.log('null');}\n"
				+ "        myroot.testObject(i);\n"
//				+ "        console.log(i);\n"
				+ "    }\n"
				+ "  }\n"
				+ "  Component.onCompleted: {\n"
//				+ "    var o = list.model;\n"
//				+ "    console.log(o, o.text);\n"
//				+ "    object = o;\n"
//				+ "    root.deleteObject(o);\n" 
//				+ "    console.log(o, o.text);\n"
//				+ "    myroot.runGC();\n"
//				+ "    console.log(list.model, list.model?.text);\n"
//				+ "    console.log(o[0]);\n"
//				+ "    text = model.text;\n"
				+ "  }\n"
				+ "  onClosing: {\n"
//				+ "    var o = list.model;\n"
//				+ "    console.log(o);\n"
				+ "  }\n"
				+ "}", ":/");
		component.create();
//		assertEquals("", item.property("text"));
		QTimer.singleShot(500, root::runGC);
		QTimer.singleShot(100, ()->{
			root.myValueChanged.emit(null);
			root.myValueChanged.emit(new MyValue());
			root.myOptionalValueChanged.emit(Optional.ofNullable(null));
			root.myOptionalValueChanged.emit(Optional.of(new MyValue()));
			root.intOptChanged.emit(OptionalInt.of(6));
//			QApplication.quit();
		});
		QTimer.singleShot(1500, QApplication::quit);
		QApplication.exec();
		Assert.assertEquals(5, root.receivedObjects.size());
		Assert.assertEquals(null, root.receivedObjects.get(0));
		Assert.assertTrue(root.receivedObjects.get(1) instanceof MyValue);
		Assert.assertEquals(Optional.ofNullable(null), root.receivedObjects.get(2));
		Assert.assertTrue(root.receivedObjects.get(3) instanceof Optional && ((Optional<?>)root.receivedObjects.get(3)).get() instanceof MyValue);
		Assert.assertTrue(root.receivedObjects.get(4) instanceof OptionalInt);
		Assert.assertEquals(6, ((OptionalInt)root.receivedObjects.get(4)).getAsInt());
		engine.dispose();
	}
}
