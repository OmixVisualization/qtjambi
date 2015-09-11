import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_svg_qgraphicssvgitem {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
    QSvgRenderer renderer = new QSvgRenderer("SvgCardDeck.svg");
    QGraphicsSvgItem black = new QGraphicsSvgItem();
    QGraphicsSvgItem red   = new QGraphicsSvgItem();

    black.setSharedRenderer(renderer);
    black.setElementId("black_joker");

    red.setSharedRenderer(renderer);
    red.setElementId("red_joker");
//! [0]


    }
}
