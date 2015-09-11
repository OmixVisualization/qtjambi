import java.util.*;
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_io_qsettings {
    public static void main(String args[]) {
        QApplication.initialize(args);

        {
//! [0]
        QSettings settings = new QSettings("MySoft", "Star Runner");
        QColor color = (QColor) settings.value("DataPump/bgcolor");
//! [0]
        }


        QWidget widget = new QWidget();
        {
//! [1]
        QSettings settings = new QSettings("MySoft", "Star Runner");
        QColor color = widget.palette().window().color();
        settings.setValue("DataPump/bgcolor", color);
//! [1]
        }


        {
//! [2]
        QSettings settings = new QSettings("/home/petra/misc/myapp.ini",
                           QSettings.Format.IniFormat);
//! [2]
        }


        {
//! [3]
        QSettings settings = new QSettings("/Users/petra/misc/myapp.plist",
                           QSettings.Format.NativeFormat);
//! [3]
        }


        {
//! [4]
        QSettings settings = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Office",
                           QSettings.Format.NativeFormat);
//! [4]


//! [5]
        settings.setValue("11.0/Outlook/Security/DontTrustInstalledFiles", 0);
//! [5]


//! [6]
        settings.setValue("HKEY_CURRENT_USER\\MySoft\\Star Runner\\Galaxy", "Milkyway");
        settings.setValue("HKEY_CURRENT_USER\\MySoft\\Star Runner\\Galaxy\\Sun", "OurStar");
        settings.value("HKEY_CURRENT_USER\\MySoft\\Star Runner\\Galaxy\\Default"); // returns "Milkyway"
//! [6]
        }


        {
//! [7]
        if (QSysInfo.operatingSystem() == QSysInfo.OS_DARWIN) {
            QSettings settings = new QSettings("grenoullelogique.fr", "Squash");
        } else {
            QSettings settings = new QSettings("Grenoulle Logique", "Squash");
        }
//! [7]
        }

/*
//! [8]
        pos = @Point(100 100)
//! [8]


//! [9]
        windir = C:\Windows
//! [9]
*/

        {
//! [10]
        QSettings settings = new QSettings("Moose Tech", "Facturo-Pro");
//! [10]
        }


        {
//! [11]
        QSettings settings = new QSettings("Moose Soft", "Facturo-Pro");
//! [11]
        }


        {
//! [12]
        QCoreApplication.setOrganizationName("Moose Soft");
        QCoreApplication.setApplicationName("Facturo-Pro");
        QSettings settings = new QSettings();
//! [12]


        QMainWindow win = new QMainWindow();
        QFrame panel = new QFrame();
//! [13]
        settings.beginGroup("mainwindow");
        settings.setValue("size", win.size());
        settings.setValue("fullScreen", win.isFullScreen());
        settings.endGroup();

        settings.beginGroup("outputpanel");
        settings.setValue("visible", panel.isVisible());
        settings.endGroup();
//! [13]


//! [14]
        settings.beginGroup("alpha");
        // settings.group() == "alpha"

        settings.beginGroup("beta");
        // settings.group() == "alpha/beta"

        settings.endGroup();
        // settings.group() == "alpha"

        settings.endGroup();
        // settings.group() == ""
//! [14]
        }


        {
//! [15]
        class Login {
            public String userName;
            public String password;
        };
        List<Login> logins = new ArrayList<Login>();
        // ...

        QSettings settings = new QSettings();
        int size = settings.beginReadArray("logins");
        for (int i = 0; i < size; ++i) {
            settings.setArrayIndex(i);
            Login login = new Login();
            login.userName = settings.value("userName").toString();
            login.password = settings.value("password").toString();
            logins.add(login);
        }
        settings.endArray();
//! [15]
        }


        {
//! [16]
        class Login {
            String userName;
            String password;
        };
        List<Login> logins = new ArrayList<Login>();
        // ...

        QSettings settings = new QSettings();
        settings.beginWriteArray("logins");
        for (int i = 0; i < logins.size(); ++i) {
            settings.setArrayIndex(i);
            settings.setValue("userName", logins.get(i).userName);
            settings.setValue("password", logins.get(i).password);
        }
        settings.endArray();
//! [16]
        }


        {
//! [17]
        QSettings settings = new QSettings();
        settings.setValue("fridge/color", Qt.GlobalColor.white);
        settings.setValue("fridge/size", new QSize(32, 96));
        settings.setValue("sofa", true);
        settings.setValue("tv", false);

        List<String> keys = settings.allKeys();
        // keys: ["fridge/color", "fridge/size", "sofa", "tv"]
//! [17]


//! [18]
        settings.beginGroup("fridge");
        keys = settings.allKeys();
        // keys: ["color", "size"]
//! [18]
        }


        {
//! [19]
        QSettings settings = new QSettings();
        settings.setValue("fridge/color", Qt.GlobalColor.white);
        settings.setValue("fridge/size", new QSize(32, 96));
        settings.setValue("sofa", true);
        settings.setValue("tv", false);

        List<String> keys = settings.childKeys();
        // keys: ["sofa", "tv"]
//! [19]


//! [20]
        settings.beginGroup("fridge");
        keys = settings.childKeys();
        // keys: ["color", "size"]
//! [20]
        }


        {
//! [21]
        QSettings settings = new QSettings();
        settings.setValue("fridge/color", Qt.GlobalColor.white);
        settings.setValue("fridge/size", new QSize(32, 96));
        settings.setValue("sofa", true);
        settings.setValue("tv", false);

        List<String> groups = settings.childGroups();
        // group: ["fridge"]
//! [21]


//! [22]
        settings.beginGroup("fridge");
        groups = settings.childGroups();
        // groups: []
//! [22]
        }


        {
//! [23]
        QSettings settings = new QSettings();
        settings.setValue("interval", 30);
        QVariant.toInt(settings.value("interval"));     // returns 30

        settings.setValue("interval", 6.55);
        QVariant.toDouble(settings.value("interval"));  // returns 6.55
//! [23]
        }


        {
//! [24]
        QSettings settings = new QSettings();
        settings.setValue("ape", 0);
        settings.setValue("monkey", 1);
        settings.setValue("monkey/sea", 2);
        settings.setValue("monkey/doe", 4);

        settings.remove("monkey");
        List<String> keys = settings.allKeys();
        // keys: ["ape"]
//! [24]
        }


        {
//! [25]
        QSettings settings = new QSettings();
        settings.setValue("ape", 0);
        settings.setValue("monkey", 1);
        settings.setValue("monkey/sea", 2);
        settings.setValue("monkey/doe", 4);

        settings.beginGroup("monkey");
        settings.remove("");
        settings.endGroup();

        List<String> keys = settings.allKeys();
        // keys: ["ape"]
//! [25]
        }


        {
//! [26]
        QSettings settings = new QSettings();
        settings.setValue("animal/snake", 58);
        QVariant.toInt(settings.value("animal/snake", 1024));   // returns 58
        QVariant.toInt(settings.value("animal/zebra", 1024));   // returns 1024
        QVariant.toInt(settings.value("animal/zebra"));         // returns 0
//! [26]
        }


/*
 * This code is commented out because QSettings::registerFormat() is not part
 * of the Qt/Jambi API
 *
//! [27]
        booleansmyReadFunc(QIODevice evice, QSettings.SettingsMap ap);
//! [27]


//! [28]
        booleansmyWriteFunc(QIODevice evice, QSettings.SettingsMap ap);
//! [28]


//! [29]
        booleansreadXmlFile(QIODevice evice, QSettings.SettingsMap ap);
        booleanswriteXmlFile(QIODevice evice, QSettings.SettingsMap ap);

        int main(int argc, char *argv[])
        {
            QSettings.Format XmlFormat =
                    QSettings.registerFormat("xml", readXmlFile, writeXmlFile);

            QSettings settings = new QSettings(QSettings.Format.XmlFormat, QSettings.UserSettings, "MySoft",
                               "Star Runner");

            ...
        }
//! [29]
*/


    }
}
