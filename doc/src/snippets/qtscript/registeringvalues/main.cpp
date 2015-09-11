#include <QtScript>

int main(int argc, char *argv[])
{
    QScriptEngine engine;
//! [0]
    QScriptValue val(&engine, 123);
    engine.globalObject().setProperty("foo", val);
    qDebug() << "foo times two is:" << engine.evaluate("foo * 2").toNumber();
//! [0]
    return 0;
}

