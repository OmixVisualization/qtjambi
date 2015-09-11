
import org.qtjambi.qt.*;
import org.qtjambi.core.*;
import org.qtjambi.gui.*;


public class ThreadSnippets
{


    public ThreadSnippets()
    {

//![1]
        MyJambiRunnable jambiRunnable = new MyJambiRunnable();
        QThreadPool.start(jambiRunnable);
//![1]

//![2]
        MyJavaRunnable javaRunnable = new MyJavaRunnable();
        QThread runner = new QThread(javaRunnable);
        runner.start();
//![2]

    }

//![3]
    class MyJavaRunnable implements Runnable
    {
        @Override
        public void run()
        {
            // Do whatever
        }
    }
//![3]

//![0]
    class MyJambiRunnable extends QRunnable
    {
        @Override
        public void run()
        {
            // Do whatever
        }
    }
//![0]

//![4]
    class EventLoopThread implements Runnable
    {
        @Iverride
        public void run()
        {
            QEventLoop loop = new QEventLoop();
            loop.exec();

            // Do whatever
            
            loop.exit();
        }
    }

//![4]

}
