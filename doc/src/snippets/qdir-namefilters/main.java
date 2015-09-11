import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class main
{

public static void main(String args[])
{
    QDir dir = new QDir();
    dir.setFilter(new QDir.Filters(QDir.Filter.Files, QDir.Filter.Hidden,
                                   QDir.Filter.NoSymLinks));
    dir.setSorting(new QDir.SortFlags(QDir.SortFlag.Size, QDir.SortFlag.Reversed));

//! [0]
    List<String> filters = new Vector<String>();
    filters.add("*.cpp");
    filters.add("*.cxx");
    filters.add("*.cc");
    dir.setNameFilters(filters);
//! [0]

    List<QFileInfo> list = dir.entryInfoList();
    System.out.println("     Bytes Filename");
    for (QFileInfo fileInfo : list) {
        System.out.println(String.valueOf(fileInfo.size()) + " " +
                           fileInfo.fileName());
    }
}

}
