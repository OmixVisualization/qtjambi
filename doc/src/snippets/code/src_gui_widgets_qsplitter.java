import org.qtjambi.qt.gui.*;


public class src_gui_widgets_qsplitter {
    public static void main(String args[]) {
        QApplication.initialize(args);
        QSplitter splitter = null;
        int index = 0;
        byte stretch = 0;
//! [0]
        QWidget widget = splitter.widget(index);
        QSizePolicy policy = widget.sizePolicy();
        policy.setHorizontalStretch(stretch);
        policy.setVerticalStretch(stretch);
        widget.setSizePolicy(policy);
//! [0]


    }
}
