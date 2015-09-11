import org.qtjambi.qt.core.*;

class fooBarWaitCondition {

    QMutex mutex;
    QWaitCondition keyPressed;

    public void do_something(){}
    public void getchar(){}

    public void foobar1() {
//! [0]
        while (true) {
            mutex.lock();
            keyPressed.wait(mutex);
            do_something();
            mutex.unlock();
        }
//! [0]
    }

        public void foobar2() {
//! [1]
        while (true) {
            getchar();
            keyPressed.wakeAll();
        }
//! [1]
    }

        int count;
        public void foobar3() {
//! [2]
        while (true) {
            mutex.lock();
            keyPressed.wait(mutex);
            ++count;
            mutex.unlock();

            do_something();

            mutex.lock();
            --count;
            mutex.unlock();
        }
//! [2]
        }

        public void foobar4() {
//! [3]
        while (true) {
            getchar();

            mutex.lock();
            // Sleep until there are no busy worker threads
            while (count > 0) {
                mutex.unlock();
                try {
                    Thread.sleep(1);
                } catch (InterruptedException e) {
                    // handle exception
                }
                mutex.lock();
            }
            keyPressed.wakeAll();
            mutex.unlock();
        }
//! [3]
        }
}
