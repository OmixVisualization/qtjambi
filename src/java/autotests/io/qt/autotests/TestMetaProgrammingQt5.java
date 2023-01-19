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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import org.junit.Assert;
import org.junit.Test;

import io.qt.QUnsuccessfulInvocationException;
import io.qt.QtExtensibleEnum;
import io.qt.QtInvokable;
import io.qt.QtPropertyDesignable;
import io.qt.QtPropertyNotify;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyRequired;
import io.qt.QtPropertyResetter;
import io.qt.QtPropertyUser;
import io.qt.QtPropertyWriter;
import io.qt.QtShortEnumerator;
import io.qt.autotests.generated.FlagsAndEnumTest;
import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QMetaEnum;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaObject.AbstractPrivateSignal1;
import io.qt.core.QMetaProperty;
import io.qt.core.QMetaType;
import io.qt.core.QMetaType.GenericFlags;
import io.qt.core.QModelIndex;
import io.qt.core.QObject;
import io.qt.core.QThread;
import io.qt.core.QUrl;
import io.qt.core.QVariant;
import io.qt.core.Qt;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QStandardItemModel;
import io.qt.widgets.QAbstractButton;
import io.qt.widgets.QApplication;
import io.qt.widgets.QCheckBox;
import io.qt.widgets.QPushButton;

public class TestMetaProgrammingQt5 extends ApplicationInitializer {
	
	private static class FullOfPropertiesSuper extends QObject {

		public FullOfPropertiesSuper() {
			super();
		}
		
        @QtPropertyReader()
        public final int readOnlyProperty() { return 0; }

        @QtPropertyWriter(enabled=false)
        public final void setReadOnlyProperty(int i) { }
	}
	
    private static class FullOfProperties extends FullOfPropertiesSuper {
    	@QtInvokable
    	public FullOfProperties(String test) {
			super();
		}
		@QtPropertyNotify(name="ordinaryProperty")
    	public final Signal1<Integer> ordinaryPropertyChanged = new Signal1<>();
        private boolean isDesignableTest;
        private FullOfProperties(boolean isDesignableTest) {
            this.isDesignableTest = isDesignableTest;
        }

        @SuppressWarnings("unused")
        public final int ordinaryProperty() { 
        	return 8; 
    	}
        @SuppressWarnings("unused")
        public final void setOrdinaryProperty(int i) { 
        }

        @QtPropertyReader(name="annotatedProperty")
        public final int fooBar() { return 0; }

        @QtPropertyWriter(name="annotatedProperty")
        public final void fooBarSetIt(int i) { }

        @QtPropertyReader()
        public final int ordinaryReadOnlyProperty() { return 0; }

        @QtPropertyDesignable(value="false")
        public final int ordinaryNonDesignableProperty() { return 0; }

        @SuppressWarnings("unused")
        public final void setOrdinaryNonDesignableProperty(int i) { }

        @QtPropertyDesignable(value="false")
        @QtPropertyReader(name="annotatedNonDesignableProperty")
        public final int fooBarXyz() { return 0; }
        @QtPropertyWriter()
        public final void setAnnotatedNonDesignableProperty(int i) { }

        @SuppressWarnings("unused")
        public final boolean hasBooleanProperty() { return false; }
        @SuppressWarnings("unused")
        public final void setBooleanProperty(boolean b) { }

        @SuppressWarnings("unused")
        public final boolean isOtherBooleanProperty() { return false; }
        @SuppressWarnings("unused")
        public final void setOtherBooleanProperty(boolean b) { }

        @QtPropertyReader
        public final int resettableProperty() { return 0; }

        @QtPropertyWriter
        public final void setResettableProperty(int i) { }

        @QtPropertyResetter(name="resettableProperty")
        public final void resetResettableProperty() { }

        @QtPropertyDesignable(value="test")
        public final int testDesignableProperty() { 
        	return 0; 
    	}
        @SuppressWarnings("unused")
		public final void setTestDesignableProperty(int i) { 
        }
        
        @QtPropertyRequired
        public final int requiredProperty() { 
        	return 0; 
    	}
        @SuppressWarnings("unused")
		public final void setRequiredProperty(int i) { 
        }

        @QtPropertyReader
        @QtPropertyUser
        public final int annotatedUserProperty() { return 0; }

