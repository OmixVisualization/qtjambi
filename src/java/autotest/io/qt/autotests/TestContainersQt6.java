package io.qt.autotests;

import static org.junit.Assert.fail;

import org.junit.Test;

import io.qt.autotests.generated.QList_int;

public class TestContainersQt6 extends ApplicationInitializer {
    @Test
    public void run_testQList_outofbounds() {
    	QList_int list = new QList_int();

        try { 
        	list.remove(23, 14); 
        	fail("IndexOutOfBoundsException expected to be thrown");
    	} catch (IndexOutOfBoundsException e) {
    	}
    }
}
