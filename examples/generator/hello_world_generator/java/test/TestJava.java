package test;

import org.qtjambi.test.TestItem;
import org.qtjambi.test.TestView;
import org.qtjambi.qt.gui.QApplication;
import org.qtjambi.qt.gui.QGraphicsScene;
import org.qtjambi.qt.gui.QGraphicsView;

/**
 * @author Helge Fredriksen
 * @version 1.0 Dec 9, 2009 3:12:08 PM
 */
public class TestJava {
    private static void setupAndRunEventLoop() {
        QGraphicsScene scene = new QGraphicsScene();

        TestView view = new TestView(scene);
        TestItem item = view.getItem();
        int id = item.getId();
        System.out.println(id);

        view.show();

        QApplication.execStatic();
    }

    public static void main(String[] args) {
        QApplication.initialize(args);

        setupAndRunEventLoop();

        QApplication.shutdown();
    }
}
