QTJAMBIAPP = QtJambiLauncher
TARGET = $$QTJAMBIAPP

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

include(../qtjambi/qtjambi_base.pri)

QTJAMBI_BUILDDIR = $$PWD/../../../$$VERSION/build/

TEMPLATE = app
#CONFIG += console
DESTDIR = ../bin
QT = core

SOURCES += \
        main.cpp

win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}

OTHER_FILES = icon.rc
win32:RC_FILE = icon.rc

macx:QMAKE_SONAME_PREFIX = @rpath
macx:QMAKE_RPATHDIR = @executable_path/. @executable_path/lib @executable_path/../lib @executable_path/../../lib @executable_path/Frameworks @executable_path/../Frameworks @executable_path/../../Frameworks
linux-g++*:QMAKE_RPATHDIR = $ORIGIN/.:$ORIGIN/lib:$ORIGIN/../lib
