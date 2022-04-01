QTJAMBILIB = QtJambiDataVisualization
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)
DESTDIR = ../../lib
DLLDESTDIR = ../../bin

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtNetwork.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core gui datavisualization

HEADERS += \
    qtjambi_datavis3d_hashes.h

SOURCES += \
    qtjambi_datavis3d.cpp

