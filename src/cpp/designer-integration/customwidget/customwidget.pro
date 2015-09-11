TARGET=JambiCustomWidget

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include($$PWD/../../qtjambi/qtjambi_include.pri)

TEMPLATE = lib
QT += designer
CONFIG += plugin
DESTDIR=../../plugins/designer

HEADERS += jambicustomwidget.h
SOURCES += jambicustomwidget.cpp

INCLUDEPATH += $$PWD/../include

macx:QMAKE_EXTENSION_SHLIB = dylib
