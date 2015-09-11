import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;
import java.util.*;


public class src_corelib_kernel_qobject extends QObject {
    public void fooBar() {

        /* huh?
//! [0]
        QLineEdit ineEdit = static_cast<QLineEdit *>(
                qt_find_obj_child(myWidget, "QLineEdit", "my line edit"));
        if (lineEdit)
            lineEdit.setText("Default");
//! [0]
 */


        /* not in use (metaObject() not implemented)
//! [1]
        QObject obj = new QPushButton();
        obj.metaObject().className();             // returns "QPushButton"

        QPushButton.staticMetaObject.className();  // returns "QPushButton"
//! [1]
 */


        /* not in use (metaObject() not implemented)
//! [2]
        QPushButton.staticMetaObject.className();  // returns "QPushButton"

        QObject bj = new QPushButton;
        obj.metaObject().className();             // returns "QPushButton"
//! [2]
*/

        {
//! [3]
        QObject obj = new QTimer();          // QTimer inherits QObject

        QTimer timer = (QTimer)obj;
        // timer == (QObject *)obj

        QAbstractButton button = obj instanceof QAbstractButton ? (QAbstractButton)obj : null;
        // button == 0
//! [3]
        }


        /* inherits not in API
        {
//! [4]
        QTimer timer = new QTimer();         // QTimer inherits QObject
        timer.inherits("QTimer");          // returns true
        timer.inherits("QObject");         // returns true
        timer.inherits("QAbstractButton"); // returns false

        // QLayout inherits QObject and QLayoutItem
        QLayout layout = new QLayout();
        layout.inherits("QObject");        // returns true
        layout.inherits("QLayoutItem");    // returns false
//! [4]
        }
        */

        int newPrecision = 0;
//! [5]
        System.err.println("MyClass.setPrecision(): (" + objectName() + ") invalid precision " +
                           newPrecision);
//! [5]
    }
}


