/*   Ported from: doc.src.qtdesigner.qdoc
<snip>
//! [0]
        #include <QtDesigner>
//! [0]


//! [1]
        CONFIG += designer
//! [1]


//! [2]
        QDesignerMemberSheetExtension *memberSheet  = 0;
        QExtensionManager manager = formEditor->extensionManager();

        memberSheet = qt_extension<QDesignerMemberSheetExtension*>(manager, widget);
        int index = memberSheet->indexOf(setEchoMode);
        memberSheet->setVisible(index, false);

        delete memberSheet;
//! [2]


//! [3]
       class MyMemberSheetExtension : public QObject,
               public QDesignerMemberSheetExtension
       {
           Q_OBJECT
           Q_INTERFACES(QDesignerMemberSheetExtension)

       public:
           ...
       }
//! [3]


//! [4]
        QObject *ANewExtensionFactory::createExtension(QObject *object,
                const QString &iid, QObject *parent) const
        {
            if (iid != Q_TYPEID(QDesignerMemberSheetExtension))
                return 0;

            if (MyCustomWidget *widget = qobject_cast<MyCustomWidget*>
                   (object))
                return new MyMemberSheetExtension(widget, parent);

            return 0;
        }
//! [4]


//! [5]
        QObject *AGeneralExtensionFactory::createExtension(QObject *object,
                const QString &iid, QObject *parent) const
        {
            MyCustomWidget *widget = qobject_cast<MyCustomWidget*>(object);

            if (widget && (iid == Q_TYPEID(QDesignerTaskMenuExtension))) {
                return new MyTaskMenuExtension(widget, parent);

            } else if (widget && (iid == Q_TYPEID(QDesignerMemberSheetExtension))) {
                return new MyMemberSheetExtension(widget, parent);

            } else {
                return 0;
            }
        }
//! [5]


//! [6]
        class MyContainerExtension : public QObject,
               public QDesignerContainerExtension
        {
            Q_OBJECT
            Q_INTERFACES(QDesignerContainerExtension)

        public:
            MyContainerExtension(MyCustomWidget *widget,
                                 QObject *parent = 0);
            int count() const;
            QWidget *widget(int index) const;
            int currentIndex() const;
            void setCurrentIndex(int index);
            void addWidget(QWidget *widget);
            void insertWidget(int index, QWidget *widget);
            void remove(int index);

        private:
            MyCustomWidget *myWidget;
        };
//! [6]


//! [7]
        QObject *ANewExtensionFactory::createExtension(QObject *object,
                const QString &iid, QObject *parent) const
        {
            if (iid != Q_TYPEID(QDesignerContainerExtension))
                return 0;

            if (MyCustomWidget *widget = qobject_cast<MyCustomWidget*>
                   (object))
                return new MyContainerExtension(widget, parent);

            return 0;
        }
//! [7]


//! [8]
        QObject *AGeneralExtensionFactory::createExtension(QObject *object,
                const QString &iid, QObject *parent) const
        {
            MyCustomWidget *widget = qobject_cast<MyCustomWidget*>(object);

            if (widget && (iid == Q_TYPEID(QDesignerTaskMenuExtension))) {
                return new MyTaskMenuExtension(widget, parent);

            } else if (widget && (iid == Q_TYPEID(QDesignerContainerExtension))) {
                return new MyContainerExtension(widget, parent);

            } else {
                return 0;
            }
        }
//! [8]


//! [9]
        class MyTaskMenuExtension : public QObject,
                public QDesignerTaskMenuExtension
        {
            Q_OBJECT
            Q_INTERFACES(QDesignerTaskMenuExtension)

        public:
            MyTaskMenuExtension(MyCustomWidget *widget, QObject *parent);

            QAction *preferredEditAction() const;
            QList<QAction *> taskActions() const;

        private slots:
            void mySlot();

        private:
            MyCustomWidget *widget;
            QAction *myAction;
        };
//! [9]


//! [10]
        QObject *ANewExtensionFactory::createExtension(QObject *object,
                const QString &iid, QObject *parent) const
        {
            if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
                return 0;

            if (MyCustomWidget *widget = qobject_cast<MyCustomWidget*>(object))
                return new MyTaskMenuExtension(widget, parent);

            return 0;
        }
//! [10]


//! [11]
        QObject *AGeneralExtensionFactory::createExtension(QObject *object,
                const QString &iid, QObject *parent) const
        {
            MyCustomWidget *widget = qobject_cast<MyCustomWidget*>(object);

            if (widget && (iid == Q_TYPEID(QDesignerContainerExtension))) {
                return new MyContainerExtension(widget, parent);

            } else if (widget && (iid == Q_TYPEID(QDesignerTaskMenuExtension))) {
                return new MyTaskMenuExtension(widget, parent);

            } else {
                return 0;
            }
        }
//! [11]


//! [12]
    #include customwidgetoneinterface.h
    #include customwidgettwointerface.h
    #include customwidgetthreeinterface.h

    #include <QtDesigner/QtDesigner>
    #include <QtCore/qplugin.h>

    class MyCustomWidgets: public QObject, public QDesignerCustomWidgetCollectionInterface
    {
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

    public:
        MyCustomWidgets(QObject *parent = 0);

        virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

    private:
        QList<QDesignerCustomWidgetInterface*> widgets;
    };
//! [12]


//! [13]
    MyCustomWidgets::MyCustomWidgets(QObject *parent)
            : QObject(parent)
    {
        widgets.append(new CustomWidgetOneInterface(this));
        widgets.append(new CustomWidgetTwoInterface(this));
        widgets.append(new CustomWidgetThreeInterface(this));
    }

    QList<QDesignerCustomWidgetInterface*> MyCustomWidgets::customWidgets() const
    {
        return widgets;
    }

    Q_EXPORT_PLUGIN2(MyCustomWidgets)
//! [13]


//! [14]
        Q_EXPORT_PLUGIN2(customwidgetplugin, MyCustomWidget)
//! [14]


//! [15]
        QDesignerPropertySheetExtension *propertySheet  = 0;
        QExtensionManager manager = formEditor->extensionManager();

        propertySheet = qt_extension<QDesignerPropertySheetExtension*>(manager, widget);
        int index = propertySheet->indexOf(QLatin1String("margin"));

        propertySheet->setProperty(index, 10);
        propertySheet->setChanged(index, true);

        delete propertySheet;
//! [15]


//! [16]
       class MyPropertySheetExtension : public QObject,
               public QDesignerPropertySheetExtension
       {
           Q_OBJECT
           Q_INTERFACES(QDesignerPropertySheetExtension)

       public:
           ...
       }
//! [16]


//! [17]
        QObject *ANewExtensionFactory::createExtension(QObject *object,
                const QString &iid, QObject *parent) const
        {
            if (iid != Q_TYPEID(QDesignerPropertySheetExtension))
                return 0;

            if (MyCustomWidget *widget = qobject_cast<MyCustomWidget*>
                   (object))
                return new MyPropertySheetExtension(widget, parent);

            return 0;
        }
//! [17]


//! [18]
        QObject *AGeneralExtensionFactory::createExtension(QObject *object,
                const QString &iid, QObject *parent) const
        {
            MyCustomWidget *widget = qobject_cast<MyCustomWidget*>(object);

            if (widget && (iid == Q_TYPEID(QDesignerTaskMenuExtension))) {
                return new MyTaskMenuExtension(widget, parent);

            } else if (widget && (iid == Q_TYPEID(QDesignerPropertySheetExtension))) {
                return new MyPropertySheetExtension(widget, parent);

            } else {
                return 0;
            }
        }
//! [18]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_qtdesigner {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        #include <QtDesigner>
//! [0]


//! [1]
        CONFIG += designer
//! [1]


//! [2]
        QDesignerMemberSheetExtension emberSheet  = 0;
        QExtensionManager manager = formEditor.extensionManager();

        memberSheet = qt_extension<QDesignerMemberSheetExtension*>(manager, widget);
        int index = memberSheet.indexOf(setEchoMode);
        memberSheet.setVisible(index, false);

        delete memberSheet;
//! [2]


//! [3]
       class MyMemberSheetExtension : public QObject,
               public QDesignerMemberSheetExtension
       {
           Q_OBJECT
           Q_INTERFACES(QDesignerMemberSheetExtension)

       public:
           ...
       }
//! [3]


//! [4]
        QObject NewExtensionFactory.createExtension(QObject bject,
                Stringsid, QObject arent)
        {
            if (iid != Q_TYPEID(QDesignerMemberSheetExtension))
                return 0;

            if (MyCustomWidget idget = qobject_cast<MyCustomWidget*>
                   (object))
                return new MyMemberSheetExtension(widget, parent);

            return 0;
        }
//! [4]


//! [5]
        QObject GeneralExtensionFactory.createExtension(QObject bject,
                Stringsid, QObject arent)
        {
            MyCustomWidget idget = qobject_cast<MyCustomWidget*>(object);

            if (widget && (iid == Q_TYPEID(QDesignerTaskMenuExtension))) {
                return new MyTaskMenuExtension(widget, parent);

            } else if (widget && (iid == Q_TYPEID(QDesignerMemberSheetExtension))) {
                return new MyMemberSheetExtension(widget, parent);

            } else {
                return 0;
            }
        }
//! [5]


//! [6]
        class MyContainerExtension : public QObject,
               public QDesignerContainerExtension
        {
            Q_OBJECT
            Q_INTERFACES(QDesignerContainerExtension)

        public:
            MyContainerExtension(MyCustomWidget idget,
                                 QObject arent = 0);
            int count();
            QWidget idget(int index);
            int currentIndex();
            void setCurrentIndex(int index);
            void addWidget(QWidget idget);
            void insertWidget(int index, QWidget idget);
            void remove(int index);

        private:
            MyCustomWidget yWidget;
        };
//! [6]


//! [7]
        QObject NewExtensionFactory.createExtension(QObject bject,
                Stringsid, QObject arent)
        {
            if (iid != Q_TYPEID(QDesignerContainerExtension))
                return 0;

            if (MyCustomWidget idget = qobject_cast<MyCustomWidget*>
                   (object))
                return new MyContainerExtension(widget, parent);

            return 0;
        }
//! [7]


//! [8]
        QObject GeneralExtensionFactory.createExtension(QObject bject,
                Stringsid, QObject arent)
        {
            MyCustomWidget idget = qobject_cast<MyCustomWidget*>(object);

            if (widget && (iid == Q_TYPEID(QDesignerTaskMenuExtension))) {
                return new MyTaskMenuExtension(widget, parent);

            } else if (widget && (iid == Q_TYPEID(QDesignerContainerExtension))) {
                return new MyContainerExtension(widget, parent);

            } else {
                return 0;
            }
        }
//! [8]


//! [9]
        class MyTaskMenuExtension : public QObject,
                public QDesignerTaskMenuExtension
        {
            Q_OBJECT
            Q_INTERFACES(QDesignerTaskMenuExtension)

        public:
            MyTaskMenuExtension(MyCustomWidget idget, QObject arent);

            QAction referredEditAction();
            QList<QAction *> taskActions();

        private slots:
            void mySlot();

        private:
            MyCustomWidget idget;
            QAction yAction;
        };
//! [9]


//! [10]
        QObject NewExtensionFactory.createExtension(QObject bject,
                Stringsid, QObject arent)
        {
            if (iid != Q_TYPEID(QDesignerTaskMenuExtension))
                return 0;

            if (MyCustomWidget idget = qobject_cast<MyCustomWidget*>(object))
                return new MyTaskMenuExtension(widget, parent);

            return 0;
        }
//! [10]


//! [11]
        QObject GeneralExtensionFactory.createExtension(QObject bject,
                Stringsid, QObject arent)
        {
            MyCustomWidget idget = qobject_cast<MyCustomWidget*>(object);

            if (widget && (iid == Q_TYPEID(QDesignerContainerExtension))) {
                return new MyContainerExtension(widget, parent);

            } else if (widget && (iid == Q_TYPEID(QDesignerTaskMenuExtension))) {
                return new MyTaskMenuExtension(widget, parent);

            } else {
                return 0;
            }
        }
//! [11]


//! [12]
    #include customwidgetoneinterface.h
    #include customwidgettwointerface.h
    #include customwidgetthreeinterface.h

    #include <QtDesigner/QtDesigner>
    #include <QtCore/qplugin.h>

    class MyCustomWidgets: public QObject, public QDesignerCustomWidgetCollectionInterface
    {
        Q_OBJECT
        Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

    public:
        MyCustomWidgets(QObject arent = 0);

        virtual QList<QDesignerCustomWidgetInterface*> customWidgets();

    private:
        QList<QDesignerCustomWidgetInterface*> widgets;
    };
//! [12]


//! [13]
    MyCustomWidgets.MyCustomWidgets(QObject arent)
            : QObject(parent)
    {
        widgets.append(new CustomWidgetOneInterface(this));
        widgets.append(new CustomWidgetTwoInterface(this));
        widgets.append(new CustomWidgetThreeInterface(this));
    }

    QList<QDesignerCustomWidgetInterface*> MyCustomWidgets.customWidgets()
    {
        return widgets;
    }

    Q_EXPORT_PLUGIN2(MyCustomWidgets)
//! [13]


//! [14]
        Q_EXPORT_PLUGIN2(customwidgetplugin, MyCustomWidget)
//! [14]


//! [15]
        QDesignerPropertySheetExtension ropertySheet  = 0;
        QExtensionManager manager = formEditor.extensionManager();

        propertySheet = qt_extension<QDesignerPropertySheetExtension*>(manager, widget);
        int index = propertySheet.indexOf(QLatin1String("margin"));

        propertySheet.setProperty(index, 10);
        propertySheet.setChanged(index, true);

        delete propertySheet;
//! [15]


//! [16]
       class MyPropertySheetExtension : public QObject,
               public QDesignerPropertySheetExtension
       {
           Q_OBJECT
           Q_INTERFACES(QDesignerPropertySheetExtension)

       public:
           ...
       }
//! [16]


//! [17]
        QObject NewExtensionFactory.createExtension(QObject bject,
                Stringsid, QObject arent)
        {
            if (iid != Q_TYPEID(QDesignerPropertySheetExtension))
                return 0;

            if (MyCustomWidget idget = qobject_cast<MyCustomWidget*>
                   (object))
                return new MyPropertySheetExtension(widget, parent);

            return 0;
        }
//! [17]


//! [18]
        QObject GeneralExtensionFactory.createExtension(QObject bject,
                Stringsid, QObject arent)
        {
            MyCustomWidget idget = qobject_cast<MyCustomWidget*>(object);

            if (widget && (iid == Q_TYPEID(QDesignerTaskMenuExtension))) {
                return new MyTaskMenuExtension(widget, parent);

            } else if (widget && (iid == Q_TYPEID(QDesignerPropertySheetExtension))) {
                return new MyPropertySheetExtension(widget, parent);

            } else {
                return 0;
            }
        }
//! [18]


    }
}
