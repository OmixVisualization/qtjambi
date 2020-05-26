QTJAMBILIB = QtJambiConcurrent
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

HEADERS += qtjambiconcurrent_p.h \
    qtjambi_concurrent_repository.h
SOURCES += qtjambiconcurrent.cpp \
    qtjambi_concurrent_repository.cpp

QT = core concurrent
