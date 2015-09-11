#include <QObject>
#include <QtScript>
#include "myobject.h"

int main(int argc, char *argv[])
{
//! [0]
    QScriptEngine engine;
    QObject *someObject = new MyObject;
    QScriptValue objectValue = engine.newQObject(someObject);
    engine.globalObject().setProperty("myObject", objectValue);
//! [0]
    qDebug() << "myObject's calculate() function returns"
             << engine.evaluate("myObject.calculate(10)").toNumber();
    return 0;
}
