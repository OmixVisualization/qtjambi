QTJAMBILIB = qtjambiplugin
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

CONFIG += skip_target_version_ext

include(../qtjambi/qtjambi_include.pri)

QT -= gui widgets

macx:{
    QMAKE_EXTENSION_SHLIB = dylib
    QMAKE_RPATHDIR =  @loader_path/../../lib
    QMAKE_RPATHDIR += @loader_path/../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../lib
}

HEADERS += 
SOURCES += plugin.cpp

DEFINES += QT_NO_VERSION_TAGGING

msvc:QMAKE_CXXFLAGS += /bigobj
