/**
 * Unit Test implementations for QThread.java
 * 
 */

package org.qtjambi.generatortests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;

import org.qtjambi.qt.QThread;
import org.qtjambi.qt.QtJambi_LibraryInitializer;

public class TestQThread {

	private QThread qthread1;
	private QThread qthread2;
	private QThread qthread3;
	private QThread qthread4;
	private QThread qthread5;
	private ThreadGroup threads;
	private Runnable r;
	private Runnable stacked;
	private String threadName = "myThread";

	@org.junit.BeforeClass
	public static void setUpClass() {
		QtJambi_LibraryInitializer.init();
	}

	@org.junit.Before
	public void setUp() {
		threads = new ThreadGroup("qtjambiThreads");
		qthread1 = new QThread(r);
		qthread2 = new QThread(threads, qthread2);
		qthread3 = new QThread(r, threadName);
		qthread4 = new QThread(threads, r, threadName);
		qthread5 = new QThread(threads, stacked, threadName, 1);
	}

	@org.junit.After
	public void tearDown() {
		qthread1 = null;
	}

	@org.junit.Test
	public void testGroupConstructor() {
		assertEquals("qthread2.getThreadGroup()", qthread2.getThreadGroup(), threads);
	}

	@org.junit.Test
	public void testNameConstructor() {
		assertEquals("qthread3.getName()", qthread3.getName(), threadName);
	}

	@org.junit.Test
	public void testNameGroupConstructor() {
		assertEquals("qthread4.getThreadGroup()", qthread4.getThreadGroup(), threads);
		assertEquals("qthread4.getName()", qthread4.getName(), threadName);
	}

	/*
	 * @org.junit.Test public void testNameGroupStackSize() { }
	 */

	@org.junit.Test
	public void testRun() {
		qthread1.run();
		assertFalse("qthread1.isAlive()", qthread1.isAlive());
	}

	@org.junit.Test
	public void testInit() {
		assertNotNull("qthread1.starting", qthread1.starting);
		assertNotNull("qthread1.finished", qthread1.finished);
	}

}
