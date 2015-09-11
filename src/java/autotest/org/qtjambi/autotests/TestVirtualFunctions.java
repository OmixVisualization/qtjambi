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

import org.junit.Test;

import org.qtjambi.autotests.generated.AbstractClass;
import org.qtjambi.autotests.generated.SetupLayout;
import org.qtjambi.autotests.generated.AnotherNonAbstractSubclass;

import org.qtjambi.qt.QNoImplementationException;
import org.qtjambi.qt.core.QPoint;
import org.qtjambi.qt.core.QRect;
import org.qtjambi.qt.core.QSize;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.QIcon;
import org.qtjambi.qt.gui.QIcon.Mode;
import org.qtjambi.qt.gui.QImage;
import org.qtjambi.qt.widgets.QLayoutItemInterface;
import org.qtjambi.qt.gui.QPaintDeviceInterface;
import org.qtjambi.qt.gui.QPaintEngine;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.gui.QPixmap;
import org.qtjambi.qt.widgets.QPushButton;
import org.qtjambi.qt.widgets.QSizePolicy;
import org.qtjambi.qt.widgets.QSpacerItem;
import org.qtjambi.qt.widgets.QStyle;
import org.qtjambi.qt.widgets.QStyleHintReturn;
import org.qtjambi.qt.widgets.QStyleOption;
import org.qtjambi.qt.widgets.QStyleOptionComplex;
import org.qtjambi.qt.widgets.QVBoxLayout;
import org.qtjambi.qt.widgets.QWidget;

class JavaNonAbstractSubclass extends AbstractClass {
    @Override
    public void abstractFunction(String something) {
        setS("Even more " + something);
    }

    @Override
    public AbstractClass getAbstractClass() {
        return new JavaNonAbstractSubclass();
    }
}

class MyLayout extends QVBoxLayout {
    @Override
    public QLayoutItemInterface itemAt(int index) {
        return super.itemAt(index);
    }

    @Override
    public void addItem(QLayoutItemInterface arg__0) {
        super.addItem(arg__0);
    }

    @Override
    public int count() {
        return super.count();
    }

    @Override
    public void setGeometry(QRect arg__0) {
        super.setGeometry(arg__0);
    }

    @Override
    public QSize sizeHint() {
        return super.sizeHint();
    }

    @Override
    public QLayoutItemInterface takeAt(int index__0) {
        return super.takeAt(index__0);
    }
}

public class TestVirtualFunctions extends QApplicationTest {
    class WidgetClass1 extends QWidget {
        public void setJavaSizeHint(QSize size) {
            m_size = size;
        }

        @Override
        public QSize sizeHint() {
            return m_size;
        }

        private QSize m_size = new QSize(0, 0);
    }

    class WidgetClass2 extends QWidget {
        public void setJavaSizeHint(QSize size) {
            m_size = size;
        }

        private QSize m_size = new QSize(0, 0);
    }

    @Test
    public void run_testOverridingMethodsThatReturnInterfaceTypes() {
        QWidget topLevel = new QWidget();
        QPushButton button1 = new QPushButton("Test", topLevel);

        MyLayout layout = new MyLayout();
        layout.addWidget(button1);
        topLevel.setLayout(layout);
        topLevel.dispose();
    }

    @Test
    public void run_testNonQObjectsInCustomLayout() {
        QWidget topLevel = new QWidget();
        QSpacerItem spacer = new QSpacerItem(10, 10);

        MyLayout layout = new MyLayout();
        layout.addItem(spacer);
        topLevel.setLayout(layout);
        topLevel.show();
        topLevel.dispose();
    }

    @Test
    public void run_testNonQObjectsInCustomLayoutAddedFromCpp() {
        QWidget topLevel = new QWidget();
        MyLayout layout = new MyLayout();
        topLevel.setLayout(layout);

        SetupLayout.setupLayout(layout);

        assertEquals(layout.count(), 4);
        topLevel.show();
        topLevel.dispose();
    }

    @Test
    public void run_testOneSubclass() {
        WidgetClass1 w = new WidgetClass1();
        w.setJavaSizeHint(new QSize(123, 456));
        assertEquals(w.sizeHint().width(), 123);
        assertEquals(w.sizeHint().height(), 456);
    }

