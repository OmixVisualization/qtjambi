/*   Ported from: src.corelib.io.qabstractfileengine.cpp
*/
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class src_corelib_io_qabstractfileengine
{
    static class MainWindow extends QMainWindow {
        public MainWindow(QWidget parent) {
            super(parent);
        }
    }

    static class ZipEngine extends QAbstractFileEngine
    {
        public ZipEngine(String fn) {}
    }



//! [0]
    public static class ZipEngineHandler extends QAbstractFileEngineHandler
    {
        public QAbstractFileEngine create(String fileName)
        {
            // ZipEngineHandler returns a ZipEngine for all .zip files
            if (fileName.toLowerCase().endsWith(".zip")) {
                return new ZipEngine(fileName);
            } else {
                return null;
            }
        }
    }

    // ...

    public static void main(String args[])
    {
        QApplication.initialize(args);

        ZipEngineHandler engine = new ZipEngineHandler();

        MainWindow window = new MainWindow(null);
        window.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [0]



//! [1]
    public QAbstractFileEngine create(String fileName)
    {
        // ZipEngineHandler returns a ZipEngine for all .zip files
        if (fileName.toLowerCase().endsWith(".zip")) {
            return new ZipEngine(fileName);
        } else {
            return null;
        }
    }
//! [1]



    class CustomFileEngineIterator extends QAbstractFileEngineIterator
    {
        public CustomFileEngineIterator(QDir.Filters filters, java.util.List<String> filterNames) {
            super(filters, filterNames);
        }
        public String next() { return ""; }
        public boolean  hasNext() { return true; }
        public String   currentFileName() { return ""; }
    }


//! [2]
    class CustomFileEngine extends QAbstractFileEngine
    {
        QAbstractFileEngineIterator beginEntryList(QDir.Filters filters, java.util.List<String> nameFilters)
        {
            return new CustomFileEngineIterator(filters, nameFilters);
        }
        //...
    }
//! [2]


//! [3]
    class CustomIterator extends QAbstractFileEngineIterator
    {
        private java.util.List<String> entries;
        private int index;

        public CustomIterator(java.util.List<String> nameFilters, QDir.Filters filters)
        {
            super(filters, nameFilters);
            index = 0;

            // In a real iterator, these entries are fetched from the
            // file system based on the value of path().
            entries.add("entry1");
            entries.add("entry2");
            entries.add("entry3");
        }

        public boolean hasNext()
        {
            return index < entries.size() - 1;
        }

        public String next()
        {
           if (!hasNext())
               return "";
           ++index;
           return currentFilePath();
        }

        public String currentFileName()
        {
            return entries.get(index);
        }
    }
//! [3]

}

