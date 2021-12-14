QTJAMBILIB = QtJambiTest
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT += core gui widgets testlib

HEADERS += \
    qtjambi_test_util.h

SOURCES += \
    qtjambi_test.cpp
