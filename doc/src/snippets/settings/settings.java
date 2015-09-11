import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;


public class settings
{

void snippet_ctor1()
{
//! [0]
    QSettings settings = new QSettings("MySoft", "Star Runner");
//! [0]
}

void snippet_ctor2()
{
    QWidget panel = null;

//! [1]
    QCoreApplication.setOrganizationName("MySoft");
//! [1] //! [2]
    QCoreApplication.setOrganizationDomain("mysoft.com");
//! [2] //! [3]
    QCoreApplication.setApplicationName("Star Runner");
//! [3]

//! [4]
    QSettings settings = new QSettings();
//! [4]

//! [5]
    settings.setValue("editor/wrapMargin", 68);
//! [5]
    if (true)
    {
////! [6]
    int margin = (Integer) settings.value("editor/wrapMargin");
//! [6]
    }

    if (true)
    {
//! [7]
    int margin = (Integer) settings.value("editor/wrapMargin", 80);
//! [7]
    }

    QWidget win = null;

//! [8]
    settings.setValue("mainwindow/size", win.size());
//! [8] //! [9]
    settings.setValue("mainwindow/fullScreen", win.isFullScreen());
//! [9] //! [10]
    settings.setValue("outputpanel/visible", panel.isVisible());
//! [10]

//! [11]
    settings.beginGroup("mainwindow");
    settings.setValue("size", win.size());
    settings.setValue("fullScreen", win.isFullScreen());
    settings.endGroup();
//! [11]


//! [12]
    settings.beginGroup("outputpanel");
    settings.setValue("visible", panel.isVisible());
    settings.endGroup();
//! [12]
}

void snippet_locations()
{
//! [13]
    QSettings obj1 = new QSettings("MySoft", "Star Runner");
//! [13] //! [14]
    QSettings obj2 = new QSettings("MySoft");
    QSettings obj3 = new QSettings(QSettings.Scope.SystemScope, "MySoft", "Star Runner");
    QSettings obj4 = new QSettings(QSettings.Scope.SystemScope, "MySoft");
//! [14]

    if (true)
    {
//! [15]
    QSettings settings = new QSettings(QSettings.Format.IniFormat,
                                       QSettings.Scope.UserScope,
                                       "MySoft", "Star Runner");
//! [15]
    }

    if (true)
    {
    QSettings settings = new QSettings("starrunner.ini", QSettings.Format.IniFormat);
    }

    {
    QSettings settings = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft",
                       QSettings.Format.NativeFormat);
    }
}

class MainWindow extends QMainWindow
{

//! [16]
public void writeSettings()
{
    QSettings settings = new QSettings("Moose Soft", "Clipper");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.endGroup();
}
//! [16]

//! [17]
public void readSettings()
{
    QSettings settings = new QSettings("Moose Soft", "Clipper");

    settings.beginGroup("MainWindow");
    resize((QSize) settings.value("size", new QSize(400, 400)));
    move((QPoint) settings.value("pos", new QPoint(200, 200)));
    settings.endGroup();
}
//! [17]

//! [18]
public MainWindow()
{
//! [18] //! [19]
    readSettings();
//! [19] //! [20]
}
//! [20]

boolean letsCutItOut() { return true; }

//! [21]
protected void closeEvent(QCloseEvent event)
{
    if (letsCutItOut()) {
        writeSettings();
        event.accept();
    } else {
        event.ignore();
    }
}
//! [21]

}

}
