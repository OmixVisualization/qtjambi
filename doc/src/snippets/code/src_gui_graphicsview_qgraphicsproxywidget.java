import org.qtjambi.qt.gui.*;


public class src_gui_graphicsview_qgraphicsproxywidget {
//! [0]
    public static void main(String args[]) {
        QApplication.initialize(args);
        QTabWidget tabWidget = new QTabWidget();

        QGraphicsScene scene = new QGraphicsScene();
        QGraphicsProxyWidget proxy = scene.addWidget(tabWidget);

        QGraphicsView view = new QGraphicsView(scene);
        view.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
//! [0]


    public void foo() {
//! [1]
        QGraphicsScene scene = new QGraphicsScene();

        QLineEdit edit = new QLineEdit();
        QGraphicsProxyWidget proxy = scene.addWidget(edit);

        edit.isVisible();  // returns false, as QWidget is hidden by default
        proxy.isVisible(); // also returns false

        edit.show();

        edit.isVisible(); // returns true
        proxy.isVisible(); // returns true
//! [1]
    }


}
