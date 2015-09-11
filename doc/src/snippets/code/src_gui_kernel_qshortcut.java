import org.qtjambi.qt.core.Qt;
import org.qtjambi.qt.gui.*;


public class src_gui_kernel_qshortcut extends QWidget{
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

    public src_gui_kernel_qshortcut(QWidget parent) {
        QShortcut shortcut;
//! [0]
        shortcut = new QShortcut(new QKeySequence(tr("Ctrl+O", "File|Open")),
                                 parent);
//! [0]


//! [1]
        shortcut.setKey(new QKeySequence(0));                  // no signal emitted
        shortcut.setKey(new QKeySequence());                   // no signal emitted
        shortcut.setKey(new QKeySequence(0x3b1));              // Greek letter alpha
        shortcut.setKey(new QKeySequence(Qt.Key.Key_D.value()));       // 'd', e.g. to delete
        shortcut.setKey(new QKeySequence('q'));                // 'q', e.g. to quit
        shortcut.setKey(new QKeySequence(Qt.KeyboardModifier.AltModifier.value() + Qt.Key.Key_P.value()));       // Ctrl+P, e.g. to print document
        shortcut.setKey(new QKeySequence("Ctrl+P"));           // Ctrl+P, e.g. to print document
//! [1]


    }
}
