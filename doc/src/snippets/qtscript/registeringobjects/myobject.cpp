#include "myobject.h"

MyObject::MyObject()
{
}

int MyObject::calculate(int value) const
{
    int total = 0;
    for (int i = 0; i <= value; ++i)
        total += i;
    return total;
}
