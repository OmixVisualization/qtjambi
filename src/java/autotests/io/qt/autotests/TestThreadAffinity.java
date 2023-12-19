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

import static org.junit.Assert.fail;

import java.lang.ref.WeakReference;
import java.util.Arrays;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicReference;

import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QThreadAffinityException;
import io.qt.QtArgument;
import io.qt.QtUtilities;
import io.qt.core.QEventLoop;
import io.qt.core.QList;
import io.qt.core.QObject;
import io.qt.core.QPoint;
import io.qt.core.QRectF;
import io.qt.core.QThread;
import io.qt.core.Qt;
import io.qt.core.Qt.CursorShape;
import io.qt.gui.*;
import io.qt.printsupport.QPrinter;
import io.qt.printsupport.QPrinterInfo;
import io.qt.widgets.*;

public class TestThreadAffinity extends ApplicationInitializer{
	
	static {
		System.setProperty("io.qt.enable-thread-affinity-check", "true");
		System.setProperty("io.qt.enable-event-thread-affinity-check", "true");
	}
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	private Throwable throwable;
	
	private static int COUNT = 300;
	
	@Test
    public void testQCursor() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			for (int i = 0; i < COUNT; i++) {
				QCursor cursor = new QCursor(CursorShape.DragCopyCursor);
				QtUtilities.getSignalOnDispose(cursor).connect(counter::incrementAndGet);
			}
		});
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		for (int i = 0; i < 20 && counter.get()!=COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(100);
            }
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test
    public void testQFont() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			for (int i = 0; i < COUNT; i++) {
				QFont font = new QFont("Helvetica", 15, QFont.Weight.Black.value());
				QtUtilities.getSignalOnDispose(font).connect(counter::incrementAndGet);
			}
		});
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		for (int i = 0; i < 20 && counter.get()!=COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(100);
            }
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test
    public void testQStyleOptionButton() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			for (int i = 0; i < COUNT; i++) {
				QStyleOptionButton value = new QStyleOptionButton();
				QtUtilities.getSignalOnDispose(value).connect(counter::incrementAndGet);
			}
		});
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		for (int i = 0; i < 20 && counter.get()!=COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(100);
            }
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test
    public void testQFontInfo() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			for (int i = 0; i < COUNT; i++) {
				QFontInfo font = new QFontInfo(new QFont("Helvetica", 15, QFont.Weight.Black.value()));
				QtUtilities.getSignalOnDispose(font).connect(counter::incrementAndGet);
			}
		});
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		for (int i = 0; i < 20 && counter.get()!=COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(100);
            }
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test
    public void testQColormap() throws InterruptedException {
		AtomicInteger counter = new AtomicInteger();
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			for (int i = 0; i < COUNT; i++) {
				QColormap value = new QColormap(QColormap.instance());
				QtUtilities.getSignalOnDispose(value).connect(counter::incrementAndGet);
			}
		});
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		for (int i = 0; i < 20 && counter.get()!=COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(100);
            }
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test
    public void testQPrinter() throws InterruptedException {
		int COUNT = 10;
		AtomicInteger counter = new AtomicInteger();
		QEventLoop loop = new QEventLoop();
		QPrinterInfo pi = QPrinterInfo.defaultPrinter();
		QThread thread = QThread.create(()->{
			for (int i = 0; i < COUNT; i++) {
				QPrinter value = new QPrinter(pi);
				QtUtilities.getSignalOnDispose(value).connect(counter::incrementAndGet);
			}
		});
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		for (int i = 0; i < 20 && counter.get()!=COUNT; i++) {
            ApplicationInitializer.runGC();
            synchronized(getClass()) {
            	Thread.sleep(100);
            }
		}
        Assert.assertEquals(COUNT, counter.get());
	}
	
	@Test
    public void testQGraphicsItem() {
    	QThread thread = new QThread();
    	thread.start();
    	QGraphicsScene scene = new QGraphicsScene();
    	QGraphicsItem parent = new QGraphicsRectItem();
    	scene.addItem(parent);
    	scene.moveToThread(thread);
		try {
    		QGraphicsObject child = new QGraphicsTextItem();
    		try {
    			child.setParentItem(parent);
    			fail("QThreadAffinityException expected to be thrown.");
    		}catch(QThreadAffinityException e) {}
			try{
				scene.addItem(child);
    			fail("QThreadAffinityException expected to be thrown.");
    		}catch(QThreadAffinityException e) {}
			try {
    			new QGraphicsItem.Impl(parent){
					@Override
					public QRectF boundingRect() {
						return null;
					}
					@Override
					public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {}
				};
    			fail("QThreadAffinityException expected to be thrown.");
    		}catch(QThreadAffinityException e) {}
			try {
    			new QGraphicsItem(){
    				{
    					QtUtilities.initializeNativeObject(this, QtArgument.begin(QGraphicsItem.class).add(parent));
    				}
					@Override
					public QRectF boundingRect() {
						return null;
					}
					@Override
					public void paint(QPainter painter, QStyleOptionGraphicsItem option, QWidget widget) {}
				};
    			fail("QThreadAffinityException expected to be thrown.");
    		}catch(QThreadAffinityException e) {}
			try {
    			new QGraphicsRectItem(parent);
    			fail("QThreadAffinityException expected to be thrown.");
    		}catch(QThreadAffinityException e) {}
		}finally {
			scene.disposeLater();
    		thread.quit();
		}
	}
	
	@Test
    public void testQObject() {
    	QThread thread = new QThread();
    	thread.start();
		QObject parent = new QObject();
		parent.moveToThread(thread);
    	try {
    		try {
    			new QObject(parent);
    			fail("QThreadAffinityException expected to be thrown.");
    		}catch(QThreadAffinityException e) {}
    		QObject child = new QObject();
    		try {
    			child.setParent(parent);
    			fail("QThreadAffinityException expected to be thrown.");
    		}catch(QThreadAffinityException e) {}
    		
    		parent.setObjectName("ThreadAffine");
    		
    		try{
    			parent.setProperty("objectName", "");
    			fail("QThreadAffinityException expected to be thrown.");
    		}catch(QThreadAffinityException e) {}
    		try{
    			parent.setParent(null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
    		try{
    			QObject filteredObj = new QObject();
    			filteredObj.installEventFilter(parent);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
    		try{
    			QObject filterObj = new QObject();
    			parent.installEventFilter(filterObj);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
    		try{
    			parent.removeEventFilter(null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QtUtilities.threadCheck(parent);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
    	}finally {
    		parent.disposeLater();
    		thread.quit();
		}
    }
	
	@Test
    public void testQGraphicsWidget() throws Throwable{
		throwable = null;
		QComboBox comboBox = new QComboBox();
		QGraphicsProxyWidget widget = new QGraphicsProxyWidget();
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			try{
				widget.addAction(new QAction());
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.addActions(QList.of(new QAction()));
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.setWidget(comboBox);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QtUtilities.uiThreadCheck();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
		});
		thread.setUncaughtExceptionHandler((Thread t, Throwable e)->throwable = e);
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		if(throwable!=null)
			throw throwable;
	}
	
	@Test
    public void testQWidget() throws Throwable{
		throwable = null;
		QWidget widget = new QWidget();
		QDialog dialog = new QDialog();
		QMenu menu = new QMenu();
		menu.addAction("test");
		QComboBox comboBox = new QComboBox();
		comboBox.addItem("item1");
		QMenuBar menuBar = new QMenuBar();
		QErrorMessage errorMessage = new QErrorMessage();
		QWizard wizard = new QWizard();
		QEventLoop loop = new QEventLoop();
		QDialogButtonBox buttonBox = new QDialogButtonBox();
		QThread thread = QThread.create(()->{
			try{
				comboBox.showPopup();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				menu.addAction("test2");
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				menu.exec();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				menu.exec(new QPoint());
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QMenu.exec(Arrays.asList(new QAction()), new QPoint());
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.setVisible(true);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.show();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.showFullScreen();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.showMaximized();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.showMinimized();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.showNormal();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.setWindowState(Qt.WindowState.WindowFullScreen);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				new QLabel();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				comboBox.setDisabled(true);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				comboBox.addAction(new QAction());
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				comboBox.addActions(QList.of(new QAction()));
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				dialog.exec();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				dialog.open();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				dialog.setVisible(true);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				menuBar.addAction("test");
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				menuBar.addSeparator();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QMessageBox.aboutQt(null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QMessageBox.about(null, "", "");
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QApplication.aboutQt();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QMessageBox.information(null, null, null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QMessageBox.question(null, null, null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QMessageBox.critical(null, null, null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QInputDialog.getDouble(null, null, null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QInputDialog.getInt(null, null, null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QInputDialog.getMultiLineText(null, null, null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QInputDialog.getText(null, null, null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QColorDialog.getColor();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getExistingDirectory();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getExistingDirectoryUrl();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getOpenFileContent(null, null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getOpenFileName();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getOpenFileUrl();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getOpenFileUrls();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getOpenFileNames();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getSaveFileName();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFileDialog.getSaveFileUrl();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QFontDialog.getFont((QWidget)null);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				QErrorMessage.qtHandler();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				errorMessage.showMessage("test");
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			for(QWizard.WizardButton b : QWizard.WizardButton.values()) {
				try{
					wizard.button(b);
					fail("QThreadAffinityException expected to be thrown.");
				}catch(QThreadAffinityException e) {}
			}
			for(QDialogButtonBox.StandardButton b : QDialogButtonBox.StandardButton.values()) {
				try{
					buttonBox.addButton(b);
					fail("QThreadAffinityException expected to be thrown.");
				}catch(QThreadAffinityException e) {}
			}
			for(QDialogButtonBox.ButtonRole r : QDialogButtonBox.ButtonRole.values()) {
				try{
					buttonBox.addButton(r.name(), r);
					fail("QThreadAffinityException expected to be thrown.");
				}catch(QThreadAffinityException e) {}
			}
		});
		thread.setUncaughtExceptionHandler((Thread t, Throwable e)->throwable = e);
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		if(throwable!=null)
			throw throwable;
	}
	
	@Test
    public void testQWindow() throws Throwable{
		throwable = null;
		QWindow widget = new QWindow();
		QEventLoop loop = new QEventLoop();
		QThread thread = QThread.create(()->{
			try{
				widget.setVisible(true);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.show();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.showFullScreen();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.showMaximized();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.showMinimized();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.showNormal();
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
			try{
				widget.setWindowState(Qt.WindowState.WindowFullScreen);
				fail("QThreadAffinityException expected to be thrown.");
			}catch(QThreadAffinityException e) {}
		});
		thread.setUncaughtExceptionHandler((Thread t, Throwable e)->throwable = e);
		thread.finished.connect(loop::quit);
		thread.start();
		loop.exec();
		if(throwable!=null)
			throw throwable;
	}
	
	public void testFunctionCall() throws InterruptedException{
		AtomicReference<QObject> object = new AtomicReference<>();
		QThread jthread = QThread.create(()->{
			object.set(new QObject());
			try {
				synchronized(TestQThread.class) {
					TestQThread.class.wait();
				}
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		});
		try {
			jthread.start();
			while(object.get()==null) {
				Thread.sleep(50);
			}
			object.get().startTimer(90);
			fail("QThreadAffinityException expected to be thrown.");
		}catch(QThreadAffinityException e) {
		}finally {
			synchronized(TestQThread.class) {
				TestQThread.class.notifyAll();
			}
			instances.add(new WeakReference<>(jthread));
		}
	}

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestThreadAffinity.class.getName());
    }
}
