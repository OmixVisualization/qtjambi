import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class main
{
    public static void main(String args[])
    {
        QApplication.initialize(args);

    //! [0]
        QGraphicsEllipseItem ball = new QGraphicsEllipseItem(0, 0, 20, 20);

        QTimeLine timer = new QTimeLine(5000);
        timer.setFrameRange(0, 100);

        QGraphicsItemAnimation animation = new QGraphicsItemAnimation();
        animation.setItem(ball);
        animation.setTimeLine(timer);

        for (int i = 0; i < 200; ++i)
            animation.setPosAt(i / 200.0, new QPointF(i, i));

        QGraphicsScene scene = new QGraphicsScene();
        scene.setSceneRect(new QRectF(0, 0, 250, 250));
        scene.addItem(ball);

        QGraphicsView view = new QGraphicsView(scene);
        view.show();

        timer.start();
    //! [0]

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
