QTJAMBILIB = QtJambiRemoteObjects
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core remoteobjects remoteobjects-private

msvc:QMAKE_CXXFLAGS += /bigobj

macx:{
    INCLUDEPATH += $$(QTDIR)/lib/QtRemoteObjects.framework/Headers/$$QT_VERSION/QtRemoteObjects/private
}
INCLUDEPATH += $$(QTDIR)/include/QtRemoteObjects/$$QT_VERSION/QtRemoteObjects/private

SOURCES += \
    qtjambi_remoteobjects.cpp \
    qtjambi_remoteojects_repository.cpp

HEADERS += \
    qtjambi_remoteobjects.h \
    qtjambi_remoteojects_repository.h
