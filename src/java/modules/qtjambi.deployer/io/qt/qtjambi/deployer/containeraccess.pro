!android:{
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

VERSION = $$section(QT_VERSION, ., 0, 1).$$QTJAMBI_PATCH_VERSION

DEFINES += QT_NO_VERSION_TAGGING

CONFIG += plugin unversioned_libname skip_target_version_ext

TEMPLATE = lib
CONFIG(debug, debug|release) {
    win32:{
        TARGET = $$member(TARGET, 0)d
	    DESTDIR = ../../lib/plugins/containeraccess
	    DLLDESTDIR = ../../debug/plugins/containeraccess
    }else{
	    DESTDIR = ../../debug/plugins/containeraccess
    }
}else{
	win32*:{
	    DESTDIR = ../../lib/plugins/containeraccess
	    DLLDESTDIR = ../../release/plugins/containeraccess
	}else{
	    DESTDIR = ../../release/plugins/containeraccess
	}
}

msvc:QMAKE_CXXFLAGS += /bigobj
win32-g++* {
    QMAKE_CXXFLAGS += -Wa,-mbig-obj
    CONFIG(debug, debug|release) {
        QMAKE_CXXFLAGS += -O3
    }
}

QTJAMBIPATH = $$clean_path($$PWD/../../../../../../src/cpp)
INCLUDEPATH += $$QTJAMBIPATH
DEPENDPATH += $$QTJAMBIPATH

macx:{
	CONFIG+=no_default_rpath no_qt_rpath
    INCLUDEPATH += $$JAVA_HOME_TARGET/include
    INCLUDEPATH += $$JAVA_HOME_TARGET/include/darwin
    # this option is necessary to properly compile on mountain lion because of std::string to char* casts
    QMAKE_CXXFLAGS += -fpermissive
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

QT -= gui widgets

contains(QT_CONFIG, release):contains(QT_CONFIG, debug) {
    # Qt was configued with both debug and release libs
    CONFIG += debug_and_release build_all
}

win32*:{
        CONFIG(debug, debug|release) {
                QTJAMBI_LIB_NAME = QtJambid$$QT_MAJOR_VERSION
        }else{
                QTJAMBI_LIB_NAME = QtJambi$$QT_MAJOR_VERSION
        }
}else{
        CONFIG(debug, debug|release) {
                QTJAMBI_LIB_NAME = QtJambi_debug
        }else{
                QTJAMBI_LIB_NAME = QtJambi
        }
}

macx:{
    LIBS += ../../../qtjambi/lib/lib$$member(QTJAMBI_LIB_NAME, 0).jnilib
    QMAKE_SONAME_PREFIX = @rpath
    QMAKE_RPATHDIR = @loader_path/.
    contains(QT_CONFIG, x86):CONFIG += x86
    contains(QT_CONFIG, ppc):CONFIG += ppc
    contains(QT_CONFIG, x86_64):CONFIG += x86_64
    contains(QT_CONFIG, ppc64):CONFIG += ppc64
    contains(QT_CONFIG, arm64):CONFIG += arm64
    CONFIG -= precompile_header
    QMAKE_CXXFLAGS += -fpermissive
    QMAKE_CXXFLAGS += -Wc++14-extensions
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual

    QMAKE_RPATHDIR =  @loader_path/../../lib
    QMAKE_RPATHDIR += @loader_path/../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../lib
    QMAKE_RPATHDIR += @loader_path/../../../../../lib

    greaterThan(QT_MAJOR_VERSION, 5):{
        greaterThan(QT_MAJOR_VERSION, 6) | greaterThan(QT_MINOR_VERSION, 1):{
            QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
            DEFINES += QTJAMBI_APPLE_MULTI_ARCHS
        }
    }
}else{
    LIBS += -L../../../qtjambi/lib
    linux-g++*: QMAKE_RPATHDIR = $ORIGIN/.

    android:{
        armeabi-v7a: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_armeabi-v7a
        arm64-v8a: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_arm64-v8a
        x86: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_x86
        x86_64: LIBS += -l$$member(QTJAMBI_LIB_NAME, 0)_x86_64
    }else{
        LIBS += -l$$QTJAMBI_LIB_NAME
    }
}

linux-g++* | freebsd-g++* | win32-g++* {
    QMAKE_CXXFLAGS_WARN_OFF += -Wdollar-in-identifier-extension -Woverloaded-virtual
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-function
    QMAKE_LFLAGS_NOUNDEF   += -Wl,--no-undefined
    QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF
}

HEADERS += 
SOURCES += containeraccess.cpp
