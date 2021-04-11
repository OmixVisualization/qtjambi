QTJAMBILIB = QtJambiOpenGL
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core gui opengl

HEARDERS = qtjambi_opengl_qhashes.h

SOURCES += \
    qtjambi_opengl_util.cpp