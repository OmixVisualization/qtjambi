/**
 * Unit Test implementations for QPair.java
 * 
 */

package org.qtjambi.generatortests;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.qtjambi.qt.QPair;

public class TestQPair<T, S> {

	QPair<Integer, Integer> qp1;
	QPair<Integer, Integer> qp2;
	QPair<Integer, Boolean> qp3;
	QPair<Integer, Boolean> qp4;
	QPair<Integer, Integer> qp6;
	QPair<Integer, Integer> qp7;
	Object someObject;

	@org.junit.Before
	public void setUp() {
		someObject = null;
		qp1 = new QPair<Integer, Integer>(3, 5);
		qp2 = new QPair<Integer, Integer>(5, 3);
		qp3 = new QPair<Integer, Boolean>(1, null);
		qp4 = new QPair<Integer, Boolean>(null, true);
		qp6 = new QPair<Integer, Integer>(3, 6);
		qp7 = new QPair<Integer, Integer>(4, 5);
	}

	@org.junit.After
	public void tearDown() {
		qp1 = null;
		qp2 = null;
		qp3 = null;
		qp4 = null;
		qp6 = null;
		qp7 = null;
		someObject = null;
	}

	@org.junit.Test
	public void testEquals() {
		//@if
		//other == null
		assertFalse("qp1 vs someObject with null", qp1.equals(someObject));
		//other.second == null
		assertFalse("qp1 vs qp3 with null S", qp1.equals(qp3));
		//other.first == null
		assertFalse("qp1 vs qp4 with null T", qp1.equals(qp4));
		//first == null
		assertFalse("qp3 null S vs qp1", qp3.equals(qp1));
		//second == null
		assertFalse("qp4 null T vs qp1", qp4.equals(qp1));
		
		//@return
		//first equals
		assertFalse("qp1 vs qp6 with matching T", qp1.equals(qp6));
		//second equals
		assertFalse("qp1 vs qp7 with matching S", qp1.equals(qp7));
	}

	@org.junit.Test
	public void testToString() {
		assertEquals("qp1", "Pair(3,5)",       qp1.toString());
		assertEquals("qp3", "Pair(1,null)",    qp3.toString());
		assertEquals("qp4", "Pair(null,true)", qp4.toString());
	}

	@org.junit.Test
	public void testClone() {
		qp1 = qp2.clone();
		assertEquals("qp1==qp2", qp1, qp2);
	}

}
