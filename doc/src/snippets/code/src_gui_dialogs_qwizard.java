import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;
import java.util.List;
import java.util.LinkedList;

class LicenceWizard extends QWizard {

    private static final int Page_Intro = 0;
    private static final int Page_Evaluate = 1;
    private static final int Page_Register = 2;
    private static final int Page_Details = 3;
    private static final int Page_Conclusion = 4;

//! [0]
    public int nextId()
    {
        switch (currentId()) {
        case Page_Intro:
            if (QVariant.toBoolean(field("intro.evaluate"))) {
                return Page_Evaluate;
            } else {
                return Page_Register;
            }
        case Page_Evaluate:
            return Page_Conclusion;
        case Page_Register:
            if (QVariant.toString(field("register.upgradeKey")).length() == 0) {
                return Page_Details;
            } else {
                return Page_Conclusion;
            }
        case Page_Details:
            return Page_Conclusion;
        case Page_Conclusion:
        default:
            return -1;
        }
    }
//! [0]
}

class MyWizard extends QWizard {
//! [1]
    public MyWizard(QWidget parent)
    {
        super(parent);
        // ...
        List<QWizard.WizardButton> layout = new LinkedList<QWizard.WizardButton>();
        layout.add(QWizard.WizardButton.Stretch);
        layout.add(QWizard.WizardButton.BackButton);
        layout.add(QWizard.WizardButton.CancelButton);
        layout.add(QWizard.WizardButton.NextButton);
        layout.add(QWizard.WizardButton.FinishButton);
        setButtonLayout(layout);
        // ...
    }
//! [1]
}
