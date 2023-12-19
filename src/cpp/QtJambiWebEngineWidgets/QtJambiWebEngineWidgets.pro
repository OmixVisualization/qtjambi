include(../QtJambi/configure.pri)

QT = core qml quick widgets webenginecore webenginewidgets

HEADERS += \
    hashes.h

SOURCES +=

lessThan(QT_MAJOR_VERSION, 6) : linux-g++* : {
    CONFIG+=use_gold_linker
}
