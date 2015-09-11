// mkdir build/bugreport
// javac -g -d build/bugreport  -cp qtjambi-4.7.4*.jar -source 1.5 -target 1.5  src/java/bugreport/org/qtjambi/bugreport/bug215/Main.java
// jar -cvf $PWD/bug215.jar -C src/java/bugreport .
// jar -uvf $PWD/bug215.jar -C build/bugreport .
// java -Dorg.qtjambi.qt.debug=true -cp qtjambi-4.7.4*.jar:build/qtjambi-linux64-gcc-4.7.4.jar:bug215.jar org/qtjambi/bugreport/bug215/Main

// The UpdatablePicture class is reported to crash in QPixmap.fromImage() on windows
// On linux this application reliabily crashes on exit (using close window button)
// These two crases may not be related.

package org.qtjambi.bugreport.bug215;

import java.io.File;
import java.io.InputStream;
import java.io.FileOutputStream;

import org.qtjambi.qt.gui.*;

public class Main {

    public static void main(final String[] args) {
        File file = null;
        try {
            QApplication.initialize(args);

            QMainWindow mainWindow = new QMainWindow();

            String resName = Main.class.getPackage().getName().replaceAll("\\.", "/");
            resName += "/logo.png";
            {
                InputStream inStream = null;
                FileOutputStream fileOut = null;
                try {
                    // unload resource and save as a tempfile
                    file = File.createTempFile("logo.png", "tmp");
                    System.out.println("resource="+resName);
                    inStream = Main.class.getClassLoader().getResourceAsStream(resName);
                    fileOut = new FileOutputStream(file);
                    byte[] bA = new byte[4096];
                    int n;
                    while((n = inStream.read(bA)) > 0) {
                        fileOut.write(bA, 0, n);
                    }
                    fileOut.close();
                    fileOut = null;
                    inStream.close();
                    inStream = null;
                } catch(Exception e) {
                    e.printStackTrace();
                } finally {
                    if(fileOut != null) {
                        try {
                            fileOut.close();
                        } catch(Exception e) {
                            e.printStackTrace();
                        }
                    }
                    if(inStream != null) {
                        try {
                            inStream.close();
                        } catch(Exception e) {
                            e.printStackTrace();
                        }
                        inStream = null;
                    }
                }
            }

            if(file != null) {
                System.out.println("unloaded " + resName + " to " + file.getAbsolutePath() + "; length=" + file.length());
                System.out.flush();
            }

            UpdatablePicture updatablePicture = new UpdatablePicture(file);
            mainWindow.setCentralWidget(updatablePicture);

            mainWindow.show();

            QApplication.execStatic();
            QApplication.shutdown();

        } finally {
            // delete tempfile
            if(file != null) {
                try {
                    file.delete();
                } catch(Exception e) {
                    e.printStackTrace();
                }
                file = null;
            }
        }
    }
}
