import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


class doc_src_linguist_manual extends QWidget {
    public static void main(String args[]) {
        QApplication.initialize(args);
    }
  /*
//! [0]
        HEADERS         = main-dlg.h \
                          options-dlg.h
        SOURCES         = main-dlg.cpp \
                          options-dlg.cpp \
                          main.cpp
        FORMS           = search-dlg.ui
        TRANSLATIONS    = superapp_dk.ts \
                          superapp_fi.ts \
                          superapp_no.ts \
                          superapp_se.ts
//! [0]


//! [1]
        CODECFORTR      = ISO-8859-5
//! [1]


//! [2]
        CODECFORSRC     = UTF-8
//! [2]
*/

    public void snippet3(QLabel label) {
//! [3]
        label.setText(tr("F\374r \310lise"));
//! [3]
    }
/*
//! [4]
    Usage:
        lupdate [options] [source-file|path]... -ts ts-files
    Options:
        -help  Display this information and exit.
        -noobsolete
               Drop all obsolete strings.
        -extensions <ext>[,<ext>]...
               Process files with the given extensions only.
               The extension list must be separated with commas, not with whitespace.
               Default: 'ui,c,c++,cc,cpp,cxx,ch,h,h++,hh,hpp,hxx'.
        -silent
               Don't explain what is being done.
        -version
               Display the version of lupdate and exit.
//! [4]


//! [5]
        Usage:
            lrelease [options] ts-files [-qm qm-file]
        Options:
            -help  Display this information and exit
            -compress
                   Compress the .qm files
            -nounfinished
                   Do not include unfinished translations
            -silent
                   Don't explain what is being done
            -version
                   Display the version of lrelease and exit
//! [5]
*/
    QPushButton button;
    public void snippet6() {
//! [6]
        button = new QPushButton("&Quit", this);
//! [6]
    }

    public void snippet7() {
//! [7]
        button = new QPushButton(tr("&Quit"), this);
//! [7]
    }

    public void snippet10() {
           QRadioButton rbc;
//! [10]
           rbc = new QRadioButton(tr("Enabled", "Color frame"), this);
//! [10]
    }
    public void snippet11() {
           QRadioButton rbh;
//! [11]
           rbh = new QRadioButton(tr("Enabled", "Hue frame"), this);
//! [11]
    }

//! [12]
        /*
            TRANSLATOR FindDialog

            Choose Edit|Find from the menu bar or press Ctrl+F to pop up the
            Find dialog.

            ...
//! [12]
*/

//! [13]
        /*
            TRANSLATOR MyNamespace.MyClass

            Necessary for lupdate.

            ...
//! [13]
*/

class LoginWidget extends QWidget {}
        QApplication qApp;
//! [14]
        void some_global_function(LoginWidget logwid)
        {
            QLabel label = new QLabel(
                    logwid.tr("Password:"), logwid);
        }

        void same_global_function(LoginWidget logwid)
        {
            QLabel label = new QLabel(
                    qApp.translate("LoginWidget", "Password:"),
                    logwid);
        }
//! [14]



    }
