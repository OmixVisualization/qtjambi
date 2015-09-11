/*   Ported from: src.gui.styles.qstyleoption.cpp

*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_styles_qstyleoption {
    public static void main(String args[]) {
        QApplication.initialize(args);

        /* -- this snippet is not relevant for jambi

//! [0]
        int MyStyle.styleHint(StyleHint stylehint, QStyleOption pt,
                               QWidget idget, QStyleHintReturn* returnData);
        {
            if (stylehint == SH_RubberBand_Mask) {
                QStyleHintReturnMask askReturn =
                        qstyleoption_cast<QStyleHintReturnMask *>(hint);
                if (maskReturn) {
                    ...
                }
            }
            ...
        }
//! [0]
*/
    }
}
