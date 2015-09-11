import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{

//! [0]

public static void main(String args[])
{
    QDir dir = new QDir();
    dir.setFilter(new QDir.Filters(QDir.Filter.Files, QDir.Filter.Hidden,
                                   QDir.Filter.NoSymLinks));
    dir.setSorting(new QDir.SortFlags(QDir.SortFlag.Size,
                                      QDir.SortFlag.Reversed));

    List<QFileInfo> list = dir.entryInfoList();
    for (QFileInfo fileInfo : list) {
        System.out.println(String.valueOf(fileInfo.size()) + " " +
                           fileInfo.fileName() + "\n");
    }
}
//! [0]

}
