/*   Ported from: doc.src.qt4-accessibility.qdoc
<snip>
//! [0]
        class MyWidgetInterface : public QAccessibleWidget
        {
        public:
            MyWidgetInterface(QWidget *widget, Role role);

            QString text(Text text, int child) const;
            State state(int child) const;
            QString actionText(int action, Text text, int child) const;
            bool doAction(int action, int child, const QVariantList &params);
            ...
        };
//! [0]


//! [1]
        bool MyWidgetInterface::doAction(int action, int child,
                                         const QVariantList &params)
        {
            if (child || !widget()->isEnabled())
                return false;

            switch (action) {
            case DefaultAction:
            case Press:
                {
                    MyWidget *widget = qobject_cast<MyWidget *>(object());
                    if (widget)
                        widget->click();
                }
                return true;
            }
            return QAccessibleWidget::doAction(action, child, params);
        }
//! [1]


//! [2]
        QStringList MyFactory::keys() const
        {
            return QStringList() << "MyWidget" << "MyOtherWidget";
        }

        QAccessibleInterface *MyFactory::create(const QString &className,
                                                QObject *object)
        {
            if (classname == "MyWidget")
                return new MyWidgetInterface(object);
            if (classname == "MyOtherWidget")
                return new MyOtherWidgetInterface(object);
            return 0;
        }

        Q_EXPORT_PLUGIN2(MyFactory)
//! [2]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_qt4-accessibility {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        class MyWidgetInterface : public QAccessibleWidget
        {
        public:
            MyWidgetInterface(QWidget idget, Role role);

            Stringstext(Text text, int child);
            State state(int child);
            StringsactionText(int action, Text text, int child);
            booleansdoAction(int action, int child, QVariantList arams);
            ...
        };
//! [0]


//! [1]
        booleansMyWidgetInterface.doAction(int action, int child,
                                         QVariantList arams)
        {
            if (child || !widget().isEnabled())
                return false;

            switch (action) {
            case DefaultAction:
            case Press:
                {
                    MyWidget idget = qobject_cast<MyWidget *>(object());
                    if (widget)
                        widget.click();
                }
                return true;
            }
            return QAccessibleWidget.doAction(action, child, params);
        }
//! [1]


//! [2]
        List<String> MyFactory.keys()
        {
            return List<String>() << "MyWidget" << "MyOtherWidget";
        }

        QAccessibleInterface yFactory.create(StringslassName,
                                                QObject bject)
        {
            if (classname == "MyWidget")
                return new MyWidgetInterface(object);
            if (classname == "MyOtherWidget")
                return new MyOtherWidgetInterface(object);
            return 0;
        }

        Q_EXPORT_PLUGIN2(MyFactory)
//! [2]


    }
}
