import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class mainwindowsnippet extends QMainWindow
{

    public mainwindowsnippet()
    {
        createMenus();
        createToolBars();
        createDockWidgets();
        //setMenuWidget(new QPushButton("Hello"));
    }

    void createMenus()
    {
        //setMenuWidget(new QPushButton("Hello"));
        QMenu menu = new QMenu("File");
        menu.addAction("Save As");

        QMenuBar bar = new QMenuBar();
        bar.addMenu(menu);

        setMenuWidget(new QWidget());
    }

    void createToolBars()
    {
        setToolButtonStyle(Qt.ToolButtonStyle.ToolButtonTextOnly);
        QToolBar t1 = new QToolBar();
        t1.addAction(new QAction("t1", this));

        QToolBar t2 = new QToolBar();
        t2.addAction(new QAction("t2", this));

        addToolBar(Qt.ToolBarArea.LeftToolBarArea, t1);
        addToolBar(Qt.ToolBarArea.LeftToolBarArea, t2);
    }

    void createDockWidgets()
    {
        QWidget dockWidgetContents = new QWidget();
        QVBoxLayout layout = new QVBoxLayout();
        layout.addWidget(new QPushButton("My Button."));
        dockWidgetContents.setLayout(layout);

    //! [0]
        QDockWidget dockWidget = new QDockWidget(tr("Dock Widget"), this);
        dockWidget.setAllowedAreas(Qt.DockWidgetArea.LeftDockWidgetArea);
        dockWidget.setWidget(dockWidgetContents);
        addDockWidget(Qt.DockWidgetArea.LeftDockWidgetArea, dockWidget);
    //! [0]
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        mainwindowsnippet snipper = new mainwindowsnippet();
        snipper.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
