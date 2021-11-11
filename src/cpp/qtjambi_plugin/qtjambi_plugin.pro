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

    greaterThan(QT_MAJOR_VERSION, 5):{
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
            DEFINES += QTJAMBI_APPLE_MULTI_ARCHS
        }
    }
}

HEADERS += 
SOURCES += plugin.cpp

DEFINES += QT_NO_VERSION_TAGGING

msvc:QMAKE_CXXFLAGS += /bigobj