    @Test
    public void run_testTwoSubclasses() {
        WidgetClass1 w1 = new WidgetClass1();
        w1.setJavaSizeHint(new QSize(123, 456));

        assertEquals(w1.sizeHint().width(), 123);
        assertEquals(w1.sizeHint().height(), 456);

        WidgetClass2 w2 = new WidgetClass2();
        w2.setJavaSizeHint(new QSize(654, 321));
        QWidget w = new QWidget();
        assertEquals(w2.sizeHint().width(), w.sizeHint().width());
        assertEquals(w2.sizeHint().height(), w.sizeHint().height());
    }

    // A QObject subclass to call super.paintEngine();
    private interface CallCounter {
        public int callCount();
    }

    private static class Widget extends QWidget implements CallCounter {
        public int called;

        public int callCount() {
            return called;
        }

        @Override
        public QPaintEngine paintEngine() {
            ++called;
            if (called > 1) {
                return null;
            }
            return super.paintEngine();
        }
    }

    // A non QObject subclass to call super.paintEngine();
    private static class Image extends QImage implements CallCounter {
        public int called;

        public Image() {
            super(100, 100, QImage.Format.Format_ARGB32_Premultiplied);
        }

        public int callCount() {
            return called;
        }

        @Override
        public QPaintEngine paintEngine() {
            ++called;
            if (called > 1) {
                return null;
            }
            return super.paintEngine();
        }
    }

    /**
     * The purpose of this test is to verify that we can do things like
     * super.paintEngine() in a QPaintDevice subclass and not get recursion.
     */
    @Test
    public void run_testSupercall() {

        QPaintDeviceInterface[] testDevices = new QPaintDeviceInterface[] { new Widget(), new Image() };
        for (int i = 0; i < testDevices.length; i++) {
            QPaintDeviceInterface device = testDevices[i];
            QPainter p = new QPainter();
            p.begin(device);
            p.end();
            assertEquals(((CallCounter) device).callCount(), 1);
        }
    }

    /**
     * The purpose of this test is to verify that we are calling virtual
     * functions using the correct environment for the current thread. We create
     * a QObject in the main thread and pass it to an executing thread and
     * trigger a virtual function call
     */
    private static class PaintThread extends Thread {
        public Image image;

        @Override
        public void run() {
            QPainter p = new QPainter();
            p.begin(image);
            p.end();
        }
    }

    @Test
    public void run_testEnvironment() {
        PaintThread thread = new PaintThread();
        thread.image = new Image();
        thread.start();
        try {
            thread.join();
        } catch (Exception e) {
            e.printStackTrace();
        }

        assertEquals(((CallCounter) thread.image).callCount(), 1);
    }

    @Test
    @SuppressWarnings("deprecation")
    public void run_abstractClasses() {
        AnotherNonAbstractSubclass obj = new AnotherNonAbstractSubclass();

        obj.setS("a string");
        try {
            obj.abstractFunction("a super-string");
            assertTrue(false); // we should never get here
        } catch (QNoImplementationException e) {
            obj.setS("a non-super string");
        }
        assertEquals(obj.getS(), "a non-super string");

        obj.doVirtualCall(obj, "a super-string");
        assertEquals(obj.getS(), "Not a super-string");

        AbstractClass cls = obj.getAbstractClass();
        assertEquals(cls.getClass().getName(), "org.qtjambi.autotests.generated.AbstractClass$ConcreteWrapper");

        cls.abstractFunction("my super-string");
        assertEquals(cls.getS(), "my super-string");
        assertEquals(cls.getAbstractClass(), null);
        obj.doVirtualCall(cls, "my non-super string");
        assertEquals(cls.getS(), "my non-super string");

        JavaNonAbstractSubclass foo = new JavaNonAbstractSubclass();
        assertTrue(foo.getAbstractClass() instanceof JavaNonAbstractSubclass);

        foo.abstractFunction("of my super strings");
        assertEquals(foo.getS(), "Even more of my super strings");

        obj.doVirtualCall(foo, "of my non-super strings");
        assertEquals(foo.getS(), "Even more of my non-super strings");
    }

    private boolean myVirtualFunctionWasCalled;

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestVirtualFunctions.class.getName());
    }
}
