package io.qt.autotests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.lang.ref.WeakReference;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.QtObjectInterface;
import io.qt.autotests.generated.FunctionalTest;
import io.qt.autotests.generated.General;
import io.qt.autotests.generated.QList_String;
import io.qt.core.QBuffer;
import io.qt.core.QEasingCurve;
import io.qt.core.QHash;
import io.qt.core.QList;
import io.qt.core.QMap;
import io.qt.core.QMetaMethod;
import io.qt.core.QMetaObject;
import io.qt.core.QMetaProperty;
import io.qt.core.QModelIndex;
import io.qt.core.QMultiHash;
import io.qt.core.QObject;
import io.qt.core.QQueue;
import io.qt.core.QRunnable;
import io.qt.core.QSet;
import io.qt.core.QStack;
import io.qt.core.Qt;
import io.qt.gui.QGradient;
import io.qt.gui.QRadialGradient;
import io.qt.gui.QStandardItemModel;
import io.qt.widgets.QDialog;
import io.qt.widgets.QGraphicsItem;
import io.qt.widgets.QGraphicsWidget;

public class TestQtjambiCast extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }

	@Test
	public void test() {
		class TestObject extends QObject implements QEasingCurve.EasingFunction, QRunnable{
			@Override
			public double applyAsDouble(double arg__1) {
				return 0;
			}
			@Override
			public void run() {
			}
			{
//				QtUtilities.initializeNativeObject(this);
			}
		}
		new TestObject();
		QObject qObject = new QStandardItemModel(); 
		QGraphicsItem graphicsItem = new QGraphicsWidget();
		QGradient gradient = new QRadialGradient(2, 3, 4);
		QEasingCurve.EasingFunction functionalPointer = d->d;
		FunctionalTest.TestFunction1 functional = (a,b)->a;
		QList_String customCList = new QList_String();
		customCList.append(Arrays.asList("A", "B", "C"));
		List<String> customJavaList = new ArrayList<>();
		customJavaList.addAll(Arrays.asList("A", "B", "C", "D"));
		String text = "TestQtjambiCast::test()";
		List<Object> list = new ArrayList<>();
		List<Class<?>> types = Arrays.asList(
				io.qt.widgets.QLabel.class, 
				io.qt.widgets.QGraphicsPixmapItem.class, 
				io.qt.gui.QColor.class, 
				io.qt.gui.QColor.class, 
				io.qt.gui.QColor.class, 
				io.qt.gui.QHideEvent.class, //5
				io.qt.gui.QHideEvent.class, 
				io.qt.core.Qt.ItemFlags.class, 
				Integer.class,
				io.qt.widgets.QWidgetItem.class, 
				io.qt.widgets.QPushButton.class, //10
				io.qt.widgets.QSlider.class,
				io.qt.core.QList.class, 
				io.qt.core.QPair.class, 
				io.qt.core.QList.class, 
				io.qt.core.QList.class, 
				io.qt.core.QUrl.FormattingOptions.class, 
				io.qt.core.QHash.class, 
				java.lang.Double.class, 
				java.lang.Integer.class, 
				io.qt.core.QMap.class, //20
				FunctionalTest.TestFunction1.class,
				QEasingCurve.EasingFunction.class,
				String.class,
				String.class,
				QMetaObject.class,
				QMetaMethod.class,
				QMetaProperty.class,
				QModelIndex.class,
				QDialog.class,
				Double.class,//30
				io.qt.qml.QQmlListProperty.class,
				QBuffer.class,
				QList.class,
				QList.class,
				QSet.class,
				QSet.class,
				QQueue.class,
				QQueue.class,
				QList.class,
				QList.class,//40
				QStack.class,
				QStack.class,//42
				QMap.class,
				QMap.class,
				QHash.class,
				QHash.class,
				QMultiHash.class,
				QMultiHash.class,
				int[].class,
				List[].class,//50
				QList.class,
				QQueue.class,//52
				QHash.class,
				String.class,
				String.class,
				String.class,
				qObject.getClass(), 
				graphicsItem.getClass(), 
				gradient.getClass(), 
				functionalPointer.getClass(), 
				functional.getClass(),
				customCList.getClass(),
				QList.class,//60
				text.getClass()
			);
		QList<java.lang.Boolean> internalTests = General.start_qtjambi_cast_test(list, qObject, graphicsItem, gradient, functionalPointer, functional, customCList, customJavaList, text);
		
		assertEquals(types.size(), list.size());
		assertEquals("U8 \u00f6", list.get(list.size()-11));
		assertEquals("U16 \u0554", list.get(list.size()-10));
		assertEquals("U16 \ufb49", list.get(list.size()-9));
		assertEquals(qObject, list.get(list.size()-8));
		assertEquals(graphicsItem, list.get(list.size()-7));
		assertEquals(gradient, list.get(list.size()-6));
		assertEquals(functionalPointer, list.get(list.size()-5));
		assertEquals(functional, list.get(list.size()-4));
		assertEquals(customCList, list.get(list.size()-3));
		assertEquals(customJavaList, list.get(list.size()-2));
		assertEquals(text, list.get(list.size()-1));
		
		for (int i = 0; i < list.size() && i < types.size(); i++) {
			assertTrue(list.get(i)!=null);
			assertTrue("unequals classes at "+i+": "+list.get(i).getClass()+" != "+types.get(i), types.get(i).isInstance(list.get(i)));
			if(list.get(i) instanceof QtObjectInterface)
				instances.add(new WeakReference<>((QtObjectInterface)list.get(i)));
		}
		assertEquals(new Qt.ItemFlags(Qt.ItemFlag.ItemIsDropEnabled, Qt.ItemFlag.ItemIsEditable), list.get(7));
		assertEquals(new Qt.ItemFlags(Qt.ItemFlag.ItemIsDropEnabled, Qt.ItemFlag.ItemIsEditable).value(), list.get(8));
		assertEquals("start_qtjambi_cast_test has "+internalTests.count(false)+" fails", -1, internalTests.indexOf(false));
	}	
}
