import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class object extends QObject
{

public object(QObject parent)
{
    super(parent);
}

public void print()
{
    int numberOfPages = 10;
    int lastPage = numberOfPages - 1;

//! [0]
    QPrinter printer = new QPrinter(QPrinter.PrinterMode.HighResolution);
    printer.setOutputFileName("print.ps");
    QPainter painter = new QPainter();
    painter.begin(printer);

    for (int page = 0; page < numberOfPages; ++page) {

        // Use the painter to draw on the page.

        if (page != lastPage)
            printer.newPage();
    }

    painter.end();
//! [0]
    QApplication.quit();
}

}
