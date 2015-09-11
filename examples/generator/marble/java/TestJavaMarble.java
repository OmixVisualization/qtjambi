import org.qtjambi.marble.MarbleWidget;
import org.qtjambi.qt.gui.QApplication;

/**
 * @author Helge Fredriksen
 * @version 1.0 Jan 8, 2010 10:18:11 PM
 */
public class TestJavaMarble extends MarbleWidget {


    public static void main(String[] args) {

        QApplication.initialize(args);

        TestJavaMarble javaMarble = new TestJavaMarble();
//        javaMarble.setMapThemeId("earth/openstreetmap/openstreetmap.dgml");
        javaMarble.setMapThemeId("earth/srtm/srtm.dgml");
        javaMarble.show();
//        javaMarble.setMapThemeId("earth/bluemarble/bluemarble.dgml");

        javaMarble.showGrid();
        QApplication.execStatic();
        QApplication.shutdown();

    }


}
