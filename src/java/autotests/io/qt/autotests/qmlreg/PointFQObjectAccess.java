package io.qt.autotests.qmlreg;

import io.qt.core.QObject;

public class PointFQObjectAccess extends QObject{
	public double x() {return pointF.x();}
    public double y() {return pointF.y();}
    public void setX(double x) {pointF.setX(x);}
    public void setY(double y) {pointF.setY(y);}
    
    static PointFQObjectAccess wrap(PointF pointF) {
    	return new PointFQObjectAccess(pointF);
    }
    
    private final PointF pointF;

	private PointFQObjectAccess(PointF pointF) {
		super((QPrivateConstructor)null);
		this.pointF = pointF;
	}
}