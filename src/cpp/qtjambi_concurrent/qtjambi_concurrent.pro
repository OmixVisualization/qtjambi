QTJAMBILIB = QtJambiConcurrent
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

HEADERS += qtjambiconcurrent_p.h \
    qtjambi_concurrent_repository.h
SOURCES += qtjambiconcurrent.cpp \
    qtjambi_concurrent_repository.cpp

QT = core concurrent
