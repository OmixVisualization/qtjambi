import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;


public class src_corelib_concurrent_qthreadpool {
//! [0]
        static class HelloWorldTask extends QRunnable {
            @Override
            public void run()
            {
                System.out.println("Hello world from thread" + QThread.currentThread());
            }
        }

        public static void main(String args[]) {
            QCoreApplication.initialize(args);

            HelloWorldTask hello = new HelloWorldTask();
            // QThreadPool takes ownership and deletes 'hello' automatically
            QThreadPool.globalInstance().start(hello);
        }
//! [0]
}
