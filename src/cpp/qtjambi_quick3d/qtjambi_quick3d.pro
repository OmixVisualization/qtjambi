QTJAMBILIB = QtJambiQuick3D
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

DESTDIR = ../lib
DLLDESTDIR = ../bin
include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtNetwork.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core gui qml quick3d

HEADERS += qtjambi_quick3d_repository.h \
    qtjambi_quick3d_hashes.h

SOURCES += qtjambi_quick3d_repository.cpp

