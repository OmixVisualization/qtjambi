QTJAMBILIB = QtJambi3DAnimation
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

DESTDIR = ../lib
DLLDESTDIR = ../bin
include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT = core 3danimation

HEADERS += \
    qtjambi_3danimation_hashes.h

SOURCES +=
