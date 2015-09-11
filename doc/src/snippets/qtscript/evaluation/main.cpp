#include <QtScript>

int main(int argc, char *argv[])
{
//! [0]
    QScriptEngine engine;
    qDebug() << "the magic number is:" << engine.evaluate("1 + 2").toNumber();
//! [0]
    return 0;
}
