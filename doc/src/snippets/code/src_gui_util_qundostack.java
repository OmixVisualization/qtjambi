import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_gui_util_qundostack {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }

//! [0]
    public class AppendText extends QUndoCommand {

        private String m_document;
        private String m_text;

        public AppendText(String doc, String text) {
            m_document = doc;
            m_text= text;
            setText("append text");
        }

        public void undo() {
            m_document = m_document.substring(0,
                    m_document.length() -m_text.length());
        }

        public void redo() {
            m_document += m_text;
        }
    }
//! [0]

    public class MyCommand extends QUndoCommand {}

    private void snippetWrapper1() {
        QUndoStack stack = new QUndoStack();
//! [1]
        MyCommand command1 = new MyCommand();
        stack.push(command1);
        MyCommand command2 = new MyCommand();
        stack.push(command2);

        stack.undo();

        MyCommand command3 = new MyCommand();
        stack.push(command3); // command2 gets deleted
//! [1]
    }

    public class InsertText extends QUndoCommand {
        public InsertText(String a, int b, String c, QUndoCommand d) {}
        public InsertText(String a, int b, String c) {}
    }
    public class SetColor extends QUndoCommand {
        public SetColor(String a, int b, int c, Qt.GlobalColor d, QUndoCommand e) {}
        public SetColor(String a, int b, int c, Qt.GlobalColor d) {}
    }

    private void snippetWrapper2() {
        String document = new String();
        String text = new String();
        int idx = 666;
        QUndoStack stack = new QUndoStack();
//! [2]
        QUndoCommand insertRed = new QUndoCommand(); // an empty command
        insertRed.setText("insert red text");

        new InsertText(document, idx, text, insertRed); // becomes child of insertRed
        new SetColor(document, idx, text.length(), Qt.GlobalColor.red, insertRed);

        stack.push(insertRed);
//! [2]

//! [4]
        stack.beginMacro("insert red text");
        stack.push(new InsertText(document, idx, text));
        stack.push(new SetColor(document, idx, text.length(), Qt.GlobalColor.red));
        stack.endMacro(); // indexChanged() is emitted
//! [4]

//! [5]
        QUndoCommand nsertRed = new QUndoCommand(); // an empty command
        insertRed.setText("insert red text");

        new InsertText(document, idx, text, insertRed); // becomes child of insertRed
        new SetColor(document, idx, text.length(), Qt.GlobalColor.red, insertRed);

        stack.push(insertRed);
//! [5]

    }

    public class AppendTextV2 extends QUndoCommand {

        private String m_document;
        private String m_text;

//! [3]
        public boolean mergeWith(QUndoCommand other)
        {
            if (other.id() != id()) // make sure other is also an AppendText command
                return false;
            m_text += ((AppendText)other).m_text;
            return true;
        }
//! [3]
    }
}
