import org.qtjambi.qt.gui.*;

    class readWriteLock {
//! [0]
        QReadWriteLock lock = new QReadWriteLock();

        public void readerFunction() {
            // ...
            lock.lockForRead();
            read_file();
            lock.unlock();
            // ...
        }

        public void writerFunction() {
            // ...
            lock.lockForWrite();
            write_file();
            lock.unlock();
            // ...
        }
//! [0]

        public void read_file(){} public void write_file(){}

        }

// QReadLocker/QWriteLocker snippets remvoed.

//! [1]
//! [1]

//! [2]
//! [2]


//! [3]
//! [3]


//! [4]
//! [4]
