package io.qt.core;

import io.qt.NonNull;
import io.qt.QtObject;

/**
 * <p>Java wrapper for Qt's class <a href="https://doc.qt.io/qt/qpermission.html">QPermission</a></p>
 */
public abstract class QPermission extends QtObject implements java.lang.Cloneable{
	QPermission(QPrivateConstructor p) {
		super(p);
	}

	@Override
	public abstract @NonNull QPermission clone();
}
