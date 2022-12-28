include(../QtJambi/configure.pri)
TEMPLATE = app

DESTDIR = ../bin
QT = core

SOURCES = main.cpp

win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}

OTHER_FILES = icon.rc
win32:RC_FILE = icon.rc

macx:QMAKE_SONAME_PREFIX = @rpath
macx:QMAKE_RPATHDIR = @executable_path/. @executable_path/lib @executable_path/../lib @executable_path/../../lib @executable_path/Frameworks @executable_path/../Frameworks @executable_path/../../Frameworks
linux-g++*:{
    QMAKE_RPATHDIR = $ORIGIN/.
    QMAKE_RPATHDIR += $ORIGIN/lib
    QMAKE_RPATHDIR += $ORIGIN/../lib
}
