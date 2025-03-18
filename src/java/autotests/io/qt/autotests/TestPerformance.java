/****************************************************************************
**
** Copyright (C) 2009-2025 Dr. Peter Droste, Omix Visualization GmbH & Co. KG. All rights reserved.
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

import java.io.IOException;
import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import org.junit.After;
import org.junit.Assert;
import org.junit.Assume;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;

import io.qt.NonNull;
import io.qt.Nullable;
import io.qt.autotests.generated.PerformanceTests;
import io.qt.autotests.generated.PerformanceTests.AbstractVirtualHost;
import io.qt.autotests.generated.PerformanceTests.VirtualHost;
import io.qt.core.QAbstractItemModel;
import io.qt.core.QAbstractListModel;
import io.qt.core.QAbstractTableModel;
import io.qt.core.QByteArray;
import io.qt.core.QEvent;
import io.qt.core.QModelIndex;
import io.qt.core.QSize;
import io.qt.core.QStringListModel;
import io.qt.core.Qt.Orientation;
import io.qt.widgets.QApplication;
import io.qt.widgets.QMainWindow;
import io.qt.widgets.QTableView;

public class TestPerformance extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
		Assume.assumeTrue("Performance tests are disabled. Specify -Denable-performance-tests=true to enable them.", Boolean.getBoolean("enable-performance-tests"));
		System.setProperty("io.qt.experimental.fast-jni", "true");
		System.setProperty("io.qt.experimental.fast-jni-for-overrides", "true");
    	ApplicationInitializer.testInitializeWithWidgets();
    }
	
	static final int count = 5000;
	private AbstractVirtualHost host1;
	private AbstractVirtualHost host2;
	private AbstractVirtualHost host3;
	private QAbstractItemModel model1;
	private QAbstractItemModel model2;
	private QAbstractItemModel model3;
	final static DecimalFormat format = new DecimalFormat();
	static {
		format.setDecimalFormatSymbols(DecimalFormatSymbols.getInstance(Locale.ENGLISH));
	}
	
	@Before
	public void initialized() {
		host1 = PerformanceTests.createNativeHost();
		host2 = new VirtualHost(){
			@Override
			public boolean event(@Nullable QEvent event) {
				return super.event(event);
			}
    	};
    	host3 = new AbstractVirtualHost(){
			@Override
			public void test() {
			}
			@Override
			public void test(@NonNull QModelIndex index) {
			}
			@Override
			public void test(@NonNull QSize size) {
			}
			@Override
			public void test(@NonNull String string) {
			}
			@Override
			public void test(int i) {
			}
			@Override
			public void test(@NonNull QSize size1, @NonNull QSize size2) {
			}
			@Override
			public void test(@NonNull QSize size1, @NonNull QSize size2, @NonNull QSize size3) {
			}
			@Override
			public void test(@NonNull QSize size1, @NonNull QSize size2, @NonNull QSize size3, @NonNull QSize size4) {
			}
			@Override
			public void test(@NonNull QSize size1, @NonNull QSize size2, @NonNull QSize size3, @NonNull QSize size4, @NonNull QSize size5) {
			}
    	};
    	model1 = new QStringListModel(Arrays.asList("1", "2", "3", "4", "5"));
    	model2 = new QStringListModel(Arrays.asList("1", "2", "3", "4", "5")){
			@Override
			public boolean event(@Nullable QEvent event) {
				return super.event(event);
			}
    	};
    	model3 = new QAbstractListModel() {
			@Override
			public Object data(@NonNull QModelIndex index, int role) {
				return ""+(index.row()+1);
			}

			@Override
			public int rowCount(@NonNull QModelIndex parent) {
				return 5;
			}
    	};
	}
	
	@After
	public void cleanup() {
		host1 = null;
		host2 = null;
		host3 = null;
    	runGC();
	}
	
//    @Test
    @SuppressWarnings("unused")
	public void testHeaderDataCalledWithRoles() throws InterruptedException, IOException {
    	Set<Integer> roles = new TreeSet<>();
    	int[] counter = {0};
    	int[] counterR = {0};
    	int[] counterC = {0};
    	QAbstractItemModel model = new QAbstractTableModel() {
			
			@Override
			public int rowCount(QModelIndex parent) {
				if(parent.isValid()) return 0;
				System.out.println("rowCount() "+counterR[0]++);
				System.out.flush();
				return 300000;
			}
			
			@Override
			public int columnCount(QModelIndex parent) {
				if(parent.isValid()) return 0;
				System.out.println("columnCount() "+counterC[0]++);
				System.out.flush();
				return 3;
			}
			
			@Override
			public Object data(QModelIndex index, int role) {
			    //qWarning("TableModel::data({%1$s,%2$s},%3$s(%4$s))", index.row(), index.column(), roleNames().get(role).toString(), role);
//				System.out.println("data() "+counter[0]);
//				System.out.flush();
				++counter[0];
				return null;
			}

			@Override
			public Object headerData(int section, Orientation orientation, int role) {
				Object result = super.headerData(section, orientation, role);
//				qWarning("TableModel::headerData(%1$s,%2$s,%3$s(%4$s)) = %5$s", section, orientation, roleNames().get(role).toString(), role, result);
				roles.add(role);
				return result;
			}
			
//			@Override
//			public @NonNull QModelIndex index(int row, int column, @NonNull QModelIndex parent) {
//				return createIndex(row, column);
//			}

//			@Override
//			public @NonNull QModelIndex parent(@NonNull QModelIndex child) {
//				return null;
//			}
		};
		QTableView view = new QTableView();
		view.setModel(model);
//		view.selectionModel().selectionChanged.connect(()->{
//			counter[0] = 0;
//			QTimer.singleShot(1, ()->{
//				System.out.println("selectionChanged "+counter[0]);
//			});
//		});
		QMainWindow window = new QMainWindow();
		window.setCentralWidget(view);
		window.show();
//		QTimer.singleShot(500, QApplication::quit);
		QApplication.exec();
//		QTimer.singleShot(15000, QApplication::quit);
//		QApplication.exec();
//		QApplication.processEvents();
		window.close();
		window.dispose();
		Assert.assertTrue("headerData called with role: "+roles, roles.size()>1);
    }
    
    @Test
    public void testAcuireJniEnvironment() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironment(count);
    	System.out.println("Calling testAcuireJniEnvironment(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentExceptionHandler() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentExceptionHandler(count);
    	System.out.println("Calling testAcuireJniEnvironmentExceptionHandler(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentExceptionHandlerAndBlocker() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentExceptionHandlerAndBlocker(count);
    	System.out.println("Calling testAcuireJniEnvironmentExceptionHandlerAndBlocker(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentCalling() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentCalling(count);
    	System.out.println("Calling testAcuireJniEnvironmentCalling(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentExceptionHandlerCalling() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentExceptionHandlerCalling(count);
    	System.out.println("Calling testAcuireJniEnvironmentExceptionHandlerCalling(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentExceptionHandlerAndBlockerCalling() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentExceptionHandlerAndBlockerCalling(count);
    	System.out.println("Calling testAcuireJniEnvironmentExceptionHandlerAndBlockerCalling(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentCallingChecking() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentCallingChecking(count);
    	System.out.println("Calling testAcuireJniEnvironmentCallingChecking(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentExceptionHandlerCallingChecking() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentExceptionHandlerCallingChecking(count);
    	System.out.println("Calling testAcuireJniEnvironmentExceptionHandlerCallingChecking(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentExceptionHandlerAndBlockerCallingChecking() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentExceptionHandlerAndBlockerCallingChecking(count);
    	System.out.println("Calling testAcuireJniEnvironmentExceptionHandlerAndBlockerCallingChecking(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testConvertQModelIndex() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testConvertQModelIndex(count);
    	System.out.println("Calling testConvertQModelIndex(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testConvertQModelIndexCopy() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testConvertQModelIndexCopy(count);
    	System.out.println("Calling testConvertQModelIndexCopy(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testConvertQModelIndexInvalid() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testConvertQModelIndexInvalid(count);
    	System.out.println("Calling testConvertQModelIndexInvalid(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testConvertQSize() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testConvertQSize(count);
    	System.out.println("Calling testConvertQSize(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testConvertQString() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testConvertQString(count);
    	System.out.println("Calling testConvertQString(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentCheckException1() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentCheckException1(count);
    	System.out.println("Calling testAcuireJniEnvironmentCheckException1(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testAcuireJniEnvironmentCheckException2() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcuireJniEnvironmentCheckException2(count);
    	System.out.println("Calling testAcuireJniEnvironmentCheckException2(): "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testPlain() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testPlain(host1, count);
    	System.out.println("Calling test() on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testPlain(host2, count);
    	System.out.println("Calling test() on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testPlain(host3, count);
    	System.out.println("Calling test() on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testQModelIndex() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testQModelIndex(host1, count);
    	System.out.println("Calling test(QModelIndex) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQModelIndex(host2, count);
    	System.out.println("Calling test(QModelIndex) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQModelIndex(host3, count);
    	System.out.println("Calling test(QModelIndex) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testQModelIndexInvalid() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testQModelIndexInvalid(host1, count);
    	System.out.println("Calling test(QModelIndex{invalid}) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQModelIndexInvalid(host2, count);
    	System.out.println("Calling test(QModelIndex{invalid}) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQModelIndexInvalid(host3, count);
    	System.out.println("Calling test(QModelIndex{invalid}) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testQString() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testQString(host1, count);
    	System.out.println("Calling test(QString) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQString(host2, count);
    	System.out.println("Calling test(QString) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQString(host3, count);
    	System.out.println("Calling test(QString) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testQSize1() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testQSize1(host1, count);
    	System.out.println("Calling test(QSize) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize1(host2, count);
    	System.out.println("Calling test(QSize) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize1(host3, count);
    	System.out.println("Calling test(QSize) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testQSize2() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testQSize2(host1, count);
    	System.out.println("Calling test(QSize,QSize) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize2(host2, count);
    	System.out.println("Calling test(QSize,QSize) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize2(host3, count);
    	System.out.println("Calling test(QSize,QSize) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testQSize3() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testQSize3(host1, count);
    	System.out.println("Calling test(QSize,QSize,QSize) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize3(host2, count);
    	System.out.println("Calling test(QSize,QSize,QSize) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize3(host3, count);
    	System.out.println("Calling test(QSize,QSize,QSize) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testQSize4() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testQSize4(host1, count);
    	System.out.println("Calling test(QSize,QSize,QSize,QSize) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize4(host2, count);
    	System.out.println("Calling test(QSize,QSize,QSize,QSize) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize4(host3, count);
    	System.out.println("Calling test(QSize,QSize,QSize,QSize) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testQSize5() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testQSize5(host1, count);
    	System.out.println("Calling test(QSize,QSize,QSize,QSize,QSize) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize5(host2, count);
    	System.out.println("Calling test(QSize,QSize,QSize,QSize,QSize) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testQSize5(host3, count);
    	System.out.println("Calling test(QSize,QSize,QSize,QSize,QSize) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testInt() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testInt(host1, count);
    	System.out.println("Calling test(int) on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testInt(host2, count);
    	System.out.println("Calling test(int) on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testInt(host3, count);
    	System.out.println("Calling test(int) on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testRowCount() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testRowCount(model1, count);
    	System.out.println("Calling rowCount() on native instance:            "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testRowCount(model2, count);
    	System.out.println("Calling rowCount() on ignoring java instance:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testRowCount(model3, count);
    	System.out.println("Calling rowCount() on implementing java instance: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @Test
    public void testThreadData() {
    	java.time.Duration duration;
    	duration = PerformanceTests.testAcquireThreadLocal(count);
    	System.out.println("Calling thread_local:       "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testAcquireQThreadStorage(count);
    	System.out.println("Calling QThreadStorage:     "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }
    
    @SuppressWarnings("unused")
	private static void test() throws InterruptedException {
//    	Thread.sleep(1);
    	int c = 0;
    	for (int i = 1; i < count; i++) {
			c %= i;
		}
    }
    
    @Test
    public void testClassCall() {
    	Map<Object,Object> classMap = new HashMap<>();
    	java.time.Duration duration;
    	duration = PerformanceTests.testClassCallBuffer(count, new QByteArray(TestPerformance.class.getName().replace('.', '/')), new QByteArray("test"), classMap);
    	System.out.println("Calling class on buffer map:         "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	classMap.clear();
    	duration = PerformanceTests.testClassCallString(count, new QByteArray(TestPerformance.class.getName().replace('.', '/')), new QByteArray("test"), classMap);
    	System.out.println("Calling class on string map:         "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	classMap.clear();
    	duration = PerformanceTests.testClassCallHash(count, new QByteArray(TestPerformance.class.getName().replace('.', '/')), new QByteArray("test"), classMap);
    	System.out.println("Calling class on hash map:         "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testClassCall(count, new QByteArray(TestPerformance.class.getName().replace('.', '/')), new QByteArray("test"));
    	System.out.println("Calling class:                       "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testClassCallGlobalRef(count, new QByteArray(TestPerformance.class.getName().replace('.', '/')), new QByteArray("test"));
    	System.out.println("Calling class with global reference: "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testClassCallLocalRef(count, new QByteArray(TestPerformance.class.getName().replace('.', '/')), new QByteArray("test"));
    	System.out.println("Calling class with local reference:  "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    	duration = PerformanceTests.testClassCallLocalFromGlobalRef(count, new QByteArray(TestPerformance.class.getName().replace('.', '/')), new QByteArray("test"));
    	System.out.println("Calling class with local reference from global:  "+format.format(duration.toNanos()*0.001)+"\u00b5s");
    }

    public static void main(String args[]) {
        org.junit.runner.JUnitCore.main(TestPerformance.class.getName());
    }
}
