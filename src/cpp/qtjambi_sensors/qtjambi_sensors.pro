QTJAMBILIB = QtJambiSensors
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core sensors

HEADERS += \
    qtjambi_sensors_repository.h

SOURCES += \
    qtjambi_sensors_repository.cpp
