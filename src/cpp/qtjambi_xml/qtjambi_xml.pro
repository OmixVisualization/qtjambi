QTJAMBILIB = QtJambiXml
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

win32:CONFIG += precompile_header
PRECOMPILED_HEADER = qtjambi_xml_pch.h

# libQtXml.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core xml

HEADERS += \
    qtjambi_xml_hashes.h \
    qtjambi_xml_repository.h

SOURCES += \
    qtjambi_xml_repository.cpp
