#include <QThread>

//! [0]
class MyThread : public QThread
{
    Q_OBJECT

protected:
    void run();
};
//! [0]
