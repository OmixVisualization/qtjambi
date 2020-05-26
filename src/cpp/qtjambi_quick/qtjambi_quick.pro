QTJAMBILIB = QtJambiQuick
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core gui qml quick

SOURCES += \
    qtjambi_qquick.cpp
