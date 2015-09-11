
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_kernel_qcoreapplication {
    public void do_something(String s) {
        QPoint pos = null;
        QMainWindow mainWindow = null;
//! [0]
        QMouseEvent event = new QMouseEvent(QEvent.Type.MouseButtonPress, pos, Qt.MouseButton.NoButton,
                                            new Qt.MouseButtons(Qt.MouseButton.NoButton),
                                            new Qt.KeyboardModifiers(Qt.KeyboardModifier.NoModifier));
        QApplication.sendEvent(mainWindow, event);
//! [0]


//! [1]
        QPushButton quitButton = new QPushButton("Quit");
        quitButton.clicked.connect(QApplication.instance(), "quit()");
//! [1]


//! [2]
        for (String path : QApplication.libraryPaths())
            do_something(path);
//! [2]


        /* not in use. (function ptr)
//! [3]
        boolean myEventFilter(void *message, long *result);
//! [3]
         */


        /* not in use (global low level function)
//! [4]
        static int lobal_ptr = 0;

        static void cleanup_ptr()
        {
            delete [] global_ptr;
            global_ptr = 0;
        }

        void init_ptr()
        {
            global_ptr = new int[100];      // allocate data
            qAddPostRoutine(cleanup_ptr);   // delete later
        }
//! [4]
 */

    }
}

//! [5]
        class MyPrivateInitStuff extends QObject {
            public static MyPrivateInitStuff initStuff(QObject parent) {
                if (p == null)
                    p = new MyPrivateInitStuff(parent);
                return p;
            }

            @Override
            protected void disposed() {
                // cleanup goes here
            }

            private MyPrivateInitStuff(QObject parent) {
                super(parent);
                // initialization goes here
            }

            private static MyPrivateInitStuff p;
        }
//! [5]


        /*
//! [6]
        static inline Stringstr(char ourceText,
                                 char omment = 0);
        static inline StringstrUtf8(char ourceText,
                                     char omment = 0);
//! [6]
         */


        /*
//! [7]
        class MyMfcView : public CView
        {
            Q_DECLARE_TR_FUNCTIONS(MyMfcView)

        public:
            MyMfcView();
            ...
        };
//! [7]
         */


