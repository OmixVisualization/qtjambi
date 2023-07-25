package io.qt.autotests;

import java.util.ArrayList;
import java.util.List;

import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.autotests.generated.Tulip;
import io.qt.core.QList;
import io.qt.core.QObject;
import io.qt.core.QStringList;
import io.qt.gui.QColor;

public class TestContainerPerformance extends ApplicationInitializer{
	
	private static final int CAPACITY = 50000;
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	@Test
    public void testIntListPerformance() {
		long tdiff1;
		long tdiffc1;
		Integer object = Integer.valueOf(5);
		{
			QList<Integer> list = new QList<>(int.class);
//			list.reserve(CAPACITY);
			long t1 = System.currentTimeMillis();
//			list.fill(object, CAPACITY);
			for (int i = 0; i < CAPACITY; i++) {
				list.append(object);
			}
			long t2 = System.currentTimeMillis();
			Tulip.consumeIntList(list);
			long t3 = System.currentTimeMillis();
			list.dispose();
			tdiff1 = t2 - t1;
			tdiffc1 = t3 - t2;
		}
		long tdiff2;
		long tdiffc2;
		{
			List<Integer> jlist = new ArrayList<>();
			long t1 = System.currentTimeMillis();
			for (int i = 0; i < CAPACITY; i++) {
				jlist.add(object);
			}
			long t2 = System.currentTimeMillis();
			Tulip.consumeIntList(jlist);
			long t3 = System.currentTimeMillis();
			tdiff2 = t2 - t1;
			tdiffc2 = t3 - t2;
		}
		System.out.println("fill QList<int> performance: "+tdiff1+"ms");
		System.out.println("fill ArrayList<Integer> performance: "+tdiff2+"ms");
		System.out.println("consume QList<int> performance: "+tdiffc1+"ms");
		System.out.println("consume ArrayList<Integer> performance: "+tdiffc2+"ms");
//		System.out.println("native QList<int> performance: "+Tulip.fillIntList(CAPACITY)+"ms");
		System.out.println();
	}
	
	@Test
    public void testStringListPerformance() {
		long tdiff1;
		long tdiffc1;
		String object = "TEST";
		{
			QList<String> list = new QStringList();
//			list.resize(CAPACITY);
			long t1 = System.currentTimeMillis();
//			list.fill(object, CAPACITY);
			for (int i = 0; i < CAPACITY; i++) {
				list.append(object);
			}
			long t2 = System.currentTimeMillis();
			Tulip.consumeStringList(list);
			long t3 = System.currentTimeMillis();
			list.dispose();
			tdiff1 = t2 - t1;
			tdiffc1 = t3 - t2;
		}
		long tdiff2;
		long tdiffc2;
		{
			List<String> jlist = new ArrayList<>();
			long t1 = System.currentTimeMillis();
			for (int i = 0; i < CAPACITY; i++) {
				jlist.add(object);
			}
//			QList<String> list = new QList<>(jlist);
			long t2 = System.currentTimeMillis();
			Tulip.consumeStringList(jlist);
			long t3 = System.currentTimeMillis();
//			list.dispose();
			tdiff2 = t2 - t1;
			tdiffc2 = t3 - t2;
		}
		System.out.println("fill QList<QString> performance: "+tdiff1+"ms");
		System.out.println("fill ArrayList<String> performance: "+tdiff2+"ms");
		System.out.println("consume QList<QString> consume performance: "+tdiffc1+"ms");
		System.out.println("consume ArrayList<String> consume performance: "+tdiffc2+"ms");
		System.out.println();
	}
	
	@Test
    public void testQColorListPerformance() {
		long tdiff1;
		QColor object = new QColor();
		{
			QList<QColor> list = new QList<>(QColor.class);
//			list.resize(CAPACITY);
			long t1 = System.currentTimeMillis();
//			list.fill(object, CAPACITY);
			for (int i = 0; i < CAPACITY; i++) {
				list.append(object);
			}
			long t2 = System.currentTimeMillis();
			list.dispose();
			tdiff1 = t2 - t1;
		}
		long tdiff2;
		{
			List<QColor> jlist = new ArrayList<>();
			long t1 = System.currentTimeMillis();
			for (int i = 0; i < CAPACITY; i++) {
				jlist.add(object);
			}
			long t2 = System.currentTimeMillis();
			tdiff2 = t2 - t1;
		}
		System.out.println("fill QList<QColor> performance: "+tdiff1+"ms");
		System.out.println("fill ArrayList<QColor> performance: "+tdiff2+"ms");
		System.out.println();
	}
	
	@Test
    public void testQObjectListPerformance() {
		long tdiff1;
		long tdiffc1;
		QObject object = new QObject();
		{
			QList<QObject> list = new QList<>(QObject.class);
//			list.resize(CAPACITY);
			long t1 = System.currentTimeMillis();
//			list.fill(object, CAPACITY);
			for (int i = 0; i < CAPACITY; i++) {
				list.append(object);
			}
			long t2 = System.currentTimeMillis();
			Tulip.consumeQObjectList(list);
			long t3 = System.currentTimeMillis();
			list.dispose();
			tdiff1 = t2 - t1;
			tdiffc1 = t3 - t2;
		}
		long tdiff2;
		long tdiffc2;
		{
			List<QObject> jlist = new ArrayList<>();
			long t1 = System.currentTimeMillis();
			for (int i = 0; i < CAPACITY; i++) {
				jlist.add(object);
			}
//			QList<Integer> list = new QList<>(jlist);
			long t2 = System.currentTimeMillis();
			Tulip.consumeQObjectList(jlist);
			long t3 = System.currentTimeMillis();
//			list.dispose();
			tdiff2 = t2 - t1;
			tdiffc2 = t3 - t2;
		}
		System.out.println("fill QList<QObject*> performance: "+tdiff1+"ms");
		System.out.println("fill ArrayList<QObject> performance: "+tdiff2+"ms");
		System.out.println("consume QList<QObject*> performance: "+tdiffc1+"ms");
		System.out.println("consume ArrayList<QObject> performance: "+tdiffc2+"ms");
		System.out.println();
	}
}
