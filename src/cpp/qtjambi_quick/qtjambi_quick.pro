QTJAMBILIB = QtJambiQuick
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core gui qml quick quick-private

SOURCES += \
    qtjambi_quick.cpp \
    qtjambi_quick_repository.cpp

HEADERS += \
    qtjambi_quick_hashes.h \
    qtjambi_quick_repository.h

greaterThan(QT_MAJOR_VERSION, 5):{
    OBJECTIVE_SOURCES += QSGMetalTexture.mm
}
