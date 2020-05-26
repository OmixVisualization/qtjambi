QTJAMBILIB = QtJambiWidgets
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION


SOURCES += \
    qtjambi_widgets_repository.cpp \
    qtjambiitemeditorcreator.cpp \
    qwidgetutils.cpp \
    signalmapper.cpp \
    qtjambi_widgets_core.cpp

HEADERS += \
    qtjambi_widgets_core.h \
    qtjambi_widgets_repository.h \
    qwidgetutils.h

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

# Some classes (such as QLine) need access to <qtjambi_core_qhashes.h>
INCLUDEPATH += $$PWD/../qtjambi_core $$PWD/../qtjambi_gui

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtGui.so.4.7.4 is only dependant on libQtCore.so.4
QT = core gui widgets

win32:CONFIG += precompile_header
PRECOMPILED_HEADER = qtjambi_widgets_pch.h
