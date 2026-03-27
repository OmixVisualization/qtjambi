package io.qt.autotests;

import static org.junit.Assert.*;

import org.junit.*;

import io.qt.core.*;
import io.qt.gui.*;

public class TestInjectedCodeQt611 extends ApplicationInitializer {
	
	@BeforeClass
    public static void testInitialize() throws Exception {
    	ApplicationInitializer.testInitializeWithWidgets();
    }
    
    @Test
    public void testQQuaternion() {
    	QQuaternion quarternion = new QQuaternion(1,2,3,4);
    	QQuaternion.Axes axes = quarternion.toAxes();
    	QQuaternion.AxisAndAngle axisAndAngle = quarternion.getAxisAndAngle();
    	QQuaternion.EulerAngles eulerAngles = quarternion.getEulerAngles();
    	assertEquals(-49, axes.x().x(), 0.001);
    	assertEquals(20, axes.x().y(), 0.001);
    	assertEquals(10, axes.x().z(), 0.001);
    	assertEquals(4, axes.y().x(), 0.001);
    	assertEquals(-39, axes.y().y(), 0.001);
    	assertEquals(28, axes.y().z(), 0.001);
    	assertEquals(22, axes.z().x(), 0.001);
    	assertEquals(20, axes.z().y(), 0.001);
    	assertEquals(-25, axes.z().z(), 0.001);
    	assertTrue(axisAndAngle.axis!=null);
    	assertEquals(0.371391f, axisAndAngle.axis.x(), 0.001);
    	assertEquals(0.557086f, axisAndAngle.axis.y(), 0.001);
    	assertEquals(0.742781f, axisAndAngle.axis.z(), 0.001);
    	if(QLibraryInfo.version().compareTo(new QVersionNumber(6,7,0))>=0)
    		assertEquals(158.96054077148438f, axisAndAngle.angle, 0.001);
    	assertEquals(-41.81031799316406f, eulerAngles.pitch(), 0.001);
    	assertEquals(79.69515228271484f, eulerAngles.yaw(), 0.001);
    	assertEquals(116.56504821777344f, eulerAngles.roll(), 0.001);
    }
}
