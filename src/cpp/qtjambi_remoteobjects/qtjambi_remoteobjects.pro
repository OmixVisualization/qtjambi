QTJAMBILIB = QtJambiRemoteObjects
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT = core remoteobjects remoteobjects-private

msvc:QMAKE_CXXFLAGS += /bigobj

SOURCES += \
    qtjambi_remoteobjects.cpp \
    qtjambi_remoteojects_repository.cpp

HEADERS += \
    qtjambi_remoteobjects.h \
    qtjambi_remoteojects_repository.h
