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
    bindableowner.h \
    containers.h \
    containers_qt5.h \
    enums.h\
    abstractclass.h \
    destruction.h \
    global.h \
    injectedcode.h \
    interfaces.h \
    internal.h \
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
    bindableowner.cpp \
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
    multisignaltest.cpp \
    tulip.cpp \
    variants.cpp

win32 {
    PRECOMPILED_HEADER = global.h
    CONFIG += precompile_header
}
msvc:QMAKE_CXXFLAGS += /bigobj

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

linux-g++* | freebsd-g++* {
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
    CONFIG(debug, debug|release) {
        QMAKE_RPATHDIR = $ORIGIN/../debug/lib
    }else{
        QMAKE_RPATHDIR = $ORIGIN/../release/lib
    }
}

macx:{
    CONFIG(debug, debug|release) {
        QMAKE_RPATHDIR = @loader_path/../debug/lib
    }else{
        QMAKE_RPATHDIR = @loader_path/../release/lib
    }
}

contains(QT_CONFIG, qtjambi-widgets): QT += widgets
else:                                 DEFINES += QTJAMBI_NO_WIDGETS
contains(QT_CONFIG, qtjambi-sql):     QT += sql
else:                                 DEFINES += QTJAMBI_NO_SQL
contains(QT_CONFIG, qtjambi-xml):     QT += xml
else:                                 DEFINES += QTJAMBI_NO_XML
contains(QT_CONFIG, qtjambi-network): QT += network
else:                                 DEFINES += QTJAMBI_NO_NETWORK
contains(QT_CONFIG, qtjambi-qml):     QT += qml
else:                                 DEFINES += QTJAMBI_NO_QML
contains(QT_CONFIG, qtjambi-quick):   QT += quick
else:                                 DEFINES += QTJAMBI_NO_QUICK

CONFIG += warn_on
