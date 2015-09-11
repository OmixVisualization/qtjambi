TARGET = org_qtjambi_qt_gui

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION


SOURCES += \
    qtreemodel.cpp \
#    qtjambi_platformspecificfunctions.cpp \
    qtmatrixes.cpp \

HEADERS += \
    qtmatrixes.h \
    qtreemodel.h \
    qtjambitextobjectinterface.h \
#    qtjambi_platformspecificfunctions.h \
    qtjambi_gui_qhashes.h

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_gui/org_qtjambi_qt_gui.pri)

# because qtjambishell_QActionEvent.cpp refers to qaction.h
INCLUDEPATH += $$(QTDIR)/include/QtWidgets
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtGui.so.4.7.4 is only dependant on libQtCore.so.4
QT = core gui

win32:CONFIG += precompile_header
PRECOMPILED_HEADER = qtjambi_gui_pch.h
