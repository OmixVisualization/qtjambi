import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class main
{
    static class Widget extends QWidget
    {

    public Widget()
    {
//! [0]
        QWidget firstPageWidget = new QWidget();
        QWidget secondPageWidget = new QWidget();
        QWidget thirdPageWidget = new QWidget();

        QStackedWidget stackedWidget = new QStackedWidget();
        stackedWidget.addWidget(firstPageWidget);
        stackedWidget.addWidget(secondPageWidget);
        stackedWidget.addWidget(thirdPageWidget);

//! [0] //! [1]
        QComboBox pageComboBox = new QComboBox();
        pageComboBox.addItem(tr("Page 1"));
        pageComboBox.addItem(tr("Page 2"));
        pageComboBox.addItem(tr("Page 3"));
        pageComboBox.activated.connect(stackedWidget, "setCurrentIndex(int)");
//! [1] //! [2]
        QVBoxLayout layout = new QVBoxLayout();
//! [2]
        layout.addWidget(pageComboBox);
//! [3]
        layout.addWidget(stackedWidget);
        setLayout(layout);
//! [3]
    }
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new Widget().show();

        QApplication.execStatic();
        QApplication.shutdown();
    }

}
