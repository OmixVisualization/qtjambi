import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class polygon
{
    public static void main(String args[])
    {
        if (true)
        {
            // STREAM
    //! [0]
            QPolygon polygon = new QPolygon();
            polygon.add(new QPoint(10, 20));
            polygon.add(new QPoint(20, 30));
    //! [0]
        }

        if (true)
        {
            // STREAMF
    //! [1]
            QPolygonF polygon = new QPolygonF();
            polygon.add(new QPointF(10.4, 20.5));
            polygon.add(new QPointF(20.2, 30.2));
    //! [1]
        }

        if (true)
        {
            // SETPOINTS
    //! [2]
            //int points[] = { 10, 20, 30, 40 };
            //QPolygon polygon = new QPolygon();
            //polygon.setPoints(2, points);
    //! [2]
        }

        if (true)
        {
            // SETPOINTS2
    //! [3]
            //QPolygon polygon = new QPolygon();
            //polygon.setPoints(2, 10, 20, 30, 40);
    //! [3]
        }

        if (true)
        {
            // PUTPOINTS
    //! [4]
            //QPolygon polygon = new QPolygon(1);
            ///polygon.add(new QPoint(4, 5));
            //polygon.putPoints(1, 2, 6,7, 8,9);
    //! [4]
        }

        if (true)
        {
            // PUTPOINTS2
    //! [5]
            //QPolygon polygon = new QPolygon(3);
            //polygon.putPoints(0, 3, 4,5, 0,0, 8,9);
            //polygon.putPoints(1, 1, 6,7);
    //! [5]
        }

        if (true)
        {
            // PUTPOINTS3
    //! [6]
            // QPolygon polygon1 = new QPolygon();
            // polygon1.putPoints(0, 3, 1,2, 0,0, 5,6);
            // polygon1 is now the three-point polygon(1,2, 0,0, 5,6);

            // QPolygon polygon2 = new QPolygon();
            // polygon2.putPoints(0, 3, 4,4, 5,5, 6,6);
            // polygon2 is now (4,4, 5,5, 6,6);

            // polygon1.putPoints(2, 3, polygon2);
            // polygon1 is now the five-point polygon(1,2, 0,0, 4,4, 5,5, 6,6);
    //! [6]
        }
    }
}
