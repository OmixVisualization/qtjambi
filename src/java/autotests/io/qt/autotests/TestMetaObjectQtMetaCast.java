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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.MetaObjectQtMetaCast;
import io.qt.core.QObject;
import io.qt.core.QRect;
import io.qt.core.QRectF;
import io.qt.core.QSize;
import io.qt.core.QSizeF;
import io.qt.core.Qt.Orientations;
import io.qt.gui.QPainter;
import io.qt.gui.QTextDocument;
import io.qt.gui.QTextFormat;
import io.qt.gui.QTextObjectInterface;
import io.qt.widgets.QGraphicsWidget;
import io.qt.widgets.QLabel;
import io.qt.widgets.QLayoutItem;
import io.qt.widgets.QWidget;

public class TestMetaObjectQtMetaCast extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
    // Not used
    @SuppressWarnings("unused")
    private static class MyLauncher extends QWidget {
        public final Signal0 mySignal = new Signal0();
        private int slotCalledCount;
        public void mySlot() {
            synchronized (this) {
                slotCalledCount++;
            }
        }
		public int getSlotCalledCount() {
            synchronized (this) {
                return slotCalledCount;
            }
        }
    }

    @Test
    public void testBasicUsage() {
        String thisClass = TestMetaObjectQtMetaCast.class.getName().replace(".", "::");
        String s;

        MyLauncher myLauncher = new MyLauncher();  // the target of our inspection

        s = myLauncher.metaObject().className();
        assertEquals(thisClass + "::MyLauncher", s);

        s = MetaObjectQtMetaCast.superClassName(myLauncher, 0);
        assertEquals("QWidget", s);

        s = MetaObjectQtMetaCast.superClassName(myLauncher, 1);
        assertEquals("QObject", s);

        s = MetaObjectQtMetaCast.superClassName(myLauncher, 2);
        assertEquals("", s); // FIXME: expected null here 

        assertTrue(myLauncher.inherits("QObject"));
        assertTrue(myLauncher.inherits("QWidget"));

        //assertTrue(moqmc.inherits(myLauncher, thisClass + "$MyLauncher"));

        long l;
        l = MetaObjectQtMetaCast.do_qt_metacast(myLauncher, "QObject");
        assertTrue(isMetacastSuccessful(l));

        l = MetaObjectQtMetaCast.do_qt_metacast(myLauncher, "QWidget");
        assertTrue(isMetacastSuccessful(l));

        l = MetaObjectQtMetaCast.do_qt_metacast(myLauncher, "QPushButton");
        assertFalse(isMetacastSuccessful(l));  // NOT

        l = MetaObjectQtMetaCast.do_qt_metacast(myLauncher, thisClass + "::MyLauncher");
        assertTrue(isMetacastSuccessful(l));
    }

    /**
     * Driven by bug#213
     */
    @Test
    public void testBug213() {
        MyLauncher myLauncher = new MyLauncher();  // the target of our inspection

        assertTrue(myLauncher.inherits("QObject"));
        assertTrue(myLauncher.inherits("QWidget"));

        // This is the bug being reported
        String thisClass = TestMetaObjectQtMetaCast.class.getName().replace(".", "::");
        assertTrue(myLauncher.inherits(thisClass + "::MyLauncher"));

        long l = MetaObjectQtMetaCast.do_qt_metacast(myLauncher, thisClass + "::MyLauncher");
        assertTrue(isMetacastSuccessful(l));
    }

    private boolean isMetacastSuccessful(long l) {
        if(l != 0 && l != -1)
            return true;
        return false;
    }
    
    @Test
    public void testInheritance() {
    	QObject o = new QGraphicsWidget();
    	assertTrue(o.inherits("QObject"));
    	assertTrue(o.inherits("QGraphicsItem"));
    	assertFalse(o.inherits("QWidget"));
    	class Test extends QLabel implements QLayoutItem, QTextObjectInterface {
			@Override
			public Orientations expandingDirections() {
				return null;
			}

			@Override
			public boolean isEmpty() {
				return false;
			}

			@Override
			public QSize sizeHint() {
				return null;
			}

			@Override
			public void drawObject(QPainter painter, QRectF rect, QTextDocument doc, int posInDocument,
					QTextFormat format) {
				
			}

			@Override
			public QSizeF intrinsicSize(QTextDocument doc, int posInDocument, QTextFormat format) {
				return null;
			}
    	}
    	Test test;
    	o = test = new Test();
    	test.setGeometry(3, 6, 9, 12);
    	assertTrue(o.inherits("QObject"));
    	assertTrue(o.inherits("QWidget"));
    	assertTrue(o.inherits("QLabel"));
    	assertTrue(o.inherits("QLayoutItem"));
    	assertTrue(o.inherits("QTextObjectInterface"));
    	QRect geometry = MetaObjectQtMetaCast.layoutItemGeometry(test);
    	assertEquals(new QRect(3, 6, 9, 12), test.geometry());
    	assertEquals(new QRect(3, 6, 9, 12), geometry);
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestMetaObjectQtMetaCast.class.getName());
    }
}
