TEMPLATE = lib

TARGET = QtJambiGenerator

DESTDIR = ../lib
DLLDESTDIR = ../bin

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

isEmpty(QTJAMBI_PATCH_VERSION):{
    error("Please set QTJAMBI_PATCH_VERSION");
}
VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION
VER_MAJ = $$section(VERSION, ., 0, 0)
VER_MIN = $$section(VERSION, ., 1, 1)
VER_PAT = $$section(VERSION, ., 2, 2)
DEFINES+=QTJAMBI_PATCH_VERSION=$$QTJAMBI_PATCH_VERSION

CONFIG(debug, debug|release) {
    win32:{
        TARGET = $$member(TARGET, 0)d
    }else{
        macx:{
            TARGET = $$member(TARGET, 0)_debug
        }else{
            TARGET = $$member(TARGET, 0)_debug
        }
    }
}

INCLUDEPATH += $$PWD

DEPENDPATH += $$PWD tests parser
mac:CONFIG -= app_bundle

INCLUDEPATH += $$PWD/. $$PWD/typesystem

RESOURCES += generator.qrc

include(parser/rxx.pri)

include(parser/rpp/rpp.pri)

HEADERS += \
        cppgenerator.h \
        cppheadergenerator.h \
        cppimplgenerator.h \
        debuglog.h \
        javagenerator.h \
        metainfogenerator.h \

SOURCES += \
        cppgenerator.cpp \
        cppheadergenerator.cpp \
        cppimplgenerator.cpp \
        javagenerator.cpp \
        metainfogenerator.cpp

# Input
HEADERS += \
        abstractgenerator.h \
        bufferedoutputstream.h \
        export.h \
        generator.h \
        metabuilder.h \
        metalang.h \
        preprocesshandler.h \
        reporthandler.h \
        typeparser.h \
        typesystem/utils.h \
        typesystem/codesnip.h \
        typesystem/modification.h \
        typesystem/typedatabase.h \
        typesystem/typeentry.h \
        asttoxml.h \
        prigenerator.h \
        util.h \
        docindex/docindexreader.h \
        docindex/docclass.h \
        docindex/docelement.h \
        docindex/docenum.h \
        docindex/docfunction.h \
        docindex/docmodel.h \
        docindex/docvariable.h \
        docindex/docproperty.h \
        docindex/docnamespace.h \
        docindex/doctypedef.h \
        typesystem/qmltypesystemreader.h \
        typesystem/xmltypesystemreader.h \
        typesystem/xmltoqmlconverter.h \
        typesystem/qml_abstractobject.h \
        typesystem/qml_typesystem.h \
        typesystem/qml_include.h \
        typesystem/qml_injectcode.h \
        typesystem/qml_loadtypesystem.h \
        typesystem/qml_modify.h \
        typesystem/qml_suppressedwarning.h \
        typesystem/qml_template.h \
        typesystem/qml_types.h

SOURCES += \
        abstractgenerator.cpp \
        bufferedoutputstream.cpp \
        generator.cpp \
        metabuilder.cpp \
        metalang.cpp \
        preprocesshandler.cpp \
        reporthandler.cpp \
        typeparser.cpp \
        typesystem/codesnip.cpp \
        typesystem/qmltypesystemreader.cpp \
        typesystem/xmltypesystemreader.cpp \
        typesystem/xmltoqmlconverter.cpp \
        typesystem/modification.cpp \
        #typesystem/stackelement.cpp \
        typesystem/typedatabase.cpp \
        typesystem/typeentry.cpp \
        typesystem/utils.cpp \
        #typesystem/
        asttoxml.cpp \
        prigenerator.cpp \
        util.cpp \
        docindex/docindexreader.cpp \
        docindex/docclass.cpp \
        docindex/docelement.cpp \
        docindex/docenum.cpp \
        docindex/docfunction.cpp \
        docindex/docmodel.cpp \
        docindex/docvariable.cpp \
        docindex/docproperty.cpp \
        docindex/docnamespace.cpp \
        docindex/doctypedef.cpp \
        typesystem/qml_abstractobject.cpp \
        typesystem/qml_typesystem.cpp \
        typesystem/qml_include.cpp \
        typesystem/qml_loadtypesystem.cpp \
        typesystem/qml_suppressedwarning.cpp \
        typesystem/qml_injectcode.cpp \
        typesystem/qml_modify.cpp \
        typesystem/qml_template.cpp \
        typesystem/qml_types.cpp



