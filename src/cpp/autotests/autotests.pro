QTJAMBI_LIB_PATH = ../../qtjambi/lib
DESTDIR = ../lib
DLLDESTDIR = ../bin

include(../QtJambi/configure.pri)

GENERATOR_PRI = $$clean_path($$PWD/../../../$$VERSION/build/tests/cpp/$$QTJAMBI_PROJECT/generated.pri)
exists($$GENERATOR_PRI): include($$GENERATOR_PRI)

QT = core gui concurrent widgets sql qml network xml quick

INCLUDEPATH += ../

HEADERS += \
    bindableowner.h \
    containers.h \
    containers_qt5.h \
    enums.h\
    abstractclass.h \
    destruction.h \
    exceptiontest.h \
    global.h \
    injectedcode.h \
    interfaces.h \
    internal.h \
    messagehandler.h \
    metaobjectqtmetacast.h \
    namespace.h \
    nativepointertester.h \
    paintengine.h \
    performancetests.h \
    signalsandslots.h \
    smartpointertest.h \
    testdialog.h \
    tulip.h \
    variants.h \
    general.h \
    memorymanagement.h \
    flagsandenumtest.h \
    propertyandmethodcalltest.h \
    settingstest.h \
    multisignaltest.h


SOURCES += \
    bindableowner.cpp \
    containers.cpp \
    destruction.cpp \
    exceptiontest.cpp \
    global.cpp \
    injectedcode.cpp \
    interfaces.cpp \
    performancetests.cpp \
    smartpointertest.cpp \
    testdialog.cpp \
    memorymanagement.cpp \
    flagsandenumtest.cpp \
    propertyandmethodcalltest.cpp \
    settingstest.cpp \
    multisignaltest.cpp \
    tulip.cpp \
    variants.cpp \
    debugtools.cpp
win32-msvc*: {
    PRECOMPILED_HEADER = global.h
    CONFIG += precompile_header
    QMAKE_CXXFLAGS += /bigobj
}

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

CONFIG += warn_on

RESOURCES += \
    resources.qrc
