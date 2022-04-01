QTJAMBILIB = QtJambiMultimedia
TARGET = $$QTJAMBILIB

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_include.pri)
exists($$QTJAMBI_BUILDDIR): include($$QTJAMBI_BUILDDIR/generator/out/cpp/$$QTJAMBILIB/generated.pri)

QT = core gui multimedia

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

lessThan(QT_MAJOR_VERSION, 6):{
    macx:{
        INCLUDEPATH += $$(QTDIR)/lib/QtMultimediaWidgets.framework/Headers/
    }
    INCLUDEPATH += $$(QTDIR)/include/QtMultimediaWidgets
}else{
    QMAKE_LIBS_WMF = \

}

HEADERS += \
    qtjambi_multimedia.h \
    qtjambi_multimedia_repository.h

SOURCES += \
    qtjambi_multimedia.cpp \
    qtjambi_multimedia_repository.cpp
