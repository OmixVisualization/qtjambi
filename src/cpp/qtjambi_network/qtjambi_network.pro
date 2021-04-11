QTJAMBILIB = QtJambiNetwork
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

win32:CONFIG += precompile_header
PRECOMPILED_HEADER = qtjambi_network_pch.h

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtNetwork.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core network

HEADERS += \
    qtjambi_network.h \
    qtjambi_network_repository.h

SOURCES += \
    qtjambi_network_repository.cpp \
    qtjambi_network_util.cpp
