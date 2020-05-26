QTJAMBILIB = QtJambiQuickControls2
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core quickcontrols2
