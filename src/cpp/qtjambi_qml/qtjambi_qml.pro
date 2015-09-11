TARGET = org_qtjambi_qt_qml

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include ($$QTJAMBI_CPP/org_qtjambi_qt_qml/org_qtjambi_qt_qml.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += org_qtjambi_qt_qml_QQmlListProperty.h qmlregistry.h qqmllistproperty.h qtjambishell_QQmlListProperty.h
SOURCES += qmlregistry.cpp qqmllistproperty.cpp qtjambishell_QQmlListProperty.cpp

QT += core gui qml
