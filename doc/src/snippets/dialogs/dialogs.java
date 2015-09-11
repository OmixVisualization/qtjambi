import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;

public class dialogs extends QWidget
{
    class WordCountDialog extends QDialog {
        public WordCountDialog(QWidget w)
        { super(w); }

        void setWordCount(int i) {}
    }
    class FindDialog extends QDialog {
        public Signal0 findNext = new Signal0();

        public FindDialog(QWidget wid) {}
    }

    void setWordCount(int x) {};

    public String tr(String text)
    {
        return QApplication.translate(text, text);
    }

    class EditorWindow extends QMainWindow
    {

    FindDialog findDialog = null;


    //! [0]
    void find()
    {
        if (findDialog == null) {
            findDialog = new FindDialog(this);
            findDialog.findNext.connect(this, "findNext()");
        }

        findDialog.show();
        findDialog.raise();
        findDialog.activateWindow();
    }
    //! [0]

    //! [1]
    void countWords()
    {
        WordCountDialog dialog = new WordCountDialog(this);
        dialog.setWordCount(document().wordCount());
        dialog.exec();
    }
    //! [1]
    }

    class Document {
        public int wordCount() { return -1; }
    }

    Document document()
    {
        return new Document();
    }

    boolean boo()
    {
        /*
        QMessageBox.information(this, "Application name",
                                 "Unable to find the user preferences file.\n" +
                                 "The factory default will be used instead.");

        String filename;
        if (QFile.exists(filename)
            QMessageBox.question(
                this,
                tr("Overwrite File? -- Application Name"),
                tr("A file called"+ filename + "already exists." +
                   "Do you want to overwrite it?"),
                tr("Yes"), tr("No"),
                "", 0, 1))
            return false;

        switch(QMessageBox.warning(this, "Application name",
                                    "Could not connect to the <mumble> server.\n" +
                                    "This program can't function correctly " +
                                    "without the server.\n\n",
                                    "Retry",
                                    "Quit", 0, 0, 1)) {
        case 0: // The user clicked the Retry again button or pressed Enter
            // try again
            break;
        case 1: // The user clicked the Quit or pressed Escape
            // exit
            break;
        }

        switch(QMessageBox.information(this, "Application name here",
                                        "The document contains unsaved changes\n"
                                        "Do you want to save the changes before exiting?",
                                        "Save", "Discard", "Cancel",
                                        0,      // Enter == button 0
                                        2)) { // Escape == button 2
        case 0: // Save clicked or Alt+S pressed or Enter pressed.
            // save
            break;
        case 1: // Discard clicked or Alt+D pressed
            // don't save but exit
            break;
        case 2: // Cancel clicked or Escape pressed
            // don't exit
            break;
        }

        switch(QMessageBox.warning(this, "Application name here",
                                    "Could not save the user preferences,\n"
                                    "because the disk is full. You can delete\n"
                                    "some files and press Retry, or you can\n"
                                    "abort the Save Preferences operation.",
                                    QMessageBox.Retry | QMessageBox.Default,
                                    QMessageBox.Abort | QMessageBox.Escape)) {
        case QMessageBox.Retry: // Retry clicked or Enter pressed
            // try again
            break;
        case QMessageBox.Abort: // Abort clicked or Escape pressed
            // abort
            break;
        }

        String errorDetails;
        QMessageBox.critical(0, "Application name here",
                              QString("An internal error occurred. Please ") +
                              "call technical support at 1234-56789 and report\n"+
                              "these numbers:\n\n" + errorDetails +
                              "\n\nApplication will now exit.");

        QMessageBox.about(this, "About <Application>",
                           "<Application> is a <one-paragraph blurb>\n\n"
                           "Copyright 1991-2003 Such-and-such. "
                           "<License words here.>\n\n"
                           "For technical support, call 1234-56789 or see\n"
                           "http://www.such-and-such.com/Application/\n");

        {
            // saving the file
            QMessageBox mb("Application name here",
                           "Saving the file will overwrite the original file on the disk.\n"
                           "Do you really want to save?",
                           QMessageBox.Information,
                           QMessageBox.Yes | QMessageBox.Default,
                           QMessageBox.No,
                           QMessageBox.Cancel | QMessageBox.Escape);
            mb.setButtonText(QMessageBox.Yes, "Save");
            mb.setButtonText(QMessageBox.No, "Discard");
            switch(mb.exec()) {
            case QMessageBox.Yes:
                // save and exit
                break;
            case QMessageBox.No:
                // exit without saving
                break;
            case QMessageBox.Cancel:
                // don't save and don't exit
                break;
            }
        }*/


            // hardware failure
    //! [2]
            QMessageBox mb = new QMessageBox(QMessageBox.Icon.Question ,"Application Name",
                           "Hardware failure.\n\nDisk error detected\nDo you want to stop?",
                           new QMessageBox.StandardButtons(
                           QMessageBox.StandardButton.Yes, QMessageBox.StandardButton.Default,
                           QMessageBox.StandardButton.No, QMessageBox.StandardButton.Escape,
                           QMessageBox.StandardButton.NoButton));
            if (mb.exec() == QMessageBox.StandardButton.No.value()) {
                // try again
    //! [2]
            }
        return true;
    }

    void moo()
    {
        int numFiles = 0;
    //! [3]
        QProgressDialog progress =
            new QProgressDialog("Copying files...", "Abort Copy", 0, numFiles, this);
        progress.setWindowModality(Qt.WindowModality.WindowModal);

        for (int i = 0; i < numFiles; i++) {
            progress.setValue(i);

            if (progress.wasCanceled())
                break;
            //... copy one file
        }
        progress.setValue(numFiles);
    //! [3]
    }

    class Operation extends QObject
    {
        int steps;
        QProgressDialog pd;
        QTimer t;

    //! [4]
    // Operationructor
    public Operation(QObject parent)
    {
        super(parent);

        pd = new QProgressDialog("Operation in progress.", "Cancel", 0, 100);
        pd.canceled.connect(this, "cancel()");

        t = new QTimer(this);
        t.timeout.connect(this, "perform()");

        t.start(0);
    }
    //! [4] //! [5]

    void perform()
    {
        pd.setValue(steps);
        //... perform one percent of the operation
        steps++;
        if (steps > pd.maximum())
            t.stop();
    }
    //! [5] //! [6]

    void cancel()
    {
        t.stop();
        //... cleanup
    }
    //! [6]
    }

    public static void main(String args[])
    {
        QApplication.initialize(args);

        new dialogs();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
