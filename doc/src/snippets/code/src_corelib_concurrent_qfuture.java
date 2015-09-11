import org.qtjambi.qt.core.*;
import org.qtjambi.qt.gui.*;


public class src_corelib_concurrent_qfuture {
    public static void main(String args[]) {
        QApplication.initialize(args);
        /* const_iterator doesn't exist in Jambi
//! [0]
        QFuture<String> future = new QFuture<String>();

        QFutureIterator<String> i = new QFutureIterator<String>(future);
        while (i.hasNext())
            System.out.println(i.next());
//! [0]
     */

        {
//! [1]
        QFuture<String> future = new QFuture<String>();
        /* ... */

        QFutureIterator<String> i = new QFutureIterator<String>(future);
        while (i.hasNext())
            System.out.println(i.next());
//! [1]
        }


        {
        QFuture<String> future = new QFuture<String>();
//! [2]
        QFutureIterator<String> i = new QFutureIterator<String>(future);
        i.toBack();
        while (i.hasPrevious())
            System.out.println(i.previous());
//! [2]
        }


    }
}
