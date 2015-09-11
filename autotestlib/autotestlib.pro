TARGET = org_qtjambi_autotests_generated

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

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
include(../build/tests/autotest-generator/cpp/org_qtjambi_autotests_generated/org_qtjambi_autotests_generated.pri)

INCLUDEPATH += ./cpp/org_qtjambi_autotests_generated
INCLUDEPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION/
INCLUDEPATH += $$(QTDIR)/include/QtGui/$$QT_VERSION/
INCLUDEPATH += $$(QTDIR)/include/QtWidgets/$$QT_VERSION/

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
    sharedpointertest.h


SOURCES += \
    destruction.cpp \
    global.cpp \
    injectedcode.cpp \
    testdialog.cpp \
    qtjambiunittesttools.cpp \
    memorymanagement.cpp \
    flagsandenumtest.cpp \
    propertyandmethodcalltest.cpp \
    sharedpointertest.cpp

win32 {
    PRECOMPILED_HEADER = global.h
    CONFIG += precompile_header
}

linux-g++* | freebsd-g++* {
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
}

QT += sql xml network widgets
CONFIG += warn_on
