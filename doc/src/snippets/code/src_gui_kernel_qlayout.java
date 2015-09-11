import org.qtjambi.qt.gui.*;


public class src_gui_kernel_qlayout extends QWidget{
    public static void main(String args[]) {
        QApplication.initialize(args);
    }
//! [0]
        static void paintLayout(QPainter painter, QLayoutItemInterface item)
        {
            QLayout layout = item.layout();
            if (layout!=null) {
                for (int i = 0; i < layout.count(); ++i)
                    paintLayout(painter, layout.itemAt(i));
            }
            painter.drawRect(layout.geometry());
        }

        protected void paintEvent(QPaintEvent event)
        {
            QPainter painter = new QPainter(this);
            if (layout() != null)
                paintLayout(painter, layout());
        }
//! [0]

        void test() {
            QLayout layout = new QGridLayout(this);
//! [1]
        QLayoutItemInterface child = null;
        while ((child = layout.takeAt(0)) != null) {
            //...
            //delete child;
        }
//! [1]


    }
}
