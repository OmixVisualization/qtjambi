QTJAMBILIB = QtJambiNfc
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core nfc

HEADERS += \
    qtjambi_nfc.h

SOURCES += \
    qtjambi_nfc.cpp
