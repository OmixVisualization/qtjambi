QTJAMBILIB = QtJambiSql
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

# libQtSql.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core sql

macx:{
    INCLUDEPATH += $$(QTDIR)/lib/QtWidgets.framework/Headers
    INCLUDEPATH += $$(QTDIR)/include
}
INCLUDEPATH += $$(QTDIR)/include/QtWidgets

HEADERS += qtjambi_sql_hashes.h

SOURCES += QtJambiSqlRelationalDelegate.cpp
