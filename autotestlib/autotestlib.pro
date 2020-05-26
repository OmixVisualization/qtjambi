TARGET = io_qt_autotests_generated

VERSION = $$QT_VERSION

# Messing with this config setting is what messes up building for Linux and debug_and_release for windows/macosx.
# So before reinstating this section explain why it is required and the purpose.
#contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
#    # Qt was configued with both debug and release libs
#    CONFIG += debug_and_release
#} else {
#    contains(QT_CONFIG, debug) {
#        CONFIG += debug
#    }
#    contains(QT_CONFIG, release) {
#        CONFIG += release
#    }
#}

include(../src/cpp/qtjambi/qtjambi_include.pri)
include(../build/tests/autotest-generator/cpp/io_qt_autotests_generated/generated.pri)

INCLUDEPATH += ./cpp/io_qt_autotests_generated

HEADERS += \
    abstractclass.h \
    destruction.h \
    global.h \
    injectedcode.h \
    interfaces.h \
    messagehandler.h \
	metaobjectqtmetacast.h \
    namespace.h \
    nativepointertester.h \
    paintengine.h \
    signalsandslots.h \
    testdialog.h \
    tulip.h \
    variants.h \
    general.h \
    memorymanagement.h \
    flagsandenumtest.h \
    propertyandmethodcalltest.h \
    sharedpointertest.h \
    settingstest.h \
    multisignaltest.h


SOURCES += \
    destruction.cpp \
    global.cpp \
    injectedcode.cpp \
    interfaces.cpp \
    testdialog.cpp \
    memorymanagement.cpp \
    flagsandenumtest.cpp \
    propertyandmethodcalltest.cpp \
    sharedpointertest.cpp \
    settingstest.cpp \
    multisignaltest.cpp

win32 {
    PRECOMPILED_HEADER = global.h
    CONFIG += precompile_header
}

linux-g++* | freebsd-g++* {
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
}

QT += sql xml network widgets qml quick
CONFIG += warn_on
