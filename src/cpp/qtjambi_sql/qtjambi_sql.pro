QTJAMBILIB = QtJambiSql
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

# libQtSql.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core sql

HEADERS += qtjambi_sql_hashes.h

SOURCES += qsqldrivercreator.cpp
