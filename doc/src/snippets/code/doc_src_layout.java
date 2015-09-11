
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

import java.util.*;

public class doc_src_layout
{

/*
//! [0]

        static class CardLayout extends QLayout
        {
            public CardLayout(QWidget parent, int dist)
            {
                super(parent, 0, dist);
            }
            
            public CardLayout(QLayout parent, int dist)
            {
                super(parent, dist);
            }
            
            public CardLayout(int dist)
            {
                super(dist);
            }

            public void addItem(QLayoutItem item) { ... }
            public QSize sizeHint()  { ...}
            public QSize minimumSize() { ... }
            public QLayoutItem itemAt(int) { ... }
            public QLayoutItem takeAt(int) { ... }
            public void setGeometry(QRect rect) { ... }

        
            private List<QLayoutItem> list;
        }
//! [0]
*/

//! [1]
    // Not relevant for Jambi
//! [1]

        private static abstract class CardLayout extends QLayout
        {
            /*public CardLayout(QWidget parent, int dist)
            {
                super(parent, 0, dist);
            }
            
            public CardLayout(QLayout parent, int dist)
            {
                super(parent, dist);
            }
            
            public CardLayout(int dist)
            {
                super(dist);
            }*/
//! [2]
            public QLayoutItem itemAt(int idx)
            {
                return list.get(idx);
            }

            public QLayoutItem takeAt(int idx)
            {
                return idx >= 0 && idx < list.size() ? list.remove(idx) : null;
            }
//! [2]


//! [3]
            public void addItem(QLayoutItem item)
            {
                list.add(item);
            }
//! [3]


//! [4]
        /* Destructor for c++ (Not relevant for Jambi)
        CardLayout::~CardLayout()
        {
            deleteAllItems();
        }*/
//! [4]


//! [5]
            public void setGeometry(QRect r)
            {
                super.setGeometry(r);

                if (list.size() == 0)
                    return;

                int w = r.width() - (list.size() - 1) * widgetSpacing();
                int h = r.height() - (list.size() - 1) * widgetSpacing();
                int i = 0;
                while (i < list.size()) {
                    QLayoutItem o = list.get(i);
                    QRect geom = new QRect(r.x() + i * widgetSpacing(),
                                           r.y() + i * widgetSpacing(), w, h);
                    o.setGeometry(geom);
                    ++i;
                }
            }
//! [5]

//! [6]
            public QSize sizeHint()
            {
                QSize s  = new QSize(0,0);
                int n = list.size();
                if (n > 0)
                    s = new QSize(100, 70); //start with a nice default size
                
                int i = 0;
                while (i < n) {
                    QLayoutItem o = list.get(i);
                    s = s.expandedTo(o.sizeHint());
                    ++i;
                }
                return s.add(new QSize(widgetSpacing() * n, widgetSpacing() * n));
            }

            public QSize minimumSize()
            {
                QSize s = new QSize(0, 0);
                int n = list.size();
                int i = 0;
                while (i < n) {
                    QLayoutItem o = list.get(i);
                    s = s.expandedTo(o.minimumSize());
                    ++i;
                }
                return s.add(new QSize(widgetSpacing() * n, widgetSpacing() * n));
            }
//! [6]
            private List<QLayoutItem> list = new Vector<QLayoutItem>();
    }
}
