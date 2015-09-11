import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;
import java.lang.Math;
import java.util.*;


public class src_gui_text_qtextlayout {
    public static void main(String args[]) {
        QApplication.initialize(args);


QFontMetrics fontMetrics=new QFontMetrics(new QFont());
QTextLayout textLayout=new QTextLayout();
int lineWidth=3;

//! [0]
        int leading = fontMetrics.leading();
        double height = 0;
        double widthUsed = 0;
        textLayout.beginLayout();
        while (1==1) {
            QTextLine line = textLayout.createLine();
            if (!line.isValid())
                break;

            line.setLineWidth(lineWidth);
            height += leading;
            line.setPosition(new QPointF(0.0, height));
            height += line.height();
            widthUsed = Math.max(widthUsed, line.naturalTextWidth());
        }
        textLayout.endLayout();
//! [0]


//! [1]
        QPainter painter = new QPainter();
        textLayout.draw(painter,new QPointF(), new ArrayList<QTextLayout_FormatRange>());
//! [1]


    }
}
