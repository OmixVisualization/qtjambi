QTJAMBILIB = QtJambiNetwork
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

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

win32-msvc*: {
    PRECOMPILED_HEADER = qtjambi_network_pch.h
    CONFIG += precompile_header
}
