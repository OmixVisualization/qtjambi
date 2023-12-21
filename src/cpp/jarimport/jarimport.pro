QTJAMBI_PROJECT = $$basename(_PRO_FILE_)
QTJAMBI_PROJECT = $$section(QTJAMBI_PROJECT, .pro, 0, 0)
TARGET = $$QTJAMBI_PROJECT

isEmpty(QTJAMBI_PATCH_VERSION):{
    error("Please set QTJAMBI_PATCH_VERSION");
}
VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

CONFIG += plugin unversioned_libname skip_target_version_ext

TEMPLATE = lib
DESTDIR = ../lib
DLLDESTDIR = ../bin

CONFIG(debug, debug|release) {
    win32:{
        TARGET = $$member(TARGET, 0)d
    }else{
        TARGET = $$member(TARGET, 0)_debug
    }
}

QT -= gui widgets

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    win32-arm64-msvc* | win32-msvc* | !CONFIG(force_debug_info): {
        # Qt was configued with both debug and release libs
        CONFIG += debug_and_release build_all
    }else{
        # don't compile debug with forced debug info
        CONFIG += release
    }
}

CONFIG(release, debug|release): CONFIG(force_debug_info) {
    CONFIG += separate_debug_info
}

macx:{
    contains(QT_CONFIG, x86):CONFIG += x86
    contains(QT_CONFIG, ppc):CONFIG += ppc
    contains(QT_CONFIG, x86_64):CONFIG += x86_64
    contains(QT_CONFIG, ppc64):CONFIG += ppc64
    contains(QT_CONFIG, arm64):CONFIG += arm64
    CONFIG -= precompile_header
    QMAKE_CXXFLAGS += -fpermissive
    QMAKE_CXXFLAGS += -Wc++14-extensions
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
    QMAKE_EXTENSION_SHLIB = dylib

    CONFIG+=no_default_rpath no_qt_rpath
    QMAKE_RPATHDIR =  @loader_path/../lib
    QMAKE_RPATHDIR += @loader_path/../../lib
    QMAKE_RPATHDIR += @loader_path/../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../../../../../../../lib

    greaterThan(QT_MAJOR_VERSION, 5):{
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
            DEFINES += QTJAMBI_APPLE_MULTI_ARCHS
        }
    }
}

linux-g++*:{
    QMAKE_RPATHDIR = $ORIGIN/../lib
    QMAKE_RPATHDIR += $ORIGIN/../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../../../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../../../../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../../../../../../../lib
    QMAKE_RPATHDIR += $ORIGIN/../../../../../../../../../../../lib
}

linux-g++* | freebsd-g++* | win32-g++* {
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
}

HEADERS += 
SOURCES += lib.cpp

DEFINES += QT_NO_VERSION_TAGGING
