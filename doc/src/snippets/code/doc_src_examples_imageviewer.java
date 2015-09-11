import org.qtjambi.qt.gui.*;


public class doc_src_examples_imageviewer {
    public static void main(String args[]) {
        QApplication.initialize(args);

        QLabel imageLabel = new QLabel();
//! [0]
        imageLabel.resize(imageLabel.pixmap().size());
//! [0]


//! [1]
        if (imageLabel.pixmap().isNull())
             throw new RuntimeException("Missing pixmap in file ...");
//! [1]


        QScrollBar scrollBar = new QScrollBar();
        double factor = 0;
//! [4]
        scrollBar.setValue((int)(factor * scrollBar.value()));
//! [4]


    }
}