QT = core xml network concurrent qml
CONFIG += qmltypes

QML_IMPORT_NAME = QtJambiGenerator
QML_IMPORT_MAJOR_VERSION = 1

# These option changes are recommended since at least: win32-msvc.net
win32-arm64-msvc* | win32-msvc* {
#    QMAKE_CXXFLAGS += -Zm500
 #   QMAKE_CXXFLAGS -= -Zm200
  #  QMAKE_CFLAGS -= -Zm200
    CONFIG += embed_manifest_dll force_embed_manifest
}

macx:{
    QMAKE_TARGET_BUNDLE_PREFIX = io.qtjambi
    QMAKE_SONAME_PREFIX = @rpath
    CONFIG += no_default_rpath no_qt_rpath
    if($$QTJAMBI_GENERATE_FRAMEWORKS): CONFIG+=lib_bundle
    !isEmpty(QTJAMBI_EXTENSION_SHLIB): QMAKE_EXTENSION_SHLIB = $$QTJAMBI_EXTENSION_SHLIB
    QMAKE_CXXFLAGS += -fpermissive
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
    greaterThan(QT_MAJOR_VERSION, 5):{
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
        }
    }
}

linux-g++* | freebsd-g++* | win32-g++* {
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
}

contains(QMAKE_CXX, "g++") | contains(QMAKE_CXX, "clang++") : {
    SOURCES_BASE = $$clean_path($$dirname(_PRO_FILE_))
    QMAKE_CXXFLAGS_DEBUG += -g -fdebug-prefix-map=$$SOURCES_BASE=../sources/QtJambiGenerator
    QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO += -g -fdebug-prefix-map=$$SOURCES_BASE=../sources/QtJambiGenerator
}

linux-g++* : lessThan(QT_MAJOR_VERSION, 6):{
    QMAKE_CXXFLAGS += -fno-sized-deallocation
}

JAVA_HOME_TARGET = $$(JAVA_HOME_TARGET)
isEmpty(JAVA_HOME_TARGET):{
    JAVA_HOME_TARGET = $$(JAVA_HOME)
}
isEmpty(JAVA_HOME_TARGET):{
    error("Please set your JAVA_HOME_TARGET or JAVA_HOME environment variable to point to the directory of your Java SDK. Current JAVA_HOME_TARGET: $$(JAVA_HOME_TARGET) JAVA_HOME: $$(JAVA_HOME)")
}else{
    !exists($$(JAVA_HOME_TARGET)) {
        # Ant/Maven should set this up (this can't be a symlink)
        exists($$(JAVA_HOME)) {
            JAVA_HOME_TARGET = $$(JAVA_HOME)
        } else {
            error("Please set your JAVA_HOME_TARGET or JAVA_HOME environment variable to point to the directory of your Java SDK. Current JAVA_HOME_TARGET: $$(JAVA_HOME_TARGET) JAVA_HOME: $$(JAVA_HOME)")
        }
    } else {
        # This has the effect of making $$JAVA_HOME_TARGET work in this file as used below
        JAVA_HOME_TARGET = $$(JAVA_HOME_TARGET)
    }
}

macx | ios:{
    INCLUDEPATH += $$JAVA_HOME_TARGET/include
    INCLUDEPATH += $$JAVA_HOME_TARGET/include/darwin
} else {
    !android:INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include)
    win32 {
        INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include/win32)
    }
    solaris-g++ | solaris-cc {
        INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include/solaris)
    }
    linux-g++* {
        INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include/linux)
    }
    freebsd-g++* {
        INCLUDEPATH += $$quote($$JAVA_HOME_TARGET/include/freebsd)
    }
}
