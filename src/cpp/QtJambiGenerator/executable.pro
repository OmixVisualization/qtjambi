# Input
TEMPLATE = app
TARGET = QtJambiGenerator
CONFIG += console

QT -= core gui

SOURCES  = main.cpp

DESTDIR = ../bin

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    win32-msvc* | !CONFIG(force_debug_info): {
        # Qt was configued with both debug and release libs
        CONFIG += debug_and_release build_all
    }else{
        # don't compile debug with forced debug info
        CONFIG += release
    }
}

CONFIG(release, debug|release): CONFIG(force_debug_info) {
    CONFIG += separate_debug_info
}

CONFIG(debug, debug|release) {
    win32:{
        TARGET = $$member(TARGET, 0)d
    }else{
        macx:{
            TARGET = $$member(TARGET, 0)_debug
        }else{
            TARGET = $$member(TARGET, 0)_debug
        }
    }
}

win32*:{
    CONFIG(debug, debug|release) {
        QTJAMBI_LIB_NAME = QtJambiGeneratord$$QT_MAJOR_VERSION
    }else{
        QTJAMBI_LIB_NAME = QtJambiGenerator$$QT_MAJOR_VERSION
    }
}else{
        CONFIG(debug, debug|release) {
        QTJAMBI_LIB_NAME = QtJambiGenerator_debug
    }else{
        QTJAMBI_LIB_NAME = QtJambiGenerator
    }
}

mac:{
    CONFIG -= app_bundle
    if($$QTJAMBI_GENERATE_FRAMEWORKS):{
        LIBS += -F../lib/
        LIBS += -framework $$QTJAMBI_LIB_NAME
    }else{
        equals(QTJAMBI_EXTENSION_SHLIB,dylib) | isEmpty(QTJAMBI_EXTENSION_SHLIB):{
            LIBS += -L../lib
            LIBS += -l$$QTJAMBI_LIB_NAME
        }else{
            QMAKE_EXTENSION_SHLIB = $$QTJAMBI_EXTENSION_SHLIB
            LIBS += ../lib/lib$$member(QTJAMBI_LIB_NAME, 0).$$QTJAMBI_EXTENSION_SHLIB
        }
    }
    QMAKE_SONAME_PREFIX = @rpath
    QMAKE_RPATHDIR = @executable_path/../lib
    greaterThan(QT_MAJOR_VERSION, 5):{
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
        }
    }
} else {
    LIBS += -L../lib
    LIBS += -l$$QTJAMBI_LIB_NAME
    linux-g++*: QMAKE_RPATHDIR = $ORIGIN/../lib
}
