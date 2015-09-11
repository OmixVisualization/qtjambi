#
#

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release
#} else {
#    contains(QT_CONFIG, debug) {
#        CONFIG += debug
#    }
#    contains(QT_CONFIG, release) {
#        CONFIG += release
#    }
}

QT             += xml opengl
CONFIG         += qt warn_on

##############################################################
# OPTION 1 (default)
# To have the example working out the box as per the README
#  use this section and comment out OPTION 2.
TEMPLATE        = lib
#CONFIG         += dll
#CONFIG         += staticlib
CONFIG         += static_and_shared
TARGET          = testGenImpl
# These option changes are recommended since at least: win32-msvc2005
win32-msvc* {
    CONFIG += embed_manifest_dll force_embed_manifest
}
win32:CONFIG += precompile_header

CONFIG(debug, debug|release) {
    TARGET = $$member(TARGET, 0)_debuglib
}
#
# Look for a DLL "testGenImpl.{dll,so}"
##############################################################
# OPTIONS 2
# To test the original non-Java native exe version comment
#  out the block above and uncomment this section and comment
#  out OPTION 1.
#TARGET          = testGenerator
#
# Look for a binary "testGenerator"
##############################################################

#DESTDIR         = out
#MOC_DIR         = tmp
#OBJECTS_DIR     = tmp

HEADERS        += *.h
SOURCES        += *.cpp

INCLUDEPATH    += $$PWD
