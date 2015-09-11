import org.qtjambi.qt.core.*;
import java.lang.reflect.*;

class MyClass {}

public class src_corelib_concurrent_qfuturewatcher {
    public void main() {
        Method method = null;
        int arg1 = 0, arg2 = 0;
//! [0]
    // Instantiate the objects and connect to the finished signal.
    MyClass myObject = new MyClass();
    QFutureWatcher<Integer> watcher = new QFutureWatcher<Integer>();
    watcher.finished.connect(myObject, "handleFinished()");

    // Start the computation.
    QFuture<Integer> future = QtConcurrent.run(this, method, arg1, arg2);
    watcher.setFuture(future);
//! [0]


    }
}
