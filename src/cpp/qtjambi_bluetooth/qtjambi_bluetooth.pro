QTJAMBILIB = QtJambiBluetooth
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION
include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT = core bluetooth

HEADERS += \
    qtjambi_bluetooth_hashes.h \
    qtjambi_bluetooth_repository.h

SOURCES += \
    qtjambi_bluetooth_repository.cpp

