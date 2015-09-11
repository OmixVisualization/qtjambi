/*   Ported from: doc.src.installation.qdoc
<snip>
//! [0]
    cd /tmp
    gunzip qt-x11-opensource-desktop-%VERSION%.tar.gz        # uncompress the archive
    tar xvf qt-x11-opensource-desktop-%VERSION%.tar          # unpack it
//! [0]


//! [1]
    cd /tmp/qt-x11-opensource-desktop-%VERSION%
    ./configure
//! [1]


//! [2]
    make
//! [2]


//! [3]
    su -c "make install"
//! [3]


//! [4]
    PATH               - to locate qmake, moc and other Qt tools
//! [4]


//! [5]
    PATH=/usr/local/Trolltech/Qt-%VERSION%/bin:$PATH
    export PATH
//! [5]


//! [6]
    setenv PATH /usr/local/Trolltech/Qt-%VERSION%/bin:$PATH
//! [6]


//! [7]
    PATH               - to locate qmake, moc and other Qt tools
//! [7]


//! [8]
    C:
    cd \Qt\%VERSION%
    configure
//! [8]


//! [9]
    configure -platform win32-msvc
//! [9]


//! [10]
    nmake
//! [10]


//! [11]
    cd /tmp
    gunzip qt-mac-opensource-desktop-%VERSION%.tar.gz        # uncompress the archive
    tar xvf qt-mac-opensource-desktop-%VERSION%.tar          # unpack it
//! [11]


//! [12]
    cd /tmp/qt-mac-opensource-desktop-%VERSION%
    ./configure
//! [12]


//! [13]
    make
//! [13]


//! [14]
    sudo make install
//! [14]


//! [15]
    PATH               - to locate qmake, moc and other Qt tools
//! [15]


//! [16]
    PATH=/usr/local/Trolltech/Qt-%VERSION%/bin:$PATH
    export PATH
//! [16]


//! [17]
    setenv PATH /usr/local/Trolltech/Qt-%VERSION%/bin:$PATH
//! [17]


//! [18]
        PATH               - to locate qmake, moc and other Qt tools
//! [18]


//! [19]
        C:
        cd \Qt\%VERSION%
        configure -platform win32-msvc2005 -xplatform wincewm50pocket-msvc2005
//! [19]


//! [20]
        set INCLUDE=C:\Program Files\Microsoft Visual Studio 8\VC\ce\include;C:\Program Files\Windows CE Tools\wce500\Windows Mobile 5.0 Pocket PC SDK\Include\Armv4i
        set LIB=C:\Program Files\Microsoft Visual Studio 8\VC\ce\lib\armv4i;C:\Program Files\Windows CE Tools\wce500\Windows Mobile 5.0 Pocket PC SDK\Lib\ARMV4I
        set PATH=C:\Program Files\Microsoft Visual Studio 8\VC\ce\bin\x86_arm;%PATH%
//! [20]


//! [21]
        setcepaths wincewm50pocket-msvc2005
//! [21]


//! [22]
        nmake
//! [22]


</snip>
*/
import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class doc_src_installation {
    public static void main(String args[]) {
        QApplication.initialize(args);
//! [0]
    cd /tmp
    gunzip qt-x11-opensource-desktop-%VERSION%.tar.gz        # uncompress the archive
    tar xvf qt-x11-opensource-desktop-%VERSION%.tar          # unpack it
//! [0]


//! [1]
    cd /tmp/qt-x11-opensource-desktop-%VERSION%
    ./configure
//! [1]


//! [2]
    make
//! [2]


//! [3]
    su -c "make install"
//! [3]


//! [4]
    PATH               - to locate qmake, moc and other Qt tools
//! [4]


//! [5]
    PATH=/usr/local/Trolltech/Qt-%VERSION%/bin:$PATH
    export PATH
//! [5]


//! [6]
    setenv PATH /usr/local/Trolltech/Qt-%VERSION%/bin:$PATH
//! [6]


//! [7]
    PATH               - to locate qmake, moc and other Qt tools
//! [7]


//! [8]
    C:
    cd \Qt\%VERSION%
    configure
//! [8]


//! [9]
    configure -platform win32-msvc
//! [9]


//! [10]
    nmake
//! [10]


//! [11]
    cd /tmp
    gunzip qt-mac-opensource-desktop-%VERSION%.tar.gz        # uncompress the archive
    tar xvf qt-mac-opensource-desktop-%VERSION%.tar          # unpack it
//! [11]


//! [12]
    cd /tmp/qt-mac-opensource-desktop-%VERSION%
    ./configure
//! [12]


//! [13]
    make
//! [13]


//! [14]
    sudo make install
//! [14]


//! [15]
    PATH               - to locate qmake, moc and other Qt tools
//! [15]


//! [16]
    PATH=/usr/local/Trolltech/Qt-%VERSION%/bin:$PATH
    export PATH
//! [16]


//! [17]
    setenv PATH /usr/local/Trolltech/Qt-%VERSION%/bin:$PATH
//! [17]


//! [18]
        PATH               - to locate qmake, moc and other Qt tools
//! [18]


//! [19]
        C:
        cd \Qt\%VERSION%
        configure -platform win32-msvc2005 -xplatform wincewm50pocket-msvc2005
//! [19]


//! [20]
        set INCLUDE=C:\Program Files\Microsoft Visual Studio 8\VC\ce\include;C:\Program Files\Windows CE Tools\wce500\Windows Mobile 5.0 Pocket PC SDK\Include\Armv4i
        set LIB=C:\Program Files\Microsoft Visual Studio 8\VC\ce\lib\armv4i;C:\Program Files\Windows CE Tools\wce500\Windows Mobile 5.0 Pocket PC SDK\Lib\ARMV4I
        set PATH=C:\Program Files\Microsoft Visual Studio 8\VC\ce\bin\x86_arm;%PATH%
//! [20]


//! [21]
        setcepaths wincewm50pocket-msvc2005
//! [21]


//! [22]
        nmake
//! [22]


    }
}
