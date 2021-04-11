QTJAMBILIB = QtJambiSvg
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core svg

lessThan(VER_MAJ, 5): QT += widgets

HEADERS +=
