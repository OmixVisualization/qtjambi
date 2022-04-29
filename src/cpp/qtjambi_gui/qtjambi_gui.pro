QTJAMBILIB = QtJambiGui
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION


SOURCES += \
    qtjambi_gui_repository.cpp \
    qtjambi_gui_util.cpp

HEADERS += \
    qtjambi_gui.h \
    qtmatrixes.h \
    qtjambi_gui_repository.h \
    qtjambi_gui_qhashes.h

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

# because QActionEvent refers to qaction.h
macx:{
    INCLUDEPATH += $$(QTDIR)/lib/QtGui.framework/Headers/qpa
    INCLUDEPATH += $$(QTDIR)/lib/QtWidgets.framework/Headers
    INCLUDEPATH += $$(QTDIR)/include
}
INCLUDEPATH += $$(QTDIR)/include/QtGui/qpa
INCLUDEPATH += $$(QTDIR)/include/QtWidgets
INCLUDEPATH += $$(QTDIR)/include/QtPlatformHeaders
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtGui.so.4.7.4 is only dependant on libQtCore.so.4
QT = core core-private gui gui-private

win32-msvc*: {
    PRECOMPILED_HEADER = qtjambi_gui_pch.h
    CONFIG += precompile_header
    QMAKE_CXXFLAGS += /bigobj
}

win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

