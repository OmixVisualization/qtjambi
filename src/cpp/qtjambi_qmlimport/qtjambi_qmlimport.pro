QTJAMBILIB = jarimport
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

CONFIG += skip_target_version_ext

include(../qtjambi/qtjambi_include.pri)

HEADERS += \
    jarimport.h
SOURCES += \
    jarimport.cpp

QT += core qml

msvc:QMAKE_CXXFLAGS += /bigobj

macx:{
    QMAKE_EXTENSION_SHLIB = dylib
}

win32*:{
        CONFIG(debug, debug|release) {
            QTJAMBI_QML_LIB_NAME = QtJambiQml_debug$$QT_MAJOR_VERSION
        }else{
            QTJAMBI_QML_LIB_NAME = QtJambiQml$$QT_MAJOR_VERSION
        }
}else{
        CONFIG(debug, debug|release) {
            QTJAMBI_QML_LIB_NAME = QtJambiQml_debug
        }else{
            QTJAMBI_QML_LIB_NAME = QtJambiQml
        }
}

macx:{
    LIBS += $$PWD/../../../build/qmake-qtjambi/lib/lib$$member(QTJAMBI_QML_LIB_NAME, 0).jnilib
} else {
    LIBS += -L$$PWD/../../../build/qmake-qtjambi/lib
    android:{
        armeabi-v7a: LIBS += -l$$member(QTJAMBI_QML_LIB_NAME, 0)_armeabi-v7a
        arm64-v8a: LIBS += -l$$member(QTJAMBI_QML_LIB_NAME, 0)_arm64-v8a
        x86: LIBS += -l$$member(QTJAMBI_QML_LIB_NAME, 0)_x86
        x86_64: LIBS += -l$$member(QTJAMBI_QML_LIB_NAME, 0)_x86_64
    }else{
        LIBS += -l$$QTJAMBI_QML_LIB_NAME
    }
}

TEMPLATE = lib

QT += core qml

CONFIG += c++11

HEADERS +=
SOURCES +=

