QTJAMBILIB = QtJambi3DAnimation
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../../qtjambi/qtjambi_include.pri)
include(../$$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)
DESTDIR = ../../lib
DLLDESTDIR = ../../bin

QT += core gui qml 3dcore 3danimation

HEADERS += \
    qtjambi_3danimation_hashes.h

SOURCES +=
