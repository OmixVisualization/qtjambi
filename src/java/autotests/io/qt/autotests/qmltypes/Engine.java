package io.qt.autotests.qmltypes;

import io.qt.core.QObject;
import io.qt.qml.util.QmlElement;

@QmlElement
public class Engine extends QObject {
	private Engine(QDeclarativeConstructor dc) throws IllegalAccessException {
		super(dc);
	}
	
	private int performance;

	public int getPerformance() {
		return performance;
	}

	public void setPerformance(int performance) {
		this.performance = performance;
	}
}
