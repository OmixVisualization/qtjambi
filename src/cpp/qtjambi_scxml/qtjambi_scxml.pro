QTJAMBILIB = QtJambiScxml
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core scxml

greaterThan(QT_MAJOR_VERSION, 5):{
    lessThan(QT_MINOR_VERSION, 12):
        SOURCES += QScxmlDataModel_ForeachLoopBody_shell.cpp
}

SOURCES += \
    qtjambi_scxml_util.cpp
