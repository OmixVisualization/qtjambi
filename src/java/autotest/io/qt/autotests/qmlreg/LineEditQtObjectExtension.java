package io.qt.autotests.qmlreg;

import io.qt.QtAsGadget;
import io.qt.QtObject;
import io.qt.QtPropertyReader;
import io.qt.QtPropertyWriter;
import io.qt.core.QMargins;
import io.qt.core.QObject;
import io.qt.widgets.QLineEdit;

@QtAsGadget
public class LineEditQtObjectExtension extends QtObject
{
	public LineEditQtObjectExtension(QObject parent) {
		m_lineedit = (QLineEdit)parent;
	}

	@QtPropertyReader
    int leftMargin() {
        return m_lineedit.textMargins().left();
    }
	@QtPropertyWriter
    void setLeftMargin(int l){
        QMargins m = m_lineedit.textMargins();
        m.setLeft(l);
        m_lineedit.setTextMargins(m);
    }

    int rightMargin(){
        return m_lineedit.textMargins().right();
    }

    void setRightMargin(int r)
    {
        QMargins m = m_lineedit.textMargins();
        m.setRight(r);
        m_lineedit.setTextMargins(m);
    }

    int topMargin(){
        return m_lineedit.textMargins().top();
    }
    
    void setTopMargin(int t)
    {
        QMargins m = m_lineedit.textMargins();
        m.setTop(t);
        m_lineedit.setTextMargins(m);
    }

    int bottomMargin(){
        return m_lineedit.textMargins().bottom();
    }
    
    void setBottomMargin(int b)
    {
        QMargins m = m_lineedit.textMargins();
        m.setBottom(b);
        m_lineedit.setTextMargins(m);
    }

    private QLineEdit m_lineedit;
};