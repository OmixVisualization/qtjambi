TARGET = org_qtjambi_qt_network

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_network/org_qtjambi_qt_network.pri)

win32:CONFIG += precompile_header
PRECOMPILED_HEADER = qtjambi_network_pch.h

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# libQtNetwork.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core network

HEADERS += \
    qschemanetworkaccessmanager.h \
    qclasspathaccessmanager.h \
    qnetworkreplyclasspathimpl_p.h

SOURCES += \
    qschemanetworkaccessmanager.cpp \
    qclasspathaccessmanager.cpp
