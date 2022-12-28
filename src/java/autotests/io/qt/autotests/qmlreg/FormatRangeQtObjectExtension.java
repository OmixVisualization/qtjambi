package io.qt.autotests.qmlreg;

import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.gui.QTextCharFormat;
import io.qt.gui.QTextLayout;

public class FormatRangeQtObjectExtension extends QTextLayout.FormatRange
{
    FormatRangeQtObjectExtension(QPrivateConstructor p) {
		super(p);
	}

    @QtPropertyWriter(name="format")
	public final void _setFormat(QTextCharFormat format) {
		setFormat(format);
	}

    @QtPropertyReader(name="format")
	public final QTextCharFormat _format() {
		return format();
	}

    @QtPropertyWriter(name="length")
	public final void _setLength(int length) {
		setLength(length);
	}

    @QtPropertyReader(name="length")
	public final int _length() {
		return length();
	}

    @QtPropertyWriter(name="start")
	public final void _setStart(int start) {
		setStart(start);
	}

    @QtPropertyReader(name="start")
	public final int _start() {
		return start();
	}
};