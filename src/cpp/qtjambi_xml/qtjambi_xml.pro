QTJAMBILIB = QtJambiXml
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

# libQtXml.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core xml

HEADERS += \
    qtjambi_xml_hashes.h \
    qtjambi_xml_repository.h

SOURCES += \
    qtjambi_xml_repository.cpp

win32-msvc*: {
    PRECOMPILED_HEADER = qtjambi_xml_pch.h
    CONFIG += precompile_header
}
