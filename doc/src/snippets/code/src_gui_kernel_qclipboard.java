import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_kernel_qclipboard {
    static void main(String args[]) {
        QApplication.initialize(args);

        //! [0]
        QClipboard clipboard = QApplication.clipboard();
        String originalText = clipboard.text();
        String newText = "new words";
        clipboard.setText(newText);
        //! [0]

        QImage image = new QImage();

        //! [1]
        QMimeData data = new QMimeData();
        data.setImageData(image);
        clipboard.setMimeData(data, QClipboard.Mode.Clipboard);
        //! [1]
    }
}
