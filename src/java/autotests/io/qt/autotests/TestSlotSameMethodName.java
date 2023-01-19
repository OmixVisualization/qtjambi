/****************************************************************************
**
** Copyright (C) 2011 Darryl L. Miles.  All rights reserved.
** Copyright (C) 2011 D L Miles Consulting Ltd.  All rights reserved.
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
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html
** 
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html
** $END_LICENSE$
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.After;
import org.junit.Assume;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QEventLoop.ProcessEventsFlag;
import io.qt.core.QEventLoop.ProcessEventsFlags;
import io.qt.core.QTimer;
import io.qt.core.Qt;
import io.qt.core.Qt.GlobalColor;
import io.qt.gui.QCloseEvent;
import io.qt.gui.QGuiApplication;
import io.qt.gui.QPalette;
import io.qt.gui.QShowEvent;
import io.qt.widgets.QApplication;
import io.qt.widgets.QMainWindow;
import io.qt.widgets.QTabWidget;
import io.qt.widgets.QWidget;

//These testcases attempt to confirm a bug and fix a scenario where by a QObject
//(like QTabWidget) is subclassed and one of the signal's is connected to a
//method with the same name and signature of the base-class signal.  But the
//base-class signal implementation is declared "private final".  What appeared
//to happen is that all signals were disconnected and only the default baseclass
//one was allowed.
public class TestSlotSameMethodName extends ApplicationInitializer {
	@BeforeClass
	public static void testInitialize() throws Exception {
    	testInitializeWithWidgets();
    }

	@Before
	public void setUp() {
	}

	@After
	public void tearDown() {
	}

	// /////////////////////////////////////////////////
	// Tests start
	static class MyBaseclassQTabWidget extends QTabWidget {
		protected MyNotifiable myNotifiable;
		protected int switchTab;
		MyBaseclassQTabWidget(MyNotifiable myNotifiable, int switchTab) {
			super((QWidget)null);
			this.myNotifiable = myNotifiable;
			this.switchTab = switchTab;
		}
		public int getSwitchTab() {
			return switchTab;
		}
	}

	// This is a class with a method with the same signature as a baseclass slot 'currentChanged(int)'
	static class MySubclassedSameQTabWidget extends MyBaseclassQTabWidget {
		public MySubclassedSameQTabWidget(MyNotifiable myNotifiable, int switchTab, int mask, Qt.ConnectionType connectionType) {
			super(myNotifiable, switchTab);
			if((mask & 0x01) != 0)
				currentChanged.connect(this, "currentChanged(int)", connectionType);
			//if((mask & 0x01) != 0)
			//	currentChanged.connect(this, "currentChanged(int)", connectionType);
			//if((mask & 0x01) != 0)
			//	currentChanged.connect(this, "currentChanged(Integer)", connectionType);
			//if((mask & 0x02) != 0)
			//	currentChanged.connect(this, "currentChanged(int)", connectionType);
			//if((mask & 0x02) != 0)
			//	currentChanged.connect(this, "currentChangedI2(Integer)", connectionType);
			if((mask & 0x02) != 0)
				currentChanged.connect(this, "currentChanged(Integer)", connectionType);
		}
		// This method has the same name as the baseclass method.
		protected void currentChanged(int index) {   // rename to currentChangedY or currentChanged
			//int index = -2;
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChanged(%1$d) int", index);
			if(index < count())
				myNotifiable.orNotify(1 << index);  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
		protected void currentChangedI(Integer index) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChangedI(%1$d) Integer", index);
			if(index < count())
				myNotifiable.orNotify(1 << (index + 8));  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
		protected void currentChanged2(int index) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChanged2(%1$d) int", index);
			if(index < count())
				myNotifiable.orNotify(1 << index);  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
		protected void currentChangedI2(Integer index) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChangedI2(%1$d) Integer", index);
			if(index < count())
				myNotifiable.orNotify(1 << index);  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
		protected void currentChangedX(int index) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChangedX(%1$d) int", index);
			if(index < count())
				myNotifiable.orNotify(1 << index);  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
	}

	// This is a class with a method with the same signature as a baseclass slot 'currentChanged(int)'
	static class MySubclassedDiffQTabWidget extends MyBaseclassQTabWidget {
		public MySubclassedDiffQTabWidget(MyNotifiable myNotifiable, int switchTab, int mask, Qt.ConnectionType connectionType) {
			super(myNotifiable, switchTab);
			if((mask & 0x01) != 0)
				currentChanged.connect(this, "currentChangedY(int)", connectionType);
			//if((mask & 0x01) != 0)
			//	currentChanged.connect(this, "currentChanged(int)", connectionType);
			//if((mask & 0x01) != 0)
			//	currentChanged.connect(this, "currentChanged(Integer)", connectionType);
			//if((mask & 0x02) != 0)
			//	currentChanged.connect(this, "currentChanged2(int)", connectionType);
			//if((mask & 0x02) != 0)
			//	currentChanged.connect(this, "currentChangedI2(Integer)", connectionType);
			if((mask & 0x02) != 0)
				currentChanged.connect(this, "currentChangedI(Integer)", connectionType);
		}
		// This method has the same name as the baseclass method.
		protected void currentChangedY(int index) {   // rename to currentChangedY or currentChanged
			//int index = -2;
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChangedY(%1$d) int", index);
			if(index < count())
				myNotifiable.orNotify(1 << index);  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
		protected void currentChangedI(Integer index) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChangedI(%1$d) Integer", index);
			if(index < count())
				myNotifiable.orNotify(1 << (index + 8));  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
		protected void currentChanged2(int index) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChanged2(%1$d) int", index);
			if(index < count())
				myNotifiable.orNotify(1 << index);  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
		protected void currentChangedI2(Integer index) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChangedI2(%1$d) Integer", index);
			if(index < count())
				myNotifiable.orNotify(1 << index);  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
		protected void currentChangedX(int index) {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "**** currentChangedX(%1$d) int", index);
			if(index < count())
				myNotifiable.orNotify(1 << index);  // normal tab switch
			else
				myNotifiable.orNotify(index);  // testcase sending emit
		}
	}

	public static final int B_TAB1		= 0x000001;	// index=0
	public static final int B_TAB2		= 0x000002;	// index=1
	public static final int B_TAB3		= 0x000004;	// index=2
	public static final int B_TAB4		= 0x000008;	// index=3
	public static final int B_TAB1CB2	= 0x000100;	// index=0 callback2
	public static final int B_TAB2CB2	= 0x000200;	// index=1 callback2
	public static final int B_TAB3CB2	= 0x000400;	// index=2 callback2
	public static final int B_TAB4CB2	= 0x000800;	// index=3 callback2
	public static final int B_DIRECT	= 0x020000;
	public static final int B_SHOW		= 0x040000;
	public static final int B_CLOSE 	= 0x080000;

	private int cb2(int tab) {
		if(tab == B_TAB1)
			return B_TAB1CB2;
		else if(tab == B_TAB2)
			return B_TAB2CB2;
		else if(tab == B_TAB3)
			return B_TAB3CB2;
		else if(tab == B_TAB4)
			return B_TAB4CB2;
		return 0;
	}

	private void doSlotSameMethodNameSame(int switchTab, int mode, Qt.ConnectionType connectionType) {
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		MyNotifiable myNotifiable = new MyNotifiable();
		MySubclassedSameQTabWidget tabWidget = new MySubclassedSameQTabWidget(myNotifiable, switchTab, mode, connectionType);
		assertEquals("before", 0, myNotifiable.getNotified());

		int i = foo(tabWidget, myNotifiable);
		int expect = B_CLOSE | B_SHOW;  // We never hooked up our slots in this testcase
		if(((mode & 0x1) == 0x1) || ((mode & 0x2) == 0x2))
			expect |= B_TAB1 | switchTab;	// Tab index=0 is signalled on first show
		if((mode & 0x11) == 0x11)
			expect |= cb2(B_TAB1) | cb2(switchTab);	// Tab index=0 is signalled on first show, callback2
		assertEquals("i", expect, i);
		assertEquals("at", expect, myNotifiable.getNotified());

		tabWidget.currentChanged.emit(Integer.valueOf(B_DIRECT));
		if(mode != 0 && connectionType != Qt.ConnectionType.QueuedConnection)
			expect |= B_DIRECT;  // AutoConnection || DirectConnection || BlockingQueuedConnection
		assertEquals("during", expect, myNotifiable.getNotified());

		QCoreApplication.processEvents();  // QuquedConnection happens after processEvents()
		if(mode != 0)
			expect |= B_DIRECT;  // AutoConnection || DirectConnection || BlockingQueuedConnection || QueuedConnection
		assertEquals("after", expect, myNotifiable.getNotified());
	}

	private void doSlotSameMethodNameDiff(int switchTab, int mode, Qt.ConnectionType connectionType) {
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		MyNotifiable myNotifiable = new MyNotifiable();
		MySubclassedDiffQTabWidget tabWidget = new MySubclassedDiffQTabWidget(myNotifiable, switchTab, mode, connectionType);
		assertEquals("before", 0, myNotifiable.getNotified());

		int i = foo(tabWidget, myNotifiable);
		int expect = B_CLOSE | B_SHOW;  // We never hooked up our slots in this testcase
		if((mode & 0x1) == 0x1)
			expect |= B_TAB1 | switchTab;	// Tab index=0 is signalled on first show
		if((mode & 0x2) == 0x2)
			expect |= cb2(B_TAB1) | cb2(switchTab);	// Tab index=0 is signalled on first show, callback2
		assertEquals("i", expect, i);
		assertEquals("at", expect, myNotifiable.getNotified());

		tabWidget.currentChanged.emit(Integer.valueOf(B_DIRECT));
		if(mode != 0 && connectionType != Qt.ConnectionType.QueuedConnection)
			expect |= B_DIRECT;  // AutoConnection || DirectConnection || BlockingQueuedConnection
		assertEquals("during", expect, myNotifiable.getNotified());

		QCoreApplication.processEvents();  // QuquedConnection happens after processEvents()
		if(mode != 0)
			expect |= B_DIRECT;  // AutoConnection || DirectConnection || BlockingQueuedConnection || QueuedConnection
		assertEquals("after", expect, myNotifiable.getNotified());
	}

	@Test
	public void testSlotSameMethodNameSameAuto0() {
		doSlotSameMethodNameSame(B_TAB3, 0, Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testSlotSameMethodNameSameAuto1() {
		doSlotSameMethodNameSame(B_TAB3, 1, Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testSlotSameMethodNameSameAuto2() {
		doSlotSameMethodNameSame(B_TAB3, 2, Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testSlotSameMethodNameSameAuto3() {
		doSlotSameMethodNameSame(B_TAB3, 3, Qt.ConnectionType.AutoConnection);
	}


	@Test
	public void testSlotSameMethodNameSameDirect0() {
		doSlotSameMethodNameSame(B_TAB3, 0, Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testSlotSameMethodNameSameDirect1() {
		doSlotSameMethodNameSame(B_TAB3, 1, Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testSlotSameMethodNameSameDirect2() {
		doSlotSameMethodNameSame(B_TAB3, 2, Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testSlotSameMethodNameSameDirect3() {
		doSlotSameMethodNameSame(B_TAB3, 3, Qt.ConnectionType.DirectConnection);
	}


	@Test
	public void testSlotSameMethodNameSameQueued0() {
		doSlotSameMethodNameSame(B_TAB3, 0, Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameSameQueued1() {
		doSlotSameMethodNameSame(B_TAB3, 1, Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameSameQueued2() {
		doSlotSameMethodNameSame(B_TAB3, 2, Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameSameQueued3() {
		doSlotSameMethodNameSame(B_TAB3, 3, Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffAuto0() {
		doSlotSameMethodNameDiff(B_TAB3, 0, Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffAuto1() {
		doSlotSameMethodNameDiff(B_TAB3, 1, Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffAuto2() {
		doSlotSameMethodNameDiff(B_TAB3, 2, Qt.ConnectionType.AutoConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffAuto3() {
		doSlotSameMethodNameDiff(B_TAB3, 3, Qt.ConnectionType.AutoConnection);
	}


	@Test
	public void testSlotSameMethodNameDiffDirect0() {
		doSlotSameMethodNameDiff(B_TAB3, 0, Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffDirect1() {
		doSlotSameMethodNameDiff(B_TAB3, 1, Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffDirect2() {
		doSlotSameMethodNameDiff(B_TAB3, 2, Qt.ConnectionType.DirectConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffDirect3() {
		doSlotSameMethodNameDiff(B_TAB3, 3, Qt.ConnectionType.DirectConnection);
	}


	@Test
	public void testSlotSameMethodNameDiffQueued0() {
		doSlotSameMethodNameDiff(B_TAB3, 0, Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffQueued1() {
		doSlotSameMethodNameDiff(B_TAB3, 1, Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffQueued2() {
		doSlotSameMethodNameDiff(B_TAB3, 2, Qt.ConnectionType.QueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffQueued3() {
		doSlotSameMethodNameDiff(B_TAB3, 3, Qt.ConnectionType.QueuedConnection);
	}


	/* BlockingQueuedConnection does not work in native connections within the same thread.
	@Test
	public void testSlotSameMethodNameDiffBlockingQueued0() {
		doSlotSameMethodNameDiff(B_TAB3, 0, Qt.ConnectionType.BlockingQueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffBlockingQueued1() {
		doSlotSameMethodNameDiff(B_TAB3, 1, Qt.ConnectionType.BlockingQueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffBlockingQueued2() {
		doSlotSameMethodNameDiff(B_TAB3, 2, Qt.ConnectionType.BlockingQueuedConnection);
	}

	@Test
	public void testSlotSameMethodNameDiffBlockingQueued3() {
		doSlotSameMethodNameDiff(B_TAB3, 3, Qt.ConnectionType.BlockingQueuedConnection);
	}*/


	private int foo(final MyBaseclassQTabWidget tabWidget, final MyNotifiable myNotifiable) {
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		QWidget tab1 = new QWidget();
		tab1.setAutoFillBackground(true);
		tab1.setPalette(new QPalette(GlobalColor.darkCyan));
		tabWidget.addTab(tab1, "Tab 1");

		QWidget tab2 = new QWidget();
		tab2.setAutoFillBackground(true);
		tab2.setPalette(new QPalette(GlobalColor.darkGreen));
		tabWidget.addTab(tab2, "Tab 2");

		QWidget tab3 = new QWidget();
		tab3.setAutoFillBackground(true);
		tab3.setPalette(new QPalette(GlobalColor.darkMagenta));
		tabWidget.addTab(tab3, "Tab 3");

		QWidget tab4 = new QWidget();
		tab4.setAutoFillBackground(true);
		tab4.setPalette(new QPalette(GlobalColor.darkYellow));
		tabWidget.addTab(tab4, "Tab 4");

		final QTimer timer = new QTimer();

		QMainWindow mainWindow = new QMainWindow() {
			@Override
			protected void showEvent(QShowEvent arg__1) {
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "showEvent(" + arg__1 + ") " + Thread.currentThread());
				int switchTab = tabWidget.getSwitchTab();
				if((switchTab & B_TAB1) == B_TAB1)
					tabWidget.setCurrentIndex(0);	// tab1
				else if((switchTab & B_TAB2) == B_TAB2)
					tabWidget.setCurrentIndex(1);	// tab2
				else if((switchTab & B_TAB3) == B_TAB3)
					tabWidget.setCurrentIndex(2);	// tab3
				else if((switchTab & B_TAB4) == B_TAB4)
					tabWidget.setCurrentIndex(3);	// tab4
				timer.timeout.connect(this::close);
				timer.setSingleShot(true);
				timer.start(0);
				myNotifiable.orNotify(B_SHOW);
				super.showEvent(arg__1);
			}

			@Override
			protected void closeEvent(QCloseEvent arg__1) {
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "closeEvent(" + arg__1 + ") " + Thread.currentThread());
				myNotifiable.orNotify(B_CLOSE);
				super.closeEvent(arg__1);
			}
		};
		mainWindow.setCentralWidget(tabWidget);
		mainWindow.raise();
		mainWindow.showNormal();
		while(!mainWindow.isVisible())
			QApplication.processEvents();
		while(mainWindow.isVisible())
			QApplication.processEvents();
		return myNotifiable.getNotified();
	}

	@SuppressWarnings("unused")
	private void foo2(MyBaseclassQTabWidget tabWidget) {
		Assume.assumeTrue("A screen is required to create a window.", QGuiApplication.primaryScreen()!=null);
		QWidget tab1 = new QWidget();
		tab1.setAutoFillBackground(true);
		tab1.setPalette(new QPalette(GlobalColor.darkCyan));
		tabWidget.addTab(tab1, "Tab 1");

		QWidget tab2 = new QWidget();
		tab2.setAutoFillBackground(true);
		tab2.setPalette(new QPalette(GlobalColor.darkGreen));
		tabWidget.addTab(tab2, "Tab 2");

		final MyNotifiable myNotifiable = new MyNotifiable();

		QMainWindow mainWindow = new QMainWindow() {
			@Override
			protected void closeEvent(QCloseEvent arg__1) {
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "closeEvent(" + arg__1 + ") " + Thread.currentThread());
				myNotifiable.upNotify(1);
				super.closeEvent(arg__1);
		}};
		mainWindow.setCentralWidget(tabWidget);
		mainWindow.raise();
		mainWindow.showNormal();
		QApplication.setQuitOnLastWindowClosed(true);
		//QApplication.processEvents();

		ProcessEventsFlags flags = new ProcessEventsFlags(ProcessEventsFlag.AllEvents);
		try {
			CloseWindow closeWindow = new CloseWindow(myNotifiable);
			closeWindow.start(28000);

			long expiry = System.currentTimeMillis() + 30000;	// 30sec
			while(true) {
				int left = (int)(expiry - System.currentTimeMillis());
				if(left <= 0) {
					java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "foo() TIMEOUT");
					break;
				}
				if(myNotifiable.isUpNotified(1))
					break;
				// Sorry this eat 100% CPU due to insufficient Qt API around
				//  QEventLoop handling.
				//java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "QApplication.processEvents() left=" + left);
				//if(QApplication.hasPendingEvents()) {
					//while(QApplication.hasPendingEvents())
						QApplication.processEvents(flags, left);
				//}
				//QApplication.execStatic();
			}

			int left = (int)(expiry - System.currentTimeMillis());
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "foo() END left=" + left);
		} catch(Exception e) {
			e.printStackTrace();
		}
	}

	interface Notifiable {
		void upNotify(int kind);
		void orNotify(int kind);
		void setNotify(int kind);
	}
	static class MyNotifiable implements Notifiable {
		private int didKind = 0;
		public void upNotify(int kind) {
			synchronized (this) {
				if(didKind < kind)
					didKind = kind;
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "doNotify(" + kind + ") " + Thread.currentThread());
				notifyAll();
			}
		}
		public void orNotify(int kind) {
			synchronized (this) {
				didKind |= kind;
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "doNotify(" + kind + ") " + Thread.currentThread());
				notifyAll();
			}
		}
		public void setNotify(int kind) {
			synchronized (this) {
				didKind = kind;
				java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "doNotify(" + kind + ") " + Thread.currentThread());
				notifyAll();
			}
		}
		protected void doNotiftyReset() {
			synchronized (this) {
				didKind = 0;
			}
		}
		protected boolean isUpNotified(int kind) {
			synchronized (this) {
				if(didKind >= kind) {
					java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "isNotified(at=" + didKind + "; want=" + kind + ") " + Thread.currentThread());
					return true;
				}
			}
			return false;
		}
		public boolean waitForNotify(long timeout, int kind) {
			long millis = System.currentTimeMillis();
			long expiry = millis + timeout;
			while(true) {
				long left = expiry - System.currentTimeMillis();
				if(left <= 0)
					break;
				synchronized (this) {
					if(didKind >= kind)
						return true;
					try {
						wait(left);  // we wake when delivery was performed
					} catch (InterruptedException e) {
						e.printStackTrace();
						assertTrue(false);
					}
					if(didKind >= kind)
						return true;
				}
			}
			return false;
		}
		public int getNotified() {
			synchronized (this) {
				return didKind;
			}
		}
	}

	protected static class CloseWindow extends QTimer {
		private Notifiable notifyObject;
		public CloseWindow(Notifiable notifyObject) {
			this.notifyObject = notifyObject;
			timeout.connect(this::myMethod);
		}
		protected void myMethod() {
			stop();
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, "CloseWindow.myMethod()");
			if(notifyObject != null)
				notifyObject.upNotify(1);
		}
	}

	// Tests end
	// ///////////////////////////////////////////////

	public static void main(String[] args) {
		org.junit.runner.JUnitCore.main(TestSlotSameMethodName.class.getName());
	}
}
