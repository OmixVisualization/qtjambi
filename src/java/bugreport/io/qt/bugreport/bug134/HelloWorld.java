// mkdir build/bugreport
// javac -g -d build/bugreport  -cp qtjambi-4.7.4.jar -source 1.5 -target 1.5  src/java/bugreport/org/qtjambi/bugreport/bug134/HelloWorld.java
// jar -cvf $PWD/bug134.jar -C src/java/bugreport .
// jar -uvf $PWD/bug134.jar -C build/bugreport .
// java -Dio.qt.debug=true -cp qtjambi-4.7.4.jar:build/qtjambi-linux64-gcc-4.7.4.jar:bug134.jar org/qtjambi/bugreport/bug134/HelloWorld

package io.qt.bugreport.bug134;

import io.qt.core.Qt;
import io.qt.core.QTimer;
import io.qt.gui.*;

public class HelloWorld extends QWidget
{
    private int counter = 0;

    public static void main(String args[])
    {
        QApplication.initialize(args);

        QTimer timer = new QTimer();
        timer.setInterval(1000);
//       WeakReference<HelloWorld> hw = new HelloWorld();
//        new HelloWorld().connect(timer, "timeout()", null, "timeout()");
        timer.timeout.connect((Object)new HelloWorld(), "timeout()");
        timer.start();

        QApplication.execStatic();
        QApplication.shutdown();
    }

    public HelloWorld()
    {
        show();
    }

    protected void timeout()
    {
        System.out.println("timeout() counter="+counter);
        System.out.flush();
        counter++;
        if(counter > 4)
            System.gc();
    }

    protected void keyPressEvent(QKeyEvent arg__1)
    {
        int key = arg__1.key();
        if(key == Qt.Key.Key_B.value())
            System.gc();
        disposeLater();
        new HelloWorld();
        if(key == Qt.Key.Key_G.value())
            System.gc();
    }
}

