TARGET = org_qtjambi_qt_help

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt_help/org_qtjambi_qt_help.pri)

# libQtHelp.so.4.7.4 is dependant on many, using modifiers
#  libQtCore.so.4 libQtGui.so.4 libQtNetwork.so.4 libQtSql.so.4 libQtXml.so.4 libQtCLucene.so.4

QT += help gui widgets core
