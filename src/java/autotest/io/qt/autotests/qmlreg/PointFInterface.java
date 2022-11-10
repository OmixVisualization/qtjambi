package io.qt.autotests.qmlreg;

import io.qt.QtAsGadget;

@QtAsGadget
public interface PointFInterface{
	public double x();
    public double y();
    public void setX(double x);
    public void setY(double y);
}