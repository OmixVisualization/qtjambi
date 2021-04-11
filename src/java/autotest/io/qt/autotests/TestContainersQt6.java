package io.qt.autotests;

import static org.junit.Assert.assertTrue;

import org.junit.After;
import org.junit.Assume;
import org.junit.Before;
import org.junit.Test;

import io.qt.autotests.generated.QList_int;
import io.qt.autotests.generated.Tulip;

public class TestContainersQt6 extends QApplicationTest {
//	private Tulip tulip;

    @Before
    public void setUp() throws Exception {
//        tulip = new Tulip();
    }

    @After
    public void tearDown() throws Exception {
//        tulip = null;
    }
    
    @Test
    public void run_testQList_outofbounds() {
    	QList_int list = new QList_int();

        boolean got;
        got = false; try { list.remove(23, 14); } catch (Exception e) { got =true; } assertTrue("exception expected", got);
    }
    
    @Test
    public void emptyTest() {
		Assume.assumeTrue(false);
	}
}
