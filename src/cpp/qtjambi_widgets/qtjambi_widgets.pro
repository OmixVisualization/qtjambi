QTJAMBILIB = QtJambiWidgets
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION


SOURCES += \
    qtjambi_widgets_repository.cpp \
    qtjambiitemeditorcreator.cpp \
    qwidgetutils.cpp \
    qtjambi_widgets_core.cpp

HEADERS += \
    qtjambi_widgets_core.h \
    qtjambi_widgets_repository.h \
    qwidgetutils.h

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

win32-msvc*: {
    PRECOMPILED_HEADER = qtjambi_widgets_pch.h
    CONFIG += precompile_header
}

# Some classes (such as QLine) need access to <qtjambi_core_qhashes.h>
INCLUDEPATH += $$PWD/../qtjambi_core $$PWD/../qtjambi_gui

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtGui.so.4.7.4 is only dependant on libQtCore.so.4
QT = core core-private gui widgets widgets-private
