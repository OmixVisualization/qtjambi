import org.qtjambi.qt.gui.*;


public class src_gui_painting_qdrawutil {
    public static void main(String args[]) {
        QApplication.initialize(args);
        {
//! [0]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.HLine.value() | QFrame.Shadow.Sunken.value());
//! [0]
        }

        {
//! [1]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.Box.value() | QFrame.Shadow.Raised.value());
//! [1]
        }

        {
//! [2]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.Panel.value() | QFrame.Shadow.Sunken.value());
//! [2]
        }

        {
//! [3]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.WinPanel.value() | QFrame.Shadow.Raised.value());
//! [3]
        }

        {
//! [4]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.Box.value() | QFrame.Shadow.Plain.value());
//! [4]
        }

        {
//! [5]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.HLine.value() | QFrame.Shadow.Sunken.value());
//! [5]
        }

        {
//! [6]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.Box.value() | QFrame.Shadow.Raised.value());
//! [6]
        }

        {
//! [7]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.Panel.value() | QFrame.Shadow.Sunken.value());
//! [7]
        }

        {
//! [8]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.WinPanel.value() | QFrame.Shadow.Raised.value());
//! [8]
        }

        {
//! [9]
        QFrame frame = new QFrame();
        frame.setFrameStyle(QFrame.Shape.Box.value() | QFrame.Shadow.Plain.value());
//! [9]
        }
    }
}
