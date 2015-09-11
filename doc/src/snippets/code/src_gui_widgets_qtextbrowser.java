import org.qtjambi.qt.gui.*;


public class src_gui_widgets_qtextbrowser {
    public static void main(String args[]) {
        QApplication.initialize(args);

        QWidget widget = new QWidget();
        QAction backaction = new QAction(widget);
        QAction forwardaction = new QAction(widget);

        QTextBrowser browser = new QTextBrowser();

//! [0]
    backaction.setToolTip(browser.historyTitle(-1));
    forwardaction.setToolTip(browser.historyTitle(+1));
//! [0]


    }
}
