import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main extends QWidget
{
    public static void main(String args[])
    {
        QApplication.initialize(args);

        Widget widget = new Widget();
        widget.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}

class Widget extends QWidget
{
    public Widget()
    {
    //! [0]
        QStringListModel model = new QStringListModel();
        List<String> list = new Vector<String>();
        list.add("a");
        list.add("b");
        list.add("c");
        model.setStringList(list);
    //! [0]
    }
}
