QTJAMBILIB = QtJambiWinExtras
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT += core gui widgets winextras

HEADERS += \
    qtjambi_winextras_repository.h

SOURCES += \
    qtjambi_winextras_repository.cpp
