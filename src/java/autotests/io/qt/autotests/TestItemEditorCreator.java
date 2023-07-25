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
import static org.junit.Assert.assertTrue;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtPropertyReader;
import io.qt.QtPropertyUser;
import io.qt.QtPropertyWriter;
import io.qt.core.QByteArray;
import io.qt.core.Qt;
import io.qt.core.Qt.WindowFlags;
import io.qt.core.Qt.WindowType;
import io.qt.widgets.QItemEditorCreator;
import io.qt.widgets.QItemEditorCreatorBase;
import io.qt.widgets.QItemEditorFactory;
import io.qt.widgets.QPushButton;
import io.qt.widgets.QStandardItemEditorCreator;
import io.qt.widgets.QWidget;

public class TestItemEditorCreator extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	public static class Editor extends QWidget{
		public Editor() {
			super();
		}

		public Editor(QWidget parent, WindowFlags f) {
			super(parent, f);
		}

		public Editor(QWidget parent, WindowType... f) {
			super(parent, f);
		}

		private int intValue;

		public Editor(QWidget parent) {
			super(parent);
		}
		
		@QtPropertyReader(name="intValue")
		@QtPropertyUser
		public int intValue() {
			return this.intValue;
		}

		@QtPropertyWriter(name="intValue")
		public void setIntValue(int intValue) {
			this.intValue = intValue;
		}
	}
	
	public abstract static class AbstractEditor extends QWidget{
		public AbstractEditor(QWidget parent) {
			super(parent);
		}
	}
	
	public static class UncreatableEditor extends QWidget{
	}
	
    @Test
    public void testItemEditorCreatorBase() {
    	QItemEditorFactory factory = new QItemEditorFactory();
    	factory.registerEditor(Qt.ItemDataRole.UserRole+5, new QItemEditorCreatorBase() {
			
			@Override
			public QByteArray valuePropertyName() {
				return new QByteArray("test1");
			}
			
			@Override
			public QWidget createWidget(QWidget parent) {
				return new Editor(parent);
			}
		});
    	assertEquals(new QByteArray("test1"), factory.valuePropertyName(Qt.ItemDataRole.UserRole+5));
    	assertTrue(factory.createEditor(Qt.ItemDataRole.UserRole+5, null) instanceof Editor);
    }
    
    @Test
    public void testItemEditorCreator_Editor() {
    	QItemEditorFactory factory = new QItemEditorFactory();
    	factory.registerEditor(Qt.ItemDataRole.UserRole+5, new QItemEditorCreator<>("test2", Editor.class));
    	assertEquals(new QByteArray("test2"), factory.valuePropertyName(Qt.ItemDataRole.UserRole+5));
    	assertTrue(factory.createEditor(Qt.ItemDataRole.UserRole+5, null) instanceof Editor);
    }
    
    @Test
    public void testItemEditorCreator_AbstractEditor() {
    	try {
    		new QItemEditorCreator<>("test2", AbstractEditor.class);
			assertTrue(false);
		} catch (RuntimeException e) {
	    	assertEquals("Cannot construct QItemEditorCreator for abstract widget type "+AbstractEditor.class.getName()+".", e.getMessage());
		}
    }
    
    @Test
    public void testItemEditorCreator_UncreatableEditor() {
    	try {
    		new QItemEditorCreator<>("test2", UncreatableEditor.class);
			assertTrue(false);
		} catch (RuntimeException e) {
	    	assertEquals("Cannot construct QItemEditorCreator for widget type "+UncreatableEditor.class.getName()+" due to missing constructor T(QWidget).", e.getMessage());
		}
    }
    
    @Test
    public void testStandardItemEditorCreator_Editor() {
    	QItemEditorFactory factory = new QItemEditorFactory();
    	factory.registerEditor(Qt.ItemDataRole.UserRole+5, new QStandardItemEditorCreator<>(Editor.class));
    	assertEquals(new QByteArray("intValue"), factory.valuePropertyName(Qt.ItemDataRole.UserRole+5));
    	assertTrue(factory.createEditor(Qt.ItemDataRole.UserRole+5, null) instanceof Editor);
    }
    
    @Test
    public void testStandardItemEditorCreator_AbstractEditor() {
    	try {
    		new QStandardItemEditorCreator<>(AbstractEditor.class);
			assertTrue(false);
		} catch (RuntimeException e) {
	    	assertEquals("Cannot construct QStandardItemEditorCreator for abstract widget type "+AbstractEditor.class.getName()+".", e.getMessage());
		}
    }
    
    @Test
    public void testStandardItemEditorCreator_with_NonLambda_handle() {
		class Fun implements QStandardItemEditorCreator.Factory<QPushButton>{
			private static final long serialVersionUID = 5891633777036928275L;

			@Override
			public QPushButton apply(QWidget t) {
				return new QPushButton(t);
			}
			
		}
		new QStandardItemEditorCreator<>(new Fun());
    }
    
    @Test
    public void testStandardItemEditorCreator_with_handle() {
    	QItemEditorFactory factory = new QItemEditorFactory();
    	factory.registerEditor(Qt.ItemDataRole.UserRole+5, new QStandardItemEditorCreator<Editor>(Editor::new));
    	assertEquals(new QByteArray("intValue"), factory.valuePropertyName(Qt.ItemDataRole.UserRole+5));
    	assertTrue(factory.createEditor(Qt.ItemDataRole.UserRole+5, null) instanceof Editor);
    }
    
    @Test
    public void testStandardItemEditorCreator_with_LambdaFunction_handle() {
		new QStandardItemEditorCreator<Editor>(parent->new Editor(parent));
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestItemEditorCreator.class.getName());
    }
}
