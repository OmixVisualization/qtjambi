/****************************************************************************
**
** Copyright (C) 2011 Poalo Pacussi
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

import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import java.lang.ref.PhantomReference;
import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.WeakReference;
import java.util.HashMap;
import java.util.Map;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import io.qt.core.QCoreApplication;
import io.qt.core.QEvent;
import io.qt.core.QObject;
import io.qt.core.QOperatingSystemVersion;

public class TestSignalSlotGC extends ApplicationInitializer {

	/** Longest time to wait for garbage collection in ms. */
	private static final int TIME_LIMIT = 10000;

	private ReferenceQueue<Object> weakReferenceQueue = new ReferenceQueue<Object>();
	private ReferenceQueue<Object> phantomReferenceQueue = new ReferenceQueue<Object>();

	private Map<WeakReference<Object>, Integer> weakReferenceMap = new HashMap<WeakReference<Object>, Integer>();
	private Map<PhantomReference<Object>, Integer> phantomReferenceMap = new HashMap<PhantomReference<Object>, Integer>();
	private int counter = 0;

	@Before
	public void setUp() {
	}

	@After
	public void tearDown() {
	}

	// /////////////////////////////////////////////////
	// Tests start

	@Test
	public void testExternalConnection() {
		reset();
		createExternalConnection_internal();
		external.emit();

		gcAndWait();

		synchronized (TestSignalSlotGC.class) {
			assertTrue(weakReferenceMap.isEmpty());
			assertTrue(phantomReferenceMap.isEmpty());
		}
	}

	private Emitter external = new Emitter();

	// Done like this to ensure the reference to a Receiver object created in
	//  createLocalConnection() is discarded with the stack frame during the return.
	private void createExternalConnection_internal() {
		createExternalConnection();
	}

	private Object createExternalConnection() {
		Receiver r = new Receiver(external);
		registerObject(r);

		return r;
	}

	@Test
	public void testLocalConnection() {
		reset();
		createLocalConnection_internal();

		gcAndWait();

		synchronized (TestSignalSlotGC.class) {
			assertTrue(weakReferenceMap.isEmpty());
			assertTrue(phantomReferenceMap.isEmpty());
		}
	}

	// Done like this to ensure the reference to a Receiver object created in
	//  createLocalConnection() is discarded with the stack frame during the return.
	private void createLocalConnection_internal() {
		createLocalConnection();
	}

	private Object createLocalConnection() {
		Emitter local = new Emitter();
		Receiver r = new Receiver(local);
		registerObject(r);

		return r;
	}

	// Tests end
	// ///////////////////////////////////////////////

	private void reset() {
		synchronized (TestSignalSlotGC.class) {
			weakReferenceMap.clear();
			phantomReferenceMap.clear();
		}
	}

	private void gcAndWait() {
		final long start = System.currentTimeMillis();

		while (System.currentTimeMillis() - start < TIME_LIMIT) {
			ApplicationInitializer.runGC();
			try {
				Thread.sleep(100);
			} catch(InterruptedException e) {
				break;
			}
			QCoreApplication.processEvents();
			QCoreApplication.sendPostedEvents(null, QEvent.Type.DeferredDispose);

			Reference<? extends Object> thisWr;
			while ((thisWr = weakReferenceQueue.poll()) != null) {
				synchronized (TestSignalSlotGC.class) {
					weakReferenceMap.remove(thisWr);
				}
			}
			Reference<? extends Object> thisPr;
			while ((thisPr = phantomReferenceQueue.poll()) != null) {
				synchronized (TestSignalSlotGC.class) {
					phantomReferenceMap.remove(thisPr);
				}
			}

			synchronized (TestSignalSlotGC.class) {
				if (weakReferenceMap.isEmpty() && phantomReferenceMap.isEmpty()) {
					break; // everything has already been gc'ed
				}
			}
		}
	}

	private void registerObject(Object o) {
		assertNotNull(o);

		Integer counterObject = Integer.valueOf(counter);
		synchronized (TestSignalSlotGC.class) {
			WeakReference<Object> thisWr = new WeakReference<Object>(o, weakReferenceQueue);
			weakReferenceMap.put(thisWr, counterObject);
			PhantomReference<Object> thisPr = new PhantomReference<Object>(o, phantomReferenceQueue);
			phantomReferenceMap.put(thisPr, counterObject);
		}
		counter++;
	}

	static class Emitter extends QObject {
		public final Signal0 signal = new Signal0();

		public void emit() {
			signal.emit();
		}
	}

	static class Receiver extends QObject {
		public Receiver(Emitter e) {
			if(QOperatingSystemVersion.current().isAnyOfType(QOperatingSystemVersion.OSType.Android))
				e.signal.connect(this, "slot()");
			else
				e.signal.connect(this::slot);
		}

		void slot() {
			java.util.logging.Logger.getLogger("io.qt.autotests").log(java.util.logging.Level.FINE, Receiver.class.getName() + "#slot(): fired");
		}
	}

	public static void main(String[] args) {
		org.junit.runner.JUnitCore.main(TestSignalSlotGC.class.getName());
	}
}
