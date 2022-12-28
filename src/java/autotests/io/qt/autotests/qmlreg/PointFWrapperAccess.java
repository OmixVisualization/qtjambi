package io.qt.autotests.qmlreg;

import io.qt.QtAsGadget;

@QtAsGadget
public class PointFWrapperAccess{
	public double x() {return pointF.x();}
    public double y() {return pointF.y();}
    public void setX(double x) {pointF.setX(x);}
    public void setY(double y) {pointF.setY(y);}
    
    static PointFWrapperAccess wrap(PointF pointF) {
    	return new PointFWrapperAccess(pointF);
    }
    
    private final PointF pointF;

	private PointFWrapperAccess(PointF pointF) {
		super();
		this.pointF = pointF;
	}
}