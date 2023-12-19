include(../QtJambi/configure.pri)

QT += core qml quick webenginecore webengine

lessThan(QT_MAJOR_VERSION, 6) : linux-g++* : {
    CONFIG+=use_gold_linker
}
