TARGET = org_qtjambi_qt_core

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_core/org_qtjambi_qt_core.pri)

HEADERS += qtjambiqfuture.h qtjambi_core_qhashes.h
SOURCES += qtjambiqfuture.cpp

DEPENDPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION
DEPENDPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION/QtCore
DEPENDPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION/QtCore/private
DEPENDPATH += $$PWD

INCLUDEPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION
INCLUDEPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION/QtCore
INCLUDEPATH += $$(QTDIR)/include/QtCore/$$QT_VERSION/QtCore/private
INCLUDEPATH += $$PWD

QT = core
