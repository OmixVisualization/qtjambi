TARGET = org_qtjambi_qt_widgets

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION


SOURCES += \
    qguisignalmapper.cpp \
    qtjambiitemeditorcreator.cpp

HEADERS += \
    qguisignalmapper.h 

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_widgets/org_qtjambi_qt_widgets.pri)

# Some classes (such as QLine) need access to <qtjambi_core_qhashes.h>
INCLUDEPATH += $$PWD/../qtjambi_core

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtGui.so.4.7.4 is only dependant on libQtCore.so.4
QT = core gui widgets

win32:CONFIG += precompile_header
PRECOMPILED_HEADER = qtjambi_widgets_pch.h
