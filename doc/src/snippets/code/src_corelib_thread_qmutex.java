import org.qtjambi.qt.*;
import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;
import org.qtjambi.qt.xml.*;
import org.qtjambi.qt.network.*;
import org.qtjambi.qt.sql.*;
import org.qtjambi.qt.svg.*;


public class src_corelib_thread_qmutex {
    public static void main(String args[]) {
        QApplication.initialize(args);

class FooBar1 {
//! [0]
        int number = 6;

        public void method1()
        {
            number *= 5;
            number /= 4;
        }

        public void method2()
        {
            number *= 3;
            number /= 2;
        }
//! [0]
    public void method3() {

//! [1]
        // method1()
        number *= 5;        // number is now 30
        number /= 4;        // number is now 7

        // method2()
        number *= 3;        // number is now 21
        number /= 2;        // number is now 10
//! [1]


//! [2]
        // Thread 1 calls method1()
        number *= 5;        // number is now 30

        // Thread 2 calls method2().
        //
        // Most likely Thread 1 has been put to sleep by the operating
        // system to allow Thread 2 to run.
        number *= 3;        // number is now 90
        number /= 2;        // number is now 45

        // Thread 1 finishes executing.
        number /= 4;        // number is now 11, instead of 10
//! [2]
} // method3
} // class FooBar
class FooBar2 {
//! [3]
        QMutex mutex = new QMutex();
        int number = 6;

        public void method1()
        {
            mutex.lock();
            number *= 5;
            number /= 4;
            mutex.unlock();
        }

        public void method2()
        {
            mutex.lock();
            number *= 3;
            number /= 2;
            mutex.unlock();
        }
//! [3]
} // class FooBar2

// QMutexLocker snippets removed:

//! [4]
//! [4]
//! [5]
//! [5]
//! [6]
//! [6]


    }
}
