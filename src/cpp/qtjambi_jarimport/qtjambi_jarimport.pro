QTJAMBILIB = jarimport
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

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
    QT -= gui widgets
    LIBS += $$PWD/../../../build/qmake-qtjambi/lib/lib$$member(QTJAMBI_QML_LIB_NAME, 0).jnilib
    QMAKE_EXTENSION_SHLIB = dylib
#    INCLUDEPATH += $$(QTDIR)/lib/QtCore.framework/Headers
    QMAKE_RPATHDIR =  @loader_path/../lib
    QMAKE_RPATHDIR += @loader_path/../../lib
    QMAKE_RPATHDIR += @loader_path/../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../../../../../lib
} else {
    linux-g++*:{
        QMAKE_RPATHDIR = $ORIGIN/../lib:$ORIGIN/../../lib:$ORIGIN/../../../lib:$ORIGIN/../../../../lib:$ORIGIN/../../../../../lib:$ORIGIN/../../../../../../lib:$ORIGIN/../../../../../../../lib:$ORIGIN/../../../../../../../../lib
    }
    QT -= gui widgets
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

DEFINES += QT_NO_VERSION_TAGGING

msvc:QMAKE_CXXFLAGS += /bigobj

INCLUDEPATH += $$(QTDIR)/include
INCLUDEPATH += $$(QTDIR)/include/QtCore