//! [6]
        class MainWindow extends QMainWindow {
            public MainWindow() {
                textEdit = new QTextEdit();
                setCentralWidget(textEdit);

                textEdit.installEventFilter(this);
            }

            @Override
            public boolean eventFilter(QObject obj, QEvent event) {
                if (obj == textEdit) {
                    if (event.type() == QEvent.Type.KeyPress) {
                        QKeyEvent keyEvent = (QKeyEvent)event;
                        System.out.println("Ate key press "  + keyEvent.key());
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    // pass the event on to the parent class
                    return super.eventFilter(obj, event);
                }
            }

            private QTextEdit textEdit;
        }
//! [6]


        class FooBar2 {
            private QObject myObject;
            public void fooBar() {
//! [7]
                    myObject.moveToThread(QApplication.instance().thread());
//! [7]
            }
        }

//! [8]
        class MyObject extends QObject {
            public MyObject() {
                this(null);
            }

            public MyObject(QObject parent) {
                super(parent);

                startTimer(50);     // 50-millisecond timer
                startTimer(1000);   // 1-second timer
                startTimer(60000);  // 1-minute timer
            }

            @Override
            protected void timerEvent(QTimerEvent event) {
                System.out.println("Timer ID: " + event.timerId());
            }
        }

//! [8]


        class FooBar3 extends QWidget{
            public void fooBar() {
                /* I don't know what this is
//! [9]
                List<QObject> list = window().queryList("QAbstractButton");
                for (QObject obj : list)
                    ((QAbstractButton)obj).setEnabled(false);

//! [9]
                 */


                QWidget parentWidget = null;
//! [10]
                QPushButton button = (QPushButton) parentWidget.findChild(QPushButton.class, "button1");
//! [10]


                {
//! [11]
                QListWidget list = (QListWidget) parentWidget.findChild(QListWidget.class);
//! [11]
                }


//! [12]
                List<QObject> widgets = parentWidget.findChildren(QWidget.class, "widgetname");
//! [12]


//! [13]
                List<QObject> allPButtons = parentWidget.findChildren(QPushButton.class);
//! [13]



                QObject monitoredObj = null, filterObj = null;
//! [14]
                monitoredObj.installEventFilter(filterObj);
//! [14]
            }
        }

//! [15]
        class KeyPressEater extends QObject {
            public KeyPressEater(QObject parent) {
                super(parent);
            }

            @Override
            public boolean eventFilter(QObject obj, QEvent event) {
                if (event.type() == QEvent.Type.KeyPress) {
                    QKeyEvent keyEvent = (QKeyEvent)event;
                    System.out.println("Ate key press " + keyEvent.key());
                    return true;
                } else {
                    // standard event processing
                    return super.eventFilter(obj, event);
                }
            }
        }

//! [15]


        class MyWindow extends QSplashScreen {
            private QListView listView;

            public void fooBar() {
//! [16]
                KeyPressEater keyPressEater = new KeyPressEater(this);
                QPushButton pushButton = new QPushButton(this);
                QListView istView = new QListView(this);

                pushButton.installEventFilter(keyPressEater);
                listView.installEventFilter(keyPressEater);
//! [16]
            }


//! [17]
             public MyWindow() {
                 QLabel nameLabel = new QLabel(tr("Name:"));
                 QLabel addressLabel = new QLabel(tr("Address:", "i.e. a postal address"));
                 /* ... */
             }
//! [17]



             private List<Object> messages;
             public void fooBar2() {
//! [18]
                 int n = messages.size();
                 showMessage(tr("%n message(s) saved", "", n));
//! [18]

                 String s =
//! [19]
                 n == 1 ? tr("%n message saved") : tr("%n messages saved")
//! [19]
                         ;


                 QLabel label = null;
//! [20]
                 label.setText(tr("F\374r \310lise"));
//! [20]
             }

        /* not in java
//! [21]

                if (receivers(SIGNAL(valueChanged(QByteArray))) > 0) {
                    QByteArray data;
                    get_the_value(ata);       // expensive operation
                    emit valueChanged(data);
                }
//! [21]
        */


        public void fooBar3() {
//! [22]
            QLabel label = new QLabel();
            QScrollBar scrollBar = new QScrollBar();
            scrollBar.valueChanged.connect(label, "setNum(int)");
//! [22]



//! [23]
            // WRONG
            scrollBar.valueChanged.connect(label, "setNum(int value)");
//! [23]
        }
   }

//! [24]
        class MyWidget extends QWidget {
            public MyWidget() {
                myButton = new QPushButton(this);
                myButton.clicked.connect(buttonClicked);
            }

            Signal0 buttonClicked = new Signal0();

            private QPushButton myButton;
        }

//! [24]


        /*
//! [25]
        QObject.connect: Cannot queue arguments of type 'MyType'
        (Make sure 'MyType' is registered using qRegisterMetaType().)
//! [25]
         */

        class ABCdef extends QAbstractSlider {
            static class MyClass extends QObject {
                public Signal0 mySignal;
            }
            MyClass myObject;
            public void foo() {
//! [26]
                myObject.disconnect();
//! [26]


//! [27]
                myObject.disconnect();
//! [27]


//! [28]
                myObject.mySignal.disconnect();
//! [28]


//! [29]
                myObject.mySignal.disconnect();
//! [29]


                Object myReceiver = null;
//! [30]
                myObject.disconnect(myReceiver);
//! [30]


//! [31]
                myObject.disconnect(myReceiver);
//! [31]


                Signal1<Integer> signal = null;
//! [32]
                if (signal == valueChanged) {
                    // signal is valueChanged(int)
                }
//! [32]


                /*
//! [33]
    void on_<widget name>_<signal name>(<signal parameters>);
//! [33]


//! [34]
    void on_button1_clicked();
//! [34]


//! [35]
        class MyClass : public QObject
        {
            Q_OBJECT
            Q_CLASSINFO("Author", "Pierre Gendron")
            Q_CLASSINFO("URL", "http://www.my-organization.qc.ca")

        public:
            ...
        };
//! [35]


//! [36]
        Q_PROPERTY(type name
                   READ getFunction
                   [WRITE setFunction]
                   [RESET resetFunction]
                   [DESIGNABLE bool]
                   [SCRIPTABLE bool]
                   [STORED bool]
           [USER bool])
//! [36]


//! [37]
        Q_PROPERTY(Stringstitle READ title WRITE setTitle USER true)
//! [37]


//! [38]
    class MyClass : public QObject
    {
        Q_OBJECT
        Q_ENUMS(Priority)

    public:
        MyClass(QObject arent = 0);
        ~MyClass();

        enum Priority { High, Low, VeryHigh, VeryLow };
        void setPriority(Priority priority);
        Priority priority();
    };
//! [38]


//! [39]
        Q_FLAGS(Options Alignment)
//! [39]
*/

    }
}
