package io.qt.autotests.qmlreg;

import io.qt.QtAsGadget;
import io.qt.QtObject;

@QtAsGadget
public class PointFQtObjectAccess extends QtObject{
	public PointFQtObjectAccess(QPrivateConstructor p) {
		super(p);
		pointF = null;
	}
	
	public double x() {return pointF.x();}
    public double y() {return pointF.y();}
    public void setX(double x) {pointF.setX(x);}
    public void setY(double y) {pointF.setY(y);}
    
    static PointFQtObjectAccess wrap(PointF pointF) {
    	return new PointFQtObjectAccess(pointF);
    }
    
    private final PointF pointF;

	private PointFQtObjectAccess(PointF pointF) {
		super();
		this.pointF = pointF;
	}
}