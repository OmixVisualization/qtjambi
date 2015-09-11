TARGET = org_qtjambi_qt_declarative

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt_declarative/org_qtjambi_qt_declarative.pri)

# libQtDeclarative.so.4.7.4 is dependant on many, using modifiers
#  libQtCore.so.4 libQtGui.so.4 libQtNetwork.so.4 libQtSql.so.4 libQtXmlPatterns.so.4 libQtSvg.so.4 libQtScript.so.4

HEADERS += qdeclarativelistproperty.h qtjambishell_QDeclarativeListProperty.h

SOURCES += qdeclarativelistproperty.cpp qtjambishell_QDeclarativeListProperty.cpp

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += network declarative script
#CONFIG += core gui widgets declarative
