QTJAMBILIB = QtJambiSvg
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core svg

lessThan(QT_MAJOR_VERSION, 5): QT += widgets

HEADERS +=
