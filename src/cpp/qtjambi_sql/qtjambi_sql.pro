QTJAMBILIB = QtJambiSql
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

# libQtSql.so.4.7.4 is only dependant on libQtCore.so.4 (ensures removal of 'Qt -= gui')
QT = core sql

macx:{
    INCLUDEPATH += $$[QT_INSTALL_LIBS]/QtWidgets.framework/Headers
    INCLUDEPATH += $$[QT_INSTALL_HEADERS]
}
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets

HEADERS += qtjambi_sql_hashes.h

SOURCES += QtJambiSqlRelationalDelegate.cpp
