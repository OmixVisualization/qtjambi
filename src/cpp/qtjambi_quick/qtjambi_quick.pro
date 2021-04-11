QTJAMBILIB = QtJambiQuick
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core gui qml quick quick-private

SOURCES += \
    qtjambi_qquick.cpp\
    qtjambi_quick_repository.cpp

HEADERS += \
    qtjambi_quick_hashes.h\
    qtjambi_quick_repository.h
