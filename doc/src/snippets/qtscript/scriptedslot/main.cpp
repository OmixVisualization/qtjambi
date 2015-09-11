#include <QApplication>
#include <QTextEdit>
#include <QTimer>
#include <QtScript>
#include "myobject.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QScriptEngine engine;
    QFile scriptFile(":/object.js");
    scriptFile.open(QFile::ReadOnly);
    engine.evaluate(scriptFile.readAll());
    scriptFile.close();

    QTextEdit editor;
    QTimer timer;
    QScriptValue constructor = engine.evaluate("Object");
    QScriptValueList arguments;
    arguments << engine.newQObject(&timer);
    arguments << engine.newQObject(&editor);
    QScriptValue object = constructor.construct(arguments);
    if (engine.hasUncaughtException()) {
        qDebug() << engine.uncaughtException().toString();
    }

    editor.show();
    timer.start(1000);

    return app.exec();
}
