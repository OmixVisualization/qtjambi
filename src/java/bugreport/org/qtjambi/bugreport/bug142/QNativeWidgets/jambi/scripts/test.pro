
TARGET = qtjambi_nativewidgets

DESTDIR = ../target

INCLUDEPATH += $$PWD ../../src

HEADERS        += ./../../src/qnativewidgettest.h ./../../src/ui_qnativewidgettest.h
SOURCES        += ./../../src/qnativewidgettest.cpp 

include($(JAMBIDIR)/src/cpp/qtjambi/qtjambi_include.pri)
include(../cpp/qtjambi_nativewidgets/qtjambi_nativewidgets.pri)

