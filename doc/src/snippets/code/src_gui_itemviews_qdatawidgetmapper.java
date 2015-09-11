import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class src_gui_itemviews_qdatawidgetmapper {
    public static void main(String args[]) {
        QApplication.initialize(args);

        QAbstractItemModel model = null;
        QSpinBox mySpinBox = null;
        QLineEdit myLineEdit = null;
        QWidget myCountryChooser = null;

        {
//! [0]
    QDataWidgetMapper mapper = new QDataWidgetMapper();
    mapper.setModel(model);
    mapper.addMapping(mySpinBox, 0);
    mapper.addMapping(myLineEdit, 1);
    mapper.addMapping(myCountryChooser, 2);
    mapper.toFirst();
//! [0]
        }

        {

            QLineEdit nameLineEdit = null;
            QSpinBox ageSpinBox = null;
//! [1]
    QDataWidgetMapper mapper = new QDataWidgetMapper();
    mapper.setModel(model);
    mapper.addMapping(nameLineEdit, 0);
    mapper.addMapping(ageSpinBox, 1);
//! [1]
        }

    QTableView myTableView = null;
//! [2]
    QDataWidgetMapper mapper = new QDataWidgetMapper();
    myTableView.selectionModel().currentRowChanged.connect(mapper, "setCurrentModelIndex(QModelIndex)");
//! [2]


    }
}
