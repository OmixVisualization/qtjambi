QTJAMBILIB = QtJambiPositioning
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT = core positioning

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    qtjambi_positioning_hashes.h \
    qtjambi_positioning_repository.h

SOURCES += \
    qtjambi_positioning_repository.cpp
