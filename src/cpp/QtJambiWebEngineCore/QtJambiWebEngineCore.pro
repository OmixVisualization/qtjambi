include(../QtJambi/configure.pri)

QT += core qml webenginecore

HEADERS += \
    hashes.h

lessThan(QT_MAJOR_VERSION, 6) : linux-g++* : {
    CONFIG+=use_gold_linker
}
