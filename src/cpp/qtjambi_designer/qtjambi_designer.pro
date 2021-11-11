QTJAMBILIB = QtJambiDesigner
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT += widgets uiplugin designer

HEADERS +=  \
            designer.h \
            qtjambiresourcebrowser.h

SOURCES += \
    qtjambi_designer_util.cpp