        @QtPropertyWriter
        public final void setAnnotatedUserProperty(int i) {}

        @QtPropertyUser
        public final int myUserProperty() { return 0; }
        @SuppressWarnings("unused")
        public final void setMyUserProperty(int i) {}


        @SuppressWarnings("unused")
        public boolean test() {
            return isDesignableTest;
        }
        
        static boolean staticScriptableMethodInvoked;
        
        @QtInvokable
        public static void staticScriptableMethod() {
        	staticScriptableMethodInvoked = true;
        }
    }

    private static class ExpectedValues {
        private boolean writable;
        private boolean resettable;
        private boolean designable;
        private boolean user;
        private boolean required;
        private boolean hasNotifySignal;
        private String name;

        private ExpectedValues(String name, boolean writable, boolean resettable, boolean designable, boolean user, boolean required, boolean hasNotifySignal) {
            this.name = name;
            this.writable = writable;
            this.resettable = resettable;
            this.designable = designable;
            this.user = user;
            this.required = required;
            this.hasNotifySignal = hasNotifySignal;
        }
    }

    @Test
    public void testMetaProperties() {
    	boolean canRequired = false;
    	try{
            canRequired = io.qt.internal.PropertyFlags.valueOf("Required")!=null;
        }catch(Throwable t){}
        ExpectedValues expectedValues[] = {
                new ExpectedValues("testDesignableProperty", true, false, true, false, false, false),
                new ExpectedValues("requiredProperty", true, false, true, false, canRequired, false),
                new ExpectedValues("ordinaryProperty", true, false, true, false, false, true),
                new ExpectedValues("annotatedProperty", true, false, true, false, false, false),
                new ExpectedValues("ordinaryReadOnlyProperty", false, false, true, false, false, false),
                new ExpectedValues("readOnlyProperty", false, false, true, false, false, false),
                new ExpectedValues("ordinaryNonDesignableProperty", true, false, false, false, false, false),
                new ExpectedValues("annotatedNonDesignableProperty", true, false, false, false, false, false),
                new ExpectedValues("booleanProperty", true, false, true, false, false, false),
                new ExpectedValues("otherBooleanProperty", true, false, true, false, false, false),
                new ExpectedValues("resettableProperty", true, true, true, false, false, false),
                new ExpectedValues("objectName", true, false, true, false, false, true),
                new ExpectedValues("myUserProperty", true, false, true, true, false, false),
                new ExpectedValues("annotatedUserProperty", true, false, true, true, false, false)
        };

        FullOfProperties fop = new FullOfProperties(true);
        for (ExpectedValues e : expectedValues) {
//            java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "Current property: " + e.name);
            QMetaProperty property = fop.metaObject().property(e.name);
            assertTrue(property!=null);
            assertEquals(property.name()+" writable", e.writable, property.isWritable());
            assertEquals(property.name()+" resettable", e.resettable, property.isResettable());
            assertEquals(property.name()+" designable", e.designable, property.isDesignable(fop));
            assertEquals(property.name()+" user", e.user, property.isUser());
            assertEquals(property.name()+" required", e.required, property.isRequired());
            assertEquals(property.name()+" hasNotifySignal", e.hasNotifySignal, property.hasNotifySignal());
            if(property.hasNotifySignal()) {
            	switch(property.name()) {
            	case "ordinaryProperty":
            		assertEquals(property.name(), fop.ordinaryPropertyChanged, property.notifySignal(fop)); break;
            	case "objectName":
            		assertEquals(property.name(), fop.objectNameChanged, property.notifySignal(fop)); break;
            	}
            }
        }
        List<QMetaProperty> properties = fop.metaObject().properties();
        assertEquals(expectedValues.length, properties.size());
    }

    @Test
    public void testFunctionDesignableProperty() {
        {
            FullOfProperties fop = new FullOfProperties(true);
            QMetaProperty property = fop.metaObject().property("testDesignableProperty");
            assertTrue(property!=null);
            assertTrue(property.isDesignable(fop));
        }

        {
            FullOfProperties fop = new FullOfProperties(false);
            QMetaProperty property = fop.metaObject().property("testDesignableProperty");
            assertTrue(property!=null);
            assertFalse(property.isDesignable(fop));
        }
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaProgrammingQt5.class.getName());
    }
}
