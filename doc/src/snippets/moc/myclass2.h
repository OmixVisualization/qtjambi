#ifndef MYCLASS2_H
#define MYCLASS2_H

#include <QObject>

#define MyClass MyClass2

//! [0]
class MyClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Priority priority READ priority WRITE setPriority)
    Q_ENUMS(Priority)

public:
    enum Priority { High, Low, VeryHigh, VeryLow };

    MyClass(QObject *parent = 0);
    ~MyClass();

    void setPriority(Priority priority);
    Priority priority() const;
};
//! [0]

#endif
