
INCLUDEPATH += $$PWD/../../src/cpp/qtjambi/
DEPENDPATH += $$PWD/../../src/cpp/qtjambi/
include(../../src/cpp/qtjambi/qtjambi_include.pri)

SOURCES += jambi.cpp

DESTDIR = ../../build
DLLDESTDIR =
TEMPLATE = app

TARGET = jambi

CONFIG(debug, debug | release) {
    TARGET = jambi_debug
}

mac:CONFIG-=app_bundle

#CONFIG += console
