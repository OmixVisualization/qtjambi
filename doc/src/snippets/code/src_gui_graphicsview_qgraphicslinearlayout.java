import org.qtjambi.qt.gui.*;

public class src_gui_graphicsview_qgraphicslinearlayout {
    public static void main(String args[]) {
        QApplication.initialize(args);

//! [0]
        QGraphicsScene scene = new QGraphicsScene();
        QGraphicsWidget textEdit = scene.addWidget(new QTextEdit());
        QGraphicsWidget pushButton = scene.addWidget(new QPushButton());

        QGraphicsLinearLayout layout = new QGraphicsLinearLayout();
        layout.addItem(textEdit);
        layout.addItem(pushButton);

        QGraphicsWidget form = new QGraphicsWidget();
        form.setLayout(layout);
        scene.addItem(form);
//! [0]


    }
}
