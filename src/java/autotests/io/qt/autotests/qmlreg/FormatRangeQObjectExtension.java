package io.qt.autotests.qmlreg;

import io.qt.core.QObject;
import io.qt.gui.QTextCharFormat;
import io.qt.gui.QTextLayout;

public class FormatRangeQObjectExtension extends QObject
{
    FormatRangeQObjectExtension(QPrivateConstructor p) {
		super(p);
	}

    private QTextLayout.FormatRange value;

	public final void setFormat(QTextCharFormat format) {
		value.setFormat(format);
	}

	public final QTextCharFormat format() {
		return value.format();
	}

	public final void setLength(int length) {
		value.setLength(length);
	}

	public final int length() {
		return value.length();
	}

	public final void setStart(int start) {
		value.setStart(start);
	}

	public final int start() {
		return value.start();
	}
};