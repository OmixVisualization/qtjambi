import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_dialogs_qfiledialog {
    public static void main(String args[]) {
        QApplication.initialize(args);

        //FileDialog dialog = new FileDialog();

       }
       class FileDialog extends QWidget{
       public void myProcessing(Object string) {
       ;
       }
       public void snip() {
//! [0]
  String fileName = QFileDialog.getOpenFileName(this,
      tr("Open Image"), "/home/jana", new QFileDialog.Filter(tr("Image Files (*.png *.jpg *.bmp)")));
//! [0]


//! [1]
  String filter = "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)";
//! [1]


//! [2]
  QFileDialog dialog = new QFileDialog(this);
  dialog.setFileMode(QFileDialog.FileMode.AnyFile);
//! [2]


//! [3]
  dialog.setFilter(tr("Images (*.png *.xpm *.jpg)"));
//! [3]


//! [4]
  dialog.setViewMode(QFileDialog.ViewMode.Detail);
//! [4]


//! [5]
  java.util.List fileNames;
  if (dialog.exec() != 0)
      fileNames = dialog.selectedFiles();
//! [5]


//! [6]
       dialog.setFilter("All C++ files (*.cpp *.cc *.C *.cxx *.c++)");
       dialog.setFilter("*.cpp;*.cc;*.C;*.cxx;*.c++");
//! [6]


//! [7]
      java.util.ArrayList<String> filters = new java.util.ArrayList<String>();
      filters.add("Image files (*.png *.xpm *.jpg)");
      filters.add("Text files (*.txt)");
      filters.add("Any files (*)");

      QFileDialog dlg = new QFileDialog(this);
      dlg.setFilters(filters);
      dlg.exec();
//! [7]


//! [8]
    String fName = QFileDialog.getOpenFileName(this, tr("Open File"),
                                                    "/home",
                                                    new QFileDialog.Filter(tr("Images (*.png *.xpm *.jpg)")));
//! [8]


//! [9]
    java.util.List<String> files = QFileDialog.getOpenFileNames(
                            this,
                            "Select one or more files to open",
                            "/home",
                            new QFileDialog.Filter("Images (*.png *.xpm *.jpg)"));
//! [9]


//! [10]
    java.util.List<String> list = files;
    java.util.ListIterator it = list.listIterator();
    while(it.hasNext()) {
        myProcessing(it.next());
    }
//! [10]


//! [11]
    String saveName = QFileDialog.getSaveFileName(this, tr("Save File"),
                               "/home/jana/untitled.png",
                               new QFileDialog.Filter(tr("Images (*.png *.xpm *.jpg)")));
//! [11]


//! [12]
    String dir = QFileDialog.getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog.Option.createQFlags(
                                                    QFileDialog.Option.ShowDirsOnly,
                                                    QFileDialog.Option.DontResolveSymlinks));
//! [12]

       }
   }
}

