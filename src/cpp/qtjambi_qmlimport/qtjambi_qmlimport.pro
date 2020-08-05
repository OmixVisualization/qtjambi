QTJAMBILIB = jarimport
TARGET = $$QTJAMBILIB

#VERSION = $$QT_VERSION

CONFIG += skip_target_version_ext

include(../qtjambi/qtjambi_base.pri)

win32*:{
        CONFIG(debug, debug|release) {
            QTJAMBI_QML_LIB_NAME = QtJambiQmld$$QT_MAJOR_VERSION
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

HEADERS += 
SOURCES += lib.cpp

DEFINES += QTJAMBI_QML_EXPORT
DEFINES += QT_NO_VERSION_TAGGING

QT -= core gui widgets

msvc:QMAKE_CXXFLAGS += /bigobj

macx:{
    INCLUDEPATH += $$(QTDIR)/lib/QtCore.framework/Headers
}
INCLUDEPATH += $$(QTDIR)/include
INCLUDEPATH += $$(QTDIR)/include/QtCore
