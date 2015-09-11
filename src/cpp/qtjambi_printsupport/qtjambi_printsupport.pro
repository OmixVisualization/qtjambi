TARGET = org_qtjambi_qt_printsupport

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_printsupport/org_qtjambi_qt_printsupport.pri)

QT += core gui widgets printsupport
