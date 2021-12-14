QTJAMBILIB = QtJambiWinExtras
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT += core gui widgets winextras

HEADERS += \
    qtjambi_winextras_repository.h

SOURCES += \
    qtjambi_winextras_repository.cpp
