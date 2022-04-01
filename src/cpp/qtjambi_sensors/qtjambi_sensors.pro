QTJAMBILIB = QtJambiSensors
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT = core sensors

HEADERS += \
    qtjambi_sensors_repository.h

SOURCES += \
    qtjambi_sensors_repository.cpp
