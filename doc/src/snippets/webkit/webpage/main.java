
import org.qtjambi.qt.webkit.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.*;

public class main
{

/*
//! [0]
class Thumbnailer extends QObject
{
    public Signal0 finished = new Signal0();

    public Thumbnailer(QUrl url) { ... }

    private void render(Boolean b) { ... }

    private QWebPage page;

}
//! [0]
*/


    public static void main(String args[])
    {
        QApplication.initialize(args);

        Thumbnailer thumbnail =
            new Thumbnailer(new QUrl("http://www.trolltech.com"));

        thumbnail.finished.connect(QApplication.instance(), "quit()");

        QApplication.execStatic();
        QApplication.shutdown();
    }

    private static class Thumbnailer extends QObject
    {
    private QWebPage page = new QWebPage();
    public Signal0 finished = new Signal0();
//! [1]
    public Thumbnailer(QUrl url)
    {
        page.mainFrame().load(url);
        page.loadFinished.connect(this, "render(Boolean)");
    }
//! [1]

//! [2]
    void render(Boolean b)
    {
        page.setViewportSize(page.mainFrame().contentsSize());
        QImage image = new QImage(page.viewportSize(), QImage.Format.Format_ARGB32);
        QPainter painter = new QPainter(image);

        page.mainFrame().render(painter);
        painter.end();

        QImage thumbnail = image.scaled(400, 400);
        thumbnail.save("thumbnail.png");

        finished.emit();
    }
//! [2]
    }
}
