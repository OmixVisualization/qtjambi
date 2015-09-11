/*   Ported from: doc.src.mac-differences.qdoc
<snip>
//! [0]
        QMAKE_LFLAGS_SONAME  = -Wl,-install_name,@executable_path/../Frameworks/
//! [0]


//! [1]
    #ifdef Q_WS_MAC
        CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
        CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef,
                                               kCFURLPOSIXPathStyle);
        const char *pathPtr = CFStringGetCStringPtr(macPath,
                                               CFStringGetSystemEncoding());
        qDebug("Path = %s", pathPtr);
        CFRelease(appUrlRef);
        CFRelease(macPath);
    #endif
//! [1]


//! [2]
        <?xml version="1.0" encoding="UTF-8"?>
        <!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN"
        "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
        <plist version="1.0">
        <dict>
            <key>LprojCompatibleVersion</key>
            <string>123</string>
            <key>LprojLocale</key>
            <string>no</string>
            <key>LprojRevisionLevel</key>
            <string>1</string>
            <key>LprojVersion</key>
            <string>123</string>
        </dict>
        </plist>
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


public class doc_src_mac-differences {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
        QMAKE_LFLAGS_SONAME  = -Wl,-install_name,@executable_path/../Frameworks/
//! [0]


//! [1]
    #ifdef Q_WS_MAC
        CFURLRef appUrlRef = CFBundleCopyBundleURL(CFBundleGetMainBundle());
        CFStringRef macPath = CFURLCopyFileSystemPath(appUrlRef,
                                               kCFURLPOSIXPathStyle);
        char athPtr = CFStringGetCStringPtr(macPath,
                                               CFStringGetSystemEncoding());
        qDebug("Path = %s", pathPtr);
        CFRelease(appUrlRef);
        CFRelease(macPath);
    #endif
//! [1]


//! [2]
        <?xml version="1.0" encoding="UTF-8"?>
        <!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN"
        "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
        <plist version="1.0">
        <dict>
            <key>LprojCompatibleVersion</key>
            <string>123</string>
            <key>LprojLocale</key>
            <string>no</string>
            <key>LprojRevisionLevel</key>
            <string>1</string>
            <key>LprojVersion</key>
            <string>123</string>
        </dict>
        </plist>
//! [2]


    }
}
