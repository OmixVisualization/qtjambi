TARGET = org_qtjambi_qt_quick_widgets

greaterThan(QT_MAJOR_VERSION, 4): VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/org_qtjambi_qt_quick_widgets/org_qtjambi_qt_quick_widgets.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core gui widgets qml quick quickwidgets
