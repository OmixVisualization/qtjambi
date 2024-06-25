/****************************************************************************
**
** Copyright (C) 2009-2024 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NonNull;
import io.qt.QtInvokable;
import io.qt.QtPropertyReader;
import io.qt.core.QAbstractListModel;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.qml.QQmlComponent;
import io.qt.qml.QQmlEngine;

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
}
