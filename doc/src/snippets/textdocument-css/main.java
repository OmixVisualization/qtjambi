import org.qtjambi.qt.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.core.*;

public class main
{

public static void main(String args[])
{
    QApplication.initialize(args);

//! [0]
    QTextBrowser browser = new QTextBrowser();
    QColor linkColor = QColor.red;
    String sheet = "a { text-decoration: underline; color: " + linkColor.name();
    browser.document().setDefaultStyleSheet(sheet);
//! [0]
    browser.setSource(new QUrl("../../../html/index.html"));
    browser.resize(800, 600);
    browser.show();

    QApplication.execStatic();
    QApplication.shutdown();
}

}
