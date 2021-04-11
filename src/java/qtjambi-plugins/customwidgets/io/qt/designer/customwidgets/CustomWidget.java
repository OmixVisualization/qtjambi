package io.qt.designer.customwidgets;

import io.qt.core.QRect;
import io.qt.core.Qt;
import io.qt.core.Qt.WindowFlags;
import io.qt.core.Qt.WindowType;
import io.qt.gui.QKeySequence;
import io.qt.widgets.QWidget;

public class CustomWidget extends QWidget {

	public CustomWidget() {
		super();
	}

	public CustomWidget(QDeclarativeConstructor constructor) {
		super(constructor);
	}

	public CustomWidget(QPrivateConstructor p) {
		super(p);
	}

	public CustomWidget(QWidget parent, WindowFlags f) {
		super(parent, f);
	}

	public CustomWidget(QWidget parent, WindowType... f) {
		super(parent, f);
	}

	public CustomWidget(QWidget parent) {
		super(parent);
	}
	
	private String customText = "";
	private QRect customRect = new QRect();
	private QKeySequence customKeySequence = new QKeySequence();
	private Qt.ArrowType customArrowType = Qt.ArrowType.LeftArrow;
	
	public String customText() {
		return customText;
	}

	public void setCustomText(String customText) {
		this.customText = customText;
	}

	public QRect customRect() {
		return customRect;
	}

	public void setCustomRect(QRect customRect) {
		this.customRect = customRect;
	}

	public QKeySequence customKeySequence() {
		return customKeySequence;
	}

	public void setCustomKeySequence(QKeySequence customKeySequence) {
		this.customKeySequence = customKeySequence;
	}

	public Qt.ArrowType customArrowType() {
		return customArrowType;
	}

	public void setCustomArrowType(Qt.ArrowType customArrowType) {
		this.customArrowType = customArrowType;
	}
}
