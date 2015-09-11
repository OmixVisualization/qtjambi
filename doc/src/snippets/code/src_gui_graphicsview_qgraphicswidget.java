import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_graphicsview_qgraphicswidget extends QGraphicsWidget {


    public int lineWidth() { return 0; }

//! [0]
        @Override
        protected void initStyleOption(QStyleOption option) {
            super.initStyleOption(option);
            if (option instanceof QStyleOptionGroupBox) {
                QStyleOptionGroupBox box = (QStyleOptionGroupBox)option;
                // Add group box specific state.
                box.setLineWidth(lineWidth());
                // ...
            }
        }
//! [0]


        QGraphicsWidget a, b, c, d;

        {
//! [1]
        setTabOrder(a, b); // a to b
        setTabOrder(b, c); // a to b to c
        setTabOrder(c, d); // a to b to c to d
//! [1]


//! [2]
        // WRONG
        setTabOrder(c, d); // c to d
        setTabOrder(a, b); // a to b AND c to d
        setTabOrder(b, c); // a to b to c, but not c to d
//! [2]
        }

    }

