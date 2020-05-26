QTJAMBILIB = QtJambiSerialBus
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT += core serialbus

HEADERS += \
    qtjambi_serialbus_qhashes.h\
    qtjambi_serialbus.h

SOURCES += qtjambi_serialbus.cpp
