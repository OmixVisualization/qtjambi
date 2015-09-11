// mkdir build/bugreport
// javac -g -d build/bugreport  -cp qtjambi-4.7.4.jar -source 1.5 -target 1.5  src/java/bugreport/org/qtjambi/bugreport/bug134/Main.java
// jar -cvf $PWD/bug134.jar -C src/java/bugreport .
// jar -uvf $PWD/bug134.jar -C build/bugreport .
// java -Dorg.qtjambi.qt.debug=true -cp qtjambi-4.7.4.jar:build/qtjambi-linux64-gcc-4.7.4.jar:bug134.jar org/qtjambi/bugreport/bug134/Main

package org.qtjambi.bugreport.bug134;

import org.qtjambi.qt.gui.*;

public class Main {

    public static void main(final String[] args) {
// This one line fixes the issue
//QApplication app = 
//        new QApplication(args);
System.gc();
        new QMainWindow() {
            @Override
            protected void keyPressEvent(final QKeyEvent event) {
                System.gc();
            }
        }.show();

        QApplication.execStatic();
        QApplication.shutdown();
    }
}
