package io.qt.autotests.qmlreg;

import io.qt.QtUninvokable;

public class PointF implements Cloneable{

	public PointF() {
	}

	private double x, y;

	@QtUninvokable
	public double x() {
		return x;
	}

	@QtUninvokable
	public void setX(double x) {
		this.x = x;
	}

	@QtUninvokable
	public double y() {
		return y;
	}

	@QtUninvokable
	public void setY(double y) {
		this.y = y;
	}

	@Override
	@QtUninvokable
	public String toString() {
		return "PointF [x=" + x + ", y=" + y + "]";
	}

	@Override
	@QtUninvokable
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		long temp;
		temp = Double.doubleToLongBits(x);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		temp = Double.doubleToLongBits(y);
		result = prime * result + (int) (temp ^ (temp >>> 32));
		return result;
	}

	@Override
	@QtUninvokable
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		PointF other = (PointF) obj;
		if (Double.doubleToLongBits(x) != Double.doubleToLongBits(other.x))
			return false;
		if (Double.doubleToLongBits(y) != Double.doubleToLongBits(other.y))
			return false;
		return true;
	}
}
