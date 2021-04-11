QTJAMBILIB = QtJambiScript
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

msvc:QMAKE_CXXFLAGS += /bigobj

# libQtScript.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core script

HEADERS += \
    qtjambi_script_hashes.h
