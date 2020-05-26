QTJAMBILIB = QtJambiPositioning
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core positioning

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    qtjambi_positioning_hashes.h \
    qtjambi_positioning_repository.h

SOURCES += \
    qtjambi_positioning_repository.cpp
