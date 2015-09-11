package org.qtjambi.autotests;

import static org.junit.Assert.*;

import java.lang.ref.WeakReference;
import java.math.BigInteger;
import java.util.ArrayList;
import java.util.List;

import org.junit.Before;
import org.junit.Test;

import org.qtjambi.autotests.generated.SharedPointerTest;
import org.qtjambi.qt.QFlags;
import org.qtjambi.qt.QNoSuchEnumValueException;
import org.qtjambi.qt.QtBlockedSlot;
import org.qtjambi.qt.QtEnumerator;
import org.qtjambi.qt.QtJambi_LibraryInitializer;
import org.qtjambi.qt.QtPropertyNotify;
import org.qtjambi.qt.QtPropertyReader;
import org.qtjambi.qt.QtPropertyWriter;
import org.qtjambi.qt.core.QCoreApplication;
import org.qtjambi.qt.core.QEvent;
import org.qtjambi.qt.core.QObject;
import org.qtjambi.qt.core.QRectF;
import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.widgets.QApplication;
import org.qtjambi.qt.gui.QColor;
import org.qtjambi.qt.gui.QDrag;
import org.qtjambi.qt.widgets.QGraphicsItem;
import org.qtjambi.qt.widgets.QGraphicsItemInterface;
import org.qtjambi.qt.widgets.QGraphicsTextItem;
import org.qtjambi.qt.gui.QPainter;
import org.qtjambi.qt.core.QStringListModel;
import org.qtjambi.qt.core.QTimer;
import org.qtjambi.qt.widgets.QStyleOptionGraphicsItem;
import org.qtjambi.qt.widgets.QWidget;
import org.qtjambi.qt.internal.MetaObjectTools;

public class TestSharedPointer extends QObject {

	public static void main(String args[]) {
		org.junit.runner.JUnitCore.main(TestSharedPointer.class.getName());
	}

	static {
		org.qtjambi.qt.Utilities.loadQtLibrary("QtCore");
        if(org.qtjambi.qt.Utilities.isAvailableUtilityLibrary("libGLESv2", ""))
            org.qtjambi.qt.Utilities.loadUtilityLibrary("libGLESv2", "");         // load if exists
        if(org.qtjambi.qt.Utilities.isAvailableUtilityLibrary("libEGL", ""))
            org.qtjambi.qt.Utilities.loadUtilityLibrary("libEGL", "");         // load if exists
        org.qtjambi.qt.Utilities.loadQtLibrary("QtGui");
        org.qtjambi.qt.Utilities.loadQtLibrary("QtNetwork");
        if(org.qtjambi.qt.Utilities.isAvailableQtLibrary("QtXml"))
            org.qtjambi.qt.Utilities.loadQtLibrary("QtXml"); // usually available
        if(org.qtjambi.qt.Utilities.isAvailableQtLibrary("QtSql"))
            org.qtjambi.qt.Utilities.loadQtLibrary("QtSql"); // load if exists
              org.qtjambi.qt.Utilities.loadJambiJniLibrary("org_qtjambi_autotests_generated");
		org.qtjambi.qt.Utilities.loadQtLibrary("QtXml"); // usually available
		org.qtjambi.qt.Utilities.loadQtLibrary("QtSql"); // load if exists
	}

	private SharedPointerTest object = new SharedPointerTest(this);

	@Before
	public void setUp() {
		if (QApplication.startingUp()) {
			QCoreApplication.addLibraryPath("plugins");
			QApplication.initialize(new String[] {});
		}
	}

	@Test
	public void testSharedObject10() {
		QObject sharedObject = object.createSharedObject1();
		assertNotNull(sharedObject);
		assertEquals("SharedObject1", sharedObject.objectName());
		assertEquals("", object.deletedSharedObjectName());
		sharedObject.dispose();
		assertEquals("SharedObject1", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject11() {
		QObject sharedObject = object.createSharedObject1();
		assertNotNull(sharedObject);
		assertEquals("SharedObject1", sharedObject.objectName());
		assertEquals("", object.deletedSharedObjectName());
		WeakReference<QObject> weak = new WeakReference<QObject>(sharedObject);
		sharedObject = null;
		while(weak.get()!=null){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		while(object.deletedSharedObjectName().isEmpty()){
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
		}
		assertEquals("SharedObject1", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject20() {
		QGraphicsItemInterface sharedObject = object.createSharedObject2();
		assertNotNull(sharedObject);
		assertEquals(QGraphicsTextItem.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		((QGraphicsTextItem)sharedObject).dispose();
		assertEquals("SharedObject2", object.deletedSharedObjectName());
	}

	@Test
	public void testSharedObject21() {
		QGraphicsItemInterface sharedObject = object.createSharedObject2();
		assertNotNull(sharedObject);
		assertEquals(QGraphicsTextItem.class, sharedObject.getClass());
		assertEquals("", object.deletedSharedObjectName());
		sharedObject.setJavaOwnership();
		WeakReference<QGraphicsItemInterface> weak = new WeakReference<QGraphicsItemInterface>(sharedObject);
		sharedObject = null;
		while(weak.get()!=null){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		while(object.deletedSharedObjectName().isEmpty()){
			QApplication.sendPostedEvents(null, QEvent.Type.DeferredDelete.value());
		}
		assertEquals("SharedObject2", object.deletedSharedObjectName());
	}
	
	@Test
	public void testSharedObject30() {
		QEvent sharedObject = object.createSharedObject3();
		assertNotNull(sharedObject);
		assertEquals(QEvent.Type.User, sharedObject.type());
		assertEquals("", object.deletedSharedObjectName());
		sharedObject.dispose();
		assertEquals("SharedEvent", object.deletedSharedObjectName());
	}

	@Test
	public void testSharedObject31() {
		QEvent sharedObject = object.createSharedObject3();
		assertNotNull(sharedObject);
		assertEquals("", object.deletedSharedObjectName());
		sharedObject.setJavaOwnership();
		sharedObject = null;
		while(object.deletedSharedObjectName().isEmpty()){
			System.gc();
			synchronized(object){
				try {
					object.wait(50);
				} catch (InterruptedException e) {
				}
			}
		}
		assertEquals("SharedEvent", object.deletedSharedObjectName());
	}
}
