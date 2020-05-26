QTJAMBILIB = QtJambi3DInput
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../../qtjambi/qtjambi_include.pri)
include(../$$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)
DESTDIR = ../../lib
DLLDESTDIR = ../../bin

QT += core gui qml quick 3dcore 3dinput 3drender

HEADERS += \
    qt3dinput.h

