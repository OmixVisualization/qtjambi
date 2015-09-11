import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class main
{

    static class Widget extends QWidget
    {

    public Widget(QWidget parent)
    {
        super(parent);
//! [0]
        QWidget firstPageWidget = new QWidget();
        QWidget secondPageWidget = new QWidget();
        QWidget thirdPageWidget = new QWidget();

        QStackedLayout stackedLayout = new QStackedLayout();
        stackedLayout.addWidget(firstPageWidget);
        stackedLayout.addWidget(secondPageWidget);
        stackedLayout.addWidget(thirdPageWidget);

//! [0] //! [1]
        QComboBox pageComboBox = new QComboBox();
        pageComboBox.addItem(tr("Page 1"));
        pageComboBox.addItem(tr("Page 2"));
        pageComboBox.addItem(tr("Page 3"));
        pageComboBox.activated.connect(stackedLayout, "setCurrentIndex(int)");
//! [1]

//! [2]
        QVBoxLayout mainLayout = new QVBoxLayout();
//! [2]
        mainLayout.addWidget(pageComboBox);
//! [3]
        mainLayout.addLayout(stackedLayout);
        setLayout(mainLayout);
//! [3]
    }

    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new Widget(null).show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}

