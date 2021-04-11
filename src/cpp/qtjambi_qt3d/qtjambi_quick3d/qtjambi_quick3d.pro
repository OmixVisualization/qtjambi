QTJAMBILIB = QtJambiQuick3D
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../../qtjambi/qtjambi_include.pri)
include(../$$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)
DESTDIR = ../../lib
DLLDESTDIR = ../../bin

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtNetwork.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core gui qml quick3d

HEADERS += qtjambi_quick3d_repository.h \
    qtjambi_quick3d_hashes.h

SOURCES += qtjambi_quick3d_repository.cpp

