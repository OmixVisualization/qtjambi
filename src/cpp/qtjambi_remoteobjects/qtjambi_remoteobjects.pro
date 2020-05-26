QTJAMBILIB = QtJambiRemoteObjects
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT += remoteobjects remoteobjects-private

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
