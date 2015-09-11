#ifndef MYCLASS3_H
#define MYCLASS3_H

#include <QObject>

//! [0]
class MyClass : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("Author", "Oscar Peterson")
    Q_CLASSINFO("Status", "Active")

public:
    MyClass(QObject *parent = 0);
    ~MyClass();
};
//! [0]

#endif
