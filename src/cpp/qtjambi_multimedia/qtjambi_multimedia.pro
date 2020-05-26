QTJAMBILIB = QtJambiMultimedia
TARGET = $$QTJAMBILIB

VERSION = $$QT_VERSION

include(../qtjambi/qtjambi_include.pri)
include($$QTJAMBI_CPP/$$QTJAMBILIB/generated.pri)

QT = core gui multimedia

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

macx:{
    INCLUDEPATH += $$(QTDIR)/lib/QtMultimediaWidgets.framework/Headers/
}
INCLUDEPATH += $$(QTDIR)/include/QtMultimediaWidgets

HEADERS += \
    qtjambi_multimedia.h \
    qtjambi_multimedia_repository.h

SOURCES += \
    qtjambi_multimedia.cpp \
    qtjambi_multimedia_repository.cpp
