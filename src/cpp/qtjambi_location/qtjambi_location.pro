QTJAMBILIB = QtJambiLocation
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core gui location

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    qtjambi_location_hashes.h \
    qtjambi_location_repository.h

SOURCES += \
    qtjambi_location_repository.cpp
