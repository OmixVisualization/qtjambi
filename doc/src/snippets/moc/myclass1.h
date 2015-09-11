#ifndef MYCLASS1_H
#define MYCLASS1_H

#include <QObject>

#define MyClass MyClass1

//! [0]
class MyClass : public QObject
{
    Q_OBJECT

public:
    MyClass(QObject *parent = 0);
    ~MyClass();

signals:
    void mySignal();

public slots:
    void mySlot();
};
//! [0]

#endif
