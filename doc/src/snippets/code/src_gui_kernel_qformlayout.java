import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class src_gui_kernel_qformlayout extends QWidget {
    QLineEdit nameLineEdit, emailLineEdit, ageSpinBox;
    QLabel nameLabel, emailLabel, ageLabel;
    public void fooBar() {
//! [0]
        QFormLayout formLayout = new QFormLayout();
        formLayout.addRow(tr("Name:"), nameLineEdit);
        formLayout.addRow(tr("Email:"), emailLineEdit);
        formLayout.addRow(tr("Age:"), ageSpinBox);
        setLayout(formLayout);
//! [0]


//! [1]
        nameLabel = new QLabel(tr("Name:"));
        nameLabel.setBuddy(nameLineEdit);

        emailLabel = new QLabel(tr("Email:"));
        emailLabel.setBuddy(emailLineEdit);

        ageLabel = new QLabel(tr("Age:"));
        ageLabel.setBuddy(ageSpinBox);

        QGridLayout gridLayout = new QGridLayout();
        gridLayout.addWidget(nameLabel, 0, 0);
        gridLayout.addWidget(nameLineEdit, 0, 1);
        gridLayout.addWidget(emailLabel, 1, 0);
        gridLayout.addWidget(emailLineEdit, 1, 1);
        gridLayout.addWidget(ageLabel, 2, 0);
        gridLayout.addWidget(ageSpinBox, 2, 1);
        setLayout(gridLayout);
//! [1]


        // FormStyle does not seem to actually exist at all
        // so I removed that line
//! [2]
        formLayout.setLabelAlignment(Qt.AlignmentFlag.AlignLeft);
//! [2]


    }
}
