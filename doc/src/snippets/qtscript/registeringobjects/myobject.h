#ifndef MYOBJECT_H
#define MYOBJECT_H

#include <QObject>

class MyObject : public QObject
{
    Q_OBJECT

public:
    MyObject();

public slots:
    int calculate(int value) const;
};

#endif
